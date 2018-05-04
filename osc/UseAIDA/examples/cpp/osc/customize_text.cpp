
//
// Example to show how to customize text (size, font, color)
// of a plot title, axes labels, axes title.
//

#include <AIDA/AIDA.h>

#include <cstdlib>

#include <iostream>

double rgauss();

int main(int argc,char* argv[]) {

  AIDA::IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  AIDA::ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  AIDA::ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  AIDA::IHistogram1D* h = hf->createHistogram1D("Random Gauss",50,-10,10);
  delete hf;
  if(!h) return EXIT_FAILURE;
      
  for (int i=0; i<10000; i++) {
    h->fill(rgauss());
  }
    
 {AIDA::IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(!pf) {
    std::cout << "Can't create a plotter factory." << std::endl;
  } else {
    AIDA::IPlotter* plotter = pf->create();
    delete pf;
    if(!plotter) {
      std::cout << "Can't create a plotter." << std::endl;
    } else {
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.plot(*h);

      // Customization :
      region.setParameter("plotter.wallStyle.visible","FALSE");

      // Changing text scale :
      region.setParameter("plotter.titleStyle.scale","2");

      region.setParameter("plotter.xAxis.title","random");
      region.setParameter("plotter.xAxis.titleToAxisJustification","CENTER");
      region.setParameter("plotter.xAxis.titleStyle.scale","2");
      region.setParameter("plotter.xAxis.labelsStyle.scale","2");
      region.setParameter("plotter.xAxis.titleToAxis","0.06");

      region.setParameter("plotter.yAxis.labelsStyle.scale","1.7");

      // Changing text font :
      std::string bold("TTF/arialbd");  //TTF arial bold.
      region.setParameter("plotter.xAxis.titleStyle.fontName",bold);
      region.setParameter("plotter.xAxis.labelsStyle.fontName",bold);
      region.setParameter("plotter.yAxis.titleStyle.fontName",bold);
      region.setParameter("plotter.yAxis.labelsStyle.fontName",bold);
      region.setParameter("plotter.titleStyle.fontName",bold);

      // Changing text color :
      region.setParameter("plotter.titleStyle.color","0 0 1");

      plotter->show();
      plotter->interact();
      delete plotter;
    }
  }}

  delete af;

  return EXIT_SUCCESS;
}

#include <cmath>
#include <cstdlib>
double rgauss() {
  double sigma = 1; double mean = 0;
  double v1,v2,r;
  do {
    v1 = 2.0 * ((double)::rand()/(double)RAND_MAX) - 1.0;
    v2 = 2.0 * ((double)::rand()/(double)RAND_MAX) - 1.0;
    r = v1*v1 + v2*v2;
  } while ( r > 1.0 );
  double fac = ::sqrt(-2.0*::log(r)/r);
  return (v2 * fac) * sigma + mean;
}
