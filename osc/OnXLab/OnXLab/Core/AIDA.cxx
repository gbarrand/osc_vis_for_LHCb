
#include <OnXLab/Core/Main.h>

#include <inlib/args>
#include <inlib/osc_env>

#include <iostream>

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 

extern "C" {
AIDA::IAnalysisFactory* AIDA_createAnalysisFactory(){
  inlib::osc::set_env(std::cout);
  return new OnXLab::Main();
}
}

#else

extern "C" {
AIDA::IAnalysisFactory* AIDA_createAnalysisFactory(const std::string& aOptions){
  inlib::osc::set_env(std::cout);
  inlib::args args(aOptions,";",true);
  return new OnXLab::Main(args.tovector());
}
}

#endif

extern "C" {
Slash::AIDA::IAnalysisFactory* Slash_AIDA_createAnalysisFactory(const std::string& aOptions){
  inlib::args args(aOptions,";",true);
  return new OnXLab::Main(args.tovector());
}
}

#include <OnXLab/Core/PlotterMain.h>

extern "C" {

AIDA::IPlotterFactory* AIDA_createPlotterFactory(int aArgc,char** aArgv) {
  OnXLab::PlotterMain* pf = new OnXLab::PlotterMain(aArgc,aArgv);
  if(!pf->isValid()) {
    delete pf;
    return 0;
  }
  return pf;
}

// Backward compatibility :
AIDA::IPlotterFactory* OnXLab_createPlotterFactory(int aArgc,char** aArgv) {
  return AIDA_createPlotterFactory(aArgc,aArgv);
}

}


