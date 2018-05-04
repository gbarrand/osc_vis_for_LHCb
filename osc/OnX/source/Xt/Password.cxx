
// this :
#include "PasswordP.h"

// Private :
#include <X11/StringDefs.h>

#include <Xm/Text.h>

static void InitializeClass(void);
static void InitializeWidget(Widget, Widget,ArgList,Cardinal*);
static void DestroyWidget(Widget);
static void ModifyVerifyCallback(Widget,XtPointer,XtPointer);
static void ActivateCallback(Widget,XtPointer,XtPointer);

static XtResource resources [] ={
 {(char*)XtNpasswordEnteredCallback,(char*)XtCCallback,(char*)XtRCallback,
  sizeof(XtCallbackList),
  XtOffset(PasswordWidget,password.passwordEnteredCallback),
  XtRImmediate,(XtPointer)NULL},
};

#define athis ((PasswordWidget)This)->password

PasswordClassRec passwordClassRec = {
/* Core Class Part */
{
    (WidgetClass)&xmTextClassRec, /* pointer to superclass ClassRec   */
    (char*)"Password",            /* widget resource class name       */
    sizeof(PasswordRec),          /* size in bytes of widget record   */
    InitializeClass,              /* class_initialize                 */
    NULL,                         /* dynamic initialization           */
    FALSE,                        /* has class been initialized?      */
    InitializeWidget,             /* initialize                       */
    NULL,                         /* notify that initialize called    */
    XtInheritRealize,             /* XCreateWindow for widget         */
    NULL,                         /* widget semantics name to proc map*/
    0,                            /* number of entries in actions     */
    resources,                    /* resources for subclass fields    */
    XtNumber(resources),          /* number of entries in resources   */
    NULLQUARK,                    /* resource class quarkified        */
    TRUE,                         /* compress MotionNotify for widget */
    TRUE,                         /* compress Expose events for widget*/
    TRUE,                         /* compress enter and leave events  */
    TRUE,                         /* select for VisibilityNotify      */
    DestroyWidget,                /* free data for subclass pointers  */
    XtInheritResize,              /* geom manager changed widget size */
    XtInheritExpose,              /* expose                           */
    NULL,                         /* set subclass resource values     */
    NULL,                         /* notify that SetValues called     */
    XtInheritSetValuesAlmost,     /* SetValues got "Almost" geo reply */
    NULL,                         /* notify that get_values called    */
    XtInheritAcceptFocus,         /* assign input focus to widget     */
    XtVersion,                    /* version of intrinsics used       */
    NULL,                         /* list of callback offsets         */
    XtInheritTranslations,        /* translations                     */        
    XtInheritQueryGeometry,       /* return preferred geometry        */
    XtInheritDisplayAccelerator,  /* display your accelerator         */
    NULL                          /* pointer to extension record      */
},
/* primitive_class fields  */
  {                                
    XmInheritBorderHighlight,             /* Primitive border_highlight   */
    XmInheritBorderUnhighlight,           /* Primitive border_unhighlight */
    NULL,                                 /* translations                 */
    NULL,                                 /* arm_and_activate             */
    NULL,                                 /* get resources                */
    0,                                    /* num get_resources            */
    NULL                                  /* extension                    */
},
/* XmText */
{                                /* text class fields */
    NULL,                        /* extension         */
},
/* Password */
{
    NULL
}
   
};

WidgetClass passwordWidgetClass = (WidgetClass) &passwordClassRec;

//////////////////////////////////////////////////////////////////////////////
static void InitializeClass(
 void
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
static void InitializeWidget(
 Widget
,Widget This
,ArgList
,Cardinal*
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  This->core.width = 0;
  This->core.height = 0;

  athis.value = new std::string("");

  XtAddCallback(This,XmNmodifyVerifyCallback,
                ModifyVerifyCallback,(XtPointer)This);
  XtAddCallback(This,XmNactivateCallback,
                ActivateCallback,(XtPointer)This);

}
//////////////////////////////////////////////////////////////////////////////
static void DestroyWidget (
 Widget This
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete athis.value;
}
//////////////////////////////////////////////////////////////////////////////
void ModifyVerifyCallback(
 Widget aWidget
,XtPointer aTag
,XtPointer aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Widget This = (Widget)aTag;

  XmTextVerifyCallbackStruct* data = (XmTextVerifyCallbackStruct*)aData;

  // By default accept nothing
  data->doit = False;

  // Only accept text modification of password fields
  if (data->reason != XmCR_MODIFYING_TEXT_VALUE) return;

  if(!data->text->length) {
    // Deletion.
    data->doit = True;
    // Only can delete if we have stuff to delete.
    if (athis.value->size()) {
      int len = athis.value->size();
      // Find the start of the chunk of text to delete.
      int start = data->startPos < len ? (int)data->startPos : len - 1;
      
      // might be more stuff after the end that we
      
      int end = data->endPos > len ? len : data->endPos;

      std::string s = athis.value->erase(start,end-start);
      delete athis.value;
      athis.value = new std::string(s);
    }
  } else if (data->text->length >= 1) {
    // Insert character.

    // No insertion if it makes you exceed maxLength
    int plen = athis.value->size();
    int maxlength = 1000000;
    Arg arg[1];
    XtSetArg(arg[0], XmNmaxLength, &maxlength);
    XtGetValues(aWidget,arg,1);
    if ((plen + data->text->length) > maxlength) return;

    if (!athis.value->size()) {
      for (int i=0; i < data->text->length; i++) 
        *(athis.value) += data->text->ptr[i];
    } else {
      int len = athis.value->size();
      int start = data->startPos < len ? (int)data->startPos : len;

      std::string s = athis.value->substr(0,start);
      for (int i=0; i < data->text->length; i++) {
        s += data->text->ptr[i];
      }
      s += athis.value->substr(start,len-start);
      delete athis.value;
      athis.value = new std::string(s);
    }

    data->doit = True;
    // make a '*' show up instead of what they typed
    for(int i=0; i < data->text->length; i++) data->text->ptr[i] = '*';
  }
}
//////////////////////////////////////////////////////////////////////////////
void ActivateCallback(
 Widget
,XtPointer aTag
,XtPointer aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Widget This = (Widget)aTag;

  PasswordCallbackStruct data;
  data.reason = XtCR_PASSWORD_ENTERED;
  data.event = ((XmAnyCallbackStruct*)aData)->event;
  data.value = XtNewString(athis.value->c_str());
  XtCallCallbacks(This,XtNpasswordEnteredCallback,(XtPointer)&data);
  XtFree(data.value);
}
