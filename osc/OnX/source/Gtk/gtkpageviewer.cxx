// this :
#include <OnX/Gtk/gtkpageviewer.h>

#include <glib.h>
#include <gdk/gdkkeysyms.h>

#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>
#include <OnX/Inventor/GuiViewer.h>

#include <inlib/sprintf>

extern "C" {
static void gtk_page_viewer_class_init(GtkPageViewerClass*);
static void gtk_page_viewer_init(GtkPageViewer*);
static void gtk_page_viewer_realize(GtkWidget*);
static void gtk_page_viewer_destroy(GtkObject*);
static void gtk_page_viewer_size_allocate(GtkWidget*,GtkAllocation*);

static GtkBinClass* parent_class = 0;

static guint page_viewer_rectangular_collect_signal = 0;
static guint page_viewer_collect_signal = 0;
static guint page_viewer_popup_signal = 1;

static void gtk_page_viwer_popup_callback(GtkWidget*,gpointer);
static void draw_rectangle(GdkDrawable*,GdkGC*,gint,gint,gint,gint);
}

static SbBool eventCB(void*,GdkEvent*);

static void get_rectangle(int,int,int,int,int&,int&,unsigned int&,unsigned int&);

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

#ifndef GTK_CLASS_TYPE
#define GTK_CLASS_TYPE(object_class)  object_class->type
#endif

GtkType gtk_page_viewer_get_type() {
  static GType ttt_type = 0;
  if(!ttt_type) {
    static const GTypeInfo ttt_info = {
      sizeof (GtkPageViewerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc)gtk_page_viewer_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (GtkPageViewer),
      0,    /* n_preallocs */
      (GInstanceInitFunc)gtk_page_viewer_init
    };
    ttt_type = g_type_register_static(gtk_bin_get_type(),
                                      "GtkPageViewer",&ttt_info,(GTypeFlags)0);
  }
  return ttt_type;
}
static void gtk_page_viewer_class_init(GtkPageViewerClass* aClass) {
  parent_class = (GtkBinClass*)gtk_type_class(GTK_TYPE_BIN);

  GtkObjectClass* object_class = (GtkObjectClass*)aClass;
  object_class->destroy = gtk_page_viewer_destroy;

  page_viewer_rectangular_collect_signal =  
    gtk_signal_new ("rectangular_collect",
                    (GtkSignalRunType)(GTK_RUN_LAST | GTK_RUN_ACTION),
                    GTK_CLASS_TYPE(object_class),
                    GTK_SIGNAL_OFFSET(GtkPageViewerClass,rectangular_collect),
                    gtk_marshal_NONE__POINTER,
                    GTK_TYPE_NONE,1,GTK_TYPE_POINTER);

  page_viewer_collect_signal =  
    gtk_signal_new ("collect",
                    (GtkSignalRunType)(GTK_RUN_LAST | GTK_RUN_ACTION),
                    GTK_CLASS_TYPE(object_class),
                    GTK_SIGNAL_OFFSET(GtkPageViewerClass,collect),
                    gtk_marshal_NONE__POINTER,
                    GTK_TYPE_NONE,1,GTK_TYPE_POINTER);

  page_viewer_popup_signal =  
    gtk_signal_new ("popup",
                    (GtkSignalRunType)(GTK_RUN_LAST | GTK_RUN_ACTION),
                    GTK_CLASS_TYPE(object_class),
                    GTK_SIGNAL_OFFSET(GtkPageViewerClass,popup),
                    gtk_marshal_NONE__POINTER,
                    GTK_TYPE_NONE,1,GTK_TYPE_POINTER);

  GtkWidgetClass* widget_class = (GtkWidgetClass*)aClass;
  widget_class->size_allocate = gtk_page_viewer_size_allocate;
  widget_class->realize = gtk_page_viewer_realize;

  aClass->rectangular_collect = 0;
  aClass->collect = 0;
}

class GtkPageViewerCollectCallback : public SbCollectCallback {
public: //SbCollectCallback
  virtual void execute(const SoCanvas&,const SoNodeList& aValue) {
    std::string value;
    int number = aValue.getLength();
    for(int index=0;index<number;index++) {
      if(index) value += "\n";
      value += std::string(aValue[index]->getName().getString());      
    }
    GtkCallbackStruct data;
    data.value = g_strdup(value.c_str());
    data.x = 0;
    data.y = 0;
    gtk_signal_emit(GTK_OBJECT(fPageViewer),page_viewer_collect_signal,&data);
    g_free(data.value);
  }
public:
  GtkPageViewerCollectCallback(GtkPageViewer* aPageViewer)
  :fPageViewer(aPageViewer){}
private:
  GtkPageViewer* fPageViewer;
};

