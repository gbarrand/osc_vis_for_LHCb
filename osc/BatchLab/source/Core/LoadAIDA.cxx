
#include <AIDA/IAnalysisFactory.h> //For AIDA_createAnalysisFactory.

#include <AIDA/IConstants.h> //For AIDA_VERSION

extern "C" {

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  // Loadable entry point :
  AIDA::IAnalysisFactory* BatchLabLoadAIDA_createAnalysisFactory() {
    return AIDA_createAnalysisFactory();
  }
#else
  // Loadable entry point :
  AIDA::IAnalysisFactory* BatchLabLoadAIDA_createAnalysisFactory(
    const std::string& aOptions
  ) {
    return AIDA_createAnalysisFactory(aOptions);
  }
#endif

}
