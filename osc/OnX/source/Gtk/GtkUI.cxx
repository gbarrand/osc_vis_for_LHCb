// this :
#include <OnX/Gtk/GtkUI.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ISoViewer.h>

#include "../Core/inlib"

#include <inlib/system>
#include <inlib/cast>
#include <exlib/xml/parser>

#include <Lib/Messages.h>
#include <Lib/Debug.h>

#include <OnX/Core/Widgets.h>

#include <OnX/Gtk/gtkcommandline.h>
#include <OnX/Gtk/gtkwarningdialog.h>
#include <OnX/Gtk/GtkCyclic.h>
#include <OnX/Gtk/GtkTools.h>

//#ifndef __APPLE__
#define ONX_HAS_INVENTOR
//#endif

#ifdef ONX_HAS_INVENTOR
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>
#include <Inventor/Gtk/viewers/SoGtkFlyViewer.h>
#include <Inventor/Gtk/viewers/SoGtkWalkViewer.h>
#include <Slash/Tools/RegionSceneHandler.h>
#include <OnX/Gtk/gtkpageviewer.h>
#include <OnX/Inventor/Inventor.h>
#include <OnX/Inventor/HEPVis.h>
#include <OnX/Inventor/GuiViewer.h>
#endif

#ifdef ONX_HAS_GTK_GL_AREA
#include <gtkgl/gtkglarea.h>
#endif

#ifdef __sun
#include <time.h> // For SunOS-CC-5.1.
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define gtk_WINDOW           1
#define gtk_SCROLLED_TEXT    2
#define gtk_SCROLLED_LIST    3
#define gtk_SCROLLED_TREE    4
#define gtk_DIALOG           5
#define gtk_TOOLBAR_ITEM     6
#define gtk_MENU_SEPARATOR   7
#define gtk_PASSWORD         8
#define gtk_MENUBAR          9
#define gtk_TOOLBAR         10
#define gtk_VCONTAINER      11
#define gtk_HCONTAINER      12

#define gtk_SOEXAMINERVIEWER 13
#define gtk_SOPLANEVIEWER    14
#define gtk_SOFLYVIEWER      15
#define gtk_SOWALKVIEWER     16

#define gtk_PAGEVIEWER       17
#define gtk_PLANEPAGEVIEWER  18

//#define DEBUG_DESTROY

static GtkWidget* findInHierarchy(GtkWidget*,const std::string&);

static int sMessageID = 0;

#define NotFound (-1)

namespace OnX {
namespace Gtk {
class Destroy {
public:
  Destroy(GtkUI& aUI,inlib::xml::tree& aItemML):fUI(aUI),fItemML(aItemML){
    Lib::Debug::increment("OnX::Gtk::Destroy");
  }
  virtual ~Destroy(){
    Lib::Debug::decrement("OnX::Gtk::Destroy");
  }
  bool isValid() const {
    inlib::xml::tree* top = fUI.topItemML();
    return (top && fUI.is_in(*top,&fItemML));
  }
public:
  GtkUI& fUI;
  inlib::xml::tree& fItemML;
};
}}

bool gtk_proxy
  (void*,const std::string&,const std::vector<std::string>&,void*&);

OnX::GtkUI::GtkUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs)
:BaseUI(aSession)
,fTopWindow(0)
,fConsoleText(0)
,fFixedFont(0)
,fAccelGroup(0)
,fExitCode(0)
,fTermCyclic(0)
{
/*
 {bool ui_thread = false;
 {std::string value;
  if(aSession.parameterValue("UI_Manager.thread",value) &&(value.size())) {
    if(!inlib::to(value,ui_thread)) ui_thread = false;
  }}
  if(ui_thread) {
    g_thread_init();
    gtk_threads_init();
  }}
*/

 {int* argc = new int; 
  typedef char** argv_t;
  argv_t* argv = new argv_t;
  new_argcv(aArgs,*argc,*argv);
  gtk_init(argc,argv);}

  std::string sgtkrc;
  if(!inlib::check_getenv(fSession.cout(),"ONX_GTK_RC","ONXGTKRC",sgtkrc)) 
    sgtkrc = "$ONXROOT/scripts/gtk/OnX.gtkrc";
  std::string name;
  inlib::file_name(sgtkrc,name);
  if(name.size()) {
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "OnX::GtkUI::GtkUI :"
          << " load resource file \"" << name << "\"." 
          << std::endl;
    }
    gtk_rc_parse(name.c_str());
  }

  //  It is the UI that takes care of the visualisation viewers.
  // It needs some widget to be initialised.
  GtkWidget* topWidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
#ifdef ONX_HAS_INVENTOR
  SoGtk::init(topWidget);
  HEPVis::initClasses();
  OnX::Inventor::initialize(aSession);
  if(!aSession.findManager(Slash_SceneHandler))  {
    aSession.addManager
      (new Slash::RegionSceneHandler(aSession,Slash_SceneHandler));
  }
#endif

  // Realize top in order to have a window to create pixmaps.
  gtk_widget_realize(topWidget);  
  fTopWindow = topWidget->window;

  fWidgets.push_back(topWidget);

  // Returned value could be NULL.
  fFixedFont = gdk_font_load("*courier*-r-*--14-*");

  fAccelGroup = gtk_accel_group_new();
}

OnX::GtkUI::GtkUI(const GtkUI& aFrom)
:BaseUI(aFrom.fSession)
,fTopWindow(0)
,fConsoleText(0)
,fFixedFont(0)
,fAccelGroup(0)
,fExitCode(0)
,fTermCyclic(0)
{}

OnX::GtkUI::~GtkUI() {
  // Should do a gtk_widget_destroy(top widgets) ; not sure
  // that embedded viewers are ready for that !
  fBeingDestroyed = true;

  // Treat messages emitted between the quiting of the steering
  // and here. They may come from the destruction of various
  // managers in the ~Session.
  synchronize();

  removeCyclics();

  while(!fWidgets.empty()) {
    GtkWidget* widget = *(fWidgets.rbegin());
    if(fVerboseLevel) {
      std::string name = gtk_widget_get_name(widget);
      std::ostream& out = fSession.cout();
      out << "OnX::GtkUI::~GtkUI :"
          << " gtk_widget_destroy \"" << name << "\"..."<< std::endl;
    }
    gtk_widget_destroy(widget); //may induce a fWidget.remove(widget) !
    fWidgets.remove(widget);
  }

  synchronize(); //treat message from the close callbacks.

  //closeCallback Destroys :
 {std::vector<Gtk::Destroy*>::iterator it;
  for(it=fDestroys.begin();it!=fDestroys.end();++it) {
    delete *it;
  }
  fDestroys.clear();}

  fConsoleText = 0;

  if(sMessageID) gtk_timeout_remove(sMessageID);

  // Should destroy the main viewer.
#ifdef ONX_HAS_INVENTOR
  OnX::Inventor::finalize(fSession);
#endif
}

OnX::GtkUI& OnX::GtkUI::operator=(const GtkUI&){return *this;}

void* OnX::GtkUI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(OnX::GtkUI)
  else return BaseUI::cast(a_class);
}

void OnX::GtkUI::put(const std::string& aString){
  // Append line at end of report region.
  if(!fConsoleText) {
    fConsoleText = (GtkWidget*)findNativeWidget("consoleText");
    if(fConsoleText) {
      gtk_signal_connect(GTK_OBJECT(fConsoleText),
                         "destroy",
                         GTK_SIGNAL_FUNC(destroyConsoleTextCallback),this);
      // put cursor at end if having a default value.
      GtkTextBuffer* buffer = 
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(fConsoleText));
      if(buffer) { 
        GtkTextIter iter;  
        gtk_text_buffer_get_end_iter(buffer,&iter);
        GtkTextMark* mark = 
          gtk_text_buffer_create_mark(buffer,NULL,&iter,FALSE);
        if(mark) {
          gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(fConsoleText),mark);
          gtk_text_buffer_delete_mark(buffer,mark);
        }
      }
    }
  }
  if(!fConsoleText) {
    ::printf("%s",aString.c_str());
    return;
  }
  GtkTextBuffer* buffer = 
    gtk_text_view_get_buffer(GTK_TEXT_VIEW(fConsoleText));
  if(!buffer) return;

  GtkTextIter iter;  
  gtk_text_buffer_get_end_iter(buffer,&iter);
  gtk_text_buffer_insert(buffer,&iter,aString.c_str(),aString.length());

  gtk_text_buffer_get_end_iter(buffer,&iter);
  GtkTextMark* mark = gtk_text_buffer_create_mark(buffer,NULL,&iter,FALSE);
  if(!mark) return;
  gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(fConsoleText),mark);
  gtk_text_buffer_delete_mark(buffer,mark);
}

void OnX::GtkUI::warn(const std::string& aString){
  // Modal method.
  GtkWidget* warningDialog = 
    gtk_message_dialog_new (GTK_WINDOW(GTK_WINDOW_TOPLEVEL),
      GTK_DIALOG_MODAL,
      GTK_MESSAGE_WARNING,
      GTK_BUTTONS_OK,
      "%s",aString.c_str());

#if (GTK_MAJOR_VERSION<2) || (GTK_MINOR_VERSION<10) 
#else
  gtk_window_set_deletable(GTK_WINDOW(warningDialog),FALSE);
#endif

  gtk_dialog_run(GTK_DIALOG(warningDialog));
  gtk_widget_destroy(warningDialog);
}

int OnX::GtkUI::nativeSteer(){
  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::GtkUI::steer :"
        << " jumping in gtk_main..."
        << std::endl;
  }

  fExitCode = 0;

  fSteerLevel++;
  gtk_main ();
  fSteerLevel--;

  return fExitCode;
}

void OnX::GtkUI::synchronize(){
  gdk_flush();
  while(g_main_iteration(FALSE)) {}
}

void OnX::GtkUI::exit(int aExitCode){
  fExitCode = aExitCode;
  gtk_main_quit();
}

bool OnX::GtkUI::lock(){
  //GDK_THREAD_LEAVE();

  //GMainLoop *loop
  //LOCK_CONTEXT (loop->context);
  //#define LOCK_CONTEXT(context) g_static_mutex_lock (&context->mutex)

  return false;
}

bool OnX::GtkUI::unlock(){
  //GDK_THREAD_ENTER();

  //GMainLoop *loop
  //UNLOCK_CONTEXT (loop->context);

  return false;
}

namespace OnX {
class GtkMessage {
public:
  GtkMessage(GtkUI& aUI,Slash::Core::IMessage* aMess):fUI(aUI),fMessage(aMess){
    Lib::Debug::increment("OnX::GtkMessage");
  }
  ~GtkMessage() {
    Lib::Debug::decrement("OnX::GtkMessage");
  }
  GtkUI& fUI;
  Slash::Core::IMessage* fMessage;
};
}

bool OnX::GtkUI::notify(Slash::Core::IMessage* aMessage){
  //idmess = gtk_timeout_add(33,(GtkFunction)timer_message,0);
  sMessageID = 
    gtk_idle_add((GtkFunction)timer_message,new GtkMessage(*this,aMessage));
  return true;
}

gint OnX::GtkUI::timer_message(gpointer aTag){
  sMessageID = 0;
  OnX::GtkMessage* guiMessage = (OnX::GtkMessage*)aTag;
  OnX::GtkUI& This = guiMessage->fUI;
  Slash::Core::IMessage* mess = (Slash::Core::IMessage*)guiMessage->fMessage;
  delete guiMessage;
  This.notification(mess);
  return FALSE;
}

void OnX::GtkUI::deleteChildren(const std::string& aName){
  GtkWidget* gtkWidget = (GtkWidget*)findNativeWidget(aName);
  if(!gtkWidget) return;
  if(!GTK_IS_CONTAINER(gtkWidget)) return;
  gtk_container_foreach(GTK_CONTAINER(gtkWidget),
                        (GtkCallback)gtk_widget_destroy,0);
}

