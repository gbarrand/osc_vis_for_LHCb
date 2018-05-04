
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// Test mag, axes title.

static void mag(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  AIDA::IHistogram1D* histogram = 
    amanip::create_histogram_1D
      (a_aida,*mem,"h_mag","Rand Gauss",100,0,100000);
  if(!histogram) return;
  
  // Fill :
  inlib::random::gauss r(0,1);
  for(int index=0;index<100000;index++)  {
    histogram->fill(r.shoot(),1);
  }

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->setTitle("osc-plot");

  plotter->createRegions(1,1,0);

  ///////////////////////////////////////////////////////////////////////////
  //First region :
  ///////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    
    region.style().xAxisStyle().setLabel("Random gauss");
    region.style().yAxisStyle().setLabel("Entries");

    region.plot(*histogram);
  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;
}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_mag :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  mag(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
