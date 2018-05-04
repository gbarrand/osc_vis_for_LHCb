#include <stdio.h>
#include <stdlib.h>

#include <X11/StringDefs.h>

#include <Xm/DrawingA.h>
#include <Xm/Form.h>

#include <Xm/PushB.h>

static void button_cbk(Widget,XtPointer,XtPointer);

static Widget dform = NULL;
/***************************************************************************/
int main (
 int   a_argn
,char* a_args[]
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
#if XtSpecificationRelease == 4
  Cardinal argn = (Cardinal)a_argn;
#else
  int argn = a_argn;
#endif
  XtAppContext appContext = NULL;
  Arg args[2];
  XtSetArg(args[0],XtNgeometry,XtNewString("400x400"));
  XtSetArg(args[1],XtNborderWidth,0);
  Widget top = XtAppInitialize(&appContext,"XmPushBT",
                               NULL,(Cardinal)0,
                               &argn,a_args,NULL,
                               args,2);

  XtRealizeWidget (top);

  Widget b = 
    XtCreateManagedWidget("button",xmPushButtonWidgetClass,top,NULL,0);
  XtAddCallback(b,XmNactivateCallback,button_cbk,(XtPointer)0);

  dform = XmCreateFormDialog(top,(char*)"dform",NULL,0);
  XtSetArg(args[0],XmNwidth,200);
  XtSetArg(args[1],XmNheight,200);
  //XtCreateManagedWidget("button",xmPushButtonWidgetClass,dform,args,2);
  XtCreateManagedWidget("button",xmDrawingAreaWidgetClass,dform,args,2);



  XtAppMainLoop   (appContext);
  return          1;
}
/***************************************************************************/
static void button_cbk (
 Widget
,XtPointer
,XtPointer
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  XtManageChild(dform);
}
