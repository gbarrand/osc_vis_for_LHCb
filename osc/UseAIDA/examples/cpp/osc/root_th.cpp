
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// ROOT TH* reading example.

static void root_th(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Cleanup mem :
  mem->rmdir("/");

  // If already loaded close (not delete) the file :
  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/RootW.root";

  AIDA::ITree* rootTree = 
    amanip::create_tree(a_aida,fileName,"ROOT",true,false);
  if(!rootTree) return;

  rootTree->ls();

  // Get a TH1F :
  AIDA::IHistogram1D* totalHistogram = 
    amanip::find_histogram_1D(*rootTree,"totalHistogram",true,a_out);

  if(!totalHistogram) {
    a_out << "aida_exa_root_th :"
          << " totalHistogram not found or is not a TH1F."
          << std::endl;
  }

  // Get a TH2F :
  AIDA::IHistogram2D* hpxpy = 
    amanip::find_histogram_2D(*rootTree,"hpxpy",true,a_out);

  if(!hpxpy) {
    a_out << "aida_exa_root_th :"
          << " hpxpy not found or is not a TH2F."
          << std::endl;
  }

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->createRegions(1,2,0);
    plotter->setTitle("ROOT TH*");    
   {AIDA::IPlotterRegion& region = plotter->currentRegion();
    if(totalHistogram) region.plot(*totalHistogram);}
   {AIDA::IPlotterRegion& region = plotter->next();
    if(hpxpy) region.plot(*hpxpy);}
    
    if(a_ui) {
      plotter->show();
      plotter->interact();
    }
    delete plotter;
  }
}

int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_root_th :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  root_th(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
