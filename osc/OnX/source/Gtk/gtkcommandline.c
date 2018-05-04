/* this */
#include <OnX/Gtk/gtkcommandline.h>

#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtksignal.h>

#ifdef __cplusplus
extern "C" {
#endif

static void gtk_command_line_class_init(GtkCommandLineClass*);
static void gtk_command_line_init(GtkCommandLine*);
static void gtk_command_line_destroy(GtkObject*);

static GtkEntryClass* parent_class = NULL;

static guint gtk_command_line_complete_signal = 0;

static void Callback(GtkWidget*,gpointer);
/*static void KeyCallback(GtkWidget*,GdkEventKey*,gpointer);*/
static void AddLine(GtkCommandLine*,char*);

static gint key_press_event(GtkWidget*,GdkEventKey*);
#ifdef __cplusplus
}
#endif

#ifndef GTK_CLASS_TYPE
#define GTK_CLASS_TYPE(object_class)  object_class->type
#endif

GtkType gtk_command_line_get_type () {
  static GType ttt_type = 0;
  if(!ttt_type) {
    static const GTypeInfo ttt_info = {
      sizeof (GtkCommandLineClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc)gtk_command_line_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkCommandLine),
      0,    /* n_preallocs */
      (GInstanceInitFunc) gtk_command_line_init
    };
    ttt_type = g_type_register_static(gtk_entry_get_type(),
                                      "GtkCommandLine",&ttt_info,0);
  }
  return ttt_type;
}

static void gtk_command_line_class_init(GtkCommandLineClass* aClass) {
  GtkObjectClass* object_class;
  GtkWidgetClass* widget_class;

  parent_class = gtk_type_class(gtk_entry_get_type());
  object_class = (GtkObjectClass*)aClass;
  object_class->destroy = gtk_command_line_destroy;

  gtk_command_line_complete_signal =  
    gtk_signal_new ("complete",
                    (GtkSignalRunType)(GTK_RUN_LAST | GTK_RUN_ACTION),
                    GTK_CLASS_TYPE(object_class),
                    GTK_SIGNAL_OFFSET(GtkCommandLineClass,complete),
                    gtk_marshal_NONE__NONE,
                    GTK_TYPE_NONE,0);

  /* 
   *  Override the GtkEntry key_press_event function to master
   * the up and down keys loose of focus of the GtkEntry.
   */
  widget_class = (GtkWidgetClass*)aClass;
  aClass->key_press_event = widget_class->key_press_event;
  widget_class->key_press_event = key_press_event;
}


static void gtk_command_line_init(GtkCommandLine* aCommandLine) {
  GtkEntry* entry;

  aCommandLine->historySize = 0;
  aCommandLine->historyList = NULL;
  aCommandLine->historyPos = -1;
  aCommandLine->value = NULL;

  entry = GTK_ENTRY(GTK_WIDGET(aCommandLine));

  gtk_signal_connect(GTK_OBJECT(entry), 
                     "activate",
                     GTK_SIGNAL_FUNC(Callback), 
                     (gpointer)NULL);

  /*
  gtk_signal_connect(GTK_OBJECT(entry), 
                     "key_press_event",
                     GTK_SIGNAL_FUNC(KeyCallback), 
                     (gpointer)NULL);
  */
}

static void gtk_command_line_destroy(GtkObject* aObject) {
  GtkCommandLine* commandLine;

  g_return_if_fail (aObject != NULL);
  g_return_if_fail (GTK_IS_COMMAND_LINE(aObject));
  
  commandLine = GTK_COMMAND_LINE(aObject);

  if(commandLine->historyList!=NULL) {
    int count;
    for(count=0;count<commandLine->historySize;count++) 
      g_free(commandLine->historyList[count]);
    g_free(commandLine->historyList);
    commandLine->historyList = 0;
  }
  g_free(commandLine->value);
  commandLine->value = 0;

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (* GTK_OBJECT_CLASS (parent_class)->destroy) (aObject);
}

GtkWidget* gtk_command_line_new() {
  return GTK_WIDGET(gtk_type_new(gtk_command_line_get_type()));
}


gchar* gtk_command_line_get_value(GtkCommandLine* aCommandLine) {
  g_return_val_if_fail(aCommandLine != NULL, NULL);
  g_return_val_if_fail(GTK_IS_COMMAND_LINE (aCommandLine), NULL);
  return aCommandLine->value;
}
void gtk_command_line_set_value(GtkCommandLine* aCommandLine,char* aValue) {
  g_return_if_fail(aCommandLine != NULL);
  g_return_if_fail(GTK_IS_COMMAND_LINE (aCommandLine));
  gtk_entry_set_text(GTK_ENTRY(aCommandLine),aValue);
}

