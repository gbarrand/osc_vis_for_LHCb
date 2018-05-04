
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ICallbackData.h>

#include <Slash/Tools/Data.h>
#include <Slash/Tools/Inventor.h>
#include <Slash/Tools/Session.h>

#include <inlib/cast>

#include <inlib/args>
#include <inlib/sprintf>

#include "inlib"

static void deprecated0(Slash::UI::IUI& aUI,const std::string& aOld) {
  std::ostream& out = aUI.session().cout();
  out << "DLD callback " << inlib::sout(aOld) << " is deprecated."
      << std::endl;
}

static void deprecated(Slash::UI::IUI& aUI,const std::string& aOld,const std::string& aNew) {
  std::ostream& out = aUI.session().cout();
  out << "DLD callback " << inlib::sout(aOld) << " is deprecated."
      << " Use " << inlib::sout(aNew) << " instead."
      << std::endl;
}

static void deprecated1(Slash::UI::IUI& aUI,const std::string& aOld,const std::string& aString) {
  std::ostream& out = aUI.session().cout();
  out << "DLD callback " << inlib::sout(aOld) << " is deprecated."
      << " " << aString
      << std::endl;
}

extern "C" {

/////////////////////////////////////////////////////////////////////////////
void ui_showFileChooser(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] lib
// args[1] callback
// args[2] dirmask
// args[3] default directory
// args[...] passed to the ok callback.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"ui_showFileChooser","ui_set_fileChooser");

  if(aArgs.size()<4) return;

  std::string sargs;
  for(unsigned int index=4;index<aArgs.size();index++) {
    if(index!=4) sargs += " ";
    sargs += aArgs[index];
  }


  aUI.setCallback("fileChooser","ok","DLD",aArgs[0]+" "+aArgs[1]+" "+sargs);
  aUI.setCallback("fileChooser","cancel",
                  "DLD","OnX ui_hide fileChooser");
  aUI.setParameter("fileChooser.dirMask",aArgs[2]);
  aUI.setParameter("fileChooser.directory",aArgs[3]);
  aUI.setParameter("fileChooser.mode","open");

  Slash::UI::IWidget* w = aUI.findWidget("fileChooser");
  if(w) w->show();
}
//////////////////////////////////////////////////////////////////////////////
void ui_showFileSaver(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// args[0] lib
// args[1] callback
// args[2] dirmask
// args[3] default directory
// args[...] passed to the ok callback.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"ui_showFileSaver","ui_set_fileChooser");

  if(aArgs.size()<4) return;

  std::string sargs;
  for(unsigned int index=4;index<aArgs.size();index++) {
    if(index!=4) sargs += " ";
    sargs += aArgs[index];
  }


  aUI.setCallback("fileChooser","ok","DLD",aArgs[0]+" "+aArgs[1]+" "+sargs);
  aUI.setCallback("fileChooser","cancel",
                  "DLD","OnX ui_hide fileChooser");
  aUI.setParameter("fileChooser.dirMask",aArgs[2]);
  aUI.setParameter("fileChooser.directory",aArgs[3]);
  aUI.setParameter("fileChooser.mode","save");

  Slash::UI::IWidget* w = aUI.findWidget("fileChooser");
  if(w) w->show();
}
//////////////////////////////////////////////////////////////////////////////
void ui_showColorChooser(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
// args[0] lib
// args[1] callback
// args[..] passed to the ok callback.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated0(aUI,"ui_showColorChooser");

  if(aArgs.size()<2) return;

  std::string sargs;
  for(unsigned int index=2;index<aArgs.size();index++) {
    if(index!=2) sargs += " ";
    sargs += aArgs[index];
  }

  aUI.setCallback("colorChooser","ok","DLD",aArgs[0]+" "+aArgs[1]+" "+sargs);

  Slash::UI::IWidget* w = aUI.findWidget("colorChooser");
  if(w) w->show();
}
//////////////////////////////////////////////////////////////////////////////
void ui_executeFile(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] interpreter
// aArgs[1] file
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"ui_executeFile","ui_execute_file");
  if(aArgs.size()!=2) return;
  aUI.executeScript("DLD","OnX ui_execute_file "+aArgs[0]+" "+aArgs[1]);
}
//////////////////////////////////////////////////////////////////////////////
void tree_set_managers(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"tree_set_managers","tree_set");

  tree_set_with_strings(aUI,"mainTree.items",aUI.session().managerNames());
}
//////////////////////////////////////////////////////////////////////////////
void tree_set_styles(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"tree_set_styles",
                 "tree_set with @manager@StyleManager@styles@");

  Slash::UI::IStyleManager* styleManager = Slash::styleManager(aUI.session());
  if(!styleManager) return;
  tree_set_with_strings(aUI,"mainTree.items",styleManager->styles());
}
//////////////////////////////////////////////////////////////////////////////
void console_execute(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //correct Panoramix first.
  //deprecated(aUI,"console_execute","OnX ui_executeScript @consolePrompt.value@ @this.value@");

  std::string sexec;
  if(!aUI.parameterValue("consolePrompt.value",sexec)) return;
  aUI.executeScript(sexec,aUI.callbackValue());
}
//////////////////////////////////////////////////////////////////////////////
void colorChooser_ok(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated0(aUI,"colorChooser_ok");
  Slash::set_color(aUI.session(),aUI.callbackValue());
}
//////////////////////////////////////////////////////////////////////////////
void colorChooser_show(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated0(aUI,"colorChooser_show");
  aUI.setCallback("colorChooser","ok","DLD","OnX colorChooser_ok");
  Slash::UI::IWidget* w = aUI.findWidget("colorChooser");
  if(w) w->show();
}

