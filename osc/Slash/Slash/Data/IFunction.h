#ifndef Slash_Data_IFunction_h
#define Slash_Data_IFunction_h

#include <string>
#include <vector>

namespace Slash {namespace Core {class IValue;}}

namespace Slash {

namespace Data {

class IFunction {
public:
  virtual ~IFunction() {};
public:
  virtual std::string file() const = 0;
  virtual std::string model() const = 0;
  virtual void* address() const = 0;
  //virtual std::string signature() const = 0;
  virtual unsigned int dimension() const = 0;
  virtual unsigned int numberOfParameters() const = 0;
  virtual bool value(const std::vector<double>&,double&) const = 0;
  virtual bool value(const std::vector<Slash::Core::IValue*>&,
                     Slash::Core::IValue&) const = 0;

  virtual std::vector<Slash::Core::IValue*> signature(Slash::Core::IValue*&) const = 0;
};

} //Data

} //Slash

#endif
