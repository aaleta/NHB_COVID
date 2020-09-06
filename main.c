#include "head.h"

int main(int argc, char *argv[])
{
  int t0;
  t0 = time(NULL);

  initialize(argc,argv);

  openFiles();
  runSimulation();
  closeFiles();

  print(t0);

  freeMemory();

  return 0;
}

void initialize(int argc, char *argv[])
{
  int seed;

  n_runs = 100;
  parameters = 0;

  seed = time(NULL);
  initRandom(seed);

  readParameters();
  allocateMemory();
  readData();
  setBeta();
}

void runSimulation()
{
  for(int run=0;run<n_runs;run++)
    {
      init();
      while(n_active>0)
	spread(run);
      results(run);
    }
}