/*
//////////////////////////////////////////////////////////////////////////////
void region_clear(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"region_readScene","viewer_clear @current@ all");
  aUI.executeScript("DLD","OnX viewer_clear @current@ all");
}
*/
//////////////////////////////////////////////////////////////////////////////
void page_createRegion(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"page_createRegion","page_create_region @current@...");
  if(!inlib::check_args(aArgs,5,aUI.session().cout())) return;
  std::string s;
  for(unsigned int index=0;index<aArgs.size();index++) s += " " + aArgs[index];
  aUI.executeScript("DLD","OnX page_create_region @current@"+s);
}
//////////////////////////////////////////////////////////////////////////////
void page_addRegions(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"page_addRegions","page_add_regions @current@...");
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  std::string s;
  for(unsigned int index=0;index<aArgs.size();index++) s += " " + aArgs[index];
  aUI.executeScript("DLD","OnX page_add_regions @current@ SoDisplayRegion"+s);
}
//////////////////////////////////////////////////////////////////////////////
void page_createRegions(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"page_createRegions","page_create_regions @current@...");
  if(!inlib::check_args(aArgs,4,aUI.session().cout())) return;
  aUI.executeScript("DLD",
    "OnX page_create_regions @current@ "+aArgs[3]+" "+aArgs[0]+" "+aArgs[1]+" "+aArgs[2]);
}

//////////////////////////////////////////////////////////////////////////////
void page_deleteRegions(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"page_deleteRegions","page_delete_regions @current@");
  aUI.executeScript("DLD","OnX page_delete_regions @current@");
}
//////////////////////////////////////////////////////////////////////////////
void page_deleteRegion(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"page_deleteRegion","page_delete_region @current@");
  aUI.executeScript("DLD","OnX page_delete_region @current@");
}

//////////////////////////////////////////////////////////////////////////////
void page_destroy(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //correct Panoramix first.
  //deprecated1(aUI,"page_destroy",
  //  "See OnX/examples/OnX/page_viewer_stack.onx <destroy> callbacks.");

  std::string name = aUI.callbackData().component();
  //aUI.echo("page_destroy for : "+name);

  // Remove from Viewers pdm :
  std::string prefix = "viewers_item_";
 {std::string s = prefix + name;
  if(aUI.isComponent(s)) aUI.destroyComponent(s);}

  // Set currentViewer to another Viewer.
  // We take the last one in the OnX_viewers pdm.
 {std::vector<std::string> viewers = aUI.findChildren("OnX_viewers","name","");
  if(!viewers.empty()) {
    std::string s = *(viewers.rbegin());
    //viewers_item_Viewer_2
    //012345678901234567890
    std::string last = s.substr(prefix.size(),s.size()-prefix.size());
    set_current_widget(aUI,last);
  } else {
    aUI.setCurrentWidget(0);
  }}
}

