
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// We can see the fitter fitting !

static void animated_fit_2(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  if(mem->find("h")) mem->rm("h");
  if(mem->find("fitHistogram_h")) mem->rm("fitHistogram_h");

  //mem->ls();

  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D
      (a_aida,*mem,"h","G+BW distribution.",100,-5,5);
  if(!h) return;

 {inlib::random::gauss rg(0,1);
  for(int index=0;index<10000;index++) h->fill(rg.shoot(),1);}

 {inlib::random::bw rbw(3,0.5);
  for(int index=0;index<10000;index++) h->fill(rbw.shoot(),1);}

  // Get current plotter :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;
  plotter->createRegions(1,1,0);
  plotter->setTitle("osc-plot");

  if(a_ui) plotter->show();

  AIDA::IPlotterRegion& region = plotter->currentRegion();

  region.style().xAxisStyle().setLabel("Two random gauss");
  region.style().yAxisStyle().setLabel("Entries");

  h->annotation().addItem("plotter.modeling","top_lines");
  h->annotation().addItem("plotter.color","0 0 0");

  region.plot(*h);

  //std::string engine = "minuitc++";
  std::string engine = "midnight";
  AIDA::IFitter* fitter = amanip::create_fitter(a_aida,engine);
  if(fitter) {
    AIDA::IFunctionFactory* functionFactory = 
      a_aida.createFunctionFactory(*mem); 
    if(functionFactory) {
      AIDA::IFunction* function = 
        functionFactory->createFunctionFromScript
          ("fitHistogram_h",1,"gauss(x,H,M,W)+bw(x,A,B,C)","","","");
      if(function) {
        // Starting values :
        std::vector<double> params;
        params.push_back(300);  // H  should be around 400
        params.push_back(0);    // M  0
        params.push_back(4);    // W  1
        params.push_back(500);  // A  should be around 1200
        params.push_back(2.5);  // B  3
        params.push_back(0.2);  // C  0.5
        function->setParameters(params);
        //Ask to plot before doing the fit in order to see the fit evolution.
        region.plot(*function,"emit=render");    

        //The fit does some function.setParameters that will
        //ask to update the plotter.
        AIDA::IFitResult* fitResult = fitter->fit(*h,*function);
        delete fitResult;
      } else {
        a_out << "aida_exa_animated_fit_2 :"
              << " Can't create function."
              << std::endl;
      }
      delete functionFactory;
    }
    delete fitter;
  }

  if(a_ui) plotter->interact();

  delete plotter;
}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_animated_fit_2 :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  animated_fit_2(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
