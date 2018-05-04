// This :
#include <OnX/Win/WinUI.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/UI/IWidget.h>

#include <inlib/cast>
#include <inlib/sprintf>
#include <inlib/system>
#include <exlib/xml/parser>

#include "../Core/inlib"

#include <Lib/Messages.h>
#include <Lib/Debug.h>

#include <OnX/Core/Widgets.h>

#include <OnX/Win/WinTk.h>
#include <OnX/Win/Composite.h>
#include <OnX/Win/WinCyclic.h>

#define ONX_HAS_INVENTOR
#ifdef ONX_HAS_INVENTOR
#include <Slash/Tools/RegionSceneHandler.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Win/SoWin.h>
#include <OnX/Inventor/Inventor.h>
#include <OnX/Inventor/HEPVis.h>
#include <OnX/Inventor/GuiViewer.h>
#include <OnX/Win/SoViewers.h>
#include <OnX/Win/PageViewer.h>
#endif

#include <OnX/Win/WinTools.h>
#include <windowsx.h>

// Handle some classID to avoid too much string comparison in createWidget.
#define win_Window                1
#define win_CommandLine           2
#define win_PushButton            3
#define win_VBox                  4
#define win_HBox                  5
#define win_MenuBar               6
#define win_CascadeButton         7
#define win_Menu                  8
#define win_MenuItem              9
#define win_Tree                 10
#define win_FileSelectionDialog  12
#define win_ScrolledList         13
#define win_ScrolledText         14
#define win_VPaned               15
#define win_HPaned               16
#define win_ToggleMenuItem       17
#define win_ColorSelectionDialog 18
#define win_Label                19
#define win_Dialog               20
#define win_VContainer           21
#define win_HContainer           22
#define win_Entry                23
#define win_ComboBox             24
#define win_VScrollBar           25
#define win_HScrollBar           26
#define win_ToolBar              27
#define win_ToolBarItem          28
#define win_MenuSeparator        29
#define win_ProgressBar          30
#define win_TabStack             31
#define win_OptionMenu           32
#define win_Password             33
#define win_VScale               34
#define win_HScale               35
#define win_OpenGLArea           36
#define win_ToggleButton         37
#define win_WarningDialog        38
#define win_SoExaminerViewer     39
#define win_SoPlaneViewer        40
#define win_SoFlyViewer          41
#define win_SoWalkViewer         41
#define win_PageViewer           42
#define win_PlanePageViewer      43

static void setFocusCallback(WinTk::Shell*,void*);
WinTk::Shell* getFocusedShellProc(void* aTag);

//#define DEBUG_DESTROY

namespace OnX {
namespace Win {
class Destroy {
public:
  Destroy(WinUI& aUI,inlib::xml::tree& aItemML):fUI(aUI),fItemML(aItemML){
    Lib::Debug::increment("OnX::Win::Destroy");
  }
  virtual ~Destroy(){
    Lib::Debug::decrement("OnX::Win::Destroy");
  }
  bool isValid() const {
    inlib::xml::tree* top = fUI.topItemML();
    return (top && fUI.is_in(*top,&fItemML));
  }
public:
  WinUI& fUI;
  inlib::xml::tree& fItemML;
};
}}

bool win_proxy
      (void*,const std::string&,const std::vector<std::string>&,void*&);

OnX::WinUI::WinUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs)
:BaseUI(aSession)
,fConsoleText(0)
,fActionID(0)
,fMessageWindow(0)
,fWarningDialog(0)
,fTermCyclic(0)
,fFocusedShell(0)
{
  //change attributs from WidgetClass
 {Slash::UI::IWidgetClass* vBoxWidgetClass = findWidgetClass("VBox");
  std::vector<std::string> attrs = vBoxWidgetClass->attributes();
  attrs.push_back("sizeOfFixed");
  vBoxWidgetClass->setAttributes(attrs);
  Slash::UI::IWidgetClass* hBoxWidgetClass = findWidgetClass("VBox");
  hBoxWidgetClass->setAttributes(attrs);}

  // Message window :
  fMessageWindow = new WinTk::MessageWindow();
  fMessageWindow->addCallback("send",messageCallback,this);

  // We need a window to init the graphic :
  fTopWidget = new WinTk::Shell(WS_OVERLAPPEDWINDOW);

  if(!fTopWidget->nativeWindow()) return;

#ifdef ONX_HAS_INVENTOR
  // Init Inventor and HEPVis :
  SoWin::init(fTopWidget->nativeWindow());
  HEPVis::initClasses();
  OnX::Inventor::initialize(aSession);

  if(!aSession.findManager(Slash_SceneHandler))  {
    aSession.addManager
      (new Slash::RegionSceneHandler(aSession,Slash_SceneHandler));
  }
#endif
}

OnX::WinUI::~WinUI() { 
  fBeingDestroyed = true;

  // Treat messages emitted between the quiting of the steering
  // and here. They may come from the destruction of various
  // managers in the ~Session.
  synchronize();

  removeCyclics();

  while(!fWidgets.empty()) {
    WinTk::Component* widget = *(fWidgets.rbegin());
    delete widget; //may induce a fWidget.remove(widget) !
    fWidgets.remove(widget);
  }

  for(std::list<Win::Destroy*>::iterator it=fDestroys.begin();
      it!=fDestroys.end();it=fDestroys.erase(it)) delete *it;

  synchronize(); //treat message from the close callbacks.

  fConsoleText = 0;
  fWarningDialog = 0;

  delete fMessageWindow;
  delete fTopWidget;

#ifdef ONX_HAS_INVENTOR
  OnX::Inventor::finalize(fSession);
#endif
}

void* OnX::WinUI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(OnX::WinUI)
  else return BaseUI::cast(a_class);
}

WinTk::Component* OnX::WinUI::topWidget() const {return fTopWidget;}

void OnX::WinUI::put(const std::string& aString){
  // Append line at end of report region.
  if(!fConsoleText) {
    fConsoleText = (WinTk::ScrolledText*)findNativeWidget("consoleText");
    if(fConsoleText) {
      fConsoleText->addCallback
        ("WM_DESTROY",WinUI::destroyConsoleCallback,this);
    }
  }
  if(!fConsoleText) {
    ::printf("%s",aString.c_str());
    return;
  }
  fConsoleText->appendString(aString);
  //HWND window = fConsoleText->nativeWindow();
  //if(window) WinTk::Show(window);
}

void OnX::WinUI::warn(const std::string& aString){
  // Modal method.
  if(!fWarningDialog) {
    fWarningDialog = (WinTk::WarningDialog*)findNativeWidget("warningDialog");
    if(fWarningDialog) {
      fWarningDialog->addCallback
        ("delete",WinUI::deleteWarningDialogCallback,this);
    }
  }
  if(!fWarningDialog) {
    echo("WARNING : "+aString);
    return;
  }
  fWarningDialog->setMessage(aString);
  fWarningDialog->show();
}

int OnX::WinUI::nativeSteer(){
  fSteerLevel++;
  int value = WinTk::steer(getFocusedShellProc,this);
  fSteerLevel--;
  return value;
}

void OnX::WinUI::synchronize(){
  MSG event;
  while ( ::PeekMessage(&event, NULL, 0, 0, PM_REMOVE) ) {
    ::TranslateMessage(&event);
    ::DispatchMessage (&event);
  }
}

void OnX::WinUI::exit(int aExitCode){::PostQuitMessage(aExitCode);}

bool OnX::WinUI::lock(){return false;}
bool OnX::WinUI::unlock(){return false;}

bool OnX::WinUI::notify(Slash::Core::IMessage* aMessage){
  if(!fMessageWindow) return false;
  return fMessageWindow->sendMessage(aMessage);
}

void OnX::WinUI::messageCallback(WinTk::Component&,WinTk::CallbackData& aData,void* aTag){
  OnX::WinUI* This = (OnX::WinUI*)aTag;
  This->notification((Slash::Core::IMessage*)aData.lparam);
}

