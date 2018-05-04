// this :
#import <OnX/NextStep/NS_UI.h>

// Slash :
#import <Slash/Core/ISession.h>
#import <Slash/UI/IScriptManager.h>
#import <Slash/UI/IWidgetClass.h>
#import <Slash/UI/IWidget.h>
#import <Slash/Tools/Manager.h>

#import <inlib/sys/dir>
#import <inlib/system>
#import <inlib/cast>
#import <exlib/xml/parser>

#import "../Core/inlib"

#import <Lib/Messages.h>

//
#define ONX_HAS_INVENTOR

#ifdef ONX_HAS_INVENTOR
#include <OnX/Core/Widgets.h>
#else
#include <OnX/Core/Widget.h>
#endif

#import <OnX/NextStep/NSCyclic.h>
#import <OnX/NextStep/NSTk.h>
#import <OnX/NextStep/NSAction.h>
#import <OnX/NextStep/NSTools.h>

#import <AppKit/NSAlert.h>

#ifdef ONX_HAS_INVENTOR
#import <SCController.h>
#import <Slash/UI/ISoViewer.h>
#import <Slash/Tools/RegionSceneHandler.h>
#import <OnX/Inventor/Inventor.h>
#import <OnX/Inventor/HEPVis.h>
#import <OnX/NextStep/NSPageViewer.h>
#import <OnX/NextStep/NSSoExaminerViewer.h>
#import "viewer_proxy.mm"
#else
#import <Cocoa/Cocoa.h>
#endif

// Handle some classID to avoid too much string comparison in createWidget.
#define nextstep_Window                1
#define nextstep_CommandLine           2
#define nextstep_PushButton            3
#define nextstep_VBox                  4
#define nextstep_HBox                  5
#define nextstep_MenuBar               6
#define nextstep_CascadeButton         7
#define nextstep_Menu                  8
#define nextstep_MenuItem              9
#define nextstep_ScrolledTree         10
#define nextstep_FileSelectionDialog  11
#define nextstep_ScrolledList         12
#define nextstep_ScrolledText         13
#define nextstep_VPaned               14
#define nextstep_HPaned               15
#define nextstep_ToggleMenuItem       16
#define nextstep_ColorSelectionDialog 17
#define nextstep_Label                18
#define nextstep_Dialog               19
#define nextstep_VContainer           20
#define nextstep_HContainer           21
#define nextstep_Entry                22
#define nextstep_ComboBox             23
#define nextstep_VScrollBar           24
#define nextstep_HScrollBar           25
#define nextstep_ToolBar              26
#define nextstep_ToolBarItem          27
#define nextstep_MenuSeparator        28
#define nextstep_ProgressBar          29
#define nextstep_TabStack             30
#define nextstep_OptionMenu           31
#define nextstep_Password             32
#define nextstep_VScale               33
#define nextstep_HScale               34
#define nextstep_OpenGLArea           35
#define nextstep_ToggleButton         36
#define nextstep_WarningDialog        37

#define nextstep_SoExaminerViewer 50
#define nextstep_SoPlaneViewer    51
#define nextstep_SoFlyViewer      52
#define nextstep_SoWalkViewer     53
#define nextstep_PageViewer       54
#define nextstep_PlanePageViewer  55

#define cString UTF8String

@interface OnX_NSCallback : OnX_NSAction {
  OnX::Action* fAction;
}
- (id)initWithAction:(OnX::Action*)aAction;
- (void)dealloc;
- (void)execute;
@end
@implementation OnX_NSCallback
- (id)initWithAction:(OnX::Action*)aAction {
  fAction = 0;
  if(self = [super init]) {
    fAction = aAction;
  }
  return self;
}
- (void)dealloc {
  //NSLog(@"debug : dealloc cbk");
  [super dealloc];
}
- (void)execute {
  OnX::NS_UI::callback(*fAction);
}
@end

@interface OnX_NSMessageWindow : NSWindow {
  OnX::NS_UI* fUI;
}
- (id)initWithUI:(OnX::NS_UI*)aUI;
- (void)dealloc;
@end
@implementation OnX_NSMessageWindow
- (id)initWithUI:(OnX::NS_UI*)aUI {
  fUI = 0;
  NSRect rect;
  if(self = [super initWithContentRect:rect styleMask:0 backing:NSBackingStoreBuffered defer:NO]) {
    fUI = aUI;
  }
  return self;
}
- (void)dealloc {  
  [super dealloc];
}
- (void)sendEvent:(NSEvent*)aEvent {
  if([aEvent type]==NSApplicationDefined) {
    Slash::Core::IMessage* mess = (Slash::Core::IMessage*)[aEvent data1];
    fUI->notification(mess);
  } else {
    [super sendEvent:aEvent];
  }
}
@end

@interface OnX_NSWindow_DestroyAction : OnX_NSAction {
  OnX::NS_UI* fUI;
  inlib::xml::tree* fItemML;
}
- (id)initWithUI:(OnX::NS_UI*)aUI itemML:(inlib::xml::tree*)aItemML;
- (void)dealloc;
- (void)execute;
@end
@implementation OnX_NSWindow_DestroyAction
- (id)initWithUI:(OnX::NS_UI*)aUI itemML:(inlib::xml::tree*)aItemML {
  fUI = 0;
  fItemML = 0;
  if(self = [super init]) {
    fUI = aUI;
    fItemML = aItemML;
  }
  return self;
}
- (void)dealloc {
  //NSLog(@"debug : OnX_NSWindow_DestroyAction : dealloc\n");
  [super dealloc];
}
- (void)execute {
  if(!fItemML) return;
  //NSLog(@"debug : OnX_NSWindow_DestroyAction : execute\n");
  inlib::xml::tree* top = fUI->topItemML();
  if(top && fUI->is_in(*top,fItemML)) {
    OnX::Execute tag(*fUI,"destroy");
    fItemML->post_execute_backward(OnX::BaseUI::executeItemMLCallbacks,&tag);

    NSObject* widget = (NSObject*)fItemML->get_data1();

    fUI->removeInRelations(*fItemML);
    fItemML->invalidate();

    inlib::xml::tree* item = fItemML;
    inlib::xml::tree* parent = item->parent();
    if(parent) parent->remove_child(item,true);

    fUI->removeInPool(widget,false);

  }
  fItemML = 0;
}
@end

@interface OnX_NSPageViewer_DestroyAction : OnX_NSAction {
  OnX::NS_UI* fUI;
  NSString* fViewer;
}
- (id)initWithUI:(OnX::NS_UI*)aUI andViewer:(NSString*)aViewer;
- (void)dealloc;
- (void)execute;
@end
@implementation OnX_NSPageViewer_DestroyAction
- (id)initWithUI:(OnX::NS_UI*)aUI andViewer:(NSString*)aViewer {
  fUI = 0;
  fViewer = 0;
  if(self = [super init]) {
    fUI = aUI;
    fViewer = [[NSString alloc] initWithString:aViewer];
  }
  return self;
}
- (void)dealloc {
  //NSLog(@"debug : dealloc cbk");
  [fViewer release];
  [super dealloc];
}
- (void)execute {
}
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
@interface OnX_NSApplicationDelegate : NSObject {
#else
@interface OnX_NSApplicationDelegate : NSObject<NSApplicationDelegate> {
#endif
  OnX::BaseUI* fBaseUI;
}
- (id)initWithBaseUI:(OnX::BaseUI*)aBaseUI;
- (void)dealloc;
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)aSender;
@end
@implementation OnX_NSApplicationDelegate
- (id)initWithBaseUI:(OnX::BaseUI*)aBaseUI {
  fBaseUI = 0;
  if(self = [super init]) {
    fBaseUI = aBaseUI;
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)aSender {
  //NSLog(@"debug : delegate terminate\n");
  // Remove .log_tmp, .err, .out files :
  Slash::Core::ISession& session = fBaseUI->session();
  Slash::UI::IScriptManager* scriptManager = 
    Slash_findManager(session,"ScriptManager",Slash::UI::IScriptManager);
  if(scriptManager) {
    scriptManager->recordExecutedScripts(false);
  }
  session.redirectOutput(false);
  return NSTerminateNow;
}
- (BOOL)application:(NSApplication *)app openFile:(NSString*)aFile {
  //NSLog(@"debug : open : %s\n",[aFile cString]);
  fBaseUI->openFile([aFile cString]);
  return YES;
}
@end

#include "../source/Core/Proxy.h"
bool ns_proxy(void*,const std::string&,const std::vector<std::string>&,void*&);

OnX::NS_UI::NS_UI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs)
:BaseUI(aSession)
,fAutoreleasePool(0)
,fApplication(0)
,fAppDelegate(0)
,fConsole(0)
,fMessageWindow(0)
,fAlert(0)
,fTermCyclic(0)
{
  //printf("debug : ================================== : 100\n");
  // Initialize NS (NextStep) :
  fAutoreleasePool = [[NSAutoreleasePool alloc] init];
  fApplication = [NSApplication sharedApplication];  
  // fApplication is created with a class factory.
  // According NextStep conventions, it is then autoreleased, 
  // we do not have to release it at the end ; this will be done   
  // by the pool.

  fAppDelegate = [[OnX_NSApplicationDelegate alloc] initWithBaseUI:this];
  [fApplication setDelegate:fAppDelegate];

  // Get application name from aArgv[0] or from 
  // the name of the main .onx file :
  std::string app = "OnX";
  if(aArgs.size()>=1) {
    std::string path,name,suffix;
    if(inlib::path_name_suffix(aArgs[0],path,name,suffix)) {
      app = suffix!="" ? name.substr(0,name.size()-(suffix.size()+1)) : name;
    }
  }

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::NS_UI :"
        << " application name is " << inlib::sout(app) << "."
        << std::endl;
  }

  NSString* nsapp = [NSString stringWithUTF8String:app.c_str()];
  NSImage* image = [NSImage imageNamed:nsapp];
  [fApplication setApplicationIconImage:image];

  // Could have the menubar done in a .nib with InterfaceBuilder.
  BOOL status = [NSBundle loadNibNamed:nsapp owner:fApplication];
  if(status==NO) {
    //No .nib found. We have to create a main menu.

    //std::ostream& out = fSession.cout();
    //out << "OnX::NS_UI::NS_UI :"
    //    << " " << app << ".nib not found."
    //    << std::endl;
  }
  [nsapp release];

  if(![fApplication mainMenu]) {
    //NOTE : no mainMenu, then the Apple application
    //       pdm is empty. Then the .onx file should
    //       have a way to exit properly.

    NSMenu* menu = [[NSMenu alloc] init];
    [fApplication setMainMenu:menu];
    [menu release];

    //FIXME : set by programm an Apple application pdm.
    // See SDLMain.m...
  }

  // Message window :
  fMessageWindow = [[OnX_NSMessageWindow alloc] initWithUI:this];

