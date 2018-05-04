
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// On the fly compilation and loading of a function.

static void on_the_fly_func(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mtree = amanip::find_mem(a_aida);
  if(!mtree) {
    a_out << "aida_exa_on_the_fly_func :" 
          << " mem not found." 
          << std::endl;
    return;
  }

  mtree->mkdir("functions");
  mtree->cd("functions");

  std::string codelet = 
    "compiled:rmason_func;$OSC_HOME_DIR/Resources/AIDA/examples/data/rmason_func.cpp";
  AIDA::IFunction* function = 
    amanip::create_function(a_aida,*mtree,"rmason",codelet,codelet);

  mtree->cd("..");

  if(!function) {
    a_out << "aida_exa_on_the_fly_func :" 
        << " can't get function \"" << codelet << "\"."
        << std::endl;
    return;
  }

  // Starting values :
  std::vector<double> params;
  params.push_back(1);       // gauss_H
  params.push_back(16000);   // gauss_M
  params.push_back(1000);    // gauss_W
  params.push_back(1);       // cauchy_H
  params.push_back(20000);   // cauchy_C
  params.push_back(2*0.2);   // cauchy_W
  params.push_back(0);       // pol_0
  params.push_back(0.00001); // pol_1
  function->setParameters(params);

  // Get current plotter :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) {
    a_out << "aida_exa_on_the_fly_func :" 
        << " can't create plotter." 
        << std::endl;
    return;
  }
  plotter->createRegions(1,1,0);
  //plotter->setTitle("osc-plot");

  AIDA::IPlotterRegion& region = plotter->currentRegion();

  //region.style().xAxisStyle().setLabel("x");
  //region.style().yAxisStyle().setLabel("f(x)");

  region.setParameter("plotter.xAxisAutomated","FALSE");
  region.setParameter("plotter.xAxisMinimum","0");
  region.setParameter("plotter.xAxisMaximum","30000");

  function->annotation().addItem("domain.xAxisNumberOfSteps","100000");

  region.setParameter("plotter.yAxisAutomated","FALSE");
  region.setParameter("plotter.yAxisMinimum","0");
  region.setParameter("plotter.yAxisMaximum","3");

  region.plot(*function);

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;
}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_on_the_fly_func :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  on_the_fly_func(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
