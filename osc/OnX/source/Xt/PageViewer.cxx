
// this :
#include "PageViewerP.h"

// Private :
#include <X11/StringDefs.h>
#include <X11/keysym.h>

// For popup menu :
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>

#include <inlib/sprintf>

#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <OnX/Inventor/GuiViewer.h>

#include <OnX/Xt/XtTools.h>

static void InitializeClass(void);
static void InitializeWidget(Widget, Widget,ArgList,Cardinal*);
static void RealizeWidget(Widget,XtValueMask*,XSetWindowAttributes*);
static void DestroyWidget(Widget);
static void PopupCallback(Widget,XtPointer,XtPointer);

static SbBool eventCB(void*,XAnyEvent*);
static void GetRectangle(int,int,int,int,int&,int&,unsigned int&,unsigned int&);

static XtResource resources [] ={
 {(char*)XtNrectangularCollectCallback,(char*)XtCCallback,(char*)XtRCallback,
  sizeof(XtCallbackList),
  XtOffset(PageViewerWidget,pageViewer.rectangularCollectCallback),
  XtRImmediate,(XtPointer)NULL},
 {(char*)XtNcollectCallback,(char*)XtCCallback,(char*)XtRCallback,
  sizeof(XtCallbackList),
  XtOffset(PageViewerWidget,pageViewer.collectCallback),
  XtRImmediate,(XtPointer)NULL},
 {(char*)XtNpopupCallback,(char*)XtCCallback,(char*)XtRCallback,
  sizeof(XtCallbackList),
  XtOffset(PageViewerWidget,pageViewer.popupCallback),
  XtRImmediate,(XtPointer)NULL},
 {(char*)XtNviewerClass,(char*)"ViewerClass",XtRString,
  sizeof(String),
  XtOffset(PageViewerWidget,pageViewer.viewerClass),
  XtRString,(XtPointer)"Examiner"}
};

#define athis ((PageViewerWidget)This)->pageViewer
#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

PageViewerClassRec pageViewerClassRec = {
/* Core Class Part */
{
    (WidgetClass)&xmFormClassRec, /* pointer to superclass ClassRec   */
    (char*)"PageViewer",             /* widget resource class name       */
    sizeof(PageViewerRec),             /* size in bytes of widget record   */
    InitializeClass,            /* class_initialize                 */
    NULL,                         /* dynamic initialization           */
    FALSE,                       /* has class been initialized?      */
    InitializeWidget,           /* initialize                       */
    NULL,                        /* notify that initialize called    */
    RealizeWidget,              /* XCreateWindow for widget         */
    NULL,                              /* widget semantics name to proc mapWidget*/
    0,                          /* number of entries in actions     */
    resources,                  /* resources for subclass fields    */
    XtNumber(resources),        /* number of entries in resources   */
    NULLQUARK,                        /* resource class quarkified        */
    TRUE,                        /* compress MotionNotify for widget */
    TRUE,                        /* compress Expose events for widget*/
    TRUE,                        /* compress enter and leave events  */
    TRUE,                        /* select for VisibilityNotify      */
    DestroyWidget,                /* free data for subclass pointers  */
    XtInheritResize,            /* geom manager changed widget size */
    XtInheritExpose,                        /* rediplay window                  */
    NULL,                        /* set subclass resource values     */
    NULL,                        /* notify that SetValues called     */
    XtInheritSetValuesAlmost,   /* SetValues got "Almost" geo reply */
    NULL,                        /* notify that get_values called    */
    XtInheritAcceptFocus,       /* assign input focus to widget     */
    XtVersion,                        /* version of intrinsics used       */
    NULL,                        /* list of callback offsets         */
    XtInheritTranslations,      /* translations                     */        
    XtInheritQueryGeometry,     /* return preferred geometry        */
    XtInheritDisplayAccelerator,/* display your accelerator         */
    NULL                         /* pointer to extension record      */
},
/* Composite Class Part */
{
    XtInheritGeometryManager,   /* geometry manager for children   */
    XtInheritChangeManaged,     /* change managed state of child   */
    XtInheritInsertChild,       /* physically add child to parent  */
    XtInheritDeleteChild,       /* physically remove child         */
    NULL                         /* pointer to extension record     */
},
/* constraint_class fields */
{
    NULL,                         /* constraint resource     */
    0,                            /* number of constraints   */
    sizeof(XmFormConstraintRec),  /* constraint size         */
    NULL,                         /* initialization          */
    NULL,                         /* destroy proc            */
    NULL,                         /* set_values proc         */
    NULL,                         /* extension               */
},
/* manager_class fields   */
{                        
    XtInheritTranslations,                /* translations           */
    NULL,                                 /* syn_resources          */
    0,                                    /* num_syn_resources      */
    NULL,                                 /* syn_cont_resources     */
    0,                                    /* num_syn_cont_resources */
    XmInheritParentProcess,               /* parent_process         */
    NULL,                                 /* extension              */
},
/* bulletin_board_class fields */
{                        
    FALSE,                                /* always_install_accelerators */
    NULL,                                 /* geo_matrix_create  */
    XmInheritFocusMovedProc,              /* focus_moved_proc   */
    NULL,                                 /* extension          */
},
/* form_class fields  */
{   
    NULL,                     /* extension          */
},
/* PageViewer */
{
   NULL
}
   
};

