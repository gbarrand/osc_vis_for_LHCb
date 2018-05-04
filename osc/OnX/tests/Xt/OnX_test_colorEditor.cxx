#include <stdio.h>

#include <Xm/Xm.h>

#include <OnX/Xt/ColorEditor.h>
#include <OnX/Xt/ColorEditorDialog.h>

static void ok_callback(Widget,XtPointer,XtPointer);

//////////////////////////////////////////////////////////////////////////////
int main (
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Widget top = XtInitialize(aArgv[0],"OnX_ColorEditor",NULL,0,&aArgc,aArgv);

  Widget colorEditor = CreateColorEditorDialog(top,(char*)"",NULL,0);
  XtManageChild(XtParent(colorEditor));
  XtAddCallback(colorEditor,XmNokCallback,ok_callback,0);

  XtRealizeWidget(top);

  XtMainLoop ();
  return     1;
}
/***************************************************************************/
static void ok_callback (
 Widget    This 
,XtPointer
,XtPointer
)
/***************************************************************************/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
{
  double r,g,b;
  ColorEditorGetColor(This,&r,&g,&b);
  printf("color : %g %g %g\n",r,g,b);
}
