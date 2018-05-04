
// C++ conversion of the java/Cloud.java example.

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
  hf->createCloud1D( "cl1D" );
  hf->createCloud2D( "cl2D", "2-Dimensional Cloud" );
  hf->createCloud3D( "cl3D", "3-Dimensional Cloud", 1500, "autoConvert = false" );     
  delete hf;

  delete af;

  return EXIT_SUCCESS;
}
