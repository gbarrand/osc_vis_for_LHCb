#include <OnX/Xt/HBox.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <OnX/Xt/XtTools.h>

bool HBoxAttachChildren(
 Widget aWidget
,const std::string& aExpand
,bool aFixSize
,unsigned int aSizeOfFixed
){
  int number = XWidgetGetChildrenNumber(aWidget);
  if(number==1) {
    Widget first = XWidgetGetChild(aWidget,0);

    Arg args[4];
    XtSetArg(args[0],XmNtopAttachment,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment,XmATTACH_FORM);
    XtSetArg(args[3],XmNbottomAttachment,XmATTACH_FORM);
    XtSetValues(first,args,4);

  } else if(number==2) {

    Widget first = XWidgetGetChild(aWidget,0);
    Widget second = XWidgetGetChild(aWidget,1);

    Arg args[4];

    XtSetArg(args[0],XmNfractionBase,100);
    XtSetValues(aWidget,args,1);

    // Detach first at right :
    XtSetArg(args[0],XmNrightAttachment ,XmATTACH_NONE);
    XtSetValues(first,args,1);

    XtSetArg(args[0],XmNtopAttachment,XmATTACH_FORM);
    XtSetArg(args[1],XmNrightAttachment,XmATTACH_FORM);
    XtSetArg(args[2],XmNbottomAttachment,XmATTACH_FORM);
    XtSetArg(args[3],XmNleftAttachment,XmATTACH_NONE);
    XtSetValues(second,args,4);

    if(aExpand=="second") {
      XtSetArg(args[0],XmNleftAttachment,XmATTACH_WIDGET);
      XtSetArg(args[1],XmNleftWidget,first);
      XtSetValues(second,args,2);

      if(aFixSize) {
        // aSizeOfFixed is in pixel.
        // The below is tricky, but it works.
        XtSetArg(args[0],XmNrightAttachment,XmATTACH_OPPOSITE_FORM);
        XtSetArg(args[1],XmNrightOffset,-aSizeOfFixed);
        XtSetValues(first,args,2);
      }

    } else if(aExpand=="first") {
      XtSetArg(args[0],XmNrightAttachment,XmATTACH_WIDGET);
      XtSetArg(args[1],XmNrightWidget,second);
      XtSetValues(first,args,2);

      if(aFixSize) {
        // aSizeOfFixed is in pixel.
        // The below is tricky, but it works.
        XtSetArg(args[0],XmNleftAttachment,XmATTACH_OPPOSITE_FORM);        
        XtSetArg(args[1],XmNleftOffset,-aSizeOfFixed);
        XtSetValues(second,args,2);
      }

    } else if(aExpand=="both") {

      XtSetArg(args[0],XmNrightAttachment,XmATTACH_POSITION);
      XtSetArg(args[1],XmNrightPosition,50);
      XtSetValues(first,args,2);
      
      XtSetArg(args[0],XmNleftAttachment,XmATTACH_POSITION);
      XtSetArg(args[1],XmNleftPosition,50);
      XtSetValues(second,args,2);

    } else {
      return false;
    }
  
  } else {
    return false;
  }
  return true;
}
