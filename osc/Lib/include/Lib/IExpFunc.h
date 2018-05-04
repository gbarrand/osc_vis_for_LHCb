#ifndef Lib_IExpFunc_h
#define Lib_IExpFunc_h

#include <Lib/Value.h>

namespace Lib {

class IExpFunc {
public:
  virtual ~IExpFunc() {}
public:
  virtual std::string name() const = 0;
  virtual unsigned int numberOfArguments() const = 0;
  typedef std::vector<Lib::Value> Args;
  virtual bool eval(const Args&,Lib::Value&,std::string&) = 0;
  virtual IExpFunc* copy() const = 0;
};

}

#endif



