
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Cloud3D from a Function2D

static void func_to_cloud(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Get some factories :
  AIDA::IFunctionFactory* functionFactory = 
    a_aida.createFunctionFactory(*mem); 
  if(!functionFactory) return;

  AIDA::IFunction* function = 
    functionFactory->createFunctionFromScript
          ("func_exa_4",2,"(sin(x)/x)*(sin(y)/y)","","","");
  delete functionFactory;
  if(!function) {
    a_out << "aida_exa_func_to_cloud :"
          << " Can't create function."
          << std::endl;
    return;
  }

  //AIDA::IFunction* function = 
  //  amanip::find_function
  //    (*mem,"func_exa_4",true,aUI.session().cout());
  //if(!function) {
  //  aUI.echo("OnXLabExas_Examples_10 : func_exa_4 not found.");
  //  return;
  //}

  AIDA::IHistogramFactory* histogramFactory = 
    a_aida.createHistogramFactory(*mem);
  if(!histogramFactory) return;

  AIDA::ICloud3D* cloud = 
    histogramFactory->createCloud3D("cloud3D","Cloud from func_exa_4"); 
  if(!cloud) {
    a_out << "aida_exa_func_to_cloud :"
          << " can't create Cloud3D."
          << std::endl;
    return;
  }
  delete histogramFactory;
  
  unsigned int nx = 50;
  unsigned int ny = 50;
  
  std::vector<double> args(2);
  for(unsigned int j=0;j<ny;j++) {
    args[1] = -10 + 0.399 * j;
    for(unsigned int i=0;i<nx;i++) {
      args[0] = -10 + 0.399 * i;
      double v = function->value(args);
      cloud->fill(args[0],args[1],v);
    }
  }

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->createRegions(1,2,0);

   {AIDA::IPlotterRegion& region = plotter->currentRegion();
    
    region.setParameter("plotter.xAxisAutomated","FALSE");
    region.setParameter("plotter.xAxisMinimum","-10");
    region.setParameter("plotter.xAxisMaximum","10");
    region.setParameter("plotter.yAxisAutomated","FALSE");
    region.setParameter("plotter.yAxisMinimum","-10");
    region.setParameter("plotter.yAxisMaximum","10");

    region.setParameter("plotter.wallStyle.visible","FALSE");

    function->annotation().addItem("plotter.multiNodeLimit","1000000");
    function->annotation().addItem("plotter.painting","by_value");
    function->annotation().addItem("plotter.color","0 1 0");
    function->annotation().addItem("plotter.colorMapping","black -0.2 cyan 0. green 0.1 orange 0.4 red");

    // Since the function is not defined at 0, to avoid problem
    // at plotting, we tilt a bit the visualization grid domain. 
    function->annotation().addItem("domain.xAxisMinimum","-10");
    function->annotation().addItem("domain.xAxisMaximum","9.999");
    function->annotation().addItem("domain.yAxisMinimum","-10");
    function->annotation().addItem("domain.yAxisMaximum","9.999");

    region.plot(*function);}

   {AIDA::IPlotterRegion& region = plotter->next();
    region.setParameter("plotter.wallStyle.visible","FALSE");
    region.plot(*cloud);}

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
    std::cout << "aida_exa_func_to_cloud :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  func_to_cloud(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
