#include <X11/StringDefs.h>
#include <X11/Shell.h>

#include <OnX/Xt/CommandLine.h>

#include <stdlib.h>
#include <stdio.h> //printf

static XtAppContext appContext = 0;

/***************************************************************************/
static void CompleteCallback(
 Widget
,XtPointer
,XtPointer
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  printf("debug : complete\n");
}
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
  Arg args[10];
  XtSetArg(args[0],XtNgeometry,XtNewString("400x400"));
  XtSetArg(args[1],XtNborderWidth,0);
  Widget top = XtAppInitialize(&appContext,"XmPushBT",
                               NULL,(Cardinal)0,
                               &argn,a_args,NULL,
                               args,2);

  Widget text = XtCreateWidget("",commandLineWidgetClass,top,0,0);
  XtManageChild(text);

  XtAddCallback(text,XtNcompleteCallback,CompleteCallback,(XtPointer)NULL);
 
  XtRealizeWidget (top);

  XtAppMainLoop   (appContext);

  return          EXIT_SUCCESS;
}
