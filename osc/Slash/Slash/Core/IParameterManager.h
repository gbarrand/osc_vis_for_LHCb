#ifndef Slash_Core_IParameterManager_h
#define Slash_Core_IParameterManager_h

#include <string>

namespace Slash {

namespace Core {

class IParameterManager {
public:
  virtual ~IParameterManager() {};
public:
  virtual bool setParameter(const std::string&,const std::string&) = 0;
  virtual bool parameterValue(const std::string&,std::string&) = 0;
};

} //Core

} //Slash

#endif
