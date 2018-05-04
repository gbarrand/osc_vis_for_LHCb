#ifndef OnX_WinUI_h
#define OnX_WinUI_h 

// Inheritance :
#include <OnX/Core/BaseUI.h>

#include <windows.h>
#include <commctrl.h>

namespace WinTk {
  class Component;
  class MessageWindow;
  class ScrolledText;
  class MenuItem;
  class CascadeButton;
  class CallbackData;
  class Tree;
  class WarningDialog;
  class Shell;
}

namespace OnX {class TermCyclic;}
namespace OnX {namespace Win {class Destroy;}}

namespace OnX {

class WinUI : public BaseUI {
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
  virtual std::string ask(const std::string&);
public: //OnX::BaseUI
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
  virtual bool enableTerminal(const std::string&,const std::string&);
  virtual bool disableTerminal();
public:
  WinUI(Slash::Core::ISession&,const std::vector<std::string>&);
  virtual ~WinUI();
  bool createTree(WinTk::Tree*,inlib::xml::tree*);
  WinTk::Component* topWidget() const;
  void setFocusedShell(WinTk::Shell*);
  WinTk::Shell* focusedShell() const;
private:
  static void callback(WinTk::Component&,WinTk::CallbackData&,void*);
  static void closeCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  static void destroyConsoleCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  static void deleteWarningDialogCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  //static void destroyCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  static void messageCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  static void windowCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  static void cascadingCallback(WinTk::Component&,WinTk::CallbackData&,void*);
  static LRESULT CALLBACK Win32WindowProc(HWND,UINT,WPARAM,LPARAM);
  //static std::string getXMLTree(HWND,HTREEITEM,int);
private:
  bool destroyOneWidget(inlib::xml::tree&);
  int getType(const std::string&);
  void addCallbacks(inlib::xml::tree&,WinTk::Component*,const std::string&);
  WinTk::MenuItem* findMenuItem(int);
  WinTk::CascadeButton* findCascadeButton(HMENU);
protected:
  WinTk::Component* fTopWidget;
private:
  std::list<WinTk::Component*> fWidgets;
  // Console panel :
  WinTk::ScrolledText* fConsoleText;
  int fActionID;
  WinTk::MessageWindow* fMessageWindow;
  WinTk::WarningDialog* fWarningDialog;
  std::list< std::pair<int,WinTk::MenuItem*> > fMenuItems;
  std::list< std::pair<HMENU,WinTk::CascadeButton*> > fCascadeButtons;
  TermCyclic* fTermCyclic;
  WinTk::Shell* fFocusedShell;
  std::list<OnX::Win::Destroy*> fDestroys;
};

}

#endif

