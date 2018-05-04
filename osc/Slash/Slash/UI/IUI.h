#ifndef Slash_UI_IUI_h
#define Slash_UI_IUI_h

#include <string>
#include <vector>

namespace Slash {namespace Core { class IMessage;}}
namespace Slash {namespace Core { class ISession;}}

namespace Slash {namespace UI { class IWidgetClass;}}
namespace Slash {namespace UI { class IWidget;}}
namespace Slash {namespace UI { class ICyclic;}}
namespace Slash {namespace UI { class ICallbackData;}}

namespace Slash {
namespace UI {

class IUI {
public:
  virtual ~IUI() {};
  virtual void* cast(const std::string&) const = 0;
public:
  typedef int (*Dispatcher)(void*);
  typedef std::pair<std::string,std::string> Option;
  typedef std::vector<Option> Options;
public:
  virtual bool isValid() const = 0;
  virtual bool executeScript(const std::string&,const std::string&,const Options& = Options()) = 0;
  virtual void put(const std::string&) = 0;
  virtual int steer() = 0;
  virtual int steerLevel() const = 0;
  virtual void synchronize() = 0;
  virtual void exit(int = 0) = 0;

  virtual void steerModal() = 0;
  virtual void exitModal() = 0;

  virtual bool show() = 0;

  /** 
   * If GUI in a thread, lock the thread.
   */
  virtual bool lock() = 0;

  /** 
   * If GUI in a thread, unlock the thread.
   */
  virtual bool unlock() = 0;

  virtual void echo(const std::string&) = 0;
  virtual void warn(const std::string&) = 0;
  virtual std::string ask(const std::string&) = 0;
  virtual void addDispatcher(Dispatcher) = 0;

  /** 
   * Set a component callback.
   * @param aName : Name of the component to modify.
   * @param aCallback : Callback to set (for example "activate" for a button).
   * @param aInterpreter : Interpreter to use for the callback.
   * @param aScript : Script to execute within the choosen interpreter when
   *                  the callback is activated.
   * @param aIndex : index in case of multiple callback.
   * @return The status of the execution.
   */
  virtual bool setCallback(const std::string& aName,
                           const std::string& aCallback,
                           const std::string& aInterpreter,
                           const std::string& aScript,
                           int aIndex = 0) = 0;

  virtual bool removeCallback(const std::string& aName,
                              const std::string& aCallback,
                              int aIndex = 0) = 0;
  virtual std::string callbackValue() = 0;
  virtual const ICallbackData& callbackData() = 0;
  virtual bool setCallbackData(const ICallbackData&) = 0;
  virtual bool setParameter(const std::string&,const std::string&) = 0;
  virtual bool parameterValue(const std::string&,std::string&) = 0;
  virtual bool executeCallbacks(const std::string&,const std::string&) = 0;
  virtual bool load(const std::string&,const std::string&,bool is_file = true,bool show = true) = 0;
  virtual bool save() = 0;
  virtual void deleteChildren(const std::string&) = 0;

  virtual ICyclic* createCyclic(const std::string&,
                                int,const std::string&,const std::string&) = 0;
  virtual ICyclic* findCyclic(const std::string&) const = 0;
  virtual bool removeCyclic(ICyclic*&) = 0;

  virtual std::string createWidgetName() = 0;

  //IWidget API :
  virtual IWidgetClass* findWidgetClass(const std::string&) const = 0;

  virtual IWidget* findWidget(const std::string&) const = 0;

  virtual IWidget* createWidget(const IWidgetClass& aClass,
                                const std::string& aName,
                                IWidget& aParent,
                                const std::vector<std::string>& aArgs = std::vector<std::string>()) = 0;

  virtual bool destroyWidget(IWidget*&) = 0;

  virtual Slash::UI::IWidget* findWidgetByAttribute(const std::string& atb,
                                                    const std::string& value,
                                                    bool up_down = true,
                                                    bool left_right = true) const = 0;

  virtual bool load(IWidget& parent,const std::string&,bool is_file = true,bool show = true) = 0;

  virtual IWidget* currentWidget() const = 0; 
  virtual bool setCurrentWidget(IWidget*) = 0; 

  //"by name" API :
  /** 
   * Create a new widget.
   * @param aName : Name of the created widget.
   * @param aClass : Class (or type) of the created widget.
   * @param aParent : Name of the parent widget.
   * @return True if success, false if failure.
   */
  virtual bool createComponent(const std::string& aName,
                               const std::string& aClass,
                               const std::string& aParent) = 0;

  /** 
   * Destroy a widget.
   * @param aName : Name of the widget to destroy.
   * @return True if success, false if failure.
   */
  virtual bool destroyComponent(const std::string& aName) = 0;

  /**
   * Copy a widget.
   * @param aName : Name of the created widget.
   * @param aComponent : Name of the widget to copy.
   * @param aParent : Name of the parent into which we put the copied widget.
   * @return True if success, false if failure.
   */
  virtual bool copyComponent(const std::string& aName,
                             const std::string& aComponent,
                             const std::string& aParent) = 0;

  /**
   * Return list of children of a component.
   * @param aComponent : Name of the component.
   * @param aWhat : What to search.
   * @param aParameter : searching parameter.
   * @return List of children.
   */
  virtual std::vector<std::string> findChildren(const std::string& aComponent,
                                                const std::string& aWhat,
                                                const std::string& aParameter) const = 0;

  /**
   * Return list of parents of a component.
   * @param aComponent : Name of the component.
   * @param aWhat : What to search.
   * @param aParameter : searching parameter.
   * @return List of parents.
   */
  virtual std::vector<std::string> findParent(const std::string& aComponent,
                                                const std::string& aWhat,
                                                const std::string& aParameter) const = 0;

  /** 
   * Return list of children of a component.
   * @param aName : Name of the component.
   * @return true if component exists.
   */
  virtual bool isComponent(const std::string& aName) = 0;

  /** 
   * Return the session managing the GUI.
   *
   * @return The session.
   */
  virtual Core::ISession& session() = 0;

  /** 
   * Send a message to the UI.
   * @param aMessage : data sent.
   * @return true if message sent correctly.
   */
  virtual bool notify(Core::IMessage* aMessage) = 0;

  virtual bool beingDestroyed() const = 0;

  virtual bool finalize() = 0;

  /*
   * 
   */
  virtual bool enableTerminal(const std::string& prompt,
                              const std::string& interp) = 0; 
  virtual bool disableTerminal() = 0; 
  virtual bool redirectAskToTerminal(bool) = 0; 

  virtual bool lockScreen(const std::string& passwd,
                          const std::string& interp = "",
                          const std::string& script = "") = 0; 

  virtual bool atat(const std::string&,std::string&) const = 0;

  /*
  //REMOVED : use IWidget API.
    Slash::UI::IWidget* w = ui->findWidget(<name>);
    if(w) w-><method>(<args>);

  virtual bool refresh(const std::string&) = 0;
  virtual bool size(const std::string&,unsigned int&,unsigned int&) = 0;
  virtual bool write(const std::string& widget,
                     const std::string& what,
                     const std::string& file,
                     const std::string& options = "") = 0;

  virtual void hide(const std::string& = "") = 0;
  virtual bool position(const std::string&,int&,int&) = 0;
  virtual bool size(const IWidget&,unsigned int&,unsigned int&) = 0;
  */
  //DEPRECATED :
  virtual bool show(const std::string&) = 0;

};

} //UI
} //Slash

#endif