bool OnX::GtkUI::parameterValue(const std::string& aWhat,std::string& aValue){
  aValue = "";
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::GtkUI::parameterValue :"
        << " widget \"" << aWidget << "\""
        << " not found." << std::endl;
    return false;
  }
  GtkWidget* gtkWidget = (GtkWidget*)itemML->get_data1();
  GtkWidget* gtkContainer = (GtkWidget*)itemML->get_data2();
  if(gtkWidget) {
    if(aResource=="selection") {
      if(GTK_IS_NOTEBOOK(gtkWidget)) {
        return gtk_notebook_get_selection(GTK_NOTEBOOK(gtkWidget),aValue);
      } else if(GTK_IS_LIST(gtkWidget)) {
        return gtk_list_get_selection(GTK_LIST(gtkWidget),aValue);
      } else if(GTK_IS_TREE_VIEW(gtkWidget)) {
        aValue = getTreeSelection(gtkWidget);
        return true;
      }
    } else if(aResource=="label") {
      if(GTK_IS_LABEL(gtkWidget)) {
        gchar* s = 0;
        gtk_label_get(GTK_LABEL(gtkWidget),&s);
        aValue = s?s:"";
        return true;
      } else if(GTK_IS_MENU_ITEM(gtkWidget)) {
        GtkWidget* child = GTK_BIN(gtkWidget)->child;
        if(child) {
          gchar* s = 0;
          gtk_label_get(GTK_LABEL(child),&s);
          aValue = s?s:"";
          return true;
        }
      } else if (GTK_IS_OPTION_MENU(gtkWidget)) {
        GList* hBoxList = 
          gtk_container_get_children
            (GTK_CONTAINER(gtk_widget_get_parent(gtkWidget))); //REVISIT
        int number = g_list_length(hBoxList);
        for(int index=0;index<number;index++) {
          if(GTK_IS_LABEL(g_list_nth_data(hBoxList,index))) {
            gchar* s = 0;
            gtk_label_get(GTK_LABEL(g_list_nth_data(hBoxList,index)),&s);
            aValue = s?s:"";
            return true;
          }
        }
        g_list_free(hBoxList);
      } else if(GTK_IS_BUTTON(gtkWidget)) {
        GtkWidget *label = GTK_BIN(GTK_BUTTON(gtkWidget))->child;
        if(GTK_IS_LABEL(label)) {
          gchar* s = 0;
          gtk_label_get(GTK_LABEL(label),&s);
          aValue = s?s:"";
          return true;
        } else if(GTK_IS_TOOLBAR(gtkWidget->parent)){
          //this is probably a ToolBarItem
          GtkToolbar *toolbar = GTK_TOOLBAR(gtkWidget->parent);
          for (GList* children = toolbar->children; 
            children; children = children->next) {
                 GtkToolbarChild *child = (GtkToolbarChild*)(children->data);
            if (child->widget == gtkWidget)
              if(GTK_IS_LABEL(child->label)) {
                gchar* s = 0;
                gtk_label_get(GTK_LABEL(child->label),&s);
                aValue = s?s:"";
                return true;
              }
          }
        }
      }
    } else if(aResource=="tabLabel") { //Constraint resource
      //Constraint resource, we have to work on the parent container.
      if(gtkContainer && gtkContainer->parent) {
        if(gtkContainer->parent && GTK_IS_NOTEBOOK(gtkContainer->parent)){
          GtkWidget* label = 
            gtk_notebook_get_tab_label
          (GTK_NOTEBOOK(gtkContainer->parent),gtkWidget);
          if(label && GTK_IS_LABEL(label)) {
            gchar* s = 0;
            gtk_label_get(GTK_LABEL(label),&s);
            aValue = s?s:"";
            return true;
          }
        }
      }
    } else if(aResource=="tooltip") {
      if(GTK_IS_TOOLBAR(gtkWidget->parent)){
        GtkToolbar *toolbar = GTK_TOOLBAR(gtkWidget->parent);
        if (GTK_IS_TOOLTIPS(toolbar->tooltips)){
          // find the widget child
          bool found = false;
          for (GList* children = toolbar->children; 
               children; children = children->next) {
            GtkToolbarChild *child = (GtkToolbarChild*)(children->data);
            if (child->widget == gtkWidget) {
              GtkTooltipsData* d = gtk_tooltips_data_get(child->widget);
              if (d) {
                aValue = d->tip_text?d->tip_text:"";
                  found = true;
                break;
              }
            }
          }
          if (found) return true;
        }
      }
    } else if(aResource=="itemCount") {
      if(GTK_IS_NOTEBOOK(gtkWidget)) {
        int value  = g_list_length(GTK_NOTEBOOK(gtkWidget)->children);
        inlib::sprintf(aValue,64,"%d",value);
        return true;
      }
    } else if(aResource=="title") {
      if(GTK_IS_WINDOW(gtkWidget)) {
        const gchar * s = gtk_window_get_title(GTK_WINDOW(gtkWidget));
        aValue = s?s:"";
        return true;
      } else if(GTK_IS_WINDOW(gtk_widget_get_toplevel(gtkWidget))) {
        const gchar * s = 
          gtk_window_get_title(GTK_WINDOW(gtk_widget_get_toplevel(gtkWidget)));
        aValue = s?s:"";
        return true;
      }
    } else if(aResource=="value") {
      if(GTK_IS_ENTRY(gtkWidget)) {
        const gchar* s = gtk_entry_get_text(GTK_ENTRY(gtkWidget));
        aValue = s?s:"";
        return true;
      } else if(GTK_IS_COMBO(gtkWidget)) {
        return gtk_combo_get_value(GTK_COMBO(gtkWidget),aValue);
      } else if(GTK_IS_OPTION_MENU(gtkWidget)) {
        return gtk_option_menu_get_value(GTK_OPTION_MENU(gtkWidget),aValue);
      } else if(GTK_IS_RANGE(gtkWidget)) {
        float value = gtk_range_get_adjustment(GTK_RANGE(gtkWidget))->value;
        inlib::sprintf(aValue,64,"%d",(int)value);
        return true;
      } else if (GTK_IS_TEXT_VIEW(gtkWidget)) {
        GtkTextIter start;
        GtkTextIter end;
        GtkTextBuffer* text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtkWidget));
        gtk_text_buffer_get_start_iter(text_buffer,&start);
        gtk_text_buffer_get_end_iter(text_buffer,&end);
        gchar* s = gtk_text_buffer_get_text(text_buffer,&start, &end,TRUE);

        aValue = s?s:"";
        g_free(s);
        return true;
      }
      else if(GTK_IS_PROGRESS_BAR(gtkWidget)) {
        gfloat value = gtk_progress_get_value(GTK_PROGRESS(gtkWidget));
        inlib::sprintf(aValue,64,"%d",(int)value);
        return true;
      }
    } else if(aResource=="set") {
      if(GTK_IS_CHECK_MENU_ITEM(gtkWidget)) {
        aValue = GTK_CHECK_MENU_ITEM(gtkWidget)->active ? "true" : "false";
        return true;
      } else if(GTK_IS_TOGGLE_BUTTON(gtkWidget)) {
        aValue = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtkWidget)) ?
          "true" : "false";
        return true;
      }
    } else if(aResource=="items") {
      if(GTK_IS_LIST(gtkWidget)) {
        std::vector<std::string> v;
        if(!gtk_list_get_items(GTK_LIST(gtkWidget),v)) return false;
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      } else if(GTK_IS_COMBO(gtkWidget)) {
        std::vector<std::string> v;
        if(!gtk_combo_get_items(GTK_COMBO(gtkWidget),v)) return false;
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      } else if(GTK_IS_TREE_VIEW(gtkWidget)) {
        GtkTreeModel* model = 
          gtk_tree_view_get_model(GTK_TREE_VIEW(gtkWidget)); 
        if(model) {
          GtkTreeIter top;
          gtk_tree_model_get_iter_first(model,&top);
          aValue = "<tree>"+
                   gtk_tree_model_get_XML(model,GTK_TREE_VIEW(gtkWidget),&top)+
                   "</tree>";
        } else {
          aValue = "";
        }
        return true;
      }

      //FIXME : option.
    }
  }
  // Not found, then look in XML elements :
  if(aResource=="class") {
    return itemML->attribute_value(aResource,aValue);
  }
  return itemML->element_value(aResource,aValue);
}

