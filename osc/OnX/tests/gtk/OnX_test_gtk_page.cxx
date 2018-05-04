

#include <gtk/gtk.h>

#include <Inventor/Gtk/SoGtk.h>
#include <OnX/Gtk/gtkpageviewer.h>

extern "C" {
  void HEPVisUtils_initClasses();
}

int main(int aArgc,char** aArgv) {

  gtk_init(&aArgc, &aArgv);
  
  GtkWidget* topWidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(topWidget),600,600);

  SoGtk::init(topWidget);
  HEPVisUtils_initClasses();

  GtkWidget* pageViewer = gtk_page_viewer_new("Examiner");
  gtk_widget_show_all(pageViewer);
  gtk_container_add(GTK_CONTAINER(topWidget),pageViewer);

  gtk_widget_show_all(topWidget);

  gtk_main ();

  return 0;
}
