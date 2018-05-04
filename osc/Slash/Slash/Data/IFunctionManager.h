#ifndef Slash_Data_IFunctionManager_h
#define Slash_Data_IFunctionManager_h

#include <string>

namespace Slash {namespace Data { class IFunction;}}

namespace Slash {

namespace Data {

class IFunctionManager {
public:
  virtual ~IFunctionManager() {};
public:
  virtual IFunction* findFunction(const std::string&) = 0;
  virtual bool load(const std::string&,const std::string& = "") = 0;
};

} //Core

} //Slash

#endif
