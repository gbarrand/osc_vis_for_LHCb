#ifdef __alpha  //So that XtNewString, that uses strlen, strcpy, compiles
#include <string.h>
#endif

// this :
#include <OnX/Xt/XtUI.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/IWidget.h>

#include "../Core/inlib"

#include <inlib/tos>
#include <inlib/system>
#include <inlib/file>
#include <inlib/pointer>
#include <inlib/cast>
#include <exlib/xml/parser>

#include <Lib/Messages.h>
#include <Lib/Debug.h>

#include <OnX/Core/Widgets.h>

#include <OnX/Xt/XtCyclic.h>
#include <OnX/Xt/XtTools.h>

#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/keysym.h>

#include <Xm/Xm.h>
#include <Xm/Command.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeB.h>
#include <Xm/Text.h>
#include <Xm/FileSB.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include <Xm/List.h>
#include <Xm/PanedW.h>
#include <Xm/Label.h>
#include <Xm/ScrollBar.h>
#include <Xm/ComboBox.h>
#include <Xm/Separator.h>
#include <Xm/Scale.h>
#include <Xm/MessageB.h>
#include <Xm/MwmUtil.h>

#if (XmVERSION <= 2) && (XmREVISION >= 2)
#define HAS_XM_TABSTACK
#else 
//LessTif or OpenMotif below 2.2
#endif

//#undef HAS_XM_TABSTACK

#ifdef HAS_XM_TABSTACK
#include <Xm/TabStack.h>
#else
#include <OnX/Xt/Tabs.h>
#endif

// OnX Xt widgets :
#include <OnX/Xt/ListTree.h>
#include <OnX/Xt/CommandLine.h>
#include <OnX/Xt/ColorEditor.h>
#include <OnX/Xt/ColorEditorDialog.h>
#include <OnX/Xt/OptionMenu.h>
#include <OnX/Xt/Progress.h>
#include <OnX/Xt/Password.h>
#include <OnX/Xt/OpenGLArea.h>
#include <OnX/Xt/MainWindow.h>
#include <OnX/Xt/HBox.h>
#include <OnX/Xt/VBox.h>
#include <OnX/Xt/VContainer.h>
#include <OnX/Xt/HContainer.h>

//
#define ONX_HAS_INVENTOR

#ifdef ONX_HAS_INVENTOR
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <Inventor/Xt/viewers/SoXtFlyViewer.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/Tools/RegionSceneHandler.h>
#include <OnX/Inventor/Inventor.h>
#include <OnX/Inventor/HEPVis.h>
#include <OnX/Inventor/GuiViewer.h>
#include <OnX/Xt/PageViewer.h>
#endif

#ifdef HAS_XM_TABSTACK
// Some Motif-2.2 distrib do not have the below !
#ifndef XmNtabLabelString
#define XmNtabLabelString "tabLabelString"
#endif
#ifndef XmNtabSelectedCallback
#define XmNtabSelectedCallback "tabSelectedCallback"
#endif
#endif

// Compound widgets :
#define xm_FileSelectionDialogClass 1
#define xm_MenuBarClass             2
#define xm_ScrolledTextClass        3
#define xm_MenuClass                4
#define xm_ScrolledListClass        6
#define xm_VBoxClass                7
#define xm_HBoxClass                8
#define xm_HPanedClass             10
#define xm_ScrolledTreeClass       12
#define xm_ColorSelectionDialogClass 13
#define xm_VContainerClass           16
#define xm_HContainerClass           17
#define xm_VScrollBarClass           18
#define xm_HScrollBarClass           19
#define xm_ComboBoxClass             20
#define xm_ToolBarClass              21
#define xm_VScaleClass               22
#define xm_HScaleClass               23
#define xm_WarningDialogClass        24
#define xm_Window                    25
#define xm_SoExaminerViewerClass     26
#define xm_SoPlaneViewerClass        27
#define xm_SoFlyViewerClass          28
#define xm_SoWalkViewerClass         29
#define xm_PageViewerClass           30
#define xm_PlanePageViewerClass      31
#define xm_TabStackClass             32

#define MESSAGE_ONX 997

//#define DEBUG_DESTROY

//static Boolean KeyDispatchEvent(XEvent*);
//static void KeyHandler(::Widget,XtPointer,XEvent*,Boolean*);

static void LookDSM_Problem(std::ostream&);
extern "C" {
  static void CloseHandler(::Widget,XtPointer,XEvent*,Boolean*);
}

namespace OnX {
namespace Xt {
class Destroy {
public:
  Destroy(XtUI& aUI,inlib::xml::tree& aItemML):fUI(aUI),fItemML(aItemML){
    Lib::Debug::increment("OnX::Xt::Destroy");
  }
  virtual ~Destroy(){
    Lib::Debug::decrement("OnX::Xt::Destroy");
  }
  bool isValid() const {
    inlib::xml::tree* top = fUI.topItemML();
    return (top && fUI.is_in(*top,&fItemML));
  }
public:
  XtUI& fUI;
  inlib::xml::tree& fItemML;
};
}}

namespace OnX {
class NetCyclic : public XtCyclic {
public: //Slash::UI::ICyclic
  virtual void execute() {fUI.netSteer();}
public:
  inline NetCyclic(XtUI& aUI)
  :XtCyclic(aUI.session(),
            "onx_net",10 /*millisec*/,"","",aUI.appContext()) 
  ,fUI(aUI)
  {}
  virtual ~NetCyclic() {}
private:
  BaseUI& fUI;
};
}

bool xt_proxy(void*,const std::string&,const std::vector<std::string>&,void*&);

OnX::XtUI::XtUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs)
:BaseUI(aSession)
,fAppWidget(0)
,fAppContext(0)
,fConsoleText(0)
,fWarningDialog(0)
,fTermCyclic(0)
{
  LookDSM_Problem(out());

 {bool ui_thread = false;
 {std::string value;
  if(aSession.parameterValue("UI_Manager.thread",value) &&(value.size())) {
    if(!inlib::to(value,ui_thread)) ui_thread = false;
  }}
  if(ui_thread) {
    if(XtToolkitThreadInitialize()==False) {
      std::ostream& out = fSession.cout();
      out << "OnX::XtUI::XtUI :"
          << " problem in XtToolkitThreadInitialize."
          << std::endl;
      return;
    }
  }}

 {int* argc = new int;
  typedef char** argv_t;
  argv_t* argv = new argv_t;
  new_argcv(aArgs,*argc,*argv);
  Arg args[1];
  XtSetArg(args[0], XtNgeometry,XtNewString("100x100"));
  fAppWidget = XtAppInitialize(&fAppContext,
                               "OnX",NULL,(Cardinal)0,
                               argc,*argv,
                               NULL,args,1);}

  if(fAppWidget==NULL) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::XtUI :"
        << " problem in initializing Xt."
        << std::endl;
    return;
  }

  fWidgets.push_back(fAppWidget);

  XtSetMappedWhenManaged(fAppWidget,False);

  // In order that exit works :
  XtRealizeWidget(fAppWidget);

  std::string sxrm;
  if(!inlib::check_getenv(fSession.cout(),"ONX_XRM","ONXXRM",sxrm)) 
    sxrm = "$ONXROOT/scripts/Xt/OnX.xrm";

  std::string name;
  inlib::file_name(sxrm,name);
  if(name.size()) {
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "OnX::XtUI::XtUI :"
          << " load resource file " << inlib::sout(name) << "." 
          << std::endl;
    }
    XLoadResourceFile(XtDisplay(fAppWidget),name);
  }

 {std::string s("OnX.XtUI:");
  s += inlib::p2sx(this);
  XPutStringInResourceDatabase(XtDisplay(fAppWidget),s);}

  // Init Inventor and HEPVis :
#ifdef ONX_HAS_INVENTOR
  SoXt::init(fAppWidget);
  HEPVis::initClasses();

  OnX::Inventor::initialize(aSession);

  //addDispatcher((Slash::UI::IUI::Dispatcher)KeyDispatchEvent);
  addDispatcher((Slash::UI::IUI::Dispatcher)SoXt::dispatchEvent);

  if(!aSession.findManager(Slash_SceneHandler))  {
    aSession.addManager
      (new Slash::RegionSceneHandler(aSession,Slash_SceneHandler));
  }

#else
  addDispatcher((Slash::UI::IUI::Dispatcher)XtDispatchEvent);
#endif

  if(fNetManager) {
    fNetCyclic = new OnX::NetCyclic(*this);
  }

}

OnX::XtUI::~XtUI() { 
  // Should do an XtDestroy(top widgets) ; not sure
  // that embedded viewers are ready for that !

  fBeingDestroyed = true;

  // Treat messages emitted between the quiting of the steering
  // and here. They may come from the destruction of various
  // managers in the ~Session.
  synchronize();

  removeCyclics();

  while(!fWidgets.empty()) {
    ::Widget widget = *(fWidgets.rbegin());
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "OnX::XtUI::~XtUI :"
          << " XtDestroy" 
          << " " << inlib::sout(XtName(widget))
          << " " << inlib::sout(XWidgetGetClassName(widget))
          << "..." 
          << std::endl;
    }
    if(XtIsShell(widget)) XtPopdown(widget);
    XtDestroyWidget(widget); //may induce a fWidget.remove(widget) !
  
    fWidgets.remove(widget); //FIXME : done before the upper ?
  }

  synchronize(); //treat message from the close callbacks.

  if(fAppContext) XtDestroyApplicationContext(fAppContext);

  fAppWidget = 0;
  fAppContext = 0;

  fConsoleText = 0;
  fWarningDialog = 0;

#ifdef ONX_HAS_INVENTOR
  OnX::Inventor::finalize(fSession);
#endif
}

OnX::XtUI::XtUI(const XtUI& aFrom)
:BaseUI(aFrom)
,fAppWidget(0)
,fAppContext(0)
,fConsoleText(0)
,fWarningDialog(0)
,fTermCyclic(0)
{}

OnX::XtUI& OnX::XtUI::operator=(const XtUI&){return *this;}

void* OnX::XtUI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(OnX::XtUI)
  else return BaseUI::cast(a_class);
}

XtAppContext  OnX::XtUI::appContext() const {return fAppContext;}

::Widget OnX::XtUI::appWidget() const {return fAppWidget;}

void OnX::XtUI::put(const std::string& aString) {
  if(!fConsoleText) {
    fConsoleText = (::Widget)findNativeWidget("consoleText");
    if(fConsoleText) {
      XtAddCallback(fConsoleText,
                    XtNdestroyCallback,destroyConsoleTextCallback,this);
    }
  }
  // Append line at end of report region.
  if(!fConsoleText) {
    ::printf("%s",aString.c_str());
    return;
  }
  XmTextAppendString(fConsoleText,aString);
  // Bad idea to show here. In case of a callback
  // mapping a dialog and doing some output 
  // to the console we have some flip effect of shell
  // windows.
  //XmShow(fConsoleText);
}

void OnX::XtUI::warn(const std::string& aString){
  if(!fWarningDialog) {
    fWarningDialog = (::Widget)findNativeWidget("warningDialog");
    if(fWarningDialog) {
      XtAddCallback(fWarningDialog,
                    XtNdestroyCallback,destroyWarningDialogCallback,this);
    }
  }
  // Modal method.
  if(!fWarningDialog) {
    echo("WARNING : "+aString);
    return;
  }
  XmSetString(fWarningDialog,XmNmessageString,aString);
  XRingBell(XtDisplay(fAppWidget),1);
  XtManageChild(fWarningDialog);
  steer();
  XtUnmanageChild(fWarningDialog);
}

int OnX::XtUI::nativeSteer(){
  if(!fAppContext) return 0;
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::steer..."
        << std::endl;
  }
  fSteerLevel++;
  while(true) { 
    XEvent event;
    XtAppNextEvent(fAppContext,&event);
    //XLockDisplay(XtDisplay(fAppWidget));
    if ( (event.xany.type==ClientMessage) && 
         (event.xclient.data.l[0]==MESSAGE_ONX) )   { 
      Slash::Core::IMessage* mess = 
        (Slash::Core::IMessage*)event.xclient.data.l[1];
      if(Lib::ExitMessage* exitMess = INLIB_CAST(*mess,Lib::ExitMessage)) {
        int code = exitMess->code();
        delete mess;
        fSteerLevel--;
        //XUnlockDisplay(XtDisplay(fAppWidget));
        return code;
      } else {
        notification(mess);
      }
    } else {
      std::list<Slash::UI::IUI::Dispatcher>::iterator it;
      for(it=fDispatchers.begin();it!=fDispatchers.end();++it) {
        if((*it)(&event)==TRUE) break;
      }
      //XtDispatchEvent(&event);        
    }
    //XUnlockDisplay(XtDisplay(fAppWidget));
  }

  fSteerLevel--;
  return 0;
}

