// this :
#include <OnX/Xt/VContainer.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <OnX/Xt/XtTools.h>

//////////////////////////////////////////////////////////////////////////////
void VContainerPackUniform(
 Widget aWidget
)
//////////////////////////////////////////////////////////////////////////////
// Assumes that aWidget is a XmForm.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int number = XWidgetGetChildrenNumber(aWidget);
  if(number<=0) return;

  if(number==1) {

    Widget first = XWidgetGetChild(aWidget,0);

    Arg args[4];
    XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_FORM);
    XtSetValues(first,args,4);

  } else {

    Arg args[6];

    int fractionBase = 100;
    XtSetArg(args[0],XmNfractionBase,fractionBase);
    XtSetValues(aWidget,args,1);

    int h = int(((double)fractionBase)/((double)number));    

    Widget first = XWidgetGetChild(aWidget,0);
    XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_POSITION);
    XtSetArg(args[4],XmNbottomPosition ,h);
    XtSetValues(first,args,5);

    Widget last = XWidgetGetChild(aWidget,number-1);
    XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_POSITION);
    XtSetArg(args[1],XmNtopPosition ,fractionBase-h);
    XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
    XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[4],XmNbottomAttachment ,XmATTACH_FORM);
    XtSetValues(last,args,5);

    for(int index=1;index<(number-1);index++) {
      Widget w = XWidgetGetChild(aWidget,index);
      XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_POSITION);
      XtSetArg(args[1],XmNtopPosition ,h*index);
      XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
      XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
      XtSetArg(args[4],XmNbottomAttachment ,XmATTACH_POSITION);
      XtSetArg(args[5],XmNbottomPosition,h*index+h);
      XtSetValues(w,args,6);
    }  
  }
}
//////////////////////////////////////////////////////////////////////////////
void VContainerPackUp(
 Widget aWidget
)
//////////////////////////////////////////////////////////////////////////////
// Assumes that aWidget is a XmForm.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int number = XWidgetGetChildrenNumber(aWidget);
  if(number<=0) return;

  if(number==1) {

    Widget first = XWidgetGetChild(aWidget,0);

    Arg args[4];
    XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_NONE);
    XtSetValues(first,args,4);

  } else {

    Arg args[5];

    Widget first = XWidgetGetChild(aWidget,0);
    XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_NONE);
    XtSetValues(first,args,4);

    Widget wtop = first;
    for(int index=1;index<number;index++) {
      Widget w = XWidgetGetChild(aWidget,index);
      XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_WIDGET);
      XtSetArg(args[1],XmNtopWidget ,wtop);
      XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_FORM);
      XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
      XtSetArg(args[4],XmNbottomAttachment ,XmATTACH_NONE);
      XtSetValues(w,args,5);
      wtop = w;
    }  
  }
}
