#ifndef OnX_NS_UI_h
#define OnX_NS_UI_h 

// Inheritance :
#include <OnX/Core/BaseUI.h>

#import <Foundation/NSGeometry.h>

@class NSAutoreleasePool;
@class NSApplication;
@class NSObject;
@class NSWindow;
@class NSAlert;

@class OnX_NSTreeItem;
@class OnX_NSApplicationDelegate;
@class OnX_NSScrolledText;

namespace OnX {class TermCyclic;}

namespace OnX {

class NS_UI : public BaseUI {
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
  NS_UI(Slash::Core::ISession&,const std::vector<std::string>&);
  virtual ~NS_UI();
public:
  bool createTree(OnX_NSTreeItem*,inlib::xml::tree*);
  static void callback(Action&);
  bool findName(NSObject*,std::string&);
  void removeInPool(NSObject*,bool aReleas = true);
private:
  void addInParent(inlib::xml::tree&,NSObject*,NSObject*,const std::string&);
  void removeInParent(NSObject*,NSObject*);
  bool destroyOneWidget(inlib::xml::tree&);
  int getType(const std::string&);
  Action* addCallback(NSObject*,Slash::UI::IWidget*,
                      const std::string&,
                      const std::string&,
                      const std::string&,
                      const Action::Parameters&);
  void addCallbacks(inlib::xml::tree&,NSObject*,const std::string&);
protected:
  NSAutoreleasePool* fAutoreleasePool;
  NSApplication* fApplication;
  NSObject* fTopWidget;
private:
  std::list< std::pair<NSObject*,std::string> > fPool;
  OnX_NSApplicationDelegate* fAppDelegate;
  OnX_NSScrolledText* fConsole;
  NSWindow* fMessageWindow;
  NSAlert* fAlert;
  TermCyclic* fTermCyclic;
};

}

#endif