void OnX::XtUI::synchronize(){
  if(!fAppWidget) return;
  Display* display = XtDisplay(fAppWidget);
  XSync(display,False);
  while(true){ 
    // XtAppPending and XPending do not have the same effect.
    // XtAppPending ok if we move the mouse...
    // XPending seems to miss events.
    //if(XtAppPending(fAppContext)==0) break;
    //if(XPending(display)==0) break;
    //

    // Use XtAppPending but treat "not XEvent" with XtAppProcessEvent.
    // This is so to handle properly the picture and wait commands
    // with "opaw -term" and exec beam.kumac of osc_users/rimbault.

    XtInputMask input = XtAppPending(fAppContext);
    if(input==0) break; /* if no event exit loop */

    if((input&XtIMXEvent)!=XtIMXEvent) {
      //Treat Timers, WorkProcs (then Inventor ones)
      XtAppProcessEvent(fAppContext,input);
    } else {
      XEvent event;
      XtAppNextEvent(fAppContext,&event);
      if ( (event.xany.type==ClientMessage) && 
           (event.xclient.data.l[0]==MESSAGE_ONX) )   { 
        Slash::Core::IMessage* mess = 
          (Slash::Core::IMessage*)event.xclient.data.l[1];
        if(INLIB_CAST(*mess,Lib::ExitMessage)) {
          delete mess;
        } else {
          notification(mess);
        }
      } else {
        std::list<Slash::UI::IUI::Dispatcher>::iterator it;
        for(it=fDispatchers.begin();it!=fDispatchers.end();++it) {
          if((*it)(&event)==TRUE) break;
        }
        //XtDispatchEvent(&event);        
      }
    }
  }
}

void OnX::XtUI::exit(int aExitCode) {
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::exit..."
        << std::endl;
  }
  notify(new Lib::ExitMessage(aExitCode));
}

bool OnX::XtUI::lock(){
  if(!fAppContext) return false;
  XtAppLock(fAppContext);
  return true;
}
bool OnX::XtUI::unlock(){
  if(!fAppContext) return false;
  XtAppUnlock(fAppContext);
  return true;
}

bool OnX::XtUI::notify(Slash::Core::IMessage* aMessage){
  if(!fAppContext) return false;
  if(!fAppWidget) return false;
  XEvent event;
  Display* display = XtDisplay(fAppWidget);
  Window window = XtWindow(fAppWidget);
  if(display==NULL) return false;
  if(window==0L) return false;
  event.type = ClientMessage;
  event.xclient.display = display;
  event.xclient.window = window;
  event.xclient.message_type = XA_INTEGER;
  event.xclient.format = 8;    /* put 8 because bug with 32 on VMCMS */
  event.xclient.data.l[0] = MESSAGE_ONX;
  event.xclient.data.l[1] = (long)aMessage;
  event.xclient.data.l[2] = 0L;
  event.xclient.data.l[3] = 0L;
  event.xclient.data.l[4] = 0L;
  //lock();
  Status stat = XSendEvent(display,window,False,0L,&event);
  XFlush(display);
  //unlock();
  return (stat==0?false:true);
}

