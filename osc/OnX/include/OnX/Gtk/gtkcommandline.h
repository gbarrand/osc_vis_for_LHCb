#ifndef __GTK_COMMAND_LINE_H__
#define __GTK_COMMAND_LINE_H__

#include <gdk/gdk.h>
#include <gtk/gtkentry.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GTK_TYPE_COMMAND_LINE (gtk_command_line_get_type())
#define GTK_COMMAND_LINE(obj) (GTK_CHECK_CAST ((obj), GTK_TYPE_COMMAND_LINE, GtkCommandLine))
#define GTK_COMMAND_LINE_CLASS(klass) (GTK_CHECK_CLASS_CAST (klass, GTK_TYPE_COMMAND_LINE, GtkCommandLineClass))
#define GTK_IS_COMMAND_LINE(obj) (GTK_CHECK_TYPE ((obj), GTK_TYPE_COMMAND_LINE))
#define GTK_IS_COMMAND_LINE_CLASS (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_COMMAND_LINE))

typedef struct _GtkCommandLine       GtkCommandLine;
typedef struct _GtkCommandLineClass  GtkCommandLineClass;

typedef gint (*GtkCommandLineKeyFunc)(GtkWidget*,GdkEventKey*);

struct _GtkCommandLine {
  GtkEntry entry;
  int historySize;
  char** historyList;
  int historyPos;
  gchar* value;
};

struct _GtkCommandLineClass {
  GtkEntryClass parent_class;
  GtkCommandLineKeyFunc key_press_event;
  void (*complete)(GtkCommandLine*);
};

GtkType gtk_command_line_get_type(void);
GtkWidget* gtk_command_line_new();
gchar* gtk_command_line_get_value(GtkCommandLine*);
void gtk_command_line_set_value(GtkCommandLine*,char*);

#ifdef __cplusplus
}
#endif

#endif