#ifdef ONX_HAS_INVENTOR
  // Initialize Inventor :
  [SCController initCoin];
  HEPVis::initClasses();
  OnX::Inventor::initialize(aSession);
  if(!aSession.findManager(Slash_SceneHandler))  {
    aSession.addManager
      (new Slash::RegionSceneHandler(aSession,Slash_SceneHandler));
  }
#endif

  [fApplication finishLaunching];
}

OnX::NS_UI::~NS_UI() { 
  //NSLog(@"debug : ~NS_UI\n");
  fBeingDestroyed = true;

  // Treat messages emitted between the quiting of the steering
  // and here. They may come from the destruction of various
  // managers in the ~Session.
  synchronize();

  removeCyclics();

  while(!fPool.empty()) {
    std::list< std::pair<NSObject*,std::string> >::reverse_iterator it = 
      fPool.rbegin();
    NSObject* obj = (*it).first;
    fPool.pop_back();
    [obj release]; //may induce things in fPool.
  }

  //NSDate* distantPast = [NSDate distantPast];

  //synchronize(); //treat message from the close callbacks.
  //simply delete the remaining messages to have
  //a good memory balance at exit :
  while(true) {
    NSEvent* event = 
      [fApplication nextEventMatchingMask:NSApplicationDefinedMask 
                    //untilDate:distantPast
                    untilDate:nil 
                    inMode:NSDefaultRunLoopMode
                    dequeue:YES];
    if(event==nil) break;
    Slash::Core::IMessage* mess = (Slash::Core::IMessage*)[event data1];
    //notification(mess);
    delete mess;
  }

  fConsole = 0;
  [fMessageWindow release];

  [fApplication setDelegate:nil];
  [fAppDelegate release];

  [fAutoreleasePool release];

#ifdef ONX_HAS_INVENTOR
  OnX::Inventor::finalize(fSession);
#endif
}

void* OnX::NS_UI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(OnX::NS_UI)
  else return BaseUI::cast(a_class);
}

void OnX::NS_UI::put(const std::string& aString){
  // Append line at end of report region.
  if(!fConsole) {
    fConsole = (OnX_NSScrolledText*)findNativeWidget("consoleText");
  }
  if(!fConsole) return;  
  NSString* nss = [NSString stringWithUTF8String:aString.c_str()];
  // insertText positions the view at end of the text.
  [fConsole insertText:nss];
  [nss release];
}

void OnX::NS_UI::warn(const std::string& aString){
  // Modal method.
  if(!fAlert) {
    fAlert = (NSAlert*)findNativeWidget("warningDialog");
  }
  if(!fAlert) {
    echo("WARNING : "+aString);
    return;
  }
  NSString* nss = [NSString stringWithUTF8String:aString.c_str()];
  [fAlert setMessageText:nss];
  [nss release];
  [fAlert runModal];
}

int OnX::NS_UI::nativeSteer(){
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::steer..."
        << std::endl;
  }
  fSteerLevel++;

  //[fApplication run];

 {id distantFuture = [NSDate distantFuture]; //Cache this, safe.
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  while(true) {
    [pool release];
    pool = [[NSAutoreleasePool alloc] init];
    NSEvent* event = [fApplication
                      nextEventMatchingMask:NSAnyEventMask
                      untilDate:distantFuture
                      inMode:NSDefaultRunLoopMode
                      dequeue:YES];
    if(!event) continue;
    if([event type]==NSApplicationDefined) {
      Slash::Core::IMessage* mess = (Slash::Core::IMessage*)[event data1];
      if(Lib::ExitMessage* exitMess = INLIB_CAST(*mess,Lib::ExitMessage)) {
        int code = exitMess->code();
        delete mess;
        [pool release];
        fSteerLevel--;
        return code;
      } else {
        notification(mess);
      }
    } else {
      [fApplication sendEvent:event];
      [fApplication updateWindows];
    }
  };
  [pool release];}

  fSteerLevel--;
  return 0;
}

void OnX::NS_UI::synchronize(){
  //NSLog(@"debug : NS_UI::synchronize : enter...\n");
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  //NSDate* distantPast = [NSDate distantPast];
  while(true) {
    [pool release];
    pool = [[NSAutoreleasePool alloc] init];
    NSEvent* event = [fApplication
                      nextEventMatchingMask:NSAnyEventMask
                      //untilDate:distantPast
                      untilDate:nil
                      inMode:NSDefaultRunLoopMode
                      dequeue:YES];
    if(!event) {
      //NSLog(@"debug : NS_UI::synchronize : exit.\n");
      break;
    }
    //NSLog(@"UI::sync : treat event...\n");
    if([event type]==NSApplicationDefined) {
      Slash::Core::IMessage* mess = (Slash::Core::IMessage*)[event data1];
      if(Lib::ExitMessage* exitMess = INLIB_CAST(*mess,Lib::ExitMessage)) {
        delete mess;
      } else {
        notification(mess);
      }
    } else {
      [fApplication sendEvent:event];
      [fApplication updateWindows];
    }
  };
  [pool release];
}

void OnX::NS_UI::exit(int aExitCode){
  if(!fApplication) return;
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::exit..."
        << std::endl;
  }
  notify(new Lib::ExitMessage(aExitCode));
}

bool OnX::NS_UI::lock(){return false;}
bool OnX::NS_UI::unlock(){return false;}

bool OnX::NS_UI::notify(Slash::Core::IMessage* aMessage){
  if(!fMessageWindow) return false;

  //NOTE : NSInteger is a long (64 bits) on a 64 bits machine
  //       and an int (32 bits) on a 32 bits. Then it is ok
  //       in all cases to pass a pointer.
  NSInteger data1 = (NSInteger)aMessage;

  NSPoint loc;
  int num = [fMessageWindow windowNumber];
  NSEvent* event = 
    [NSEvent otherEventWithType:NSApplicationDefined 
                       location:loc  
                  modifierFlags:0 
                      timestamp:0 
                   windowNumber:num 
                        context:nil 
                        subtype:0 
                          data1:data1
                          data2:0];
  [fApplication postEvent:event atStart:NO];

  return true;
}

bool OnX::NS_UI::parameterValue(const std::string& aWhat,std::string& aValue){
  aValue = "";
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::parameterValue :"
        << " widget " << inlib::sout(aWidget)
        << " resource " << inlib::sout(aResource) << "..."
        << std::endl;
  }


  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::parameterValue :"
        << " widget " << inlib::sout(aWidget)
        << " not found." << std::endl;
    return false;
  }

  NSObject* widget = (NSObject*)itemML->get_data1();
  if(widget) {
    if(aResource=="set") {
      if([widget isKindOfClass:[OnX_NSToggleMenuItem class]]==YES) {
        int state = [(NSMenuItem*)widget state];
        aValue = state==NSOnState ? "true" : "false";
        return true;
      } else if([widget isKindOfClass:[OnX_NSToggleButton class]]==YES) {
        int state = [(NSButton*)widget state];
        aValue = state==NSOnState ? "true" : "false";
        return true;
      }
    } else if(aResource=="label") {
      if([widget isKindOfClass:[NSButton class]]==YES) {
        NSString* title = [(NSButton*)widget title];
        aValue = title ? std::string([title cString]) : "";
        return true;
      } else if([widget isKindOfClass:[OnX_NSMenuItem class]]==YES) {
        NSString* title = [(OnX_NSMenuItem*)widget title];
        aValue = title ? std::string([title cString]) : "";
        return true;
      }
    } else if(aResource=="selection") {
      if([widget isKindOfClass:[OnX_NSScrolledTree class]]==YES) {
        NSString* path = [(OnX_NSScrolledTree*)widget selectPath];
        aValue = std::string([path cString]);
        return true;
      } else if([widget isKindOfClass:[OnX_NSScrolledList class]]==YES) {
        NSString* selection = [(OnX_NSScrolledList*)widget selection];
        aValue = selection ? std::string([selection cString]) : "";
        return true;
      } else if([widget isKindOfClass:[NSTabView class]]==YES) {
        NSTabViewItem* item = [(NSTabView*)widget selectedTabViewItem];
        if(item) {
          NSString* label = [item label];
          aValue = label ? std::string([label cString]) : "";
        }
        return true;
      }
    } else if(aResource=="items") {
      if([widget isKindOfClass:[OnX_NSScrolledTree class]]==YES) {
        OnX_NSScrolledTree* tree = (OnX_NSScrolledTree*)widget;
        NSString* ns_oldXMLTree = getXML([tree rootItem]);
        std::string oldXMLTree = [ns_oldXMLTree cString];
        [ns_oldXMLTree release];
        if(oldXMLTree.size()) {
          aValue = "<tree>"+oldXMLTree+"</tree>";
        } else {
          aValue = "";
        }
        return true;
      }
    } else if(aResource=="itemCount") {
      if([widget isKindOfClass:[NSTabView class]]==YES) {
        int value  = [[(NSTabView*)widget tabViewItems] count];
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="value") {
      if([widget isKindOfClass:[OnX_NSOptionMenu class]]==YES) {
        NSString* nss = [(OnX_NSOptionMenu*)widget value];
        aValue = std::string([nss cString]);
        return true;
      } else if([widget isKindOfClass:[OnX_NSScrolledText class]]==YES) {
        NSString* value = [(OnX_NSScrolledText*)widget value];
        aValue = std::string([value cString]);
        return true;
      } else if([widget isKindOfClass:[NSTextField class]]==YES) {
        // Entry and CommandLine
        NSString* value = [(NSTextField*)widget stringValue];
        aValue = std::string([value cString]);
        return true;
      } else if([widget isKindOfClass:[NSProgressIndicator class]]==YES) {
        double value = [(NSProgressIndicator*)widget doubleValue];
        inlib::sprintf(aValue,64,"%d",(int)value);
        return true;
      } else if([widget isKindOfClass:[NSSlider class]]==YES) {
        double value = [(NSSlider*)widget doubleValue];
        inlib::sprintf(aValue,64,"%d",(int)value);
        return true;
      } else if([widget isKindOfClass:[NSScroller class]]==YES) {
        double value = [(NSScroller*)widget doubleValue];
        inlib::sprintf(aValue,64,"%d",(int)value);
        return true;
      }
    } else if(aResource=="tabLabel") { //Constraint resource.
      //Constraint resource, we have to work on the parent.
      NSView* view = 0;
      if([widget isKindOfClass:[NSView class]]==YES) {
        view = (NSView*)widget;
      } else if([widget isKindOfClass:[NSPageViewer class]]==YES) {
        view = [(NSPageViewer*)widget view];
      }
      if(view) {
        inlib::xml::tree* p = parentItemML(*itemML);
        if(p) {
          NSObject* parent = (NSObject*)p->get_data1();
          if(parent && ([parent isKindOfClass:[NSTabView class]]==YES) ) {
            NSInteger number = [(NSTabView*)parent numberOfTabViewItems];
            for(NSInteger index=0;index<number;index++) {
              NSTabViewItem* tabViewItem = 
                [(NSTabView*)parent tabViewItemAtIndex:index];
              if([tabViewItem view]==view) {
                aValue = std::string([[tabViewItem label] cString]);
                return true;
              }
            } 
          }
        }
      }
    }

  }
  // Not found, then look in XML elements :
  if(aResource=="class") {
    return itemML->attribute_value(aResource,aValue);
  }
  return itemML->element_value(aResource,aValue);
}

