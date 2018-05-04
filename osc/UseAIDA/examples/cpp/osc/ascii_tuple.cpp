
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Simple ASCII tuple file reading example.

static void ascii_tuple(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->setTitle("Ascii tuple example");
  plotter->createRegions(1,1,0);
    
  // If already loaded close (not delete) the file :
  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/data.ascii";

  AIDA::ITree* tree = 
    amanip::create_tree(a_aida,fileName,"ASCII",true,false);
  if(!tree) return;

  // Create some factories to work with the tree :
  AIDA::ITupleFactory* tupleFactory = a_aida.createTupleFactory(*tree); 
  if(!tupleFactory) return;

  /////////////////////////////////////////////////////////////////////////////
  // Get a tuple, create an histo from one of its column, plot it. 
  /////////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();

    // Column names are v0 v1 ...
    AIDA::ITuple* tuple = amanip::find_tuple(*tree,"data",true,a_out);
    if(!tuple) return;

    AIDA::IHistogram1D* h = 
      amanip::create_histogram_1D(a_aida,*mem,"h","tuple/v1",10,1,11);

    AIDA::IEvaluator* evaluator = tupleFactory->createEvaluator("v1");
    AIDA::IFilter* filter = tupleFactory->createFilter("");
    tuple->project(*h,*evaluator,*filter);
    delete evaluator;
    delete filter;

    region.plot(*h);
  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;

  delete tupleFactory;

}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_ascii_tuple :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  ascii_tuple(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
