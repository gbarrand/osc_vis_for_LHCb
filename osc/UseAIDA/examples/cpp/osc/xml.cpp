
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// XML example.

static void xml(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Cleanup mem :
  mem->rmdir("/");

  // If already loaded close (not delete) the file :
  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/exampleHistogram.aida";

  AIDA::ITree* tree = amanip::create_tree(a_aida,fileName,"XML",true,false);
  if(!tree) return;

  tree->ls();

  AIDA::IHistogram1D* histo = 
    amanip::find_histogram_1D(*tree,"Aida 1D Histogram",true,a_out);
  if(histo) {

    AIDA::IPlotter* plotter = 
      amanip::create_plotter(a_aida,a_plotter);
    if(plotter) {
      plotter->setTitle("AIDA XML example");
      plotter->createRegions(1,1,0);
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.plot(*histo);
      if(a_ui) {
        plotter->show();
        plotter->interact();
      }
      delete plotter;
    }
  }

}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_xml :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  xml(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
