
// C++ conversion of the java/Cloud.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

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
  ICloud1D* cl1D = hf->createCloud1D( "cl1D","1-Dimensional Cloud", 1500, "" );
  ICloud2D* cl2D = hf->createCloud2D( "cl2D","2-Dimensional Cloud", 1500, "" );
  delete hf;
  if(!cl1D) return EXIT_FAILURE;
  if(!cl2D) return EXIT_FAILURE;
      
  int entries = 20000;
  Random r;
      
  for ( int i = 0; i < entries; i++ ) {
    double xval = r.nextGaussian();
    double yval = r.nextGaussian();
    double w    = r.nextDouble();         
    cl1D->fill( xval, w );
    cl2D->fill( xval, yval, w );
  }
      
  const IHistogram1D& cl1DHist = cl1D->histogram();
  cl2D->histogram();

  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(1,2,0);
    plotter->region(0)->plot(cl1DHist);
    plotter->region(1)->plot(*cl2D);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete af;

  return EXIT_SUCCESS;
}
