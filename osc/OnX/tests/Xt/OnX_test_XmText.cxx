#include <X11/StringDefs.h>

#include <Xm/Xm.h>
#include <Xm/Text.h>

#include <stdlib.h>

static XtAppContext appContext = 0;

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

  Widget text = XmCreateText (top,(char*)"text",NULL,0);
  XtManageChild(text);
 
  XtRealizeWidget (top);

  XtAppMainLoop   (appContext);

  return          EXIT_SUCCESS;
}
