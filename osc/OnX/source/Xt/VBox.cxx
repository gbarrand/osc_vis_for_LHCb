// this :
#include <OnX/Xt/VBox.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <OnX/Xt/XtTools.h>

bool VBoxAttachChildren(
 Widget aWidget
,const std::string& aExpand
,bool aFixSize
,unsigned int aSizeOfFixed
){
  int number = XWidgetGetChildrenNumber(aWidget);

  if(number==1) {

    Widget first = XWidgetGetChild(aWidget,0);

    //printf("debug : VBox attach 1 : %s : %s : %s\n",
    //   XtName(aWidget),XtName(first),aExpand.c_str());

    Arg args[4];
    XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
    XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_FORM);
    XtSetValues(first,args,4);

  } else if(number==2) {
    Widget first = XWidgetGetChild(aWidget,0);
    Widget second = XWidgetGetChild(aWidget,1);

    //printf("debug : VBox attach 2 : %s : %s %s : %s\n",
    //XtName(aWidget),XtName(first),XtName(second),aExpand.c_str());

    Arg args[4];

    XtSetArg(args[0],XmNfractionBase,100);
    XtSetValues(aWidget,args,1);

    // Detach first at bottom :
    XtSetArg(args[0],XmNbottomAttachment ,XmATTACH_NONE);
    XtSetValues(first,args,1);

    XtSetArg(args[0],XmNbottomAttachment ,XmATTACH_FORM);
    XtSetArg(args[1],XmNleftAttachment   ,XmATTACH_FORM);
    XtSetArg(args[2],XmNrightAttachment  ,XmATTACH_FORM);
    XtSetArg(args[3],XmNtopAttachment ,XmATTACH_NONE);
    XtSetValues(second,args,4);
  
    if(aExpand=="second") {
      XtSetArg(args[0],XmNtopAttachment,XmATTACH_WIDGET);
      XtSetArg(args[1],XmNtopWidget,first);
      XtSetValues(second,args,2);
      if(aFixSize) {
        // aSizeOfFixed is in pixel.
        // The below is tricky, but it works.
        XtSetArg(args[0],XmNbottomAttachment,XmATTACH_OPPOSITE_FORM);
        XtSetArg(args[1],XmNbottomOffset,-aSizeOfFixed);
        XtSetValues(first,args,2);
      }
    } else if(aExpand=="first") {
      XtSetArg(args[0],XmNbottomAttachment,XmATTACH_WIDGET);
      XtSetArg(args[1],XmNbottomWidget,second);
      XtSetValues(first,args,2);
      if(aFixSize) {
        // aSizeOfFixed is in pixel.
        // The below is tricky, but it works.
        XtSetArg(args[0],XmNtopAttachment,XmATTACH_OPPOSITE_FORM);        
        XtSetArg(args[1],XmNtopOffset,-aSizeOfFixed);
        XtSetValues(second,args,2);
      }
    } else if(aExpand=="both") {
      XtSetArg(args[0],XmNbottomAttachment,XmATTACH_POSITION);
      XtSetArg(args[1],XmNbottomPosition,50);
      XtSetValues(first,args,2);
      
      XtSetArg(args[0],XmNtopAttachment,XmATTACH_POSITION);
      XtSetArg(args[1],XmNtopPosition,50);
      XtSetValues(second,args,2);
    } else {
      return false;
    }
  
  } else {
    return false;
  }
  return true;
}
