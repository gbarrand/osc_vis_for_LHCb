#ifndef OnXLab_AnalysisFactory_h
#define OnXLab_AnalysisFactory_h

// Inheritance :
#include <BatchLab/Core/AnalysisFactory.h>

#include <OnXLab/Core/TreeFactory.h>
#include <OnXLab/Core/PlotterFactory.h>

namespace OnXLab {

class AnalysisFactory : public BatchLab::AnalysisFactory {
public: //AIDA/IAnalysisFactory
  virtual AIDA::ITreeFactory* createTreeFactory(
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    const std::string& //aOptions
#endif
  ){
    return new OnXLab::TreeFactory(fSession);
  }

  virtual AIDA::IPlotterFactory* createPlotterFactory(int aArgc = 0,char** aArgv = 0,const std::string& aImplementation = "",const std::string& aLibrary = ""
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
,const std::string& = ""//aOptions
#endif
  ){
    if( (aImplementation.size()) && (aLibrary.size()) ) {
      return BatchLab::AnalysisFactory::createPlotterFactory
               (aArgc,aArgv,aImplementation,aLibrary);
    } else {
      return new OnXLab::PlotterFactory
        (fSession,aImplementation,aLibrary,aArgc,aArgv);
    }
  }

public: // Slash::Core::IManager :
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnXLab::AnalysisFactory)
    else return BatchLab::AnalysisFactory::cast(a_class);
  }

public:
  AnalysisFactory(Slash::Core::ISession& aSession)
  :BatchLab::AnalysisFactory(aSession){
    Lib::Debug::increment("OnXLab::AnalysisFactory");
  }
  virtual ~AnalysisFactory() {
    Lib::Debug::decrement("OnXLab::AnalysisFactory");
  }
};

}

#endif
