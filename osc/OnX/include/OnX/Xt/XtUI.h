#ifndef OnX_XtUI_h
#define OnX_XtUI_h 

// Inheritance :
#include <OnX/Core/BaseUI.h>

#include <X11/Intrinsic.h>

#include <Xm/Xm.h>

namespace OnX {class TermCyclic;}

namespace OnX {

class XtUI : public BaseUI {
public: //Slash::UI::IUI
  virtual void* cast(const std::string&) const;
  virtual void put(const std::string&);
  virtual void synchronize();
  virtual void exit(int = 0);
  virtual bool lock();
  virtual bool unlock();
  virtual void warn(const std::string&);
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&);
  virtual Slash::UI::ICyclic* createCyclic(const std::string&,
                                int,const std::string&,const std::string&);
  virtual bool notify(Slash::Core::IMessage*);
  virtual bool enableTerminal(const std::string&,const std::string&);
  virtual bool disableTerminal();
protected: //OnX::BaseUI
  virtual int nativeSteer();
  virtual bool createNativeWidget(inlib::xml::tree&,
                            const std::string&,
                            const std::string&,
                            inlib::xml::tree*);
  virtual bool destroyNativeWidget(inlib::xml::tree&);
  virtual Action* addCallback(inlib::xml::tree&,
                           const std::string&,
                           const std::string&,
                           const std::string&);
  virtual void deleteCallback(inlib::xml::tree&,const std::string&,Action*);
  virtual std::string widgetName(inlib::xml::tree&);
  virtual bool receiveScene(const std::string&,
                            const std::string&,
                            const std::string&);

  virtual std::string ask(const std::string&);
public:
  /** 
   * @param aSession : a valid Slash::Core::ISession.
   * @param aArgs : argc, argv coming from main, needed to initialize Xt.
   */
  XtUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs);

  virtual ~XtUI();
private:
  XtUI(const XtUI&);
  XtUI& operator=(const XtUI&);
public:
  bool createTree(::Widget,inlib::xml::tree*);
  XtAppContext appContext() const;
  ::Widget appWidget() const;
private:
  static void callback(::Widget,XtPointer,XtPointer);
  static void destroyConsoleTextCallback(::Widget,XtPointer,XtPointer);
  static void destroyWarningDialogCallback(::Widget,XtPointer,XtPointer);
  static void warningCallback(::Widget,XtPointer,XtPointer);
  static void closeCallback(::Widget,XtPointer,XtPointer);
  static void destroyShellCallback(::Widget,XtPointer,XtPointer);
  static void destroyCallback(::Widget,XtPointer,XtPointer);

  static int getCompound(const std::string&);
  static WidgetClass getType(const std::string&);
  static std::string getCallbackName(WidgetClass,const std::string&);
private:
  void addCallbacks(inlib::xml::tree&,::Widget,const std::string&,const std::string&);
protected:
  ::Widget fAppWidget;
  XtAppContext fAppContext;
private:
  std::list< ::Widget > fWidgets;
  ::Widget fConsoleText;
  ::Widget fWarningDialog;
  TermCyclic* fTermCyclic;
};

}

#endif