static void gtk_page_viewer_init(GtkPageViewer* aPageViewer) {
  GTK_WIDGET_SET_FLAGS (aPageViewer, GTK_NO_WINDOW);

  // Create a "pick mode" popup menu :
  aPageViewer->popupMenu = gtk_menu_new();
  aPageViewer->rubberGC = 0;
  aPageViewer->rubberMove = false;
  aPageViewer->beginMoveX = 0;
  aPageViewer->beginMoveY = 0;
  aPageViewer->endMoveX = 0;
  aPageViewer->endMoveY = 0;
  aPageViewer->offsetMoveX = 0;
  aPageViewer->offsetMoveY = 0;
  aPageViewer->wMove = 0;
  aPageViewer->hMove = 0;
  aPageViewer->viewer = 0;
}
static void gtk_page_viewer_realize(GtkWidget* aWidget) {
  g_return_if_fail(aWidget != 0);
  g_return_if_fail(GTK_IS_PAGE_VIEWER(aWidget));

  if (GTK_WIDGET_CLASS(parent_class)->realize)
    (*GTK_WIDGET_CLASS(parent_class)->realize)(aWidget);

  GtkPageViewer* pageViewer = GTK_PAGE_VIEWER(aWidget);

  GdkGCValues values;
  values.foreground = aWidget->style->white;
  values.function = GDK_XOR;
  values.subwindow_mode = GDK_INCLUDE_INFERIORS;
  pageViewer->rubberGC = 
    gdk_gc_new_with_values(aWidget->window,
                   &values,
                   (GdkGCValuesMask)(GDK_GC_FOREGROUND
                                     |GDK_GC_FUNCTION|GDK_GC_SUBWINDOW));
}
static void gtk_page_viewer_destroy(GtkObject* aObject) {
  g_return_if_fail (aObject != 0);
  g_return_if_fail (GTK_IS_PAGE_VIEWER(aObject));
  
  GtkPageViewer* pageViewer = GTK_PAGE_VIEWER(aObject);
  if(pageViewer->viewer) {
    GuiViewer_finalize<SoGtkViewer>(*(pageViewer->viewer));
    if(pageViewer->viewer->isOfType(SoGtkExaminerViewer::getClassTypeId())) {
      delete (SoGtkExaminerViewer*)pageViewer->viewer;
    } else if(pageViewer->viewer->isOfType
                (SoGtkPlaneViewer::getClassTypeId())) {
      delete (SoGtkPlaneViewer*)pageViewer->viewer;
    } else {
      SoDebugError::postInfo
        ("gtk_page_viewer_destroy","unknown viewer class");
    }
    pageViewer->viewer = 0;
  }

  if(pageViewer->rubberGC) {
    gdk_gc_destroy(pageViewer->rubberGC);
    pageViewer->rubberGC = 0;
  }


  if(pageViewer->popupMenu){
    gtk_widget_destroy(pageViewer->popupMenu);
    pageViewer->popupMenu = 0;
  }

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    (*GTK_OBJECT_CLASS(parent_class)->destroy)(aObject);
}
GtkWidget* gtk_page_viewer_new(const char* aViewerClass) {
  GtkPageViewer* pageViewer;

  pageViewer = (GtkPageViewer*)gtk_type_new(gtk_page_viewer_get_type());

  // The SoGtkFullViewer::buildWidget does a gtk_container_add.
  if(aViewerClass && !strcmp(aViewerClass,"Plane")) {
    pageViewer->viewer = 
      new SoGtkPlaneViewer
        (GTK_WIDGET(pageViewer),"PAGEVIEWER_VIEWER",TRUE);
  } else {
    pageViewer->viewer = 
      new SoGtkExaminerViewer
        (GTK_WIDGET(pageViewer),"PAGEVIEWER_VIEWER",TRUE);
  }
  GuiViewer_initialize<SoGtkViewer>(*(pageViewer->viewer));

  // Give an initial size (else problem with SoAnyThumbWheel).        
  GtkWidget* child = pageViewer->viewer->baseWidget();
  gtk_widget_show(child);
  GtkAllocation alloc;
  alloc.x = 0;
  alloc.y = 0;
  alloc.width = 500;
  alloc.height = 500;
  gtk_widget_size_allocate(child,&alloc);

  SoPage* soPage = GuiViewer_SoPage<SoGtkViewer>(*(pageViewer->viewer));
  if(soPage) {
    soPage->setCollectCallback(new GtkPageViewerCollectCallback(pageViewer));
  }

  pageViewer->viewer->setEventCallback(eventCB,pageViewer);

  return GTK_WIDGET(pageViewer);
}
static void gtk_page_viewer_size_allocate(GtkWidget * widget,GtkAllocation * allocation) {
  g_return_if_fail(widget != 0);
  g_return_if_fail(GTK_IS_PAGE_VIEWER(widget));
  g_return_if_fail(allocation != 0);

  widget->allocation = *allocation;

  GtkPageViewer* pageViewer = GTK_PAGE_VIEWER(widget);
  if(!pageViewer->viewer) return;
  pageViewer->viewer->setSize(SbVec2s(allocation->width,allocation->height));

  GtkWidget* child = pageViewer->viewer->baseWidget();
  GtkAllocation child_allocation;
  child_allocation.x = allocation->x;
  child_allocation.y = allocation->y;
  child_allocation.width = allocation->width;
  child_allocation.height = allocation->height;
  gtk_widget_size_allocate(child,&child_allocation);
}
SoNode* gtk_page_viewer_get_scene_graph(GtkPageViewer* aPageViewer) {
  if(!aPageViewer->viewer) return 0;
  return aPageViewer->viewer->getSceneGraph();
}
SoGtkFullViewer* gtk_page_viewer_get_viewer(GtkPageViewer* aPageViewer) {
  return aPageViewer->viewer;
}