bool OnX::NS_UI::setParameter(const std::string& aWhat,const std::string& aValue){
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::setParameter :"
        << " widget " << inlib::sout(aWidget)
      //<< " native class " << [[widget className] cString]
        << " resource " << inlib::sout(aResource)
        << " value " << inlib::sout(aValue) << "."
        << std::endl;
  }

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::NS_UI::setParameter :"
        << " widget " << inlib::sout(aWidget)
        << " not found." << std::endl;
    return false;
  }

  NSObject* widget = (NSObject*)itemML->get_data1();

  std::string svalue;

  if(aResource=="label") {
    if([widget isKindOfClass:[NSButton class]]==YES) {
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [(NSButton*)widget setTitle:nss];
      [nss release];
      //itemML->setPropertyValue("label",aValue);
      return true;
    } else if([widget isKindOfClass:[NSMenuItem class]]==YES) {
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [(NSMenuItem*)widget setTitle:nss];
      [nss release];
      //itemML->setPropertyValue("label",aValue);
      return true;
    }
  } else if(aResource=="dirMask") {
    if([widget isKindOfClass:[OnX_NSFileChooser class]]==YES) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
      [(OnX_NSFileChooser*)widget setFilter:nss];
      [nss release];
      return true;
    }
  } else if(aResource=="directory") {
    if([widget isKindOfClass:[OnX_NSFileChooser class]]==YES) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      if(svalue==".") {
        //NSOpenPanel wants an absolute path.
        inlib::dir::pwd(svalue);
      }
      NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
      [(OnX_NSFileChooser*)widget setDirectory:nss];
      [nss release];
      return true;
    }
  } else if(aResource=="mode") {
    if([widget isKindOfClass:[OnX_NSFileChooser class]]==YES) {
      if(aValue=="save") {
        [(OnX_NSFileChooser*)widget setModeOpen:false];
      } else {
        [(OnX_NSFileChooser*)widget setModeOpen:true];
      }
      return true;
    }
  } else if(aResource=="set") {
    bool value;
    if(inlib::to(aValue,value)) {
      if([widget isKindOfClass:[OnX_NSToggleMenuItem class]]==YES) {
        [(NSMenuItem*)widget setState:value?NSOnState:NSOffState];
        return true;
      } else if([widget isKindOfClass:[OnX_NSToggleButton class]]==YES) {
        [(NSButton*)widget setState:value?NSOnState:NSOffState];
        return true;
      }
    }
  } else if(aResource=="value") {
    if([widget isKindOfClass:[OnX_NSScrolledText class]]==YES) {
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [(OnX_NSScrolledText*)widget setValue:nss];
      [nss release];
      return true;
    } else if([widget isKindOfClass:[NSTextField class]]==YES) {
      // Entry, CommandLine and Password
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [(NSTextField*)widget setStringValue:nss];
      [nss release];
      return true;
    } else if([widget isKindOfClass:[OnX_NSOptionMenu class]]==YES) {
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [(OnX_NSOptionMenu*)widget setValue:nss];
      [nss release];
      return true;
    } else if([widget isKindOfClass:[NSProgressIndicator class]]==YES) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        // [0,100]
        [(NSProgressIndicator*)widget setDoubleValue:(double)value];
        return true;
      }
      return true;
    } else if([widget isKindOfClass:[NSSlider class]]==YES) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        // [0,100]
        [(NSSlider*)widget setDoubleValue:(double)value];
        return true;
      }
      return true;
    } else if([widget isKindOfClass:[NSScroller class]]==YES) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        // [0,100]
        [(NSScroller*)widget setDoubleValue:(double)value];
        return true;
      }
      return true;
    }
  } else if(aResource=="selection") {
    if([widget isKindOfClass:[OnX_NSScrolledList class]]==YES) {
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [(OnX_NSScrolledList*)widget setSelection:nss];
      [nss release];
      return true;
    // } else  if([widget isKindOfClass:[OnX_NSScrolledTree class]]==YES) {
    } else if([widget isKindOfClass:[NSTabView class]]==YES) {
      NSInteger number = [(NSTabView*)widget numberOfTabViewItems];
      for(NSInteger index=0;index<number;index++) {
        NSTabViewItem* item = [(NSTabView*)widget tabViewItemAtIndex:index];
        std::string label = std::string([[item label] cString]);
        if(label==aValue) {
          [(NSTabView*)widget selectTabViewItem:item];
          return true;
        }
      } 
    }
/* FIXME
    if(widget->type()=="ScrolledList") {
      if(((NSTk::ScrolledList*)widget)->setSelection(aValue)) return true;
    } else if(widget->type()=="Tree") {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      if(((NSTk::Tree*)widget)->setSelection(items)) return true;
*/

  } else if(aResource=="items") {
    if([widget isKindOfClass:[OnX_NSOptionMenu class]]==YES) {
      OnX_NSOptionMenu* optionMenu = (OnX_NSOptionMenu*)widget;
      [optionMenu removeItems];
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      for(unsigned int index=0;index<items.size();index++) {
        NSString* nss = 
          [NSString stringWithUTF8String:items[index].c_str()];
        [optionMenu addItem:nss];
        [nss release];
      }
      return true;
    } else if([widget isKindOfClass:[OnX_NSComboBox class]]==YES) {
      OnX_NSComboBox* comboBox = (OnX_NSComboBox*)widget;
      [comboBox removeAllItems];
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      for(unsigned int index=0;index<items.size();index++) {
        NSString* nss = 
          [NSString stringWithUTF8String:items[index].c_str()];
        [comboBox addItemWithObjectValue:nss];
        [nss release];
      }
      return true;
    } else if([widget isKindOfClass:[OnX_NSScrolledList class]]==YES) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      OnX_NSTreeItem* rootItem = [[OnX_NSTreeItem alloc] initWithParent:nil];
      for(unsigned int index=0;index<items.size();index++) {
        OnX_NSTreeItem* item = 
          [[OnX_NSTreeItem alloc] initWithParent:rootItem];
        NSString* nss = 
          [NSString stringWithUTF8String:items[index].c_str()];
        [item setLabel:nss];
        [nss release];
        [rootItem addObject:item];
        [item release];
      }
      [(OnX_NSScrolledList*)widget setRootItem:rootItem];
      return true;
    } else if([widget isKindOfClass:[OnX_NSScrolledTree class]]==YES) {
      OnX_NSScrolledTree* tree = (OnX_NSScrolledTree*)widget;

      // retrieve the old tree in a string
      NSString* ns_oldXMLTree = getXML([tree rootItem]);
      std::string oldXMLTree = [ns_oldXMLTree cString];
      [ns_oldXMLTree release];

      // retrieve the old tree selection in a string :
      int selrow = [tree selectedRow];

      // load the new Tree
      inlib::xml::default_factory factory;
      exlib::xml::parser treeML(factory,fSession.out(),fSession.verboseLevel());
      std::vector<std::string> tags;
      tags.push_back("tree");
      tags.push_back("treeItem");
      treeML.set_tags(tags);
      if(treeML.load_string(aValue)) {
        inlib::xml::tree* top = treeML.top_item();
        //top->dump();
        if(top) {
          if(oldXMLTree.size()) {
            oldXMLTree = "<tree>"+oldXMLTree+"</tree>";
            exlib::xml::parser oldTreeML
              (factory,fSession.out(),fSession.verboseLevel());
            std::vector<std::string> tags2;
            tags2.push_back("tree");
            tags2.push_back("treeItem");
            oldTreeML.set_tags(tags2);
            if(oldTreeML.load_string(oldXMLTree)) {
              const inlib::xml::tree* old = oldTreeML.top_item();
              if (old) {
                top->update_tree(*old);
              }
            }
          }
          //FIXME
          // disable the scripts of the callback to avoid recursive loop 
          // if someone has defined the same callback in the script.

          OnX_NSTreeItem* item = [[OnX_NSTreeItem alloc] initWithParent:nil];
          createTree(item,top);
          [tree setRootItem:item];

          if(selrow!=(-1)) {
            NSIndexSet* indexes = [[NSIndexSet alloc] initWithIndex:selrow];
            [tree selectRowIndexes:indexes byExtendingSelection:NO];
            [indexes release];
          }

          //FIXNME : enable callback scripts :

          return true; 
        }
      }
    }
  } else if(aResource=="default") {
    if([widget isKindOfClass:[OnX_NSComboBox class]]==YES) {
      OnX_NSComboBox* comboBox = (OnX_NSComboBox*)widget;
      NSString* nss = [NSString stringWithUTF8String:aValue.c_str()];
      [comboBox setStringValue:nss];
      [nss release];
      return true; 
    }
  } else if(aResource=="tabLabel") { //Constraint resource.
    //Constraint resource, we have to work on the parent.
    NSView* view = 0;
    if([widget isKindOfClass:[NSView class]]==YES) {
      view = (NSView*)widget;
    } else if([widget isKindOfClass:[NSPageViewer class]]==YES) {
      view = [(NSPageViewer*)widget view];
    }
    if(view) {
      inlib::xml::tree* p = parentItemML(*itemML);
      if(p) {
        NSObject* parent = (NSObject*)p->get_data1();
        if(parent && ([parent isKindOfClass:[NSTabView class]]==YES) ) {
          NSInteger number = [(NSTabView*)parent numberOfTabViewItems];
          for(NSInteger index=0;index<number;index++) {
            NSTabViewItem* tabViewItem = 
              [(NSTabView*)parent tabViewItemAtIndex:index];
            if([tabViewItem view]==view) {
              NSString* nss = 
                [NSString stringWithUTF8String:aValue.c_str()];
              [tabViewItem setLabel:nss];
              [nss release];
              return true;
            }
          } 
        }
      }
    }
  }

  std::ostream& out = fSession.cout();
  out << "OnX::NS_UI::setParameter :"
      << " unable to set parameter for : widget " << inlib::sout(aWidget)
      << " widget " << inlib::sout(aWidget)
      << " resource " << inlib::sout(aResource)
      << " value " << inlib::sout(aValue) << "."
      << std::endl;

  return false;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::NS_UI::createTree(OnX_NSTreeItem* aTreeItem,inlib::xml::tree* aItemML){
  inlib::xml::looper _for(*aItemML);
  while(inlib::xml::tree* _tree = _for.next_tree()) {
    std::string sopened;
    bool opened = false;
    if(_tree->element_value("opened",sopened)) {
      inlib::to(sopened,opened);
    }

    std::string slabel;
    _tree->element_value("label",slabel);

    OnX_NSTreeItem* item = [[OnX_NSTreeItem alloc] initWithParent:aTreeItem];
    NSString* nss = [NSString stringWithUTF8String:slabel.c_str()];
    [item setLabel:nss];
    [nss release];

    [item setExpanded:opened];

    [aTreeItem addObject:item];
    [item release];

    if(!createTree(item,_tree)) return false;
  }
  return true;
}

