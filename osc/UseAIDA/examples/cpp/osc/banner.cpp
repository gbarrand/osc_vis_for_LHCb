
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>
#include <inlib/sout>
#include <inlib/smatch>

static void banner(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) {
    a_out << "aida_exa_banner :" 
          << " mem not found." 
          << std::endl;
    return;
  }

  if(mem->find("h")) mem->rm("h");

 {std::vector<std::string> objs = mem->listObjectNames();
  for(std::vector<std::string>::iterator it = objs.begin();
      it!=objs.end();++it) {
    if(inlib::match((*it),"fit*_*")) {
      a_out << "aida_exa_banner :" 
            << " remove existing object " << inlib::sout((*it))
            << std::endl;
      mem->rm(*it);
    }
  }}

  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D(a_aida,*mem,
      "h","The famous normal distribution.",100,-5,5);
  if(!h) {
    a_out << "aida_exa_banner :" 
          << " can't create histogram." 
          << std::endl;
    return;
  }

  inlib::random::gauss rg(0,1);
  for(int index=0;index<10000;index++) h->fill(rg.shoot(),1);

  // Get current plotter :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) {
    a_out << "aida_exa_banner :" 
          << " can't create plotter." 
          << std::endl;
    return;
  }
  plotter->createRegions(1,1,0);
  plotter->setTitle("osc-plot");

  AIDA::IPlotterRegion& region = plotter->currentRegion();

  region.style().xAxisStyle().setLabel("Random gauss");
  region.style().yAxisStyle().setLabel("Entries");

  region.plot(*h);

  AIDA::IFitter* fitter = amanip::create_fitter(a_aida);
  if(fitter) {
    AIDA::IFitResult* fitResult = fitter->fit(*h,"G");
    if(fitResult) {
      AIDA::IFunction& function = fitResult->fittedFunction();
      region.plot(function);
      delete fitResult;
    }
    delete fitter;
  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;
}

#include <iostream>

int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_banner :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  banner(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
