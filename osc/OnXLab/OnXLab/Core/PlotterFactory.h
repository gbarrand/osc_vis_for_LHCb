#ifndef OnXLab_PlotterFactory_h
#define OnXLab_PlotterFactory_h

// Inheritance :
#include <BatchLab/Core/BasePlotterFactory.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

namespace OnXLab {

class PlotterFactory : public BatchLab::BasePlotterFactory {
public: //AIDA/IPlotterFactory
  virtual AIDA::IPlotter* create(
    const std::string& aName
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOption
#endif
  ){
    return loadPlotter("OnXLabInventor","OnXLabInventor","OnXLabInventor",
                       "OnXLab_PlotterManager",aName);
  }
public:
  PlotterFactory(Slash::Core::ISession& aSession,
                 const std::string& aImplementation,
                 const std::string& aLibrary,
                 int aArgc,char** aArgv)
  :BatchLab::BasePlotterFactory(aSession,aImplementation,aLibrary,aArgc,aArgv){
    Lib::Debug::increment("OnXLab::PlotterFactory");
  }

  virtual ~PlotterFactory(){
    Lib::Debug::decrement("OnXLab::PlotterFactory");
  }

};

}

#endif