bool OnX::XtUI::parameterValue(const std::string& aWhat,std::string& aValue){
  aValue = "";
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;


  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::parameterValue :"
        << " widget " << inlib::sout(aWidget)
        << " not found." << std::endl;
    return false;
  }

  ::Widget widget = (::Widget)itemML->get_data1();
  ::Widget container = (::Widget)itemML->get_data2();
  if(widget) {
    if(aResource=="background") {
      //FIXME : to be tested on OpenGLArea.
      XColor color;
      XtVaGetValues(widget,XtVaTypedArg,
                    XtNbackground,XtRColor,&color,sizeof(color),
                    NULL);
      double r = color.red;
      double g = color.green;
      double b = color.blue;
      aValue = inlib::to<double>(r/65535.)+" "+
               inlib::to<double>(g/65535.)+" "+
               inlib::to<double>(b/65535.);
      return true;
    } else if(aResource=="foreground") {
      if(XtIsSubclass(widget,xmPrimitiveWidgetClass)) {
        XColor color;
        XtVaGetValues(widget,XtVaTypedArg, 
                      XtNforeground,XtRColor,&color,sizeof(color),
                      NULL);      
        double r = color.red;
        double g = color.green;
        double b = color.blue;
        aValue = inlib::to<double>(r/65535.)+" "+
                 inlib::to<double>(g/65535.)+" "+
                 inlib::to<double>(b/65535.);
        return true;
      }
    } else if(aResource=="width") {
      Dimension value  = XWidgetGetWidth(widget);
      inlib::sprintf(aValue,64,"%d",value);
      return true;
    } else if(aResource=="height") {
      Dimension value  = XWidgetGetHeight(widget);
      inlib::sprintf(aValue,64,"%d",value);
      return true;
    } else if(aResource=="label") {
      if(XtIsSubclass(widget,xmLabelWidgetClass)) {
        return XmGetString(widget,XmNlabelString,aValue);
      } else if(XtIsSubclass(widget,optionMenuWidgetClass)) {
        return XmGetString(OptionMenuGetLabel(widget),XmNlabelString,aValue);
      }
    } else if(aResource=="tabLabel") {
      // Constraint resource.
      if(container && XtParent(container)) {
#ifdef HAS_XM_TABSTACK
        if(XtIsSubclass(XtParent(container),xmTabStackWidgetClass)) {
          return XmGetString(container,XmNtabLabelString,aValue);
        }
#else
        if(XtIsSubclass(XtParent(container),tabsWidgetClass)) {
          aValue = XWidgetGetString(container,XtNtabLabel);
          return true;
        }
#endif
      }
    } else if(aResource=="items") {
      if(XtIsSubclass(widget,listtreeWidgetClass)) {
        if(ListTreeFirstItem(widget)) {
          aValue = 
           "<tree>"+ListTreeItemGetXML(ListTreeFirstItem(widget),1)+"</tree>";
        } else {
          aValue = "";
        }
        return true;
      } else if(XtIsSubclass(widget,xmListWidgetClass)) {
        std::vector<std::string> v;
        if(!XmGetItems(widget,XmNitemCount,XmNitems,v)) return false;
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      } else if(XtIsSubclass(widget,xmComboBoxWidgetClass)) {
        std::vector<std::string> v;
        if(!XmComboBoxGetItems(widget,v)) return false;
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      } else if(XtIsSubclass(widget,optionMenuWidgetClass)) {
        std::vector<std::string> v;
        if(!OptionMenuGetItems(widget,v)) return false;
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      }

    } else if(aResource=="selection") {
      if(XtIsSubclass(widget,xmListWidgetClass)) {
        std::vector<std::string> v;
        if(!XmGetItems
              (widget,XmNselectedItemCount,XmNselectedItems,v)) return false;
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      } else if(XtIsSubclass(widget,listtreeWidgetClass)) {
        bool expansion;
        aValue = ListTreeGetSelection(widget,expansion);
        return true;
#ifdef HAS_XM_TABSTACK
      } else if(XtIsSubclass(widget,xmTabStackWidgetClass)) {
        ::Widget selection = XmTabStackGetSelectedTab(widget);
        return XmGetString(selection,XmNtabLabelString,aValue);
#else
      } else if(XtIsSubclass(widget,tabsWidgetClass)) {
        ::Widget selection = NULL;
        Arg args[1];
        XtSetArg(args[0],XtNtopWidget,&selection);
        XtGetValues(widget,args,1);
        if(selection==NULL) {
          aValue = "";
        } else {
          aValue = XWidgetGetString(selection,XtNtabLabel);
        }
        return true;
#endif
      }
    } else if(aResource=="set") {
      if(XtIsSubclass(widget,xmToggleButtonWidgetClass)) {
        int value = 0;
        Arg args[1];
        XtSetArg(args[0],XmNset,&value);
        XtGetValues(widget,args,1);
        aValue = value ? "true" : "false";
        return true;
      }
    } else if(aResource=="geometry") {
      if(IsMainWindow(widget)) { 
        ::Widget shell = XWidgetGetShell(widget);
        if(shell) {
          aValue = XWidgetGetString(shell,XtNgeometry);
          return true;
        }
      } else if(XtIsSubclass(XtParent(widget),xmDialogShellWidgetClass)) {
        // FIXME : this doesn't give exactly the right geometry value
        // FIXME : GB : compare with get "geometry" ?
        ::Widget shell = XWidgetGetShell(widget);
        if(shell) {
          aValue = inlib::tos(XWidgetGetWidth(shell))+"x"+
	    inlib::tos(XWidgetGetHeight(shell))+"+"+
            inlib::tos(XWidgetGetX(shell))+"+"+
            inlib::tos(XWidgetGetY(shell));   
          return true;
        }
      }
    } else if(aResource=="itemCount") {
#ifdef HAS_XM_TABSTACK
      if(XtIsSubclass(widget,xmTabStackWidgetClass)) {
#else
      if(XtIsSubclass(widget,tabsWidgetClass)) {
#endif
        int value  = XWidgetGetChildrenNumber(widget)-1;
        if(value<=0) value = 0;
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="value") {
      if(XtIsSubclass(widget,xmTextWidgetClass)) {
        char* s = XmTextGetString(widget);
        aValue = s ? s : "";
        XtFree(s);
        return true;
      } else if(XtIsSubclass(widget,optionMenuWidgetClass)) {
        OptionMenuGetOption(widget,aValue);
        return true;
      } else if(XtIsSubclass(widget,xmComboBoxWidgetClass)) {
        return XmComboBoxGetValue(widget,aValue);
      } else if(XtIsSubclass(widget,colorEditorWidgetClass)) {
        double r,g,b;
        ColorEditorGetColor(widget,&r,&g,&b);
        inlib::sprintf(aValue,64,"%g %g %g",r,g,b);
        return true;
      } else if(XtIsSubclass(widget,commandLineWidgetClass)) {
        char* s = CommandLineGetString(widget);
        aValue = s ? s : "";
        XtFree(s);
        return true;
      } else if( (XtIsSubclass(widget,xmScrollBarWidgetClass)) ||
                 (XtIsSubclass(widget,xmScaleWidgetClass)) ) {
        int value;
        Arg args[1];
        XtSetArg(args[0],XmNvalue,&value);
        XtGetValues(widget,args,1);
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(XtIsSubclass(widget,xrhpProgressWidgetClass)) {
        int value;
        Arg args[1];
        XtSetArg(args[0],XmNvalue,&value);
        XtGetValues(widget,args,1);
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="minimum") {
      if( (XtIsSubclass(widget,xmScrollBarWidgetClass)) ||
          (XtIsSubclass(widget,xmScaleWidgetClass)) ) {
        int value;
        Arg args[1];
        XtSetArg(args[0],XmNminimum,&value);
        XtGetValues(widget,args,1);
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="maximum") {
      if(XtIsSubclass(widget,xmScaleWidgetClass)) {
        Arg args[1];
        int value;
        XtSetArg(args[0],XmNmaximum,&value);
        XtGetValues(widget,args,1);
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(XtIsSubclass(widget,xmScrollBarWidgetClass)) {
        Arg args[2];
        int sliderSize;
        int value;
        XtSetArg(args[0],XmNsliderSize,&sliderSize);
        XtSetArg(args[1],XmNmaximum,&value);
        XtGetValues(widget,args,2);
        inlib::sprintf(aValue,64,"%d",value-sliderSize);
        return true;
      }
    }
  }
  // Not found, then look in XML elements :
  if(aResource=="class") {
    return itemML->attribute_value("class",aValue);
  }
  return itemML->element_value(aResource,aValue);
}

bool OnX::XtUI::setParameter(const std::string& aWhat,const std::string& aValue){
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::setParameter :"
        << " widget " << inlib::sout(aWidget)
        << " resource " << inlib::sout(aResource)
        << " value " << inlib::sout(aValue) << "."
        << std::endl;
  }

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::setParameter :"
        << " widget " << inlib::sout(aWidget)
        << " not found." << std::endl;
    return false;
  }

  ::Widget widget = (::Widget)itemML->get_data1();
  if(!widget) {
    std::ostream& out = fSession.cout();
    out << "OnX::XtUI::setParameter :"
        << " " << inlib::sout(aWidget)
        << " without a native widget." << std::endl;
    return false;
  }

  ::Widget container = (::Widget)itemML->get_data2();

  std::string svalue;

  if(aResource=="foreground") {
    Pixel pixel = XWidgetConvertStringToPixel(widget,aValue);
    Arg args[1];
    XtSetArg(args[0],XtNforeground,pixel);
    XtSetValues(widget,args,1);
    return true;
  } else if(aResource=="background") {
    Pixel pixel = XWidgetConvertStringToPixel(widget,aValue);
    Arg args[1];
    XtSetArg(args[0],XtNbackground,pixel);
    XtSetValues(widget,args,1);
    return true;
  } else if(aResource=="decoration") {
    if(XtIsSubclass(XtParent(widget),xmDialogShellWidgetClass) ||
       IsMainWindow(widget)) {
      // FIXME : this doesn't work after widget creation.
      // If we call setParameter just after creation, it works, but in the
      // interface builder, we call the good element, but nothing happens...
      ::Widget shell = XWidgetGetShell(widget);
      if(shell) {
        XmShellSetDecorations(shell,parseDecorations(aValue));
        return true;
      }
    }
  } else if(aResource=="geometry") {
    // The below on MainWindow does not work.
    //XWidgetSetString(shell,XtNgeometry,aValue);
    if(IsMainWindow(widget) ||
       XtIsSubclass(XtParent(widget),xmDialogShellWidgetClass)) {
      ::Widget shell = XWidgetGetShell(widget);
      if(shell) {
        int x,y,w,h;
        if(!to_geometry(aValue,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << "XtUI::setParameter :"
              << " \"" << aValue << "\" : bad value for geometry." 
              << std::endl;
        } else {
          Arg args[4];
          XtSetArg(args[0],XmNwidth,w);
          XtSetArg(args[1],XmNheight,h);
          XtSetArg(args[2],XmNx,x);
          XtSetArg(args[3],XmNy,y);
          XtSetValues(shell,args,4);
          return true;
        }
      }
    }
  } else if(aResource=="expand") {
    //FIXME : have hboxWidgetClass, vboxWidgetClass
    std::string sclass;
    itemML->attribute_value("class",sclass);
    //FIXME if(XtIsSubclass(xtWidget,vboxWidgetClass))
    //FIXME if(XtIsSubclass(xtWidget,hboxWidgetClass))
    if(getCompound(sclass)==xm_VBoxClass) {
      if(VBoxAttachChildren(widget,aValue,false,0)) return true;
    } else if(getCompound(sclass)==xm_HBoxClass) {
      if(HBoxAttachChildren(widget,aValue,false,0)) return true;
    }
  } else if(aResource=="splitPosition") {
    if(XtIsSubclass(widget,xmPanedWindowWidgetClass)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        //FIXME : does not work properly.
        //return XmPanedSetSplitPosition(widget,value);
        std::ostream& out = fSession.cout();
        out << "OnX::XtUI::setParameter :"
            << " Sorry, we have not found yet a clean way" 
            << " to set splitPosition in Motif."
            << std::endl;
        return false;
      }
    }
  } else if(aResource=="label") {
    if(XtIsSubclass(widget,xmLabelWidgetClass)) {
      XmSetString(widget,XmNlabelString,aValue);
      return true;
    } else if(XtIsSubclass(widget,optionMenuWidgetClass)) {
      XmSetString(OptionMenuGetLabel(widget),XmNlabelString,aValue);
      return true;
    }
  } else if(aResource=="dirMask") {
    if(XtIsSubclass(widget,xmFileSelectionBoxWidgetClass)) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      std::vector<std::string> items;
      inlib::words(svalue,";",false,items);
      //FIXME : handle multiple wildcards.
      if(items.size()) XmSetString(widget,XmNdirMask,items[0]);
      return true;
    }
  } else if(aResource=="directory") {
    if(XtIsSubclass(widget,xmFileSelectionBoxWidgetClass)) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      XmSetString(widget,XmNdirectory,svalue);
      return true;
    }
  } else if(aResource=="value") {
    //WARNING : commandLine inherits XmText.

    if(XtIsSubclass(widget,commandLineWidgetClass)) {
      CommandLineSetString(widget,(char*)aValue.c_str());
      return true;
    } else if(XtIsSubclass(widget,xmTextWidgetClass)) {
      XmTextSetString(widget,(char*)aValue.c_str());
      return true;
    } else if( (XtIsSubclass(widget,xmScrollBarWidgetClass)) ||
               (XtIsSubclass(widget,xmScaleWidgetClass)) ) {

      int mn,mx;
      if(XtIsSubclass(widget,xmScaleWidgetClass)) {
        XmScaleGetMinMax(widget,mn,mx);
      } else if(XtIsSubclass(widget,xmScrollBarWidgetClass)) {
        XmScrollBarGetMinMax(widget,mn,mx);
      }

      int value;
      if(inlib::to<int>(aValue,value)) {
        if((value>=mn)&&(value<=mx)) {
          Arg args[1];
          XtSetArg(args[0],XmNvalue,value);
          XtSetValues(widget,args,1);
          return true;
        } else {
          std::ostream& out = fSession.cout();
          out << "OnX::XtUI::setParameter :"
              << " value " << value 
              << " out of bounds (min=" << mn 
              << ", max=" << mx << ")." 
              << std::endl;
          return false;
        }
      }


    } else if(XtIsSubclass(widget,xrhpProgressWidgetClass)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        Arg args[1];
        XtSetArg(args[0],XmNvalue,value);
        XtSetValues(widget,args,1);
        return true;
      }
    } else if(XtIsSubclass(widget,optionMenuWidgetClass)) {
      if(OptionMenuSetOption(widget,aValue)) return true;
    } else if(XtIsSubclass(widget,xmComboBoxWidgetClass)) {
      XmComboBoxSetValue(widget,aValue);
      return true;
    }
  } else if(aResource=="minimum") {
    if( (XtIsSubclass(widget,xmScrollBarWidgetClass)) ||
        (XtIsSubclass(widget,xmScaleWidgetClass)) ) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        Arg args[1];
        XtSetArg(args[0],XmNminimum,value);
        XtSetValues(widget,args,1);
        return true;
      }
    }
  } else if(aResource=="maximum") {
    if(XtIsSubclass(widget,xmScaleWidgetClass)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        Arg args[1];
        XtSetArg(args[0],XmNmaximum,value);
        XtSetValues(widget,args,1);
        return true;
      }
    } else if(XtIsSubclass(widget,xmScrollBarWidgetClass)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        Arg args[1];
        int sliderSize;
        XtSetArg(args[0],XmNsliderSize,&sliderSize);
        XtGetValues(widget,args,1);
        XtSetArg(args[0],XmNmaximum,value+sliderSize);
        XtSetValues(widget,args,1);
        return true;
      }
    }
  } else if(aResource=="set") {
    if(XtIsSubclass(widget,xmToggleButtonWidgetClass)) {
      bool value;
      if(inlib::to(aValue,value)) {
        Arg args[1];
        XtSetArg(args[0],XmNset,value?TRUE:FALSE);
        XtSetValues(widget,args,1);
        return true;
      }
    }
  } else if(aResource=="selection") {
    if(XtIsSubclass(widget,xmListWidgetClass)) {
      std::vector<std::string> text;
      text.push_back(aValue);
      int itemn;
      XmString* items = XmStringTableCreateFromText(text,itemn);
      Arg args[2];
      XtSetArg(args[0],XmNselectedItems,items);
      XtSetArg(args[1],XmNselectedItemCount,itemn);
      XtSetValues(widget,args,2);
      XmStringTableDelete(items);
      return true;
    } else if(XtIsSubclass(widget,listtreeWidgetClass)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      if(ListTreeSetSelection(widget,items)) return true;
#ifdef HAS_XM_TABSTACK
    } else if(XtIsSubclass(widget,xmTabStackWidgetClass)) {
      std::vector< ::Widget > ws;
      XWidgetGetChildren(widget,ws);
      for(unsigned int index=0;index<ws.size();index++) {
        std::string s;
        if(XmGetString(ws[index],XmNtabLabelString,s)) {
          if(s==aValue) {
            XmTabStackSelectTab(ws[index],True);
            return true;
          }
        }
      }
#else
    } else if(XtIsSubclass(widget,tabsWidgetClass)) {
      std::vector< ::Widget > ws;
      XWidgetGetChildren(widget,ws);
      for(unsigned int index=0;index<ws.size();index++) {
        if(XWidgetGetString(ws[index],XtNtabLabel)==aValue) {
          XawTabsSetTop(ws[index],True);
          break;
        }
      }
      return true;
#endif
    }
  } else if(aResource=="items") {
    if(XtIsSubclass(widget,xmListWidgetClass)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      XmListDeleteAllItems(widget);
      XmListDeselectAllItems(widget);
      unsigned int itemn = items.size();
      for(unsigned int count=0;count<itemn;count++){
        XmString cps = XmStringLtoRCreate((char*)items[count].c_str(),
                                          XmSTRING_DEFAULT_CHARSET);
        XmListAddItemUnselected(widget,cps,0);
        XmStringFree(cps);
      }
      return true;
    } else if(XtIsSubclass(widget,optionMenuWidgetClass)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      OptionMenuInitialize(widget,items,"");
      return true;
    } else if(XtIsSubclass(widget,xmComboBoxWidgetClass)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      XmComboBoxSetItems(widget,items);
      return true;
  /*} else if(XtIsSubclass(widget,xmListWidgetClass)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      XmListDeleteAllItems(widget);
      XmListDeselectAllItems(widget);
     {int xmsn;
      XmString* xmss = XmStringTableCreateFromText(items,xmsn);
      XmListAddItems(widget,xmss,xmsn,0);
      XmStringTableDelete(xmss);}
      return true;*/
    } else if(XtIsSubclass(widget,listtreeWidgetClass)) {
      // retrieve the old tree in a string
      std::string oldXMLTree;
      if(ListTreeFirstItem(widget))
        oldXMLTree = ListTreeItemGetXML(ListTreeFirstItem(widget),1);
      // retrieve the old tree selection in a string :
      bool expansion;
      std::string selection = ListTreeGetSelection(widget,expansion);
      std::string stop = ListTreeGetTopItem(widget);
      // clear the old tree in memory
      ListTreeClear(widget);
      // load the new Tree
      inlib::xml::default_factory factory;
      exlib::xml::parser treeML(factory,out(),fSession.verboseLevel());
      std::vector<std::string> tags;
      tags.push_back("tree");
      tags.push_back("treeItem");
      treeML.set_tags(tags);
      if(treeML.load_string(aValue)) {
        inlib::xml::tree* top = treeML.top_item();
        //top->dump();
        if(top) {
          if (oldXMLTree.size()) {
            oldXMLTree = "<tree>"+oldXMLTree+"</tree>";
	    exlib::xml::parser oldTreeML
              (factory,out(),fSession.verboseLevel());
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
          //disable refresh when adding
          ListTreeRefreshOff(widget);
          createTree(widget,top);
          //Restore selection :
          std::vector<std::string> items = smanip_text(selection);
          ListTreeSetSelection(widget,items);
          //Restor top visible item :
          items = smanip_text(stop);
          ListTreeSetTopItem(widget,items);
          //enable refresh
          ListTreeRefreshOn(widget);
          return true; 
        }
      }
    }
  } else if(aResource=="default") {
    if(XtIsSubclass(widget,optionMenuWidgetClass)) {
      OptionMenuSetDefault(widget,aValue);
      return true;
    } else if(XtIsSubclass(widget,xmComboBoxWidgetClass)) {
      XmComboBoxSetValue(widget,aValue);
      return true;
    }
  } else if (aResource=="title") {
    ::Widget shell = XWidgetGetShell(widget);
    if(shell) {
      XWidgetSetString(shell,XtNtitle,aValue);
      return true;
    }
  } else if (aResource=="pixmap") {
    if(XtIsSubclass(widget,xmLabelWidgetClass)) {
      std::string fileName;
      inlib::file_name(aValue,fileName);
      XmSetLabelPixmap(widget,fileName);
      return true;
    }
  } else if (aResource=="tooltip") {
    if(XtIsSubclass(widget,xmLabelWidgetClass)) {
#if (XmVERSION <= 2) && (XmREVISION >= 2)
      XmSetString(widget,XmNtoolTipString,aValue);
#endif
      return true;
    }
  } else if(aResource=="popupItems") {
#ifdef ONX_HAS_INVENTOR
    if(XtIsSubclass(widget,pageViewerWidgetClass)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      for(unsigned int index=0;index<items.size();index++)
        PageViewerAddPopupEntry(widget,items[index]);
      return true;
    }
#endif
  } else if(aResource=="tabLabel") { //Constraint resource
    if(container && XtParent(container)) {
#ifdef HAS_XM_TABSTACK
      if(XtIsSubclass(XtParent(container),xmTabStackWidgetClass)) {
        XmSetString(container,XmNtabLabelString,aValue);
        return true;
      }
#else
      if(XtIsSubclass(XtParent(container),tabsWidgetClass)) {
        XWidgetSetString(container,XtNtabLabel,aValue);
        return true;
      }
#endif
    }
  } else if(aResource=="mode") {
    return true;
  }

  std::ostream& out = fSession.cout();
  out << "OnX::XtUI::setParameter :"
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

bool OnX::XtUI::createTree(::Widget aTree,inlib::xml::tree* aItemML){
  std::list<inlib::xml::tree*> list;
  aItemML->sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    std::string slabel;
    (*it)->element_value("label",slabel);

    std::string sopened;
    bool opened = false;
    if((*it)->element_value("opened",sopened)) {
      inlib::to(sopened,opened);
    }

    void* data1;
    void* data2;
    int dataInt;
    (*it)->parent()->get_data(data1,data2,dataInt);
    ListTreeItem* item = ListTreeAdd(aTree,
                                     (ListTreeItem*)data1,
                                     (char*)slabel.c_str());
    ListTreeOpenCloseItem(aTree,item,opened);
    (*it)->set_data(item,0,0);

    if(!createTree(aTree,*it)) return false;
  }
  return true;
}

