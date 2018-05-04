#ifndef OnX_CallbackData_h
#define OnX_CallbackData_h 

#include <Slash/Tools/CallbackData.h>

#include <Lib/Debug.h>

namespace OnX {

class CallbackData : public Slash::CallbackData {
public:
  inline CallbackData() {
    Lib::Debug::increment("OnX::CallbackData");
  }
  inline CallbackData(Slash::UI::IWidget* aWidget,
                      const std::string& aComponent,
                      const std::string& aValue,
                      const std::string& aEvent,
                      int aX,int aY) 
  : Slash::CallbackData(aWidget,aComponent,aValue,aEvent,aX,aY)
  {
    Lib::Debug::increment("OnX::CallbackData");
  }
  virtual ~CallbackData() {
    Lib::Debug::decrement("OnX::CallbackData");
  }
public:
  inline CallbackData(const CallbackData& aFrom) 
  : Slash::CallbackData(aFrom)
  {
    Lib::Debug::increment("OnX::CallbackData");
  }
  inline CallbackData& operator=(const CallbackData& aFrom){
    Slash::CallbackData::operator=(aFrom);
    return *this;
  }
};

}

#endif
