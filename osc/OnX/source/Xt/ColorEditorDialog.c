/* this : */
#include <OnX/Xt/ColorEditorDialog.h>

#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <OnX/Xt/ColorEditor.h>

static void okCallback(Widget,XtPointer,XtPointer);
static void cancelCallback(Widget,XtPointer,XtPointer);

Widget CreateColorEditorDialog(Widget aParent,String aName,ArgList aArgs,Cardinal aArgn){
  Widget form,colorEditor;
  Arg args[4];

  form = XmCreateFormDialog(aParent,aName,NULL,0);
  
  colorEditor = 
    XtCreateManagedWidget("colorEditor",colorEditorWidgetClass,form,NULL,0);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment,XmATTACH_FORM);
  XtSetValues(colorEditor,args,4);

  XtAddCallback(colorEditor,XmNokCallback,okCallback,0);
  XtAddCallback(colorEditor,XmNcancelCallback,cancelCallback,0);

  XtRealizeWidget(form);

  aArgs = NULL;
  aArgn = 0;

  return colorEditor;

}
static void okCallback(Widget aWidget,XtPointer aTag,XtPointer aData){
  XtUnmanageChild(XtParent(aWidget));
  XtCallCallbacks(aWidget,XmNapplyCallback,0);
  aTag = 0;
  aData = 0;
}
static void cancelCallback(Widget aWidget,XtPointer aTag,XtPointer aData){
  XtUnmanageChild(XtParent(aWidget));
  aTag = 0;
  aData = 0;
}
