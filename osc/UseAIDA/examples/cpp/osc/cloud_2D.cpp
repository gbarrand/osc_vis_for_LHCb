
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// Cloud2D

static void cloud_2D(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  AIDA::IHistogramFactory* histogramFactory = 
    a_aida.createHistogramFactory(*mem);
  if(!histogramFactory) return;

  AIDA::ICloud2D* cloud = histogramFactory->createCloud2D("cloud2D","Cloud2D");
  delete histogramFactory;
  if(!cloud) return;

  // Fill the cloud :
  inlib::random::gauss r(0,1);
  for(int index=0;index<10000;index++)  {
    cloud->fill(r.shoot(),r.shoot(),1);
  }

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->setTitle("Cloud 2D");

  plotter->createRegions(1,1,0);

  ///////////////////////////////////////////////////////////////////////////
  //First region :
  ///////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    
    region.plot(*cloud);

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
    std::cout << "aida_exa_cloud_2D :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  cloud_2D(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
