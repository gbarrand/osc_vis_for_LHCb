// this :
#include "AccessorAccessor.h"

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Data/IIterator.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Value.h>

// Lib :
#include <Lib/Debug.h>


OnX::AccessorAccessor::AccessorAccessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.out())
,fSession(aSession)
,fName("Lib_Accessor")
{
  addProperty("name",Slash::Data::IProperty::STRING);
  //addProperty("address",Slash::Data::IProperty::POINTER);
}

OnX::AccessorAccessor::~AccessorAccessor() {}

std::string OnX::AccessorAccessor::name() const {return fName;}

namespace OnX {
class AccessorAccessorIterator : public virtual Slash::Data::IIterator {
public: //Slash::Data::IIterator
  virtual Slash::Data::IAccessor::Data object() {
    if(fIterator==fList.end()) return 0;
    return *fIterator;
  }
  virtual void next() { ++fIterator;}
  virtual void* tag() { return 0;}
public:
  AccessorAccessorIterator(const std::vector<Slash::Data::IAccessor*>& aList)
  :fList(aList) {
    fIterator = fList.begin();
    Lib::Debug::increment("OnX::AccessorAccessorIterator");
  }
  virtual ~AccessorAccessorIterator() {
    Lib::Debug::decrement("OnX::AccessorAccessorIterator");
  }
private:
  std::vector<Slash::Data::IAccessor*> fList; //and not a ref.
  std::vector<Slash::Data::IAccessor*>::const_iterator fIterator;
};
}

Slash::Data::IIterator* OnX::AccessorAccessor::iterator()  {
  // Returned object should be deleted by the receiver.
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(fSession);
  if(!accessorManager) return 0;
  return new AccessorAccessorIterator(accessorManager->accessors());
}

Slash::Core::IValue* OnX::AccessorAccessor::findValue(
 Slash::Data::IAccessor::Data aData
,const std::string& aName
,void*
){
  Slash::Data::IAccessor* obj = (Slash::Data::IAccessor*)aData;
  if(aName=="address") {
    return new Slash::Value((void*)obj);
  } else if(aName=="name") {
    return new Slash::Value(obj->name());
  } else {
    return new Slash::Value();
  }
}
