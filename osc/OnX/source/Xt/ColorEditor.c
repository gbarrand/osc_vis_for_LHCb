/* this : */
#include "ColorEditorP.h"

#include <X11/StringDefs.h>

#include <Xm/Form.h>
#include <Xm/Scale.h>
#include <Xm/PushB.h>

#include <OnX/Xt/ColorPatch.h>

#ifdef __cplusplus
extern "C"{
#endif
static void init_class();
static void init_widget(Widget,Widget,ArgList,Cardinal*);
static void realize_widget(Widget,XtValueMask*,XSetWindowAttributes*);
static void resize_widget(Widget);

static void red_callback(Widget,XtPointer,XtPointer);
static void green_callback(Widget,XtPointer,XtPointer);
static void blue_callback(Widget,XtPointer,XtPointer);
static void ok_callback(Widget,XtPointer,XtPointer);
static void apply_callback(Widget,XtPointer,XtPointer);
static void cancel_callback(Widget,XtPointer,XtPointer);

#ifdef __cplusplus
}
#endif

#define fThis   ((ColorEditorWidget)This)->color_editor

static XtResource resources [] ={
  {XmNokCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(ColorEditorWidget,color_editor.okCallback),XtRImmediate,(XtPointer)NULL},
  {XmNapplyCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(ColorEditorWidget,color_editor.applyCallback),XtRImmediate,(XtPointer)NULL},
  {XmNcancelCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(ColorEditorWidget,color_editor.cancelCallback),XtRImmediate,(XtPointer)NULL}
};

ColorEditorClassRec  colorEditorClassRec = {
/* Core Class Part */
{
   (WidgetClass)&xmManagerClassRec,/* pointer to superclass ClassRec   */
    "ColorEditor",                 /* widget resource class name       */
    sizeof(ColorEditorRec),        /* size in bytes of widget record   */
    init_class,                 /* class_initialize                 */
    NULL,                         /* dynamic initialization           */
    FALSE,                       /* has class been initialized?      */
    init_widget,                /* initialize                       */
    NULL,                        /* notify that initialize called    */
    realize_widget,             /* XCreateWindow for widget         */
    NULL,                             /* widget semantics name to proc map*/
    0,                                 /* number of entries in actions     */
    resources,                  /* resources for subclass fields    */
    XtNumber(resources),        /* number of entries in resources   */
    NULLQUARK,                        /* resource class quarkified        */
    TRUE,                        /* compress MotionNotify for widget */
    TRUE,                        /* compress Expose events for widget*/
    TRUE,                        /* compress enter and leave events  */
    TRUE,                        /* select for VisibilityNotify      */
    NULL,                        /* free data for subclass pointers  */
    resize_widget,              /* geom manager changed widget size */
    XtInheritExpose,               /* rediplay window                  */
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
/* Composite Class Part */
{
    XtInheritGeometryManager,   /* geometry manager for children   */
    XtInheritChangeManaged,     /* change managed state of child   */
    XtInheritInsertChild,       /* physically add child to parent  */
    XtInheritDeleteChild,       /* physically remove child         */
    NULL                        /* pointer to extension record     */
},
/* Constraint Class Part */
{
    NULL,                        /* constraint resource list            */
    0,                                /* number of constraints in list       */
    0,                                /* size of constraint record           */
    NULL,                        /* constraint initialization           */
    NULL,                        /* constraint destroy proc             */
    NULL,                        /* constraint set_values proc          */
    NULL                        /* pointer to extension record         */
},
/* Manager Class Part (Motif) */
{
   NULL,
   NULL,
   0,   
   NULL,   
   0,   
   XmInheritParentProcess,
   NULL
},
/* ColorEditor */
{
  0
}
   
};

WidgetClass colorEditorWidgetClass = (WidgetClass) &colorEditorClassRec;

static void init_class (){}

static void init_widget (
 Widget a_request
,Widget This
,ArgList a_args
,Cardinal* a_narg
){
  Arg args[6];
  Widget form,ok,apply,cancel;

  if(a_request->core.width<=0)  This->core.width  = 200;
  if(a_request->core.height<=0) This->core.height = 200;

  fThis.fForm = XmCreateForm(This,"form",NULL,0);
  XtManageChild(fThis.fForm);

  fThis.fColorPatch = XtCreateManagedWidget("colorPatch",colorPatchWidgetClass,
                                            fThis.fForm,NULL,0);

  XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
  fThis.fRedScale = XmCreateScale(fThis.fForm,"redScale",args,1);
  XtManageChild(fThis.fRedScale);
  XtAddCallback(fThis.fRedScale,XmNdragCallback,red_callback,This);

  XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
  fThis.fGreenScale = XmCreateScale(fThis.fForm,"greenScale",args,1);
  XtManageChild(fThis.fGreenScale);
  XtAddCallback(fThis.fGreenScale,XmNdragCallback,green_callback,This);

  XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
  fThis.fBlueScale = XmCreateScale(fThis.fForm,"blueScale",args,1);
  XtManageChild(fThis.fBlueScale);
  XtAddCallback(fThis.fBlueScale,XmNdragCallback,blue_callback,This);

  /*ok,apply, cancel form*/
  form = XmCreateForm(fThis.fForm,"ok_apply_cancel_form",NULL,0);
  XtManageChild(form);

  ok = XmCreatePushButton(form,"ok",NULL,0);
  XtManageChild(ok);
  XtAddCallback(ok,XmNactivateCallback,ok_callback,This);

  apply = XmCreatePushButton(form,"apply",NULL,0);
  XtManageChild(apply);
  XtAddCallback(apply,XmNactivateCallback,apply_callback,This);

  cancel = XmCreatePushButton(form,"cancel",NULL,0);
  XtManageChild(cancel);
  XtAddCallback(cancel,XmNactivateCallback,cancel_callback,This);

 {XmString cps = XmStringLtoRCreate("Ok",XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[0],XmNlabelString,cps);
  XtSetValues(ok,args,1);
  XmStringFree(cps);}

 {XmString cps = XmStringLtoRCreate("Apply",XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[0],XmNlabelString,cps);
  XtSetValues(apply,args,1);
  XmStringFree(cps);}

 {XmString cps = XmStringLtoRCreate("Cancel",XmSTRING_DEFAULT_CHARSET);
  XtSetArg(args[0],XmNlabelString,cps);
  XtSetValues(cancel,args,1);
  XmStringFree(cps);}

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNbottomWidget    ,fThis.fGreenScale);
  XtSetValues(fThis.fRedScale,args,5);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNbottomWidget    ,fThis.fBlueScale);
  XtSetValues(fThis.fGreenScale,args,5);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNbottomWidget    ,form);
  XtSetValues(fThis.fBlueScale,args,5);

  /*ok,apply,cancel*/
  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[3],XmNrightAttachment ,XmATTACH_POSITION);
  XtSetArg(args[4],XmNrightPosition   ,33);
  XtSetValues(ok,args,5);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_POSITION);
  XtSetArg(args[3],XmNleftPosition    ,33);
  XtSetArg(args[4],XmNrightAttachment ,XmATTACH_POSITION);
  XtSetArg(args[5],XmNrightPosition   ,66);
  XtSetValues(apply,args,6);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_POSITION);
  XtSetArg(args[3],XmNleftPosition    ,50);
  XtSetArg(args[4],XmNrightAttachment ,XmATTACH_FORM);
  XtSetValues(cancel,args,5);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_FORM);
  XtSetValues(form,args,4);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNbottomWidget    ,fThis.fRedScale);
  XtSetValues(fThis.fColorPatch,args,5);

  fThis.fRed = 0;
  fThis.fGreen = 0;
  fThis.fBlue = 0;

  a_request = 0;
  a_args    = 0;
  a_narg    = 0;
}