WidgetClass pageViewerWidgetClass = (WidgetClass) &pageViewerClassRec;

class PageViewerCollectCallback : public virtual SbCollectCallback {
public: //SbCollectCallback
  virtual void execute(const SoCanvas&,const SoNodeList& aValue) {
    std::string s;
    int number = aValue.getLength();
    for(int index=0;index<number;index++) {
      if(index) s += "\n";
      s += std::string(aValue[index]->getName().getString());      
    }
    PageViewerCallbackStruct data;
    data.reason = XtCR_COLLECT;
    data.event = 0; //FIXME
    data.value = XtNewString(s.c_str());
    XtCallCallbacks(fWidget,XtNcollectCallback,(XtPointer)&data);
    XtFree(data.value);
  }
public:
  PageViewerCollectCallback(Widget aWidget):fWidget(aWidget){}
private:
  Widget fWidget;
};

//////////////////////////////////////////////////////////////////////////////
static void InitializeClass (
 void
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
static void destroy_viewer(Widget widget,XtPointer closure,XtPointer) {
  Widget This = (Widget)closure;
  PageViewerDisable(This);
}
//////////////////////////////////////////////////////////////////////////////
static void InitializeWidget (
 Widget aRequest
,Widget This
,ArgList aArgs
,Cardinal* aArgn
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aRequest->core.width<=0)  This->core.width  = 500;
  if(aRequest->core.height<=0) This->core.height = 500;

  athis.rubberGC = NULL;
  athis.rubberMove = false;
  athis.beginMove.x = 0;
  athis.beginMove.y = 0;
  athis.endMove.x = 0;
  athis.endMove.y = 0;
  athis.wMove = 0;
  athis.hMove = 0;
  athis.popx = 0;
  athis.popy = 0;

  athis.viewerClass = (char*)XtNewString(athis.viewerClass);
  for(int argi=0;argi<*aArgn;argi++) {
    if(!strcmp(aArgs[argi].name,XtNviewerClass)) {
      XtFree(athis.viewerClass);
      athis.viewerClass = (char*)XtNewString((char*)aArgs[argi].value);
    }
  }

  if(!strcmp(athis.viewerClass,"Plane")) {
    athis.viewer = new SoXtPlaneViewer(This,"PAGEVIEWER_VIEWER",TRUE);
  } else {
    athis.viewer = new SoXtExaminerViewer(This,"PAGEVIEWER_VIEWER",TRUE);
  }
  GuiViewer_initialize<SoXtViewer>(*athis.viewer);

  XtAddCallback(athis.viewer->getBaseWidget(),
                XmNdestroyCallback,
                destroy_viewer,This);

  athis.viewer->setSize(SbVec2s(This->core.width,This->core.height));
  athis.viewer->show(); // Will do the XtManageChild.

  Arg args[4];
  XtSetArg(args[0],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(args[1],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(args[3],XmNrightAttachment,XmATTACH_FORM);
  Widget widget = athis.viewer->getWidget();
  XtSetValues(widget,args,4);

  // CollectCallback object deleted by the SoPage.
  SoPage* soPage = GuiViewer_SoPage<SoXtViewer>(*athis.viewer);
  if(soPage) {
    soPage->setCollectCallback(new PageViewerCollectCallback(This));
  }

#ifdef LESSTIF_VERSION
  //FIXME : CYGWIN : problem with LessTif and the XmCreatePopupMenu.
  printf("PageViewer::InitializeWidget : pick mode popupmenu disconnected. Problem when using LessTif\n");
#else
  // Create a "pick mode" popup menu :
  XtSetArg(args[0],XmNwhichButton,Button3);
  //FIXME : parent should be single, double wins.
  Widget parent = athis.viewer->getNormalWidget(); 
  athis.popup = XmCreatePopupMenu(parent,(char*)"PAGEVIEWER_POPUP",args,1);
#endif

  athis.viewer->setEventCallback(eventCB,This);
}
//////////////////////////////////////////////////////////////////////////////
static void RealizeWidget (
 Widget This
,XtValueMask* aMask
,XSetWindowAttributes* aAtbs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(pageViewerWidgetClass->core_class.superclass->core_class.realize) {
    (pageViewerWidgetClass->core_class.superclass->core_class.realize)
      (This,aMask,aAtbs);
  }
  athis.rubberGC = XGetRubberGC(XtDisplay(This),XtWindow(This));
}
//////////////////////////////////////////////////////////////////////////////
static void DestroyWidget (
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(athis.rubberGC) XFreeGC(XtDisplay(This),athis.rubberGC);
  XtFree(athis.viewerClass);

  /* athis.viewer not deleted here. */

  //XtDestroyWidget(athis.popup); //Done by Xt.
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void PageViewerDisable(
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!athis.viewer) return;
  GuiViewer_finalize<SoXtViewer>(*athis.viewer);
  if(athis.viewer->isOfType(SoXtExaminerViewer::getClassTypeId())) {
    delete (SoXtExaminerViewer*)athis.viewer;
  } else if(athis.viewer->isOfType(SoXtPlaneViewer::getClassTypeId())) {
    delete (SoXtPlaneViewer*)athis.viewer;
  } else {
    SoDebugError::postInfo("PageViewerDisable","unknown viewer class");
  }
  athis.viewer = 0;
}
//////////////////////////////////////////////////////////////////////////////
SoPage* PageViewerGetSoPage(
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!athis.viewer) return 0;
  SoNode* node = athis.viewer->getSceneGraph();
  if(!node) return 0;        
  if(node->isOfType(SoPage::getClassTypeId())==FALSE) return 0;
  return ((SoPage*)node);
}
//////////////////////////////////////////////////////////////////////////////
SoNode* PageViewerGetSceneGraph(
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!athis.viewer) return 0;
  return athis.viewer->getSceneGraph();
}
//////////////////////////////////////////////////////////////////////////////
SoXtFullViewer* PageViewerGetSoViewer(
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return athis.viewer;
}
//////////////////////////////////////////////////////////////////////////////
void PageViewerSetVerboseLevel(
 Widget This
,int aLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SoPage* page = PageViewerGetSoPage(This);
  if(!page) return;
  page->verbose.setValue(aLevel?TRUE:FALSE);
}
//////////////////////////////////////////////////////////////////////////////
void PageViewerAddPopupEntry (
 Widget This
,const std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!athis.popup) return;
  Widget widget = XmCreatePushButton(athis.popup,(char*)aLabel.c_str(),0,0);
  XmString cps = 
    XmStringLtoRCreate((char*)aLabel.c_str(),XmSTRING_DEFAULT_CHARSET);
  Arg args[1];
  XtSetArg(args[0],XmNlabelString,cps);
  XtSetValues(widget,args,1);
  XmStringFree(cps);
  XtAddCallback(widget,XmNactivateCallback,PopupCallback,(XtPointer)This);
  if(widget) XtManageChild(widget);
}
//////////////////////////////////////////////////////////////////////////////
void PopupCallback(
 Widget aWidget
,XtPointer aTag
,XtPointer aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Widget This = (Widget)aTag;

  XButtonEvent* event = (XButtonEvent*)((XmAnyCallbackStruct*)aData)->event;
  event->x = athis.popx;
  event->y = athis.popy;

  std::string s;
  XmGetString(aWidget,XmNlabelString,s);

  PageViewerCallbackStruct data;
  data.reason = XtCR_POPUP;
  data.event = (XEvent*)event;
  data.value = XtNewString(s.c_str());
  XtCallCallbacks(This,XtNpopupCallback,(XtPointer)&data);
  XtFree(data.value);
}
//////////////////////////////////////////////////////////////////////////////
SbBool eventCB(
 void* aTag
,XAnyEvent* aEvent
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Widget This = (Widget)aTag;

  if(aEvent->type==ButtonPress) {
    XButtonEvent* event = (XButtonEvent*)aEvent;
    if(event->button==Button1) {
      if( ((event->state & ShiftMask) == ShiftMask) &&
          ((event->state & ControlMask) == ControlMask) ) {

        athis.rubberMove = true;

        // The event->window is not the XtWindow(This) ! 
        // (If decoration are "on", the render area window
        //  is smaller than the whole examiner viewer area).
        int px,py;
        Window child;
        XTranslateCoordinates(XtDisplay(This),
                              event->window,
                              XtWindow(This),
                              event->x,event->y,&px,&py,&child);
  
        athis.endMove.x  = athis.beginMove.x  = px;
        athis.endMove.y  = athis.beginMove.y  = py;

        XDrawRectangle(XtDisplay(This),XtWindow(This),athis.rubberGC,
                       athis.beginMove,athis.endMove);

        Window root;
        int cx,cy;
        unsigned int border,depth;
        XGetGeometry(XtDisplay(This),event->window,&root,
                     &cx,&cy,&athis.wMove,&athis.hMove,&border,&depth);
  
        //return FALSE; //Induces a redraw and then problems...
        return TRUE;

      } else {
        // Simple pick :
        if(XtHasCallbacks(This,XtNcollectCallback)==XtCallbackHasSome) {
        } else {
          SoPage* soPage = GuiViewer_SoPage<SoXtViewer>(*athis.viewer);
          if(soPage) soPage->setHandlePickedPoint(FALSE);
        }
        return FALSE; //And then let the viewer treats also the event.
      }

    } else if(event->button==Button3) {
      if(athis.viewer->isViewing()==TRUE) return FALSE;
      if(!athis.popup) return FALSE;

      Window root;
      int cx,cy;
      unsigned int w,h,border,depth;
      XGetGeometry(XtDisplay(This),event->window,&root,
                   &cx,&cy,&w,&h,&border,&depth);

      athis.popx = event->x;
      athis.popy = h - event->y;

      XmMenuPosition(athis.popup,(XButtonPressedEvent*)aEvent);
      XtManageChild(athis.popup);
      return FALSE; //So that SoPage::mouseEventCB is treated.
    }
  } else if(aEvent->type==ButtonRelease) {
    XButtonEvent* event = (XButtonEvent*)aEvent;
    if(event->button==Button1) {
      if(athis.rubberMove) {
        athis.rubberMove = false;

        XDrawRectangle(XtDisplay(This),XtWindow(This),athis.rubberGC,
                       athis.beginMove,athis.endMove);

        // We translate to event->window coordinate :
        int bx,by;
        int ex,ey;
        Window child;
        XTranslateCoordinates(XtDisplay(This),
                              XtWindow(This),
                              event->window,
                              athis.beginMove.x,athis.beginMove.y,
                              &bx,&by,&child);
        XTranslateCoordinates(XtDisplay(This),
                              XtWindow(This),
                              event->window,
                              athis.endMove.x,athis.endMove.y,
                              &ex,&ey,&child);

        int x,y;
        unsigned int w,h;
        GetRectangle(bx,by,ex,ey,x,y,w,h);

        // We pass in GL coordinate (origin is bottom-left, y toup) :
        int xgl = x;
        int ygl = athis.hMove - (y+h);

        if(athis.viewer->isViewing()==TRUE) {
          GuiViewer_map<SoXtViewer>(*athis.viewer,
                            SbVec2s((short)xgl,(short)ygl),
                            SbVec2s((short)w,(short)h),
                            SbVec2s((short)athis.wMove,(short)athis.hMove));
        } else {

          std::string s;
          inlib::sprintf(s,132,"%d %d %d %d",xgl,ygl,w,h);

          PageViewerCallbackStruct data;
          data.reason = XtCR_RECTANGULAR_COLLECT;
          data.event = (XEvent*)event;
          data.value = XtNewString(s.c_str());
          XtCallCallbacks(This,XtNrectangularCollectCallback,(XtPointer)&data);
          XtFree(data.value);
        }

        return TRUE;
      }
    }
  } else if(aEvent->type == MotionNotify) {

    XMotionEvent* event = (XMotionEvent*)aEvent;
    if((event->state & Button1Mask) == Button1Mask) {
      if( ((event->state & ShiftMask) == ShiftMask) &&
          ((event->state & ControlMask) == ControlMask) ) {

        if(athis.rubberMove) {
        
          if( (event->x<0) || (event->y<0) || 
              (event->x>=(int)athis.wMove) || (event->y>=(int)athis.hMove) ){
            // Out of the window 
            return TRUE;
          }

          // Erase previous box by redrawing it !!! 
          // (in mode COMP this is equivallent to a deletion )
          XDrawRectangle(XtDisplay(This),XtWindow(This),athis.rubberGC,
                         athis.beginMove,athis.endMove);
          int px,py;
          Window child;
          XTranslateCoordinates(XtDisplay(This),
                                event->window,
                                XtWindow(This),
                                event->x,event->y,&px,&py,&child);
        
          athis.endMove.x = px;
          athis.endMove.y = py;
          XDrawRectangle(XtDisplay(This),XtWindow(This),athis.rubberGC,
                         athis.beginMove,athis.endMove);
          return TRUE;
        }
      } else {
        //Picking mode : not discarded. We can be in resize/move region.
        if(athis.viewer->isViewing()==FALSE) return FALSE; //Picking mode.
        // Viewing mode :
        //GL coordinates.
        Window root;
        int cx,cy;
        unsigned int w,h,b,d;
        XGetGeometry(XtDisplay(This),event->window,&root,&cx,&cy,&w,&h,&b,&d);
        int px = event->x;
        int py = h - event->y;
        // Discard mouse move event not in current region in viewing mode.
        if(!GuiViewer_isInCurrentRegion<SoXtViewer>
              (*athis.viewer,SbVec2s(px,py))) return TRUE;
      }

    }
  } else if(aEvent->type == KeyPress) {
    if(athis.viewer->isViewing()!=TRUE) return FALSE;
    XKeyEvent* event = (XKeyEvent*)aEvent;
    KeySym keySym;
    XLookupString(event,NULL,0,&keySym,NULL);
    if(keySym==XK_BackSpace) {
      GuiViewer_popCamera<SoXtViewer>(*athis.viewer);
      return TRUE;
    }
  }
  return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
void GetRectangle(
 int aBeginX
,int aBeginY
,int aEndX
,int aEndY
,int& aX
,int& aY
,unsigned int& aWidth
,unsigned int& aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aX = MINIMUM(aBeginX,aEndX);
  aY = MINIMUM(aBeginY,aEndY);
  aWidth = MAXIMUM(aBeginX,aEndX) - aX;
  aHeight = MAXIMUM(aBeginY,aEndY) - aY;
}
