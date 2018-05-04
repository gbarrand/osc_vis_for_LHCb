
// C++ conversion of the java/Tree.java example.

#include <AIDA/AIDA.h>

using namespace AIDA;

#include <stdlib.h>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;

  hf->createHistogram1D("test 1d",50,-3,3);
  hf->createHistogram2D("test 2d",50,-3,3,50,-3,3);
      
  tree->mkdir("/folder1");
  tree->cd("/folder1");
  hf->createHistogram1D("test 1d",50,-3,3);
  hf->createHistogram2D("test 2d",50,-3,3,50,-3,3);   
      
  tree->mkdir("/folder2");
  tree->cd("/folder2");
  hf->createHistogram1D("test 1d",50,-3,3);
  hf->createHistogram2D("test 2d",50,-3,3,50,-3,3);         

  delete hf;
  delete af;

  return EXIT_SUCCESS;
}
