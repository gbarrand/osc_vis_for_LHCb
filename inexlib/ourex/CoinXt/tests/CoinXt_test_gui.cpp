
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/engines/SoTimeCounter.h>

#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>

//////////////////////////////////////////////////////////////////////////////
int main(
 int /*aArgc*/
,char** aArgv
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  Arg args[1];
  XtSetArg(args[0], XtNgeometry,XtNewString("100x100"));
  XtAppContext appContext;
  Widget top = XtAppInitialize(&appContext,
			       "CoinXt_cube",NULL,(Cardinal)0,
			       &aArgc,aArgv,
			       NULL,args,1);
  if (top==NULL) exit(1);

  SoXt::init(top);
  */

  Widget top = SoXt::init(aArgv[0]);
  if (top==NULL) exit(1);

  Widget form = XmCreateForm(top,(char*)"",0,0);
  XtManageChild(form);

  Widget pushButton = XmCreatePushButton(form,(char*)"Button",0,0);
  XtManageChild(pushButton);

  Arg args[10];
  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNbottomAttachment ,XmATTACH_FORM);
  XtSetValues(pushButton,args,3);

  // Create a scene :

  SoSeparator* root=new SoSeparator;
  root->ref();

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue (SbColor(1,1,0));
  root->addChild(material);

  SoScale* scale = new SoScale;
  scale->scaleFactor.setValue(0.1F,0.1F,0.1F);
  //scale->scaleFactor.setValue(0.5F,0.5F,0.5F);
  root->addChild(scale);

  SoCube* cube = new SoCube;
  root->addChild(cube);

  Widget frame = XmCreateFrame(form,(char*)"",0,0);
  XtManageChild(frame);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNrightAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNleftAttachment  ,XmATTACH_WIDGET);
  XtSetArg(args[3],XmNleftWidget,pushButton);
  XtSetArg(args[4],XmNbottomAttachment ,XmATTACH_FORM);
  XtSetValues(frame,args,5);

  SoXtExaminerViewer* viewer = new SoXtExaminerViewer(frame);
  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(700,500));
  //viewer->show();
  viewer->setTitle("CoinXt");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);

  SoXt::show(top);

  XtRealizeWidget(top);

  SoXt::mainLoop();

  //Cleanup
  root->unref();

  delete viewer;

  //SoXt::clear();  

  return 0;
}
