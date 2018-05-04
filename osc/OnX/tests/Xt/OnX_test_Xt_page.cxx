#include <stdio.h>
#include <stdlib.h>

#include <X11/StringDefs.h>

#include <Xm/DrawingA.h>
#include <Xm/Form.h>

#include <Xm/PushB.h>

#include <Inventor/Xt/SoXt.h>
#include <OnX/Inventor/HEPVis.h>
#include <OnX/Xt/PageViewer.h>

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

  SoXt::init(top);
  OnX::HEPVis::initClasses();

  printf("debug : main : 000\n");
  /*
  Widget b = 
    XtCreateManagedWidget("button",xmPushButtonWidgetClass,top,NULL,0);
  XtAddCallback(b,XmNactivateCallback,button_cbk,(XtPointer)0);
  printf("debug : main : 001\n");
  dform = XmCreateFormDialog(top,(char*)"dform",NULL,0);
  */

  dform = XmCreateForm(top,(char*)"dform",NULL,0);
  XtSetArg(args[0],XmNwidth,200);
  XtSetArg(args[1],XmNheight,200);
  //XtCreateManagedWidget("button",xmPushButtonWidgetClass,dform,args,2);
  //XtCreateManagedWidget("button",xmDrawingAreaWidgetClass,dform,args,2);
  //XtCreateManagedWidget("page",xmDrawingAreaWidgetClass,dform,args,2);
  printf("debug : main : 002+\n");
  XtCreateManagedWidget("page",pageViewerWidgetClass,dform,args,2);

  printf("debug : main : 003\n");
  XtRealizeWidget (top);
  printf("debug : main : 004\n");

  //XtAppMainLoop   (appContext);
  SoXt::mainLoop();
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
