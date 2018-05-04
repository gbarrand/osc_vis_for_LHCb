#ifndef __GTK_WARNING_DIALOG_H__
#define __GTK_WARNING_DIALOG_H__

#include <gtk/gtkdialog.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GTK_TYPE_WARNING_DIALOG (gtk_warning_dialog_get_type())
#define GTK_WARNING_DIALOG(obj) (GTK_CHECK_CAST ((obj), GTK_TYPE_WARNING_DIALOG, GtkWarningDialog))
#define GTK_WARNING_DIALOG_CLASS(klass) (GTK_CHECK_CLASS_CAST (klass, GTK_TYPE_WARNING_DIALOG, GtkWarningDialogClass))
#define GTK_IS_WARNING_DIALOG(obj) (GTK_CHECK_TYPE ((obj), GTK_TYPE_WARNING_DIALOG))
#define GTK_IS_WARNING_DIALOG_CLASS (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_WARNING_DIALOG))

typedef struct _GtkWarningDialog       GtkWarningDialog;
typedef struct _GtkWarningDialogClass  GtkWarningDialogClass;

struct _GtkWarningDialog {
  GtkDialog dialog;
  GtkWidget* label;
  GtkWidget* ok;
};

struct _GtkWarningDialogClass {
  GtkDialogClass parent_class;
};

GtkType gtk_warning_dialog_get_type(void);
GtkWidget* gtk_warning_dialog_new();
void gtk_warning_dialog_set_message(GtkWarningDialog*,char*);

#ifdef __cplusplus
}
#endif

#endif
