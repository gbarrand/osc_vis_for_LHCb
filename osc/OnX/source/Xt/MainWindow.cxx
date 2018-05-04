// this :
#include <OnX/Xt/MainWindow.h>

#include <X11/Intrinsic.h>
#include <X11/keysym.h>

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>

#include <OnX/Xt/XtTools.h>

#define MAIN_WINDOW_CONTAINER "main_window_container"
#define MAIN_WINDOW_MENU_BAR "main_window_menu_bar"
#define MAIN_WINDOW_TOOL_BAR "main_window_tool_bar"

Widget CreateMainWindow(
 Widget aParent
,char* aName
,ArgList
,Cardinal
) 
{
  /*
  Widget tip = XmToolTipGetLabel(xtWidget);
  XtAddCallback(XtParent(tip),XmNpopupCallback,
                (XtCallbackProc)ToolTipPopup,(XtPointer)0);
  */
  Widget form = XmCreateForm(aParent,(char*)MAIN_WINDOW_CONTAINER,0,0);
  XtManageChild(form);

  Arg args[10];
  XtSetArg(args[0],XmNtopAttachment,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment,XmATTACH_FORM);
  Widget menuBar = XmCreateMenuBar(form,(char*)MAIN_WINDOW_MENU_BAR,args,3);
  // Not managed now.

  XtSetArg(args[0],XmNorientation,XmHORIZONTAL);
  XtSetArg(args[1],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(args[3],XmNtopAttachment,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNtopWidget,menuBar);
  //Widget toolBar = 
  XmCreateRowColumn(form,(char*)MAIN_WINDOW_TOOL_BAR,args,5);
  // Not managed now.

  // The top attachment will be changed when a MenuBar
  // or a ToolBar will be requested.
  XtSetArg(args[0],XmNbottomAttachment,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment,XmATTACH_FORM);
  XtSetArg(args[3],XmNtopAttachment,XmATTACH_FORM);
  return XmCreateForm(form,aName,args,4);
}
Boolean IsMainWindow(
 Widget aWidget
) 
{
  if(!XtIsSubclass(aWidget,xmFormWidgetClass)) return False;
  if(strcmp(XtName(XtParent(aWidget)),MAIN_WINDOW_CONTAINER)!=0) return False;
  return True;
}

Widget GetMainWindowMenuBar(
 Widget aWidget
) 
{
  Widget form = XtParent(aWidget);
  return XWidgetFindChild(form,MAIN_WINDOW_MENU_BAR);
}

Widget GetMainWindowToolBar(
 Widget aWidget
) 
{
  Widget form = XtParent(aWidget);
  return XWidgetFindChild(form,MAIN_WINDOW_TOOL_BAR);
}


void ActivateMainWindowMenuBar(
 Widget aWidget
) 
{
  Widget form = XtParent(aWidget);
  Widget menuBar = XWidgetFindChild(form,MAIN_WINDOW_MENU_BAR);
  Widget toolBar = XWidgetFindChild(form,MAIN_WINDOW_TOOL_BAR);
  Arg args[2];
  Widget topWidget;
  XtSetArg(args[0],XmNtopWidget,&topWidget);
  XtGetValues(aWidget,args,1);
  if(topWidget==toolBar) { 
    // A toolBar had already been given. 
    // The "user" form widget (aWidget) is already attached to it.
  } else {
    XtSetArg(args[0],XmNtopAttachment,XmATTACH_WIDGET);
    XtSetArg(args[1],XmNtopWidget,menuBar);
    XtSetValues(aWidget,args,2);
  }
}

void ActivateMainWindowToolBar(
 Widget aWidget
) 
{
  Widget form = XtParent(aWidget);
  Widget toolBar = XWidgetFindChild(form,MAIN_WINDOW_TOOL_BAR);
  // Reattach the user form (aWidget) to the toolBar :
  Arg args[2];
  XtSetArg(args[0],XmNtopAttachment,XmATTACH_WIDGET);
  XtSetArg(args[1],XmNtopWidget,toolBar);
  XtSetValues(aWidget,args,2);
}

#include <X11/Shell.h>

Widget CreateMainWindowShell(
 Display* aDisplay
,String aName
,ArgList
,Cardinal
) 
{
  Arg args[2];
  XtSetArg(args[0],XtNgeometry,XtNewString("400x400"));
  XtSetArg(args[1],XtNborderWidth,0);
  //xtWidget = XtCreateWidget(xtName.c_str(),xtClass,xtParent,args,2);
  Widget shell = XtAppCreateShell("OnX", // So that OnX*xxx resources work.
                                  "OnX",
                                  topLevelShellWidgetClass,
                                  aDisplay,
                                  args,2); 
  XtSetMappedWhenManaged(shell,True);

  // Returns a form.
  return CreateMainWindow(shell,aName,NULL,0);
}