//////////////////////////////////////////////////////////////////////////////
void OnX_removeManips(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"OnX_removeManips","viewer_remove_manips @current@");
  aUI.executeScript("DLD","OnX viewer_remove_manips @current@");
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_dumpCamera(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //check Panoramix.
  deprecated(aUI,"OnX_region_dumpCamera","OnX region_dump_camera @current@");
  aUI.executeScript("DLD","OnX region_dump_camera @current@");
}
//////////////////////////////////////////////////////////////////////////////
void OnX_region_resetCamera(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //check Panoramix.
  deprecated(aUI,"OnX_region_resetCamera","OnX region_reset_camera @current@");
  aUI.executeScript("DLD","OnX region_reset_camera @current@");
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnX_Tab_select(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"OnX_Tab_select","OnX ui_setCurrentWidget @this.value@");
  aUI.setCurrentWidget(aUI.findWidget(aUI.callbackValue()));
}
//////////////////////////////////////////////////////////////////////////////
void OnX_Finalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aSession.cout();
  out << "OnX_Finalize : deprecated. Use session_echo." << std::endl;
  out << "OnX_Finalize." << std::endl;
}
//////////////////////////////////////////////////////////////////////////////
void OnX_Page_Delete_Page(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"OnX_Page_Delete_Page","OnX ui_destroy_widget @current@");
  aUI.executeScript("DLD","OnX ui_destroy_widget @current@");
}

//////////////////////////////////////////////////////////////////////////////
void page_create(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"page_create","OnX viewer_add_in_viewers_pdm @this@");
  aUI.executeScript("DLD","OnX viewer_add_in_viewers_pdm @this@");
  aUI.executeScript("DLD","OnX ui_set_current_widget @this@");
}

//////////////////////////////////////////////////////////////////////////////
void viewer_popup(
 Slash::UI::IUI& aUI
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated1(aUI,"viewer_popup",
                  "See examples/OnX/page_viewer.onx <popup> callback.");

  std::string item = aUI.callbackValue();
  if(item == "No highlighted") {
    aUI.executeScript("DLD","OnX region_reset_highlight");
  } else if(item == "Dump highlighted") {
    aUI.executeScript("DLD","OnX viewer_collect");
  } else if(item == "Region to page") {
    aUI.executeScript("DLD","OnX page_currentRegionToPage");
  } else if(item == "Region on top") {
    aUI.executeScript("DLD","OnX page_currentRegionOnTop");
  } else if(item == "Decorations on") {
    aUI.executeScript("DLD","OnX ui_setParameter viewer.decoration TRUE");
  } else if(item == "Decorations off") {
    aUI.executeScript("DLD","OnX ui_setParameter viewer.decoration FALSE");
  }
}