bool OnX::GtkUI::setParameter(const std::string& aWhat,const std::string& aValue){ 
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::GtkUI::setParameter :"
        << " widget \"" << aWidget << "\""
        << " resource \"" << aResource << "\""
        << " value \"" << aValue << "\"."
        << std::endl;
  }

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::GtkUI::setParameter :"
        << " widget \"" << aWidget << "\""
        << " not found." << std::endl;
    return false;
  }

  GtkWidget* gtkWidget = (GtkWidget*)itemML->get_data1();
  if(!gtkWidget) return false;
  GtkWidget* gtkContainer = (GtkWidget*)itemML->get_data2();

  std::string svalue;

  if(aResource=="background") {
    if(GTK_IS_MENU_ITEM(gtkWidget)) {
      if(gtk_menu_item_set_background(GTK_MENU_ITEM(gtkWidget),aValue))
        return true;
    } else if (GTK_IS_LABEL(gtkWidget)) {
      if(gtk_widget_set_background(gtk_widget_get_parent(gtkWidget),aValue))
        return true;
    } else if (GTK_IS_BUTTON(gtkWidget)) {
      if(gtk_button_set_background(GTK_BUTTON(gtkWidget),aValue))
        return true;
    } else if (GTK_IS_TREE_VIEW(gtkWidget)) {
      if(gtk_widget_set_background2(gtkWidget,aValue))
        return true;
    } else {
      if(gtk_widget_set_background(gtkWidget,aValue))
        return true;
    }
  } else if(aResource=="foreground") {
    if(GTK_IS_LABEL(gtkWidget)) {
      if(gtk_widget_set_foreground(gtk_widget_get_parent(gtkWidget),aValue))
        return true;
    } else if(GTK_IS_MENU_ITEM(gtkWidget)) {
      if(gtk_menu_item_set_foreground(GTK_MENU_ITEM(gtkWidget),aValue))
        return true;
    } else if(GTK_IS_BUTTON(gtkWidget)) {
      if(gtk_button_set_foreground(GTK_BUTTON(gtkWidget),aValue))
        return true;
    } else {
      if(gtk_widget_set_foreground(gtkWidget,aValue))
        return true;
    }
  } else if(aResource=="label") {
    if(GTK_IS_LABEL(gtkWidget)) {
      gtk_label_set_text(GTK_LABEL(gtkWidget),aValue.c_str());
      return true;
    } else if(GTK_IS_MENU_ITEM(gtkWidget)) {
      GtkWidget* child = GTK_BIN(gtkWidget)->child;
      if(child) {
        gtk_label_set_text(GTK_LABEL(child),aValue.c_str());
        return true;
      }
    } else if(GTK_IS_BUTTON(gtkWidget)) {
      if (GTK_IS_OPTION_MENU(gtkWidget)) {
        GList* hBoxList = 
          gtk_container_get_children
            (GTK_CONTAINER(gtk_widget_get_parent(gtkWidget)));
        bool found = false;
        int number = g_list_length(hBoxList);
        for(int index=0;index<number;index++) {
          if(GTK_IS_LABEL(g_list_nth_data(hBoxList,index))) {
            gtk_label_set_text
              (GTK_LABEL(g_list_nth_data(hBoxList,index)),aValue.c_str());
            found = true;
          }
        }
        g_list_free(hBoxList);
        if (found) return true;
      }
      else {
        GtkWidget *label = GTK_BIN (GTK_BUTTON(gtkWidget))->child;
        if(GTK_IS_LABEL(label)) {
          gtk_label_set_text(GTK_LABEL(label),aValue.c_str());
          return true;
        } else if(GTK_IS_TOOLBAR(gtkWidget->parent)){
          //this is probably a ToolBarItem
          // FIXME : we can't set the label or icon after the 
          // creation of a new toolbaritem (in UI builder for ex)
          // if no one has been specified at widget creation.

          GtkToolbar *toolbar = GTK_TOOLBAR(gtkWidget->parent);
          bool found = false;
          for (GList* children = toolbar->children; 
               children; children = children->next) {
            GtkToolbarChild *child = (GtkToolbarChild*)(children->data);
            if (child->widget == gtkWidget)
              if(GTK_IS_LABEL(child->label)) {
                 gtk_label_set_text(GTK_LABEL(child->label),aValue.c_str());
                 found = true;
                 break;
              }
          }
          if (found) return true;
        }
      }
    }
  } else if(aResource=="tooltip") {
    if(GTK_IS_TOOLBAR(gtkWidget->parent)){
      GtkToolbar *toolbar = GTK_TOOLBAR(gtkWidget->parent);
      if (GTK_IS_TOOLTIPS(toolbar->tooltips)){
        // find the widget child
        bool found = false;
        for (GList* children = toolbar->children; 
             children; children = children->next) {
          GtkToolbarChild *child = (GtkToolbarChild*)(children->data);
          if (child->widget == gtkWidget) {
            gtk_tooltips_set_tip (toolbar->tooltips,child->widget,aValue.c_str(), NULL);
            found = true;
            break;
          }
        }
        if (found) return true;
      }
    }
  } else if(aResource=="pixmap") {
    if(GTK_IS_TOOLBAR(gtkWidget->parent)){
      // FIXME : we can't set the pixmap after the 
      // creation of a new toolbaritem (in UI builder for ex)
      // if no one has been specified at widget creation.

      GtkToolbar* toolbar = GTK_TOOLBAR(gtkWidget->parent);
      // find the widget child
      bool found = false;
      for (GList * children = toolbar->children; 
           children; children = children->next) {
        GtkToolbarChild* child = (GtkToolbarChild*)(children->data);
        if (child->widget == gtkWidget) {
          if(child->icon && GTK_IS_PIXMAP(child->icon)) {
            gtk_pixmap_set_from_file
              (GTK_PIXMAP(child->icon),fTopWindow,aValue);
            found = true;
            break;
          }
        }
      }
      if (found) return true;
    }
  } else if(aResource=="dirMask") {
    if(GTK_IS_FILE_SELECTION(gtkWidget)) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      //FIXME : how to do a selection ?
      return true;
    }
  } else if(aResource=="directory") {
    if(GTK_IS_FILE_SELECTION(gtkWidget)) {
      std::string directory = aValue;
      if(directory.size()) {
        directory += inlib::sep();
      }
      inlib::expand_vars(directory);
      gtk_file_selection_set_filename(GTK_FILE_SELECTION(gtkWidget),
                                      directory.c_str());
      return true;
    }
  } else if(aResource=="selection") {
    if(GTK_IS_NOTEBOOK(gtkWidget)) {
      if(gtk_notebook_set_selection(GTK_NOTEBOOK(gtkWidget),aValue)) 
        return true;
    } else if(GTK_IS_LIST(gtkWidget)) {
      if(gtk_list_set_selection(GTK_LIST(gtkWidget),aValue)) 
        return true;
    } else if(GTK_IS_TREE_VIEW(gtkWidget)) {
      setTreeSelection(gtkWidget,aValue);
      return true;
    }
  } else if(aResource=="set") {
    bool value;
    if(inlib::to(aValue,value)) {
      if(GTK_IS_CHECK_MENU_ITEM(gtkWidget)) {
        gtk_check_menu_item_set_active(
          GTK_CHECK_MENU_ITEM(gtkWidget),value ? TRUE : FALSE);
        return true;
      } else if(GTK_IS_TOGGLE_BUTTON(gtkWidget)) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget),
          value ? TRUE : FALSE);
        return true;
      }
    }
  } else if(aResource=="splitPosition") {
    if(GTK_IS_PANED(gtkWidget)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        gtk_paned_set_position(GTK_PANED(gtkWidget),value);
        return true;
      }     
    } 
  } else if(aResource=="expand") {
    if ((GTK_IS_VBOX(gtkWidget)) || 
        (GTK_IS_HBOX(gtkWidget))) {
      return gtk_box_set_expand(GTK_BOX(gtkWidget),aValue);
    }
  } else if(aResource=="value") {
    if(GTK_IS_ENTRY(gtkWidget)) {
      gtk_entry_set_text(GTK_ENTRY(gtkWidget),aValue.c_str());
      return true;
    } else if(GTK_IS_TEXT_VIEW(gtkWidget)) {
      OnX::gtk_text_view_set_value(GTK_TEXT_VIEW(gtkWidget),aValue);
      return true;
    } else if(GTK_IS_PROGRESS_BAR(gtkWidget)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        gfloat percent = GTK_PROGRESS_BAR(gtkWidget)->blocks;
        percent = ((gfloat)value)/percent;
        if(percent<0) percent = 0;
        else if(percent>1) percent = 1;
        gtk_progress_bar_update(GTK_PROGRESS_BAR(gtkWidget),percent);
        return true;
      }
    } else if(GTK_IS_RANGE(gtkWidget)) {
      double value;
      if(inlib::to<double>(aValue,value)) {
        gtk_adjustment_set_value
          (gtk_range_get_adjustment(GTK_RANGE(gtkWidget)),(float)value);
        return true;
      }
    } else if(GTK_IS_COMBO(gtkWidget)) {
      gtk_combo_set_value(GTK_COMBO(gtkWidget),aValue);
      return true;
    } else if(GTK_IS_OPTION_MENU(gtkWidget)) {
      if(gtk_option_menu_set_default(GTK_OPTION_MENU(gtkWidget),aValue)) 
        return true;
    }
  } else if(aResource=="maximum") {
    if(GTK_IS_SCALE(gtkWidget)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        GtkAdjustment* adjustment = 
          gtk_range_get_adjustment(GTK_RANGE(gtkWidget));
        float xvalue = adjustment->value;
        float slider = adjustment->page_size;
        adjustment->upper = value+slider;
        gtk_adjustment_set_value(adjustment,(int)xvalue);
        return true;
      }
    }
  } else if(aResource=="minimum") {
    if(GTK_IS_SCALE(gtkWidget)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        GtkAdjustment* adjustment = 
          gtk_range_get_adjustment(GTK_RANGE(gtkWidget));
        float xvalue = adjustment->value;
        adjustment->lower = value;
        gtk_adjustment_set_value(adjustment,(int)xvalue);
        return true;
      }
    }
  } else if(aResource=="default") {
    if(GTK_IS_OPTION_MENU(gtkWidget)) {
      gtk_option_menu_set_default(GTK_OPTION_MENU(gtkWidget),aValue); 
      return true;
    } else if(GTK_IS_COMBO(gtkWidget)) {
      gtk_combo_set_value(GTK_COMBO(gtkWidget),aValue);
      return true;
    }
  } else if(aResource=="items") {
    if(GTK_IS_LIST(gtkWidget)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      gtk_list_clear_items(GTK_LIST(gtkWidget),0,-1);
      unsigned int itemn = items.size();
      for(unsigned int index=0;index<itemn;index++){
        GtkWidget *item = 
          gtk_list_item_new_with_label((char*)items[index].c_str());
        gtk_widget_show(item);
        gtk_container_add(GTK_CONTAINER(gtkWidget),item);
      }
      return true;
    } else if(GTK_IS_COMBO(gtkWidget)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      gtk_combo_set_items(GTK_COMBO(gtkWidget),items);
      return true;
    } else if(GTK_IS_OPTION_MENU(gtkWidget)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      std::string def;
      itemML->element_value("default",def);
      gtk_option_menu_initialize(GTK_OPTION_MENU(gtkWidget),items,def);
      return true;
    } else if(GTK_IS_TREE_VIEW(gtkWidget)) {
      // retrieve the old selection
      std::string selection = getTreeSelection(gtkWidget);

      gtk_container_foreach(GTK_CONTAINER(gtkWidget),
                            (GtkCallback)gtk_widget_destroy,0);
      inlib::xml::default_factory factory;
      exlib::xml::parser treeML(factory,out(),fSession.verboseLevel());
      std::vector<std::string> tags;
      tags.push_back("tree");
      tags.push_back("treeItem");
      treeML.set_tags(tags);
      if(treeML.load_string(aValue)) {
        inlib::xml::tree* top = treeML.top_item();
        //top->dump();
        if(top) createTree(gtkWidget,top,out());
      }

      // disable the scripts of the callback to avoid recursive loop 
      // if someone has define the same callback in the script :
      std::vector<std::string> scripts = 
        disableCallbackScript(gtkWidget,"select");
      // restore selection
      setTreeSelection(gtkWidget,selection);
      // enable callback scripts :
      enableCallbackScript(gtkWidget,"select",scripts);

      return true;
    }
#ifdef ONX_HAS_INVENTOR
  } else if(aResource=="popupItems") {
    if(GTK_IS_PAGE_VIEWER(gtkWidget)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      for(unsigned int index=0;index<items.size();index++)
        gtk_page_viewer_add_popup_entry(GTK_PAGE_VIEWER(gtkWidget),
                                        items[index]);
      return true;
    }
  } else if(aResource=="title") {
    if(GTK_IS_WINDOW(gtkWidget)) {
      gtk_window_set_title(GTK_WINDOW(gtkWidget),aValue.c_str());
      return true;
    } else if(GTK_IS_WINDOW(gtk_widget_get_toplevel(gtkWidget))) {
        gtk_window_set_title(GTK_WINDOW(gtk_widget_get_toplevel(gtkWidget)),aValue.c_str());
      return true;
    }

#endif
  } else if(aResource=="mode") {
    return true;
  } else if(aResource=="tabLabel") { //Constraint resource
    //Constraint resource, we have to work on the parent.
    if(gtkContainer && gtkContainer->parent) {
      if(gtkContainer->parent && GTK_IS_NOTEBOOK(gtkContainer->parent)){
        gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(gtkContainer->parent),
                                        gtkContainer,aValue.c_str());
        return true;
      }
    }
  }

  std::ostream& out = fSession.cout();
  out << "OnX::GtkUI::setParameter :"
      << " unable to set parameter for : widget \"" << aWidget << "\""
      << " widget \"" << aWidget << "\""
      << " resource \"" << aResource << "\""
      << " value \"" << aValue << "\"."
      << std::endl;

  return false;
}

bool OnX::GtkUI::createTree(GtkWidget* aView,inlib::xml::tree* aItemML,std::ostream& a_out){
  // clear all the old tree if it is present
  if(! GTK_IS_TREE_VIEW(aView)) return false;
  GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(aView)); 
  if (model) {

    // view old model content
    inlib::xml::tree* oldTreeItemML = 0;
    GtkTreeIter top;
    gtk_tree_model_get_iter_first(model,&top);
    std::string line;
    line = "<tree>"+
      gtk_tree_model_get_XML(model,GTK_TREE_VIEW(aView),&top)+"</tree>";

    inlib::xml::default_factory factory;
    exlib::xml::parser treeML(factory,a_out);
    std::vector<std::string> tags;
    tags.push_back("tree");
    tags.push_back("treeItem");
    treeML.set_tags(tags);
    if(treeML.load_string(line)) {
      oldTreeItemML = treeML.top_item();
    }
    if (!oldTreeItemML) return false;
    aItemML->update_tree(*oldTreeItemML);
    GtkTreeStore* treestore = gtk_tree_store_new(1, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(aView), GTK_TREE_MODEL(treestore));
    fillModel(aView,treestore,NULL,aItemML);
    
    gtk_tree_selection_set_mode
     (gtk_tree_view_get_selection(GTK_TREE_VIEW(aView)),GTK_SELECTION_SINGLE); 
  } else {
    GtkTreeViewColumn* col = gtk_tree_view_column_new();  
    
    gtk_tree_view_append_column(GTK_TREE_VIEW(aView), col);
    GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col,renderer,"text",GTK_TREE_COL_TEXT);
    
    //model
    GtkTreeStore* treestore = gtk_tree_store_new(1, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(aView), GTK_TREE_MODEL(treestore));
    fillModel(aView,treestore,NULL,aItemML);
    //end model
    
    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(aView)), GTK_SELECTION_SINGLE); 
  }
  return true;
}

bool OnX::GtkUI::fillModel(
 GtkWidget* aWidget
,GtkTreeStore* aTreestore
,GtkTreeIter* aParent
,inlib::xml::tree* aItemML
){
  GtkTreeIter child;
  std::list<inlib::xml::tree*> list;
  aItemML->sub_trees(list);
  if(!list.size()) return true;
  
  aItemML->set_data(aWidget,0,0);

  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    std::string slabel;
    (*it)->element_value("label",slabel);
    void* data;
    void* ptree;
    int dataInt;
    (*it)->parent()->get_data(ptree,data,dataInt);
    void* tree;
    (*it)->get_data(tree,data,dataInt);
    gtk_tree_store_append(aTreestore, &child, aParent);
    gtk_tree_store_set(aTreestore, &child,
                       GTK_TREE_COL_TEXT,slabel.c_str(),-1);
    (*it)->set_data(tree,aWidget,0);
    std::string sopened;

    bool opened = false;
    if((*it)->element_value("opened",sopened)) {
      inlib::to(sopened,opened);
    }

    bool fill = fillModel(aWidget,aTreestore,&child,*it);

    if (opened) {
      GtkTreePath* path = gtk_tree_model_get_path(GTK_TREE_MODEL(aTreestore),&child);
      gtk_tree_view_expand_to_path(GTK_TREE_VIEW(aWidget),path); 
      gtk_tree_path_free(path);
    }
    if (!fill)  return false;
  }

  return true;
}

