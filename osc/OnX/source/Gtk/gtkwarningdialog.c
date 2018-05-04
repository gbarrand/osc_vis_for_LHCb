/* this */
#include <OnX/Gtk/gtkwarningdialog.h>

#include <gtk/gtkvbox.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkbutton.h>

#ifdef __cplusplus
extern "C" {
#endif

static void gtk_warning_dialog_class_init(GtkWarningDialogClass*);
static void gtk_warning_dialog_init(GtkWarningDialog*);
static void gtk_warning_dialog_destroy(GtkObject*);

static GtkDialogClass* parent_class = NULL;
#ifdef __cplusplus
}
#endif


GtkType gtk_warning_dialog_get_type () {
  static GType ttt_type = 0;
  if(!ttt_type) {
    static const GTypeInfo ttt_info = {
      sizeof (GtkWarningDialogClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) gtk_warning_dialog_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkWarningDialog),
      0,    /* n_preallocs */
      (GInstanceInitFunc) gtk_warning_dialog_init
    };
    ttt_type = g_type_register_static (GTK_TYPE_DIALOG,
                                       "GtkWarningDialog",
                                       &ttt_info,
                                       0);
  }
  return ttt_type;
}

static void gtk_warning_dialog_class_init(GtkWarningDialogClass* aClass) {
  GtkObjectClass* object_class;

  parent_class = gtk_type_class(gtk_dialog_get_type());
  object_class = (GtkObjectClass*)aClass;
  object_class->destroy = gtk_warning_dialog_destroy;
}


static void gtk_warning_dialog_init(GtkWarningDialog* aWarningDialog) {
  GtkDialog* dialog;

  dialog = GTK_DIALOG(GTK_WIDGET(aWarningDialog));

  gtk_window_set_title(GTK_WINDOW(dialog),"WARNING");

  aWarningDialog->label = gtk_label_new("");
  gtk_container_add(GTK_CONTAINER(dialog->vbox),aWarningDialog->label);
  gtk_widget_show(aWarningDialog->label);

  aWarningDialog->ok = gtk_button_new_with_label("Ok");
  gtk_container_add(GTK_CONTAINER(dialog->action_area),aWarningDialog->ok);
  gtk_widget_show(aWarningDialog->ok);
}

static void gtk_warning_dialog_destroy(GtkObject* aObject) {
  g_return_if_fail (aObject != NULL);
  g_return_if_fail (GTK_IS_WARNING_DIALOG(aObject));
  
  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (aObject);
}

GtkWidget* gtk_warning_dialog_new() {
  return GTK_WIDGET(gtk_type_new(gtk_warning_dialog_get_type()));
}


void gtk_warning_dialog_set_message(GtkWarningDialog* aWarningDialog,char* aMessage) {
  g_return_if_fail(aWarningDialog != NULL);
  g_return_if_fail(GTK_IS_WARNING_DIALOG (aWarningDialog));
  gtk_label_set_text(GTK_LABEL(aWarningDialog->label),aMessage);
}