bool OnX::NS_UI::createNativeWidget(
 inlib::xml::tree& aItemML                      
,const std::string& aClass
,const std::string& aName
,inlib::xml::tree* aParent
){
    OnX::Widget* owidget = INLIB_CAST(aItemML,OnX::Widget);
    if(!owidget) {
      std::ostream& out = fSession.cout();
      out << "NextSteoUI::createNativeWidget :"
          << " ItemML not a OnX::Widget." 
          << aClass << std::endl;
      return false;
    }

    int nextstepClass = getType(aClass);
    if(nextstepClass==0) {
      std::ostream& out = fSession.cout();
      out << "NS_UI::createNativeWidget : unknown widget type : " 
          << aClass << std::endl;
      return false;
    }

    // Get parent :
    NSObject* nextStepParent = 0;
    int nextStepParentClass = 0;
    if(aParent) {
      //printf("debug : parent :%s|\n",parent->getValue("class"));
      nextStepParent = (NSObject*)aParent->get_data1();
      //FIXME nextStepParentClass = getType(nextStepParent->type());
    }

    std::string svalue;

    // What is changing :
    unsigned int autoResizeMask = NSViewWidthSizable | NSViewHeightSizable;

#define CHECK_PARENT \
      if(!nextStepParent) {\
        std::ostream& out = fSession.cout();\
        out << "NS_UI::createNativeWidget : " << aClass \
            << " needs a parent." << std::endl;\
        return false;\
      }  

    // Create the NextStep widget :
    NSObject* nextStepWidget = 0;
    if(nextstepClass==nextstep_Window) {
      NSRect srect = [[NSScreen mainScreen] visibleFrame];
      NSRect rect;
      float woffset = 100;
      float hoffset = 100;
      rect.origin.x = woffset;
      rect.origin.y = hoffset;
      rect.size.width = srect.size.width - 2 * woffset;
      rect.size.height = srect.size.height - hoffset;;
      if(aItemML.element_value("geometry",svalue)) {
        int x,y,w,h;
        if(!to_geometry(svalue,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << svalue << " : bad value for geometry." << std::endl;
        } else {
          rect.origin.x = x;
          // NS origin is lower-left corner, x toright, y toup.
          rect.origin.y = srect.size.height - h - y;
          rect.size.width = w;
          rect.size.height = h;
        }
      }
      aItemML.element_value("decoration",svalue);
      unsigned int mask = NSGetDecorationMask(parseDecorations(svalue));
      OnX_NSWindow* window = 
        [[OnX_NSWindow alloc] initWithContentRect:rect mask:mask];
      [window setShowsResizeIndicator:YES];
      //[window makeKeyAndOrderFront:fApplication];
      nextStepWidget = window;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_Dialog) {
      NSRect srect = [[NSScreen mainScreen] visibleFrame];
      NSRect rect;
      float woffset = 100;
      float hoffset = 100;
      rect.origin.x = woffset;
      rect.origin.y = hoffset;
      rect.size.width = srect.size.width - 2 * woffset;
      rect.size.height = srect.size.height - hoffset;;
      OnX_NSDialog* window = [[OnX_NSDialog alloc] initWithContentRect:rect];
      NSString* nss = [NSString stringWithUTF8String:"Dialog"];
      [window setTitle:nss];
      [nss release];
      nextStepWidget = window;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_WarningDialog) {
      NSAlert* alert = [[NSAlert alloc] init];
      nextStepWidget = alert;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_Label) {
      CHECK_PARENT
      bool bitmap = aItemML.element_value("pixmap",svalue);
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      NSButton* button = [[NSButton alloc] initWithFrame:rect];
      [button setBordered:NO];
      [button setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,button,aName);
      nextStepWidget = button;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_PushButton) {
      CHECK_PARENT
      bool bitmap = aItemML.element_value("pixmap",svalue);
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSButton* button = [[OnX_NSButton alloc] initWithFrame:rect];
      [button setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,button,aName);
      nextStepWidget = button;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_ToggleButton) {
      CHECK_PARENT
      bool bitmap = aItemML.element_value("pixmap",svalue);
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSToggleButton* button = [[OnX_NSToggleButton alloc] initWithFrame:rect];
      [button setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,button,aName);
      nextStepWidget = button;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_VScrollBar) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      //OnX_NSScroller* scroller = [[OnX_NSScroller alloc] initWithFrame:rect];
      OnX_NSSlider* scroller = [[OnX_NSSlider alloc] initWithFrame:rect];
      [scroller setMinValue:0];
      [scroller setMaxValue:1000];
      [scroller setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,scroller,aName);
      nextStepWidget = scroller;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_HScrollBar) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      //OnX_NSScroller* scroller = [[OnX_NSScroller alloc] initWithFrame:rect];
      OnX_NSSlider* scroller = [[OnX_NSSlider alloc] initWithFrame:rect];
      [scroller setMinValue:0];
      [scroller setMaxValue:1000];
      [scroller setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,scroller,aName);
      nextStepWidget = scroller;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_VScale) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSSlider* slider = [[OnX_NSSlider alloc] initWithFrame:rect];
      [slider setMinValue:0];
      [slider setMaxValue:1000];
      [slider setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,slider,aName);
      nextStepWidget = slider;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_HScale) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSSlider* slider = [[OnX_NSSlider alloc] initWithFrame:rect];
      //[slider rotateByAngle:-90];
      [slider setMinValue:0];
      [slider setMaxValue:1000];
      [slider setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,slider,aName);
      nextStepWidget = slider;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_CommandLine) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSCommandLine* command = 
        [[OnX_NSCommandLine alloc] initWithFrame:rect];
      [command setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,command,aName);
      nextStepWidget = command;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_Password) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSSecureTextField* command = 
        [[OnX_NSSecureTextField alloc] initWithFrame:rect];
      [command setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,command,aName);
      nextStepWidget = command;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_Entry) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSEntry* command = [[OnX_NSEntry alloc] initWithFrame:rect];
      [command setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,command,aName);
      nextStepWidget = command;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_VBox) {
      CHECK_PARENT

      std::string sexpand;
      if(!aItemML.element_value("expand",sexpand)) sexpand = "second";
      OnX_Expand expand = OnX_SECOND;
      if(sexpand=="first") expand = OnX_FIRST;
      else if(sexpand=="both") expand = OnX_BOTH;

      int sizeOfFixed = -1;
      if(aItemML.element_value("sizeOfFixed",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          sizeOfFixed = value;
        }      
      } else if(aItemML.element_value("splitPosition",svalue)) { 
        //deprecated.
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget : "
            << " splitPosition resource deprecated for VBox." 
            << " Use sizeOfFixed." 
            << std::endl;
        int value;
        if(inlib::to<int>(svalue,value)) {
          sizeOfFixed = value;
        }      
      }

      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSVBox* vbox = [[OnX_NSVBox alloc] initWithFrame:rect];
      [vbox setExpand:expand];
      [vbox setSizeOfFixed:sizeOfFixed];
      [vbox setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,vbox,aName);
      nextStepWidget = vbox;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_HBox) {
      CHECK_PARENT

      std::string sexpand;
      if(!aItemML.element_value("expand",sexpand)) sexpand = "second";
      OnX_Expand expand = OnX_SECOND;
      if(sexpand=="first") expand = OnX_FIRST;
      else if(sexpand=="both") expand = OnX_BOTH;

      int sizeOfFixed = -1;
      if(aItemML.element_value("sizeOfFixed",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          sizeOfFixed = value;
        }      
      } else if(aItemML.element_value("splitPosition",svalue)) { 
        //deprecated.
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget : "
            << " splitPosition resource deprecated for HBox." 
            << " Use sizeOfFixed." 
            << std::endl;
        int value;
        if(inlib::to<int>(svalue,value)) {
          sizeOfFixed = value;
        }      
      }

      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSHBox* hbox = [[OnX_NSHBox alloc] initWithFrame:rect];
      [hbox setExpand:expand];
      [hbox setSizeOfFixed:sizeOfFixed];
      [hbox setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,hbox,aName);
      nextStepWidget = hbox;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_VPaned) {
      CHECK_PARENT
      int splitPosition = -1;
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          splitPosition = value;
        }      
      }
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSVPaned* vpaned = [[OnX_NSVPaned alloc] initWithFrame:rect];
      [vpaned setSplitPosition:splitPosition];
      [vpaned setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,vpaned,aName);
      nextStepWidget = vpaned;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_HPaned) {
      CHECK_PARENT
      int splitPosition = -1;
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          splitPosition = value;
        }      
      }
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSHPaned* hpaned = [[OnX_NSHPaned alloc] initWithFrame:rect];
      [hpaned setSplitPosition:splitPosition];
      [hpaned setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,hpaned,aName);
      nextStepWidget = hpaned;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_VContainer) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSVContainer* vcontainer = 
        [[OnX_NSVContainer alloc] initWithFrame:rect];
      [vcontainer setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,vcontainer,aName);
      nextStepWidget = vcontainer;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_HContainer) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSHContainer* hcontainer = 
        [[OnX_NSHContainer alloc] initWithFrame:rect];
      [hcontainer setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,hcontainer,aName);
      nextStepWidget = hcontainer;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_MenuBar) {
      CHECK_PARENT
      //nextStepParent should be a NSWindow :
      if([nextStepParent isKindOfClass:[NSWindow class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSWindow." << std::endl;
        return false;
      }
      NSMenu* menuBar = [fApplication mainMenu];
      if(!menuBar) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " NSApplication has no mainMenu."
            << std::endl;
        return false;
      }
      nextStepWidget = menuBar;
    } else if(nextstepClass==nextstep_CascadeButton) {
      CHECK_PARENT
      //nextStepParent should be a NSMenu (menubar) :
      if([nextStepParent isKindOfClass:[NSMenu class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSMenu." << std::endl;
        return false;
      }
      NSMenu* menuBar = (NSMenu*)nextStepParent;
      std::string slabel;
      aItemML.element_value("label",slabel);
      NSString* nss = [NSString stringWithUTF8String:slabel.c_str()];
      NSMenuItem* casc = [[NSMenuItem alloc] 
                          initWithTitle:nss 
                          action:nil  
                          keyEquivalent:@""]; 
      [nss release];
      [menuBar addItem:casc];
      [casc release];
      nextStepWidget = casc;
    } else if(nextstepClass==nextstep_Menu) {
      CHECK_PARENT
      //nextStepParent should be a NSMenuItem (cascade).
      if([nextStepParent isKindOfClass:[NSMenuItem class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSMenuItem." << std::endl;
        return false;
      }
      NSMenuItem* casc = (NSMenuItem*)nextStepParent;
      NSMenu* pdm = [[NSMenu alloc] init];
      [pdm setTitle:[casc title]];
      [casc setSubmenu:pdm];
      [pdm release];
      nextStepWidget = pdm;
    } else if(nextstepClass==nextstep_MenuItem) {
      CHECK_PARENT
      //nextStepParent should be a NSMenu (pdm) :
      if([nextStepParent isKindOfClass:[NSMenu class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSMenu." << std::endl;
        return false;
      }
      NSMenu* pdm = (NSMenu*)nextStepParent;
      std::string slabel;
      aItemML.element_value("label",slabel);
      NSString* nss = [NSString stringWithUTF8String:slabel.c_str()];
      OnX_NSMenuItem* item = [[OnX_NSMenuItem alloc] 
                              initWithTitle:nss 
                              action:nil
                              keyEquivalent:@""]; 
      [nss release];
      [pdm addItem:item];
      [item release];
      nextStepWidget = item;
    } else if(nextstepClass==nextstep_ToggleMenuItem) {
      CHECK_PARENT
      //nextStepParent should be a NSMenu (pdm) :
      if([nextStepParent isKindOfClass:[NSMenu class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSMenu." << std::endl;
        return false;
      }
      NSMenu* pdm = (NSMenu*)nextStepParent;
      std::string slabel;
      aItemML.element_value("label",slabel);
      NSString* nss = [NSString stringWithUTF8String:slabel.c_str()];
      OnX_NSToggleMenuItem* item = [[OnX_NSToggleMenuItem alloc] 
                                    initWithTitle:nss 
                                    action:nil
                                    keyEquivalent:@""]; 
      [nss release];
      [pdm addItem:item];
      [item release];
      nextStepWidget = item;
    } else if(nextstepClass==nextstep_MenuSeparator) {
      CHECK_PARENT
      NSMenu* pdm = (NSMenu*)nextStepParent;
      NSMenuItem* item = [NSMenuItem separatorItem];
      [pdm addItem:item];
      //class created [item release];
      nextStepWidget = item;
    } else if(nextstepClass==nextstep_OpenGLArea) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSOpenGLArea* openGL = [[OnX_NSOpenGLArea alloc] initWithFrame:rect];
      [openGL setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,openGL,aName);
      nextStepWidget = openGL;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_FileSelectionDialog) {
      CHECK_PARENT
      OnX_NSFileChooser* panel = [[OnX_NSFileChooser alloc] init];
      nextStepWidget = panel;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_ColorSelectionDialog) {
      CHECK_PARENT
      OnX_NSColorChooser* panel = [[OnX_NSColorChooser alloc] init];
      nextStepWidget = panel;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_ScrolledList) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSScrolledList* list = 
        [[OnX_NSScrolledList alloc] initWithFrame:rect];
      [list setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,list,aName);
      nextStepWidget = list;
      fPool.push_back( std::pair<NSObject*,std::string>(list,aName) );
    } else if(nextstepClass==nextstep_ScrolledText) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSScrolledText* text = 
        [[OnX_NSScrolledText alloc] initWithFrame:rect];
      [text setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,text,aName);
      nextStepWidget = text;
      fPool.push_back( std::pair<NSObject*,std::string>(text,aName) );
    } else if(nextstepClass==nextstep_ScrolledTree) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSScrolledTree* tree = 
        [[OnX_NSScrolledTree alloc] initWithFrame:rect];
      [tree setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,tree,aName);
      nextStepWidget = tree;
      fPool.push_back( std::pair<NSObject*,std::string>(tree,aName) );
    } else if(nextstepClass==nextstep_ToolBar) {
      CHECK_PARENT
      //nextStepParent should be a NSWindow :
      if([nextStepParent isKindOfClass:[NSWindow class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSWindow." << std::endl;
        return false;
      }
      NSWindow* window = (NSWindow*)nextStepParent;        
      if(!window) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :" 
            << " " << aClass << " : NSWindow not found." << std::endl;
        return false;
      }  
      NSString* nss = [NSString stringWithUTF8String:aName.c_str()];
      OnX_NSToolbar* toolbar = 
        [[OnX_NSToolbar alloc] initWithIdentifier:nss];
      [nss release];
      [toolbar setDisplayMode:NSToolbarDisplayModeIconAndLabel];

      [window setToolbar:toolbar];
      nextStepWidget = toolbar;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_ToolBarItem) {
      CHECK_PARENT
      //nextStepParent should be a NSToolbar :
      if([nextStepParent isKindOfClass:[OnX_NSToolbar class]]==NO) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :"
            << " " << aClass << " : parent not a NSToolbar." << std::endl;
        return false;
      }
      OnX_NSToolbar* toolbar = (OnX_NSToolbar*)nextStepParent;

      std::string spixmap;
      aItemML.element_value("pixmap",spixmap);
      std::string shelp;
      aItemML.element_value("tooltip",shelp);
      std::string slabel;
      aItemML.element_value("label",slabel);

      std::string sspixmap;
      inlib::file_name(spixmap,sspixmap);
      //FIXME : NSImage does not understand .xpm files.
      inlib::replace(sspixmap,".xpm",".gif");

      NSString* identifier = 
        [NSString stringWithUTF8String:aName.c_str()];
      NSString* label = [NSString stringWithUTF8String:slabel.c_str()];
      NSString* pixmap = [NSString stringWithUTF8String:sspixmap.c_str()];
      NSString* toolTip = [NSString stringWithUTF8String:shelp.c_str()];

      OnX_NSToolbarItem* toolbarItem = 
        [[OnX_NSToolbarItem alloc] initWithItemIdentifier:identifier];
      [toolbarItem autorelease];

      [toolbarItem setLabel:label];

      [toolbarItem setToolTip:toolTip];

      //[toolbarItem setPaletteLabel:label];
      //FIXME : handle .xpm (.png, .jpg, .gif ok).

      NSImage* image = [[NSImage alloc] initWithContentsOfFile:pixmap];
      [toolbarItem setImage:image];
      [image release]; 

      // Put in parent :
      [toolbar addToolbarItem:toolbarItem];

      unsigned int number = [[toolbar items] count];
      [toolbar insertItemWithItemIdentifier:identifier atIndex:number];

      [identifier release];
      [label release];
      [pixmap release];
      [toolTip release];

      nextStepWidget = toolbarItem;
    } else if(nextstepClass==nextstep_ComboBox) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSComboBox* comboBox = [[OnX_NSComboBox alloc] initWithFrame:rect];
      [comboBox setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,comboBox,aName);
      nextStepWidget = comboBox;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_OptionMenu) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSOptionMenu* optionMenu = 
        [[OnX_NSOptionMenu alloc] initWithFrame:rect];
      [optionMenu setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,optionMenu,aName);
      nextStepWidget = optionMenu;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_ProgressBar) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      NSProgressIndicator* progress = 
        [[NSProgressIndicator alloc] initWithFrame:rect];
      [progress setAutoresizingMask:autoResizeMask]; //Before addInParent.
      [progress setIndeterminate:NO];
      [progress setStyle:NSProgressIndicatorBarStyle];
      addInParent(aItemML,nextStepParent,progress,aName);
      nextStepWidget = progress;
      fPool.push_back( std::pair<NSObject*,std::string>(nextStepWidget,aName) );
    } else if(nextstepClass==nextstep_TabStack) {
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      OnX_NSTabView* tabView = [[OnX_NSTabView alloc] initWithFrame:rect];
      [tabView setAutoresizingMask:autoResizeMask]; //Before addInParent.
      addInParent(aItemML,nextStepParent,tabView,aName);
      nextStepWidget = tabView;
      fPool.push_back(std::pair<NSObject*,std::string>(nextStepWidget,aName));
#ifdef ONX_HAS_INVENTOR
    } else if( (nextstepClass==nextstep_SoExaminerViewer) ||
               (nextstepClass==nextstep_SoPlaneViewer)    ||
               (nextstepClass==nextstep_SoFlyViewer)      ||
               (nextstepClass==nextstep_SoWalkViewer)     ){
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      NSSoExaminerViewer* nsSoExaminerViewer = 
        [[NSSoExaminerViewer alloc] initWithFrame:rect];
      [[nsSoExaminerViewer view] setAutoresizingMask:autoResizeMask];
      SoSeparator* sg = new SoSeparator;
      sg->ref(); //NOTE : else iv_viewer/clear crashes.
      [nsSoExaminerViewer setSceneGraph:sg];
      addInParent(aItemML,nextStepParent,[nsSoExaminerViewer view],aName);
      nextStepWidget = nsSoExaminerViewer;
      fPool.push_back(std::pair<NSObject*,std::string>(nextStepWidget,aName));

      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :" 
            << " ItemML not a OnX::SoViewer." 
            << std::endl;
        return false;
      }
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(nsSoExaminerViewer,viewer_proxy);
    } else if( (nextstepClass==nextstep_PageViewer)       ||
               (nextstepClass==nextstep_PlanePageViewer)  ){
      CHECK_PARENT
      NSRect rect = getFrame(nextStepParent);
      rect.origin.x = 0;
      rect.origin.y = 0;
      NSPageViewer* nsPageViewer = [[NSPageViewer alloc] initWithFrame:rect];
      [[nsPageViewer view] setAutoresizingMask:autoResizeMask];
      addInParent(aItemML,nextStepParent,[nsPageViewer view],aName);
      nextStepWidget = nsPageViewer;
      fPool.push_back(std::pair<NSObject*,std::string>(nextStepWidget,aName));

      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) {
        std::ostream& out = fSession.cout();
        out << "NS_UI::createNativeWidget :" 
            << " ItemML not a OnX::SoViewer." 
            << std::endl;
        return false;
      }
      SoNode* soNode = [(NSPageViewer*)nsPageViewer sceneGraph];
      soViewer->setSceneGraph(soNode);
      soViewer->setViewerProxy(nsPageViewer,viewer_proxy);
#endif
    } else {
      std::ostream& out = fSession.cout();
      out << "NS_UI::createNativeWidget :" << aClass 
          << " unknown case." << std::endl;
      return false;
    }
    
    //NOTE : NS widgets has no name. It is handled in fPool.
    aItemML.set_data(nextStepWidget,nextStepWidget,0);
    owidget->setProxy(nextStepWidget,ns_proxy);

    // Set resources :
    if(nextstepClass==nextstep_Window) {
      if(aItemML.element_value("title",svalue)) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [(NSWindow*)nextStepWidget setTitle:nss];
        [nss release];
      }
      OnX_NSWindow_DestroyAction* act = 
      [[OnX_NSWindow_DestroyAction alloc] initWithUI:this itemML:&aItemML];

      fPool.push_back( std::pair<NSObject*,std::string>(act,"destroyAction") );
      [(OnX_NSWindow*)nextStepWidget addDestroyAction:act];

    } else if(nextstepClass==nextstep_Dialog)  {
      if(aItemML.element_value("geometry",svalue)) {
        int w,h;
        if(sscanf(svalue.c_str(),"%dx%d",&w,&h)!=2) {
          std::ostream& out = fSession.cout();
          out << svalue << " : bad value for geometry." << std::endl;
        } else {
          NSWindow* window = (NSWindow*)nextStepWidget;
          NSSize size;
          size.width = w;
          size.height = h;
          [window setContentSize:size];
        }
      }
      if(aItemML.element_value("title",svalue)) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [(NSWindow*)nextStepWidget setTitle:nss];
        [nss release];
      }
    } else if(nextstepClass==nextstep_Label) {
      if(aItemML.element_value("label",svalue) ||
         aItemML.element_value("labelString",svalue) ) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [(NSButton*)nextStepWidget setTitle:nss];
        [nss release];
      }
    } else if(nextstepClass==nextstep_PushButton) {
      if(aItemML.element_value("label",svalue) ||
         aItemML.element_value("labelString",svalue) ) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [(NSButton*)nextStepWidget setTitle:nss];
        [nss release];
      }
      if(aItemML.element_value("pixmap",svalue)) {
        //FIXME ((NSTk::Button*)nextStepWidget)->setPixmap(svalue);
      }
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_ToggleButton) {
      if(aItemML.element_value("label",svalue)) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [(NSButton*)nextStepWidget setTitle:nss];
        [nss release];
      }
      if(aItemML.element_value("pixmap",svalue)) {
        std::string fileName;
        inlib::file_name(svalue,fileName);
        //FIXME
      }
      if(aItemML.element_value("set",svalue)) {
        bool value;
        if(inlib::to(svalue,value)) {
          [(NSButton*)nextStepWidget setState:value?NSOnState:NSOffState];
        }
      }
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_HScrollBar) {
      addCallbacks(aItemML,nextStepWidget,"drag");
    } else if(nextstepClass==nextstep_VScrollBar) {
      addCallbacks(aItemML,nextStepWidget,"drag");
    } else if(nextstepClass==nextstep_HScale) {
      addCallbacks(aItemML,nextStepWidget,"drag");
    } else if(nextstepClass==nextstep_VScale) {
      addCallbacks(aItemML,nextStepWidget,"drag");
    } else if(nextstepClass==nextstep_ComboBox) {
      std::string def;
      aItemML.element_value("default",def);
      OnX_NSComboBox* comboBox = (OnX_NSComboBox*)nextStepWidget;
      NSString* nss = [NSString stringWithUTF8String:def.c_str()];
      [comboBox setStringValue:nss];
      [nss release];
      if(aItemML.element_value("items",svalue)) {
        [comboBox removeAllItems];
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++) {
          NSString* nss = 
            [NSString stringWithUTF8String:items[index].c_str()];
          [comboBox addItemWithObjectValue:nss];
          [nss release];
        }
      }
      addCallbacks(aItemML,nextStepWidget,"valueChanged");
    } else if(nextstepClass==nextstep_OptionMenu) {
      OnX_NSOptionMenu* optionMenu = (OnX_NSOptionMenu*)nextStepWidget;
      if(aItemML.element_value("label",svalue)) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [optionMenu setLabel:nss];
        [nss release];
      }
      std::string def;
      aItemML.element_value("default",def); //FIXME : handle def
      if(aItemML.element_value("items",svalue)) {
        [optionMenu removeItems];
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++) {
          NSString* nss = 
            [NSString stringWithUTF8String:items[index].c_str()];
          [optionMenu addItem:nss];
          if(items[index]==def) [optionMenu setValue:nss];
          [nss release];
        }
      }
      addCallbacks(aItemML,nextStepWidget,"valueChanged");
    } else if(nextstepClass==nextstep_ScrolledList) {
      std::string def;
      aItemML.element_value("selection",def); //FIXME : handle
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        OnX_NSTreeItem* rootItem = [[OnX_NSTreeItem alloc] initWithParent:nil];
        for(unsigned int index=0;index<items.size();index++) {
          OnX_NSTreeItem* item = 
            [[OnX_NSTreeItem alloc] initWithParent:rootItem];
          NSString* nss = 
            [NSString stringWithUTF8String:items[index].c_str()];
          [item setLabel:nss];
          [nss release];
          [rootItem addObject:item];
          [item release];
        }
        [(OnX_NSScrolledList*)nextStepWidget setRootItem:rootItem];
      }
      addCallbacks(aItemML,nextStepWidget,"select");
    } else if(nextstepClass==nextstep_ScrolledText) {
      if(aItemML.element_value("value",svalue)) {
        bool as_it = false;
        std::string s;
        if(aItemML.element_atb_value("value","as_it",s)) {
          if(!inlib::to(s,as_it)) as_it = false;
        }
        if(!as_it) inlib::replace(svalue,"\\n","\n");
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        //to have cursor at end.
        [(OnX_NSScrolledText*)nextStepWidget insertText:nss];
        [nss release];
      }
    } else if(nextstepClass==nextstep_Entry) {
      if(aItemML.element_value("value",svalue)) {
        NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
        [(NSTextField*)nextStepWidget setStringValue:nss];
        [nss release];
      }
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_ScrolledTree) {
      addCallbacks(aItemML,nextStepWidget,"select");
      addCallbacks(aItemML,nextStepWidget,"selectBranch");
      addCallbacks(aItemML,nextStepWidget,"open");
      addCallbacks(aItemML,nextStepWidget,"close");
    } else if(nextstepClass==nextstep_ToolBarItem) {
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_CommandLine) {
      addCallbacks(aItemML,nextStepWidget,"activate");
      addCallbacks(aItemML,nextStepWidget,"complete");
    } else if(nextstepClass==nextstep_Password) {
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_TabStack) {
      addCallbacks(aItemML,nextStepWidget,"select");
    } else if(nextstepClass==nextstep_MenuItem) {
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_ToggleMenuItem) {
      addCallbacks(aItemML,nextStepWidget,"activate");
    } else if(nextstepClass==nextstep_OpenGLArea) {
      addCallbacks(aItemML,nextStepWidget,"paint");
      addCallbacks(aItemML,nextStepWidget,"event");

#ifdef ONX_HAS_INVENTOR
    } else if( (nextstepClass==nextstep_SoExaminerViewer) ||
               (nextstepClass==nextstep_SoPlaneViewer)    ||
               (nextstepClass==nextstep_SoFlyViewer)      ||
               (nextstepClass==nextstep_SoWalkViewer)     ||
               (nextstepClass==nextstep_PageViewer)       ||
               (nextstepClass==nextstep_PlanePageViewer)  ){

/*FIXME
      if(aItemML.element_value("popupItems",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++)
          ((NSTk::PageViewer*)nextStepWidget)->addPopupEntry(items[index]);
      }

      ((NSTk::PageViewer*)nextStepWidget)->
        setVerboseLevel(fVerboseLevel);

      addCallbacks(aItemML,nextStepWidget,"collect");
      addCallbacks(aItemML,nextStepWidget,"popup");
      addCallbacks(aItemML,nextStepWidget,"update");

*/
      NSString* nss = [NSString stringWithUTF8String:aName.c_str()];
      OnX_NSPageViewer_DestroyAction* act = 
        [[OnX_NSPageViewer_DestroyAction alloc] initWithUI:this andViewer:nss];
      fPool.push_back( std::pair<NSObject*,std::string>(act,"destroyAction") );
      [nss release];
      [(NSPageViewer*)nextStepWidget addDestroyAction:act];
#endif

    } else if(nextstepClass==nextstep_FileSelectionDialog) {
      addCallbacks(aItemML,nextStepWidget,"ok");
      addCallbacks(aItemML,nextStepWidget,"cancel");

/*FIXME
      if(aItemML.element_value("dirMask",svalue)) {
        inlib::expand_vars(svalue);
        ((NSTk::FileSelectionDialog*)nextStepWidget)->setDirMask(svalue);
      }

      if(aItemML.element_value("directory",svalue)) {
        inlib::expand_vars(svalue);
        ((NSTk::FileSelectionDialog*)nextStepWidget)->setDirectory(svalue);
      }

*/
    } else if(nextstepClass==nextstep_ColorSelectionDialog) {
      addCallbacks(aItemML,nextStepWidget,"ok");
      addCallbacks(aItemML,nextStepWidget,"cancel");
    }

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "NS_UI::createNativeWidget :" 
        << " class " << inlib::sout(aClass)
        << " name " << inlib::sout(aName)
        << " done." << std::endl;
  }

  return createChildren(aItemML);
}

