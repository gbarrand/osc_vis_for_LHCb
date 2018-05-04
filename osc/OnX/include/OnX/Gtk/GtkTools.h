#ifndef OnX_GtkTools_h
#define OnX_GtkTools_h

#include <gtk/gtk.h>
#include <string>
#include <vector>

#include <gtk/gtkbox.h>
#include <gtk/gtkpixmap.h>
#include <gtk/gtkoptionmenu.h>
#include <gtk/gtkcombo.h>
#include <gtk/gtknotebook.h>

#ifdef ONX_HAS_GTK_GL_AREA
#include <gtkgl/gtkglarea.h>
#endif

namespace OnX {

  //GtkWidget* gtk_widget_get_windowed_parent(GtkWidget*);
  bool gtk_widget_set_foreground(GtkWidget*,const std::string&);
  bool gtk_widget_set_background(GtkWidget*,const std::string&);
  bool gtk_widget_set_foreground2(GtkWidget*,const std::string&);
  bool gtk_widget_set_background2(GtkWidget*,const std::string&);

  bool gtk_button_set_foreground(GtkButton*,const std::string&);
  bool gtk_button_set_background(GtkButton*,const std::string&);
  bool gtk_menu_item_set_foreground(GtkMenuItem*,const std::string&);
  bool gtk_menu_item_set_background(GtkMenuItem*,const std::string&);
  bool gtk_box_set_expand(GtkBox*,const std::string&,GtkWidget* = 0);
  bool gtk_pixmap_set_from_file(GtkPixmap*,GdkWindow*,const std::string&);
  bool gtk_option_menu_initialize(GtkOptionMenu*,
                                  const std::vector<std::string>&,
                                  const std::string&);
  GList* gtk_option_menu_get_menu_items(GtkOptionMenu*);
  bool gtk_option_menu_set_default(GtkOptionMenu*,const std::string&);
  bool gtk_option_menu_get_value(GtkOptionMenu*,std::string&);

  void gtk_combo_set_items(GtkCombo*,const std::vector<std::string>&);
  void gtk_combo_set_value(GtkCombo*,const std::string&);
  bool gtk_combo_get_value(GtkCombo*,std::string&);
  bool gtk_combo_get_items(GtkCombo*,std::vector<std::string>&);

  bool gtk_list_get_items(GtkList*,std::vector<std::string>&);
  bool gtk_list_set_selection(GtkList*,const std::string&);
  bool gtk_list_get_selection(GtkList*,std::string&);

  bool gtk_notebook_set_selection(GtkNotebook*,const std::string&);
  bool gtk_notebook_get_selection(GtkNotebook*,std::string&);

  bool gtk_text_view_set_value(GtkTextView*,const std::string&);

  std::string gtk_tree_model_get_XML(GtkTreeModel*,GtkTreeView*,GtkTreeIter*);
  std::string gtk_tree_selection_get_path(GtkTreeSelection*);
  void gtk_tree_view_expand_item(GtkTreeView*,const std::string&);

  // column number for the treeView widget
#define GTK_TREE_COL_TEXT 0

#ifdef ONX_HAS_GTK_GL_AREA
  bool gtk_gl_area_write_gl2ps
         (GtkGLArea*,const std::string&,const std::string&);
#endif

}

#endif
