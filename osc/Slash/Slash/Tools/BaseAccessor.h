#ifndef Slash_Tools_BaseAccessor_h
#define Slash_Tools_BaseAccessor_h

#include <Slash/Data/IAccessor.h>

#include <vector>
#include <inlib/cast>

#include "Property.h"

namespace Slash {

class BaseAccessor : public virtual Slash::Data::IAccessor {
public: // IAccessor :
  virtual unsigned int numberOfProperties() const {return fProps.size();}
  virtual const Slash::Data::IProperty* property(unsigned int aIndex) const {
    if(aIndex>=fProps.size()) return 0;
    return &(fProps[aIndex]);
  }
  virtual Slash::Data::IIterator* iterator() {return 0;}
  virtual bool setName(const std::string&) {return false;}
  virtual void setIterator(Slash::Data::IIterator*) {}
  virtual void setIteratorArguments(const std::vector<std::string>& aArgs) { 
    fIteratorArgs = aArgs;
  }
  virtual void destroy(Slash::Data::IAccessor::Data,void*){}
  virtual void set(Slash::Data::IAccessor::Data,const std::string&,const std::string&,void*) {}
  virtual void calculate(Slash::Data::IAccessor::Data,const std::string&,const std::vector<Slash::Core::IValue*>&,void*) {}
  virtual void setExecuteArguments(const std::vector<std::string>& aArgs) { 
    fExecuteArgs = aArgs;
  }

  //virtual std::string name() const {
  //  Still pure virtual.
  //}
  //virtual Core::IValue* findValue(Data,const std::string&,void*) {
  //  Still pure virtual.
  //}
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::BaseAccessor)
    else INLIB_IF_CAST(Slash::Data::IAccessor)
    else return 0;
  }
public:
  inline BaseAccessor(std::ostream& a_out):f_out(a_out){
    //Lib::Debug::increment("Lib::BaseAccessor");
  }
  virtual ~BaseAccessor() {
    //Lib::Debug::decrement("Lib::BaseAccessor");
    fProps.clear();
  }
public:
  inline BaseAccessor(const BaseAccessor& aFrom)
  :f_out(aFrom.f_out)
  ,fProps(aFrom.fProps)
  ,fIteratorArgs(aFrom.fIteratorArgs)
  ,fExecuteArgs(aFrom.fExecuteArgs)
  {}
  inline BaseAccessor& operator=(const BaseAccessor& aFrom){
    fProps = aFrom.fProps;
    fIteratorArgs = aFrom.fIteratorArgs;
    fExecuteArgs = aFrom.fExecuteArgs;
    return *this;
  }
public:
  inline std::ostream& out() const {return f_out;}
  //inline std::ostream& printer() const {return f_out;} //Panoramix.
  inline const std::vector<std::string>& iteratorArguments() const {
    return fIteratorArgs;
  }
  inline const std::vector<std::string>& executeArguments() const {
    return fExecuteArgs;
  }
protected:
  inline void addProperty(const std::string& aName,Slash::Data::IProperty::Type aType,int aHead = 0,const std::string& aLabel = ""){
    fProps.push_back(Property(aName,aType,aHead,aLabel));
  }
  inline void clearProperties() {fProps.clear();}

protected:
  std::ostream& f_out;
  std::vector<Property> fProps;
  std::vector<std::string> fIteratorArgs;
  std::vector<std::string> fExecuteArgs;
};

}

#endif



