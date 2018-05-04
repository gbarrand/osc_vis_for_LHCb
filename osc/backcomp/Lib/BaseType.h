#ifndef Lib_BaseType_h
#define Lib_BaseType_h

// For Panoramix backward compatibility.

#include <Slash/Tools/BaseAccessor.h>

#include <Slash/Tools/StreamBuffer.h>
#include <Slash/Core/IWriter.h>

namespace Lib {typedef void* Identifier;}

#include <Lib/Variable.h>

namespace Lib {

namespace Property { //for Panoramix
 enum Type {
    NONE = Slash::Data::IProperty::NONE,
    INTEGER = Slash::Data::IProperty::INTEGER,
    DOUBLE = Slash::Data::IProperty::DOUBLE,
    STRING = Slash::Data::IProperty::STRING,
    POINTER = Slash::Data::IProperty::POINTER,
    BOOLEAN = Slash::Data::IProperty::BOOLEAN,
    INT64 = Slash::Data::IProperty::INT64,
    //
    VECTOR_INTEGER = Slash::Data::IProperty::VECTOR_INTEGER,
    VECTOR_DOUBLE = Slash::Data::IProperty::VECTOR_DOUBLE,
    VECTOR_STRING = Slash::Data::IProperty::VECTOR_STRING,
    //VECTOR_POINTER = Slash::Data::IProperty::VECTOR_POINTER,
    VECTOR_BOOLEAN = Slash::Data::IProperty::VECTOR_BOOLEAN
  };
}

class BaseType : public Slash::BaseAccessor {
public: // IAccessor :
  virtual void* cast(const std::string& aClass) const {
    if (aClass=="Lib::BaseType") {
      return (void*)static_cast<const Lib::BaseType*>(this);
    } else {
      return BaseAccessor::cast(aClass);
    }
  }
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data aData,
                                         const std::string& aName,
                                         void* aTag) {
    return Lib::Variable::toValue(value(aData,aName,aTag));
  }
public:
  virtual Variable value(Identifier,const std::string&,void*) = 0;
public:
  //BaseType(std::ostream& aPrinter):BaseAccessor(aPrinter) {}
  BaseType(Slash::Core::IWriter& aPrinter)
  :BaseAccessor(f_out)
  ,fPrinter(aPrinter)
  ,fStreamBuffer(aPrinter)
  ,f_out(&fStreamBuffer)
  {}
  virtual ~BaseType() {}
protected:
  BaseType(const BaseType& aFrom)
  :BaseAccessor(aFrom)
  ,fPrinter(aFrom.fPrinter)
  ,fStreamBuffer(aFrom.fPrinter)
  ,f_out(&fStreamBuffer)
  {}
private:
  BaseType& operator=(const BaseType&){return *this;}
protected:
  void addProperty(const std::string& aName,
                   Lib::Property::Type aType,
                   int aHead = 0,const std::string& aLabel = ""){
    fProps.push_back(Slash::Property
      (aName,(Slash::Data::IProperty::Type)aType,aHead,aLabel));
  }
  Slash::Core::IWriter& printer() const {return fPrinter;}
protected:
  Slash::Core::IWriter& fPrinter;
  Slash::StreamBuffer fStreamBuffer;
  std::ostream f_out;
    
};

}

#endif
