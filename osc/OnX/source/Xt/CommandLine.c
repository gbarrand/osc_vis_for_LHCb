
/* this : */
#include "CommandLineP.h"

#include <X11/StringDefs.h>
#include <X11/keysym.h>

#include <Xm/Text.h>

#include <stdlib.h>
#include <string.h>

#define STRDUP(str)  ((str) != NULL ? (strcpy((char*)malloc((unsigned)strlen(str) + 1), str)) : (char*)NULL)
#define FREE(p) {if(p) free(p);}

#ifdef __cplusplus
extern "C" {
#endif
static void InitializeClass(void);
static void InitializeWidget(Widget, Widget,ArgList,Cardinal*);

static void EnterCallback(Widget,XtPointer,XtPointer);
static void KeyHandler(Widget,XtPointer,XEvent*,Boolean*);
static void AddLine(Widget,char*);
#ifdef __cplusplus
}
#endif

static XtResource resources [] ={
 {XmNcommandEnteredCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(CommandLineWidget,command_line.commandEnteredCallback),XtRImmediate,(XtPointer)NULL},
 {XtNcompleteCallback,XtCCallback,XtRCallback,sizeof(XtCallbackList),
   XtOffset(CommandLineWidget,command_line.completeCallback),XtRImmediate,(XtPointer)NULL}
};

#define athis ((CommandLineWidget)This)->command_line

CommandLineClassRec commandLineClassRec = {
/* Core Class Part */
{
    (WidgetClass)&xmFormClassRec, /* pointer to superclass ClassRec   */
    (char*)"CommandLine",        /* widget resource class name       */
    sizeof(CommandLineRec),      /* size in bytes of widget record   */
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
    NULL,                         /* free data for subclass pointers  */
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
/* Composite Class Part */
{
    XtInheritGeometryManager,    /* geometry manager for children   */
    XtInheritChangeManaged,      /* change managed state of child   */
    XtInheritInsertChild,        /* physically add child to parent  */
    XtInheritDeleteChild,        /* physically remove child         */
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


/* CommandLine */
{
    NULL
}
   
};

WidgetClass commandLineWidgetClass = (WidgetClass) &commandLineClassRec;

/***************************************************************************/
static void InitializeClass(
 void
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
}
/***************************************************************************/
static void InitializeWidget(
 Widget a_request
,Widget This
,ArgList a_args
,Cardinal* a_argn
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  This->core.width = 0;
  This->core.height = 0;

  Arg args[4];
  XtSetArg(args[0],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(args[1],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(args[3],XmNrightAttachment,XmATTACH_FORM);
  athis.text = XmCreateText(This,(char*)"COMMANDLINE_TEXT",args,4);
  XtManageChild(athis.text);

  XtAddCallback(athis.text,XmNactivateCallback,EnterCallback,(XtPointer)This);
  XtAddEventHandler(athis.text,KeyPressMask,False,KeyHandler,(XtPointer)This);

  athis.historySize = 0;
  athis.historyList = NULL;
  athis.historyPos = -1;

  a_request = NULL;
  a_args = NULL;
  a_argn = 0; /*avoid C++ warnings*/
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
static void EnterCallback (
 Widget a_text
,XtPointer a_tag
,XtPointer a_data
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  Widget This;
  char* s;
  CommandLineCallbackStruct value;

  This = (Widget)a_tag;

  s = XmTextGetString(a_text);
  XmTextSetString(a_text,(char*)"");
  
  AddLine(This,s); /* Store in the history list. */

  athis.historyPos = -1;

  value.reason = XmCR_ACTIVATE;
  value.event = ((XmAnyCallbackStruct*)a_data)->event;
  value.value = s;
  XtCallCallbacks(This,XmNcommandEnteredCallback,(XtPointer)&value);

  XtFree(s);
}
/***************************************************************************/
static void KeyHandler (
 Widget a_text
,XtPointer a_tag
,XEvent* a_event
,Boolean* a_dispatch
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  Widget This;
  KeySym keySym;
  XKeyEvent* event;

  This = (Widget)a_tag;
  event = (XKeyEvent*)a_event;

  XLookupString(event,NULL,0,&keySym,NULL);
  if(keySym==XK_Escape) {
    /*completion*/
    CommandLineCallbackStruct value;
    char* s = XmTextGetString(a_text);
    value.reason = XtCR_COMPLETE;
    value.event = a_event;
    value.value = s;
    XtCallCallbacks(This,XtNcompleteCallback,(XtPointer)&value);
    XtFree(s);

  } if(keySym==XK_Up) {
    int n,pos;
    n = athis.historySize; 
    pos = athis.historyPos == -1 ? n-1 : athis.historyPos-1;
    if((pos>=0)&&(pos<n)) {
      char* command = athis.historyList[pos];
      if(command) {
        XmTextSetString(a_text,command);
        XmTextSetInsertionPosition(a_text,
                                   XmTextGetLastPosition(a_text));
      }
      athis.historyPos = pos;
    }

  } if(keySym==XK_Down) {
    int n,pos;
    n = athis.historySize; 
    pos = athis.historyPos == -1 ? n : athis.historyPos + 1;
    if((pos>=0)&&(pos<n)) {
      char* command = athis.historyList[pos];
      if(command) {
        XmTextSetString(a_text,command);
        XmTextSetInsertionPosition(a_text,
                                   XmTextGetLastPosition(a_text));
      }
      athis.historyPos = pos;
    } else if(pos>=n) {
      XmTextSetString(a_text,"");
      athis.historyPos = -1;
    }
  }
}
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
static void AddLine (
 Widget This                     
,char* a_string
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(athis.historyList==NULL) {
    athis.historyList = (char**)malloc(sizeof(char*));
    if(athis.historyList==NULL) {
      athis.historySize = 0;
      athis.historyPos = 0;
      return;
    }
    athis.historyList[0] = STRDUP(a_string);
    athis.historySize = 1;
  } else {
    athis.historyList = (char**)realloc((char*)athis.historyList,
                        (athis.historySize + 1)*sizeof(char*));
    if(athis.historyList==NULL) {
      athis.historySize = 0;
      athis.historyPos = 0;
      return;
    }
    athis.historyList[athis.historySize] = STRDUP(a_string);
    athis.historySize++;
  }
}
/***************************************************************************/
void CommandLineSetString(
 Widget This
,char* aValue
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  XmTextSetString(athis.text,aValue);
  XmTextSetInsertionPosition(athis.text,XmTextGetLastPosition(athis.text));
}
/***************************************************************************/
char* CommandLineGetString(
 Widget This
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  return XmTextGetString(athis.text);
}