static void realize_widget (
 Widget This
,XtValueMask* a_mask
,XSetWindowAttributes* a_watbs
){
  if(colorEditorWidgetClass->core_class.superclass->core_class.realize!=NULL)
    (colorEditorWidgetClass->core_class.superclass->core_class.realize)(This,a_mask,a_watbs);

  XtResizeWidget(fThis.fForm,This->core.width,This->core.height,0);
}

static void resize_widget(Widget This){
  XtResizeWidget(fThis.fForm,This->core.width,This->core.height,0);
}

static void red_callback (Widget This,XtPointer a_tag,XtPointer a_data){
  int value;
  XmScaleGetValue(This,&value);

  ((ColorEditorWidget)a_tag)->color_editor.fRed = ((double)value)/100;
  ColorPatchSetColor(((ColorEditorWidget)a_tag)->color_editor.fColorPatch,
                     ((ColorEditorWidget)a_tag)->color_editor.fRed,
                     ((ColorEditorWidget)a_tag)->color_editor.fGreen,
                     ((ColorEditorWidget)a_tag)->color_editor.fBlue);

  a_data = NULL;
}

static void green_callback(Widget This,XtPointer a_tag,XtPointer a_data){
  int value;
  XmScaleGetValue(This,&value);
  ((ColorEditorWidget)a_tag)->color_editor.fGreen = ((double)value)/100;
  ColorPatchSetColor(((ColorEditorWidget)a_tag)->color_editor.fColorPatch,
                     ((ColorEditorWidget)a_tag)->color_editor.fRed,
                     ((ColorEditorWidget)a_tag)->color_editor.fGreen,
                     ((ColorEditorWidget)a_tag)->color_editor.fBlue);
  a_data = NULL;
}

static void blue_callback(Widget This,XtPointer a_tag,XtPointer a_data){
  int value;
  XmScaleGetValue(This,&value);
  ((ColorEditorWidget)a_tag)->color_editor.fBlue = ((double)value)/100;
  ColorPatchSetColor(((ColorEditorWidget)a_tag)->color_editor.fColorPatch,
                     ((ColorEditorWidget)a_tag)->color_editor.fRed,
                     ((ColorEditorWidget)a_tag)->color_editor.fGreen,
                     ((ColorEditorWidget)a_tag)->color_editor.fBlue);
  a_data = NULL;
}

static void ok_callback(Widget This,XtPointer a_tag,XtPointer a_data){
  XtCallCallbacks((Widget)a_tag,XmNokCallback,0);
  This = NULL;
  a_data = NULL;
}

static void apply_callback(Widget This,XtPointer a_tag,XtPointer a_data){
  XtCallCallbacks((Widget)a_tag,XmNapplyCallback,0);
  This = NULL;
  a_data = NULL;
}

static void cancel_callback(Widget This,XtPointer a_tag,XtPointer a_data){
  XtCallCallbacks((Widget)a_tag,XmNcancelCallback,0);
  This = NULL;
  a_data = NULL;
}

void ColorEditorGetColor(Widget This,double* aRed,double* aGreen,double* aBlue){
  *aRed = fThis.fRed;
  *aGreen = fThis.fGreen;
  *aBlue = fThis.fBlue;
  This = NULL;
}
