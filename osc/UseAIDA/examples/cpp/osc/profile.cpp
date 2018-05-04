
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// Profile example.

static void profile(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Get some factories :
  AIDA::IHistogramFactory* histogramFactory = 
    a_aida.createHistogramFactory(*mem);
  if(!histogramFactory) return;

  // Cleanup mem :
  mem->rmdir("/");

  AIDA::IProfile1D* histo = 
    histogramFactory->createProfile1D("prof_1","Profile",100,-5,5);
  delete histogramFactory;

  if(!histo) return;

  inlib::random::flat f;
  inlib::random::gauss r(0,1);
  for(int index=0;index<100000;index++) {
    histo->fill(r.shoot(),f.shoot()-0.5,1);
    //histo->fill(r.shoot(),r.shoot(),1);
  }
  
  a_out << "aida_exa_profile :"
        << " Profile \"" << histo->title() << "\""
        << " mean " << histo->mean()
        << " rms " << histo->rms()
        << std::endl;


  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->setTitle("Profile example");
  plotter->createRegions(1,1,0);
    
  AIDA::IPlotterRegion& region = plotter->currentRegion();
    
  region.plot(*histo);
  region.setParameter("plotter.binsStyle[0].modeling","points");
  region.setParameter("plotter.errorsStyle[0].visible","TRUE");

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;
}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_profile :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  profile(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
