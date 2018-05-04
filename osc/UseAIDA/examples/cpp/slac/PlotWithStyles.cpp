
// C++ conversion of the java/PlotWithStyles.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <iostream>
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
  IHistogram1D* h1 = hf->createHistogram1D("h1",100,-5,5);
  IHistogram1D* h2 = hf->createHistogram1D("h2",100,-5,5);
  if(!h1) return EXIT_FAILURE;
  if(!h2) return EXIT_FAILURE;
      
  Random r;
  for ( int i = 0; i<10000; i++ ) {
    h1->fill(r.nextGaussian());
    h2->fill(r.nextGaussian());
  }
      
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  if(plotter) {
    //This are the default styles for the region
    IPlotterStyle& regionStyle = plotter->region(0)->style();
    regionStyle.dataStyle().lineStyle().setParameter("color","black");
    regionStyle.xAxisStyle().setLabel("E/m");
    regionStyle.yAxisStyle().setLabel("# Evt");
        
    regionStyle.titleStyle().textStyle().setFontSize(30);
    regionStyle.titleStyle().textStyle().setColor("orange");
        
    regionStyle.xAxisStyle().labelStyle().setFontSize(24);
    regionStyle.xAxisStyle().labelStyle().setItalic(true);
    regionStyle.xAxisStyle().labelStyle().setColor("black");
    regionStyle.xAxisStyle().tickLabelStyle().setFontSize(14);
    regionStyle.xAxisStyle().tickLabelStyle().setBold(true);
    regionStyle.xAxisStyle().tickLabelStyle().setColor("blue");
        
    regionStyle.yAxisStyle().labelStyle().setFontSize(24);
    regionStyle.yAxisStyle().labelStyle().setItalic(true);
    regionStyle.yAxisStyle().labelStyle().setColor("brown");
    regionStyle.yAxisStyle().tickLabelStyle().setFontSize(14);
    regionStyle.yAxisStyle().tickLabelStyle().setBold(true);
    regionStyle.yAxisStyle().tickLabelStyle().setColor("green");
        
    plotter->region(0)->plot(*h1);

    //This styles overwrite some of the region styles
    IPlotterStyle* style = pf->createPlotterStyle();
    if(!style) { //GB
      std::cout << "Can't get a plotter style."<< std::endl;
    } else {
      style->dataStyle().markerStyle().setParameter("size","12");
      style->dataStyle().markerStyle().setParameter("shape","3");
      style->dataStyle().markerStyle().setParameter("color","blue");
      plotter->region(0)->plot(*h2,*style);
      delete style;
    }
 
    plotter->show();
    plotter->interact();

    delete plotter;
  }
  delete pf;
  }

  delete hf;

  delete af;

  return EXIT_SUCCESS;
}
