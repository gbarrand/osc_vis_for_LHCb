
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// We can see the fitter fitting !

static void animated_fit(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  if(mem->find("h")) mem->rm("h");
  if(mem->find("fitHistogram_h")) mem->rm("fitHistogram_h");

  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D(a_aida,*mem,
      "h","The famous normal distribution.",100,-5,5);
  if(!h) return;

  inlib::random::gauss rg(0,1);
  for(int index=0;index<10000;index++) h->fill(rg.shoot(),1);

  // Get current plotter :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;
  plotter->createRegions(1,1,0);
  plotter->setTitle("osc-plot");

  if(a_ui) plotter->show();

  AIDA::IPlotterRegion& region = plotter->currentRegion();

  region.style().xAxisStyle().setLabel("Random gauss");
  region.style().yAxisStyle().setLabel("Entries");

  h->annotation().addItem("plotter.modeling","top_lines");
  h->annotation().addItem("plotter.color","0 0 0");

  region.plot(*h);

  AIDA::IFitter* fitter = amanip::create_fitter(a_aida);
  if(fitter) {
    AIDA::IFunctionFactory* functionFactory = 
      a_aida.createFunctionFactory(*mem); 
    if(functionFactory) {
      AIDA::IFunction* function = 
        functionFactory->createFunctionByName("fitHistogram_h","G");
      if(function) {
        std::vector<double> params(3);
        params[0] = 100;  //amplitude
        params[1] = 0;    //mean
        params[2] = 3;    //sigma
        function->setParameters(params);
        //Ask to plot before doing the fit in order to see the fit evolution.
        region.plot(*function,"emit=render");    

        //The fit does some function.setParameters that will
        //ask to update the plotter.
        AIDA::IFitResult* fitResult = fitter->fit(*h,*function);
        delete fitResult;
      }
      delete functionFactory;
    }
    delete fitter;
  }

  if(a_ui) {
    plotter->interact();
  }

  delete plotter;
}

int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_animated_fit :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  animated_fit(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
