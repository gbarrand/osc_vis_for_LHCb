

#include <gtk/gtk.h>

void callback(GtkObject*,gpointer);

static gfloat sCounter = 0;

int main(int aArgc,char** aArgv) {

  gtk_init(&aArgc, &aArgv);
  
  GtkWidget* topWidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(topWidget),600,600);

  GtkWidget* vbox = gtk_vbox_new(FALSE,0);
  gtk_container_add(GTK_CONTAINER(topWidget),vbox);

  GtkWidget* button = gtk_button_new_with_label("xxx");
  gtk_container_add(GTK_CONTAINER(vbox),button);

  GtkWidget* pb = gtk_progress_bar_new();
  gtk_progress_bar_set_bar_style(GTK_PROGRESS_BAR(pb),GTK_PROGRESS_DISCRETE);
  gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(pb),100);
  //gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(pb),100);
  gtk_container_add(GTK_CONTAINER(vbox),pb);

  gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(callback), 
                     (gpointer)pb);

  gtk_widget_show_all(topWidget);

  gtk_main ();

  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void callback(
 GtkObject* //aObject
,gpointer aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  GtkWidget* pb = (GtkWidget*)aTag;
  sCounter += 0.005;
  gtk_progress_bar_update(GTK_PROGRESS_BAR(pb),sCounter);
}