bool OnX::XtUI::createNativeWidget(
 inlib::xml::tree& aItemML                      
,const std::string& aClass
,const std::string& aName
,inlib::xml::tree* aParent
){
    OnX::Widget* owidget = INLIB_CAST(aItemML,OnX::Widget);
    if(!owidget) {
      std::ostream& out = fSession.cout();
      out << "XtUI::createNativeWidget :"
          << " ItemML not a OnX::Widget." 
          << aClass << std::endl;
      return false;
    }

    int compoundId = getCompound(aClass);
    WidgetClass xtClass = getType(aClass);
    if( (compoundId==0) && (xtClass==0) ) {
      std::ostream& out = fSession.cout();
      out << "XtUI::createNativeWidget :"
          << " unknown widget type : " 
          << aClass << std::endl;
      return false;
    }
    
    // Get parent :
    ::Widget xtContainer = 0;
    ::Widget xtParent = 0;
    int parentCompoundId = 0;
    std::string sparentclass; 
    if(aParent) {
      xtParent = (::Widget)aParent->get_data1();
      aParent->attribute_value("class",sparentclass);
      parentCompoundId = getCompound(sparentclass);
    } 
    
    // having a PageViewer and something else (like a XmPushButton)
    // in the fAppWidget whilst it is realized is crashy.
    // See OnX_test_page.cxx to reproduce.
    // We prevent that.
    //if(!xtParent) {
    //  xtParent = fAppWidget; // do not authorize that.
    //}  

    if(!xtParent) {
      if(compoundId==xm_Window) {
        //only xm_Window can be created without parent.
      } else {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " can't create widget " << inlib::sout(aName)
            << " of class " << inlib::sout(aClass)
            << " without a parent."
            << std::endl;
        return false;
      }  
    }  

    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "XtUI::createNativeWidget :" 
          << " parent with Xt class "  
          << inlib::sout(xtParent?XWidgetGetClassName(xtParent):"none")
          << "." << std::endl;
    }


    // Check parent number of children :
    if ((parentCompoundId==xm_HBoxClass)||(parentCompoundId==xm_VBoxClass)) {
      if(XWidgetGetChildrenNumber(xtParent)>=2) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " could not create a third widget in  : " 
            << sparentclass << std::endl;
        return false;
      }
    }
    if(xtParent && (XtIsSubclass(xtParent,xmPanedWindowWidgetClass))) {
      if(XmPanedGetNumberOfUserChildren(xtParent)>=2) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " could not create a third widget in  : " 
            << sparentclass << std::endl;
        return false;
      }
    }

    std::string svalue;

    // Create the Xt widget :
    ::Widget xtWidget = 0;
    if(compoundId==xm_Window) {
      // Returns an XmForm.
      xtWidget = 
        CreateMainWindowShell
          (XtDisplay(fAppWidget),(char*)aName.c_str(),NULL,0);
      xtContainer = XWidgetGetShell(xtWidget);
     {::Widget shell = XWidgetGetShell(xtWidget);
      fWidgets.push_back(shell);      
      Xt::Destroy* dst = new Xt::Destroy(*this,aItemML);
      XtAddEventHandler(shell,0L,True,CloseHandler,(XtPointer)dst);
      XtAddCallback(shell,XtNdestroyCallback,closeCallback,dst);
      XtAddCallback(shell,XtNdestroyCallback,destroyShellCallback,this);}
    } else if(xtClass==xmDialogShellWidgetClass) {
      //We do not use the below in order to master geometry.
      //xtWidget = XmCreateFormDialog(xtParent,(char*)dname.c_str(),args,argc);

      //NOTE : XtNgeometry on the XmDialogShell does not work.
      //       The doc says that we have to change the width,height
      //       of the child.
     {std::string dname = aName + "_popup";
      Arg args[1];
      XtSetArg(args[0],XmNallowShellResize,False);
      //FIXME : strange, if True, the dialog does not resize !
      xtContainer = XmCreateDialogShell(xtParent,(char*)dname.c_str(),args,1);}
      
      // set the geometry at creation because after, it doen't work
      // x&y param have no effect because the dialog automaticaly
      // center from the main window
      Arg args[4];
      int argc = 0;
      std::string sgeom;
      if(aItemML.element_value("geometry",sgeom)) {
        int x,y,w,h;
        if(!to_geometry(sgeom,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << "XtUI::createNativeWidget :"
              << " \"" << sgeom << "\" : bad value for geometry." 
              << std::endl;
        } else {
          XtSetArg(args[0],XmNwidth,w);
          XtSetArg(args[1],XmNheight,h);
          //XtSetArg(args[2],XmNx,x);
          //XtSetArg(args[3],XmNy,y);
          argc = 2;
          //::printf("debug : %d %d\n",w,h);
	}
      }
      xtWidget = XmCreateForm(xtContainer,(char*)aName.c_str(),args,argc);
     {::Widget shell = XWidgetGetShell(xtWidget);
      fWidgets.push_back(shell);

      unsigned long Mwm_funct = 0;
      unsigned long Mwm_decor = 0;
      Mwm_funct |= MWM_FUNC_MOVE;
      Mwm_decor |= MWM_DECOR_TITLE;
      Mwm_funct |= MWM_FUNC_RESIZE;
      Mwm_decor |= MWM_DECOR_RESIZEH; //usefull for some palette (Fitter.onx).
      XtSetArg(args[0],XmNmwmFunctions,Mwm_funct);
      XtSetArg(args[1],XmNmwmDecorations,Mwm_decor);
      XtSetValues(shell,args,2);

      XtAddCallback(shell,XtNdestroyCallback,destroyShellCallback,this);}
    } else if(compoundId==xm_FileSelectionDialogClass) {
      xtWidget = XmCreateFileSelectionDialog(xtParent,
                                             (char*)aName.c_str(),0,0);
     {::Widget shell = XWidgetGetShell(xtWidget);
      fWidgets.push_back(shell);
      XtAddCallback(shell,XtNdestroyCallback,destroyShellCallback,this);}
    } else if(compoundId==xm_ColorSelectionDialogClass) {
      xtWidget = CreateColorEditorDialog(xtParent,(char*)aName.c_str(),0,0);
     {::Widget shell = XWidgetGetShell(xtWidget);
      fWidgets.push_back(shell);
      XtAddCallback(shell,XtNdestroyCallback,destroyShellCallback,this);}
    } else if(compoundId==xm_WarningDialogClass) {
      Arg args[4];
      int argc = 0;
      if(aItemML.element_value("geometry",svalue)) {
        int x,y,w,h;
        if(!to_geometry(svalue,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << "XtUI::createNativeWidget :"
              << " \"" << svalue << "\" : bad value for geometry." 
              << std::endl;
        } else {
          XtSetArg(args[0],XmNwidth,w);
          XtSetArg(args[1],XmNheight,h);
          XtSetArg(args[2],XmNx,x);
          XtSetArg(args[3],XmNy,y);
          argc = 4;
          //WARNING : the below does not work.
          //XtSetArg(args[0],XmNallowShellResize,True);
          //XtSetArg(args[1],XtNgeometry,XtNewString(svalue.c_str()));
          //argc = 2;
	}
      }
      // The below returns a xmMessageBoxWidgetClass widget
      // with a xmDialogShellWidgetClass as parent.
      xtWidget = 
        XmCreateWarningDialog(xtParent,(char*)aName.c_str(),args,argc);
      XtAddCallback(xtWidget,XmNokCallback,warningCallback,this);
      XtAddCallback(xtWidget,XmNcancelCallback,warningCallback,this);
     {::Widget shell = XWidgetGetShell(xtWidget);
      fWidgets.push_back(shell);
      unsigned long Mwm_funct = 0;
      unsigned long Mwm_decor = 0;
      Mwm_funct |= MWM_FUNC_MOVE;
      Mwm_decor |= MWM_DECOR_TITLE;
      XtSetArg(args[0],XmNmwmFunctions,Mwm_funct);
      XtSetArg(args[1],XmNmwmDecorations,Mwm_decor);
      XtSetValues(shell,args,2);
      XtAddCallback(shell,XtNdestroyCallback,destroyShellCallback,this);}
    } else if(compoundId==xm_MenuBarClass) {
      if(IsMainWindow(xtParent)==False) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " Parent not a main window."
            << std::endl;
        return false;
      }
      ::Widget menuBar = GetMainWindowMenuBar(xtParent);
      if(!menuBar) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " MenuBar not found in parent,"
            << std::endl;
        return false;
      }
      ActivateMainWindowMenuBar(xtParent);
      xtWidget = menuBar;
    } else if(compoundId==xm_ToolBarClass) {
      if(IsMainWindow(xtParent)==False) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " Parent not a main window."
            << std::endl;
        return false;
      }
      ::Widget toolBar = GetMainWindowToolBar(xtParent);
      if(!toolBar) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget :"
            << " ToolBar not found in parent,"
            << std::endl;
        return false;
      }
      ActivateMainWindowToolBar(xtParent);
      xtWidget = toolBar;
    } else if(compoundId==xm_ScrolledTextClass) {
      Arg args[1];
      XtSetArg(args[0],XmNeditMode,XmMULTI_LINE_EDIT);
      xtWidget = XmCreateScrolledText(xtParent,(char*)aName.c_str(),args,1);
      XmTextSetNumberOfRowsAndColumns(xtWidget,20,80);
      xtContainer = XtParent(xtWidget);
    } else if(compoundId==xm_ScrolledListClass) {
      Arg args[3];
      XtSetArg(args[0],XmNscrollBarDisplayPolicy,XmSTATIC);
      XtSetArg(args[1],XmNselectionPolicy,XmSINGLE_SELECT);
      XtSetArg(args[2],XmNvisibleItemCount,10);
      xtWidget = XmCreateScrolledList(xtParent,
                                      (char*)aName.c_str(),args,3);
      xtContainer = XtParent(xtWidget);
    } else if(compoundId==xm_VBoxClass) {
      xtWidget = XmCreateForm(xtParent,(char*)aName.c_str(),0,0);
    } else if(compoundId==xm_HBoxClass) {
      xtWidget = XmCreateForm(xtParent,(char*)aName.c_str(),0,0);
    } else if(compoundId==xm_VContainerClass) {
      Arg args[1];
      XtSetArg(args[0],XmNfractionBase,1000);
      xtWidget = XmCreateForm(xtParent,(char*)aName.c_str(),args,1);
    } else if(compoundId==xm_HContainerClass) {
      Arg args[1];
      XtSetArg(args[0],XmNfractionBase,1000);
      xtWidget = XmCreateForm(xtParent,(char*)aName.c_str(),args,1);
    } else if(compoundId==xm_ComboBoxClass) {
      xtWidget = 
        XmCreateDropDownComboBox(xtParent,(char*)aName.c_str(),0,0);
    } else if(compoundId==xm_HPanedClass) {
      Arg args[1];
      XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
      xtWidget = XmCreatePanedWindow(xtParent,(char*)aName.c_str(),args,1);
  //} else if(compoundId==xm_TreeItemClass) {
    } else if(compoundId==xm_MenuClass) {
      // xtParent is the cascade button :
      // XtParent of the cascade button should be a menu bar.
      ::Widget menuBar = XtParent(xtParent);
      xtWidget = XmCreatePulldownMenu(menuBar,(char*)aName.c_str(),0,0);
      // Connect cascade button and pulldown menu :
      Arg args[1];
      XtSetArg(args[0],XmNsubMenuId,xtWidget);
      XtSetValues(xtParent,args,1);

    } else if(compoundId==xm_ScrolledTreeClass) {
      Arg args[1];
      XtSetArg(args[0],((char*)XtNdoSingleClickCallback),True);
      xtWidget = 
        XmCreateScrolledListTree(xtParent,(char*)aName.c_str(),args,1);
      xtContainer = XtParent(xtWidget);

    } else if(compoundId==xm_TabStackClass) {
#ifdef HAS_XM_TABSTACK
      xtContainer = XmCreateForm(xtParent,(char*)"tab_stack_container",0,0);
      XtManageChild(xtContainer);
      Arg args[5];
      XtSetArg(args[0],XmNbottomAttachment,XmATTACH_FORM);
      XtSetArg(args[1],XmNleftAttachment,XmATTACH_FORM);
      XtSetArg(args[2],XmNrightAttachment,XmATTACH_FORM);
      XtSetArg(args[3],XmNtopAttachment,XmATTACH_FORM);
      XtSetArg(args[4],XmNtabMode,XmTABS_STACKED);
      xtWidget = 
        XtCreateWidget(aName.c_str(),xmTabStackWidgetClass,xtContainer,args,5);
#else
      xtWidget = 
        XtCreateWidget(aName.c_str(),tabsWidgetClass,xtParent,0,0);
#endif

    } else if(compoundId==xm_VScrollBarClass) {
      Arg args[1];
      XtSetArg(args[0],XmNorientation,XmVERTICAL);
      xtWidget = XmCreateScrollBar(xtParent,(char*)aName.c_str(),args,1);
    } else if(compoundId==xm_HScrollBarClass) {
      Arg args[1];
      XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
      xtWidget = XmCreateScrollBar(xtParent,(char*)aName.c_str(),args,1);
    } else if(compoundId==xm_VScaleClass) {
      Arg args[1];
      XtSetArg(args[0],XmNorientation,XmVERTICAL);
      xtWidget = XmCreateScale(xtParent,(char*)aName.c_str(),args,1);
    } else if(compoundId==xm_HScaleClass) {
      Arg args[1];
      XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
      xtWidget = XmCreateScale(xtParent,(char*)aName.c_str(),args,1);

#ifdef ONX_HAS_INVENTOR
    } else if(compoundId==xm_SoExaminerViewerClass) {
      SoXtExaminerViewer* viewer = 
        new SoXtExaminerViewer(xtParent,aName.c_str(),TRUE);
      xtContainer = viewer->getBaseWidget();
      xtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy
        (viewer,viewer_proxy<SoXtFullViewer,SoXtExaminerViewer>);
    } else if(compoundId==xm_SoPlaneViewerClass) {
      SoXtPlaneViewer* viewer = 
        new SoXtPlaneViewer(xtParent,aName.c_str(),TRUE);
      xtContainer = viewer->getBaseWidget();
      xtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoXtFullViewer>);
    } else if(compoundId==xm_SoFlyViewerClass) {
      SoXtFlyViewer* viewer = new SoXtFlyViewer(xtParent,aName.c_str(),TRUE);
      xtContainer = viewer->getBaseWidget();
      xtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoXtFullViewer>);
    } else if(compoundId==xm_SoWalkViewerClass) {
      SoXtWalkViewer* viewer = new SoXtWalkViewer(xtParent,aName.c_str(),TRUE);
      xtContainer = viewer->getBaseWidget();
      xtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoXtFullViewer>);
    } else if( (compoundId==xm_PageViewerClass) ||
               (compoundId==xm_PlanePageViewerClass) ){
      std::string vclass = "Examiner";
      if(compoundId==xm_PlanePageViewerClass) vclass = "Plane";
      Arg args[3];
      XtSetArg(args[0],XtNwidth,500);
      XtSetArg(args[1],XtNheight,400);
      XtSetArg(args[2],(char*)XtNviewerClass,vclass.c_str());
      xtWidget = 
        XtCreateWidget(aName.c_str(),pageViewerWidgetClass,xtParent,args,3);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) {
        std::ostream& out = fSession.cout();
        out << "XtUI::createNativeWidget : ItemML not a OnX::SoViewer." 
            << std::endl;
        aItemML.dump_xml(fSession.out(),"  ");
        return false;
      }
      SoNode* soNode = PageViewerGetSceneGraph(xtWidget);
      soViewer->setSceneGraph(soNode);
      SoXtFullViewer* viewer = PageViewerGetSoViewer(xtWidget);
      if(viewer->isOfType(SoXtExaminerViewer::getClassTypeId())) {    
        soViewer->setViewerProxy
          (viewer,viewer_proxy<SoXtFullViewer,SoXtExaminerViewer>);
      } else {
        soViewer->setViewerProxy(viewer,viewer_proxy<SoXtFullViewer>);
      }