bool OnX::GtkUI::createNativeWidget(
 inlib::xml::tree& aItemML                      
,const std::string& aClass
,const std::string& aName
,inlib::xml::tree* aParent
){
    OnX::Widget* owidget = INLIB_CAST(aItemML,OnX::Widget);
    if(!owidget) {
      std::ostream& out = fSession.cout();
      out << "GtkUI::createNativeWidget :"
          << " ItemML not a OnX::Widget." 
          << aClass << std::endl;
      return false;
    }

    int compoundId = getCompound(aClass);
    GtkType gtkType = getType(aClass);
    if( (compoundId==0) && (gtkType==GTK_TYPE_INVALID) ) {
      std::ostream& out = fSession.cout();
      out << "GtkUI::createNativeWidget :"
          << " unknown widget type : " << aClass << std::endl;
      return false;
    }
    
    // Get parent :
    GtkWidget* gtkParent = 0;
    GtkWidget* gtkParentContainer = 0;
    int parentCompoundId = 0;
    GtkType gtkParentType = 0;
    if(aParent) {
      gtkParent = (GtkWidget*)aParent->get_data1();
      gtkParentContainer = (GtkWidget*)aParent->get_data2();
      std::string sparentclass; 
      aParent->attribute_value("class",sparentclass);
      parentCompoundId = getCompound(sparentclass);
      if(gtkParent) gtkParentType = GTK_WIDGET_TYPE(gtkParent);

      // Get the parent number of child
      if((parentCompoundId==gtk_VCONTAINER) ||
         (parentCompoundId==gtk_HCONTAINER) ){
      } else {
        if ((gtkParentType==gtk_vpaned_get_type()) ||
            (gtkParentType==gtk_hpaned_get_type()) ||
            (gtkParentType==gtk_vbox_get_type())   ||
            (gtkParentType==gtk_hbox_get_type())   ){
          if(g_list_length(gtk_container_get_children
	  		 (GTK_CONTAINER(gtkParent)))>=2) {
            std::ostream& out = fSession.cout();
            out << "GtkUI::createNativeWidget :"
                << " could not create a 3rd widget in  : " 
                << sparentclass <<std::endl;
            return false;
          }
        }
      }
    }   
    
    // List of gtk1.2 widgets without window (gadgets) :
    // GtkAlignment, GtkArrow, GtkBin, GtkBox, GtkImage, GtkItem
    // GtkLabel, GtkPixmap, GtkScrolledWindow, GtkSeparator, GtkTable
    // GtkAspectFrame, GtkFrame, GtkVBox, GtkHBox, GtkVSeparator, GtkHSeparator


    // List of gtk2 widgets without window (gadgets) :
    // GtkAlignment, GtkArrow, GtkBin, GtkBox, GtkImage
    // GtkLabel,            GtkScrolledWindow, GtkSeparator, GtkTable
    // GtkAspectFrame, GtkFrame, GtkVBox, GtkHBox, GtkVSeparator, GtkHSeparator
    // GtkButton, GtkCheckButton, GtkFixed, GtkMenuItem, GtkNotebook,
    // GtkPaned, GtkRadioButton, GtkRange, GtkToolbar,

    //  These are put in an GtkEventBox in order to get a window
    // to set background color.

#define CHECK_PARENT \
      if(!gtkParent) {\
        std::ostream& out = fSession.cout();\
        out << "GtkUI::createNativeWidget : " << aClass \
            << " needs a parent." << std::endl;\
        return false;\
      }  

    // Create the gtk widget :
    std::string svalue;
    std::string sdef;
    GtkWidget* gtkWidget = 0;
    GtkWidget* gtkContainer = 0;
    if(gtkType==gtk_check_menu_item_get_type()) {
      aItemML.element_value("label",svalue);
      gtkWidget = gtk_check_menu_item_new_with_label(svalue.c_str());
      gtk_widget_show(gtkWidget);
    } else if(gtkType==gtk_vbox_get_type()) {
      gtkWidget = gtk_vbox_new (FALSE, 0);
      gtk_widget_show(gtkWidget);
    } else if(gtkType==gtk_label_get_type()) {
      // GtkLabel has no window, we parent it with a windowed container.
      gtkContainer = gtk_event_box_new();
      gtk_widget_show(gtkContainer);
      if(!aItemML.element_value("label",svalue))
        aItemML.element_value("labelString",svalue);
      gtkWidget = gtk_label_new(svalue.c_str());
      gtk_widget_show(gtkWidget);
      gtk_container_add(GTK_CONTAINER(gtkContainer),gtkWidget);
    } else if(gtkType==gtk_button_get_type()) {
      aItemML.element_value("label",svalue);
      gtkWidget = gtk_button_new_with_label(svalue.c_str());
      gtk_widget_show(gtkWidget);
    } else if(gtkType==gtk_toggle_button_get_type()) {
      aItemML.element_value("label",svalue);
      gtkWidget = gtk_toggle_button_new_with_label(svalue.c_str());
      gtk_widget_show(gtkWidget);
    } else if(gtkType==gtk_menu_item_get_type()) {
      aItemML.element_value("label",svalue);
      gtkWidget = gtk_menu_item_new_with_label(svalue.c_str());
      gtk_widget_show(gtkWidget); //For Viewers pdm.
  /*} else if(gtkType==gtk_tree_item_get_type()) {
      aItemML.element_value("label",svalue);
      gtkWidget = gtk_tree_item_new_with_label((char*)svalue.c_str());
      gtk_widget_show(gtkWidget); // Seems needed. */

    } else if(gtkType==gtk_option_menu_get_type()) {
      gtkContainer = gtk_hbox_new(FALSE,10);
      gtk_widget_show(gtkContainer);
      aItemML.element_value("label",svalue);
      GtkWidget* label = gtk_label_new((char*)svalue.c_str());
      gtk_widget_show(label);
      gtk_container_add(GTK_CONTAINER(gtkContainer),label);
      gtkWidget = gtk_option_menu_new();
      gtk_widget_show(gtkWidget);
      gtk_container_add(GTK_CONTAINER(gtkContainer),gtkWidget);
      GtkWidget* menu = gtk_menu_new();
      //??? gtk_widget_show(menu);
      gtk_option_menu_set_menu(GTK_OPTION_MENU(gtkWidget),menu);
    } else if(gtkType==gtk_hscrollbar_get_type()) {
      int slider = 10;
      GtkObject* adjustment = gtk_adjustment_new(0,0,1000+slider,1,1,slider);
      gtkWidget = gtk_hscrollbar_new(GTK_ADJUSTMENT(adjustment));
      gtk_widget_show(gtkWidget);
    } else if(gtkType==gtk_vscrollbar_get_type()) {
      int slider = 10;
      GtkObject* adjustment = gtk_adjustment_new(0,0,1000+slider,1,1,slider);
      gtkWidget = gtk_vscrollbar_new(GTK_ADJUSTMENT(adjustment));
      gtk_widget_show(gtkWidget);

    } else if((gtkType==gtk_hscale_get_type()) ||
              (gtkType==gtk_vscale_get_type()) ) {
      int slider = 10;
      int current = 0;
      int mn = 0;
      if(aItemML.element_value("minimum",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          mn = value;
        }
      }
      int mx = 1000+slider;
      if(aItemML.element_value("maximum",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          mx = value;
        }
      }
      if(aItemML.element_value("value",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          current = value;
        }
      }
      GtkObject* adjustment = gtk_adjustment_new(current,mn,mx,1,1,slider);
      if(gtkType==gtk_hscale_get_type()) {
        gtkWidget = gtk_hscale_new(GTK_ADJUSTMENT(adjustment));
      } else {
        gtkWidget = gtk_vscale_new(GTK_ADJUSTMENT(adjustment));
      }
      gtk_widget_show(gtkWidget);

    } else if(gtkType==gtk_progress_bar_get_type()) {
      gtkWidget = gtk_progress_bar_new();
      gtk_widget_show(gtkWidget);
      gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(gtkWidget),100);
      gtk_progress_bar_update(GTK_PROGRESS_BAR(gtkWidget),0);
#ifdef ONX_HAS_GTK_GL_AREA
    } else if(gtkType==gtk_gl_area_get_type()) {

      int glAttributes[16];
      int i = 0;
      glAttributes[i] = GDK_GL_RGBA; i++;
      glAttributes[i] = GDK_GL_DOUBLEBUFFER; i++;
      glAttributes[i] = GDK_GL_DEPTH_SIZE; i++;
      glAttributes[i] = 1; i++;
      glAttributes[i] = GDK_GL_NONE; i++;
      gtkWidget = gtk_gl_area_new(glAttributes);    
      gtk_widget_show(gtkWidget);
      //FIXME : gtk2 : the below avoid a crash in case doing a
      //              gtk_widget_destroy on a container having a GtkGLArea.
      gdk_gl_context_ref(GTK_GL_AREA(gtkWidget)->glcontext);
#endif
    } else if(gtkType==gtk_warning_dialog_get_type()) {
      gtkWidget = gtk_warning_dialog_new();
      //gtk_widget_show(gtkWidget); //not done here.
      gtk_window_set_position(GTK_WINDOW(gtkWidget),GTK_WIN_POS_CENTER);
      gtk_window_set_default_size(GTK_WINDOW(gtkWidget),300,100);

#if (GTK_MAJOR_VERSION<2) || (GTK_MINOR_VERSION<10) 
#else
      gtk_window_set_deletable(GTK_WINDOW(gtkWidget),FALSE);
#endif

     {fWidgets.push_back(gtkWidget);
      gtk_signal_connect(GTK_OBJECT(gtkWidget), 
                         "destroy",
                         GTK_SIGNAL_FUNC(destroyShellCallback),
                         this);}

    } else if(compoundId==gtk_WINDOW) {
      // Emulate a "main window" with internal menubar and toolbar.
      gtkContainer = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      //gtk_widget_show(gtkContainer); //not done here.
     {fWidgets.push_back(gtkContainer);
      OnX::Gtk::Destroy* dst = new OnX::Gtk::Destroy(*this,aItemML);
      gtk_signal_connect(GTK_OBJECT(gtkContainer), 
                         "delete_event",
                         GTK_SIGNAL_FUNC(closeCallback),
                         dst);
      fDestroys.push_back(dst);
      gtk_signal_connect(GTK_OBJECT(gtkContainer), 
                         "destroy",
                         GTK_SIGNAL_FUNC(destroyShellCallback),
                         this);
      }
      gtk_window_add_accel_group(GTK_WINDOW(gtkContainer),fAccelGroup);
      GtkWidget* box = gtk_vbox_new(FALSE, 0);
      gtk_widget_show(box);
      gtk_container_add(GTK_CONTAINER(gtkContainer),box);
      GtkWidget* menuBar = gtk_menu_bar_new();
      gtk_widget_show(menuBar);
      gtk_widget_set_name(menuBar,(char*)"OnX_gtk_menuBar");
      GtkWidget* toolBar = gtk_toolbar_new();
      gtk_widget_show(toolBar);
      gtk_toolbar_set_orientation
        (GTK_TOOLBAR(toolBar),GTK_ORIENTATION_HORIZONTAL);
      gtk_toolbar_set_style(GTK_TOOLBAR(toolBar),GTK_TOOLBAR_ICONS);
      gtk_widget_set_name(toolBar,(char*)"OnX_gtk_toolBar");
      gtk_box_pack_start(GTK_BOX(box),menuBar,FALSE,TRUE,0);
      gtk_box_pack_start(GTK_BOX(box),toolBar,FALSE,TRUE,0);
      gtkWidget = gtk_vbox_new(FALSE, 0);
      gtk_widget_show(gtkWidget);
      gtk_box_pack_start(GTK_BOX(box),gtkWidget,TRUE,TRUE,0);
    } else if(compoundId==gtk_MENUBAR) {
      gtkWidget = findInHierarchy(gtkParentContainer,"OnX_gtk_menuBar");
      if(!gtkWidget) {
        std::ostream& out = fSession.cout();
        out << "GtkUI::createNativeWidget :"
            << " MenuBar not found in parent."
            << std::endl;
        return false;
      }
    } else if(compoundId==gtk_TOOLBAR) {
      gtkWidget = findInHierarchy(gtkParentContainer,"OnX_gtk_toolBar");
      if(!gtkWidget) {
        std::ostream& out = fSession.cout();
        out << "GtkUI::createNativeWidget :"
            << " ToolBar not found in parent."
            << std::endl;
        return false;
      }
    } else if(compoundId==gtk_MENU_SEPARATOR) {
      gtkContainer = gtk_menu_item_new();
      gtk_widget_show(gtkContainer);
      gtkWidget = gtk_hseparator_new();
      gtk_widget_show(gtkWidget);
      gtk_container_add(GTK_CONTAINER(gtkContainer),gtkWidget);
    } else if(compoundId==gtk_DIALOG) {
      gtkContainer = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_type_hint(GTK_WINDOW(gtkContainer),
                               GDK_WINDOW_TYPE_HINT_DIALOG);
      gtk_window_set_position(GTK_WINDOW(gtkContainer),
                              GTK_WIN_POS_CENTER_ON_PARENT);
      //gtk_widget_show(gtkContainer); //not done here.
      gtkWidget = gtk_vbox_new(FALSE,0);
      gtk_widget_show(gtkWidget);
      gtk_container_add(GTK_CONTAINER(gtkContainer),gtkWidget);
      //gtk_window_set_modal(GTK_WINDOW(gtkContainer),FALSE);
#if (GTK_MAJOR_VERSION<2) || (GTK_MINOR_VERSION<10) 
#else
      gtk_window_set_deletable(GTK_WINDOW(gtkContainer),FALSE);
#endif

     {fWidgets.push_back(gtkContainer);
      gtk_signal_connect(GTK_OBJECT(gtkContainer), 
                         "destroy",
                         GTK_SIGNAL_FUNC(destroyShellCallback),
                         this);}

     //gtkWidget = (GTK_DIALOG(gtkContainer))->vbox;

    } else if(compoundId==gtk_PASSWORD) {
      gtkWidget = gtk_entry_new();
      gtk_widget_show(gtkWidget);
      gtk_entry_set_visibility(GTK_ENTRY(gtkWidget),FALSE);
    } else if(compoundId==gtk_SCROLLED_TEXT) {
      
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      GtkWidget* scrolled_window = gtk_scrolled_window_new(0,0);
      gtk_widget_show(scrolled_window);
      gtk_container_add(GTK_CONTAINER(gtkContainer),scrolled_window);
      gtkWidget = gtk_text_view_new();
      gtk_widget_show(gtkWidget);
      gtk_container_add(GTK_CONTAINER(scrolled_window),gtkWidget);
      //gtk_text_set_editable(GTK_TEXT(fConsoleText),FALSE);
    } else if(compoundId==gtk_SCROLLED_LIST) {
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      GtkWidget* scrolled_window = gtk_scrolled_window_new(0,0);
      gtk_widget_show(scrolled_window);
      gtk_container_add(GTK_CONTAINER(gtkContainer),scrolled_window);
      gtkWidget = gtk_list_new();
      gtk_widget_show(gtkWidget);
      gtk_scrolled_window_add_with_viewport(
                                       GTK_SCROLLED_WINDOW(scrolled_window),
                                       gtkWidget);
    } else if(compoundId==gtk_SCROLLED_TREE) {
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      GtkWidget* scrolled_window = gtk_scrolled_window_new(0,0);
      gtk_widget_show(scrolled_window);
      gtkWidget = gtk_tree_view_new();
      gtk_widget_show(gtkWidget);
      gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkWidget)),GTK_SELECTION_SINGLE);
      gtk_container_add(GTK_CONTAINER(scrolled_window), gtkWidget);

      gtk_container_add(GTK_CONTAINER(gtkContainer),scrolled_window);

      gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                     GTK_POLICY_AUTOMATIC,
                                     GTK_POLICY_AUTOMATIC);
    } else if(compoundId==gtk_VCONTAINER) {
      gtkWidget = gtk_vbox_new(FALSE,0);
      gtk_widget_show(gtkWidget);
    } else if(compoundId==gtk_HCONTAINER) {
      gtkWidget = gtk_hbox_new(FALSE,0);
      gtk_widget_show(gtkWidget);
    } else if(compoundId==gtk_TOOLBAR_ITEM) {
      CHECK_PARENT

      // Assume that parent is a gtk_toolbar...

      aItemML.element_value("label",svalue);
      std::string shelp;
      aItemML.element_value("tooltip",shelp);
      std::string sfile;
      aItemML.element_value("pixmap",sfile);
      std::string fileName;
      inlib::file_name(sfile,fileName);

      GtkWidget* icon = 0;
      if((fileName.size()) && fTopWindow) {
        GtkPixmap* pixmap = (GtkPixmap*)gtk_type_new(gtk_pixmap_get_type());
        gtk_pixmap_set_build_insensitive(pixmap,TRUE);
        if(!gtk_pixmap_set_from_file(pixmap,fTopWindow,sfile)) {
          gtk_widget_destroy(GTK_WIDGET(pixmap));
        } else {
          icon = GTK_WIDGET(pixmap);
        }        
      }

      gtkWidget = gtk_toolbar_append_item(GTK_TOOLBAR(gtkParent),
                                          svalue.c_str(),
                                          shelp.c_str(),
                                          0,icon,0,0);
      gtk_widget_show(gtkWidget);
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"clicked");
#ifdef ONX_HAS_INVENTOR
    } else if(compoundId==gtk_SOEXAMINERVIEWER) {
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      std::string name = aName + "_viewer";

      SoGtkExaminerViewer* viewer = 
        new SoGtkExaminerViewer(gtkContainer,name.c_str(),TRUE);
      gtkWidget = viewer->getWidget(); //GtkVBox
      gtk_widget_show(gtkWidget);

      //{GtkType type = GTK_WIDGET_TYPE(gtkWidget);
      // printf("debug : exam widget \"%s\"\n",gtk_type_name(type));}

      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy
        (viewer,viewer_proxy<SoGtkFullViewer,SoGtkExaminerViewer>);

      //FIXME : who delete the viewer

    } else if(compoundId==gtk_SOPLANEVIEWER) {
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      std::string name = aName + "_viewer";
      SoGtkPlaneViewer* viewer = 
        new SoGtkPlaneViewer(gtkContainer,name.c_str(),TRUE);
      gtkWidget = viewer->getWidget();
      gtk_widget_show(gtkWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoGtkFullViewer>);
    } else if(compoundId==gtk_SOFLYVIEWER) {
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      std::string name = aName + "_viewer";
      SoGtkFlyViewer* viewer = 
        new SoGtkFlyViewer(gtkContainer,name.c_str(),TRUE);
      gtkWidget = viewer->getWidget();
      gtk_widget_show(gtkWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoGtkFullViewer>);
    } else if(compoundId==gtk_SOWALKVIEWER) {
      gtkContainer = gtk_vbox_new (FALSE, 10);
      gtk_widget_show(gtkContainer);
      gtk_container_set_border_width (GTK_CONTAINER(gtkContainer), 10);
      std::string name = aName + "_viewer";
      SoGtkWalkViewer* viewer = 
        new SoGtkWalkViewer(gtkContainer,name.c_str(),TRUE);
      gtkWidget = viewer->getWidget();
      gtk_widget_show(gtkWidget);
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoGtkFullViewer>);

    } else if( (compoundId==gtk_PAGEVIEWER) ||
               (compoundId==gtk_PLANEPAGEVIEWER) ){
      std::string vclass = "Examiner";
      if(compoundId==gtk_PLANEPAGEVIEWER) vclass = "Plane";
      gtkWidget = gtk_page_viewer_new(vclass.c_str());
      gtk_widget_show(gtkWidget);
      SoNode* soNode = 
        gtk_page_viewer_get_scene_graph(GTK_PAGE_VIEWER(gtkWidget));
      SoGtkFullViewer* viewer = 
        gtk_page_viewer_get_viewer(GTK_PAGE_VIEWER(gtkWidget));
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) {
        std::ostream& out = fSession.cout();
        out << "GtkUI::createNativeWidget : ItemML not a OnX::SoViewer."
            << std::endl;
        return false;
      }
      soViewer->setSceneGraph(soNode);
      if(viewer->isOfType(SoGtkExaminerViewer::getClassTypeId())) {
        soViewer->setViewerProxy
          (viewer,viewer_proxy<SoGtkFullViewer,SoGtkExaminerViewer>);
      } else {
        soViewer->setViewerProxy(viewer,viewer_proxy<SoGtkFullViewer>);
      }
