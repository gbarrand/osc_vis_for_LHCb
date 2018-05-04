#ifndef OnX_BaseType_h
#define OnX_BaseType_h

// for Panoramix backward compatibility.

// Deprecated. Use OnX::BaseAccessor.

// Inheritance :
#include <Lib/BaseType.h>
#include <Slash/Data/IVisualizer.h>

namespace OnX {

class BaseType 
:public Lib::BaseType 
,public virtual Slash::Data::IVisualizer
{
public:
  virtual void* cast(const std::string& aTo) const {
    if(aTo=="OnX::BaseType") {
      return (void*)static_cast<const OnX::BaseType*>(this);
    }else if(aTo=="Slash::Data::IVisualizer") {
      return (void*)static_cast<const Slash::Data::IVisualizer*>(this);
    } else {
      return Lib::BaseType::cast(aTo);
    }
  }
public: //IVisualizer
  virtual void beginVisualize(const Slash::UI::IStyle&) {}
  virtual void beginVisualize() {}
  virtual void visualize(Slash::Data::IAccessor::Data,void*) {}
  virtual void endVisualize() {}
public:
  //BaseType(std::ostream& aPrinter):Lib::BaseType(aPrinter){}
  BaseType(Slash::Core::IWriter& aPrinter):Lib::BaseType(aPrinter){}
  virtual ~BaseType() {}
};

}

#endif
