
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// Cloud3D slice XY

static void cloud_slice(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  AIDA::IHistogramFactory* histogramFactory = 
    a_aida.createHistogramFactory(*mem);
  if(!histogramFactory) return;

  AIDA::ICloud3D* cloud = histogramFactory->createCloud3D("cloud3D","Cloud3D");
  if(!cloud) return;
  
  // Fill the cloud :
  inlib::random::gauss r(0,1);
  for(int index=0;index<10000;index++)  {
    cloud->fill(r.shoot(),r.shoot(),r.shoot(),1);
  }

  // Convert to an histogram :
  cloud->convert(50,-10,10,50,-10,10,50,-10,10);
  const AIDA::IHistogram3D& histogram = cloud->histogram();
  // Get a 2D slice :
  AIDA::IHistogram2D* sliceXY = 
    histogramFactory->projectionXY("slice_XY",histogram);

  delete histogramFactory;

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->setTitle("Cloud 3D");

  plotter->createRegions(1,1,0);

  ///////////////////////////////////////////////////////////////////////////
  //First region :
  ///////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    
    region.plot(*sliceXY);

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
    std::cout << "aida_exa_cloud_slice :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  cloud_slice(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