bool OnX::NS_UI::destroyNativeWidget(inlib::xml::tree& aItemML){
  inlib::xml::looper _for(aItemML);
  while(inlib::xml::tree* _tree = _for.next_tree()) {
    if(!destroyNativeWidget(*_tree)) return false;
  }
  // Destroy this component :
  return destroyOneWidget(aItemML);
}

bool OnX::NS_UI::destroyOneWidget(inlib::xml::tree& aItemML){
  NSObject* widget = (NSObject*)aItemML.get_data1();
  if(widget) {
    if(fVerboseLevel) {
      std::string name;
      findName(widget,name);
      std::ostream& out = fSession.cout();
      out << "NS_UI::destroyWidget :"
          << " destroy widget with name " << inlib::sout(name) << "..." 
          << std::endl;
    }
    removeActions(widget);
    NSObject* parent = 
      aItemML.parent()?(NSObject*)aItemML.parent()->get_data1():0;
    removeInParent(parent,widget);
    removeInPool(widget);
  }
  removeInRelations(aItemML);
  aItemML.invalidate();
  return true;
}

void OnX::NS_UI::deleteCallback(
 inlib::xml::tree& /*aItemML */
,const std::string& /*aNameXML*/
,Action* /*aAction*/
){
  //FIXME : to implement.
  std::ostream& out = fSession.cout();
  out << "NS_UI::deleteCallback :"
      << " dummy."
      << std::endl;
}

