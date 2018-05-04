#include "AIDA/AIDA.h"
using namespace AIDA;

#include <stdlib.h> //rand
#include <math.h>

//prototype randgauss()
double randgauss(double, double, double, double);

//define randgauss()
double randgauss( double min, double max, double sigma, double center)
{
  double random= (min+ (max-min)*(double)rand()/RAND_MAX);

  double tmp=(random-center)/sigma;
  double gauss=exp(-tmp*tmp/2);
  return gauss;
}

int main()
{
  IAnalysisFactory *analysisFactory = AIDA_createAnalysisFactory();

  ITreeFactory *treeFactory = analysisFactory->createTreeFactory();

  ITree *tree = treeFactory->create("gaussian.aida", "xml", false, true);  

  delete treeFactory;

  ITupleFactory* tupleFactory = analysisFactory->createTupleFactory (*tree);

  ITuple* tuple = tupleFactory->create("tuplePhoton", "the   characteristics of photons ", "double r1,  r2,  r3");

  delete tupleFactory;

  for(int i=0; i<24; i++)
    {
        double r1=0.;
        double r2=0.;
        double r3=0.;
        double min=0., max=1.0, sigma=1.0, center=0.;
        r1=randgauss(min, max, sigma, center);
        r2=randgauss(min, max, sigma, center)+10;
        r3=randgauss(min, max, sigma, center)+100;
        tuple->fill(0,r1);
        tuple->fill(1,r2);
        tuple->fill(2,r3);
        tuple->addRow();
  }

  tree->commit();
  tree->close();

  delete analysisFactory;

  return 0;
}