#endif
    } else {
      // Generic case : 
      xtWidget = XtCreateWidget(aName.c_str(),xtClass,xtParent,0,0);
    }
    if(!xtWidget) return false;
    if(!xtContainer) xtContainer = xtWidget;

    XtAddCallback(xtContainer,XtNdestroyCallback,
                  destroyCallback,
                  (XtPointer)new Xt::Destroy(*this,aItemML));

    aItemML.set_data(xtWidget,xtContainer,0);
    owidget->setProxy(xtWidget,xt_proxy);

    // Management :
    //  if compound, xtParent could be different of XtParent().
    if( ( (XtParent(xtWidget)!=NULL) && (
          XtIsSubclass(XtParent(xtWidget),xmDialogShellWidgetClass)  ||
          XtIsSubclass(XtParent(xtWidget),xmMenuShellWidgetClass) )
          ) || (XClassIsSubclass(xtClass,shellWidgetClass)==True) ) {
    }  else {
      XtManageChild(xtWidget);
    }
    
    // Put in parent :
    if(parentCompoundId==xm_Window) {
      if(compoundId==xm_MenuBarClass) {
      } else if(compoundId==xm_ToolBarClass) {
      } else {
        Arg args[4];
        XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
        XtSetArg(args[1],XmNbottomAttachment ,XmATTACH_FORM);
        XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
        XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
        XtSetValues(xtContainer,args,4);
      }
    } else if(parentCompoundId==xm_VContainerClass) {
      //Done later.
    } else if(parentCompoundId==xm_HContainerClass) {
      //Done later.
    } else if(parentCompoundId==xm_VBoxClass) {
      if(!aParent->element_value("expand",svalue)) svalue = "second";
      /*FIXME : have the below logic.
                It induces edge sunc Warning on the VBox of the CommandBox.
      std::string s;
      unsigned int v;
      if(aParent->element_value("sizeOfFixed",s) && inlib::to<unsigned int>(s,v)){
        if(!VBoxAttachChildren(xtParent,svalue,true,v)) return false;
      } else {
        if(!VBoxAttachChildren(xtParent,svalue,false,0)) return false;
      }*/
      if(!VBoxAttachChildren(xtParent,svalue,false,0)) return false;
    } else if(parentCompoundId==xm_HBoxClass) {
      if(!aParent->element_value("expand",svalue)) svalue = "second";
      /*FIXME : have the below logic.
      std::string s;
      unsigned int v;
      if(aParent->element_value("sizeOfFixed",s) && inlib::to<unsigned int>(s,v)){
        if(!HBoxAttachChildren(xtParent,svalue,true,v)) return false;
      } else {
        if(!HBoxAttachChildren(xtParent,svalue,false,0)) return false;
      }*/
      if(!HBoxAttachChildren(xtParent,svalue,false,0)) return false;
    } else if( XtParent(xtWidget) && 
               XtIsSubclass(XtParent(xtWidget),xmFormWidgetClass) ) {
      ::Widget parent = XtParent(xtWidget);
      if( XtParent(parent) && 
          XtIsSubclass(XtParent(parent),xmDialogShellWidgetClass) ) {
        Arg args[4];
        XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
        XtSetArg(args[1],XmNbottomAttachment ,XmATTACH_FORM);
        XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
        XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
        XtSetValues(xtContainer,args,4);
      }
    }    

    if(compoundId==xm_Window) {
      ::Widget shell = XWidgetGetShell(xtWidget);
      if(shell) {
        if(aItemML.element_value("decoration",svalue)) {
          XmShellSetDecorations(shell,parseDecorations(svalue));
        }
        std::string stitle;
        aItemML.element_value("title",stitle);
        XWidgetSetString(shell,XtNtitle,stitle);
        if(aItemML.element_value("geometry",svalue)) {
          XWidgetSetString(shell,XtNgeometry,svalue);
        }
      }
    } else if( (xtClass==xmDialogShellWidgetClass) ||
               (compoundId==xm_WarningDialogClass) ) {
      ::Widget shell = XWidgetGetShell(xtWidget);
      if(shell) {
        if(aItemML.element_value("decoration",svalue)) {
          XmShellSetDecorations(shell,parseDecorations(svalue));
        }
        std::string stitle;
        aItemML.element_value("title",stitle);
        XWidgetSetString(shell,XtNtitle,stitle);
        //if(aItemML.element_value("geometry",svalue)) {
        // //done at creation.
        //}
      }
    } else if( (xtClass==xmCascadeButtonWidgetClass) || 
               (xtClass==xmPushButtonWidgetClass) ) {
      if(aItemML.element_value("label",svalue)){
        XmSetString(xtWidget,XmNlabelString,svalue);
      }
      if(aItemML.element_value("pixmap",svalue)) {
        std::string fileName;
        inlib::file_name(svalue,fileName);
        XmSetLabelPixmap(xtWidget,fileName);
      }

      if(aItemML.element_value("accelerator",svalue)) {
        XmSetString(xtWidget,XmNacceleratorText,svalue);
        //Assumed to be the Qt syntax (exa : Ctrl+a)
        //Ctrl+a -> Ctrl<Key>a
        inlib::replace(svalue,"+","<Key>");
        XWidgetSetString(xtWidget,XmNaccelerator,svalue);
      }
#if (XmVERSION <= 2) && (XmREVISION >= 2)
      if(aItemML.element_value("tooltip",svalue)) {
        XmSetString(xtWidget,XmNtoolTipString,svalue);
      }
#endif
      // Callbacks :
      addCallbacks(aItemML,xtWidget,"activate",XmNactivateCallback);

      if(xtClass==xmCascadeButtonWidgetClass) {
        addCallbacks(aItemML,xtWidget,"cascading",XmNcascadingCallback);
      }
    } else if(xtClass==xmToggleButtonWidgetClass) {
      if(aItemML.element_value("label",svalue)){
        XmSetString(xtWidget,XmNlabelString,svalue);
      }
      if(aItemML.element_value("pixmap",svalue)) {
        std::string fileName;
        inlib::file_name(svalue,fileName);
        XmSetLabelPixmap(xtWidget,fileName);
      }

      if(aItemML.element_value("set",svalue)) {
        bool value;
        if(inlib::to(svalue,value)) {
          Arg args[1];
          XtSetArg(args[0],XmNset,value?TRUE:FALSE);
          XtSetValues(xtWidget,args,1);
        }
      }
      // Callbacks :
      addCallbacks(aItemML,xtWidget,"activate",XmNvalueChangedCallback);

    } else if(xtClass==xmLabelWidgetClass) {
      if(aItemML.element_value("label",svalue)){
        XmSetString(xtWidget,XmNlabelString,svalue);
      }
      if(aItemML.element_value("pixmap",svalue)) {
        std::string fileName;
        inlib::file_name(svalue,fileName);
        XmSetLabelPixmap(xtWidget,fileName);
      }

    } else if(xtClass==xmCommandWidgetClass) {
      addCallbacks(aItemML,xtWidget,"activate",XmNcommandEnteredCallback);
    } else if((xtClass==xmTextWidgetClass)||
              (compoundId==xm_ScrolledTextClass)) {
      if(aItemML.element_value("value",svalue)) {
        bool as_it = false;
        std::string s;
        if(aItemML.element_atb_value("value","as_it",s)) {
          if(!inlib::to(s,as_it)) as_it = false;
        }
        if(!as_it) inlib::replace(svalue,"\\n","\n");
        XmTextSetString(xtWidget,(char*)svalue.c_str());
      }
      addCallbacks(aItemML,xtWidget,"activate",XmNactivateCallback);
    } else if(xtClass==commandLineWidgetClass) {
      addCallbacks(aItemML,xtWidget,"activate",XmNcommandEnteredCallback);
      addCallbacks(aItemML,xtWidget,"complete",XtNcompleteCallback);

#ifdef ONX_HAS_INVENTOR
    } else if( (compoundId==xm_PageViewerClass) ||
               (compoundId==xm_PlanePageViewerClass) ){

      if(aItemML.element_value("geometry",svalue)) {
        int x,y,w,h;
        if(!to_geometry(svalue,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << "XtUI::createNativeWidget :"
              << " \"" << svalue << "\" : bad value for geometry." 
              << std::endl;
        } else {
          Arg args[2];
          XtSetArg(args[0],XmNwidth,w);
          XtSetArg(args[1],XmNheight,h);
          XtSetValues(xtWidget,args,2);
        }
      }

      if(aItemML.element_value("popupItems",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++)
          PageViewerAddPopupEntry(xtWidget,items[index]);
      }

      PageViewerSetVerboseLevel(xtWidget,fVerboseLevel);

      addCallbacks(aItemML,xtWidget,"rectangularCollect",XtNrectangularCollectCallback);
      addCallbacks(aItemML,xtWidget,"collect",XtNcollectCallback);
      addCallbacks(aItemML,xtWidget,"popup",XtNpopupCallback);
      addCallbacks(aItemML,xtWidget,"update","");

#endif
    } else if(xtClass==passwordWidgetClass) {
      addCallbacks(aItemML,xtWidget,"activate",XtNpasswordEnteredCallback);
    } else if(xtClass==openGLAreaWidgetClass) {
      if(aItemML.element_value("width",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          Arg args[1];
          XtSetArg(args[0],XmNwidth,value);
          XtSetValues(xtWidget,args,1);
        }
      }
      if(aItemML.element_value("height",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          Arg args[1];
          XtSetArg(args[0],XmNheight,value);
          XtSetValues(xtWidget,args,1);
        }
      }
      addCallbacks(aItemML,xtWidget,"paint",XoNpaintCallback);
      addCallbacks(aItemML,xtWidget,"event",XoNeventCallback);
    } else if(xtClass==optionMenuWidgetClass) {
      if(aItemML.element_value("label",svalue)) {
        XmSetString(OptionMenuGetLabel(xtWidget),XmNlabelString,svalue);
      }
      std::string def;
      aItemML.element_value("default",def);
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        OptionMenuInitialize(xtWidget,items,def);
      }
      addCallbacks(aItemML,xtWidget,"valueChanged",XmNvalueChangedCallback);

    } else if(compoundId==xm_TabStackClass) {
        
#ifdef HAS_XM_TABSTACK
      addCallbacks(aItemML,xtWidget,"select",XmNtabSelectedCallback);
#else
      addCallbacks(aItemML,xtWidget,"select",XtNcallback);
#endif
  
    } else if(compoundId==xm_ScrolledTreeClass) {

      addCallbacks(aItemML,xtWidget,"select",XtNhighlightCallback);
      addCallbacks(aItemML,xtWidget,"selectBranch",XtNhighlightCallback);
      addCallbacks(aItemML,xtWidget,"open",XtNactivateCallback);
      addCallbacks(aItemML,xtWidget,"close",XtNactivateCallback);
      //addCallbacks(aItemML,xtWidget,"menu",
      //                   XtNmenuCallback);

    } else if(compoundId==xm_ScrolledListClass) {

      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        int xmsn;
        XmString* xmss = XmStringTableCreateFromText(items,xmsn);
        XmListDeleteAllItems(xtWidget);
        XmListDeselectAllItems(xtWidget);
        XmListAddItems(xtWidget,xmss,xmsn,0);
        XmStringTableDelete(xmss);
      }

      if(aItemML.element_value("selection",svalue)) {
        std::vector<std::string> text;
        text.push_back(svalue);
        int itemn;
        XmString* items = XmStringTableCreateFromText(text,itemn);
        Arg args[2];
        XtSetArg(args[0],XmNselectedItems,items);
        XtSetArg(args[1],XmNselectedItemCount,itemn);
        XtSetValues(xtWidget,args,2);
        XmStringTableDelete(items);
      }

      addCallbacks(aItemML,xtWidget,"select",XmNsingleSelectionCallback);

    } else if(compoundId==xm_FileSelectionDialogClass) {
      if(XtIsSubclass(XtParent(xtWidget),xmDialogShellWidgetClass)) {
        std::string stitle;
        aItemML.element_value("title",stitle);
        XWidgetSetString(XtParent(xtWidget),XtNtitle,stitle);
      }

      addCallbacks(aItemML,xtWidget,"ok",XmNokCallback);
      addCallbacks(aItemML,xtWidget,"cancel",XmNcancelCallback);

      if(aItemML.element_value("dirMask",svalue)) {
        inlib::expand_vars(svalue);
        std::vector<std::string> items;
        inlib::words(svalue,";",false,items);
        //FIXME : handle multiple wildcards
        if(items.size()) XmSetString(xtWidget,XmNdirMask,items[0]);
      }
      if(aItemML.element_value("directory",svalue)) {
        inlib::expand_vars(svalue);
        XmSetString(xtWidget,XmNdirectory,svalue);
      }
      /* default selection ?
      if(aItemML.element_value("textString",svalue)) {
        XmSetString(xtWidget,XmNtextString,svalue);
      }
      */
    } else if(compoundId==xm_ColorSelectionDialogClass) {
      addCallbacks(aItemML,xtWidget,"ok",XmNapplyCallback);
    } else if(compoundId==xm_ComboBoxClass) {
      //    } else if(xtClass==xmComboBoxWidgetClass) {
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        XmComboBoxSetItems(xtWidget,items);
      }
      if(aItemML.element_value("default",svalue)) {
        XmComboBoxSetValue(xtWidget,svalue);
      }
      addCallbacks(aItemML,xtWidget,"valueChanged",XmNselectionCallback);
    } else if((compoundId==xm_VScrollBarClass) ||
              (compoundId==xm_HScrollBarClass) ||
              (compoundId==xm_VScaleClass) ||
              (compoundId==xm_HScaleClass) )  {
      int mn = 0;
      if(aItemML.element_value("minimum",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          mn = value;
        }
      }
      int mx = 1000;
      if(aItemML.element_value("maximum",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          mx = value;
        }
      }
      int val = 0;
      if(aItemML.element_value("value",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          val = value;
        }
      }
      Arg args[3];
      int sliderSize;
      XtSetArg(args[0],XmNsliderSize,&sliderSize);
      XtGetValues(xtWidget,args,1);
      XtSetArg(args[0],XmNminimum,mn);
      if((compoundId==xm_VScaleClass) ||
         (compoundId==xm_HScaleClass) )  {
        XtSetArg(args[1],XmNmaximum,mx);
      } else {
        XtSetArg(args[1],XmNmaximum,mx+sliderSize);
      }
      XtSetArg(args[2],XmNvalue,val);
      XtSetValues(xtWidget,args,3);
      // Callbacks :        
      if((compoundId==xm_VScrollBarClass) ||
         (compoundId==xm_HScrollBarClass)) {
        addCallbacks(aItemML,xtWidget,"valueChanged",XmNvalueChangedCallback);
        addCallbacks(aItemML,xtWidget,"increment",XmNincrementCallback);
        addCallbacks(aItemML,xtWidget,"decrement",XmNdecrementCallback);
      }
      addCallbacks(aItemML,xtWidget,"drag",XmNdragCallback);
    }

    // Constraint resources :
    if(xtParent) {
      if(XtClass(xtParent)==xmPanedWindowWidgetClass) {
        std::string splitPos;
        if(aParent->element_value("splitPosition",splitPos)) {
          Arg args[1];
          XtSetArg(args[0],XmNallowResize,True);
          XtSetValues(xtContainer,args,1);
          int value;
          if(inlib::to<int>(splitPos,value)) {
            //FIXME : GB : does not work properly at creation.
            //XmPanedSetSplitPosition(xtParent,value);
          }
        }
        // The below permits to have a good comportement
        // of children when passing the shell widget to 
        // full screen when displaying on a Mac or an Exceed.
        Arg args[1];
        XtSetArg(args[0],XmNpaneMaximum,10000);
        XtSetValues(xtContainer,args,1);
#ifdef HAS_XM_TABSTACK
      } else if(XtClass(xtParent)==xmTabStackWidgetClass) {
        if(!aItemML.element_value("tabLabel",svalue)) svalue = aName;
        XmSetString(xtContainer,XmNtabLabelString,svalue);
#else
      } else if(XtClass(xtParent)==tabsWidgetClass) {
        if(!aItemML.element_value("tabLabel",svalue)) svalue = aName;
        XWidgetSetString(xtContainer,XtNtabLabel,svalue,true);
#endif
      }
    }

    // Very general resources :
    if(aItemML.element_value("background",svalue)) {
      Pixel pixel = XWidgetConvertStringToPixel(xtWidget,svalue);
      Arg args[1];
      XtSetArg(args[0],XtNbackground,pixel);
      XtSetValues(xtWidget,args,1);
    }
    if(aItemML.element_value("foreground",svalue)) {
      Pixel pixel = XWidgetConvertStringToPixel(xtWidget,svalue);
      Arg args[1];
      XtSetArg(args[0],XtNforeground,pixel);
      XtSetValues(xtWidget,args,1);
    }


    bool status = createChildren(aItemML);

    // Post children create resources :
    if(compoundId==xm_VContainerClass) {
      if(!aItemML.element_value("pack",svalue)) svalue = "uniform";
      if(svalue=="up") {
        VContainerPackUp(xtWidget);
      } else { // uniform
        VContainerPackUniform(xtWidget);
      }
    } else if(compoundId==xm_HContainerClass) {
      /*FIXME
      if(!aItemML.element_value("pack",svalue)) svalue = "uniform";
      if(svalue=="uniform") {
        VContainerAttachChildren(xtWidget);
      } else { // uniform
      }*/
      HContainerPackUniform(xtWidget);
      /*
    } else if(compoundId==xm_VBoxClass) {
      if(!aItemML.element_value("expand",svalue)) svalue = "second";
      if(!VBoxAttachChildren(xtWidget,svalue)) return false;
    } else if(compoundId==xm_HBoxClass) {
      if(!aItemML.element_value("expand",svalue)) svalue = "second";
      if(!HBoxAttachChildren(xtWidget,svalue)) return false;
      */

    }

    return status;
}