OnX::Action* OnX::NS_UI::addCallback(
 NSObject* aWidget
,Slash::UI::IWidget* aIWidget
,const std::string& aName
,const std::string& aInterp
,const std::string& aScript
,const Action::Parameters& aParams
){
  Action* action = 
    new Action(*this,aIWidget,aWidget,aInterp,aScript,aParams,aName,0);
  addAction(action);

  OnX_NSCallback* cbk = [[OnX_NSCallback alloc] initWithAction:action];

  if([aWidget isKindOfClass:[NSMenuItem class]]==YES) {
    OnX_NSMenuItem* widget = (OnX_NSMenuItem*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[NSButton class]]==YES) {
    OnX_NSButton* widget = (OnX_NSButton*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSOptionMenu class]]==YES) {
    OnX_NSOptionMenu* widget = (OnX_NSOptionMenu*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[NSToolbarItem class]]==YES) {
    OnX_NSToolbarItem* widget = (OnX_NSToolbarItem*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSOpenGLArea class]]==YES) {
    OnX_NSOpenGLArea* widget = (OnX_NSOpenGLArea*)aWidget;
    if(aName=="paint") {
      [widget addPaintAction:cbk];
    } else if(aName=="event") {
      [widget addEventAction:cbk];
    }
  } else if([aWidget isKindOfClass:[OnX_NSFileChooser class]]==YES) {
    OnX_NSFileChooser* widget = (OnX_NSFileChooser*)aWidget;
    if(aName=="ok") {
      [widget addOkAction:cbk];
    } else if(aName=="cancel") {
      [widget addCancelAction:cbk];
    }
  } else if([aWidget isKindOfClass:[OnX_NSColorChooser class]]==YES) {
    OnX_NSColorChooser* widget = (OnX_NSColorChooser*)aWidget;
    if(aName=="ok") {
      [widget addOkAction:cbk];
    } else if(aName=="cancel") {
    }
  } else if([aWidget isKindOfClass:[OnX_NSEntry class]]==YES) {
    OnX_NSEntry* widget = (OnX_NSEntry*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSCommandLine class]]==YES) {
    OnX_NSCommandLine* widget = (OnX_NSCommandLine*)aWidget;
    if(aName=="activate") {
      [widget addActivateAction:cbk];
    } else if(aName=="complete") {
      [widget addCompleteAction:cbk];
    }
  } else if([aWidget isKindOfClass:[OnX_NSScrolledTree class]]==YES) {
    OnX_NSScrolledTree* widget = (OnX_NSScrolledTree*)aWidget;
    if(aName=="select") {
      [widget addSelectAction:cbk];
    } else if(aName=="selectBranch") {
      [widget addSelectBranchAction:cbk];
    } else if(aName=="open") {
      [widget addOpenAction:cbk];
    } else if(aName=="close") {
      [widget addCloseAction:cbk];
    }
  } else if([aWidget isKindOfClass:[OnX_NSScrolledList class]]==YES) {
    OnX_NSScrolledList* widget = (OnX_NSScrolledList*)aWidget;
    [widget addSelectAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSComboBox class]]==YES) {
    OnX_NSComboBox* widget = (OnX_NSComboBox*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSSecureTextField class]]==YES) {
    OnX_NSSecureTextField* widget = (OnX_NSSecureTextField*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSSlider class]]==YES) {
    OnX_NSSlider* widget = (OnX_NSSlider*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSScroller class]]==YES) {
    OnX_NSScroller* widget = (OnX_NSScroller*)aWidget;
    [widget addActivateAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSTabView class]]==YES) {
    OnX_NSTabView* widget = (OnX_NSTabView*)aWidget;
    [widget addSelectAction:cbk];
  } else if([aWidget isKindOfClass:[OnX_NSWindow class]]==YES) {
    OnX_NSWindow* widget = (OnX_NSWindow*)aWidget;
    if(aName=="destroy") {
      [widget addDestroyAction:cbk];
    }
  }

  [cbk release];
  return action;
}
OnX::Action* OnX::NS_UI::addCallback(
 inlib::xml::tree& aItemML
,const std::string& aName
,const std::string& aInterp
,const std::string& aScript
){
  NSObject* aWidget = (NSObject*)aItemML.get_data1();
  if(!aWidget) return 0;
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  Action::Parameters params;
  return addCallback(aWidget,widget,aName,aInterp,aScript,params);
}

