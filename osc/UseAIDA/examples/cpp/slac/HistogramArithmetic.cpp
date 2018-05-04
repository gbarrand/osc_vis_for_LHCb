
// C++ conversion of the java/HistogramArithmetic.java example.

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

  IHistogram1D* h1 = hf->createHistogram1D("test 1d",50,-3,6);
  IHistogram1D* h2 = hf->createHistogram1D("test 2d",50,-3,6);
  if(!h1) return EXIT_FAILURE;
  if(!h2) return EXIT_FAILURE;

  Random r;
  for (int i=0; i<10000; i++) {
    h1->fill(r.nextGaussian());
    h2->fill(3+r.nextGaussian());
  }

  IHistogram1D* plus = hf->add("h1+h2",*h1,*h2);
  IHistogram1D* minus = hf->subtract("h1-h2",*h1,*h2);
  IHistogram1D* mul = hf->multiply("h1*h2",*h1,*h2);
  IHistogram1D* div = hf->divide("h1 over h2",*h1,*h2);
  delete hf;
  
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(2,2,0);
    plotter->region(0)->plot(*plus);
    plotter->region(1)->plot(*minus);
    plotter->region(2)->plot(*mul);
    plotter->region(3)->plot(*div);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete af;

  return EXIT_SUCCESS;
}