bool OnX::XtUI::destroyNativeWidget(inlib::xml::tree& aItemML){
  ::Widget widget = (::Widget)aItemML.get_data2();
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::XtUI::destryNativeWidget : %lu\n",widget);
  ::printf("debug : OnX::XtUI::destryNativeWidget : %lu name %s\n",
     widget,XtName(widget));
#endif
  if(!widget) return false;
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "XtUI::destroyNativeWidget :"
        << " destroy Xt widget \"" 
        << std::string(XtName(widget)) << "\"." << std::endl;
  }
  if(XtIsShell(widget)) {
    fWidgets.remove(widget);
    XtPopdown(widget);
  }
  ::Widget parent = XtParent(widget);
  if(parent && XtIsShell(parent)) { // dialog_popup.
    fWidgets.remove(parent);
    XtPopdown(parent);
  }
  // WARNING : 
  //  If executed within a Xt callback, the 
  //  number of children may be still the same after the
  //  call to XtDestroyWidget. Someone then must execute
  //  the script in "background" (out of the Xt callback)
  //  in order to get the correct number of children 
  //  in the parent.
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::XtUI::destryNativeWidget : %lu %s : XtDestroy...\n",
     widget,XtName(widget));
#endif
  XtDestroyWidget(widget);
  //XWidgetDumpChildren(parent);

#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::XtUI::destryNativeWidget : %lu : XtDestroy done.\n",
     widget);
#endif

  removeInRelations(aItemML);
  aItemML.invalidate();

  return true;
}