static void Callback(GtkWidget* aWidget,gpointer aTag){
  GtkCommandLine* commandLine = GTK_COMMAND_LINE(aWidget);

  gchar* command = (gchar*)gtk_entry_get_text(GTK_ENTRY(aWidget));

  if(command) {
    AddLine(commandLine,command); /* Store in the history list. */
    commandLine->historyPos = -1;
    g_free(commandLine->value);
    commandLine->value = g_strdup(command);
  }

  gtk_entry_set_text(GTK_ENTRY(aWidget),"");

}
static gint key_press_event(GtkWidget* aWidget,GdkEventKey* aEvent) {
  GtkCommandLine* commandLine = GTK_COMMAND_LINE(aWidget);
  if(aEvent->keyval==GDK_Up) {
    int n,pos;
    n = commandLine->historySize; 
    pos = commandLine->historyPos == -1 ? n-1 : commandLine->historyPos-1;
    if((pos>=0)&&(pos<n)) {
      char* command = commandLine->historyList[pos];
      if(command) {
        gtk_entry_set_text(GTK_ENTRY(aWidget),command);
      }
      commandLine->historyPos = pos;
    }
    return TRUE;
  } if(aEvent->keyval==GDK_Down) {
    int n,pos;
    n = commandLine->historySize; 
    pos = commandLine->historyPos == -1 ? n : commandLine->historyPos + 1;
    if((pos>=0)&&(pos<n)) {
      char* command = commandLine->historyList[pos];
      if(command) {
        gtk_entry_set_text(GTK_ENTRY(aWidget),command);
      }
      commandLine->historyPos = pos;
    } else if(pos>=n) {
      gtk_entry_set_text(GTK_ENTRY(aWidget),"");
      commandLine->historyPos = -1;
    }
    return TRUE;
  } if(aEvent->keyval==GDK_Escape) {
    gchar* command = (gchar*)gtk_entry_get_text(GTK_ENTRY(commandLine));
    g_free(commandLine->value);
    commandLine->value = g_strdup(command);
    gtk_signal_emit(GTK_OBJECT(aWidget),gtk_command_line_complete_signal);
    return TRUE;
  } else {
    return GTK_WIDGET_CLASS(parent_class)->key_press_event(aWidget,aEvent);
  }
}

/*
static void KeyCallback(GtkWidget* aWidget,GdkEventKey* aEvent,gpointer aPointer) {
  if(aEvent->keyval==GDK_Up) {
    GtkCommandLine* commandLine = GTK_COMMAND_LINE(aWidget);
    int n,pos;
    n = commandLine->historySize; 
    pos = commandLine->historyPos == -1 ? n-1 : commandLine->historyPos-1;
    if((pos>=0)&&(pos<n)) {
      char* command = commandLine->historyList[pos];
      if(command) {
        gtk_entry_set_text(GTK_ENTRY(aWidget),command);
      }
      commandLine->historyPos = pos;
    }
  } if(aEvent->keyval==GDK_Down) {
    GtkCommandLine* commandLine = GTK_COMMAND_LINE(aWidget);
    int n,pos;
    n = commandLine->historySize; 
    pos = commandLine->historyPos == -1 ? n : commandLine->historyPos + 1;
    if((pos>=0)&&(pos<n)) {
      char* command = commandLine->historyList[pos];
      if(command) {
        gtk_entry_set_text(GTK_ENTRY(aWidget),command);
      }
      commandLine->historyPos = pos;
    } else if(pos>=n) {
      gtk_entry_set_text(GTK_ENTRY(aWidget),"");
      commandLine->historyPos = -1;
    }
  }
  aPointer = NULL;
}
*/

static void AddLine(GtkCommandLine* aCommandLine,char* a_string){
  /* FIXME : Should use the glib lists. */
  if(aCommandLine->historyList==NULL) {
    aCommandLine->historyList = (char**)g_malloc(sizeof(char*));
    if(aCommandLine->historyList==NULL) {
      aCommandLine->historySize = 0;
      aCommandLine->historyPos = 0;
      return;
    }
    aCommandLine->historyList[0] = g_strdup(a_string);
    aCommandLine->historySize = 1;
  } else {
    aCommandLine->historyList = (char**)g_realloc((char*)aCommandLine->historyList,
                        (aCommandLine->historySize + 1)*sizeof(char*));
    if(aCommandLine->historyList==NULL) {
      aCommandLine->historySize = 0;
      aCommandLine->historyPos = 0;
      return;
    }
    aCommandLine->historyList[aCommandLine->historySize] = g_strdup(a_string);
    aCommandLine->historySize++;
  }
}
