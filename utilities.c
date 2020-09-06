#include "head.h"

void allocateMemory()
{
  //Spreading
  latent_asymptomatic.v = malloc(N * sizeof *latent_asymptomatic.v);
  latent_symptomatic.v = malloc(N * sizeof *latent_symptomatic.v);
  infectious_asymptomatic.v = malloc(N * sizeof *infectious_asymptomatic.v);
  pre_symptomatic.v = malloc(N * sizeof *pre_symptomatic.v);
  infectious_symptomatic.v = malloc(N * sizeof *infectious_symptomatic.v);
  home.v = malloc(N * sizeof *home.v);
  hospital.v = malloc(N * sizeof *hospital.v);
  icu.v = malloc(N * sizeof *icu.v);
  recovered.v = malloc(N * sizeof *recovered.v);
  
  //New spreading
  new_latent_asymptomatic.v = malloc(N * sizeof *new_latent_asymptomatic.v);
  new_latent_symptomatic.v = malloc(N * sizeof *new_latent_symptomatic.v);
  new_infectious_asymptomatic.v = malloc(N * sizeof *new_infectious_asymptomatic.v);
  new_pre_symptomatic.v = malloc(N * sizeof *new_pre_symptomatic.v);
  new_infectious_symptomatic.v = malloc(N * sizeof *new_infectious_symptomatic.v);
  new_home.v = malloc(N * sizeof *new_home.v);
  new_hospital.v = malloc(N * sizeof *new_hospital.v);
  new_icu.v = malloc(N * sizeof *new_icu.v);
  new_recovered.v = malloc(N * sizeof *new_recovered.v);
}

void initRandom(int seed)
{
  gsl_rng_env_setup();
  T = gsl_rng_default;
  random_gsl = gsl_rng_alloc(T);
  gsl_rng_set(random_gsl,seed);
}

void setBeta()
{
  double beta_pre;

  beta_pre = beta_normal*gammita*k/(mu*(1-k));
    
  for(int i=0;i<NCOMPARTMENTS;i++)
    beta[i] = 0;

  beta[IA] = r*beta_normal;
  beta[IS] = beta_normal;
  beta[PS] = beta_pre;
}

void openFiles()
{
  char name_cum[100], name_data[100];

  sprintf(name_cum,"Results/cum_baseline_p%d.txt",parameters);
  sprintf(name_data,"Results/data_baseline_p%d.txt",parameters);

  f_cum = fopen(name_cum,"w");
  f_data = fopen(name_data,"w");
}

void addToList(List *list, int id)
{
  list->v[list->n] = id;
  list->n++;
}

int removeFromList(List *list, int i)
{
  list->n--;
  list->v[i] = list->v[list->n];

  return i-1;
}

void updateList(List* list, List *new)
{
  for(int i=0;i<new->n;i++)
    {
      list->v[list->n] = new->v[i];
      list->n++;
      list->cum[node[new->v[i]].age]++;
    }
}

void closeFiles()
{
  fclose(f_cum);
  fclose(f_data);
}

void print(int t0)
{
  printf("Execution time %d seconds\n",(int)time(NULL)-t0);
}

void freeMemory()
{
  for(int i=0;i<N;i++)
    {
      free(node[i].v);
      free(node[i].w);
    }
  free(node);

  free(latent_asymptomatic.v);
  free(latent_symptomatic.v);
  free(infectious_asymptomatic.v);
  free(pre_symptomatic.v);
  free(infectious_symptomatic.v);
  free(home.v);
  free(hospital.v);
  free(icu.v);
  free(recovered.v);

  free(new_latent_asymptomatic.v);
  free(new_latent_symptomatic.v);
  free(new_infectious_asymptomatic.v);
  free(new_pre_symptomatic.v);
  free(new_infectious_symptomatic.v);
  free(new_home.v);
  free(new_hospital.v);
  free(new_icu.v);
  free(new_recovered.v);
         
  gsl_rng_free(random_gsl);
}
