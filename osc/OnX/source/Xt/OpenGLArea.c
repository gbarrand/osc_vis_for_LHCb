/*
#define DEBUG
#undef HAS_GL
*/
#define HAS_GL

/*this*/
#include "OpenGLAreaP.h"

#include <X11/StringDefs.h>

#ifdef HAS_GL
#include <GL/glx.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif
static void InitializeClass(void);
static void InitializeWidget(Widget, Widget,ArgList,Cardinal*);
static void RealizeWidget(Widget, XtValueMask*, XSetWindowAttributes*);
static void DestroyWidget(Widget);
static void ChangeWidgetSize(Widget);
static void DrawWidget(Widget, XEvent*, Region);
static Boolean SetValues(Widget,Widget,Widget,ArgList,Cardinal *);

static void EventHandler(Widget,XtPointer,XEvent*,Boolean*);
static int MakeCurrent(Widget);
static void XWidgetInstallColormap(Widget);
static void XWidgetUninstallColormap(Widget);
static Widget XWidgetGetShell(Widget);
#ifdef __cplusplus
}
#endif

#define athis ((OpenGLAreaWidget)This)->openGLArea
#define acur ((OpenGLAreaWidget)a_current)->openGLArea
#define CWarn printf
#define CWarnF printf

static struct {
  Display* gldisp;
#ifdef HAS_GL
  GLXContext glContext1;
  GLXContext glContext2;
#else
  void* glContext1;
  void* glContext2;
#endif /*HAS_GL*/
  int glDepth1,glDepth2;
  Visual* glVisual1;
  Visual* glVisual2;
  Colormap glColormap1;
  Colormap glColormap2;
  Boolean glInstall1;
  Boolean glInstall2;
} Class = {
NULL,
NULL,NULL,                     /*GLXContext*/
0,0,                           /*Depth*/
CopyFromParent,CopyFromParent, /*Visual*/
0L,0L,                         /*Colormap*/
False,False                    /*Install*/
};

static XtResource resources [] = {
  {XoNdoubleBufferOn,"DoubleBufferOn",XtRBoolean,sizeof(Boolean),
   XtOffset(OpenGLAreaWidget,openGLArea.doubleBufferOn),XtRImmediate,(XtPointer)True},
  {XoNpaintCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(OpenGLAreaWidget,openGLArea.paintCallback),XtRImmediate,(XtPointer)NULL},
  {XoNeventCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(OpenGLAreaWidget,openGLArea.eventCallback),XtRImmediate,(XtPointer)NULL}
};

