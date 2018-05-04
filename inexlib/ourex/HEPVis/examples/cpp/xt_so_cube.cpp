
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>

//
#define HAS_SELECTION

#ifdef HAS_SELECTION
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
//#include <Inventor/actions/SoLineHighlightRenderAction.h>
static void selectionCB(void* aTag,SoPath* aPath)  {
  ::fprintf(stdout, "selectionCB : %s\n",
            aPath->getTail()->getTypeId().getName().getString());
  SoSelection* sel = (SoSelection*)aTag;
  sel->touch(); // to redraw
}
static void deselectionCB(void* aTag,SoPath* aPath)  {
  ::fprintf(stdout, "deselectionCB : %s\n",
            aPath->getTail()->getTypeId().getName().getString());
  SoSelection* sel = (SoSelection*)aTag;
  sel->touch(); // to redraw
}
#endif

//#define ADD_BUTTON

#ifdef ADD_BUTTON
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <Xm/PushB.h>
static void buttonCbk(Widget,XtPointer,XtPointer) {
  SoDebugError::postInfo("buttonCbk","exiting...");
  XtAppContext context = SoXt::getAppContext();
  XtAppSetExitFlag(context);  
}
#endif

int main(int,char** aArgv) {

  Widget top = SoXt::init(aArgv[0]);
  if(top==NULL) ::exit(1);

  // Create a scene :

#ifdef HAS_SELECTION
  SoSelection* root = new SoSelection;
  root->addSelectionCallback(selectionCB,(void*)root);
  root->addDeselectionCallback(deselectionCB,(void*)root);
  root->policy = SoSelection::TOGGLE;
  root->policy = SoSelection::SINGLE;
  root->policy = SoSelection::SHIFT;
#else
  SoSeparator* root = new SoSeparator;
#endif
  root->ref();

 {SoSeparator* sep = new SoSeparator;
  root->addChild(sep);  

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue (SbColor(1,1,0));
  sep->addChild(material);

  SoScale* scale = new SoScale;
  scale->scaleFactor.setValue(0.1F,0.1F,0.1F);
  //scale->scaleFactor.setValue(0.5F,0.5F,0.5F);
  sep->addChild(scale);

  SoCube* cube = new SoCube;
  sep->addChild(cube);}

 {SoSeparator* sep = new SoSeparator;
  root->addChild(sep);  

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue (SbColor(1,0,1));
  sep->addChild(material);

  SoTranslation* trans = new SoTranslation;
  trans->translation.setValue(0.5F,0,0);
  sep->addChild(trans);

  SoScale* scale = new SoScale;
  scale->scaleFactor.setValue(0.1F,0.1F,0.1F);
  //scale->scaleFactor.setValue(0.5F,0.5F,0.5F);
  sep->addChild(scale);

  SoSphere* sphe = new SoSphere;
  sep->addChild(sphe);}

  SoXtExaminerViewer* viewer = new SoXtExaminerViewer(top);
    
#ifdef ADD_BUTTON
  Widget buttonsParent = viewer->getAppPushButtonParent();
  if(buttonsParent) {
    Widget button = XtCreateManagedWidget
      ("Esc",xmPushButtonWidgetClass,buttonsParent,NULL,0);
    XtAddCallback(button,XmNactivateCallback,buttonCbk,(XtPointer)0);
    viewer->addAppPushButton(button);
  } else {
    SoDebugError::postInfo("main","no app buttons parent !");
  }
#endif
#ifdef HAS_SELECTION
  viewer->setGLRenderAction(new SoBoxHighlightRenderAction());
  //viewer->setGLRenderAction(new SoLineHighlightRenderAction());
#endif
  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(700,500));
  viewer->setTitle("CoinXt");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);
  //viewer->show();

  SoXt::show(top);

  //XtRealizeWidget(top);

  SoXt::mainLoop();

  //Cleanup
  root->unref();

  delete viewer;

  return 0;
}