SbBool eventCB(void* aTag,GdkEvent* aEvent){
  GtkPageViewer* This = (GtkPageViewer*)aTag;
  if(!This->viewer) return FALSE;
  GtkWidget* widget = (GtkWidget*)aTag;

  if(aEvent->type==GDK_BUTTON_PRESS) {
    GdkEventButton* event = (GdkEventButton*)aEvent; 
    if(event->button==3) {
      if(This->viewer->isViewing()==TRUE) return FALSE;
      if(!This->popupMenu) return FALSE;
      gtk_menu_popup(GTK_MENU(This->popupMenu),0,0,0,0,
                     event->button, event->time);

      // event->window is not widget->window !
      GtkWidget* normalWidget = This->viewer->getNormalWidget();
      int ww,wh;
      gdk_window_get_size(normalWidget->window,&ww,&wh);

      This->popx = (int)event->x;
      This->popy = wh - (int)event->y; //GL coordinates.
      return FALSE; //So that SoPage::mouseEventCB is treated.
    } else if(event->button==1) {
      if( ((event->state & GDK_SHIFT_MASK)==GDK_SHIFT_MASK) &&
          ((event->state & GDK_CONTROL_MASK)==GDK_CONTROL_MASK)) {
        This->rubberMove = true;
  
        // event->window is not widget->window !
        GtkWidget* normalWidget = This->viewer->getNormalWidget();
        int nx,ny;
        gdk_window_get_origin(normalWidget->window,&nx,&ny);
        int x,y;
        gdk_window_get_origin(widget->window,&x,&y);
        This->offsetMoveX = nx - x;
        This->offsetMoveY = ny - y;
  
        This->endMoveX = This->beginMoveX = (int)event->x + This->offsetMoveX;
        This->endMoveY = This->beginMoveY = (int)event->y + This->offsetMoveY;
  
        draw_rectangle(widget->window,This->rubberGC,
                       This->beginMoveX,This->beginMoveY,
                       This->endMoveX,This->endMoveY);
  
        gdk_window_get_size(event->window,&(This->wMove),&(This->hMove));
  
        return TRUE;
      } else {
        // Simple pick :
        guint sigid = g_signal_lookup("collect",G_OBJECT_TYPE(This));
        if(g_signal_handler_find(GTK_OBJECT(This),
                                 G_SIGNAL_MATCH_ID,sigid,0,0,0,0)) {
        } else {
          SoPage* soPage = GuiViewer_SoPage<SoGtkViewer>(*(This->viewer));
          if(soPage) soPage->setHandlePickedPoint(FALSE);
        }
        return FALSE; //And then let the viewer treats also the event.
      }
    }
  } else if(aEvent->type==GDK_BUTTON_RELEASE) {
    GdkEventButton* event = (GdkEventButton*)aEvent; 
    if(event->button==1) {
      if(This->rubberMove) {
        This->rubberMove = false;

        // Erase previous rectangle :
        draw_rectangle(widget->window,This->rubberGC,
                       This->beginMoveX,This->beginMoveY,
                       This->endMoveX,This->endMoveY);

        int x,y;
        unsigned int w,h;
        get_rectangle(This->beginMoveX - This->offsetMoveX,
                      This->beginMoveY - This->offsetMoveY,
                      This->endMoveX - This->offsetMoveX,
                      This->endMoveY - This->offsetMoveY,
                      x,y,w,h);

        // We pass in GL coordinate (origin is bottom-left, y toup) :
        int xgl = x;
        int ygl = This->hMove - (y+h);

        if(This->viewer->isViewing()==TRUE) {
          GuiViewer_map<SoGtkViewer>(*(This->viewer),
                            SbVec2s((short)xgl,(short)ygl),
                            SbVec2s((short)w,(short)h),
                            SbVec2s((short)This->wMove,(short)This->hMove));
        } else {

          GtkCallbackStruct data;
          std::string value;
          inlib::sprintf(value,132,"%d %d %d %d",xgl,ygl,w,h);
          data.value = g_strdup(value.c_str());
          data.x = 0;
          data.y = 0;
          gtk_signal_emit(GTK_OBJECT(This),
                          page_viewer_rectangular_collect_signal,&data);
          g_free(data.value);

        }
        
        return TRUE;
      }
    }
  } else if(aEvent->type==GDK_MOTION_NOTIFY) {
    GdkEventMotion* event = (GdkEventMotion*)aEvent; 
    if((event->state & GDK_BUTTON1_MASK)!=GDK_BUTTON1_MASK) return FALSE;

    if( ((event->state & GDK_SHIFT_MASK)==GDK_SHIFT_MASK) &&
        ((event->state & GDK_CONTROL_MASK)==GDK_CONTROL_MASK)) {

      if(This->rubberMove) {
  
        if( (event->x<0) || (event->y<0) 
            || (event->x>=This->wMove) || (event->y>=This->hMove) ){
          // Out of the window :
          return TRUE;
        }
  
        // Erase previous rectangle :
        draw_rectangle(widget->window,This->rubberGC,
                       This->beginMoveX,This->beginMoveY,
                       This->endMoveX,This->endMoveY);
        
        This->endMoveX = (int)event->x + This->offsetMoveX;
        This->endMoveY = (int)event->y + This->offsetMoveY;
        
        draw_rectangle(widget->window,This->rubberGC,
                       This->beginMoveX,This->beginMoveY,
                       This->endMoveX,This->endMoveY);
        
        return TRUE;
      }
    } else {
      //Picking mode : not discarded. We can be in resize/move region.
      if(This->viewer->isViewing()==FALSE) return FALSE; //Picking mode.
      GtkWidget* normalWidget = This->viewer->getNormalWidget();
      int ww,wh;
      gdk_window_get_size(normalWidget->window,&ww,&wh);
      int px = (int)event->x;
      int py = wh - (int)event->y;
      // Discard mouse move event not in current region in viewing mode.
      if(!GuiViewer_isInCurrentRegion<SoGtkViewer>
            (*(This->viewer),SbVec2s(px,py))) return TRUE;
    }

  } else if(aEvent->type==GDK_KEY_PRESS) {
    if(This->viewer->isViewing()!=TRUE) return FALSE;
    GdkEventKey* event = (GdkEventKey*)aEvent; 
    if(event->keyval==GDK_BackSpace) {
      GuiViewer_popCamera<SoGtkViewer>(*(This->viewer));
      return TRUE;
    }
  }
  return FALSE;
}
void gtk_page_viewer_add_popup_entry(GtkPageViewer* aPageViewer,const std::string& aLabel){
  if(!aPageViewer->popupMenu) return;
  GtkWidget* menu_items = gtk_menu_item_new_with_label(aLabel.c_str());
  gtk_widget_set_name (menu_items,aLabel.c_str());
  gtk_menu_append(GTK_MENU(aPageViewer->popupMenu), menu_items);
  gtk_signal_connect (GTK_OBJECT(menu_items), 
                      "activate",
                      GTK_SIGNAL_FUNC(gtk_page_viwer_popup_callback), 
                      (gpointer)aPageViewer);
  gtk_widget_show(menu_items);
}
void gtk_page_viwer_popup_callback(GtkWidget* aWidget,gpointer aTag){
  GtkPageViewer* This = (GtkPageViewer*)aTag;
  std::string value = gtk_widget_get_name(aWidget);
  GtkCallbackStruct data;
  data.value = g_strdup(value.c_str());
  data.x = This->popx;
  data.y = This->popy;
  gtk_signal_emit(GTK_OBJECT(This),page_viewer_popup_signal,&data);
  g_free(data.value);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void draw_rectangle( GdkDrawable* aDrawable,GdkGC* aGC,gint aBeginX,gint aBeginY,gint aEndX,gint aEndY){
  gdk_draw_line(aDrawable,aGC,aBeginX,aBeginY,aBeginX,aEndY);
  gdk_draw_line(aDrawable,aGC,aBeginX,aEndY  ,aEndX,aEndY);
  gdk_draw_line(aDrawable,aGC,aEndX,aEndY    ,aEndX,aBeginY);
  gdk_draw_line(aDrawable,aGC,aEndX,aBeginY  ,aBeginX,aBeginY);
}
void get_rectangle(int aBeginX,int aBeginY,int aEndX,int aEndY,int& aX,int& aY,unsigned int& aWidth,unsigned int& aHeight){
  aX = MINIMUM(aBeginX,aEndX);
  aY = MINIMUM(aBeginY,aEndY);
  aWidth = MAXIMUM(aBeginX,aEndX) - aX;
  aHeight = MAXIMUM(aBeginY,aEndY) - aY;
}