#endif
    } else {
      // Generic case :
      gtkWidget = (GtkWidget*)gtk_type_new(gtkType);
      if(GTK_IS_WINDOW(gtkWidget)) {
        fWidgets.push_back(gtkWidget);
        gtk_signal_connect(GTK_OBJECT(gtkWidget), 
                           "destroy",
                           GTK_SIGNAL_FUNC(destroyShellCallback),
                           this);
      } else {
        gtk_widget_show(gtkWidget);        
      }
    }

    if(!gtkWidget) return false;
    if(!gtkContainer) gtkContainer = gtkWidget;

    gtk_signal_connect(GTK_OBJECT(gtkWidget), 
                       "destroy",
                       GTK_SIGNAL_FUNC(destroyCallback),
                       new Gtk::Destroy(*this,aItemML));
      
    aItemML.set_data(gtkWidget,gtkContainer,0);
    owidget->setProxy(gtkWidget,gtk_proxy);

    // Set the widget name and customize according 
    // to resources in the rc files. A specific widget
    // can be customized in a rc file by some :
    //    widget "*<widdget name>" style "<specific style>".
    // See scripts/gtk/OnX.gtkrc for examples.
    gtk_widget_set_name(gtkWidget,aName.c_str());
    
    // Put in parent :
    if((gtkType==gtk_file_selection_get_type())
       || (gtkType==gtk_color_selection_dialog_get_type()) 
       || (gtkType==gtk_warning_dialog_get_type())
       ){
      if(gtkParent) {
        GtkWidget* shell = gtk_widget_get_toplevel(gtkParent);
        if(shell) gtk_window_set_transient_for(GTK_WINDOW(gtkContainer),
                                               GTK_WINDOW(shell));
      }
    } else if(compoundId==gtk_WINDOW) {
    } else if(compoundId==gtk_DIALOG) {
    } else if(compoundId==gtk_MENUBAR) {
    } else if(compoundId==gtk_TOOLBAR) {
    } else if(parentCompoundId==gtk_VCONTAINER) {
      CHECK_PARENT
      gtk_container_add(GTK_CONTAINER(gtkParent),gtkContainer);
    } else if(parentCompoundId==gtk_HCONTAINER) {
      CHECK_PARENT
      gtk_container_add(GTK_CONTAINER(gtkParent),gtkContainer);
    } else if(parentCompoundId==gtk_WINDOW) {
      CHECK_PARENT
      gtk_box_pack_start (GTK_BOX(gtkParent), gtkContainer, TRUE, TRUE, 0);
    // if the gtkDIALOG is the parent, we have to pack in
    } else if(parentCompoundId==gtk_DIALOG) {
      CHECK_PARENT
      gtk_box_pack_start(GTK_BOX(gtkParent),gtkContainer,TRUE,TRUE,0);
    } else if(gtkParent) {
      GtkType gtkParentType = GTK_WIDGET_TYPE(gtkParent);
      if( (gtkParentType==gtk_vbox_get_type()) ||
          (gtkParentType==gtk_hbox_get_type()) ) {
        std::string sexpand;
        if(!aParent->element_value("expand",sexpand)) sexpand = "second";
        gtk_box_set_expand(GTK_BOX(gtkParent),sexpand,gtkContainer);
      } else if(gtkParentType==gtk_menu_bar_get_type()) {
        // Cascade button in a menu bar :
        gtk_menu_bar_append (GTK_MENU_BAR(gtkParent),gtkContainer);
      } else if(gtkParentType==gtk_menu_item_get_type()) {
        // Pulldown menu attached to a cascade button :
        gtk_menu_item_set_submenu (GTK_MENU_ITEM(gtkParent),gtkContainer);
      } else if(gtkParentType==gtk_menu_get_type()) {
        // Button in a menu or MenuSeparator :
        gtk_menu_append (GTK_MENU(gtkParent),gtkContainer);
      } else if(gtkParentType==gtk_notebook_get_type()) {
        if(!aItemML.element_value("tabLabel",svalue)) svalue = aName;
        gtk_notebook_append_page(GTK_NOTEBOOK(gtkParent),
                                 gtkContainer,
                                 gtk_label_new(svalue.c_str()));
      } else if(compoundId==gtk_TOOLBAR_ITEM) {
        // Done at creation.
      } else {
        // Generic case :
        gtk_container_add(GTK_CONTAINER(gtkParent),gtkContainer);
      }
    }
    
    // Set resources :
    if(compoundId==gtk_WINDOW) {
      if(aItemML.element_value("title",svalue)) {
        gtk_window_set_title(GTK_WINDOW(gtkContainer),svalue.c_str());
      }
      if(aItemML.element_value("geometry",svalue)) {
        int x,y,w,h;
        if(!to_geometry(svalue,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << "GtkUI::createNativeWidget :"
              << " " << svalue << " : bad value for geometry." << std::endl;
        } else {
          gtk_window_set_default_size(GTK_WINDOW(gtkContainer),w,h);
          gtk_window_move(GTK_WINDOW(gtkContainer),x,y);
        }
      }
      gtk_window_set_resizable(GTK_WINDOW(gtkContainer),TRUE);
      
    } else if(compoundId==gtk_DIALOG) {
      if(aItemML.element_value("title",svalue)) {
        gtk_window_set_title(GTK_WINDOW(gtkContainer),svalue.c_str());
      }
      if(aItemML.element_value("geometry",svalue)) {
        int w,h;
        if(sscanf(svalue.c_str(),"%dx%d",&w,&h)!=2) {
          std::ostream& out = fSession.cout();
          out << "GtkUI::createNativeWidget :"
              << " " << svalue << " : bad value for geometry." << std::endl;
        } else {
          gtk_window_set_default_size(GTK_WINDOW(gtkContainer),w,h);
        }
      }

    } else if(compoundId==gtk_SCROLLED_TEXT) {
      if(aItemML.element_value("value",svalue)) {
        bool as_it = false;
        std::string s;
        if(aItemML.element_atb_value("value","as_it",s)) {
          if(!inlib::to(s,as_it)) as_it = false;
        }
        if(!as_it) inlib::replace(svalue,"\\n","\n");
        OnX::gtk_text_view_set_value(GTK_TEXT_VIEW(gtkWidget),svalue);
      }
    } else if(compoundId==gtk_SCROLLED_LIST) {
      aItemML.element_value("selection",sdef);
      if(aItemML.element_value("items",svalue)) {
        gtk_list_clear_items(GTK_LIST(gtkWidget),0,-1);
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        unsigned int itemn = items.size();
        int found = NotFound;
        for(unsigned int index=0;index<itemn;index++){
          GtkWidget *item = 
            gtk_list_item_new_with_label((char*)items[index].c_str());
          gtk_container_add(GTK_CONTAINER(gtkWidget),item);
          if(items[index]==sdef) found = index;
        }
        if(found!=NotFound) gtk_list_select_item(GTK_LIST(gtkWidget),found);
      }
      addCallbacks(aItemML,
                   gtkWidget,"select",
                   GTK_OBJECT(gtkWidget),"selection_changed");
    } else if(compoundId==gtk_PASSWORD) {
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"activate");
    } else if(compoundId==gtk_SCROLLED_TREE) {
      addCallbacks(aItemML,
                   gtkWidget,
                   "select",
                   GTK_OBJECT(gtkWidget),
                   "row-activated",
                   (GtkSignalFunc)treeViewActivatedCallback);
      addCallbacks(aItemML,
                   gtkWidget,
                   "selectBranch",
                   GTK_OBJECT(gtkWidget),
                   "row-activated",
                   (GtkSignalFunc)treeViewActivatedCallback);
      addCallbacks(aItemML,
                   gtkWidget,
                   "close",
                   GTK_OBJECT(gtkWidget),
                   "row_collapsed",
                   (GtkSignalFunc)treeViewOpenCloseCallback);
      addCallbacks(aItemML,
                   gtkWidget,
                   "open",
                   GTK_OBJECT(gtkWidget),
                   "row-expanded",
                   (GtkSignalFunc)treeViewOpenCloseCallback);
    } else if( (gtkType==gtk_hpaned_get_type()) ||
               (gtkType==gtk_vpaned_get_type()) ) {
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          gtk_paned_set_position(GTK_PANED(gtkWidget),value);
        }      
      }
    } else if( (gtkType==gtk_check_menu_item_get_type()) ||
               (gtkType==gtk_menu_item_get_type()) ){

      if(aItemML.element_value("accelerator",svalue)) {
        if(fAccelGroup) {
          //Assumed svalue to be the Qt syntax (exa : Ctrl+a)
          //Ctrl+a -> <control>a
          inlib::replace(svalue,"Ctrl+","<control>");
          inlib::replace(svalue,"Shift+","<shift>");
          inlib::replace(svalue,"Alt+","<alternate>");
          guint key;
          GdkModifierType mods;
          gtk_accelerator_parse(svalue.c_str(),&key,&mods);
          if(key) {
            //printf("debug : accel \"%s\" %x %x\n",
            //  svalue.c_str(),key,mods);
            gtk_widget_add_accelerator(gtkWidget,"activate",fAccelGroup, 
              key,mods,GTK_ACCEL_VISIBLE); 
          }
        }
      }

      //FIXME : pass here too for cascade button...      
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"activate");
      //FIXME : should have cascading only on cascade menu items.
      addCallbacks(aItemML,
                   gtkWidget,"cascading",
                   GTK_OBJECT(gtkWidget),"activate");
    } else if(gtkType==gtk_toggle_button_get_type()) {
      if(aItemML.element_value("set",svalue)) {
        bool value;
        if(inlib::to(svalue,value)) {
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtkWidget),value?TRUE:FALSE);
        }
      }
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"clicked");
    } else if(gtkType==gtk_button_get_type()) {
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"clicked");
    } else if(gtkType==gtk_notebook_get_type()) {
      addCallbacks(aItemML,
                   gtkWidget,"select",
                   GTK_OBJECT(gtkWidget),"switch_page",
                   (GtkSignalFunc)notebookCallback);
    } else if(gtkType==gtk_file_selection_get_type()) {

      gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(gtkWidget));

      addCallbacks(aItemML,gtkWidget,"ok",
        GTK_OBJECT(GTK_FILE_SELECTION(gtkWidget)->ok_button),"clicked");
      addCallbacks(aItemML,gtkWidget,"cancel",
        GTK_OBJECT(GTK_FILE_SELECTION(gtkWidget)->cancel_button),"clicked");

      std::string directory;
      if(!aItemML.element_value("directory",directory)) directory = "";
      if(directory.size()) {
        directory += inlib::sep();
      }

      inlib::expand_vars(directory);
      gtk_file_selection_set_filename(GTK_FILE_SELECTION(gtkWidget),
                                      directory.c_str());

      //FIXME : how to do a selection ?
      if(aItemML.element_value("dirMask",svalue)) {
        inlib::expand_vars(svalue);
      }

    } else if(gtkType==gtk_entry_get_type()) {
      std::string svalue;
      if(aItemML.element_value("value",svalue)) {
        gtk_entry_set_text(GTK_ENTRY(gtkWidget),svalue.c_str());
      }
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"activate");
    } else if(gtkType==gtk_command_line_get_type()) {
      addCallbacks(aItemML,
                   gtkWidget,"activate",
                   GTK_OBJECT(gtkWidget),"activate");
      addCallbacks(aItemML,
                   gtkWidget,"complete",
                   GTK_OBJECT(gtkWidget),"complete");
    } else if( (gtkType==gtk_hscrollbar_get_type()) ||
               (gtkType==gtk_vscrollbar_get_type()) ||
               (gtkType==gtk_hscale_get_type()) ||
               (gtkType==gtk_vscale_get_type())  )  {
      GtkAdjustment* adjustment = 
        gtk_range_get_adjustment(GTK_RANGE(gtkWidget));
      addCallbacks(aItemML,
                   gtkWidget,"drag",
                   GTK_OBJECT(adjustment),"value_changed");
    } else if(gtkType==gtk_tree_view_get_type()) {
      gtk_tree_selection_set_mode (gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkWidget)),
                                   GTK_SELECTION_SINGLE);
    } else if(gtkType==gtk_option_menu_get_type()) {
      aItemML.element_value("default",sdef);
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        gtk_option_menu_initialize(GTK_OPTION_MENU(gtkWidget),items,sdef);
      }
      addCallbacks(aItemML,
                   gtkWidget,"valueChanged",
                   GTK_OBJECT(gtkWidget),"changed");
    } else if(gtkType==gtk_combo_get_type()) {
      if(aItemML.element_value("items",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        gtk_combo_set_items(GTK_COMBO(gtkWidget),items);
      }
      if(aItemML.element_value("default",svalue)) {
        gtk_combo_set_value(GTK_COMBO(gtkWidget),svalue);
      }

      //FIXME : a return in the entry popup the list.
      GtkEntry* entry = GTK_ENTRY(GTK_COMBO(gtkWidget)->entry);
      addCallbacks(aItemML,
                   gtkWidget,"valueChanged",
                   GTK_OBJECT(entry),"activate");

      //GtkList* list = GTK_LIST(GTK_COMBO(gtkWidget)->list);
      //addCallbacks(aItemML,
      //             gtkWidget,"valueChanged",
      //             GTK_OBJECT(list),"button_release_event",
      //             (GtkSignalFunc)listCallback);

    } else if(gtkType==gtk_color_selection_dialog_get_type()) {
      // ok should be labelled Apply
      addCallbacks(aItemML,gtkWidget,"ok",
        GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(gtkWidget)->ok_button),
        "clicked");
      gtk_signal_connect(
        GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(gtkWidget)->cancel_button),
        "clicked",
        (GtkSignalFunc)cancelColorDialogCallback,
        (gpointer)gtkWidget); 
    } else if(gtkType==gtk_toolbar_get_type()) {
      gtk_toolbar_set_orientation(GTK_TOOLBAR(gtkWidget),
                                  GTK_ORIENTATION_HORIZONTAL);
      //gtk_toolbar_set_style(GTK_TOOLBAR(gtkWidget),GTK_TOOLBAR_BOTH);
      gtk_toolbar_set_style(GTK_TOOLBAR(gtkWidget),GTK_TOOLBAR_ICONS);
#ifdef ONX_HAS_GTK_GL_AREA
    } else if(gtkType==gtk_gl_area_get_type()) {
      //REVISIT      
      inlib::xml::looper _for(aItemML);
      while(inlib::xml::element* _elem = _for.next_element()) {
        if("event"==_elem->name()) {
          std::string sexec;
          _elem->attribute_value("exec",sexec);
          Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
          Action* action = new Action(*this,widget,gtkWidget,
                                     sexec,
                                     _elem->value(),
                                     _elem->attributes(),
                                     "event",0);
          addAction(action);
          gtk_signal_connect(GTK_OBJECT(gtkWidget),
                             "button_press_event",
                             (GtkSignalFunc)eventCallback,
                             (gpointer)action); 
          gtk_signal_connect(GTK_OBJECT(gtkWidget),
                             "button_release_event",
                             (GtkSignalFunc)eventCallback,
                             (gpointer)action); 
          gtk_signal_connect(GTK_OBJECT(gtkWidget),
                             "motion_notify_event",
                             (GtkSignalFunc)eventCallback,
                             (gpointer)action); 
        }
      }
      addCallbacks(aItemML,
                   gtkWidget,"paint",
                   GTK_OBJECT(gtkWidget),"expose_event",
                   (GtkSignalFunc)exposeCallback);
      gtk_widget_add_events(gtkWidget,
        GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_MOTION_MASK);
#endif
#ifdef ONX_HAS_INVENTOR
    } else if( (compoundId==gtk_PAGEVIEWER) ||
               (compoundId==gtk_PLANEPAGEVIEWER) ){

      if(aItemML.element_value("popupItems",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++)
          gtk_page_viewer_add_popup_entry(GTK_PAGE_VIEWER(gtkWidget),
                                          items[index]);
      }

      addCallbacks(aItemML,
                   gtkWidget,"rectangularCollect",
                   GTK_OBJECT(gtkWidget),"rectangular_collect",
                   (GtkSignalFunc)pageCallback);
      addCallbacks(aItemML,
                   gtkWidget,"collect",
                   GTK_OBJECT(gtkWidget),"collect",
                   (GtkSignalFunc)pageCallback);
      addCallbacks(aItemML,
                   gtkWidget,"popup",
                   GTK_OBJECT(gtkWidget),"popup",
                   (GtkSignalFunc)pageCallback);
      addCallbacks(aItemML,gtkWidget,"update",0,"",0);
#endif
    } 
        
    if(compoundId==gtk_WINDOW) {
      
      if(aItemML.element_value("decoration",svalue)) {

        //::printf("debug : decoration \"%s\"\n",svalue.c_str());

        // default values for decorations are in BaseUI.cxx
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
          //::printf("debug :   parse : \"%s\" %d\n",
          //    (*it).second.c_str(),(*it).first);
          if ((*it).second == "close_button") close_button = (*it).first; 
          if ((*it).second == "iconify_button") iconify_button = (*it).first; 
          if ((*it).second == "min_max_button") min_max_button = (*it).first; 
          if ((*it).second == "resize_handle") resize_handle = (*it).first; 
          if ((*it).second == "border") border = (*it).first;
          if ((*it).second == "caption") caption = (*it).first; 
          if ((*it).second == "system_menu") system_menu = (*it).first;
        }
        std::ostream& out = fSession.cout();
        std::string msg = "GtkUI::createNativeWidget : WARNING : ";
        
        if(system_menu) {
          //FIXME out << msg 
          //        <<"This toolkit haven't got any system menu" << std::endl;
        }
      
        //gtk_window_set_decorated(GTK_WINDOW(gtkContainer),TRUE);
        if (!iconify_button) {
          out << msg  
              <<"To hide iconify button, you should try no_caption option" 
              << std::endl;
        }
#if (GTK_MAJOR_VERSION<2) || (GTK_MINOR_VERSION<10) 
#else
        gtk_window_set_deletable
          (GTK_WINDOW(gtkContainer),close_button?TRUE:FALSE);
#endif
        gtk_window_set_resizable
          (GTK_WINDOW(gtkContainer),resize_handle?TRUE:FALSE);

      }
    }

    if(aItemML.element_value("background",svalue)) {
      if(GTK_IS_MENU_ITEM(gtkWidget)) {
        gtk_menu_item_set_background(GTK_MENU_ITEM(gtkWidget),svalue);
      } else if (GTK_IS_BUTTON(gtkWidget)) {
        gtk_button_set_background(GTK_BUTTON(gtkWidget),svalue);
      } else if (GTK_IS_TREE_VIEW(gtkWidget)) {
        gtk_widget_set_background2(gtkWidget,svalue);
      } else {
        gtk_widget_set_background(gtkWidget,svalue);
      }
    }

    if(aItemML.element_value("foreground",svalue)) {
      if(GTK_IS_LABEL(gtkWidget)) {
        gtk_widget_set_foreground(gtk_widget_get_parent(gtkWidget),svalue);
      } else if(GTK_IS_MENU_ITEM(gtkWidget)) {
        gtk_menu_item_set_foreground(GTK_MENU_ITEM(gtkWidget),svalue);
      } else if(GTK_IS_BUTTON(gtkWidget)) {
        gtk_button_set_foreground(GTK_BUTTON(gtkWidget),svalue);
      } else {
        gtk_widget_set_foreground(gtkWidget,svalue);
      }
    }

    bool value;
    if(aItemML.element_value_boolean("grabFocus",value)) {
      gtk_widget_grab_focus(gtkWidget);
    }  

    return createChildren(aItemML);
}

