
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

#include <inlib/random>

// Store/retrieve and histogram.

static void reopen(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  std::string fileName = "reopen.root";    

 {a_out << "Create a tree associated to a " << fileName << " file." 
        << std::endl;

  AIDA::ITree* tree = amanip::create_tree(a_aida,fileName,"ROOT",false,true);
  if(tree) {
    tree->mkdir("histograms");
    tree->cd("histograms");

    // Create an histogram in the ROOT tree :
    AIDA::IHistogram1D* histogram = 
      amanip::create_histogram_1D(a_aida,*tree,"h","Rand gauss",100,-5,5);
    if(histogram) {
      inlib::random::gauss r(0,1);
      for(int index=0;index<10000;index++) histogram->fill(r.shoot(),1);
    }

    tree->commit();
    tree->ls();

    // Destroy the tree :
    delete tree;
  }}

  
  //////////////////////////////////////////////////////
  // Reopen the file in read mode, get the histo and plot :
  //////////////////////////////////////////////////////
 {a_out << "Reopen the " << fileName << " file..." << std::endl;

  AIDA::ITree* tree = amanip::create_tree(a_aida,fileName,"ROOT",true,false);
  if(tree) {

    tree->cd("histograms");

    AIDA::IHistogram1D* histogram = 
      amanip::find_histogram_1D(*tree,"h",true,a_out);
    if(histogram) {

      AIDA::IPlotter* plotter = 
        amanip::create_plotter(a_aida,a_plotter);
      if(plotter) {
        plotter->setTitle("Store/retrieve an histogram");
        plotter->createRegions(1,1,0);
        AIDA::IPlotterRegion& region = plotter->currentRegion();
        region.plot(*histogram);
        if(a_ui) {
          plotter->show();
          plotter->interact();
        }
        delete plotter;
      }
    }
  }}

}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_reopen :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  reopen(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
