#ifndef OnX_NetUI_h
#define OnX_NetUI_h 

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/IUI.h>

#include <OnX/Core/CallbackData.h>

#include <inlib/net/inet_socket>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace UI {class IScriptManager;}}

namespace OnX {

class NetUI : public virtual Slash::UI::IUI {
public: //Slash::UI::IUI :
  virtual void* cast(const std::string&) const;
  virtual bool executeScript(const std::string&,const std::string&,const Slash::UI::IUI::Options& = Slash::UI::IUI::Options());
  virtual void put(const std::string&);
  virtual int steer();
  virtual int steerLevel() const;
  virtual void synchronize();
  virtual void exit(int = 0);
  virtual void steerModal();
  virtual void exitModal();

  virtual bool lock(){return false;}
  virtual bool unlock(){return false;}
  virtual bool show(){return false;}

  virtual void echo(const std::string&);
  virtual void warn(const std::string&);
  virtual std::string ask(const std::string&);
  virtual void addDispatcher(Slash::UI::IUI::Dispatcher);
  virtual bool removeCallback(const std::string&,
                              const std::string&,
                              int = 0);
  virtual bool setCallback(const std::string&,
                           const std::string&,
                           const std::string&,
                           const std::string&,
                           int = 0);

  virtual std::string callbackValue();
  virtual const Slash::UI::ICallbackData& callbackData(){return fCallbackData;}
  virtual const std::string& thisComponent();

  virtual bool parameterValue(const std::string&,std::string&);
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool executeCallbacks(const std::string&,const std::string&);
  virtual bool load(const std::string&,const std::string&,bool = true,bool = true);
  virtual bool save(){return false;}

  virtual void deleteChildren(const std::string&);

  virtual Slash::UI::IWidget* findWidget(const std::string&) const;
  virtual Slash::UI::IWidget* findWidgetByAttribute(const std::string& atb,
                                                    const std::string& value,
                                                    bool ud = true,
                                                    bool lr = true) const;
  virtual bool load(Slash::UI::IWidget& parent,const std::string&,bool = true,bool = true);

  virtual Slash::UI::IWidget* createWidget(
                                const Slash::UI::IWidgetClass& aClass,
                                const std::string& aName,
                                Slash::UI::IWidget& aParent,
                                const std::vector<std::string>& aArgs = std::vector<std::string>());

  virtual bool destroyWidget(Slash::UI::IWidget*&){return false;}

  virtual Slash::UI::ICyclic* createCyclic(const std::string&,
                                int,const std::string&,const std::string&);

  virtual Slash::UI::ICyclic* findCyclic(const std::string&) const {return 0;}
  virtual bool removeCyclic(Slash::UI::ICyclic*&){return false;}

  virtual bool createComponent(const std::string& aName,
                               const std::string& aClass,
                               const std::string& aParent);
  virtual bool copyComponent(const std::string&,
                             const std::string&,
                             const std::string&);
  virtual std::vector<std::string> findChildren(const std::string&,
                                                const std::string&,
                                                const std::string&) const;
  virtual std::vector<std::string> findParent(const std::string& aComponent,
                                                const std::string& aWhat,
                                                const std::string& aParameter) const;
  virtual bool destroyComponent(const std::string&);
  virtual bool isComponent(const std::string&){return false;}
  virtual Slash::Core::ISession& session(){return fSession;}

  virtual bool finalize(){return true;}

  virtual bool beingDestroyed() const {return fBeingDestroyed;}
  virtual bool isValid() const {return fIsValid;}

  virtual bool setCallbackData(const Slash::UI::ICallbackData&){return true;}

  virtual std::string createWidgetName(){return "";}

  virtual Slash::UI::IWidgetClass* findWidgetClass(const std::string&) const;

  virtual Slash::UI::IWidget* currentWidget() const {return fCurrentWidget;}
  virtual bool setCurrentWidget(Slash::UI::IWidget*){return false;} 

  virtual bool enableTerminal(const std::string& aPrompt,const std::string& aInterp){return false;}
  virtual bool disableTerminal(){return false;}
  virtual bool redirectAskToTerminal(bool){return false;} 


  virtual bool lockScreen(const std::string& passwd,
                          const std::string& interp = "",
                          const std::string& script = "") {return false;} 

  virtual bool atat(const std::string&,std::string&) const;

  virtual bool show(const std::string&){return false;}
public: //Lib::IAuditor
  virtual bool notify(Slash::Core::IMessage*){return false;}
public:
  NetUI(Slash::Core::ISession&,const std::vector<std::string>&);
  virtual ~NetUI();
public:
  std::ostream& out() const;
private:
  Slash::Core::ISession& fSession;
  Slash::UI::IScriptManager* fScriptManager;
  int fVerboseLevel;
  bool fIsValid;
  inlib::net::inet_socket fClient;
  std::vector<Slash::UI::IWidget*> fWidgets;
  Slash::UI::IWidget* fCurrentWidget;
  bool fBeingDestroyed;

  std::string fValue;
  CallbackData fCallbackData;
  bool fToExit;
  bool fEnableOutput;
};

}

#endif