void OnX::NS_UI::addCallbacks(
 inlib::xml::tree& aItemML
,NSObject* aWidget
,const std::string& aName
){
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  inlib::xml::looper _for(aItemML);
  while(inlib::xml::element* _elem = _for.next_element()) {
    if(aName==_elem->name()) {
      std::string sexec;
      _elem->attribute_value("exec",sexec);
      addCallback(aWidget,widget,aName,sexec,_elem->value(),
                  _elem->attributes());
      //found = true;
    }
  }
  //if(!found) {
    //  Create a callback even if no activate property given
    // (someone may want to add some action by program later).
  //}
}

int OnX::NS_UI::getType(const std::string& aName){
       if(aName=="Window")         return nextstep_Window;
  else if(aName=="VBox")           return nextstep_VBox;
  else if(aName=="HBox")           return nextstep_HBox;
  else if(aName=="VPaned")         return nextstep_VPaned;
  else if(aName=="HPaned")         return nextstep_HPaned;
  else if(aName=="MenuBar")        return nextstep_MenuBar;
  else if(aName=="CascadeButton")  return nextstep_CascadeButton;
  else if(aName=="Menu")           return nextstep_Menu;
  else if(aName=="MenuItem")       return nextstep_MenuItem;
  else if(aName=="ToggleMenuItem") return nextstep_ToggleMenuItem;
  else if(aName=="Label")          return nextstep_Label;
  else if(aName=="PushButton")     return nextstep_PushButton;
  else if(aName=="ToggleButton")   return nextstep_ToggleButton;
  else if(aName=="CommandLine")    return nextstep_CommandLine;
  else if(aName=="ScrolledList")   return nextstep_ScrolledList;
  else if(aName=="ScrolledText")   return nextstep_ScrolledText;
  else if(aName=="Dialog")         return nextstep_Dialog;
  else if(aName=="VContainer")     return nextstep_VContainer;
  else if(aName=="HContainer")     return nextstep_HContainer;
  else if(aName=="Entry")          return nextstep_Entry;
  else if(aName=="ComboBox")       return nextstep_ComboBox;
  else if(aName=="VScrollBar")     return nextstep_VScrollBar;
  else if(aName=="HScrollBar")     return nextstep_HScrollBar;
  else if(aName=="VScale")         return nextstep_VScale;
  else if(aName=="HScale")         return nextstep_HScale;
  else if(aName=="ToolBar")        return nextstep_ToolBar;
  else if(aName=="ToolBarItem")    return nextstep_ToolBarItem;
  else if(aName=="MenuSeparator")  return nextstep_MenuSeparator;
  else if(aName=="ProgressBar")    return nextstep_ProgressBar;
  else if(aName=="TabStack")       return nextstep_TabStack;
  else if(aName=="OptionMenu")     return nextstep_OptionMenu;
  else if(aName=="OpenGLArea")     return nextstep_OpenGLArea;

#ifdef ONX_HAS_INVENTOR
  else if(aName=="SoExaminerViewer")    return nextstep_SoExaminerViewer;
  else if(aName=="SoPlaneViewer")       return nextstep_SoPlaneViewer;
  else if(aName=="SoFlyViewer")         return nextstep_SoFlyViewer;
  else if(aName=="SoWalkViewer")        return nextstep_SoWalkViewer;
  else if(aName=="PageViewer")          return nextstep_PageViewer;
  else if(aName=="PlanePageViewer")     return nextstep_PlanePageViewer;
#endif

  else if(aName=="Password")       return nextstep_Password;
  else if(aName=="ScrolledTree")   return nextstep_ScrolledTree;
  else if(aName=="FileSelection")  return nextstep_FileSelectionDialog;
  else if(aName=="ColorSelection") return nextstep_ColorSelectionDialog;
  else if(aName=="WarningDialog")  return nextstep_WarningDialog;
  return 0;
}

