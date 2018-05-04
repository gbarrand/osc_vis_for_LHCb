
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Function 2D example.

static void func_2D(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Get some factories :
  AIDA::IFunctionFactory* functionFactory = 
    a_aida.createFunctionFactory(*mem); 
  if(!functionFactory) return;

  AIDA::IFunction* function = 
    functionFactory->createFunctionFromScript
          ("func_exa_4",2,"(sin(x)/x)*(sin(y)/y)","","","");
  if(!function) {
    a_out << "aida_exa_func_2D :"
          << " Can't create function."
          << std::endl;
    return;
  }

  function->setTitle("");

  // Since the function is not defined at 0, to avoid problem
  // at plotting, we tilt a bit the visualization grid domain. 
  function->annotation().addItem("domain.xAxisMinimum","-10");
  function->annotation().addItem("domain.xAxisMaximum","9.999");
  function->annotation().addItem("domain.yAxisMinimum","-10");
  function->annotation().addItem("domain.yAxisMaximum","9.999");

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->setTitle("Function 2D");
    plotter->createRegions(1,1,0);
    
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    
    region.style().xAxisStyle().setLabel("x1");
    region.style().yAxisStyle().setLabel("y1");
    region.style().zAxisStyle().setLabel("func");

    region.setParameter("plotter.xAxisAutomated","FALSE");
    region.setParameter("plotter.xAxisMinimum","-10");
    region.setParameter("plotter.xAxisMaximum","10");
    region.setParameter("plotter.yAxisAutomated","FALSE");
    region.setParameter("plotter.yAxisMinimum","-10");
    region.setParameter("plotter.yAxisMaximum","10");

    region.setParameter("plotter.wallStyle.visible","FALSE");

    function->annotation().addItem("plotter.multiNodeLimit","1000000");
    function->annotation().addItem("plotter.painting","by_value");
    function->annotation().addItem("plotter.colorMapping","black -0.2 cyan 0. green 0.1 orange 0.4 red");

    region.plot(*function);

    if(a_ui) {
      plotter->show();
      plotter->interact();
    }

    delete plotter;
  }
 
  delete functionFactory;

}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_func_2D :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  func_2D(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