bool OnX::WinUI::parameterValue(const std::string& aWhat,std::string& aValue){
  aValue = "";
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;


  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.out();
    out << "OnX::WinUI::parameterValue :"
        << " widget " << inlib::sout(aWidget)
        << " not found." << std::endl;
    return false;
  }

  WinTk::Component* widget = (WinTk::Component*)itemML->get_data1();
  if(widget) {
    if(aResource=="set") {
      if(widget->type()=="MenuItem") {
        WinTk::MenuItem* menuItem = (WinTk::MenuItem*)widget;
        if(menuItem->isToggle()) {
          aValue = (menuItem->isChecked() ? "true" : "false");
          return true;
        }
      } else if(widget->type()=="ToggleButton") {
        aValue = 
          (((WinTk::ToggleButton*)widget)->isChecked() ? "true" : "false");
        return true;
      }
    } else if(aResource=="label") {
      if(widget->type()=="Label") {
        aValue = ((WinTk::Label*)widget)->label();
        return true;
      }
    } else if(aResource=="tabLabel") {
      //Constraint resource, we have to work on the parent.
      WinTk::Component* parent = widget->parent();
      if(parent&&(parent->type()=="TabStack")) {
        ((WinTk::TabStack*)parent)->label(*widget,aValue);
        return true;
      }
    } else if(aResource=="items") {
      if(widget->type()=="Tree") {
        HTREEITEM item = ((WinTk::Tree*)widget)->firstItem();
        if(item) {
          HWND hwnd = ((WinTk::Tree*)widget)->nativeWindow();
          aValue = "<tree>"+WinTk::TreeGetItemXML(hwnd,item)+"</tree>";
        } else {
          aValue = "";
        }
        return true;
      } else if(widget->type()=="ScrolledList") {
        aValue = 
          inlib::to_string(((WinTk::ScrolledList*)widget)->items(),"\n");
        return true;
      } else if(widget->type()=="OptionMenu") {
        std::vector<std::string> v;
        if(!((WinTk::OptionMenu*)widget)->items(v)) return false;
        aValue = inlib::to_string(v,"\n");
        return true;
      } else if(widget->type()=="ComboBox") {
        std::vector<std::string> v;
        if(!((WinTk::ComboBox*)widget)->items(v)) return false;
        aValue = inlib::to_string(v,"\n");
        return true;
      }

    } else if(aResource=="selection") {
      if(widget->type()=="TabStack") {
        if(((WinTk::TabStack*)widget)->selection(aValue)) return true;
      } else if(widget->type()=="ScrolledList") {
        if(((WinTk::ScrolledList*)widget)->selection(aValue)) return true;
      } else if(widget->type()=="Tree") {
        if(((WinTk::Tree*)widget)->selection(aValue)) return true;
      }
    } else if(aResource=="itemCount") {
      if(widget->type()=="TabStack") {
        int value  = ((WinTk::TabStack*)widget)->itemCount();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="value") {
      if(widget->type()=="OptionMenu") {
        aValue = ((WinTk::OptionMenu*)widget)->value();
        return true;
      } else if(widget->type()=="ComboBox") {
        aValue = ((WinTk::ComboBox*)widget)->value();
        return true;
      } else if(widget->type()=="CommandLine") {
        aValue = ((WinTk::CommandLine*)widget)->value();
        return true;
      } else if(widget->type()=="ScrolledText") {
        aValue = ((WinTk::ScrolledText*)widget)->value();
        return true;
      } else if(widget->type()=="Entry") {
        aValue = ((WinTk::Entry*)widget)->value();
        return true;
      } else if(widget->type()=="HScrollBar") {
        int value  = ((WinTk::HScrollBar*)widget)->value();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="VScrollBar") {
        int value  = ((WinTk::VScrollBar*)widget)->value();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="HScale") {
        int value  = ((WinTk::HScale*)widget)->value();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="VScale") {
        int value  = ((WinTk::VScale*)widget)->value();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="ProgressBar") {
        int value  = ((WinTk::ProgressBar*)widget)->value();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="minimum") {
      if(widget->type()=="HScrollBar") {
        int value  = ((WinTk::HScrollBar*)widget)->minimum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="VScrollBar") {
        int value  = ((WinTk::VScrollBar*)widget)->minimum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="HScale") {
        int value  = ((WinTk::HScale*)widget)->minimum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="VScale") {
        int value  = ((WinTk::VScale*)widget)->minimum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="maximum") {
      if(widget->type()=="HScrollBar") {
        int value  = ((WinTk::HScrollBar*)widget)->maximum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="VScrollBar") {
        int value  = ((WinTk::VScrollBar*)widget)->maximum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="HScale") {
        int value  = ((WinTk::HScale*)widget)->maximum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      } else if(widget->type()=="VScale") {
        int value  = ((WinTk::VScale*)widget)->maximum();
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    }
  }
  // Not found, then look in XML elements :
  if(aResource=="class") {
    return itemML->attribute_value("",aResource,aValue);
  }
  return itemML->element_value(aResource,aValue);
}

bool OnX::WinUI::setParameter(const std::string& aWhat,const std::string& aValue){
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  if(fVerboseLevel) {
    std::ostream& out = fSession.out();
    out << "OnX::WinUI::setParameter :"
        << " widget " << inlib::sout(aWidget)
        << " resource " << inlib::sout(aResource)
        << " value " << inlib::sout(aValue) << "."
        << std::endl;
  }

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.out();
    out << "OnX::WinUI::setParameter :"
        << " widget " << inlib::sout(aWidget)
        << " not found." << std::endl;
    return false;
  }

  WinTk::Component* widget = (WinTk::Component*)itemML->get_data1();
  if(!widget) {
    std::ostream& out = fSession.out();
    out << "OnX::WinUI::setParameter :"
        << " " << inlib::sout(aWidget)
        << " without a native widget." << std::endl;
    return false;
  }

  std::string svalue;

  if(aResource=="background") {
    double r,g,b;
    if(inlib::to_rgb(aValue,r,g,b)) {
      if(widget->setBackground(r,g,b)) return true;
    }
  } else if(aResource=="foreground") {
  } else if(aResource=="label") {
    if(widget->type()=="Label") {
      ((WinTk::Label*)widget)->setLabel(aValue);
      //itemML->setPropertyValue("label",aValue);
      return true;
    } else if(widget->type()=="CascadeButton") {
      ((WinTk::CascadeButton*)widget)->setLabel(aValue);
      //itemML->setPropertyValue("label",aValue);
      return true;
    } else if(widget->type()=="MenuItem") {
      ((WinTk::MenuItem*)widget)->setLabel(aValue);
      //itemML->setPropertyValue("label",aValue);
      return true;
    }
  } else if(aResource=="title") {
    if(widget->type()=="Shell") {
      ((WinTk::Shell*)widget)->setTitle(aValue);
      return true;
    }
  } else if(aResource=="tabLabel") {
    //Constraint resource, we have to work on the parent.
    WinTk::Component* parent = widget->parent();
    if(parent&&(parent->type()=="TabStack")) {
      if(((WinTk::TabStack*)parent)->setLabel(*widget,aValue)) {
        return true;
      }
    }
  } else if(aResource=="dirMask") {
    if(widget->type()=="FileSelectionDialog") {
      svalue = aValue;
      inlib::expand_vars(svalue);
      ((WinTk::FileSelectionDialog*)widget)->setDirMask(svalue);
      return true;
    }
  } else if(aResource=="directory") {
    if(widget->type()=="FileSelectionDialog") {
      svalue = aValue;
      inlib::expand_vars(svalue);
      ((WinTk::FileSelectionDialog*)widget)->setDirectory(svalue);
      return true;
    }
  } else if(aResource=="mode") {
    if(widget->type()=="FileSelectionDialog") {
      ((WinTk::FileSelectionDialog*)widget)->setMode(aValue);
      return true;
    }
  } else if(aResource=="set") {
    if(widget->type()=="MenuItem") {
      WinTk::MenuItem* menuItem = (WinTk::MenuItem*)widget;
      if(menuItem->isToggle()) {
        bool value;
        if(inlib::to(aValue,value)) {
          menuItem->setChecked(value);
          return true;
        }
      }
    } else if(widget->type()=="ToggleButton") {
      bool value;
      if(inlib::to(aValue,value)) {
        ((WinTk::ToggleButton*)widget)->setChecked(value);
        return true;
      }
    }
  } else if(aResource=="value") {
    if(widget->type()=="HScrollBar") {
      int mn = ((WinTk::HScrollBar*)widget)->minimum();
      int mx  = ((WinTk::HScrollBar*)widget)->maximum();
      int value;
      if(inlib::to<int>(aValue,value)) {
        if((value>=mn)&&(value<=mx)) {
          ((WinTk::HScrollBar*)widget)->setValue(value);
          return true;
        } else {
          std::ostream& out = fSession.out();
          out << "OnX::WinUI::setParameter :"
              << " value " << value 
              << " out of bounds (min=" << mn 
              << ", max=" << mx << ")." 
              << std::endl;
          return false;
        }
      }
    } else if(widget->type()=="VScrollBar") {
      int mn = ((WinTk::VScrollBar*)widget)->minimum();
      int mx  = ((WinTk::VScrollBar*)widget)->maximum();
      int value;
      if(inlib::to<int>(aValue,value)) {
        if((value>=mn)&&(value<=mx)) {
          ((WinTk::VScrollBar*)widget)->setValue(value);
          return true;
        } else {
          std::ostream& out = fSession.out();
          out << "OnX::WinUI::setParameter :"
              << " value " << value 
              << " out of bounds (min=" << mn 
              << ", max=" << mx << ")." 
              << std::endl;
          return false;
        }
      }
    } else if(widget->type()=="HScale") {
      int mn = ((WinTk::HScale*)widget)->minimum();
      int mx  = ((WinTk::HScale*)widget)->maximum();
      int value;
      if(inlib::to<int>(aValue,value)) {
        if((value>=mn)&&(value<=mx)) {
          ((WinTk::HScale*)widget)->setValue(value);
          return true;
        } else {
          std::ostream& out = fSession.out();
          out << "OnX::WinUI::setParameter :"
              << " value " << value 
              << " out of bounds (min=" << mn 
              << ", max=" << mx << ")." 
              << std::endl;
          return false;
        }
      }
    } else if(widget->type()=="VScale") {
      int mn = ((WinTk::VScale*)widget)->minimum();
      int mx  = ((WinTk::VScale*)widget)->maximum();
      int value;
      if(inlib::to<int>(aValue,value)) {
        if((value>=mn)&&(value<=mx)) {
          ((WinTk::VScale*)widget)->setValue(value);
          return true;
        } else {
          std::ostream& out = fSession.out();
          out << "OnX::WinUI::setParameter :"
              << " value " << value 
              << " out of bounds (min=" << mn 
              << ", max=" << mx << ")." 
              << std::endl;
          return false;
        }
      }
    } else if(widget->type()=="ProgressBar") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::ProgressBar*)widget)->setValue(value);
        return true;
      }
    } else if(widget->type()=="Entry") {
      ((WinTk::Entry*)widget)->setValue(aValue);
      return true;
    } else if(widget->type()=="CommandLine") {
      ((WinTk::CommandLine*)widget)->setValue(aValue);
      return true;
    } else if(widget->type()=="ScrolledText") {
      ((WinTk::ScrolledText*)widget)->setText(aValue);
      return true;
    } else if(widget->type()=="OptionMenu") {
      if(((WinTk::OptionMenu*)widget)->setDefault(aValue)) return true;
    } else if(widget->type()=="ComboBox") {
      if(((WinTk::ComboBox*)widget)->setValue(aValue)) return true;
    }
  } else if(aResource=="minimum") {
    if(widget->type()=="HScrollBar") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::HScrollBar*)widget)->setMinimum(value);
        return true;
      }
    } else if(widget->type()=="VScrollBar") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::VScrollBar*)widget)->setMinimum(value);
        return true;
      }
    } else if(widget->type()=="HScale") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::HScale*)widget)->setMinimum(value);
        return true;
      }
    } else if(widget->type()=="VScale") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::VScale*)widget)->setMinimum(value);
        return true;
      }
    }
  } else if(aResource=="maximum") {
    if(widget->type()=="HScrollBar") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::HScrollBar*)widget)->setMaximum(value);
        return true;
      }
    } else if(widget->type()=="VScrollBar") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::VScrollBar*)widget)->setMaximum(value);
        return true;
      }
    } else if(widget->type()=="HScale") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::HScale*)widget)->setMaximum(value);
        return true;
      }
    } else if(widget->type()=="VScale") {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((WinTk::VScale*)widget)->setMaximum(value);
        return true;
      }
    }
  } else if(aResource=="selection") {
    if(widget->type()=="TabStack") {
      if(((WinTk::TabStack*)widget)->setSelection(aValue)) return true;
    } else if(widget->type()=="ScrolledList") {
      if(((WinTk::ScrolledList*)widget)->setSelection(aValue)) return true;
    } else if(widget->type()=="Tree") {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      if(((WinTk::Tree*)widget)->setSelection(items)) return true;
    }
  } else if(aResource=="default") {
    if(widget->type()=="OptionMenu") {
      ((WinTk::OptionMenu*)widget)->setDefault(aValue);
      return true;
    } else if(widget->type()=="ComboBox") {
      ((WinTk::ComboBox*)widget)->setValue(aValue);
      return true;
    }
  } else if(aResource=="items") {
    if(widget->type()=="OptionMenu") {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      ((WinTk::OptionMenu*)widget)->initialize(items,"");
      return true;
    } else if(widget->type()=="ComboBox") {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      ((WinTk::ComboBox*)widget)->setItems(items);
      return true;
    } else if(widget->type()=="ScrolledList") {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      ((WinTk::ScrolledList*)widget)->setItems(items);
      return true;
    } else if(widget->type()=="Tree") {
      std::string oldXMLTree;
     {HTREEITEM item = ((WinTk::Tree*)widget)->firstItem();
      if(item) {
        HWND hwnd = ((WinTk::Tree*)widget)->nativeWindow();
        oldXMLTree = WinTk::TreeGetItemXML(hwnd,item);
      }}
      // retrieve the old tree selection in a string :
      std::string selection;
      ((WinTk::Tree*)widget)->selection(selection);
      // clear old tree
      ((WinTk::Tree*)widget)->clear();
      // load new tree
      inlib::xml::default_factory factory;
      exlib::xml::parser treeML
        (factory,out(),fSession.verboseLevel()?true:false);
      std::vector<std::string> tags;
      tags.push_back("tree");
      tags.push_back("treeItem");
      treeML.set_tags(tags);
      if(treeML.load_string(aValue)) {
        inlib::xml::tree* top = treeML.top_item();
        //top->dump(;)
        if(top) {
          if (oldXMLTree.size()) {
            oldXMLTree = "<tree>"+oldXMLTree+"</tree>";
            exlib::xml::parser 
              oldTreeML(factory,out(),fSession.verboseLevel()?true:false);
            std::vector<std::string> tags2;
            tags2.push_back("tree");
            tags2.push_back("treeItem");
            oldTreeML.set_tags(tags2);
            if(oldTreeML.load_string(oldXMLTree)) {
              inlib::xml::tree* old = oldTreeML.top_item();
              if (old) {
                top->update_tree(*old);
              }
            }
          }
          createTree((WinTk::Tree*)widget,top);
          //((WinTk::Tree*)widget)->selectFirst();
          // Restore selection :
          std::vector<std::string> items;
          smanip_lines(selection,items);
          ((WinTk::Tree*)widget)->setSelection(items);
          return true; 
        }
      }
    }
  } else if(aResource=="popupItems") {
#ifdef ONX_HAS_INVENTOR
    if(widget->type()=="PageViewer") {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      for(unsigned int index=0;index<items.size();index++)
        ((WinTk::PageViewer*)widget)->addPopupEntry(items[index]);
      return true;
    }
#endif
  }

  std::ostream& out = fSession.out();
  out << "OnX::WinUI::setParameter :"
      << " unable to set parameter for :"
      << " widget " << inlib::sout(aWidget)
      << " resource " << inlib::sout(aResource)
      << " value " << inlib::sout(aValue) << "."
      << std::endl;

  return false;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool OnX::WinUI::createTree(WinTk::Tree* aTree,inlib::xml::tree* aItemML){
  bool old = aTree->enableCallbacks(false);
  const std::list<inlib::xml::tree*>& list = aItemML->children();
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    std::string slabel;
    (*it)->element_value("label",slabel);

    std::string sopened;
    bool opened = false;
    if((*it)->element_value("opened",sopened)) {
      inlib::to(sopened,opened);
    }

    void* treeItem;
    void* data;
    int dataInt;
    (*it)->parent()->get_data(treeItem,data,dataInt);
    HTREEITEM item = aTree->insertItem((HTREEITEM)treeItem,slabel);
    (*it)->set_data(item,0,0);
    if(!createTree(aTree,*it)) return false;
    if(opened) 
      TreeView_Expand(aTree->nativeWindow(),item,TVE_EXPAND);
  }
  aTree->enableCallbacks(old);
  return true;
}

