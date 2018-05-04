/* this : */
#include "ColorPatchP.h"

#include <X11/StringDefs.h>

#ifdef __cplusplus
extern "C"{
#endif
static void init_class();
static void init_widget(Widget,Widget,ArgList,Cardinal*);
static void destroy_widget(Widget);
static void expose_widget(Widget,XEvent*,Region);

static void draw_box(ColorPatchWidget);
static void get_GC(ColorPatchWidget);
static void free_GC(ColorPatchWidget);
#ifdef __cplusplus
}
#endif

#define aThis   ((ColorPatchWidget)This)->color_patch

ColorPatchClassRec  colorPatchClassRec = {
/* Core Class Part */
{
   (WidgetClass) &coreClassRec, /* pointer to superclass ClassRec   */
    "ColorPatch",               /* widget resource class name       */
    sizeof(ColorPatchRec),        /* size in bytes of widget record   */
    init_class,                 /* class_initialize                 */
    NULL,                         /* dynamic initialization           */
    FALSE,                       /* has class been initialized?      */
    init_widget,                /* initialize                       */
    NULL,                        /* notify that initialize called    */
    XtInheritRealize,           /* XCreateWindow for widget         */
    NULL,                             /* widget semantics name to proc map*/
    0,                                 /* number of entries in actions     */
    NULL,                        /* resources for subclass fields    */
    0,                                /* number of entries in resources   */
    NULLQUARK,                        /* resource class quarkified        */
    TRUE,                        /* compress MotionNotify for widget */
    TRUE,                        /* compress Expose events for widget*/
    TRUE,                        /* compress enter and leave events  */
    TRUE,                        /* select for VisibilityNotify      */
    destroy_widget,                /* free data for subclass pointers  */
    XtInheritResize,            /* geom manager changed widget size */
    expose_widget,               /* rediplay window                  */
    NULL,                        /* set subclass resource values     */
    NULL,                        /* notify that set_values called    */
    XtInheritSetValuesAlmost,   /* set_values got "Almost" geo reply*/
    NULL,                        /* notify that get_values called    */
    XtInheritAcceptFocus,       /* assign input focus to widget     */
    XtVersion,                        /* version of intrinsics used       */
    NULL,                        /* list of callback offsets         */
    XtInheritTranslations,      /* translations                     */        
    XtInheritQueryGeometry,     /* return preferred geometry        */
    XtInheritDisplayAccelerator,/* display your accelerator         */
    NULL                        /* pointer to extension record      */
},
/* ColorPatch */
{
   0
}
   
};

WidgetClass colorPatchWidgetClass = (WidgetClass) &colorPatchClassRec;

/***************************************************************************/
static void init_class (
/*endmethod*/
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
}
/***************************************************************************/
static void init_widget (
 Widget a_request
,Widget This
,ArgList a_args
,Cardinal* a_narg
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(a_request->core.width<=0)  This->core.width  = 100;
  if(a_request->core.height<=0) This->core.height = 100;

  aThis.fRed = 0;
  aThis.fGreen = 0;
  aThis.fRed = 0;
  aThis.fPixel = XBlackPixelOfScreen(XDefaultScreenOfDisplay(XtDisplay(This)));

  aThis.fAllocatedNumber = 0;

  get_GC ((ColorPatchWidget)This);

  a_request = 0;
  a_args    = 0;
  a_narg    = 0;
}
/***************************************************************************/
static void destroy_widget (
 Widget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(aThis.fAllocatedNumber) {
    /* WARNING : XFreeColors crashes if 0 is passed for pixel number ! */
    XFreeColors(XtDisplay(This),
                DefaultColormapOfScreen(DefaultScreenOfDisplay(This)),
                aThis.fAllocatedPixels,aThis.fAllocatedNumber,0);
  }
  free_GC((ColorPatchWidget)This);
}
/***************************************************************************/
static void expose_widget (
 Widget  This
,XEvent* a_event
,Region a_region
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  draw_box((ColorPatchWidget)This);
  a_event  = 0;
  a_region = 0;
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
static void get_GC (
  ColorPatchWidget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  XGCValues values;
  XtGCMask mask;
  mask = GCForeground | GCBackground ;
  values.background = This->core.background_pixel;
  values.foreground = This->core.background_pixel;
  This->color_patch.fGC = XtGetGC((Widget)This,mask,&values);
}
/***************************************************************************/
static void free_GC(
  ColorPatchWidget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  XtReleaseGC ((Widget) This, This->color_patch.fGC);
}
/***************************************************************************/
static void draw_box (
  ColorPatchWidget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  Display* xdisp;
  Window xwindow;

  xdisp          = XtDisplay (This);
  xwindow        = XtWindow  (This);
  XClearWindow   (xdisp,xwindow);
  
  XSetForeground (xdisp,This->color_patch.fGC,aThis.fPixel);

  XFillRectangle (xdisp,xwindow,This->color_patch.fGC,
                  0,0,
                  This->core.width,This->core.height);
}
/***************************************************************************/
void ColorPatchSetColor(
 Widget This
,double aRed
,double aGreen
,double aBlue
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  Display* display;
  Screen* screen;
  int vclass;
  Colormap cmap;
  int is_grey;
  double grey;
  XColor xcolor;

  if(!This) return;
  aThis.fRed = aRed;
  aThis.fGreen = aGreen;
  aThis.fRed = aBlue;

  display       = XtDisplay(This);
  screen        = DefaultScreenOfDisplay(display);
#ifdef __cplusplus
  vclass        = DefaultVisualOfScreen(screen)->c_class;
#else
  vclass        = DefaultVisualOfScreen(screen)->class;
#endif
  cmap          = DefaultColormapOfScreen(screen);

  is_grey       = ((vclass==GrayScale) || (vclass==StaticGray) ) ? 1 : 0;
  grey          = is_grey==1 ?  
    (0.30 * aRed + 0.59 * aGreen + 0.11 * aBlue) : 0;

  xcolor.pixel  = 0L;
  xcolor.red    = (unsigned short) ((is_grey==1 ? grey : aRed)   * 0xffff);
  xcolor.green  = (unsigned short) ((is_grey==1 ? grey : aGreen) * 0xffff);
  xcolor.blue   = (unsigned short) ((is_grey==1 ? grey : aBlue)  * 0xffff);
  /*Could be done on read_only/read_write visual class.*/
  if(XAllocColor(display,cmap,&xcolor)==0) {
    /*Cell is taken from a common pool. It is read only.*/
    /*Color not found. Try to allocate a private color cell.*/
    if( (vclass==TrueColor)   || 
        (vclass==StaticColor) || 
        (vclass==StaticGray)
        ) { /*Viusal class is read_only.*/
      return;
    } else {
      /*read/write cell*/
      if(XAllocColorCells(display,cmap,False,NULL,0,&(xcolor.pixel),1)==0) { 
        /*Alloc fail.*/
        return;
      } else {
        xcolor.flags = DoRed|DoGreen|DoBlue;
        XStoreColor(display,cmap,&xcolor);

        if(aThis.fAllocatedNumber<MAX_COLOR) {
          aThis.fAllocatedPixels[aThis.fAllocatedNumber] = xcolor.pixel;
          aThis.fAllocatedNumber++;
        } /* FIXME : else should issue a warning. */
      }
    }
  } else {
    /*Not allocated, do not have to free it.*/
  }

  aThis.fPixel = xcolor.pixel;
  draw_box((ColorPatchWidget)This);
}
