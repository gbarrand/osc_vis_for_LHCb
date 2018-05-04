#include <X11/StringDefs.h>

#include <Xm/Xm.h>
#include <Xm/Command.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/CascadeB.h>
#include <Xm/Text.h>

#include <stdlib.h>

static XtAppContext appContext = 0;
static Widget menuBar = 0;

/***************************************************************************/
static void ButtonCallback(
 Widget
,XtPointer
,XtPointer
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  XtAppSetExitFlag(appContext);
}
/***************************************************************************/
static Widget AddMenu(
 const char* a_name
,const char* a_label
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(menuBar==NULL) return 0;
  if(a_name==NULL) return 0;
  if(a_label==NULL) return 0;
  XtManageChild (menuBar);
  // Pulldown menu :
  Widget menu = XmCreatePulldownMenu (menuBar,(char*)a_name,NULL,0);
  // Cascade button :
  Arg args[2];
  XmString cps = XmStringLtoRCreate((char*)a_label,XmSTRING_DEFAULT_CHARSET);
  XtSetArg (args[0],XmNlabelString,cps);
  XtSetArg (args[1],XmNsubMenuId,menu);
  Widget widget = XmCreateCascadeButton (menuBar,(char*)a_name,args,2);
  XmStringFree (cps);
  XtManageChild (widget);
  return menu;
}
/***************************************************************************/
static void AddButton (
 Widget a_menu
,const char* a_label
,const char* a_command
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  if(a_menu==NULL) return;
  if(a_label==NULL) return;
  if(a_command==NULL) return;
  Widget widget = XmCreatePushButton(a_menu,(char*)a_label,NULL,0);
  XtManageChild (widget);
  XtAddCallback (widget,XmNactivateCallback,ButtonCallback,(XtPointer)0);
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

  Widget form = XmCreateForm (top,(char*)"form",NULL,0);
  XtManageChild (form);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  menuBar = XmCreateMenuBar (form,(char*)"menuBar",args,3);

  Widget menu = AddMenu("File","File");
  AddButton(menu,"Exit","Exit");

  XtSetArg(args[0],XmNtopAttachment      ,XmATTACH_WIDGET);
  XtSetArg(args[1],XmNtopWidget          ,menuBar);
  XtSetArg(args[2],XmNleftAttachment     ,XmATTACH_FORM);
  XtSetArg(args[3],XmNrightAttachment    ,XmATTACH_FORM);
  XtSetArg(args[4],XmNbottomAttachment   ,XmATTACH_FORM);
  Widget termForm = XmCreateForm (form,(char*)"termForm",args,5);
  XtManageChild (termForm);

/*
  XtAddCallback(clearButton,XmNactivateCallback,
                clearButtonCallback,(XtPointer)text);
  XtAddCallback(command,XmNcommandEnteredCallback,
                commandEnteredCallback,(XtPointer)this);
*/

  XtRealizeWidget (top);

  char cmd[128];
  sprintf(cmd,"/usr/X11R6/bin/xterm -into %ld &",XtWindow(termForm));
  ::system(cmd);
  //printf("debug : window id %ld\n",XtWindow(termForm));

  XtAppMainLoop   (appContext);

  return          EXIT_SUCCESS;
}
