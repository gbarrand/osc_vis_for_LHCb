#ifndef Slash_Tools_AIDA_h
#define Slash_Tools_AIDA_h

// over Slash and AIDA.

#include <Slash/Tools/Manager.h>

#include <AIDA/IAnalysisFactory.h>

namespace Slash {

inline ::AIDA::IAnalysisFactory* find_AIDA(Slash::Core::ISession& aSession) {
  //NOTE : take care of the :: in front of AIDA.
  return Slash::findManager< ::AIDA::IAnalysisFactory >
    (aSession,"AnalysisFactory","AIDA::IAnalysisFactory");
}

}

#endif
