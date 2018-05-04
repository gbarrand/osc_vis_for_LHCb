#ifndef Slash_Data_IAccessor_h
#define Slash_Data_IAccessor_h

#include <string>
#include <vector>

namespace Slash {namespace Core { class IValue;}}
namespace Slash {namespace Data { class IProperty;}}
namespace Slash {namespace Data { class IIterator;}}

namespace Slash {

namespace Data {

class IAccessor {
public:
  virtual ~IAccessor() {}
  virtual void* cast(const std::string&) const = 0;
public:
  typedef void* Data;
public:
  virtual std::string name() const = 0;
  virtual unsigned int numberOfProperties() const = 0;
  virtual const IProperty* property(unsigned int) const = 0;
  virtual IIterator* iterator() = 0;  

  //WARNING : returned object must be deleted by the caller.
  virtual Core::IValue* findValue(Data,
                                  const std::string&,void*) = 0;

  virtual bool setName(const std::string&) = 0;
  virtual void setIterator(IIterator*) = 0;  
  virtual void setIteratorArguments(const std::vector<std::string>&) = 0;  
  virtual void setExecuteArguments(const std::vector<std::string>&) = 0;  
  // Default actions :
  virtual void destroy(Data,void*) = 0 ;
  virtual void set(Data,const std::string&,
                   const std::string&,void*) = 0 ;
  virtual void calculate(Data,const std::string&,
                         const std::vector<Core::IValue*>&,void*) = 0;
};

} //Data

} //Slash

#endif