int OnX::XtUI::getCompound(const std::string& aName){
       if(aName=="MenuBar")        return xm_MenuBarClass;
  else if(aName=="Menu")           return xm_MenuClass;
  else if(aName=="FileSelection")  return xm_FileSelectionDialogClass;
  else if(aName=="ScrolledText")   return xm_ScrolledTextClass;
  else if(aName=="ScrolledList")   return xm_ScrolledListClass;
  else if(aName=="VBox")           return xm_VBoxClass;
  else if(aName=="HBox")           return xm_HBoxClass;
  else if(aName=="HPaned")         return xm_HPanedClass;
  else if(aName=="ScrolledTree")   return xm_ScrolledTreeClass;
  else if(aName=="ColorSelection") return xm_ColorSelectionDialogClass;
  else if(aName=="VContainer")     return xm_VContainerClass;
  else if(aName=="HContainer")     return xm_HContainerClass;
  else if(aName=="VScrollBar")     return xm_VScrollBarClass;
  else if(aName=="HScrollBar")     return xm_HScrollBarClass;
  else if(aName=="VScale")         return xm_VScaleClass;
  else if(aName=="HScale")         return xm_HScaleClass;
  else if(aName=="ComboBox")       return xm_ComboBoxClass;
  else if(aName=="ToolBar")        return xm_ToolBarClass;
  else if(aName=="WarningDialog")  return xm_WarningDialogClass;
  else if(aName=="Window")         return xm_Window;
  else if(aName=="TabStack")       return xm_TabStackClass;
#ifdef ONX_HAS_INVENTOR
  else if(aName=="SoExaminerViewer")    return xm_SoExaminerViewerClass;
  else if(aName=="SoPlaneViewer")       return xm_SoPlaneViewerClass;
  else if(aName=="SoFlyViewer")         return xm_SoFlyViewerClass;
  else if(aName=="SoWalkViewer")        return xm_SoWalkViewerClass;
  else if(aName=="PageViewer")          return xm_PageViewerClass;
  else if(aName=="PlanePageViewer")     return xm_PlanePageViewerClass;
#endif
  return 0;
}

WidgetClass OnX::XtUI::getType(const std::string& aName){
       if(aName=="Label")          return xmLabelWidgetClass;
  else if(aName=="CascadeButton")  return xmCascadeButtonWidgetClass;
  else if(aName=="MenuItem")       return xmPushButtonWidgetClass;
  else if(aName=="ToggleMenuItem") return xmToggleButtonWidgetClass;
  else if(aName=="PushButton")     return xmPushButtonWidgetClass;
  else if(aName=="ToggleButton")   return xmToggleButtonWidgetClass;
  else if(aName=="VPaned")         return xmPanedWindowWidgetClass;

  else if(aName=="CommandLine")    return commandLineWidgetClass;
  else if(aName=="OptionMenu")     return optionMenuWidgetClass;
  else if(aName=="ProgressBar")    return xrhpProgressWidgetClass;
  else if(aName=="Password")       return passwordWidgetClass;
  else if(aName=="OpenGLArea")     return openGLAreaWidgetClass;

  else if(aName=="Dialog")         return xmDialogShellWidgetClass;
  else if(aName=="Entry")          return xmTextWidgetClass;
  else if(aName=="ToolBarItem")    return xmPushButtonWidgetClass;
  else if(aName=="MenuSeparator")  return xmSeparatorWidgetClass;

  return 0;
}

std::string OnX::XtUI::getCallbackName(WidgetClass aClass,const std::string& aName){
  if (aName == "activate") {
    if( (aClass == xmCascadeButtonWidgetClass) ||
        (aClass == xmPushButtonWidgetClass) ) {
      return XmNactivateCallback;
    } else if (aClass == xmToggleButtonWidgetClass) {
      return XmNvalueChangedCallback;
    } else if (aClass == commandLineWidgetClass) {
      return XmNcommandEnteredCallback;
    } else if (aClass == passwordWidgetClass) {
      return XtNpasswordEnteredCallback;
    }
  } else if (aName == "valueChanged") {
    if ((aClass == xmScrollBarWidgetClass) ||
        (aClass == optionMenuWidgetClass)){
      return XmNvalueChangedCallback;
    } else if (aClass == xmComboBoxWidgetClass) {
      return XmNselectionCallback;
    }
  } else if (aName == "select") {
    if (aClass == listtreeWidgetClass) {
      return XtNhighlightCallback;
#ifdef HAS_XM_TABSTACK
    } else if (aClass == xmTabStackWidgetClass) {
      return XmNtabSelectedCallback;
#else
    } else if (aClass == tabsWidgetClass) {
      return XtNcallback;
#endif
    } else if (aClass == xmListWidgetClass) {
      return XmNsingleSelectionCallback;
    }
  } else if (aName == "ok") {
    if(aClass==colorEditorWidgetClass) {
      return XmNapplyCallback;
    } else {
      return XmNokCallback;
    }
  }
  // unique callback name
  else if (aName == "complete") return XtNcompleteCallback;
#ifdef ONX_HAS_INVENTOR
  else if (aName == "collect") return XtNcollectCallback;
#endif
  else if (aName == "popup") return XtNpopupCallback;
  else if (aName == "paint") return XoNpaintCallback;
  else if (aName == "event") return XoNeventCallback;
  else if (aName == "selectBranch") return XtNhighlightCallback;
  else if (aName == "open") return XtNactivateCallback;
  else if (aName == "close") return XtNactivateCallback;
  else if (aName == "cancel") return XmNcancelCallback;
  else if (aName == "drag") return XmNdragCallback;
  else if (aName == "increment") return XmNincrementCallback;
  else if (aName == "decrement") return XmNdecrementCallback;
  else if (aName == "drag") return XmNdragCallback;
  
  return "";
}

OnX::Action* OnX::XtUI::addCallback(
 inlib::xml::tree& aItemML
,const std::string& aNameXML
,const std::string& aInterp
,const std::string& aScript
){
  ::Widget nativeWidget = (::Widget)aItemML.get_data1(); 
  if(!nativeWidget) return 0;
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  Action::Parameters params;
  Action* action = 
    new Action(*this,widget,nativeWidget,aInterp,aScript,params,aNameXML,0);
  addAction(action);
  std::string cbk = getCallbackName(XtClass(nativeWidget),aNameXML);
  if(cbk.empty()) return 0;
  XtAddCallback(nativeWidget,cbk.c_str(),callback,(XtPointer)action);
  return action;
}

void OnX::XtUI::deleteCallback(
 inlib::xml::tree& aItemML
,const std::string& aNameXML
,Action* aAction
){
  //FIXME : check combo.
  ::Widget widget = (::Widget)aItemML.get_data1();
  if(!widget) return;
  std::string cbk = getCallbackName(XtClass(widget),aNameXML);
  if(cbk.empty()) return;
  XtRemoveCallback(widget,cbk.c_str(),callback,(XtPointer)aAction);
}

void OnX::XtUI::addCallbacks(
 inlib::xml::tree& aItemML
,::Widget aNativeWidget
,const std::string& aNameXML
,const std::string& aNameGUI
){
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    if(aNameXML==(*it)->name()) {
      std::string sexec;
      (*it)->attribute_value("exec",sexec);
      Action* action = new Action(*this,widget,aNativeWidget,
                                  sexec,(*it)->value(),(*it)->attributes(),
                                  aNameXML,0);
      addAction(action);
      if(aNameGUI.size())
        XtAddCallback
          (aNativeWidget,aNameGUI.c_str(),callback,(XtPointer)action);
    }
  }
}

void OnX::XtUI::destroyCallback(::Widget aWidget,XtPointer aTag,XtPointer){
#ifdef DEBUG_DESTROY
  ::printf("debug : destroyCallback : %lu %s\n",aWidget,XtName(aWidget));
#endif
  OnX::Xt::Destroy* d = (OnX::Xt::Destroy*)aTag; 
  // fItemML may be no more valid at this point...
  if(d->isValid()) {
    d->fUI.removeInRelations(d->fItemML);
    d->fItemML.invalidate();
  }
  d->fUI.removeActions(aWidget);

  delete d;
}

void OnX::XtUI::closeCallback(::Widget aWidget,XtPointer aTag,XtPointer){
#ifdef DEBUG_DESTROY
  ::printf("debug : closeCallback : %lu %s\n",aWidget,XtName(aWidget));
#endif
  OnX::Xt::Destroy* d = (OnX::Xt::Destroy*)aTag; 
  delete d;
}

void OnX::XtUI::destroyShellCallback(::Widget aWidget,XtPointer aTag,XtPointer){
#ifdef DEBUG_DESTROY
  ::printf("debug : destroyShellCallback : %lu %s\n",aWidget,XtName(aWidget));
#endif
  OnX::XtUI* ui = (OnX::XtUI*)aTag; 
  ui->fWidgets.remove(aWidget);
}

void OnX::XtUI::destroyConsoleTextCallback(::Widget aWidget,XtPointer aTag,XtPointer){
  OnX::XtUI* ui = (OnX::XtUI*)aTag; 
  ui->fConsoleText = 0;
}

void OnX::XtUI::destroyWarningDialogCallback(::Widget aWidget,XtPointer aTag,XtPointer){
#ifdef DEBUG_DESTROY
  ::printf("debug : destroyWarningDialogCallback : %lu %s\n",aWidget,XtName(aWidget));
#endif
  OnX::XtUI* ui = (OnX::XtUI*)aTag; 
  ui->fWarningDialog = 0;
}

