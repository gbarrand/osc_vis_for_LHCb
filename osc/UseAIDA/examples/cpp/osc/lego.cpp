
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// Histogram2D, lego plot

static void lego(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  AIDA::IHistogram2D* histogram = 
    amanip::create_histogram_2D
      (a_aida,*mem,"Histogram2D","Histogram2D",
     100,-5,5,100,-5,5);
  if(!histogram) return;
  
  // Fill :
  inlib::random::gauss r(0,1);
  for(int index=0;index<10000;index++)  {
    histogram->fill(r.shoot(),r.shoot(),1);
  }

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->setTitle("Histogram 2D");

  plotter->createRegions(1,1,0);

  AIDA::IPlotterRegion& region = plotter->currentRegion();
    
  region.setParameter("plotter.wallStyle.visible","FALSE");

  region.style().xAxisStyle().setLabel("Random gauss");
  region.style().yAxisStyle().setLabel("Random gauss");
  region.style().zAxisStyle().setLabel("Entries");

  histogram->annotation().addItem("plotter.multiNodeLimit","1000000000");
  histogram->annotation().addItem("plotter.modeling","boxes");
  //histogram->annotation().addItem("plotter.painting","grey_scale");
  histogram->annotation().addItem("plotter.painting","violet_to_red");
  //histogram->annotation().addItem("plotter.painting","by_value");
  //histogram->annotation().addItem("plotter.colorMapping","black 0 cyan 5 green 10 orange 15 red");

  region.plot(*histogram);

  //Lego layout :
  region.setParameter("plotter.shapeAutomated","FALSE");
  region.setParameter("plotter.shape","XYZ");

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;
}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_lego :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  lego(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
