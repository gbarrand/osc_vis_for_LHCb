#ifndef Slash_AIDA_IAnalysisFactory_h
#define Slash_AIDA_IAnalysisFactory_h

// over AIDA only.

// Inheritance :
#include <AIDA/IAnalysisFactory.h>

namespace Slash {
namespace AIDA {
class IAnalysisFactory : public virtual ::AIDA::IAnalysisFactory {
public:
  virtual void* cast(const std::string&) const = 0;
  virtual ~IAnalysisFactory(){}
};
}}

extern "C" {
  Slash::AIDA::IAnalysisFactory* 
    Slash_AIDA_createAnalysisFactory(const std::string& options = "");
}

#endif