OpenGLAreaClassRec  openGLAreaClassRec = {
/* Core Class Part */
{
   (WidgetClass) &compositeClassRec, /* pointer to superclass ClassRec   */
    "OpenGLArea",                 /* widget resource class name       */
    sizeof(OpenGLAreaRec),        /* size in bytes of widget record   */
    InitializeClass,            /* class_initialize                 */
    NULL,                         /* dynamic initialization           */
    FALSE,                       /* has class been initialized?      */
    InitializeWidget,           /* initialize                       */
    NULL,                        /* notify that initialize called    */
    RealizeWidget,              /* XCreateWindow for widget         */
    NULL,                          /* widget semantics name to proc mapWidget*/
    0,                          /* number of entries in actions     */
    resources,                  /* resources for subclass fields    */
    XtNumber(resources),        /* number of entries in resources   */
    NULLQUARK,                        /* resource class quarkified        */
    TRUE,                        /* compress MotionNotify for widget */
    TRUE,                        /* compress Expose events for widget*/
    TRUE,                        /* compress enter and leave events  */
    TRUE,                        /* select for VisibilityNotify      */
    DestroyWidget,                /* free data for subclass pointers  */
    ChangeWidgetSize,           /* geom manager changed widget size */
    DrawWidget,                        /* rediplay window                  */
    SetValues,                        /* set subclass resource values     */
    NULL,                        /* notify that SetValues called    */
    XtInheritSetValuesAlmost,   /* SetValues got "Almost" geo reply*/
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
/* OpenGLArea */
{
   NULL 
}
   
};

WidgetClass openGLAreaWidgetClass = (WidgetClass) &openGLAreaClassRec;
/***************************************************************************/
static void InitializeClass (
 void
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
}
/***************************************************************************/
static void InitializeWidget (
 Widget a_request
,Widget This
,ArgList a_args
,Cardinal* a_argn
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(a_request->core.width<=0)  This->core.width  = 100;
  if(a_request->core.height<=0) This->core.height = 100;

#ifdef DEBUG
  printf ("debug : OpenGLArea : InitializeWidget : %s\n",XtName(This));
#endif

  athis.initialDepth       = This->core.depth;
  athis.initialColormap    = This->core.colormap;
  athis.visual             = CopyFromParent;
  athis.installColormap    = False;

#ifdef HAS_GL
 {Display* display;
  int attributeList[11];
  XVisualInfo* vinfo;
  display = XtDisplay(This);

 {int error,event;
  if(glXQueryExtension(display,&error,&event)==0) {
      CWarn ("X server does not have OpenGL extensions.\n");
  }}

  if(Class.glContext1==NULL) {
    /*First context for RGBA without double buffer.*/
    attributeList[0] = GLX_RGBA;
    attributeList[1] = GLX_RED_SIZE;
    attributeList[2] = 1;
    attributeList[3] = GLX_GREEN_SIZE;
    attributeList[4] = 1;
    attributeList[5] = GLX_BLUE_SIZE;
    attributeList[6] = 1;
    attributeList[7] = GLX_DEPTH_SIZE;
    attributeList[8] = 1;
    attributeList[9] = None;
    vinfo = glXChooseVisual  (display,DefaultScreen(display),attributeList);
    if(vinfo==NULL) {
      /*Try something else.*/
      attributeList[0] = GLX_RGBA;
      attributeList[1] = GLX_RED_SIZE;
      attributeList[2] = 1;
      attributeList[3] = GLX_GREEN_SIZE;
      attributeList[4] = 1;
      attributeList[5] = GLX_BLUE_SIZE;
      attributeList[6] = 1;
      attributeList[7] = GLX_DOUBLEBUFFER;
      attributeList[8] = GLX_DEPTH_SIZE;
      attributeList[9] = 1;
      attributeList[10] = None;
      vinfo = glXChooseVisual  (display,DefaultScreen(display),attributeList);
      if(vinfo==NULL) {
        CWarn ("Can't choose a visual.\n");
        return;
      }
    }
    Class.glContext1  = glXCreateContext (display,vinfo,NULL,GL_FALSE);
    if(Class.glContext1==NULL) {
      CWarn ("Can't create a GLX context.\n");
    }
    if( (vinfo->depth ==DefaultDepth (display,DefaultScreen(display)))  &&
        (vinfo->visual==DefaultVisual(display,DefaultScreen(display))) ) {
      Class.glColormap1 = XDefaultColormap (display,DefaultScreen(display));
      Class.glInstall1  = False;
    } else {
      Class.glColormap1 = XCreateColormap  (display,XDefaultRootWindow(display),vinfo->visual, AllocNone); 
      Class.glInstall1  = True;
    }
    Class.glDepth1    = vinfo->depth;
    Class.glVisual1   = vinfo->visual;
    XFree             (vinfo);
    if(Class.glColormap1==0L) {
      CWarn ("Can't create a X colormap.\n");
    }
  }

  if(Class.glContext2==NULL) {
    /*Second context for RGBA with double buffer.*/
    attributeList[0] = GLX_RGBA;
    attributeList[1] = GLX_RED_SIZE;
    attributeList[2] = 1;
    attributeList[3] = GLX_GREEN_SIZE;
    attributeList[4] = 1;
    attributeList[5] = GLX_BLUE_SIZE;
    attributeList[6] = 1;
    attributeList[7] = GLX_DOUBLEBUFFER;
    attributeList[8] = GLX_DEPTH_SIZE;
    attributeList[9] = 1;
    attributeList[10] = None;
    vinfo = glXChooseVisual  (display,DefaultScreen(display),attributeList);
    if(vinfo==NULL) {
      CWarn ("Can't choose a visual for double buffering.\n");
      return;
    }
    Class.glContext2 = glXCreateContext (display,vinfo,NULL,GL_FALSE);
    if(Class.glContext2==NULL) {
      CWarn ("Can't create a GLX context.\n");
    }
    if( (vinfo->depth ==Class.glDepth1) &&
        (vinfo->visual==Class.glVisual1) ) {
      /*Can take same as first context.*/
      Class.glColormap2 = Class.glColormap1;
      Class.glInstall2  = Class.glInstall1;
      Class.glDepth2    = Class.glDepth1;
      Class.glVisual2   = Class.glVisual1;
    } else {
      if( (vinfo->depth ==DefaultDepth (display,DefaultScreen(display)))  &&
          (vinfo->visual==DefaultVisual(display,DefaultScreen(display))) ) {
        Class.glColormap2 = XDefaultColormap (display,DefaultScreen(display));
        Class.glInstall2  = False;
      } else {
        Class.glColormap2 = XCreateColormap  (display,XDefaultRootWindow(display),vinfo->visual,AllocNone); 
        Class.glInstall2  = True;
      }
      Class.glDepth2  = vinfo->depth;
      Class.glVisual2 = vinfo->visual;
    }
    XFree           (vinfo);
    if(Class.glColormap2==0L) {
      CWarn ("Can't create a X colormap.\n");
    }
  }
  Class.gldisp = display;

  if(athis.doubleBufferOn==True) {
    This->core.depth      = Class.glDepth2;
    This->core.colormap   = Class.glColormap2;
    athis.visual          = Class.glVisual2;
    athis.installColormap = Class.glInstall2;
  } else {
    This->core.depth      = Class.glDepth1;
    This->core.colormap   = Class.glColormap1;
    athis.visual          = Class.glVisual1;
    athis.installColormap = Class.glInstall1;
  }

  }
#endif

  XtAddEventHandler(This,ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,
                    0,EventHandler,NULL);

#ifdef DEBUG
  printf("debug : OpenGLArea : InitializeWidget : end\n");
#endif

  /*avoid C++ warnings*/
  a_request = NULL;
  a_args = NULL;
  a_argn = 0;
}
/***************************************************************************/
static void RealizeWidget (
 Widget This
,XtValueMask* a_mask
,XSetWindowAttributes* a_watbs
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
#ifdef DEBUG
  printf("debug : OpenGLArea : RealizeWidget : %s\n",XtName(This));
#endif

  /*Have to create window ourselves due to OpenGL that compells it's visual.*/
  /*In principle colormap is correctly set in a_watbs.*/

  XtCreateWindow(This,(unsigned int)InputOutput,athis.visual,*a_mask,a_watbs);

  /* Call the Realize procedure (XtInheritRealize) */
  if(openGLAreaWidgetClass->core_class.superclass->core_class.realize!=NULL)
    (openGLAreaWidgetClass->core_class.superclass->core_class.realize)(This, a_mask, a_watbs);

  /*If window is delete, all seems ok.*/
  if(athis.installColormap==True) XWidgetInstallColormap(This);

  /*MakeCurrent(This);*/

#ifdef DEBUG
  printf("debug : OpenGLArea : RealizeWidget : end\n");
#endif
}
/***************************************************************************/
static void DestroyWidget (
 Widget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(athis.installColormap==True) {
    XWidgetUninstallColormap (This);
    athis.installColormap    = False;
  }
}
#define IFMOD(a_field)  if(athis.a_field != acur.a_field)
/***************************************************************************/
static Boolean SetValues (
 Widget a_current
,Widget a_request
,Widget This
,ArgList a_args
,Cardinal* a_argn
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  IFMOD(doubleBufferOn) {
    /* Can't change buffering here if X window is created. 
       With OpenGL, buffering fix parameter of the X window.
       Buffering must be choosen before the execution of the 
       Realize method that create the window. */
    if(XtIsRealized(This) && (athis.installColormap==True)) {
      CWarn("Can't change buffering after \"realization\" of the widget.\n");
      athis.doubleBufferOn = acur.doubleBufferOn;
    }
  }
  /*avoid C++ warnings*/
  a_request = NULL;
  a_args = NULL;
  a_argn = 0;
  return False;
}
/***************************************************************************/
static void ChangeWidgetSize (
 Widget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
#ifdef DEBUG
  printf("debug : OpenGLArea : ChangeWidgetSize : %s\n",XtName(This));
#endif

  /* Call the Resize procedure (XtInheritResize) */
  if(openGLAreaWidgetClass->core_class.superclass->core_class.resize!=NULL)
    (openGLAreaWidgetClass->core_class.superclass->core_class.resize)(This);

#ifdef DEBUG
  printf("debug : OpenGLArea : ChangeWidgetSize : end\n");
#endif
}
     
/***************************************************************************/
static void DrawWidget (
 Widget  This
,XEvent* a_event
,Region a_region
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
#ifdef DEBUG
  printf("debug : OpenGLArea : DrawWidget : %s\n",XtName(This));
#endif

  if(openGLAreaWidgetClass->core_class.superclass->core_class.expose!=NULL)
    (openGLAreaWidgetClass->core_class.superclass->core_class.expose)(This,a_event,a_region);

  if(MakeCurrent(This)==1) {
    XoAnyCallbackStruct value;
    value.reason = XoCR_PAINT;
    value.event = a_event;
    XtCallCallbacks(This,XoNpaintCallback,(XtPointer)&value);
    glXSwapBuffers(XtDisplay(This),XtWindow(This));
    glXMakeCurrent(Class.gldisp,None,NULL);
  }

#ifdef DEBUG
  printf("debug : OpenGLArea : DrawWidget : end\n");
#endif

  /*avoid C++ warnings*/
  a_event = NULL;
  a_region = NULL;
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
void OpenGLAreaClearClass (
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
#ifdef HAS_GL
  if(Class.glContext1!=NULL) {
    glXMakeCurrent(Class.gldisp,None,NULL);
    glXDestroyContext(Class.gldisp,Class.glContext1);
    XFreeColormap(Class.gldisp,Class.glColormap1);
    Class.glContext1 = NULL;
    Class.glColormap1 = 0L;
  }
  if(Class.glContext2!=NULL) {
    glXMakeCurrent(Class.gldisp,None,NULL);
    glXDestroyContext(Class.gldisp,Class.glContext2);
    XFreeColormap(Class.gldisp,Class.glColormap2);
    Class.glContext2 = NULL;
    Class.glColormap2 = 0L;
  }
  Class.gldisp = NULL;
#endif
}
/***************************************************************************/
void OpenGLAreaPaint(
 Widget This 
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(!XtIsRealized(This)) return;
  if(MakeCurrent(This)==1) {
    XoAnyCallbackStruct value;
    value.reason = XoCR_PAINT;
    value.event = 0;
    XtCallCallbacks(This,XoNpaintCallback,(XtPointer)&value);
    glXSwapBuffers(XtDisplay(This),XtWindow(This));
    glXMakeCurrent(Class.gldisp,None,NULL);
  }
}

#include "../source/Core/OnX_gl2ps.h"

int OpenGLAreaWrite_gl2ps(Widget This,const char* aFileName,const char* opts) {
  FILE* file;
  int options;
  int sort;
  GLint vp[4];
  int bufsize = 0;

  file = fopen(aFileName,"w");
  if(!file) return 1;

  options = GL2PS_OCCLUSION_CULL 
    | GL2PS_BEST_ROOT 
    | GL2PS_SILENT
    | GL2PS_DRAW_BACKGROUND;
  sort = GL2PS_BSP_SORT;
  //sort = GL2PS_SIMPLE_SORT;
    
  vp[0] = 0;
  vp[1] = 0;
  vp[2] = This->core.width;
  vp[3] = This->core.height;

  gl2psBeginPage("title","OnX_gl2ps", 
                 vp,GL2PS_EPS,sort,options, 
                 GL_RGBA,0, NULL,0,0,0,bufsize, 
                 file,aFileName);
    
 {XoAnyCallbackStruct value;
  value.reason = XoCR_PAINT;
  value.event = 0;
  XtCallCallbacks(This,XoNpaintCallback,(XtPointer)&value);}
    
  gl2psEndPage();
  fclose(file);

  return 0;
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
static int MakeCurrent (
 Widget This 
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(This==NULL)          return 0;
  if(!XtIsRealized(This)) return 0;
#ifdef HAS_GL
  if(athis.doubleBufferOn==True) {
    if(Class.glContext2==NULL) return 0;
    return (int)glXMakeCurrent(XtDisplay(This),XtWindow(This),Class.glContext2);
  } else {
    if(Class.glContext1==NULL) return 0;
    return (int)glXMakeCurrent(XtDisplay(This),XtWindow(This),Class.glContext1);
  }
#else 
  return 0;
#endif
}
/***************************************************************************/
static void EventHandler(
 Widget This
,XtPointer a_tag
,XEvent* a_event 
,Boolean* a_continue
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  XoAnyCallbackStruct value;
  value.reason = XoCR_EVENT;
  value.event = a_event;
  XtCallCallbacks(This,XoNeventCallback,(XtPointer)&value);
  a_tag = NULL;
  a_continue = NULL;
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
static void XWidgetInstallColormap (
 Widget This 
)
/***************************************************************************/
/* 
  From Mesa/widgets/GLwDrawingArea.c/post_colormap routine.
  Could use also XtSetWMColormapWindows.
*/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  Display*          display;
  XWindowAttributes watbs;
  Widget            shell;
  Window*           ws = NULL;
  int               wn = 0,found,count;
  Window            wshell,wthis;
  Colormap          cmapthis;
/*.........................................................................*/
  if(This==NULL) return;
  if( !XtIsWidget(This) || !XtIsRealized(This) ) return;
  shell = XWidgetGetShell (This);
  if(shell==NULL) return;
  display = XtDisplay (This);
  wthis   = XtWindow  (This);
  wshell  = XtWindow  (shell);
  XGetWMColormapWindows (display,wshell, &ws, &wn);
/* Check if colormap of this is a colormap of a Window in list.*/
  XGetWindowAttributes  (display,wthis,&watbs);
  cmapthis              = watbs.colormap;
  found                 = -1;
  for(count=0;count<wn;count++) {
    Colormap             cmap;
    XGetWindowAttributes (display,ws[count],&watbs);
    cmap                 = watbs.colormap;
    if(cmap==cmapthis) {
      XFree (ws);
      return;  /*done*/
    }
    if(ws[count]==wshell) {
      found = count;
    }
  }
  /*Have to add window of this in list.*/
  if(wn==0) {
    if(ws!=NULL) XFree(ws);
    ws = (Window*)malloc ( 2 * sizeof(Window));
  } else {
    ws = (Window*)realloc (ws,(wn + 2) * sizeof(Window));  
  }
  if(ws==NULL) return;
  if(found==-1) {
    /*Window of shell not in list.*/
    ws[wn] = wthis; wn++;
    ws[wn] = wshell;wn++;
  } else {
    ws[found] = wthis;
    ws[wn]    = wshell; wn++;  /*Shell must be last.*/
  }
  if (XSetWMColormapWindows(display,wshell, ws, wn)==0) {
    CWarnF ("XWidgetInstallColormap: can't install colormap of %s in %s.\n",XtName(This),XtName(shell));
  }
  XFree (ws);
}
/***************************************************************************/
static void XWidgetUninstallColormap (
 Widget This 
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  int               count;
  Widget            shell;
  Display*          display;
  Window            wthis,wshell;
  Window*           ws  = NULL;
  int               wn  = 0;
  Window*           nws = NULL;
  int               nwn = 0;
/*.........................................................................*/
  if(This==NULL) return;
  if( !XtIsWidget(This) || !XtIsRealized(This) ) return;
  shell = XWidgetGetShell (This);
  if(shell==NULL) return;
  display               = XtDisplay (This);
  wthis                 = XtWindow  (This);
  wshell                = XtWindow  (shell);
  XGetWMColormapWindows (display,wshell, &ws, &wn);
  if( (wn==0) || (ws==NULL) ) return;
  nws = (Window*)malloc ( wn  * sizeof(Window));
  if(nws==NULL) {
    XFree (ws);
    return;
  }
  nwn = 0;
  for(count=0;count<wn;count++) {
    if(ws[count]!=wthis) {
      nws[nwn] = ws[count];
      nwn++;
    }
  }
  if(wn!=nwn) {
    if (XSetWMColormapWindows(display,wshell, nws, nwn)==0) {
      CWarnF("XWidgetUninstallColormap: can't install colormap of %s in %s.\n",XtName(This),XtName(shell));
    }
  }
  XFree (ws);
  XFree (nws);
}
/***************************************************************************/
Widget XWidgetGetShell(
 Widget This 
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  Widget widget;
  if(This==NULL) return NULL;
  widget = This;
  while(1) {
    if(widget==NULL) return NULL;
    if(XtIsShell(widget)) return widget;
    widget = XtParent(widget);
  }
}
