
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <Xm/PanedW.h>

#include <stdlib.h>

int main (int   a_argn,char* a_args[]) {
#if XtSpecificationRelease == 4
  Cardinal argn = (Cardinal)a_argn;
#else
  int argn = a_argn;
#endif
  XtAppContext appContext = 0;
  Arg args[10];
  Widget top;
  Widget paned;
  Widget b;

  XtSetArg(args[0],XtNgeometry,XtNewString("400x400"));
  XtSetArg(args[1],XtNborderWidth,0);
  top = XtAppInitialize
    (&appContext,"XmPanedW",NULL,(Cardinal)0,&argn,a_args,NULL,args,2);

  XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
  paned = XmCreatePanedWindow(top,(char*)"paned",args,1);
  XtManageChild (paned);

  XtSetArg(args[0],XmNpaneMaximum,20000);
  b = XmCreatePushButton(paned,(char*)"b1",args,1);
  XtManageChild (b);

  XtSetArg(args[0],XmNpaneMaximum,20000);
  b = XmCreatePushButton(paned,(char*)"b2",args,1);
  XtManageChild (b);

  XtRealizeWidget (top);

  XtAppMainLoop   (appContext);

  return          EXIT_SUCCESS;
}