void OnX::XtUI::callback(::Widget aWidget,XtPointer aTag,XtPointer aData){
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::XtUI::callback : begin : %lu %s\n",aWidget,XtName(aWidget));
#endif

  Action* action =(Action*)aTag;
  if(!action) return;

  XtUI* This = INLIB_CAST(action->ui(),OnX::XtUI);
  if(!This) return;
  
  std::string svalue;
  std::string sevent;
  int cbkx = 0;
  int cbky = 0;

  // Retreive "value" :

  ::Widget xtWidget = (::Widget)action->nativeWidget();
  WidgetClass xtClass = XtClass(xtWidget);
  if(xtClass==xmCommandWidgetClass) {
    if(aData) {
      XmConvertString(((XmCommandCallbackStruct*)aData)->value,svalue);
    }
  } else if(xtClass==xmListWidgetClass) {
    if(aData) {
      XmListCallbackStruct* data = (XmListCallbackStruct*)aData;
      if(data->reason==XmCR_SINGLE_SELECT) {
        XmConvertString(data->item,svalue);
      }
    }
  } else if(xtClass==commandLineWidgetClass) {
    if(aData) {
      char* ss = ((CommandLineCallbackStruct*)aData)->value;
      svalue = ss?ss:"";
      // ss free by the callback.
    }
  } else if(xtClass==passwordWidgetClass) {
    if(aData) {
      svalue = ((PasswordCallbackStruct*)aData)->value;
    }
  } else if(xtClass==optionMenuWidgetClass) {
    if(aData) {
      svalue = ((OptionMenuCallbackStruct*)aData)->value;
    }
  } else if(xtClass==xmToggleButtonWidgetClass) {
    if(aData) {
      svalue = ((XmToggleButtonCallbackStruct*)aData)->set ? "true" : "false";
    }
  } else if(xtClass==xmFileSelectionBoxWidgetClass) {
    if(aData) {
      XmFileSelectionBoxCallbackStruct* data = 
        (XmFileSelectionBoxCallbackStruct*)aData;
      if( (data->reason==XmCR_OK) || (data->reason==XmCR_APPLY) ) {
        XmConvertString(data->value,svalue);
      }
    }
  } else if(xtClass==colorEditorWidgetClass) {
    double r,g,b;
    ColorEditorGetColor(xtWidget,&r,&g,&b);
    inlib::sprintf(svalue,64,"%g %g %g",r,g,b);
  } else if(xtClass==xmScrollBarWidgetClass) {
    if(aData) {        
      XmScrollBarCallbackStruct* data = (XmScrollBarCallbackStruct*)aData;
      inlib::sprintf(svalue,64,"%d",data->value);
    }
  } else if(xtClass==xmScaleWidgetClass) {
    if(aData) {        
      XmScaleCallbackStruct* data = (XmScaleCallbackStruct*)aData;
      inlib::sprintf(svalue,64,"%d",data->value);
    }
#ifdef ONX_HAS_INVENTOR
  } else if(xtClass==pageViewerWidgetClass) {
    if(aData) {        
      PageViewerCallbackStruct* data = (PageViewerCallbackStruct*)aData;
      if(data->reason==XtCR_POPUP) {
        cbkx = ((XButtonEvent*)data->event)->x;
        cbky = ((XButtonEvent*)data->event)->y;
      }
      svalue = data->value;
    }
#endif
  } else if(xtClass==xmComboBoxWidgetClass) {
    if(aData) {        
      XmConvertString(((XmComboBoxCallbackStruct*)aData)->item_or_text,svalue);
    }
#ifdef HAS_XM_TABSTACK
  } else if(xtClass==xmTabStackWidgetClass) {
    if(aData) {        
      ::Widget child = ((XmTabStackCallbackStruct*)aData)->selected_child;
      if(child) {
        XmGetString(child,XmNtabLabelString,svalue);
      }
    }
#else
  } else if(xtClass==tabsWidgetClass) {
    if(aData) {        
      ::Widget child = (::Widget)aData;
      if(child) {
        svalue = XWidgetGetString(child,XtNtabLabel);
      }
    }
#endif
  } else if(xtClass==xmTextWidgetClass) {
    char* s = XmTextGetString(xtWidget);
    svalue = s ? s : "";
    XtFree(s);
  } else if(xtClass==listtreeWidgetClass) {

    if(aData) {
      ListTreeActivateStruct* data = (ListTreeActivateStruct*)aData;
      if(data->reason==XtBRANCH) {
        if(data->open==True) {  //open 
          if(action->name()!="open") return;
          svalue = ListTreeItemGetPath(data->item);
        } else {  //close
          if(action->name()!="close") return;
          svalue = ListTreeItemGetPath(data->item);
        }
      } else if(data->reason==XtLEAF) {
        // should not pass here.
        return;
      } else { //select
        bool branch;
        svalue = ListTreeGetSelection(aWidget,branch);
        if(branch) { //select branch
          if(action->name()!="selectBranch") return;
        } else { //select leaf
          if(action->name()!="select") return;
        }
      }
    }

  } else if(xtClass==openGLAreaWidgetClass) {
    if(aData) {        
      XoAnyCallbackStruct* data = (XoAnyCallbackStruct*)aData;
      if( (data->reason==XoCR_EVENT) && data->event ) {
        switch(data->event->type) {
          case ButtonPress:
            sevent = "ButtonPress";
            cbkx = ((XButtonEvent*)data->event)->x;
            cbky = ((XButtonEvent*)data->event)->y;
            break;
          case ButtonRelease:
            sevent = "ButtonRelease";
            cbkx = ((XButtonEvent*)data->event)->x;
            cbky = ((XButtonEvent*)data->event)->y;
            break;
          case MotionNotify:{
            sevent = "MotionNotify";
            cbkx = ((XMotionEvent*)data->event)->x;
            cbky = ((XMotionEvent*)data->event)->y;
          }break;
        }
      }
    }
  }

  std::string name = XtName(xtWidget);

  if(This->session().verboseLevel()) {
    std::ostream& out  = This->session().out();
    out << "OnX::XtUI::callback :"
        << " component " << inlib::sout(name)
        << " type " << inlib::sout(XClassGetName(xtClass))
        << " value " << inlib::sout(svalue)
        << " event " << sevent 
        << " x " << cbkx
        << " y " << cbky
        << "."        << std::endl;
  }

  This->fCallbackData = 
    CallbackData(action->widget(),name,svalue,sevent,cbkx,cbky);

  // Execute script :
  // the action may delete widget that may do a ui.removeActions
  // that may then delete action ! We then execute the script
  // with a local copy of the action.
  OnX::Action tmp(*action);
  tmp.execute(); //FIXME : ret value.

  This->fCallbackData = CallbackData();

#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::XtUI::callback : end : %lu %s\n",aWidget,XtName(aWidget));
#endif
}

void OnX::XtUI::warningCallback(::Widget,XtPointer aTag,XtPointer){
  XtUI* This = (XtUI*)aTag; 
  This->notify(new Lib::ExitMessage());
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Slash::UI::ICyclic* OnX::XtUI::createCyclic(
 const std::string& aName
,int aDelay
,const std::string& aInterp
,const std::string& aScript
){
  Slash::UI::ICyclic* cyclic = new XtCyclic(fSession,aName,
                                 aDelay,aInterp,aScript,fAppContext);
  fCyclics.push_back(cyclic);
  return cyclic;
}

/*
Boolean KeyDispatchEvent(
 XEvent* aEvent
){
  if(aEvent->type!=KeyPress) return FALSE;
  KeySym keySym;
  XLookupString(&(aEvent->xkey),NULL,0,&keySym,NULL);
  //if(keySym!=XK_Tab) return;
  if( (keySym!=XK_Alt_L) && (keySym!=XK_Alt_R) ) return FALSE;
  //printf("debug : Alt key\n");
  //FIXME : should search by type and not by name.
  //FIXME : should search directly the first cascade button.
  //Widget menuBar = (::Widget)findNativeWidget("menuBar");
  //if(!menuBar) return FALSE;
  //Widget child = XWidgetGetFirstChild(menuBar);
  //if(!child) return FALSE;
  //printf("debug : hello2 : %s %s\n",
  //XtName(child),XClassName(XtClass(child)));
  return TRUE;
}

void KeyHandler (
 ::Widget aWidget
,XtPointer aTag
,XEvent* aEvent
,Boolean* aDispatch
){
  KeySym keySym;
  XLookupString(&(aEvent->xkey),NULL,0,&keySym,NULL);
  //if(keySym!=XK_Tab) return;
  if( (keySym!=XK_Alt_L) && (keySym!=XK_Alt_R) ) return;
  // How to activate the first pdm ?
  ::Widget child = XWidgetGetFirstChild(aWidget);
  if(!child) return;
  printf("debug : hello2 : %s %s\n",XtName(child),XClassName(XtClass(child)));
  //XtCallCallbacks(child,XmNactivateCallback,NULL);
}
*/

//////////////////////////////////////////////////////////////////////////////
//// BaseUI pure virtual implementation //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

std::string OnX::XtUI::widgetName(inlib::xml::tree& aItemML){
  ::Widget widget = (::Widget)aItemML.get_data1();
  if(!widget) return "";
  char* cname = XtName(widget);
  return std::string(cname?cname:"");
}

#include <Xm/Display.h>
#include <Xm/DialogS.h>
#include <Xm/DragOverS.h>
#include <Xm/GrabShell.h>
#ifdef PRINTING_SUPPORTED
#include <Xm/Print.h>
#endif
#include <X11/IntrinsicP.h>
static void ResetVendorField(WidgetClass);

void LookDSM_Problem(std::ostream& a_out){
  // In some situation we could have at startup the Xt error message :
  //   Error: attempt to add non-widget child "dsm" to parent...
  // This appears, for example, when the link order is wrong
  // between Xt and Xm (-lXt -lXm). It appears also when 
  // loading DLLs when a first DLLL linked with X11 had been
  // loaded before the one linked with Motif.
  //  The intrinsic problem is a bad initialization of some class 
  // structure (like XmDisplayClassRec) concerning the ancestor
  // VendorShellWidgetClass.
  //  Each of the libXt and libXm have a vendorShellWidgetClass
  // pointer but it may happen that, for exa, the XmDisplayClassRec 
  // is initialized with the vendorShellWidgetClass of liXt instead of libXm.
  //  Then the composite_class.extension of XmDisplayClassRec
  // will have no extension and then the XtCreateWidget will
  // fail when trying to add a DSM to a XmDisplay widget ; a
  // thing that happens internaly at any Motif widget creation.
  //  Below, we try to detect a bad initialization of XmDisplayClassRec
  // (and others) and try to restore the right vendorShellWidgetClass of Motif.
  if(!vendorShellWidgetClass->core_class.class_part_initialize) {
    // The vendorShellWidgetClass we get is the Xt one.
    a_out << "OnX::XtUI::LookDSM_Problem :" << std::endl;
    a_out << " Can't solve the dsm problem." << std::endl;
    a_out << " Bad linking order of -lXm and -lXt " << std::endl;
    a_out << " or you try to load a DLL linked to X11 prior to" << std::endl;
    a_out << " load a DLL linked with Motif." << std::endl;
    return;
  }
  /*
    a_out << "OnX::XtUI::LookDSM_Problem : WARNING : " << std::endl;
    a_out << " Some Motif class structures touched in order" << std::endl;
    a_out << " to avoid the Motif dsm problem at startup." << std::endl;
  */
  ResetVendorField(xmDisplayClass);
  ResetVendorField(xmDialogShellWidgetClass);
  ResetVendorField(xmDragOverShellWidgetClass);
  ResetVendorField(xmGrabShellWidgetClass);
#ifdef PRINTING_SUPPORTED
  ResetVendorField(xmPrintShellWidgetClass);
#endif
}

void ResetVendorField(WidgetClass aWidgetClass){
  WidgetClass wc = aWidgetClass;
  while(wc) {
    WidgetClass super = wc->core_class.superclass;
    if(super) {
      std::string name = (char*)super->core_class.class_name;
      if( (name=="VendorShell") && (super!=vendorShellWidgetClass) ) {
        // The Vendor super class is not the Motif one, override :
        wc->core_class.superclass = vendorShellWidgetClass;
        return;
      }
    }
    wc = super;
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <OnX/Core/Term.h>

namespace OnX {
class TermCyclic 
:public XtCyclic
,public Term {
public: //Slash::UI::ICyclic
  virtual void execute() { cyclicExecute(fUI,fInterpreter); }
public:
  inline TermCyclic(XtUI& aUI,
             const std::string& aPrompt,
             const std::string& aInterp)
  //10 is millisec.
  :XtCyclic(aUI.session(),"onx_terminal",10,aInterp,"",aUI.appContext()) 
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

bool OnX::XtUI::enableTerminal(const std::string& aPrompt,const std::string& aInterp){
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

bool OnX::XtUI::disableTerminal(){
  if(!fTermCyclic) return true;
  delete fTermCyclic;
  fTermCyclic = 0;
  return true;
}

std::string OnX::XtUI::ask(const std::string& aPrompt){
  if(fAskToTerminal && fTermCyclic) {
    return fTermCyclic->ask(aPrompt);
  } else {
    return BaseUI::ask(aPrompt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#ifdef ONX_HAS_INVENTOR
#include "../Core/receive_scene.cxx"
#endif

bool OnX::XtUI::receiveScene(
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

/*
    //Display* display = XtDisplay(fXtUI.appWidget());
    //f_fd_X11 = XConnectionNumber(display);

#include <unistd.h>
#include <sys/time.h>
#define MAXIMUM(a,b) ((a)>(b)?(a):(b))

bool is_there_X11_input(int a_fd_X11,bool& aIs) {
  fd_set mask;
  FD_ZERO(&mask);
  FD_SET(a_fd_X11,&mask);

  static struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 10; //microsec

  int nfds = 0;
  nfds = MAXIMUM(nfds,a_fd_X11);
  nfds++;
  if(select(nfds,&mask,0,0,&timeout)==(-1)) {
    aIs = false;
    return false;
  }

  aIs = FD_ISSET(a_fd_X11,&mask)?true:false;
  return true;
}
*/

bool is_there_Xt_input(XtAppContext aContext,bool& aIs) {
  XtInputMask input = XtAppPending(aContext);
  aIs = input!=0?true:false;
  return true;
}


extern "C" {

void CloseHandler(Widget aWidget,XtPointer aTag,XEvent* aEvent,Boolean*){
  if(aEvent->type!=ClientMessage) return;
  Atom atom = XInternAtom(XtDisplay(aWidget),"WM_DELETE_WINDOW",False);
  if(aEvent->xclient.data.l[0]==atom) { 
#ifdef DEBUG_DESTROY
    ::printf("debug : CloseHandler : %lu %s\n",aWidget,XtName(aWidget));
#endif
    OnX::Xt::Destroy* d = (OnX::Xt::Destroy*)aTag; 
    if(d->isValid()) { 
#ifdef DEBUG_DESTROY
      ::printf("debug : CloseHandler : exec destroy cbks\n");
#endif
      OnX::Execute tag(d->fUI,"destroy");
      d->fItemML.post_execute_backward(OnX::BaseUI::executeItemMLCallbacks,&tag);

      inlib::xml::tree* item = &(d->fItemML);
      inlib::xml::tree* parent = item->parent();
      if(parent) parent->remove_child(item,true);
    }

    //delete d; //deleted by the closeCallback.
  }
}

}

