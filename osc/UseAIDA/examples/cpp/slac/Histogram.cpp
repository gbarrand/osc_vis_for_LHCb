
// C++ conversion of the java/Histogram.java example.

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
  IHistogram1D* h1d = hf->createHistogram1D("test 1d",50,-3,3);
  IHistogram2D* h2d = hf->createHistogram2D("test 2d",50,-3,3,50,-3,3);
  delete hf;
  if(!h1d) return EXIT_FAILURE;
  if(!h2d) return EXIT_FAILURE;
      
  Random r;
  for (int i=0; i<10000; i++) {
    h1d->fill(r.nextGaussian());
    h2d->fill(r.nextGaussian(),r.nextGaussian());
  }
      
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(1,2,0);
    plotter->region(0)->plot(*h1d);
    plotter->region(1)->plot(*h2d);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete af;

  return EXIT_SUCCESS;
}
