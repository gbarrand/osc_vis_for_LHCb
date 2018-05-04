#ifndef OnX_DummyUI_h
#define OnX_DummyUI_h 

// Inheritance :
#include <Slash/UI/IUI.h>

#include <Slash/Core/ISession.h>

#include "CallbackData.h"

#include <inlib/cast>

namespace Slash {

class DummyUI : public virtual Slash::UI::IUI {
public: //Slash::UI::IUI :

  virtual int steerLevel() const {return 0;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::UI::IUI)
    else return 0;
  }

  virtual void put(const std::string& aString) {}

  virtual bool executeScript(const std::string& aInterp,
                             const std::string& aScript,
                             const Slash::UI::IUI::Options& aParams){
    return false;
  }

  virtual int steer() {return 0;}

  virtual void steerModal() {}
  virtual void exitModal() {}

  virtual void synchronize(){}

  virtual void exit(int = 0){}

  virtual void echo(const std::string& aString) {
    fSession.out() << aString << std::endl;
  }

  virtual void warn(const std::string& aString) {
    fSession.out() << "WARNING : " << aString << std::endl;
  }

  virtual std::string ask(const std::string&){return "";}

  virtual void addDispatcher(Slash::UI::IUI::Dispatcher){}

  virtual bool removeCallback(const std::string&,const std::string&,int = 0){
    return false;
  }

  virtual bool setCallback(const std::string&,const std::string&,
                           const std::string&,const std::string&,int = 0){
    return false;
  }

  virtual bool show(){return false;}

  virtual std::string callbackValue(){return "";}

  virtual const std::string& thisComponent(){return fDummy;}

  virtual Slash::UI::IWidget* findWidget(const std::string&) const {return 0;}

  virtual Slash::UI::IWidget* findWidgetByAttribute(const std::string&, 
                                                    const std::string&,
                                              bool = true,bool = true) const {
    return 0;
  }

  virtual bool load(Slash::UI::IWidget&,const std::string&,
                    bool = true,bool = true) {
    return false;
  }

  virtual Slash::UI::IWidget* createWidget(const Slash::UI::IWidgetClass&,
                                           const std::string&,
                                           Slash::UI::IWidget&,
                const std::vector<std::string>& = std::vector<std::string>()){
    return 0;
  }

  virtual bool size(const Slash::UI::IWidget&,unsigned int&,unsigned int&){
    return false;
  }

  virtual bool parameterValue(const std::string&,std::string& aValue){
    aValue = "";
    return false;
  }

  virtual bool setParameter(const std::string&,const std::string&){
    return false;
  }

  virtual bool load(const std::string&,const std::string&,bool,bool){
    return false;
  }

  virtual void deleteChildren(const std::string&){}

  virtual Slash::UI::ICyclic* createCyclic(const std::string&,int,
                                           const std::string&,
                                           const std::string&){
    return 0;
  }

  virtual bool createComponent(const std::string&,const std::string&,
                               const std::string&){
    return false;
  }

  virtual std::vector<std::string> findChildren(const std::string&,
                                                const std::string&,
                                                const std::string&) const {
    return std::vector<std::string>();
  }

  virtual bool destroyComponent(const std::string&) {return false;}

  virtual Slash::Core::ISession& session(){return fSession;}

  virtual bool executeCallbacks(const std::string&,const std::string&) {
    return false;
  }

  virtual bool notify(Slash::Core::IMessage*){return false;}

  virtual bool copyComponent(const std::string&,const std::string&,
                             const std::string&){
    return false;
  }

  virtual bool save(){return false;}

  virtual const Slash::UI::ICallbackData& callbackData(){return fCallbackData;}

  virtual Slash::UI::ICyclic* findCyclic(const std::string&) const {return 0;}

  virtual bool isComponent(const std::string&){return false;}

  virtual bool lock(){return false;}

  virtual bool unlock(){return false;}

  virtual bool finalize(){return true;}

  virtual bool beingDestroyed() const {return false;}

  virtual bool isValid() const {return true;}

  virtual bool setCallbackData(const Slash::UI::ICallbackData&) {return true;}

  virtual std::string createWidgetName(){return "";}

  virtual Slash::UI::IWidgetClass* findWidgetClass(const std::string&) const {
    return 0;
  }

  virtual Slash::UI::IWidget* currentWidget() const {return 0;}

  virtual bool setCurrentWidget(Slash::UI::IWidget*){return false;} 

  virtual bool enableTerminal(const std::string&,const std::string&){
    return false;
  }

  virtual bool disableTerminal(){return false;}

  virtual bool redirectAskToTerminal(bool aValue){return false;} 

  virtual bool destroyWidget(Slash::UI::IWidget*&){return false;}

  virtual bool removeCyclic(Slash::UI::ICyclic*&){return false;}

  virtual bool show(const std::string&){return false;}


  virtual bool lockScreen(const std::string& passwd,
                          const std::string& interp = "",
	                  const std::string& script = "") {return false;}

  virtual bool atat(const std::string&,std::string&) const {return false;}

  virtual std::vector<std::string> findParent(const std::string& aComponent,const std::string& aWhat,const std::string& aParameter) const {
    return std::vector<std::string>();
  }

public:
  inline DummyUI(Slash::Core::ISession& aSession)
  :fSession(aSession)
  {}
  virtual ~DummyUI() {}
protected:
  inline DummyUI(const DummyUI& aFrom):fSession(aFrom.fSession){}
private:
  inline DummyUI& operator=(const DummyUI&) {return *this;}
private:
  inline static void check_instantiation() {
    Slash::Core::ISession* sess = 0;
    DummyUI ui(*sess);
  }  
protected:
  Slash::Core::ISession& fSession;
private:
  CallbackData fCallbackData;
  std::string fDummy;
};

}

#endif

