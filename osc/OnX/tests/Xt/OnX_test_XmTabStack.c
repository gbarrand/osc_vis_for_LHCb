
#include <Xm/Xm.h>
#include <Xm/PanedW.h>
#include <Xm/TabStack.h>
#include <Xm/PushB.h>

#include <stdlib.h>

static void create_cbk(Widget w,XtPointer a_tag,XtPointer p) {
  Widget tabStack = (Widget)a_tag;
  Widget b;
  Arg args[10];
  b = XmCreatePushButton(tabStack,(char*)"b3",args,0);
  XtManageChild (b);
}
static void delete_cbk(Widget w,XtPointer a_tag,XtPointer p) {
  Widget b = (Widget)a_tag;
  XtDestroyWidget (b);
}

int main (int   a_argn,char* a_args[]) {
#if XtSpecificationRelease == 4
  Cardinal argn = (Cardinal)a_argn;
#else
  int argn = a_argn;
#endif
  XtAppContext appContext = 0;
  Arg args[10];
  Widget top;
  Widget vp,hp;
  Widget tabStack;
  Widget b1,b2,b3;

  XtSetArg(args[0],XtNgeometry,XtNewString("400x400"));
  XtSetArg(args[1],XtNborderWidth,0);
  top = XtAppInitialize
    (&appContext,"XmTabStack",NULL,(Cardinal)0,&argn,a_args,NULL,args,2);

  vp = XmCreatePanedWindow(top,(char*)"vpaned",args,0);
  XtManageChild (vp);

  XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
  hp = XmCreatePanedWindow(vp,(char*)"hpaned",args,1);
  XtManageChild (hp);
 {Arg args[1];
  XtSetArg(args[0],XmNallowResize,True);
  XtSetValues(hp,args,1);}

  //tabStack = XmCreateTabStack(top,(char*)"tabStack",args,0);
  tabStack = XtCreateWidget("tabStack",xmTabStackWidgetClass,hp,0,0);
  XtManageChild (tabStack);
/* FIXME : if commenting out, then the creation of a new button
           in the stack (b1) makes the program looping
           in the XtManageChild of the create_cbk.
 {Arg args[1];
  XtSetArg(args[0],XmNallowResize,True);
  XtSetValues(tabStack,args,1);}
*/

  b1 = XmCreatePushButton(tabStack,(char*)"b1",args,0);
  XtManageChild (b1);
  XtAddCallback (b1,XmNactivateCallback,create_cbk,(XtPointer)tabStack);

  b2 = XmCreatePushButton(tabStack,(char*)"b2",args,0);
  XtManageChild (b2);
  XtAddCallback (b2,XmNactivateCallback,delete_cbk,(XtPointer)b3);

  b3 = XmCreatePushButton(tabStack,(char*)"b3",args,0);
  XtManageChild (b3);

  XtRealizeWidget (top);

  XtAppMainLoop   (appContext);

  return          EXIT_SUCCESS;
}