void OnX::NS_UI::callback(Action& aAction){
  NS_UI* This = INLIB_CAST(aAction.ui(),OnX::NS_UI);
  if(!This) return;

  std::string svalue;
  std::string sevent;
  int cbkx = 0;
  int cbky = 0;


  NSObject* component = (NSObject*)aAction.nativeWidget();
  if([component isKindOfClass:[OnX_NSFileChooser class]]==YES) {
    NSString* fileName = [(OnX_NSFileChooser*)component fileName];
    svalue = std::string([fileName cString]);
  } else if([component isKindOfClass:[OnX_NSScrolledTree class]]==YES) {
    if(aAction.name()=="select") {
      NSString* path = [(OnX_NSScrolledTree*)component selectPath];
      svalue = std::string([path cString]);
    } else if(aAction.name()=="selectBranch") {
      NSString* path = [(OnX_NSScrolledTree*)component selectPath];
      svalue = std::string([path cString]);
    } else if(aAction.name()=="open") {
      NSString* path = [(OnX_NSScrolledTree*)component openPath];
      svalue = std::string([path cString]);
    } else if(aAction.name()=="close") {
      NSString* path = [(OnX_NSScrolledTree*)component closePath];
      svalue = std::string([path cString]);
    }
  } else if([component isKindOfClass:[OnX_NSScrolledList class]]==YES) {
    NSString* selection = [(OnX_NSScrolledList*)component selection];
    if(!selection) return; //expansion ; not a leaf.
    svalue = std::string([selection cString]);
  } else if([component isKindOfClass:[OnX_NSColorChooser class]]==YES) {
    NSString* selection = [(OnX_NSColorChooser*)component choosenColor];
    if(!selection) return;
    svalue = std::string([selection cString]);
  } else if([component isKindOfClass:[OnX_NSOpenGLArea class]]==YES) {
    NSEventType eventType = [(OnX_NSOpenGLArea*)component eventType];
    NSPoint location = [(OnX_NSOpenGLArea*)component eventLocation];
    if(eventType==NSLeftMouseDown) {
      sevent = "ButtonPress";
      cbkx = (int)location.x;
      cbky = (int)location.y;
    } else if(eventType==NSLeftMouseUp) {
      sevent = "ButtonRelease";
      cbkx = (int)location.x;
      cbky = (int)location.y;
    } else if(eventType==NSMouseMoved) {
      sevent = "MotionNotify";
      cbkx = (int)location.x;
      cbky = (int)location.y;
    }
  } else if([component isKindOfClass:[OnX_NSToggleMenuItem class]]==YES) {
    int state = [(NSMenuItem*)component state];
    svalue = state==NSOnState ? "true" : "false";
  } else if([component isKindOfClass:[OnX_NSToggleButton class]]==YES) {
    int state = [(NSButton*)component state];
    svalue = state==NSOnState ? "true" : "false";
  } else if([component isKindOfClass:[OnX_NSOptionMenu class]]==YES) {
    NSString* nss = [(OnX_NSOptionMenu*)component value];
    svalue = std::string([nss cString]);
  } else if([component isKindOfClass:[NSTextField class]]==YES) {
    // Entry, CommandLine and Password
    NSString* value = [(NSTextField*)component stringValue];
    svalue = std::string([value cString]);
  } else if([component isKindOfClass:[NSSecureTextField class]]==YES) {
    // Entry, CommandLine and Password
    NSString* value = [(NSTextField*)component stringValue];
    svalue = std::string([value cString]);
  } else if([component isKindOfClass:[NSComboBox class]]==YES) {
    NSString* value = [(NSTextField*)component stringValue];
    svalue = std::string([value cString]);
  } else if([component isKindOfClass:[NSSlider class]]==YES) {
    double value  = [(NSSlider*)component doubleValue];
    inlib::sprintf(svalue,64,"%d",(int)value);
  } else if([component isKindOfClass:[NSScroller class]]==YES) {
    double value  = [(NSScroller*)component doubleValue];
    inlib::sprintf(svalue,64,"%d",(int)value);
  } else if([component isKindOfClass:[NSTabView class]]==YES) {
    NSTabViewItem* item = [(NSTabView*)component selectedTabViewItem];
    if(item) {
      NSString* label = [item label];
      svalue = label ? std::string([label cString]) : "";
    }
#ifdef ONX_HAS_INVENTOR
  } else if([component isKindOfClass:[NSPageViewer class]]==YES) {
/*FIXME
    svalue = aData.value;
    cbkx = aData.x;
    cbky = aData.y;
*/
#endif
  }

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::NS_UI::callback :"
        << " value " << inlib::sout(svalue) << "."        << std::endl;
  }

  std::string name;
  This->findName(component,name);
  This->fCallbackData = 
    CallbackData(aAction.widget(),name,svalue,sevent,cbkx,cbky);

  // Execute script :
  // the action may delete widget that may do a ui.removeActions
  // that may then delete action ! We then execute the script
  // with a local copy of the action.
  OnX::Action tmp(aAction);
  tmp.execute(); //FIXME : ret value.

  This->fCallbackData = CallbackData();
}

Slash::UI::ICyclic* OnX::NS_UI::createCyclic(
 const std::string& aName
,int aDelay
,const std::string& aInterp
,const std::string& aScript
){
  Slash::UI::ICyclic* cyclic = new NSCyclic(fSession,aName,aDelay,aInterp,aScript);
  fCyclics.push_back(cyclic);
  return cyclic;
}

std::string OnX::NS_UI::widgetName(inlib::xml::tree& aItemML){
  NSObject* widget = (NSObject*)aItemML.get_data1();
  if(!widget) return "";
  std::string name;
  if(!findName(widget,name)) return "";
  return name;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::NS_UI::findName(NSObject* aObject,std::string& aName){
  std::list< std::pair<NSObject*,std::string> >::iterator it;
  for(it=fPool.begin();it!=fPool.end();++it) {
    if((*it).first==aObject) {
      aName = (*it).second;
      return true;
    }
  }
  aName = "";
  return false;
}

void OnX::NS_UI::addInParent(
 inlib::xml::tree& aItemML
,NSObject* aParent
,NSObject* aObject
,const std::string& aName
){
  if( ([aParent isKindOfClass:[NSWindow class]]==YES) &&
      ([aObject isKindOfClass:[NSView class]]==YES) ) {
    NSView* parentView = [(NSWindow*)aParent contentView];
    [parentView addSubview:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[OnX_NSVBox class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(OnX_NSVBox*)aParent addView:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[OnX_NSHBox class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(OnX_NSHBox*)aParent addView:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[OnX_NSVContainer class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(OnX_NSVContainer*)aParent addView:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[OnX_NSHContainer class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(OnX_NSHContainer*)aParent addView:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[OnX_NSVPaned class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(OnX_NSVPaned*)aParent addView:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[OnX_NSHPaned class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(OnX_NSHPaned*)aParent addView:(NSView*)aObject];
  } else if( ([aParent isKindOfClass:[NSTabView class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    std::string svalue;
    if(!aItemML.element_value("tabLabel",svalue)) svalue = aName;
    NSString* nss = [NSString stringWithUTF8String:svalue.c_str()];
    NSTabViewItem* tabViewItem = [[NSTabViewItem alloc] init];    
    [tabViewItem setView:(NSView*)aObject];
    [tabViewItem setLabel:nss];
    [(NSTabView*)aParent addTabViewItem:tabViewItem];
    [nss release];
  } else if( ([aParent isKindOfClass:[NSView class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    [(NSView*)aParent addSubview:(NSView*)aObject];
  }
}

void OnX::NS_UI::removeInParent(NSObject* aParent,NSObject* aObject){
  if([aObject isKindOfClass:[NSMenuItem class]]==YES) {
    [[(NSMenuItem*)aObject menu] removeItem:(NSMenuItem*)aObject];
  } else if( aParent && 
             ([aParent isKindOfClass:[NSTabView class]]==YES) &&
             ([aObject isKindOfClass:[NSView class]]==YES) ) {
    NSView* view = (NSView*)aObject;
    NSInteger number = [(NSTabView*)aParent numberOfTabViewItems];
    for(NSInteger index=0;index<number;index++) {
      NSTabViewItem* tabViewItem = 
        [(NSTabView*)aParent tabViewItemAtIndex:index];
      if([tabViewItem view]==view) {
        //[tabViewItem setView:0];
        [(NSTabView*)aParent removeTabViewItem:tabViewItem];
        return;
      } 
    }
  }
}

void OnX::NS_UI::removeInPool(NSObject* aObject,bool aRelease){
  std::list< std::pair<NSObject*,std::string> >::iterator it;
  for(it=fPool.begin();it!=fPool.end();) {
    if((*it).first==aObject) {
      it = fPool.erase(it);
      if(aRelease) [aObject release];
    } else {
      ++it;
    }
  }
}

#ifdef ONX_HAS_INVENTOR
#include "../Core/receive_scene.cxx"
#endif

bool OnX::NS_UI::receiveScene(
 const std::string& aWhere
,const std::string& aPlacement
,const std::string& aFile
)
{
#ifdef ONX_HAS_INVENTOR
  return receive_scene(*this,aWhere,aPlacement,aFile);
#else
  return false;
#endif
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <OnX/Core/Term.h>

namespace OnX {
class TermCyclic 
:public NSCyclic
,public Term {
public: //Slash::UI::ICyclic
  virtual void execute() { cyclicExecute(fUI,fInterpreter); }
public:
  TermCyclic(NS_UI& aUI,
             const std::string& aPrompt,
             const std::string& aInterp)
  //10 is millisec.
  :NSCyclic(aUI.session(),"onx_terminal",10,aInterp,"") 
  ,Term(aPrompt),fUI(aUI){}
  virtual ~TermCyclic() {}
public:
  virtual std::string ask(const std::string& aPrompt) {
    return Term::ask(fUI,aPrompt);
  }
private:
  Slash::UI::IUI& fUI;
};
}

bool OnX::NS_UI::enableTerminal(const std::string& aPrompt,const std::string& aInterp){
  //WARNING : it assumes that stdout is directed to the terminal !
  if(fTermCyclic) return true;
  fTermCyclic = new TermCyclic(*this,aPrompt,aInterp);
  if(!fTermCyclic->isValid()) {
    delete fTermCyclic;
    fTermCyclic = 0;
    return false;
  }
  fTermCyclic->start();
  return true;
}

bool OnX::NS_UI::disableTerminal(){
  if(!fTermCyclic) return true;
  delete fTermCyclic;
  fTermCyclic = 0;
  return true;
}

std::string OnX::NS_UI::ask(const std::string& aPrompt){
  if(fAskToTerminal && fTermCyclic) {
    return fTermCyclic->ask(aPrompt);
  } else {
    return BaseUI::ask(aPrompt);
  }
}

