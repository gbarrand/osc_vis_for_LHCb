
/*
 * This example is a UseAIDA one that demonstrate the dynamic loading
 * of an implementation.
 */

#include <AIDA/AIDA.h>

#include "lib/Loader.h" //Loader class found in AIDA-3.3/test/src/cpp.

#include <inlib/random>
#include <iostream>

#include <stdlib.h>

int main(int argc,char* argv[]) {

  // Instead of doing the usual below that compells to link an 
  // implementation, we are going to dynamic load an implementation.
  //IAnalysisFactory* af = AIDA_createAnalysisFactory();
  //if(!af) return EXIT_FAILURE;

  // The name of the DLL of the AIDA implementation containing 
  // the main analysis factory.
  std::string dll = "OnXLabLoadAIDA"; //Default is OSC/OnXLab one.
  if(argc==2) dll = std::string(argv[1]); //Someone can pass another one.
  
  // WARNING : the loader object must be deleted last.
  AIDA::Loader* loader = new AIDA::Loader(dll);
  if(!loader->isOpened()) {
    std::cout << "load_aida :" 
              << " dll " << dll << " not found."
              << std::endl;
    delete loader;
    return EXIT_FAILURE;
  }

  AIDA::IAnalysisFactory* af = loader->findAIDA();
  if(!af) {
    std::cout << "load_aida :" 
              << " can't create an AIDA::IAnalysisFactory." 
              << std::endl;
    delete loader;
    return EXIT_FAILURE;
  }

  AIDA::ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  AIDA::ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  AIDA::IHistogram1D* h1d = hf->createHistogram1D("test 1d",50,-3,3);
  AIDA::IHistogram2D* h2d = hf->createHistogram2D("test 2d",50,-3,3,50,-3,3);
  delete hf;
  if(!h1d) return EXIT_FAILURE;
  if(!h2d) return EXIT_FAILURE;
      
  inlib::random::gauss r(0,1);
  for (int i=0; i<10000; i++) {
    h1d->fill(r.shoot());
    h2d->fill(r.shoot(),r.shoot());
  }
      
  AIDA::IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  AIDA::IPlotter* plotter = pf->create();
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
  delete loader; //Must be the last one.

  return EXIT_SUCCESS;
}
