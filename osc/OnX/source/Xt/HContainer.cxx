// this :
#include <OnX/Xt/HContainer.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <OnX/Xt/XtTools.h>

//////////////////////////////////////////////////////////////////////////////
void HContainerPackUniform(
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
    XtSetArg(args[2],XmNbottomAttachment ,XmATTACH_FORM);
    XtSetArg(args[3],XmNrightAttachment ,XmATTACH_POSITION);
    XtSetArg(args[4],XmNrightPosition ,h);
    XtSetValues(first,args,5);

    Widget last = XWidgetGetChild(aWidget,number-1);
    XtSetArg(args[0],XmNleftAttachment   ,XmATTACH_POSITION);
    XtSetArg(args[1],XmNleftPosition ,fractionBase-h);
    XtSetArg(args[2],XmNtopAttachment  ,XmATTACH_FORM);
    XtSetArg(args[3],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[4],XmNbottomAttachment ,XmATTACH_FORM);
    XtSetValues(last,args,5);

    for(int index=1;index<(number-1);index++) {
      Widget w = XWidgetGetChild(aWidget,index);
      XtSetArg(args[0],XmNleftAttachment   ,XmATTACH_POSITION);
      XtSetArg(args[1],XmNleftPosition ,h*index);
      XtSetArg(args[2],XmNtopAttachment  ,XmATTACH_FORM);
      XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_FORM);
      XtSetArg(args[4],XmNrightAttachment ,XmATTACH_POSITION);
      XtSetArg(args[5],XmNrightPosition,h*index+h);
      XtSetValues(w,args,6);
    }  
  }
}