bool OnX::GtkUI::destroyNativeWidget(inlib::xml::tree& aItemML){
  GtkWidget* widget = (GtkWidget*)aItemML.get_data2();
  if(!widget) return false;
  if(fVerboseLevel) {
    std::string name = gtk_widget_get_name(widget);
    std::ostream& out = fSession.cout();
    out << "GtkUI::destroyNativeWidget :"
        << " destroy Gtk widget \""
        << name << "\"." << std::endl;
  }
  if(GTK_IS_WINDOW(widget)) fWidgets.remove(widget);
  gtk_widget_destroy(widget);
  removeActions(widget);
  removeInRelations(aItemML);
  aItemML.invalidate();
  return true;
}

std::string OnX::GtkUI::getCallbackName(GtkType aType,const std::string& aName){
  if (aName == "valueChanged") {
    if (aType == gtk_option_menu_get_type()){
      return "changed";
    }
  } else if (aName == "activate") {
    if (aType == gtk_button_get_type()){
      return "clicked";
    } else if ((aType == gtk_entry_get_type()) ||
               (aType == gtk_menu_item_get_type()) ||
               (aType == gtk_command_line_get_type())) {
      return "activate";
    }
  } else if (aName == "select") {
    if ((aType == gtk_list_get_type()) ||
        (aType == gtk_hscrollbar_get_type()) ||
        (aType == gtk_vscrollbar_get_type()) ||
        (aType == gtk_hscale_get_type()) ||
        (aType == gtk_vscale_get_type())){
      return "selection_changed";
    } else if (aType == gtk_tree_view_get_type()){
      return "row-activated";
    } else if (aType == gtk_notebook_get_type()) {
      return "switch_page";
    } 
  } else if (aName == "paint")    {
#ifdef ONX_HAS_GTK_GL_AREA
    if(aType == gtk_gl_area_get_type()) {
      return "expose_event";
    }
#endif
  }
  else if (aName == "drag")     return "value_changed";
  else if (aName == "complete") return "complete";
  else if (aName == "collect")  return "collect";
  else if (aName == "popup")    return "popup";
  else if (aName == "ok")       return "clicked";
  else if (aName == "cancel")   return "clicked";
  else if (aName == "open")     return "row-expanded";
  else if (aName == "close")    return "row-collapsed";
  else if (aName == "selectBranch") return "row-activated";

  return "";
}