//////////////////////////////////////////////////////////////////////////////
void ui_createComponent(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// arguments : 
//   -class : if not, create a PageViewer.
//   -name : if not, generate one.
//   -parent : if not, take "ViewerTabStack".
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,"ui_createComponent","ui_create_component");

  inlib::args args(aArgs);
  std::string sclass;
  if(!args.find("-class",sclass)) sclass = "PageViewer";
  std::string sparent;
  if(!args.find("-parent",sparent)) sparent = "ViewerTabStack";
  std::string sname;
  if(!args.find("-name",sname)) {
    if(sparent=="ViewerTabStack") {
      int n;
      if(!Slash::parameter<int>(aUI,"ViewerTabStack.itemCount",n)) {
        std::ostream& out = aUI.session().cout();
        out << "ui_createComponent :"
            << " can't get ViewerTabStack.itemCount value."
            << std::endl;
        return;
      }
      if(!inlib::sprintf(sname,64,"Viewer_%d",n+1)) {
        std::ostream& out = aUI.session().cout();
        out << "ui_createComponent :"
            << " problem with Lib::printf."
            << std::endl;
        return;
      }
    } else {
      sname = aUI.createWidgetName();
    }
  }

  if(!aUI.createComponent(sname,sclass,sparent)) {
    std::ostream& out = aUI.session().cout();
    out << "ui_createComponent :"
        << " problem when creating component."
        << std::endl;
    return;
  }

  Slash::UI::IWidget* widget = aUI.findWidget(sname);
  if(widget) {
    // If a PageViewer or an OpenGLArea, set it as current widget.
    Slash::UI::ISoViewer* soViewer = INLIB_CAST(*widget,Slash::UI::ISoViewer);
    if(soViewer) {
      aUI.setCurrentWidget(widget);
    } else if(sclass=="OpenGLArea") {
      aUI.setCurrentWidget(widget);
    }
  }

  /*
  Slash::UI::IWidgetClass* wclass = aUI.findWidgetClass(sclass);
  if(!wclass) {
    std::ostream& out = aUI.session().cout();
    out << "ui_createComponent :"
        << " widget class " << inlib::sout(sclass) << " not found." 
        << std::endl;
    return;
  }

  Slash::UI::IWidget* wparent = aUI.findWidget(sparent);
  if(!wparent) {
    std::ostream& out = aUI.session().cout();
    out << "ui_createComponent :"
        << " parent widget " << inlib::sout(sparent) << " not found." 
        << std::endl;
    return;
  }

  Slash::UI::IWidget* widget = aUI.createWidget(*wclass,sname,*wparent);
  if(!widget) {
    std::ostream& out = aUI.session().cout();
    out << "ui_createComponent :"
        << " can't create widget " << inlib::sout(sname)
        << Lib::dot_endl;
    return;
  }

  // If a PageViewer, set it as current viewer.
  Slash::UI::ISoViewer* soViewer = INLIB_CAST(*widget,Slash::UI::ISoViewer);
  if(soViewer) aUI.setCurrentWidget(soViewer);*/

}

}

#include <Inventor/nodes/SoSeparator.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void region_erase(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated1(aUI,"region_erase",
    "See OnX/script/OnX/Scene.onx pull down menu.");

  if(!aArgs.size()) return;
  if( (aArgs[0]=="static") || (aArgs[0]=="staticScene") ){
    SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
    if(!soPage) return;
    SoRegion* soRegion = soPage->currentRegion();
    if(!soRegion) return;
    if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) return;
    SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;
    SoSeparator* sep = (SoSeparator*)displayRegion->getStaticScene();
    if(sep) sep->removeAllChildren();
  } else if( (aArgs[0]=="dynamic") || (aArgs[0]=="dynamicScene") ) {
    SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
    if(!soPage) return;
    SoRegion* soRegion = soPage->currentRegion();
    if(!soRegion) return;
    if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) return;
    SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;
    SoSeparator* sep = (SoSeparator*)displayRegion->getDynamicScene();
    if(sep) sep->removeAllChildren();
  } else if(aArgs[0]=="highlighted") {
    Slash::Data::collect(aUI.session(),"SceneGraph","highlight==true");
    Slash::Data::destroy(aUI.session());
  } else if(aArgs[0]=="not_highlighted") {
    Slash::Data::collect(aUI.session(),"SceneGraph","highlight==false");
    Slash::Data::destroy(aUI.session());

  } else if(aArgs[0]=="with_manip") {
    Slash::Data::collect(aUI.session(),"SceneGraph","with_manip==true");
    Slash::Data::destroy(aUI.session());
  } else if(aArgs[0]=="without_manip") {
    Slash::Data::collect(aUI.session(),"SceneGraph","with_manip==false");
    Slash::Data::destroy(aUI.session());
  }
}

void OnX_region_readScene(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  deprecated(aUI,"region_readScene","region_read_scene");
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  aUI.executeScript("DLD","OnX region_read_scene @current@ "+aArgs[0]);
}

void OnX_region_readInventorFile(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs)
{
  deprecated(aUI,"region_readInventorFile","region_read_scene");
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  aUI.executeScript("DLD","OnX region_read_scene @current@ "+aArgs[0]);
}

}
