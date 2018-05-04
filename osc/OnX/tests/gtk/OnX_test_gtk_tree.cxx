#include <gtk/gtk.h>
#include <string>

#ifndef HAS_GTK1

enum
{
  COL_TEXT = 0,
  NUM_COLS
};

struct searchTreeItem
{
  GtkTreeView *view;
  std::string txt;
};

static void
activated (GtkWidget *view, gpointer data)
{
  GtkTreeSelection *selection;
  GtkTreeModel     *model;
  GtkTreeIter       child,parent;
  
  /* This will only work in single or browse selection mode! */
  
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  if (gtk_tree_selection_get_selected(selection, &model, &child))
    {
      std::string path;
      gchar* s = 0;
      gtk_tree_model_get (model, &child, COL_TEXT, &s, -1); 
      path = "\n";
      path += s;
      while(gtk_tree_model_iter_parent (model,&parent,&child)) {
        gchar* s = 0;
        gtk_tree_model_get (model, &parent, COL_TEXT, &s, -1);
        std::string ss = s?s:"";
        std::string opath = path;
        path = "\n";
        path += ss;
        path += opath;
        child = parent;
      }
      // Remove the leading \n
      if(path.size()) path = path.substr(1,path.size()-1);
      g_print ("selected row is: \n%s\n",path.c_str());
    }
  else
    {
      g_print ("no row selected.\n");
    }
}

gboolean selectAndExpandTreeItem (GtkTreeModel *model,
                       GtkTreePath *path,
                       GtkTreeIter *iter,
                        void *data)
{
  gchar *txt ;
  searchTreeItem *user_data = (searchTreeItem*) data;
  gtk_tree_model_get(model, iter,COL_TEXT, &txt,-1);
  if (user_data->txt == txt) {
    GtkTreeSelection *selection= gtk_tree_view_get_selection(GTK_TREE_VIEW(user_data->view));
    gtk_tree_view_expand_to_path(GTK_TREE_VIEW(user_data->view),path); 
    gtk_tree_selection_select_path (selection,path);
  }
  g_free(txt);
  return (user_data->txt == txt);
}


static void
selectItem (std::string txt, GtkTreeView *view) {
  searchTreeItem mySearch;
  mySearch.view = view;
  mySearch.txt = txt;
  gtk_tree_model_foreach(gtk_tree_view_get_model(view),selectAndExpandTreeItem,&mySearch);

}

static GtkTreeModel *
create_and_fill_model (void)
{
  GtkTreeStore  *treestore;
  GtkTreeIter    toplevel, child,child2;

  treestore = gtk_tree_store_new(1, G_TYPE_STRING);

  /* Append a top level row and leave it empty */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_TEXT, "Maria",
                     -1);

  /* Append a second top level row, and fill it with some data */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_TEXT, "Jane",
                     -1);

  /* Append a child to the second top level row, and fill in some data */
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_TEXT, "Janinita",
                     -1);
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_TEXT, "Jean",
                     -1);
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_TEXT, "Pierre",
                     -1);
  /* Append a second top level row, and fill it with some data */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_TEXT, "Hyvette",
                     -1);

  /* Append a child to the second top level row, and fill in some data */
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_TEXT, "Janine",
                     -1);
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_TEXT, "Paul",
                     -1);
  gtk_tree_store_append(treestore, &child2, &child);
  gtk_tree_store_set(treestore, &child2,
                     COL_TEXT, "Elodie",
                     -1);
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_TEXT, "Marine",
                     -1);

  return GTK_TREE_MODEL(treestore);
}


static GtkWidget *
create_view_and_model (void)
{
    GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *view;
  GtkTreeModel        *model;

  view = gtk_tree_view_new();

  /* --- Column #1 --- */

  col = gtk_tree_view_column_new();

  //    gtk_tree_view_column_set_title(col, "First Name");

  /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
    gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to model column that contains the first name */
      gtk_tree_view_column_add_attribute(col, renderer, "text", COL_TEXT);


  model = create_and_fill_model();

  g_signal_connect(GTK_TREE_VIEW(view), "row-activated",G_CALLBACK(activated), NULL);

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

  g_object_unref(model); /* destroy model automatically with view */

  //  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_NONE);

  return view;
}


int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *view;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */

  view = create_view_and_model();

  gtk_container_add(GTK_CONTAINER(window), view);

  gtk_widget_show_all(window);

#ifndef HAS_GTK1
  selectItem(std::string("Elodie"),GTK_TREE_VIEW(view));
#endif
  gtk_main();

  return 0;
}
#endif
