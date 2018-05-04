//
// G.Barrand : to provide a file to the Restore example.
//        


#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

int main(int argc,char* argv[]) {
  // The main and only concrete entry point !
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) return EXIT_FAILURE;

  // Someone get everything through factories...
  AIDA::ITreeFactory* trf = aida->createTreeFactory();
  if(!trf) return EXIT_FAILURE;

  // Create a tree-like container associated to a file...
  AIDA::ITree* tree = trf->create("myFile.aida","aida",false,true);
  delete trf;
  if(!tree) return EXIT_FAILURE;

  AIDA::IHistogramFactory* hf = aida->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;

  Random r;

 {AIDA::IHistogram1D* h = hf->createHistogram1D("test 1d",50,-3,3);
  if(!h) return EXIT_FAILURE;      
  for (int i=0; i<10000; i++) h->fill(r.nextGaussian());}
      
 {AIDA::IHistogram2D* h = hf->createHistogram2D("test 2d",50,-3,3,50,-3,3);
  if(!h) return EXIT_FAILURE;      
  for (int i=0; i<10000; i++) h->fill(r.nextGaussian(),r.nextGaussian(),1);}
      
  tree->commit();

  delete hf;
  delete aida;

  return EXIT_SUCCESS;
}