void OnX::GtkUI::deleteCallback(
 inlib::xml::tree& aItemML
,const std::string& aNameXML
,OnX::Action* aAction
){
  GtkWidget* widget = (GtkWidget*)aItemML.get_data1();
  if(!widget) return;
  GtkType type = GTK_OBJECT_TYPE(widget);

  // function callback
  GtkSignalFunc callbackFunction = (GtkSignalFunc)callback;
  if (type == gtk_notebook_get_type()) {
    callbackFunction = (GtkSignalFunc)notebookCallback;
  } else if (type == gtk_tree_view_get_type()) {
    if ((aNameXML == "select") ||
        (aNameXML == "selectBranch")) {
      callbackFunction = (GtkSignalFunc)treeViewActivatedCallback;
    } else if ((aNameXML == "open") ||
               (aNameXML == "close")) {
      callbackFunction = (GtkSignalFunc)treeViewOpenCloseCallback;
    }
  }
#ifdef ONX_HAS_GTK_GL_AREA
  else if(type == gtk_gl_area_get_type()) {
    if (aNameXML == "paint") {
      callbackFunction = (GtkSignalFunc)exposeCallback;
    } else {
      callbackFunction = (GtkSignalFunc)eventCallback;
    }
  }
#else
  else if(type == gtk_drawing_area_get_type()) {
    callbackFunction = (GtkSignalFunc)pageCallback;
  }
#endif

  // specials widget cases
  if (type == gtk_file_selection_get_type()) {
    if (aNameXML == "ok") {
      GtkObject* obj = GTK_OBJECT(GTK_FILE_SELECTION(widget)->ok_button);
      gtk_signal_disconnect_by_data(obj,(gpointer)aAction);
    } else if (aNameXML == "cancel") {
      GtkObject* obj = 
        GTK_OBJECT(GTK_FILE_SELECTION(widget)->cancel_button);
      gtk_signal_disconnect_by_data(obj,(gpointer)aAction);
    }
  } else if ((type == gtk_hscrollbar_get_type()) ||
             (type == gtk_vscrollbar_get_type()) ||
             (type == gtk_hscale_get_type()) ||
             (type == gtk_vscale_get_type())) {
    GtkAdjustment* adjustment = gtk_range_get_adjustment(GTK_RANGE(widget));
    GtkObject* obj = GTK_OBJECT(adjustment);
    gtk_signal_disconnect_by_data(obj,(gpointer)aAction);
  }
  else if (type == gtk_color_selection_dialog_get_type()) {
    GtkObject* obj = 
      GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(widget)->ok_button);
    gtk_signal_disconnect_by_data(obj,(gpointer)aAction);
  } else if ((aNameXML == "valueChanged") && (type == gtk_combo_get_type())) {

    GtkEntry* entry = GTK_ENTRY(GTK_COMBO(widget)->entry);
    gtk_signal_disconnect_by_data(GTK_OBJECT(entry),(gpointer)aAction);

    //GtkList* list = GTK_LIST(GTK_COMBO(widget)->list);
    //gtk_signal_disconnect_by_data(GTK_OBJECT(list),(gpointer)aAction);    

  } else { //Generic case :
    gtk_signal_disconnect_by_data(GTK_OBJECT(widget),(gpointer)aAction);
  }

}

OnX::Action* OnX::GtkUI::addCallback(
 inlib::xml::tree& aItemML
,const std::string& aNameXML
,const std::string& aInterp
,const std::string& aScript
){
  GtkWidget* gtkWidget = (GtkWidget*)aItemML.get_data1();
  if(!gtkWidget) return 0;
  GtkType type = GTK_OBJECT_TYPE(gtkWidget);
  GtkObject* objectGUI = GTK_OBJECT(gtkWidget);

  // function callback
  GtkSignalFunc callbackFunction = (GtkSignalFunc)callback;
  if (type == gtk_notebook_get_type()) {
    callbackFunction = (GtkSignalFunc)notebookCallback;
  } else if (type == gtk_tree_view_get_type()) {
    if ((aNameXML == "select") ||
        (aNameXML == "selectBranch")) {
      callbackFunction = (GtkSignalFunc)treeViewActivatedCallback;
    } else if ((aNameXML == "open") ||
               (aNameXML == "close")) {
      callbackFunction = (GtkSignalFunc)treeViewOpenCloseCallback;
    }
  }
#ifdef ONX_HAS_GTK_GL_AREA
  else if(type == gtk_gl_area_get_type()) {
    if (aNameXML == "paint") {
      callbackFunction = (GtkSignalFunc)exposeCallback;
    } else if(aNameXML == "event") {
      Slash::UI::IWidget* iwidget = INLIB_CAST(aItemML,Slash::UI::IWidget);
      Action::Parameters params;
      Action* action = 
        new Action(*this,iwidget,gtkWidget,aInterp,aScript,params,"event",0);
      addAction(action);
      gtk_signal_connect(objectGUI,
                         "button_press_event",
                         (GtkSignalFunc)eventCallback,
                         (gpointer)action);
      gtk_signal_connect(objectGUI,
                         "button_release_event",
                         (GtkSignalFunc)eventCallback,
                         (gpointer)action);
      gtk_signal_connect(objectGUI,
                         "motion_notify_event",
                         (GtkSignalFunc)eventCallback,
                         (gpointer)action);
      return action;
    }
  }
#else
  else if(type == gtk_drawing_area_get_type()) {
    callbackFunction = (GtkSignalFunc)pageCallback;
  }
#endif
  else if (type == gtk_file_selection_get_type()) {
    if (aNameXML == "ok") {
      objectGUI = 
        GTK_OBJECT(GTK_FILE_SELECTION(gtkWidget)->ok_button);
    } else if (aNameXML == "cancel") {
      objectGUI = 
        GTK_OBJECT(GTK_FILE_SELECTION(gtkWidget)->cancel_button);
    } else {
      return 0;
    }
  } else if ((type == gtk_hscrollbar_get_type()) ||
             (type == gtk_vscrollbar_get_type()) ||
             (type == gtk_hscale_get_type()) ||
             (type == gtk_vscale_get_type())) {
    GtkAdjustment* adjustment = 
      gtk_range_get_adjustment(GTK_RANGE(gtkWidget));
    objectGUI = GTK_OBJECT(adjustment);
  }  else if (type == gtk_color_selection_dialog_get_type()) {
    objectGUI = 
      GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(gtkWidget)->ok_button);
  }  else if(type == gtk_combo_get_type()) {
    if(aNameXML=="valueChanged") {
      Slash::UI::IWidget* iwidget = INLIB_CAST(aItemML,Slash::UI::IWidget);
      Action::Parameters params;
      Action* action = 
        new Action(*this,iwidget,gtkWidget,aInterp,aScript,params,
                   "valueChanged",0);
      addAction(action);
      objectGUI = GTK_OBJECT(GTK_ENTRY(GTK_COMBO(gtkWidget)->entry));
      gtk_signal_connect
        (objectGUI,"activate",callbackFunction,(gpointer)action);    
      //objectGUI = GTK_OBJECT(GTK_LIST(GTK_COMBO(gtkWidget)->list));
      //gtk_signal_connect
      // (objectGUI,
      // "button_release_event",(GtkSignalFunc)listCallback,(gpointer)action);
      return action;
    }
  }

  // Generic case :
  std::string gtkCbkName = getCallbackName(type,aNameXML);
  if(gtkCbkName.empty()) return 0;

  Slash::UI::IWidget* iwidget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  Action::Parameters params;
  Action* action = 
    new Action(*this,iwidget,gtkWidget,aInterp,aScript,params,aNameXML,0);
  addAction(action);
  gtk_signal_connect
    (objectGUI,gtkCbkName.c_str(),callbackFunction,(gpointer)action);

  return action;
}

void OnX::GtkUI::addCallbacks(
 inlib::xml::tree& aItemML
,GtkWidget* aWidget
,const std::string& aNameXML
,GtkObject* aObjectGUI
,const std::string& aNameGUI
,GtkSignalFunc aCallback
){
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  if(!widget) return;

  inlib::xml::looper _for(aItemML);
  while(inlib::xml::element* _elem = _for.next_element()) {
    if(aNameXML==_elem->name()) {
      std::string sexec;
      _elem->attribute_value("exec",sexec);
      Action* action = new Action(*this,widget,aWidget,sexec,
                                  _elem->value(),
                                  _elem->attributes(),
                                  aNameXML,0);
      addAction(action);
      if(aNameGUI.size()) {
        gtk_signal_connect(aObjectGUI, 
                           aNameGUI.c_str(),
                           aCallback, 
                           (gpointer)action);
      }
    }
  }
}

void OnX::GtkUI::destroyCallback(GtkObject* aObject,gpointer aTag){
#ifdef DEBUG_DESTROY
  std::string name = gtk_widget_get_name(GTK_WIDGET(aObject));
  ::printf("debug : OnX::GtkUI::destroyCallback : %s\n",name.c_str());
#endif
  OnX::Gtk::Destroy* d = (OnX::Gtk::Destroy*)aTag; 
  if(d->isValid()) {
    d->fUI.removeInRelations(d->fItemML);
    d->fItemML.invalidate();
  }
  //d->fUI.removeActions(GTK_WIDGET(aObject));
  delete d;
}

void OnX::GtkUI::closeCallback(GtkObject* aObject,GdkEventAny*,gpointer aTag){
#ifdef DEBUG_DESTROY
  std::string name = gtk_widget_get_name(GTK_WIDGET(aObject));
  ::printf("debug : OnX::GtkUI::closeCallback : %s\n",name.c_str());
#endif
  gtk_widget_hide(GTK_WIDGET(aObject));
  OnX::Gtk::Destroy* d = (OnX::Gtk::Destroy*)aTag; 
  if(d->isValid()) { 
#ifdef DEBUG_DESTROY
    std::string name = gtk_widget_get_name(GTK_WIDGET(aObject));
    ::printf("debug : OnX::GtkUI::closeCallback : exec destrpy cbks\n");
#endif
    Execute tag(d->fUI,"destroy");
    d->fItemML.post_execute_backward(executeItemMLCallbacks,&tag);

    inlib::xml::tree* item = &(d->fItemML);
    inlib::xml::tree* parent = item->parent();
    if(parent) parent->remove_child(item,true);
  }
  //delete d; //done through fDestroys.
}

void OnX::GtkUI::destroyShellCallback(GtkObject* aObject,gpointer aTag){
#ifdef DEBUG_DESTROY
  std::string name = gtk_widget_get_name(GTK_WIDGET(aObject));
  ::printf("debug : OnX::GtkUI::destroyShellCallback : %s\n",name.c_str());
#endif
  GtkUI* This = (GtkUI*)aTag; 
  This->fWidgets.remove(GTK_WIDGET(aObject));
}

void OnX::GtkUI::destroyConsoleTextCallback(GtkObject* aObject,gpointer aTag){
  GtkUI* This = (GtkUI*)aTag; 
  This->fConsoleText = 0;
}

void OnX::GtkUI::destroyWarningDialogCallback(GtkObject* aObject,gpointer aTag){
  GtkUI* This = (GtkUI*)aTag; 
  This->fWarningDialog = 0;
}

gboolean OnX::GtkUI::avoidDeleteCallback(GtkObject* /*aObject*/,gpointer /*aTag*/){return TRUE;}

int OnX::GtkUI::getCompound(const std::string& aName){
       if(aName=="Window")        return gtk_WINDOW;
  else if(aName=="MenuBar")       return gtk_MENUBAR;
  else if(aName=="ToolBar")       return gtk_TOOLBAR;
  else if(aName=="ScrolledText")  return gtk_SCROLLED_TEXT;
  else if(aName=="ScrolledList")  return gtk_SCROLLED_LIST;
  else if(aName=="ScrolledTree")  return gtk_SCROLLED_TREE;
  else if(aName=="Dialog")        return gtk_DIALOG;
  else if(aName=="ToolBarItem")   return gtk_TOOLBAR_ITEM;
  else if(aName=="MenuSeparator") return gtk_MENU_SEPARATOR;
  else if(aName=="Password")      return gtk_PASSWORD;
  else if(aName=="VContainer")    return gtk_VCONTAINER;
  else if(aName=="HContainer")    return gtk_HCONTAINER;

#ifdef ONX_HAS_INVENTOR
  else if(aName=="SoExaminerViewer") return gtk_SOEXAMINERVIEWER;
  else if(aName=="SoPlaneViewer")    return gtk_SOPLANEVIEWER;
  else if(aName=="SoFlyViewer")      return gtk_SOFLYVIEWER;
  else if(aName=="SoWalkViewer")     return gtk_SOWALKVIEWER;
  else if(aName=="PageViewer")       return gtk_PAGEVIEWER;
  else if(aName=="PlanePageViewer")  return gtk_PLANEPAGEVIEWER;
#endif
  return 0;
}

GtkType OnX::GtkUI::getType(const std::string& aName){
       if(aName=="VBox")           return gtk_vbox_get_type();
  else if(aName=="HBox")           return gtk_hbox_get_type();
  else if(aName=="VPaned")         return gtk_vpaned_get_type();
  else if(aName=="HPaned")         return gtk_hpaned_get_type();
  else if(aName=="MenuItem")       return gtk_menu_item_get_type();
  else if(aName=="Menu")           return gtk_menu_get_type();
  else if(aName=="CommandLine")    return gtk_command_line_get_type();
  else if(aName=="Label")          return gtk_label_get_type();
  else if(aName=="PushButton")     return gtk_button_get_type();
  else if(aName=="ToggleButton")   return gtk_toggle_button_get_type();
  else if(aName=="FileSelection")  return gtk_file_selection_get_type();
  else if(aName=="Entry")          return gtk_entry_get_type();
  else if(aName=="OptionMenu")     return gtk_option_menu_get_type();
  else if(aName=="ComboBox")       return gtk_combo_get_type();
  else if(aName=="HScrollBar")     return gtk_hscrollbar_get_type();
  else if(aName=="VScrollBar")     return gtk_vscrollbar_get_type();
  else if(aName=="HScale")         return gtk_hscale_get_type();
  else if(aName=="VScale")         return gtk_vscale_get_type();
  else if(aName=="ProgressBar")    return gtk_progress_bar_get_type();

  else if(aName=="TabStack")       return gtk_notebook_get_type();
  else if(aName=="ToggleMenuItem") return gtk_check_menu_item_get_type();
  else if(aName=="CascadeButton")  return gtk_menu_item_get_type();
  else if(aName=="ColorSelection") return gtk_color_selection_dialog_get_type();
  else if(aName=="WarningDialog")  return gtk_warning_dialog_get_type();
#ifdef ONX_HAS_GTK_GL_AREA
  else if(aName=="OpenGLArea")     return gtk_gl_area_get_type();
#else
  else if(aName=="OpenGLArea")     return gtk_drawing_area_get_type();
#endif

  else return GTK_TYPE_INVALID;
}

