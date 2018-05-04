#ifndef Slash_Tools_CallbackData_h
#define Slash_Tools_CallbackData_h 

// used in DummyUI.h

#include <Slash/UI/ICallbackData.h>

namespace Slash {

class CallbackData : public virtual Slash::UI::ICallbackData {
public://Slash::UI::ICallbackData
  virtual Slash::UI::IWidget* widget() const {return fWidget;}
  virtual std::string component() const {return fComponent;}
  virtual std::string value() const {return fValue;}
  virtual std::string event() const {return fEvent;}
  virtual int x() const {return fX;}
  virtual int y() const {return fY;}
  virtual bool copy(const ICallbackData& aFrom) {
    fWidget = aFrom.widget();
    fComponent = aFrom.component();
    fValue = aFrom.value();
    fEvent = aFrom.event();
    fX = aFrom.x();
    fY = aFrom.y();
    return true;
  }
public:
  inline CallbackData() 
  :fWidget(0),fX(0),fY(0)
  {
    //Lib::Debug::increment("OnX::CallbackData");
  }
  inline CallbackData(Slash::UI::IWidget* aWidget,
                      const std::string& aComponent,
                      const std::string& aValue,
                      const std::string& aEvent,
                      int aX,int aY) 
  :fWidget(aWidget)
  ,fComponent(aComponent)
  ,fValue(aValue)
  ,fEvent(aEvent)
  ,fX(aX)
  ,fY(aY)
  {
    //Lib::Debug::increment("OnX::CallbackData");
  }

  virtual ~CallbackData(){
    //Lib::Debug::decrement("OnX::CallbackData");
  }
public:
  inline CallbackData(const CallbackData& aFrom) 
  :Slash::UI::ICallbackData(aFrom)
  ,fWidget(aFrom.fWidget)
  ,fComponent(aFrom.fComponent)
  ,fValue(aFrom.fValue)
  ,fEvent(aFrom.fEvent)
  ,fX(aFrom.fX)
  ,fY(aFrom.fY)
  {
    //Lib::Debug::increment("OnX::CallbackData");
  }
  inline CallbackData& operator=(const CallbackData& aFrom) {
    fWidget = aFrom.fWidget;
    fComponent = aFrom.fComponent;
    fValue = aFrom.fValue;
    fEvent = aFrom.fEvent;
    fX = aFrom.fX;
    fY = aFrom.fY;
    return *this;
  }
private:
  Slash::UI::IWidget* fWidget;
  std::string fComponent;
  std::string fValue;
  std::string fEvent;
  int fX;
  int fY;
};

}

#endif
