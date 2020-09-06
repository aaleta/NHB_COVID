#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define NAGE 5
#define NCOMPARTMENTS 9

typedef struct Node{
  //Info
  int age;
  //Infection
  int state;
  int hospitalization;
  //Connections
  int k;
  int *v;
  double *w;
} Node;

typedef struct List{
  int *v;
  int n;
  int cum[NAGE];
} List;

//States
#define S 0
#define L 1
#define IA 2
#define PS 3
#define IS 4
#define HOME 5
#define H 6
#define ICU 7
#define R 8

//Random numbers
const gsl_rng_type * T;
gsl_rng *random_gsl;

//Parameters
int N, n_runs, parameters;
double r, epsilon_asymptomatic, epsilon_symptomatic, p, gammita, mu, delta, muH, muICU, k, beta_normal;
double alpha[NAGE], xi[NAGE], beta[NCOMPARTMENTS];
//Spreading
int n_active, index_node;
List latent_asymptomatic, latent_symptomatic, infectious_asymptomatic, pre_symptomatic, infectious_symptomatic, home, hospital, icu, recovered;
List new_latent_asymptomatic, new_latent_symptomatic, new_infectious_asymptomatic, new_pre_symptomatic, new_infectious_symptomatic, new_home, new_hospital, new_icu, new_recovered;
//Network
Node *node;
//Various
int t;
FILE *f_cum, *f_data;
//

//main
void initialize(int, char**);
void runSimulation();
//spread
void init();
void seedInfection();
void spread(int);
void infection();
void infect(int, int);
void latency();
void IaToR();
void preToI();
void IsTransition();
void homeTransition();
void hospitals();
void updateTime();
void resetVariables();
void resetNodes();
void resetNew();
void updateLists();
void results(int);
//read
void readData();
void readParameters();
void readNetwork();
void readNodes();
//utilities
void allocateMemory();
void initRandom(int);
void openFiles();
void setBeta();
void addToList(List*, int);
int removeFromList(List*, int);
void updateList(List*, List*);
void closeFiles();
void print(int);
void freeMemory();