bool OnX::WinUI::createNativeWidget(inlib::xml::tree& aItemML,const std::string& aClass,const std::string& aName,inlib::xml::tree* aParent){
    OnX::Widget* owidget = INLIB_CAST(aItemML,OnX::Widget);
    if(!owidget) {
      std::ostream& out = fSession.out();
      out << "WinUI::createNativeWidget :"
          << " ItemML not a OnX::Widget." 
          << aClass << std::endl;
      return false;
    }

    int winClass = getType(aClass);
    if(winClass==0) {
      std::ostream& out = fSession.out();
      out << "WinUI::createNativeWidget : unknown widget type : " 
          << aClass << std::endl;
      return false;
    }

    // Get parent :
    WinTk::Component* winParent = 0;
    int winParentClass = 0;
    if(aParent) {
      //printf("debug : parent :%s|\n",aParent->getValue("class"));
      winParent = (WinTk::Component*)aParent->get_data1();
      winParentClass = getType(winParent->type());
    }

    /*
    if(fVerboseLevel) {
      std::ostream& out = fSession.out();
      out << "WinUI::createNativeWidget :" 
          << " parent class " << inlib::sout(winParent?winParent->type():"nil")
          << "." << std::endl;
    }
    */

    std::string svalue;

#define CHECK_PARENT \
      if(!winParent) {\
        std::ostream& out = fSession.out();\
        out << "WinUI::createNativeWidget : " << aClass \
            << " needs a parent." << std::endl;\
        return false;\
      }  

    // Create the WinTk widget :
    WinTk::Component* winWidget = 0;
    if(winClass==win_Window) {

      if(aItemML.element_value("decoration",svalue)) {
        bool close_button = true;
        bool iconify_button = true;
        bool min_max_button = true;
        bool resize_handle = true;
        bool border = true;
        bool caption = true;
        bool system_menu = true;

        std::vector< std::pair<bool,std::string> > decoItems = 
          parseDecorations(svalue);      
        std::vector< std::pair<bool,std::string> >::iterator it;
        for(it=decoItems.begin();it!=decoItems.end();++it) {
          if ((*it).second == "close_button") close_button = (*it).first; 
          if ((*it).second == "iconify_button") iconify_button = (*it).first; 
          if ((*it).second == "min_max_button") min_max_button = (*it).first; 
          if ((*it).second == "resize_handle") resize_handle = (*it).first; 
          if ((*it).second == "border") border = (*it).first;
          if ((*it).second == "caption") caption = (*it).first; 
          if ((*it).second == "system_menu") system_menu = (*it).first;
        }

        unsigned int decorations = 0;
        if (system_menu)    decorations |= WS_SYSMENU;
        if (iconify_button) decorations |= WS_MINIMIZEBOX;
        if (min_max_button) decorations |= WS_MAXIMIZEBOX;
        if (resize_handle)  decorations |= WS_SIZEBOX;
        if (border)         decorations |= WS_BORDER;
        if (caption)        decorations |= WS_CAPTION;

        winWidget = new WinTk::Shell(decorations);
        if ((!close_button) ||(!min_max_button) || (!iconify_button)) {
          HMENU hMenu = GetSystemMenu(winWidget->nativeWindow(), FALSE);
          int nCount = GetMenuItemCount(hMenu);
          if (!close_button) RemoveMenu(hMenu, nCount - 1, MF_BYPOSITION);
          if (!min_max_button) RemoveMenu(hMenu, nCount - 2, MF_BYPOSITION);
          if (!iconify_button) RemoveMenu(hMenu, nCount - 3, MF_BYPOSITION);
        }
      } else {
        winWidget = new WinTk::Shell(WS_OVERLAPPEDWINDOW);
      }

      ((WinTk::Shell*)winWidget)->setSetFocusCallback(setFocusCallback,this);
      fWidgets.push_back(winWidget);
     {OnX::Win::Destroy* dst = new OnX::Win::Destroy(*this,aItemML);
      fDestroys.push_back(dst); //FIXME.
      //FIXME : give the dst to a "delete" callback that will delete it.
      //        See XtUI.
      winWidget->addCallback("close",WinUI::closeCallback,dst);}
    } else if(winClass==win_Dialog) {

      unsigned int decorations = WS_POPUP | WS_CAPTION | WS_THICKFRAME;
      winWidget = new WinTk::Dialog(*winParent,decorations);
      // We enforce to have no close button on dialogs.
     {HMENU hMenu = GetSystemMenu(winWidget->nativeWindow(), FALSE);
      int nCount = GetMenuItemCount(hMenu);
      RemoveMenu(hMenu, nCount - 1, MF_BYPOSITION);
      //if (!min_max_button) RemoveMenu(hMenu, nCount - 2, MF_BYPOSITION);
      //if (!iconify_button) RemoveMenu(hMenu, nCount - 3, MF_BYPOSITION);
      }

      fWidgets.push_back(winWidget);
      // If no close button, then no need to have a closeCallback.
    } else if(winClass==win_WarningDialog) {
      winWidget = new WinTk::WarningDialog(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_Label) {
      CHECK_PARENT
      winWidget = new WinTk::Label(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_PushButton) {
      CHECK_PARENT
      bool bitmap = aItemML.element_value("pixmap",svalue);
      winWidget = new WinTk::Button(*winParent,bitmap);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ToggleButton) {
      CHECK_PARENT
      winWidget = new WinTk::ToggleButton(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_VScrollBar) {
      CHECK_PARENT
      winWidget = new WinTk::VScrollBar(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_HScrollBar) {
      CHECK_PARENT
      winWidget = new WinTk::HScrollBar(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_VScale) {
      CHECK_PARENT
      winWidget = new WinTk::VScale(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_HScale) {
      CHECK_PARENT
      winWidget = new WinTk::HScale(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_CommandLine) {
      CHECK_PARENT
      winWidget = new WinTk::CommandLine(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_Password) {
      CHECK_PARENT
      winWidget = new WinTk::Entry(*winParent,true);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_Entry) {
      CHECK_PARENT
      winWidget = new WinTk::Entry(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_VBox) {
      CHECK_PARENT
      winWidget = new WinTk::VBox(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_VPaned) {
      CHECK_PARENT
      winWidget = new WinTk::VPaned(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_HBox) {
      CHECK_PARENT
      winWidget = new WinTk::HBox(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_HPaned) {
      CHECK_PARENT
      winWidget = new WinTk::HPaned(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_VContainer) {
      CHECK_PARENT
      winWidget = new WinTk::VContainer(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_HContainer) {
      CHECK_PARENT
      winWidget = new WinTk::HContainer(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_MenuBar) {
      CHECK_PARENT
      if(winParent && (winParentClass!=win_Window)) {
        std::ostream& out = fSession.out();
        out << "WinUI::createNativeWidget :"
            << " a MenuBar needs a Window parent (" 
            << winParent->type() << " given)." 
            << std::endl;
        return false;\
      }
      winWidget = new WinTk::MenuBar(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_CascadeButton) {
      CHECK_PARENT
      std::string slabel;
      aItemML.element_value("label",slabel);
      WinTk::CascadeButton* casc = new WinTk::CascadeButton(*winParent,slabel);
      winWidget = casc;
      fWidgets.push_back(winWidget);
      //FIXME : who rm ?
      fCascadeButtons.push_back
        (std::pair<HMENU,WinTk::CascadeButton*>(casc->hmenu(),casc));
    } else if(winClass==win_Menu) {
      CHECK_PARENT
      winWidget = new WinTk::Menu(*winParent);
      fWidgets.push_back(winWidget);
    } else if( (winClass==win_MenuItem) ||
               (winClass==win_ToggleMenuItem) ) {
      CHECK_PARENT
      std::string slabel;
      aItemML.element_value("label",slabel);
      fActionID++;
      WinTk::MenuItem* menuItem = new WinTk::MenuItem(*winParent,
        slabel,fActionID,winClass==win_ToggleMenuItem?true:false);
      winWidget = menuItem;
      fWidgets.push_back(winWidget);
      //FIXME : who rm ?
      fMenuItems.push_back
        (std::pair<int,WinTk::MenuItem*>(fActionID,menuItem));
    } else if(winClass==win_MenuSeparator) {
      CHECK_PARENT
      winWidget = new WinTk::MenuSeparator(*winParent);
      fWidgets.push_back(winWidget);

#ifdef ONX_HAS_INVENTOR
    } else if(winClass==win_SoExaminerViewer) {
      CHECK_PARENT
      WinTk::SoExaminerViewer* viewer = 
        new WinTk::SoExaminerViewer(*winParent,aName);
      winWidget = viewer;
      fWidgets.push_back(winWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      SoWinExaminerViewer* soWinViewer = viewer->soViewer();
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy
        (soWinViewer,viewer_proxy<SoWinFullViewer,SoWinExaminerViewer>);
    } else if(winClass==win_SoPlaneViewer) {
      CHECK_PARENT
      WinTk::SoPlaneViewer* viewer = 
        new WinTk::SoPlaneViewer(*winParent,aName);
      winWidget = viewer;
      fWidgets.push_back(winWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      SoWinPlaneViewer* soWinViewer = viewer->soViewer();
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(soWinViewer,viewer_proxy<SoWinFullViewer>);
    } else if(winClass==win_SoFlyViewer) {
      CHECK_PARENT
      WinTk::SoFlyViewer* viewer = 
        new WinTk::SoFlyViewer(*winParent,aName);
      winWidget = viewer;
      fWidgets.push_back(winWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      SoWinFlyViewer* soWinViewer = viewer->soViewer();
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(soWinViewer,viewer_proxy<SoWinFullViewer>);
    } else if(winClass==win_SoWalkViewer) {
      CHECK_PARENT
      WinTk::SoWalkViewer* viewer = 
        new WinTk::SoWalkViewer(*winParent,aName);
      winWidget = viewer;
      fWidgets.push_back(winWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      SoWinWalkViewer* soWinViewer = viewer->soViewer();
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(soWinViewer,viewer_proxy<SoWinFullViewer>);

    } else if( (winClass==win_PageViewer) ||
               (winClass==win_PlanePageViewer) ){
      CHECK_PARENT
      std::string vclass = "Examiner";
      if(winClass==win_PlanePageViewer) vclass = "Plane";
      winWidget = new WinTk::PageViewer(*winParent,aName,vclass);
      SoNode* soNode = ((WinTk::PageViewer*)winWidget)->sceneGraph();
      SoWinFullViewer* viewer = ((WinTk::PageViewer*)winWidget)->soViewer();
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) {
        std::ostream& out = fSession.out();
        out << "WinUI::createNativeWidget :"
            << " ItemML not a OnX::SoViewer." 
            << std::endl;
        return false;
      }
      soViewer->setSceneGraph(soNode);
      if(viewer->isOfType(SoWinExaminerViewer::getClassTypeId())) {
        soViewer->setViewerProxy
          (viewer,viewer_proxy<SoWinFullViewer,SoWinExaminerViewer>);
      } else {
        soViewer->setViewerProxy(viewer,viewer_proxy<SoWinFullViewer>);
      }

      fWidgets.push_back(winWidget);
#endif

    } else if(winClass==win_OpenGLArea) {
      CHECK_PARENT
      winWidget = new WinTk::OpenGLArea(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_FileSelectionDialog) {
      CHECK_PARENT
      winWidget = new WinTk::FileSelectionDialog(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ColorSelectionDialog) {
      CHECK_PARENT
	//winWidget = new WinTk::ColorSelectionDialog(*winParent);
      winWidget = new WinTk::ColorChooser(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ScrolledList) {
      CHECK_PARENT
      winWidget = new WinTk::ScrolledList(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ScrolledText) {
      CHECK_PARENT
      winWidget = new WinTk::ScrolledText(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_Tree) {
      CHECK_PARENT
      winWidget = new WinTk::Tree(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ToolBar) {
      CHECK_PARENT
      if(winParent && (winParentClass!=win_Window)) {
        std::ostream& out = fSession.out();
        out << "WinUI::createNativeWidget :"
            << " a ToolBar needs a Window parent (" 
            << winParent->type() << " given)." 
            << std::endl;
        return false;\
      }
      winWidget = new WinTk::ToolBar(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ToolBarItem) {
      CHECK_PARENT
      aItemML.element_value("label",svalue);
      std::string spixmap;
      aItemML.element_value("pixmap",spixmap);
      std::string shelp;
      aItemML.element_value("tooltip",shelp);
      winWidget = new WinTk::ToolBarItem(*winParent,svalue,spixmap,shelp);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_OptionMenu) {
      CHECK_PARENT
      winWidget = new WinTk::OptionMenu(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ComboBox) {
      CHECK_PARENT
      winWidget = new WinTk::ComboBox(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_ProgressBar) {
      CHECK_PARENT
      winWidget = new WinTk::ProgressBar(*winParent);
      fWidgets.push_back(winWidget);
    } else if(winClass==win_TabStack) {
      CHECK_PARENT
      winWidget = new WinTk::TabStack(*winParent);
      fWidgets.push_back(winWidget);
    } else {
      std::ostream& out = fSession.out();
      out << "WinUI::createNativeWidget :" << aClass 
          << " unknown case." << std::endl;
      return false;
    }
    
    winWidget->setName(aName);

    // The below is not needed since all widgets are in fWidgets.
    //winWidget->addCallback
    //  ("delete",WinUI::destroyCallback,new OnX::Win::Destroy(*this,aItemML));

    aItemML.set_data(winWidget,0,0);
    owidget->setProxy(winWidget,win_proxy);

    // Put in parent :
    if(aParent && winParentClass && winParent) {
      if(winParentClass==win_TabStack) {
        if(!aItemML.element_value("tabLabel",svalue)) svalue = aName;
        ((WinTk::TabStack*)winParent)->addItem(*winWidget,svalue);
      }
    }

    // Set resources :
    if(winClass==win_Window) {
      winWidget->addCallback("activate",WinUI::windowCallback,this);
      winWidget->addCallback("cascading",WinUI::cascadingCallback,this);
      if(aItemML.element_value("geometry",svalue)) {
        int x,y,w,h;
        if(!to_geometry(svalue,x,y,w,h)) {
          std::ostream& out = fSession.out();
          out << svalue << " : bad value for geometry." << std::endl;
        } else {
          ((WinTk::Shell*)winWidget)->setGeometry(x,y,w,h);
        }
      }
      if(aItemML.element_value("title",svalue)) {
        ((WinTk::Shell*)winWidget)->setTitle(svalue);
      }
    } else if(winClass==win_Dialog)  {
      if(aItemML.element_value("geometry",svalue)) {
        int w,h;
        if(sscanf(svalue.c_str(),"%dx%d",&w,&h)!=2) {
          std::ostream& out = fSession.out();
          out << svalue << " : bad value for geometry." << std::endl;
        } else {
          ((WinTk::Dialog*)winWidget)->setSize(w,h);
        }
      }
      if(aItemML.element_value("title",svalue)) {
        ((WinTk::Dialog*)winWidget)->setTitle(svalue);
      }
    } else if(winClass==win_HBox) {
      if(aItemML.element_value("sizeOfFixed",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((WinTk::HBox*)winWidget)->setSizeOfFixed(value);
        }      
      } else if(aItemML.element_value("splitPosition",svalue)) { 
        //deprecated
        std::ostream& out = fSession.out();
        out << "WinUI::createNativeWidget : "
            << " splitPosition resource deprecated for HBox." 
            << " Use sizeOfFixed." << std::endl;
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((WinTk::HBox*)winWidget)->setSizeOfFixed(value);
        }      
      }
      if(aItemML.element_value("expand",svalue)) {
        if(svalue=="first") {
          ((WinTk::HBox*)winWidget)->setExpand(WinTk::HBox::FIRST);
        } else if(svalue=="both") {
          ((WinTk::HBox*)winWidget)->setExpand(WinTk::HBox::BOTH);
        } else {
          ((WinTk::HBox*)winWidget)->setExpand(WinTk::HBox::SECOND);
        }
      }
    } else if(winClass==win_VBox) {
      if(aItemML.element_value("sizeOfFixed",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((WinTk::VBox*)winWidget)->setSizeOfFixed(value);
        }      
      } else if(aItemML.element_value("splitPosition",svalue)) { 
        //deprecated
        std::ostream& out = fSession.out();
        out << "WinUI::createNativeWidget : "
            << " splitPosition resource deprecated for HBox." 
            << " Use sizeOfFixed." << std::endl;
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((WinTk::VBox*)winWidget)->setSizeOfFixed(value);
        }      
      }
      if(aItemML.element_value("expand",svalue)) {
        if(svalue=="first") {
          ((WinTk::VBox*)winWidget)->setExpand(WinTk::VBox::FIRST);
        } else if(svalue=="both") {
          ((WinTk::VBox*)winWidget)->setExpand(WinTk::VBox::BOTH);
        } else {
          ((WinTk::VBox*)winWidget)->setExpand(WinTk::VBox::SECOND);
        }
      }
    } else if(winClass==win_HPaned) {
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((WinTk::HPaned*)winWidget)->setSplitPositionPixel(value);
        }      
      }
    } else if(winClass==win_VPaned) {
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((WinTk::VPaned*)winWidget)->setSplitPositionPixel(value);
        }      
      }
    } else if(winClass==win_PushButton) {
      if(aItemML.element_value("label",svalue) ||
         aItemML.element_value("labelString",svalue) ) {
        ((WinTk::Button*)winWidget)->setLabel(svalue);
      }
      if(aItemML.element_value("pixmap",svalue)) {
        ((WinTk::Button*)winWidget)->setPixmap(svalue);
      }
      addCallbacks(aItemML,winWidget,"activate");
    } else if(winClass==win_ToggleButton) {
      if(aItemML.element_value("set",svalue)) {
        bool value;
        if(inlib::to(svalue,value)) {
          ((WinTk::ToggleButton*)winWidget)->setChecked(value);
        }
      }
      if(aItemML.element_value("label",svalue) ||
         aItemML.element_value("labelString",svalue) ) {
        ((WinTk::Button*)winWidget)->setLabel(svalue);
      }
      addCallbacks(aItemML,winWidget,"activate");
    } else if(winClass==win_Label) {
      if(aItemML.element_value("label",svalue) ||
         aItemML.element_value("labelString",svalue) ) {
        ((WinTk::Label*)winWidget)->setLabel(svalue);
      }
    } else if((winClass==win_HScale)||(winClass==win_VScale)||
              (winClass==win_HScrollBar)||(winClass==win_VScrollBar) ) {
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
      if(winClass==win_VScale) {
        ((WinTk::VScale*)winWidget)->setMinimum(mn);
        ((WinTk::VScale*)winWidget)->setMaximum(mx);
        ((WinTk::VScale*)winWidget)->setValue(val);
      } else if(winClass==win_HScale) {
        ((WinTk::HScale*)winWidget)->setMinimum(mn);
        ((WinTk::HScale*)winWidget)->setMaximum(mx);
        ((WinTk::HScale*)winWidget)->setValue(val);
      } else if(winClass==win_VScrollBar) {
        ((WinTk::VScrollBar*)winWidget)->setMinimum(mn);
        ((WinTk::VScrollBar*)winWidget)->setMaximum(mx);
        ((WinTk::VScrollBar*)winWidget)->setValue(val);
      } else if(winClass==win_HScrollBar) {
        ((WinTk::HScrollBar*)winWidget)->setMinimum(mn);
        ((WinTk::HScrollBar*)winWidget)->setMaximum(mx);
        ((WinTk::HScrollBar*)winWidget)->setValue(val);
      }
      addCallbacks(aItemML,winWidget,"drag");
    } else if(winClass==win_OptionMenu) {
      //if(aItemML.element_value("label",svalue)) {
        //FIXME
      //}
      std::string def;
      aItemML.element_value("default",def);
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        ((WinTk::OptionMenu*)winWidget)->initialize(items,def);
      }
      addCallbacks(aItemML,winWidget,"valueChanged");
    } else if(winClass==win_ComboBox) {
      //if(aItemML.element_value("label",svalue)) {
        //FIXME
      //}
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        ((WinTk::ComboBox*)winWidget)->setItems(items);
      }
      if(aItemML.element_value("default",svalue)) {
        ((WinTk::ComboBox*)winWidget)->setValue(svalue);
      }
      addCallbacks(aItemML,winWidget,"valueChanged");
    } else if(winClass==win_ScrolledList) {
      std::string def;
      aItemML.element_value("selection",def);
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        ((WinTk::ScrolledList*)winWidget)->setItems(items,def);
      }
      addCallbacks(aItemML,winWidget,"select");
    } else if(winClass==win_ScrolledText) {
      if(aItemML.element_value("value",svalue)) {
        bool as_it = false;
        std::string s;
        if(aItemML.attribute_value("value","as_it",s)) {
          if(!inlib::to(s,as_it)) as_it = false;
        }
        if(!as_it) inlib::replace(svalue,"\\n","\n");
        ((WinTk::ScrolledText*)winWidget)->setText(svalue);
      }
    } else if(winClass==win_Tree) {
      addCallbacks(aItemML,winWidget,"select");
      addCallbacks(aItemML,winWidget,"selectBranch");
      addCallbacks(aItemML,winWidget,"open");
      addCallbacks(aItemML,winWidget,"close");
    } else if(winClass==win_Entry) {
      if(aItemML.element_value("value",svalue)) {
        ((WinTk::Entry*)winWidget)->setValue(svalue);
      }
      addCallbacks(aItemML,winWidget,"activate");
    } else if(winClass==win_ToolBarItem) {
      addCallbacks(aItemML,winWidget,"activate");
    } else if(winClass==win_CommandLine) {
      addCallbacks(aItemML,winWidget,"activate");
      addCallbacks(aItemML,winWidget,"complete");
    } else if(winClass==win_Password) {
      addCallbacks(aItemML,winWidget,"activate");
    } else if(winClass==win_TabStack) {
      addCallbacks(aItemML,winWidget,"select");
    } else if(winClass==win_CascadeButton) {
      addCallbacks(aItemML,winWidget,"cascading");
    } else if( (winClass==win_MenuItem) ||
               (winClass==win_ToggleMenuItem) ) {
      if(aItemML.element_value("accelerator",svalue)) {
        WinTk::MenuItem* menuItem = ((WinTk::MenuItem*)winWidget);
	std::string s;
        menuItem->label(s);
        menuItem->setLabel(s+"\t"+svalue);
        if(!menuItem->addAccelerator(svalue)) {
          std::ostream& out = fSession.out();
          out << "Can't set accelerator " << inlib::sout(svalue) << std::endl;
        }
      }
      addCallbacks(aItemML,winWidget,"activate");
    } else if(winClass==win_OpenGLArea) {
      addCallbacks(aItemML,winWidget,"paint");
      addCallbacks(aItemML,winWidget,"event");
#ifdef ONX_HAS_INVENTOR
    } else if( (winClass==win_PageViewer) ||
               (winClass==win_PlanePageViewer) ){

      if(aItemML.element_value("popupItems",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++)
          ((WinTk::PageViewer*)winWidget)->addPopupEntry(items[index]);
      }

      ((WinTk::PageViewer*)winWidget)->
        setVerboseLevel(fVerboseLevel);

      addCallbacks(aItemML,winWidget,"rectangularCollect");
      addCallbacks(aItemML,winWidget,"collect");
      addCallbacks(aItemML,winWidget,"popup");
      addCallbacks(aItemML,winWidget,"update");
#endif
    } else if(winClass==win_FileSelectionDialog) {
      addCallbacks(aItemML,winWidget,"ok");
      addCallbacks(aItemML,winWidget,"cancel");

      if(aItemML.element_value("dirMask",svalue)) {
        inlib::expand_vars(svalue);
        ((WinTk::FileSelectionDialog*)winWidget)->setDirMask(svalue);
      }

      if(aItemML.element_value("directory",svalue)) {
        inlib::expand_vars(svalue);
        ((WinTk::FileSelectionDialog*)winWidget)->setDirectory(svalue);
      }

    } else if(winClass==win_ColorSelectionDialog) {
      addCallbacks(aItemML,winWidget,"ok");
      addCallbacks(aItemML,winWidget,"cancel");
    }

    if(fVerboseLevel) {
      std::ostream& out = fSession.out();
      out << "WinUI::createNativeWidget :" 
          << " class " << inlib::sout(aClass)
          << " name " << inlib::sout(aName)
          << " done." << std::endl;
    }

    bool status = createChildren(aItemML);

    if(winClass==win_ToolBar) {
      ((WinTk::ToolBar*)winWidget)->createItems();
    }

    return status;
}

bool OnX::WinUI::destroyNativeWidget(inlib::xml::tree& aItemML){
  // Destroy sub WinTk::Component :
  const std::list<inlib::xml::tree*>& list = aItemML.children();
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!destroyNativeWidget(*(*it))) return false;
  }
  // Destroy this component :
  return destroyOneWidget(aItemML);
}

bool OnX::WinUI::destroyOneWidget(inlib::xml::tree& aItemML){
  //WARNING : can't be called from a aItemML callback !
  WinTk::Component* widget = (WinTk::Component*)aItemML.get_data1();
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::WinUI::destryOneWidget : %lu : begin\n",widget);
#endif
  if(widget) {
    if(fVerboseLevel) {
      std::ostream& out = fSession.out();
      out << "WinUI::destroyWidget :"
          << " destroy WinTk widget of type " 
          << inlib::sout(widget->type()) << "..." << std::endl;
    }
#ifdef DEBUG_DESTROY
    ::printf("debug : OnX::WinUI::destryOneWidget : \"%s\" \"%s\"\n",widget->name().c_str(),widget->type().c_str());
#endif

    // remove in native parent if needed :
   {WinTk::Component* parent = (WinTk::Component*)widget->parent();
    if(parent&&(parent->type()=="TabStack")) {
      ((WinTk::TabStack*)parent)->removeItem(*widget);
    }}

    removeActions(widget);
    fWidgets.remove(widget);
    delete widget;
  }
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::WinUI::destryOneWidget : %lu : end\n",widget);
#endif
  removeInRelations(aItemML);
  aItemML.invalidate();
  return true;
}

void OnX::WinUI::deleteCallback(inlib::xml::tree& aItemML,const std::string& aNameXML,OnX::Action* aAction){
  WinTk::Component* widget = (WinTk::Component*)aItemML.get_data1(); 
  if(!widget) return;

  //std::string name;
  //if (!aItemML.attribute_value("","name",name)) return;
  //FIXME :   std::string xtCbkName = getCallbackName(name,aNameXML);
  //   if(xtCbkName.empty()) return;
}

OnX::Action* OnX::WinUI::addCallback(inlib::xml::tree& aItemML,const std::string& aName,const std::string& aInterp,const std::string& aScript){
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  WinTk::Component* nativeWidget = (WinTk::Component*)aItemML.get_data1(); 
  if(!nativeWidget) return 0;
  Action::Parameters params;
  Action* action = 
    new Action(*this,widget,nativeWidget,aInterp,aScript,params,aName,0);
  addAction(action);
  nativeWidget->addCallback(aName,callback,action);
  return action;
}

void OnX::WinUI::addCallbacks(inlib::xml::tree& aItemML,WinTk::Component* aWidget,const std::string& aName){
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  const std::list<inlib::xml::element*> props = aItemML.elements();
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    if(aName==(*it)->name()) {
      std::string sexec;
      (*it)->attribute_value("exec",sexec);
      Action* action = new Action(*this,widget,aWidget,
                                  sexec,(*it)->value(),(*it)->attributes(),
                                  aName,0);
      addAction(action);
      aWidget->addCallback(aName,callback,action);
    }
  }
}

int OnX::WinUI::getType(const std::string& aName){
       if(aName=="Window")         return win_Window;
  else if(aName=="VBox")           return win_VBox;
  else if(aName=="HBox")           return win_HBox;
  else if(aName=="VPaned")         return win_VPaned;
  else if(aName=="HPaned")         return win_HPaned;
  else if(aName=="MenuBar")        return win_MenuBar;
  else if(aName=="CascadeButton")  return win_CascadeButton;
  else if(aName=="Menu")           return win_Menu;
  else if(aName=="MenuItem")       return win_MenuItem;
  else if(aName=="ToggleMenuItem") return win_ToggleMenuItem;
  else if(aName=="Label")          return win_Label;
  else if(aName=="PushButton")     return win_PushButton;
  else if(aName=="ToggleButton")   return win_ToggleButton;
  else if(aName=="CommandLine")    return win_CommandLine;
  else if(aName=="OpenGLArea")     return win_OpenGLArea;
  else if(aName=="ScrolledList")   return win_ScrolledList;
  else if(aName=="ScrolledText")   return win_ScrolledText;
  else if(aName=="Dialog")         return win_Dialog;
  else if(aName=="VContainer")     return win_VContainer;
  else if(aName=="HContainer")     return win_HContainer;
  else if(aName=="Entry")          return win_Entry;
  else if(aName=="ComboBox")       return win_ComboBox;
  else if(aName=="VScrollBar")     return win_VScrollBar;
  else if(aName=="HScrollBar")     return win_HScrollBar;
  else if(aName=="VScale")         return win_VScale;
  else if(aName=="HScale")         return win_HScale;
  else if(aName=="ToolBar")        return win_ToolBar;
  else if(aName=="ToolBarItem")    return win_ToolBarItem;
  else if(aName=="MenuSeparator")  return win_MenuSeparator;
  else if(aName=="ProgressBar")    return win_ProgressBar;
  else if(aName=="TabStack")       return win_TabStack;
  else if(aName=="OptionMenu")     return win_OptionMenu;

  else if(aName=="Password")       return win_Password;
  else if(aName=="ScrolledTree")   return win_Tree;
  else if(aName=="FileSelection")  return win_FileSelectionDialog;
  else if(aName=="ColorSelection") return win_ColorSelectionDialog;
  else if(aName=="WarningDialog")  return win_WarningDialog;

#ifdef ONX_HAS_INVENTOR
  else if(aName=="PageViewer")     return win_PageViewer;
  else if(aName=="PlanePageViewer")return win_PlanePageViewer;
  else if(aName=="SoExaminerViewer") return win_SoExaminerViewer;
  else if(aName=="SoPlaneViewer")    return win_SoPlaneViewer;
  //FIXME : SoType problem at creation with the two belows.
  //else if(aName=="SoFlyViewer")      return win_SoFlyViewer;
  //else if(aName=="SoWalkViewer")     return win_SoWalkViewer;
#endif

  // So that in createWidget, getType(winParent->type()) works :
  else if(aName=="Shell")          return win_Window;
  return 0;
}

void OnX::WinUI::callback(WinTk::Component& aComponent,WinTk::CallbackData& aData,void* aTag){
  Action* action = (Action*)aTag;
  if(!action) return;
  WinUI* This = INLIB_CAST(action->ui(),OnX::WinUI);
  if(!This) return;

  std::string svalue;
  std::string sevent;
  int cbkx = 0;
  int cbky = 0;


  // Retreive "value" :
  const std::string& type = aComponent.type();
  if(type=="CommandLine") {
    svalue = ((WinTk::CommandLine*)&aComponent)->value();
  } else if(type=="Entry") {
    svalue = ((WinTk::Entry*)&aComponent)->value();
  } else if(type=="ToggleButton") {
    svalue = 
      (((WinTk::ToggleButton*)&aComponent)->isChecked() ? "true" : "false");
  } else if(type=="FileSelectionDialog") {
    svalue = aData.value;
  } else if(type=="ColorSelectionDialog") {
    svalue = aData.value;
  } else if(type=="ColorChooser") {
    svalue = aData.value;
  } else if(type=="MenuItem") {
    svalue = aData.value;
  } else if(type=="CascadeButton") {
    svalue = aData.value;
  } else if(type=="HScrollBar") {
    int value  = ((WinTk::HScrollBar*)&aComponent)->value();
    inlib::sprintf(svalue,64,"%d",value);
  } else if(type=="VScrollBar") {
    int value  = ((WinTk::VScrollBar*)&aComponent)->value();
    inlib::sprintf(svalue,64,"%d",value);
  } else if(type=="HScale") {
    int value  = ((WinTk::HScale*)&aComponent)->value();
    inlib::sprintf(svalue,64,"%d",value);
  } else if(type=="VScale") {
    int value  = ((WinTk::VScale*)&aComponent)->value();
    inlib::sprintf(svalue,64,"%d",value);
  } else if(type=="ComboBox") {
    svalue = ((WinTk::ComboBox*)&aComponent)->value();
  } else if(type=="OptionMenu") {
    svalue = ((WinTk::OptionMenu*)&aComponent)->value();
  } else if(type=="ScrolledList") {
    ((WinTk::ScrolledList*)&aComponent)->selection(svalue);
  } else if(type=="TabStack") {
    ((WinTk::TabStack*)&aComponent)->selection(svalue);
  } else if(type=="Tree") {
    svalue = aData.value;
  } else if(type=="OpenGLArea") {
    sevent = aData.value;
    cbkx = aData.x;
    cbky = aData.y;
#ifdef ONX_HAS_INVENTOR
  } else if(type=="PageViewer") {
    svalue = aData.value;
    cbkx = aData.x;
    cbky = aData.y;
#endif
  }

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::WinUI::callback :"
        << " component " << inlib::sout(aComponent.name())
        << " type " << inlib::sout(aComponent.type())
        << " value " << inlib::sout(svalue)
        << " event " << inlib::sout(sevent)
        << " x " << cbkx
        << " y " << cbky
        << "."
        << std::endl;
  }

  This->fCallbackData =  
    CallbackData(action->widget(),aComponent.name(),svalue,sevent,cbkx,cbky);

  // Execute script :
  OnX::Action tmp(*action);
  // the action may delete widget that may do a ui.removeActions
  // that may then deleta action ! We then execute the script
  // with a local copy of the action.
  tmp.execute();

  This->fCallbackData = CallbackData();
}

void OnX::WinUI::closeCallback(WinTk::Component& aComponent,WinTk::CallbackData&,void* aTag){
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::WinUI::closeCallback : %lu : begin\n",&aComponent);
#endif
  OnX::Win::Destroy* d = (OnX::Win::Destroy*)aTag; 
  if(d->isValid()) { 
#ifdef DEBUG_DESTROY
    ::printf("debug : OnX::WinUI::closeCallback : exec destroy\n");
#endif
    Execute tag(d->fUI,"destroy");
    d->fItemML.post_execute_backward(executeItemMLCallbacks,&tag);

    d->fUI.removeInRelations(d->fItemML);
    d->fItemML.invalidate();

    inlib::xml::tree* item = &(d->fItemML);
    inlib::xml::tree* parent = item->parent();
    if(parent) parent->remove_child(item,true);
  }
  d->fUI.fWidgets.remove(&aComponent);
  d->fUI.removeActions(&aComponent);
}

void OnX::WinUI::destroyConsoleCallback(WinTk::Component& aComponent,WinTk::CallbackData&,void* aTag){
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::WinUI::destroyConsoleCallback\n");
#endif
  OnX::WinUI* ui = (OnX::WinUI*)aTag; 
  ui->fConsoleText = 0;
}

void OnX::WinUI::deleteWarningDialogCallback(WinTk::Component& aComponent,WinTk::CallbackData&,void* aTag){
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::WinUI::deleteWarningDialogCallback\n");
#endif
  OnX::WinUI* ui = (OnX::WinUI*)aTag; 
  ui->fWarningDialog = 0;
}

/*
void OnX::WinUI::destroyCallback(WinTk::Component& aComponent,WinTk::CallbackData&,void* aTag){
  OnX::Win::Destroy* d = (OnX::Win::Destroy*)aTag; 
  d->fUI.fWidgets.remove(&aComponent);
  d->fItemML.invalidate();
  delete d;
}
*/

void OnX::WinUI::windowCallback(WinTk::Component&,WinTk::CallbackData& aData,void* aTag){
  if(!aTag) return;
  WinUI* This = (WinUI*)aTag;
  if(!This) return;
  int menuID = (int)aData.wparam;
  WinTk::MenuItem* menuItem = This->findMenuItem(menuID);
  if(!menuItem) return;
  std::string svalue;
  if(menuItem->isToggle()) {
    menuItem->toggleChecked();
    svalue = (menuItem->isChecked() ? "true" : "false");
  }
  WinTk::CallbackData data;
  data.value = svalue;
  menuItem->executeCallbacks("activate",data);
}

void OnX::WinUI::cascadingCallback(WinTk::Component&,WinTk::CallbackData& aData,void* aTag){
  if(!aTag) return;
  WinUI* This = (WinUI*)aTag;
  if(!This) return;
  HMENU menuID = (HMENU)aData.wparam;
  WinTk::CascadeButton* casc = This->findCascadeButton(menuID);
  if(!casc) return;
  WinTk::CallbackData data;
  casc->executeCallbacks("cascading",data);
}

Slash::UI::ICyclic* OnX::WinUI::createCyclic(const std::string& aName,int aDelay,const std::string& aInterp,const std::string& aScript){
  Slash::UI::ICyclic* cyclic = 
    new WinCyclic(fSession,aName,aDelay,aInterp,aScript);
  fCyclics.push_back(cyclic);
  return cyclic;
}

WinTk::MenuItem* OnX::WinUI::findMenuItem(int aID){
  std::list< std::pair<int,WinTk::MenuItem*> >::iterator it;
  for(it=fMenuItems.begin();it!=fMenuItems.end();++it) {
    if((*it).first==aID) return (*it).second;
  }
  return 0;
}

WinTk::CascadeButton* OnX::WinUI::findCascadeButton(HMENU aMenu){
  std::list< std::pair<HMENU,WinTk::CascadeButton*> >::iterator it;
  for(it=fCascadeButtons.begin();it!=fCascadeButtons.end();++it) {
    if((*it).first==aMenu) return (*it).second;
  }
  return 0;
}

std::string OnX::WinUI::widgetName(inlib::xml::tree& aItemML){
  WinTk::Component* widget = (WinTk::Component*)aItemML.get_data1();
  if(!widget) return "";
  return widget->name();
}
/*
std::string OnX::WinUI::getXMLTree(HWND aTV,HTREEITEM aItem,int depth){
  //return a XML string representing this tree
  TCHAR text[256];
  std::string line;
  std::string spaceItem = "";
  std::string spaceRoot = "";
  //  spaceItem.insert(0,depth*2,' ');
  //  spaceRoot.insert(0,(depth-1)*2,' ');
  
  do  {
    TV_ITEM item;
    item.hItem = aItem;
    item.mask = TVIF_TEXT;
    item.pszText = text;
    item.cchTextMax = 256;

    if(!TreeView_GetItem(aTV,&item)) return "";
    
    line += spaceRoot + "<treeItem>";
    line += spaceItem + "<label>";
    std::string label = inlib::to_xml(text);
    line.append(label);
    line += "</label>";
    line += spaceItem + "<opened>";
    if ((item.state & TVIS_EXPANDED)== TVIS_EXPANDED) line.append("true");
    else line.append("false");
    line += "</opened>";
    if (TreeView_GetChild(aTV,aItem)) {
      line +=getXMLTree(aTV,TreeView_GetChild(aTV,aItem),depth+1);
    }
    line += spaceRoot + "</treeItem>";
    aItem = TreeView_GetNextSibling(aTV,aItem);
  }
  while (aItem);
  return line;
}
*/
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <OnX/Core/Term.h>

namespace OnX {
class TermCyclic 
:public WinCyclic
,public Term {
public: //Slash::UI::ICyclic
  virtual void execute() { cyclicExecute(fUI,fInterpreter); }
public:
  TermCyclic(Slash::UI::IUI& aUI,
             const std::string& aPrompt,
             const std::string& aInterp)
  :WinCyclic(aUI.session(),"onx_terminal",10,aInterp,"") //10 is millisec.
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

bool OnX::WinUI::enableTerminal(const std::string& aPrompt,const std::string& aInterp){
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

bool OnX::WinUI::disableTerminal(){
  if(!fTermCyclic) return true;
  delete fTermCyclic;
  fTermCyclic = 0;
  return true;
}

std::string OnX::WinUI::ask(const std::string& aPrompt){
  if(fAskToTerminal && fTermCyclic) {
    return fTermCyclic->ask(aPrompt);
  } else {
    return BaseUI::ask(aPrompt);
  }
}

void OnX::WinUI::setFocusedShell(WinTk::Shell* aShell){fFocusedShell = aShell;}
WinTk::Shell* OnX::WinUI::focusedShell() const {return fFocusedShell;}

static void setFocusCallback(WinTk::Shell* aShell,void* aTag) {    
  OnX::WinUI* ui = (OnX::WinUI*)aTag;
  ui->setFocusedShell(aShell);
}
WinTk::Shell* getFocusedShellProc(void* aTag) {
  OnX::WinUI* ui = (OnX::WinUI*)aTag;
  return ui->focusedShell();  
}

#ifdef ONX_HAS_INVENTOR
#include "../Core/receive_scene.cxx"
#endif

bool OnX::WinUI::receiveScene(
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
