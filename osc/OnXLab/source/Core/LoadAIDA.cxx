
#include <AIDA/IAnalysisFactory.h> //For AIDA_createAnalysisFactory.

#include <AIDA/IConstants.h> //For AIDA_VERSION

namespace AIDA {class IPlotterFactory;}
extern "C" {
  AIDA::IPlotterFactory* AIDA_createPlotterFactory(int,char**);
}

// Loadable entry points :

extern "C" {
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  AIDA::IAnalysisFactory* OnXLabLoadAIDA_createAnalysisFactory() {
    return AIDA_createAnalysisFactory();
  }
#else
  AIDA::IAnalysisFactory* OnXLabLoadAIDA_createAnalysisFactory(const std::string& aOptions) {
    return AIDA_createAnalysisFactory(aOptions);
  }
#endif

  AIDA::IPlotterFactory* OnXLabLoadAIDA_createPlotterFactory(int aArgc,char** aArgv) {
    return AIDA_createPlotterFactory(aArgc,aArgv);
  }

}