void OnX::GtkUI::callback(GtkObject* aObject,gpointer aTag){
  Action* action =(Action*)aTag;
  if(!action) return;

  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  std::string svalue;
  

  GtkWidget* widget = GTK_WIDGET(action->nativeWidget());
  //WARNING : take care of inheritance !

  if(GTK_IS_COMMAND_LINE(widget)) {
    gchar* cmd = gtk_command_line_get_value(GTK_COMMAND_LINE(aObject));
    svalue = cmd ? cmd : "";
  } else if(GTK_IS_ENTRY(widget)) {
    svalue = gtk_entry_get_text(GTK_ENTRY(aObject));
  } else if(GTK_IS_FILE_SELECTION(widget)) {
    svalue = 
      gtk_file_selection_get_filename(GTK_FILE_SELECTION(widget));
  } else if(GTK_IS_COLOR_SELECTION_DIALOG(widget)) {
    gdouble rgba[4];
    gtk_color_selection_get_color(
      GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(widget)->colorsel),rgba);
    std::string s;
    inlib::sprintf(s,3 * 16,"%g %g %g",rgba[0],rgba[1],rgba[2]);
    svalue = s;
  } else if(GTK_IS_LIST(widget)) {
    svalue  = "";
    GList* dlist = GTK_LIST(aObject)->selection;
    while(dlist) {
      GtkWidget* item = GTK_WIDGET(dlist->data);
      GtkLabel* label = GTK_LABEL(GTK_BIN(item)->child);
      gchar* s = 0;
      gtk_label_get(label,&s);
      svalue += s?s:"";
      dlist=dlist->next;
      if(dlist) svalue += "\n";
    }
  // for GTK 2.2, we have a treeViewActivatedCallback
  } else if(GTK_IS_RANGE(widget)) {
    float value = gtk_range_get_adjustment(GTK_RANGE(widget))->value;
    inlib::sprintf(svalue,64,"%d",(int)value);
  } else if(GTK_IS_CHECK_MENU_ITEM(widget)) {
    svalue = GTK_CHECK_MENU_ITEM(aObject)->active ? "true" : "false";
  } else if(GTK_IS_OPTION_MENU(widget)) {
    gtk_option_menu_get_value(GTK_OPTION_MENU(widget),svalue);
  } else if(GTK_IS_COMBO(widget)) {
    gtk_combo_get_value(GTK_COMBO(widget),svalue);
  }

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::GtkUI::callback :"
        << " value \"" << svalue << "\"."        << std::endl;
  }

  This->fCallbackData = 
    CallbackData(action->widget(),gtk_widget_get_name(widget),svalue,"",0,0);
  
  // Execute script :
  OnX::Action tmp(*action);
  // the action may delete widget that may do a ui.removeActions
  // that may then deleta action ! We then execute the script
  // with a local copy of the action.
  tmp.execute();

  This->fCallbackData = CallbackData();
}
void OnX::GtkUI::treeViewActivatedCallback(
 GtkTreeView* aView
,GtkTreePath*
,GtkTreeViewColumn*
,gpointer aTag
){
  // select and selectBranch callbacks
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  GtkWidget* widget = GTK_WIDGET(action->nativeWidget());

  if(GTK_IS_TREE_VIEW(widget)) {
    GtkTreeSelection* dselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(aView));
    if (dselection) {
      GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(aView)); 
      if(!model) return;
      GtkTreeIter child;
      if (gtk_tree_selection_get_selected(dselection, &model, &child)) {
        if (gtk_tree_model_iter_has_child(model,&child)) {
          if (action->name() == "select") return;
        } else {
          if (action->name() != "select") return;
        }
        std::string svalue = gtk_tree_selection_get_path(dselection);
        This->fCallbackData = 
          CallbackData(action->widget(),
                       gtk_widget_get_name(widget),svalue,"",0,0);
        action->execute();
        This->fCallbackData = CallbackData();
      }
    }
  }
}
void OnX::GtkUI::treeViewOpenCloseCallback(
 GtkTreeView* aView
,GtkTreeIter* aIter
,GtkTreePath*
,gpointer aTag
){
  // open and close callbacks
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  GtkWidget* widget = GTK_WIDGET(action->nativeWidget());

  if(GTK_IS_TREE_VIEW(widget)) {
    GtkTreeSelection* dselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(aView));
    if (dselection) {
      GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(aView)); 
      if(!model) return;
      GtkTreeIter child;
      if (!gtk_tree_model_iter_has_child(model,aIter)) {
        return;
      }
      if (gtk_tree_selection_get_selected(dselection, &model, &child)) {
          std::string svalue = gtk_tree_selection_get_path(dselection);
          This->fCallbackData = 
            CallbackData(action->widget(),
                         gtk_widget_get_name(widget),svalue,"",0,0);
          action->execute();
          This->fCallbackData = CallbackData();
      }
    }
  }
}

void OnX::GtkUI::pageCallback(
 GtkObject*
,GtkCallbackStruct* aData
,gpointer aTag
){
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  std::string svalue = aData->value;
  int cbkx = aData->x;
  int cbky = aData->y;

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::GtkUI::pageCallback :"
        << " value \"" << svalue << "\"."        << std::endl;
  }

  This->fCallbackData = 
    CallbackData(action->widget(),
                 gtk_widget_get_name(GTK_WIDGET(action->nativeWidget())),
                 svalue,"",cbkx,cbky);
  
  // Execute script :
  action->execute();

  This->fCallbackData = CallbackData();
}
void OnX::GtkUI::notebookCallback(
 GtkObject*
,GtkNotebookPage* aPage
,guint aPageNum
,gpointer aTag
){
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  // The notebookCallback is called also when children are destroyed.
  // If quiting and action is background, we do not send the 
  // execution message (else we have a bad memory balance).
  if(This->beingDestroyed() && action->isBackground()) return;

  GtkWidget* gtkWidget = GTK_WIDGET(action->nativeWidget());

  std::string svalue;
 {GtkWidget* child = 
    gtk_notebook_get_nth_page(GTK_NOTEBOOK(gtkWidget),aPageNum);
  const gchar* s = 
    gtk_notebook_get_tab_label_text(GTK_NOTEBOOK(gtkWidget),child);
  svalue = s?s:"";}

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::GtkUI::notebookCallback :"
        << " value \"" << svalue << "\"."        << std::endl;
  }

  This->fCallbackData = 
    CallbackData(action->widget(),
                 gtk_widget_get_name(gtkWidget),svalue,"",0,0);
  
  // Execute script :
  action->execute();

  This->fCallbackData = CallbackData();
}
/*
void OnX::GtkUI::listCallback(
 GtkObject*
,GdkEvent*
,gpointer aTag
){
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  std::string svalue;
  
  GtkWidget* widget = GTK_WIDGET(action->nativeWidget());

  if(GTK_IS_COMBO(widget)) {
    gtk_combo_get_value(GTK_COMBO(widget),svalue);
  }

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::GtkUI::listCallback :"
        << " value \"" << svalue << "\"."        << std::endl;
  }


  This->fCallbackData = 
    CallbackData(action->widget(),gtk_widget_get_name(widget),svalue,"",0,0);
  
  // Execute script :
  action->execute();

  This->fCallbackData = CallbackData();
}
*/
void OnX::GtkUI::exposeCallback(GtkObject*,GdkEventExpose*,gpointer aTag){
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  std::string svalue;

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::GtkUI::exposeCallback :"
        << " value \"" << svalue << "\"."        << std::endl;
  }

  GtkWidget* widget = GTK_WIDGET(action->nativeWidget());

  This->fCallbackData = 
    CallbackData(action->widget(),gtk_widget_get_name(widget),svalue,"",0,0);
  
#ifdef ONX_HAS_GTK_GL_AREA
  if(gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
    action->execute();
    gtk_gl_area_swap_buffers(GTK_GL_AREA(widget));
  }
#endif

  This->fCallbackData = CallbackData();

}
void OnX::GtkUI::eventCallback(GtkObject*,GdkEvent* aEvent,gpointer aTag){
  Action* action =(Action*)aTag;
  if(!action) return;
  GtkUI* This = INLIB_CAST(action->ui(),OnX::GtkUI);
  if(!This) return;

  std::string svalue;
  std::string sevent;
  int cbkx = 0;
  int cbky = 0;

  if(aEvent->type==GDK_BUTTON_PRESS) {
    sevent = "ButtonPress";
    cbkx = (int)((GdkEventButton*)aEvent)->x;
    cbky = (int)((GdkEventButton*)aEvent)->y;
  } else if(aEvent->type==GDK_BUTTON_RELEASE) {
    sevent = "ButtonRelease";
    cbkx = (int)((GdkEventButton*)aEvent)->x;
    cbky = (int)((GdkEventButton*)aEvent)->y;
  } else if(aEvent->type==GDK_MOTION_NOTIFY) {
    sevent = "MotionNotify";
    cbkx = (int)((GdkEventMotion*)aEvent)->x;
    cbky = (int)((GdkEventMotion*)aEvent)->y;
  }

  if(This->session().verboseLevel()) {
    std::ostream& out = This->session().out();
    out << "OnX::GtkUI::eventCallback :"
        << " event " << sevent 
        << " x " << cbkx
        << " y " << cbky
        << "."        << std::endl;
  }

  GtkWidget* widget = GTK_WIDGET(action->nativeWidget());

  This->fCallbackData =  
    CallbackData(action->widget(),
                 gtk_widget_get_name(widget),svalue,sevent,cbkx,cbky);
  
  // Execute script :
  action->execute();

  This->fCallbackData = CallbackData();

}

void OnX::GtkUI::cancelColorDialogCallback(GtkObject*,gpointer aTag){
  GtkWidget* colorDialog = (GtkWidget*)aTag;
  gtk_widget_hide(colorDialog);
}

GtkWidget* findInHierarchy(GtkWidget* aWidget,const std::string& aName){
  if(!aWidget) return 0;
  const gchar* s = gtk_widget_get_name(aWidget);
  if(s && (aName==s)) return aWidget;
  // Look children :
  if(!GTK_IS_CONTAINER(aWidget)) return 0;
  GList* dlist = gtk_container_children(GTK_CONTAINER(aWidget));
  if(!dlist) return 0;
  GList* item = dlist;
  while(item) {
    GtkWidget* w = findInHierarchy((GtkWidget*)(item->data),aName);
    if(w) return w;
    item = item->next;
  }
  g_list_free(dlist);
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Slash::UI::ICyclic* OnX::GtkUI::createCyclic(
 const std::string& aName
,int aDelay
,const std::string& aInterp
,const std::string& aScript
){
  Slash::UI::ICyclic* cyclic = new GtkCyclic(fSession,aName,aDelay,aInterp,aScript);
  fCyclics.push_back(cyclic);
  return cyclic;
}

std::string OnX::GtkUI::widgetName(inlib::xml::tree& aItemML){
  GtkWidget* widget = (GtkWidget*)aItemML.get_data1();
  if(!widget) return "";
  const gchar* cname = gtk_widget_get_name(GTK_WIDGET(widget));
  return std::string(cname?cname:"");
}

std::string OnX::GtkUI::getTreeSelection(GtkWidget* aWidget){
  GtkTreeSelection* dselection = gtk_tree_view_get_selection(GTK_TREE_VIEW(aWidget));
  return gtk_tree_selection_get_path(dselection);
}

void OnX::GtkUI::setTreeSelection(GtkWidget* aWidget,const std::string& aValue){
  std::vector<std::string> items;
  smanip_lines(aValue,items);
  // Try to open all the items selected 
  // if SELECTION_TYPE is single, we expand all 
  // items and select the last.
  for(unsigned int index=0;index<items.size();index++) {
    gtk_tree_view_expand_item(GTK_TREE_VIEW(aWidget),items[index]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <OnX/Core/Term.h>

namespace OnX {
class TermCyclic 
:public GtkCyclic
,public Term {
public: //Slash::UI::ICyclic
  virtual void execute() { cyclicExecute(fUI,fInterpreter); }
public:
  TermCyclic(Slash::UI::IUI& aUI,
             const std::string& aPrompt,
             const std::string& aInterp)
  //10 is millisec.
  :GtkCyclic(aUI.session(),"onx_terminal",10,aInterp,"") 
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

bool OnX::GtkUI::enableTerminal(const std::string& aPrompt,const std::string& aInterp){
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

bool OnX::GtkUI::disableTerminal(){
  if(!fTermCyclic) return true;
  delete fTermCyclic;
  fTermCyclic = 0;
  return true;
}

std::string OnX::GtkUI::ask(const std::string& aPrompt){
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

bool OnX::GtkUI::receiveScene(
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
