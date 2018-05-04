#ifndef __GTK_PAGE_VIEWER_H__
#define __GTK_PAGE_VIEWER_H__

// Inheritance :
#include <gtk/gtkbin.h>

#include <gdk/gdk.h>

#include <string>

class SoGtkFullViewer;
class SoNode;

extern "C" {

typedef struct {
  gchar* value;
  int x,y;
} GtkCallbackStruct;

#define GTK_TYPE_PAGE_VIEWER (gtk_page_viewer_get_type())
#define GTK_PAGE_VIEWER(obj) (GTK_CHECK_CAST ((obj), GTK_TYPE_PAGE_VIEWER, GtkPageViewer))
#define GTK_PAGE_VIEWER_CLASS(klass) (GTK_CHECK_CLASS_CAST (klass, GTK_TYPE_PAGE_VIEWER, GtkPageViewerClass))
#define GTK_IS_PAGE_VIEWER(obj) (GTK_CHECK_TYPE ((obj), GTK_TYPE_PAGE_VIEWER))
#define GTK_IS_PAGE_VIEWER_CLASS (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PAGE_VIEWER))

typedef struct _GtkPageViewer       GtkPageViewer;
typedef struct _GtkPageViewerClass  GtkPageViewerClass;

struct _GtkPageViewer {
  GtkBin bin;
  SoGtkFullViewer* viewer;
  GtkWidget* popupMenu;
  GdkGC* rubberGC;
  bool rubberMove;
  int beginMoveX,beginMoveY;
  int endMoveX,endMoveY;
  int offsetMoveX,offsetMoveY;
  int wMove,hMove;
  int popx,popy;
};

struct _GtkPageViewerClass {
  GtkBinClass parent_class;
  void (*rectangular_collect)(GtkPageViewer*);
  void (*collect)(GtkPageViewer*);
  void (*popup)(GtkPageViewer*);
};

GtkType gtk_page_viewer_get_type(void);
GtkWidget* gtk_page_viewer_new(const char*);
}

SoNode* gtk_page_viewer_get_scene_graph(GtkPageViewer*);
SoGtkFullViewer* gtk_page_viewer_get_viewer(GtkPageViewer*); 
void gtk_page_viewer_add_popup_entry(GtkPageViewer*,const std::string&);

#endif
