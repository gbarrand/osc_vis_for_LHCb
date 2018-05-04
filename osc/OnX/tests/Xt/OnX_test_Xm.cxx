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
/***************************************************************************/
/***************************************************************************/
#if defined(__APPLE__) && (XmVERSION>=2) && (XmREVISION>=3)
#include <Xm/Display.h>
#include <Xm/DialogS.h>
#include <Xm/DragOverS.h>
#include <Xm/GrabShell.h>
#include <Xm/Print.h>
#include <X11/IntrinsicP.h>
#include <iostream>
static void ResetVendorField(WidgetClass aWidgetClass){
  WidgetClass wc = aWidgetClass;
  while(wc) {
    WidgetClass super = wc->core_class.superclass;
    if(super) {
      char* name = (char*)super->core_class.class_name;
      if( !strcmp(name,"VendorShell") && (super!=vendorShellWidgetClass) ) {
        // The Vendor super class is not the Motif one, override :
        wc->core_class.superclass = vendorShellWidgetClass;
        return;
      }
    }
    wc = super;
  }
}
static void LookDSM_Problem(){
  if(!vendorShellWidgetClass->core_class.class_part_initialize) {
    // The vendorShellWidgetClass we get is the Xt one.
    std::cout << "OnX::XtUI::LookDSM_Problem :" << std::endl;
    std::cout << " Can't solve the dsm problem." << std::endl;
    std::cout << " Bad linking order of -lXm and -lXt " << std::endl;
    std::cout << " or you try to load a DLL linked to X11 prior to" << std::endl;
    std::cout << " load a DLL linked with Motif." << std::endl;
    return;
  }
  ResetVendorField(xmDisplayClass);
  ResetVendorField(xmDialogShellWidgetClass);
  ResetVendorField(xmDragOverShellWidgetClass);
  ResetVendorField(xmGrabShellWidgetClass);
  ResetVendorField(xmPrintShellWidgetClass);
}
#else
static void LookDSM_Problem(){}
#endif
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

/***************************************************************************/
int main (
 int   a_argn
,char* a_args[]
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  LookDSM_Problem();
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

  XtSetArg(args[0],XmNtopAttachment      ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment     ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment    ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment   ,XmATTACH_FORM);
  Widget command = XmCreateCommand (form,(char*)"command",args,4);
  XtManageChild (command);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNbottomWidget    ,command);
  XmString cps = XmStringLtoRCreate((char*)"Clear",XmSTRING_DEFAULT_CHARSET);
  XtSetArg (args[5],XmNlabelString,cps);
  Widget clearButton = XmCreatePushButton(form,(char*)"clearButton",args,6);
  XmStringFree (cps);
  XtManageChild (clearButton);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_WIDGET);
  XtSetArg(args[1],XmNtopWidget       ,menuBar);
  XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[4],XmNbottomAttachment,XmATTACH_WIDGET);
  XtSetArg(args[5],XmNbottomWidget    ,clearButton);
  XtSetArg(args[6],XmNeditMode        ,XmMULTI_LINE_EDIT);
  XtSetArg(args[7],XmNrows            ,12);
  XtSetArg(args[8],XmNcolumns         ,80);
  Widget text = XmCreateScrolledText (form,(char*)"text",args,9);
  XtManageChild (text);

/*
  XtAddCallback(clearButton,XmNactivateCallback,
                clearButtonCallback,(XtPointer)text);
  XtAddCallback(command,XmNcommandEnteredCallback,
                commandEnteredCallback,(XtPointer)this);
*/

  XtRealizeWidget (top);

  XtAppMainLoop   (appContext);

  return          EXIT_SUCCESS;
}
