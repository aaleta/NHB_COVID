#include "head.h"

void init()
{
  resetVariables();
  resetNodes();
  
  //Start
  seedInfection();
}

void seedInfection()
{
  int seed;

  seed = gsl_rng_uniform_int(random_gsl, N);
  index_node = seed;

  node[seed].state = L;
  if(gsl_rng_uniform(random_gsl)<p) //asymptomatic
    addToList(&latent_asymptomatic,seed);
  else
    addToList(&latent_symptomatic,seed);
  
  n_active = 1;

  t = 1;
}

void spread(int run)
{  
  resetNew();

  //S to L
  infection();
  //L to P
  latency();
  //P to I
  preToI();
  //IatoR
  IaToR();
  //IstoR
  IsTransition();
  //Home
  homeTransition();
  //Hospital dynamics
  hospitals();

  updateLists();
  updateTime();

  //Write data
  fprintf(f_data,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",latent_asymptomatic.n,latent_symptomatic.n,infectious_asymptomatic.n,pre_symptomatic.n,infectious_symptomatic.n,home.n,hospital.n,icu.n,recovered.n,new_latent_asymptomatic.n,new_latent_symptomatic.n,new_infectious_asymptomatic.n,new_pre_symptomatic.n,new_infectious_symptomatic.n,new_home.n,new_hospital.n,new_icu.n,new_recovered.n,run);
}

void infection()
{
  //Infectious asymptomatic
  for(int i=0;i<infectious_asymptomatic.n;i++)
    infect(infectious_asymptomatic.v[i],IA);

  //Pre-symptomatic
  for(int i=0;i<pre_symptomatic.n;i++)
    infect(pre_symptomatic.v[i],PS);
    
  //Infectious symptomatic
  for(int i=0;i<infectious_symptomatic.n;i++)
    infect(infectious_symptomatic.v[i],IS);
}

void infect(int source, int type)
{
  int target;
  double prob;

  for(int j=0;j<node[source].k;j++)
    {
      target = node[source].v[j];
      if(node[target].state==S)
	{
	  prob = beta[type]*node[source].w[j];
	  
	  if(gsl_rng_uniform(random_gsl)<prob)
	    {
	      //Check if asymptomatic
	      if(gsl_rng_uniform(random_gsl)<p)
		addToList(&new_latent_asymptomatic,target);
	      else
		addToList(&new_latent_symptomatic,target);
	      
	      //Update target data
	      node[target].state = L;

	      //Various
	      n_active++;
	      n_active_cum++;
	    }
	}
    }  
}

void latency()
{
  int id;

  for(int i=0;i<latent_asymptomatic.n;i++)
    {
      id = latent_asymptomatic.v[i];
      if(gsl_rng_uniform(random_gsl)<epsilon_asymptomatic)
	{
	  addToList(&new_infectious_asymptomatic,id);
	  node[id].state = IA;

	  i = removeFromList(&latent_asymptomatic,i);
	}
    }

  for(int i=0;i<latent_symptomatic.n;i++)
    {
      id = latent_symptomatic.v[i];
      if(gsl_rng_uniform(random_gsl)<epsilon_symptomatic)
	{
	  addToList(&new_pre_symptomatic,i);
	  node[id].state = PS;

	  i = removeFromList(&latent_symptomatic,i);
	}
    }
}

void IaToR()
{
  int id;

  for(int i=0;i<infectious_asymptomatic.n;i++)
    {
      id = infectious_asymptomatic.v[i];
      if(gsl_rng_uniform(random_gsl)<mu) //recovery
	{
	  addToList(&new_recovered,id);
	  node[id].state = R;

	  i = removeFromList(&infectious_asymptomatic,i);
	  n_active--;
	}
    }
}

void preToI()
{
  int id;

  for(int i=0;i<pre_symptomatic.n;i++)
    {
      id = pre_symptomatic.v[i];
      if(gsl_rng_uniform(random_gsl)<gammita) //onset of symptoms
	{
	  addToList(&new_infectious_symptomatic,id);
	  node[id].state = IS;

	  if(gsl_rng_uniform(random_gsl)<alpha[node[id].age]) //if hospitalization will be required
	    node[id].hospitalization = 1;
	  else
	    node[id].hospitalization = 0;

	  i = removeFromList(&pre_symptomatic,i);
	}
    }
}

void IsTransition()
{
  int id;

  for(int i=0;i<infectious_symptomatic.n;i++)
    {
      id = infectious_symptomatic.v[i];
      if(gsl_rng_uniform(random_gsl)<mu) //days to R/Home
	{
	  if(node[id].hospitalization==1) //Home
	    {
	      addToList(&new_home,id);
	      node[id].state = HOME;
	    }
	  else //Recovery
	    {
	      addToList(&new_recovered,id);
	      node[id].state = R;

	      n_active--;
	    }
	  i = removeFromList(&infectious_symptomatic,i);
	}
    }
}

void homeTransition()
{
  int id;
  
  for(int i=0;i<home.n;i++)
    {
      id = home.v[i];
      if(gsl_rng_uniform(random_gsl)<delta) //time to hospitalization
	{
	  if(gsl_rng_uniform(random_gsl)<(1-xi[node[id].age])) //Go to hospital
	    {
	      addToList(&new_hospital,id);
	      node[id].state = H;
	    }
	  else //Go to ICU
	    {
	      addToList(&new_icu,id);
	      node[id].state = ICU;
	    }
	        
	  i = removeFromList(&home,i);
	}
    }
}

void hospitals()
{
  int id;

  //Hospital
  for(int i=0;i<hospital.n;i++)
    {
      id = hospital.v[i];
      if(gsl_rng_uniform(random_gsl)<muH) //transition
	{
	  addToList(&new_recovered,id);
	  node[id].state = R;

	  i = removeFromList(&hospital,i);
	  n_active--;
	}
    }
  
  //ICU
  for(int i=0;i<icu.n;i++)
    {
      id = icu.v[i];
      if(gsl_rng_uniform(random_gsl)<muICU) //transition
	{
	  addToList(&new_recovered,id);
	  node[id].state = R;
 
	  i = removeFromList(&icu,i);
	  n_active--;
	}
    }
}

void updateTime()
{ 
  t++;
}

void resetVariables()
{  
  latent_asymptomatic.n = 0;
  latent_symptomatic.n = 0;
  infectious_asymptomatic.n = 0;
  pre_symptomatic.n = 0;
  infectious_symptomatic.n = 0;
  home.n = 0;
  hospital.n = 0;
  icu.n = 0;
  recovered.n = 0;

  for(int i=0;i<NAGE;i++)
    {
      latent_asymptomatic.cum[i] = 0;
      latent_symptomatic.cum[i] = 0;
      infectious_asymptomatic.cum[i] = 0;
      pre_symptomatic.cum[i] = 0;
      infectious_symptomatic.cum[i] = 0;
      home.cum[i] = 0;
      hospital.cum[i] = 0;
      icu.cum[i] = 0;
      recovered.cum[i] = 0;
    }

  t = 0;
  n_active = 0;
  n_active_cum = 0;
}

void resetNodes()
{
  for(int i=0;i<N;i++)
    node[i].state = S;
}

void resetNew()
{
  new_latent_asymptomatic.n = 0;
  new_latent_symptomatic.n = 0;
  new_infectious_asymptomatic.n = 0;
  new_pre_symptomatic.n = 0;
  new_infectious_symptomatic.n = 0;
  new_home.n = 0;
  new_hospital.n = 0;
  new_icu.n = 0;
  new_recovered.n = 0;
}

void updateLists()
{
  updateList(&latent_asymptomatic,&new_latent_asymptomatic);//LA
  updateList(&latent_symptomatic,&new_latent_symptomatic);//LS
  updateList(&infectious_asymptomatic,&new_infectious_asymptomatic);//IA
  updateList(&pre_symptomatic,&new_pre_symptomatic);//PS
  updateList(&infectious_symptomatic,&new_infectious_symptomatic);//IS
  updateList(&home,&new_home); //Home
  updateList(&hospital,&new_hospital);//H
  updateList(&icu,&new_icu);//ICU
  updateList(&recovered,&new_recovered);//R
}

void results(int run)
{
  //Cumulative values
  for(int i=0;i<NAGE;i++)
    fprintf(f_cum,"%d %d %d %d %d %d %d %d %d ",latent_asymptomatic.cum[i],latent_symptomatic.cum[i],infectious_asymptomatic.cum[i],pre_symptomatic.cum[i],infectious_symptomatic.cum[i],home.cum[i],hospital.cum[i],icu.cum[i],recovered.cum[i]);
  fprintf(f_cum,"%d\n",run);
}
