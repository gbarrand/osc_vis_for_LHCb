
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Hippodraw tuple file reading example.

static void hippo(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // If already loaded close (not delete) the file :
  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/aptuple.tnt.gz";

  AIDA::ITree* tree = 
    amanip::create_tree(a_aida,fileName,"hippodraw",true,false);
  if(!tree) return;

  AIDA::ITuple* tuple = amanip::find_tuple(*tree,"aptuple",true,a_out);
  if(!tuple) return;

  // Create some factories to work with the tree :
  AIDA::ITupleFactory* tupleFactory = a_aida.createTupleFactory(*tree); 
  if(!tupleFactory) return;

  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D
      (a_aida,*mem,"h_age","Age",50,20,70);

  AIDA::IEvaluator* evaluator = tupleFactory->createEvaluator("Age");
  AIDA::IFilter* filter = tupleFactory->createFilter("");
  tuple->project(*h,*evaluator,*filter);
  delete evaluator;
  delete filter;

  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->setTitle("hippodraw tuple example");
    plotter->createRegions(1,1,0);
    AIDA::IPlotterRegion& region = plotter->next();
    region.plot(*h);
    if(a_ui) {
      plotter->show();
      plotter->interact();
    }  
    delete plotter;
  }
 
  delete tupleFactory;

}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_hippo :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  hippo(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
