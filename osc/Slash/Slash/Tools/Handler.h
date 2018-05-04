#ifndef Slash_Tools_Handler_h
#define Slash_Tools_Handler_h

// Inheritance :
#include <Slash/Data/IHandler.h>

#include <Slash/Data/IAccessor.h>

namespace Slash {

class Handler : public virtual Slash::Data::IHandler {
public: //IHandler
  virtual Slash::Data::IAccessor* type() const {return fAccessor;}
  virtual void* object() const {return fObject;}
  virtual void* tag() const {return fTag;}
  virtual bool isEqual(const Slash::Data::IHandler& aHandler) const {
    if(fAccessor->name()!=aHandler.type()->name()) return false;
    if(fObject!=aHandler.object()) return false;
    if(fTag!=aHandler.tag()) return false;
    return true;
  }
public:
  inline Handler(Slash::Data::IAccessor* aAccess,void* aObject,void* aTag)
  :fAccessor(aAccess)
  ,fObject(aObject)
  ,fTag(aTag)
  {
    //Debug::increment();
  }
  virtual ~Handler() {
    //Debug::decrement();
  }
public:
  inline Handler(const Handler& aFrom)
  :IHandler()
  ,fAccessor(aFrom.fAccessor)
  ,fObject(aFrom.fObject)
  ,fTag(aFrom.fTag)
  {
    //Debug::increment();
  }
  inline Handler& operator=(const Handler& aFrom) {
    fAccessor = aFrom.fAccessor;
    fObject = aFrom.fObject;
    fTag = aFrom.fTag;
    return *this;
  }
private:
  Slash::Data::IAccessor* fAccessor;
  void* fObject;
  void* fTag;
};

}

#endif
