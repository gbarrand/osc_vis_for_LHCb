
// C++ conversion of the java/PlotExample.java example.

#include <AIDA/AIDA.h>

#include <inlib/app>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

#include <iostream>

int main(int argc,char* argv[]) {

  if(argc>0) {
    //Darwin : OSC specific if mouse starting 
    //         from AIDA_example_PlotExample.app :
    std::vector<std::string> home_dirs;
    home_dirs.push_back("OpenScientist");
    home_dirs.push_back("AIDA");
    home_dirs.push_back("session");
    inlib::if_mouse_startup(argv[0],"OSC_HOME_DIR",home_dirs,std::cout);
  }

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  IHistogram2D* h2d = hf->createHistogram2D("test 2d",50,-30,30,50,-3,3);
  if(!h2d) return EXIT_FAILURE;
      
  Random r;
  for (int i=0; i<10000; i++) {
    h2d->fill(10*r.nextGaussian(),r.nextGaussian());
  }
    
 {
 //int argc = 1;
 //static char* argv[] = {(char*)"-Qt"}; 
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(!pf) {
    std::cout << "Can't create a plotter factory." << std::endl;
  } else {
    IPlotter* plotter = pf->create();
    delete pf;
    if(!plotter) {
      std::cout << "Can't create a plotter." << std::endl;
    } else {
      plotter->destroyRegions();
      plotter->createRegion(0,0,.66,1)->plot(*h2d);
      plotter->createRegion(.66,0,.33,.5)->plot(*hf->projectionX("X Projection",*h2d));
      plotter->createRegion(.66,.5,.33,.5)->plot(*hf->projectionY("Y Projection",*h2d));
      plotter->show();
      plotter->interact();
      delete plotter;
    }
  }}

  delete hf;

  delete af;

  return EXIT_SUCCESS;
}
