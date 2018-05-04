// this :
#include <OnX/Gtk/GtkTools.h>

#include <inlib/srep>
#include <inlib/system>

#define NotFound (-1)

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
//////////////////////////////////////////////////////////////////////////////
GtkWidget* OnX::gtk_widget_get_windowed_parent(
 GtkWidget* aWidget
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!GTK_WIDGET_NO_WINDOW(aWidget)) return aWidget; // has a window.
  //FIXME : loop on parent ?
  GtkWidget* parent = gtk_widget_get_parent(aWidget);
  if(GTK_WIDGET_NO_WINDOW(parent)) return 0; // parent has no window.
  return parent;
}*/

bool OnX::gtk_widget_set_background(GtkWidget* aWidget,const std::string& aValue){
  GdkColor color;
  gdk_color_parse(aValue.c_str(),&color);
  gtk_widget_modify_bg (aWidget, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_base (aWidget, GTK_STATE_NORMAL, &color);
  return true;
}

bool OnX::gtk_widget_set_foreground(GtkWidget* aWidget,const std::string& aValue){
  if(GTK_WIDGET_NO_WINDOW(aWidget)) return false; //REVISIT : needed ?
  GdkColor color;
  gdk_color_parse(aValue.c_str(),&color);
  gtk_widget_modify_fg (aWidget, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_text (aWidget, GTK_STATE_NORMAL, &color);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_widget_set_background2(
 GtkWidget* aWidget
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(GTK_WIDGET_NO_WINDOW(aWidget)) return false;

  GdkColor color;
  gdk_color_parse(aValue.c_str(),&color);

  GdkColor* colorPale = gdk_color_copy(&color);
  colorPale->red = (int)(65535+2*color.red)/3;
  colorPale->green = (int)(65535+2*color.green)/3;
  colorPale->blue = (int)(65535+2*color.blue)/3;

  GdkColor* colorDark = gdk_color_copy(&color);
  colorDark->red = (int)(color.red/1.5);
  colorDark->green = (int)(color.green/1.5);
  colorDark->blue = (int)(color.blue/1.5);

  gtk_widget_modify_bg (aWidget, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_base (aWidget, GTK_STATE_NORMAL, &color);

  gtk_widget_modify_bg (aWidget, GTK_STATE_PRELIGHT, colorPale);
  gtk_widget_modify_base (aWidget, GTK_STATE_PRELIGHT, colorPale);

  gtk_widget_modify_bg (aWidget, GTK_STATE_ACTIVE, colorDark);
  gtk_widget_modify_base (aWidget, GTK_STATE_ACTIVE, colorDark);

  gtk_widget_modify_bg (aWidget, GTK_STATE_SELECTED, colorDark);
  gtk_widget_modify_base (aWidget, GTK_STATE_SELECTED, colorDark);

  gtk_widget_modify_bg (aWidget, GTK_STATE_INSENSITIVE, colorDark);
  gtk_widget_modify_base (aWidget, GTK_STATE_INSENSITIVE, colorDark);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_widget_set_foreground2(
 GtkWidget* aWidget
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(GTK_WIDGET_NO_WINDOW(aWidget)) return false; //REVISIT : needed ?
  GdkColor color;
  gdk_color_parse(aValue.c_str(),&color);
  gtk_widget_modify_fg (aWidget, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_text (aWidget, GTK_STATE_NORMAL, &color);

  gtk_widget_modify_fg (aWidget, GTK_STATE_PRELIGHT, &color);
  gtk_widget_modify_text (aWidget, GTK_STATE_PRELIGHT, &color);

  gtk_widget_modify_fg (aWidget, GTK_STATE_ACTIVE, &color);
  gtk_widget_modify_text (aWidget, GTK_STATE_ACTIVE, &color);

  gtk_widget_modify_fg (aWidget, GTK_STATE_SELECTED, &color);
  gtk_widget_modify_text (aWidget, GTK_STATE_SELECTED, &color);

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_button_set_background(
 GtkButton* aWidget
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return gtk_widget_set_background2(GTK_WIDGET(aWidget),aValue);
} 
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_button_set_foreground(
 GtkButton* aWidget
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return gtk_widget_set_foreground2(GTK_WIDGET(aWidget),aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_menu_item_set_background(
 GtkMenuItem* aWidget
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return gtk_widget_set_background2(GTK_WIDGET(aWidget),aValue);
} 
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_menu_item_set_foreground(
 GtkMenuItem* aWidget
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return gtk_widget_set_foreground2(GTK_WIDGET(aWidget),aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_box_set_expand(
 GtkBox* aBox
,const std::string& aExpand
,GtkWidget* aContainer
)
//////////////////////////////////////////////////////////////////////////////
// @param aBox The box widget in wich we want to pack child
// @param aExpand The expand value for this parent widget : "first", "second" 
// or both. Anything else would be interpreted as "both"
// @param optional parameter to give a container to pack in the parent widget 
// if this parent hasn't already got two childs
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  GList* dlist = aBox->children;
  int number  = g_list_length(dlist);

  if (number == 0) { // this is the first widget to pack in
    if (aExpand=="first") { 
      gtk_box_pack_start(aBox,aContainer,TRUE,TRUE,0);
    } else if(aExpand == "second") {
      gtk_box_pack_start(aBox,aContainer,FALSE,TRUE,0);
    } else {
      gtk_box_pack_start(aBox,aContainer,TRUE,TRUE,0);
    }
  } else if (number == 1) { // this is the second widget to pack in
    if (aExpand=="first") {
      gtk_box_pack_start(aBox,aContainer,FALSE,TRUE,0);
    } else if(aExpand == "second") {
      gtk_box_pack_start(aBox,aContainer,TRUE,TRUE,0);
    } else {
      gtk_box_pack_start(aBox,aContainer,TRUE,TRUE,0);
    }
  } else if (number == 2) { // change expand value for these widgets
    if (!dlist) return false;
    GtkBoxChild* first = (GtkBoxChild*)(dlist->data);
    dlist = dlist->next;
    GtkBoxChild* second = (GtkBoxChild*)(dlist->data);

    if (aExpand == "first") {
      gtk_box_set_child_packing(aBox,
                                first->widget,
                                true,
                                first->fill,
                                first->padding,
                                (GtkPackType)first->pack);
      gtk_box_set_child_packing(aBox,
                                second->widget,
                                false,
                                second->fill,
                                second->padding,
                                (GtkPackType)second->pack);
    } else if (aExpand == "second") {
      gtk_box_set_child_packing(aBox,
                                first->widget,
                                false,
                                first->fill,
                                first->padding,
                                (GtkPackType)first->pack);
      gtk_box_set_child_packing(aBox,
                                second->widget,
                                true,
                                second->fill,
                                second->padding,
                                (GtkPackType)second->pack);
      
    } else {
      gtk_box_set_child_packing(aBox,
                                first->widget,
                                true,
                                first->fill,
                                first->padding,
                                (GtkPackType)first->pack);
      gtk_box_set_child_packing(aBox,
                                second->widget,
                                true,
                                second->fill,
                                second->padding,
                                (GtkPackType)second->pack);
      
    } 
  } else {
    return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_pixmap_set_from_file(
 GtkPixmap* aPixmap
,GdkWindow* aWindow
,const std::string& aFile
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string fileName;
  inlib::file_name(aFile,fileName);
  GdkColor color;
  GdkBitmap* mask = 0;
  GdkPixmap* pixmap = 
    gdk_pixmap_create_from_xpm(aWindow,
                               &mask,
                               &color,
                               fileName.c_str());
  if(!pixmap) return false;
  gtk_pixmap_set(aPixmap,pixmap,mask);
  gdk_pixmap_unref(pixmap);
  gdk_pixmap_unref(mask);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_option_menu_initialize(
 GtkOptionMenu* aWidget
,const std::vector<std::string>& aItems
,const std::string& aDefault 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Remove some existing menu :
  GtkWidget* menu = gtk_menu_new();
  gtk_option_menu_set_menu(aWidget,menu);
  menu = gtk_option_menu_get_menu(aWidget);
  if(!menu) return false;
  //Set aItems
  int history = NotFound;
  std::vector<std::string>::const_iterator it;
  unsigned int index = 0;
  for(it=aItems.begin();it!=aItems.end();++it,index++) {
    GtkWidget* item = gtk_menu_item_new_with_label((*it).c_str());
    gtk_menu_append(GTK_MENU(menu),item);
    gtk_widget_show(item);
    if(history==NotFound) history = index;//first item.
    if(aDefault.size()&&((*it)==aDefault)) history = index;
  }
  if(history!=NotFound) gtk_option_menu_set_history(aWidget,history);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
GList* OnX::gtk_option_menu_get_menu_items(
 GtkOptionMenu* aWidget
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  GtkWidget* menu = gtk_option_menu_get_menu(aWidget);
  if(!menu) return 0;
  GtkMenuShell* menu_shell = GTK_MENU_SHELL (menu);
  if(!menu_shell) return 0;
  return menu_shell->children;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_option_menu_set_default(
 GtkOptionMenu* aWidget
,const std::string& aDefault 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  GList* items = gtk_option_menu_get_menu_items(aWidget);
  int index = 0;
  bool found = false;
  while (items) {
    GtkWidget* item = (GtkWidget*)items->data;
    // FIXME : the first item in the option menu 
    // is a GtkMenuItem but NOT a GtkLabel...
    if (GTK_BIN(item)) {
      GtkWidget* child = GTK_BIN(item)->child;
      if(GTK_IS_LABEL(child)) {
        char* str = 0;
        gtk_label_get(GTK_LABEL(child),&str);
        std::string label = str?str:"";
        if(label==aDefault) {
          gtk_option_menu_set_history(aWidget,index);
          found = true;
          break;
        }
      }
    }
    items = items->next;
    index++;
  }  
  if (!found) gtk_option_menu_set_history(aWidget,0);
  return found;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_option_menu_get_value(
 GtkOptionMenu* aWidget
,std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  GtkWidget* menu = gtk_option_menu_get_menu(aWidget);
  if(!menu) return false;
  GtkWidget* active = gtk_menu_get_active(GTK_MENU(menu));
  if(!active) return false;
  if(GTK_IS_MENU_ITEM(active)==FALSE) return false;
  GtkWidget* child = GTK_BIN(GTK_MENU_ITEM(active))->child;
  if(child && GTK_IS_LABEL(child)) {
    gchar* s = 0;
    gtk_label_get(GTK_LABEL(child),&s);
    if(!s) return false;
    aValue = s;
    return true;
  } else { //REVISIT
    // this work to get the value of a ComboBox non focus 
    GtkWidget* child = GTK_BIN(GTK_BUTTON(aWidget))->child;
    if(child && GTK_IS_LABEL(child)) {
      gchar* s = 0;
      gtk_label_get(GTK_LABEL(child),&s);
      if(!s) return false;
      aValue = s;
      return true;
    }
  }
  return false;
}

void OnX::gtk_combo_set_items(GtkCombo* aWidget,const std::vector<std::string>& aItems){
  GList* list = 0;
  std::vector<std::string>::const_iterator it;
  for(it=aItems.begin();it!=aItems.end();++it) {
    list = g_list_append(list,(char*)(*it).c_str());
  }
  gtk_combo_set_popdown_strings(aWidget,list);
  g_list_free(list);
}
void OnX::gtk_combo_set_value(GtkCombo* aWidget,const std::string& aValue){
  gtk_entry_set_text(GTK_ENTRY(aWidget->entry),(char*)aValue.c_str());
}
bool OnX::gtk_combo_get_value(GtkCombo* aWidget,std::string& aValue){
  const gchar* s = gtk_entry_get_text(GTK_ENTRY(aWidget->entry));
  if(!s) {aValue="";return false;}
  aValue = s;
  return true;
}
bool OnX::gtk_combo_get_items(GtkCombo* aWidget,std::vector<std::string>& aItems){
  return gtk_list_get_items(GTK_LIST(aWidget->list),aItems);
}

bool OnX::gtk_list_get_items(GtkList* aWidget,std::vector<std::string>& aItems){
  aItems.clear();
  GList* dlist = aWidget->children;
  while(dlist) {
    GtkWidget* item = GTK_WIDGET(dlist->data);
    GtkLabel* label = GTK_LABEL(GTK_BIN(item)->child);
    gchar* s = 0;
    gtk_label_get(label,&s);
    if(!s) {aItems.clear();return false;}
    aItems.push_back(s);
    dlist=dlist->next;
  }
  return true;
}

bool OnX::gtk_list_get_selection(GtkList* aWidget,std::string& aValue) {
  aValue = "";
  GList* dlist = aWidget->selection;
  while(dlist) {
    GtkWidget* item = GTK_WIDGET(dlist->data);
    GtkLabel* label = GTK_LABEL(GTK_BIN(item)->child);
    gchar* s = 0;
    gtk_label_get(label,&s);
    aValue += s?s:"";
    dlist=dlist->next;
    if(dlist) aValue += "\n";
  }
  return true;
}

bool OnX::gtk_list_set_selection(GtkList* aWidget,const std::string& aValue) {
  gtk_list_unselect_all(aWidget);
  GList* dlist = aWidget->children;
  while(dlist) {
    GtkWidget* item = GTK_WIDGET(dlist->data);
    GtkLabel* label = GTK_LABEL(GTK_BIN(item)->child);
    gchar* s = 0;
    gtk_label_get(label,&s);
    if(s && (aValue==std::string(s))) {
      gtk_list_select_child(GTK_LIST(aWidget),item);
      return true;
    }
    dlist=dlist->next;
  }
  return false;
}

bool OnX::gtk_text_view_set_value(GtkTextView* aWidget,const std::string& aValue){
  gtk_text_buffer_set_text
    (gtk_text_view_get_buffer(aWidget),aValue.c_str(),-1);
  return true;
}


class SearchTreeItem {
public:
  GtkTreeView* fView;
  std::string fLabel;
};

#include <cstring>

//////////////////////////////////////////////////////////////////////////////
static gboolean expandTreeModelItem(
 GtkTreeModel* aModel
,GtkTreePath* aPath
,GtkTreeIter* aIter
,gpointer aData
)
//////////////////////////////////////////////////////////////////////////////
// select the entrie with label equal data->txt and expand it if possible
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  gchar* txt = 0;
  SearchTreeItem* data = (SearchTreeItem*)aData;
  gtk_tree_model_get(aModel, aIter,GTK_TREE_COL_TEXT, &txt,-1);
  if (!txt) return FALSE;
  if (::strcmp(data->fLabel.c_str(),txt)==0) {
    gtk_tree_view_expand_to_path(GTK_TREE_VIEW(data->fView),aPath);
    gtk_tree_selection_select_path(gtk_tree_view_get_selection(GTK_TREE_VIEW(data->fView)),aPath);
  }
  g_free(txt);
  return (::strcmp(data->fLabel.c_str(),txt)==0);
}

//////////////////////////////////////////////////////////////////////////////
void OnX::gtk_tree_view_expand_item(
 GtkTreeView* aView
,const std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
// loop on all tree and call the expandTreeModelItem to see 
// if search string==tree_item_label. If it matches, expand tree 
// to view this item
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (gtk_tree_view_get_model(aView)) {
    SearchTreeItem search;
    search.fView = aView;
    search.fLabel = aLabel;
    gtk_tree_model_foreach(gtk_tree_view_get_model(aView),
                           expandTreeModelItem,&search);
  }
}

//////////////////////////////////////////////////////////////////////////////
std::string OnX::gtk_tree_model_get_XML(
 GtkTreeModel* aModel
,GtkTreeView* aView
,GtkTreeIter* aIter
)
//////////////////////////////////////////////////////////////////////////////
//return a XML string representing this tree
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string line;
  std::string value;
  GtkTreePath* path = gtk_tree_model_get_path(aModel,aIter);
  //gint depth = gtk_tree_path_get_depth(path);
  std::string spaceItem = "";
  std::string spaceRoot = "";
  //spaceItem.insert(0,depth*2,' ');
  //spaceRoot.insert(0,(depth-1)*2,' ');

  do  {
    GtkTreeIter child;
    gchar *label = 0;
    gtk_tree_model_get(aModel, aIter,GTK_TREE_COL_TEXT, &label,-1);
    gboolean expand = gtk_tree_view_row_expanded(aView,path);
    line += spaceRoot + "<treeItem>";
    line += spaceItem + "<label>";
    value = inlib::to_xml(label);
    line.append(value);
    line += "</label>";
    line += spaceItem + "<opened>";
    if (expand) line.append("true");
    else line.append("false");
    line += "</opened>";
    if (gtk_tree_model_iter_children (aModel, &child, aIter)) {
      gtk_tree_path_down (path);
      line += gtk_tree_model_get_XML(aModel, aView,&child);
      gtk_tree_path_up (path);
    }    
    gtk_tree_path_next (path);
    line += spaceRoot + "</treeItem>";
    g_free(label);
  }
  while (gtk_tree_model_iter_next(aModel,aIter));
  gtk_tree_path_free (path);
  return line;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::gtk_tree_selection_get_path(
 GtkTreeSelection* aSelection
)
//////////////////////////////////////////////////////////////////////////////
// This will only work in single or browse selection mode!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  GtkTreeModel* model;
  GtkTreeIter child;
  std::string path ="";
  if(gtk_tree_selection_get_selected(aSelection, &model, &child)) {
    gchar* s = 0;
    gtk_tree_model_get (model, &child, GTK_TREE_COL_TEXT, &s, -1); 
    path = "\n";
    path += s?s:"";
    g_free(s);
    GtkTreeIter parent;
    while(gtk_tree_model_iter_parent (model,&parent,&child)) {
      gchar* s = 0;
      gtk_tree_model_get (model, &parent, GTK_TREE_COL_TEXT, &s, -1);
      std::string ss = s?s:"";
      g_free(s);
      std::string opath = path;
      path = "\n";
      path += ss;
      path += opath;
      child = parent;
    }
    // Remove the leading \n
    if(path.size()) path = path.substr(1,path.size()-1);
  }
  return path;
}

bool OnX::gtk_notebook_get_selection(GtkNotebook* aWidget,std::string& aValue){
  int index = gtk_notebook_get_current_page(aWidget);
  if(index==NotFound) return false;
  GtkWidget* page = gtk_notebook_get_nth_page(aWidget,index);
  if(!page) return false;
  const gchar* s = gtk_notebook_get_tab_label_text(aWidget,GTK_WIDGET(page));
  aValue = s?s:"";
  return true;
}

bool OnX::gtk_notebook_set_selection(GtkNotebook* aWidget,const std::string& aValue){
  gint pagen = gtk_notebook_get_n_pages(aWidget);
  for(int index=0;index<pagen;index++) {
    GtkWidget* page = gtk_notebook_get_nth_page(aWidget,index);
    if(page) {
      const gchar* s = 
        gtk_notebook_get_tab_label_text(aWidget,GTK_WIDGET(page));
      if(s && (aValue==std::string(s))) {
        gtk_notebook_set_current_page(aWidget,index);
        return true;
      }
    }
  }
  return false;
}

#ifdef ONX_HAS_GTK_GL_AREA
#include "../source/Core/OnX_gl2ps.h"

//////////////////////////////////////////////////////////////////////////////
bool OnX::gtk_gl_area_write_gl2ps(
 GtkGLArea* aGLArea
,const std::string& aFileName
,const std::string&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  FILE* file = ::fopen(aFileName.c_str(),"w");
  if(!file) return false;

  int options = GL2PS_OCCLUSION_CULL 
    | GL2PS_BEST_ROOT 
    | GL2PS_SILENT
    | GL2PS_DRAW_BACKGROUND;
  int sort = GL2PS_BSP_SORT;
  //sort = GL2PS_SIMPLE_SORT;
    
  GLint vp[4];
  vp[0] = 0;
  vp[1] = 0;
  vp[2] = GTK_WIDGET(aGLArea)->allocation.width;
  vp[3] = GTK_WIDGET(aGLArea)->allocation.height;

  int bufsize = 0;
  gl2psBeginPage("title","OnX_gl2ps", 
                 vp,GL2PS_EPS,sort,options, 
                 GL_RGBA,0, NULL,0,0,0,bufsize, 
                 file,aFileName.c_str());
    
  // Send an expose event :
  GdkEventExpose event;
  event.type = GDK_EXPOSE;
  event.window = GTK_WIDGET(aGLArea)->window;
  event.count = 0;
  event.area.x = 0;
  event.area.y = 0;
  event.area.width = GTK_WIDGET(aGLArea)->allocation.width;
  event.area.height = GTK_WIDGET(aGLArea)->allocation.height;
  gtk_widget_event(GTK_WIDGET(aGLArea),(GdkEvent*)&event);

  gl2psEndPage();
  ::fclose(file);

  return true;
}

#endif
