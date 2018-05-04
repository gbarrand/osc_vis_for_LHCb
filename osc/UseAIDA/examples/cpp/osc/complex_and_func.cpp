
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Complex numbers and function example.

static void complex_and_func(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Get some factories :
  AIDA::IFunctionFactory* functionFactory = 
    a_aida.createFunctionFactory(*mem); 
  if(!functionFactory) return;

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {

  plotter->setTitle("Complex");
  plotter->createRegions(1,2,0);
    
  /////////////////////////////////////////////////////////////
  // complex function : sqrt(exp(-(x*x)/2))*(cos(x)+i*sin(x))
  /////////////////////////////////////////////////////////////

  std::string f_abs = "sqrt(exp(-(x*x)/2))";
  std::string f_real = f_abs+"*cos(x)";
  std::string f_imag = f_abs+"*sin(x)";

  ///////////////////////////////////////////////////////////////////////////
  //First region :
  ///////////////////////////////////////////////////////////////////////////
  {

    AIDA::IPlotterRegion& region = plotter->currentRegion();
    
    region.setParameter("plotter.xAxisAutomated","FALSE");
    region.setParameter("plotter.xAxisMinimum","-10");
    region.setParameter("plotter.xAxisMaximum","10");

    AIDA::IFunction* fa = 
      functionFactory->createFunctionFromScript
          ("complexFunction_abs",1,f_abs,"","","");

    fa->setTitle(f_abs);

    region.plot(*fa);
  }

  ///////////////////////////////////////////////////////////////////////////
  // Second region :
  ///////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();

    region.setParameter("plotter.xAxisAutomated","FALSE");
    region.setParameter("plotter.xAxisMinimum","-10");
    region.setParameter("plotter.xAxisMaximum","10");
    region.setParameter("legendRegionVisible","TRUE");

    AIDA::IFunction* fr = 
      functionFactory->createFunctionFromScript
          ("complexFunction_real",1,f_real,"","","");
    if(fr) {
      fr->annotation().addItem("Legend","real part (red)");
      fr->annotation().addItem("plotter.color","1 0 0");
      region.plot(*fr);
    }
  
    AIDA::IFunction* fi = 
      functionFactory->createFunctionFromScript
          ("complexFunction_imag",1,f_imag,"","","");
    if(fi) {
      fi->annotation().addItem("Legend","imag part (blue)");
      fi->annotation().addItem("plotter.color","0 0 1");
      region.plot(*fi);
    }
  
    AIDA::IFunction* fn = 
      functionFactory->createFunctionFromScript
          ("complexFunction_norm",1,"exp(-(x*x)/2)","","","");
    if(fn) {
      fn->annotation().addItem("Legend","norm (green)");
      fn->annotation().addItem("plotter.color","0 1 0");
      region.plot(*fn);
    }
  
    AIDA::IFunction* fa = 
      functionFactory->createFunctionFromScript
          ("complexFunction_abs_2",1,f_abs,"","","");
    if(fa) {
      fa->annotation().addItem("Legend","abs (cyan)");
      fa->annotation().addItem("plotter.color","0 1 1");
      region.plot(*fa);
    }

  }

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
    std::cout << "aida_exa_complex_and_func :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  complex_and_func(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
