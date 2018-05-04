
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <HEPVis/Gui/SoGui.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#define HEPVIS_HAS_VRML2

#ifdef HEPVIS_HAS_VRML2
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#endif

#include <HEPVis/SbMath.h>
#include <HEPVis/SbArguments.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodes/SoMarkerSet.h>
#include <HEPVis/nodes/SoTubs.h>
#include <HEPVis/nodes/SoBox.h>
#include <HEPVis/nodes/SoTrd.h>
#include <HEPVis/nodekits/SoDetectorTreeKit.h>
#include <HEPVis/actions/SoGL2PSAction.h>
#include <HEPVis/actions/SoCounterAction.h>
#include <HEPVis/actions/SoAlternateRepAction.h>

static SoGuiExaminerViewer* examinerViewer = NULL;

static SbBool sAlternateRep = FALSE;

#ifdef HEPVisWin
#include "MenuBar.ic"
HEPVis_menuBar_def
#endif

//
// Test of SoDetectorTreeKit class
// Joe Boudreau Nov. 11, 1996
//
 
void addWafer (SoDetectorTreeKit *Wafer, float radius) {
  SoSeparator *previewSeparator = 
    (SoSeparator *) Wafer->getPart("previewSeparator",TRUE);
  SoSeparator *fullSeparator = 
    (SoSeparator *) Wafer->getPart("fullSeparator",   TRUE);

  //Global Properties
  SoTranslation *T0 = new SoTranslation();
  T0->ref();
  T0->translation.setValue(0,0,radius);

  SoTransform *Transform = 
    (SoTransform *) Wafer->getPart("transform",TRUE);
  Transform->combineLeft(T0);
  T0->unref();

  SoMaterial *Silicon  = 
    (SoMaterial *) Wafer->getPart("appearance.material",TRUE);
  Silicon->diffuseColor.setValue (1,0,0);

  //  Full Representation
  SoBox *BoxShape = new SoBox();
  BoxShape->fDx=2*radius*FSIN(15*FM_PI/180);
  BoxShape->fDy=15;
  BoxShape->fDz=0.03F;
  if (sAlternateRep==TRUE) BoxShape->generateAlternateRep(0);
  previewSeparator->addChild(BoxShape);
  
  //  Contracted Representation 
  SoSphere *Sphere=new SoSphere();
  fullSeparator->addChild(Sphere);
 
}

void addWedge (SoDetectorTreeKit *Wedge, float angle) {
  SoSeparator *previewSeparator = 
    (SoSeparator *) Wedge->getPart("previewSeparator",TRUE);
  SoSeparator *fullSeparator    = 
    (SoSeparator *) Wedge->getPart("fullSeparator",   TRUE);

  // Global Properties
  SoRotationXYZ  *R0 = new SoRotationXYZ();
  R0->ref();
  R0->axis=SoRotationXYZ::X;
  R0->angle= FM_PI_2;
  
  SoRotationXYZ *R1 = new SoRotationXYZ();
  R1->ref();
  R1->axis=SoRotationXYZ::Z;
  R1->angle=angle;
  
  SoTranslation *T0 = new SoTranslation();
  T0->ref();
  T0->translation.setValue(0,0,7.25);
  
  SoTransform *TotalTransform = 
    (SoTransform *) Wedge->getPart("transform",TRUE);
  TotalTransform->combineLeft(R1);
  TotalTransform->combineLeft(R0);
  TotalTransform->combineLeft(T0);
  R1->unref();
  R0->unref();
  T0->unref();

  // Preview Representation
  SoTrd *WedgeTrd = new SoTrd();
  WedgeTrd->fDx1  = 0.65F;
  WedgeTrd->fDx2  = 3.1F;
  WedgeTrd->fDy1  = 15;
  WedgeTrd->fDy2  = 15;
  WedgeTrd->fDz   = 4.75F;
  if (sAlternateRep==TRUE) WedgeTrd->generateAlternateRep(0);
  previewSeparator->addChild(WedgeTrd); 

  for (int i=0;i<5;i++) {
    SoDetectorTreeKit *Wafer= new SoDetectorTreeKit();
    if (sAlternateRep==TRUE) Wafer->generateAlternateRep(0);
    addWafer(Wafer, (float)(i+1));
    fullSeparator->addChild(Wafer);
  }
}

void addBarrel(SoDetectorTreeKit * Barrel, float z) {

  SoSeparator *previewSeparator = 
    (SoSeparator *) Barrel->getPart("previewSeparator",TRUE);
  SoSeparator *fullSeparator    = 
    (SoSeparator *) Barrel->getPart("fullSeparator",   TRUE);

  // Global Properties
  SoTransform *TheTransform  = 
    (SoTransform *) Barrel->getPart("transform",TRUE);
  TheTransform->translation.setValue(0,0,z);

  SoMaterial *BluePlastic  = 
    (SoMaterial *) Barrel->getPart("appearance.material",TRUE);
  BluePlastic->diffuseColor.setValue(0,0,1);

  // Preview Representation
  SoTubs *BarrelTubs = new SoTubs();
  BarrelTubs->pRMin = 2.5F;
  BarrelTubs->pRMax = 12;
  BarrelTubs->pDz   = 15;
  if (sAlternateRep==TRUE) BarrelTubs->generateAlternateRep(0);
  previewSeparator->addChild(BarrelTubs);

  // Full Representation
  for (int i=0;i<12;i++) {
    SoDetectorTreeKit *Wedge=new SoDetectorTreeKit();
    if (sAlternateRep==TRUE) Wedge->generateAlternateRep(0);
    addWedge(Wedge,i*FM_PI/6);
    fullSeparator->addChild(Wedge);
    SoMaterial* GreenPlastic = 
      (SoMaterial*)Wedge->getPart("appearance.material",TRUE); 
    GreenPlastic->diffuseColor.setValue(0,1,0);
    if (i==0) Wedge->setPreview(FALSE);
  }
}

void addDetector (SoDetectorTreeKit *SVX) {

  SoSeparator* previewSeparator = 
    (SoSeparator*)SVX->getPart("previewSeparator",TRUE);
  SoSeparator* fullSeparator = 
    (SoSeparator*)SVX->getPart("fullSeparator",TRUE);

  // Global Properties
  SoMaterial* RedPlastic = 
    (SoMaterial*)SVX->getPart("appearance.material",TRUE);
  RedPlastic->diffuseColor.setValue(1,0,0);

  // Preview Representation
  SoTubs* SVXTubs = new SoTubs();
  SVXTubs->pRMin = 2.5F;
  SVXTubs->pRMax = 12;
  SVXTubs->pDz = 48;
  if (sAlternateRep==TRUE) SVXTubs->generateAlternateRep(0);
  previewSeparator->addChild(SVXTubs);

  // Full Representation
  for (int j=0;j<3;j++) {
    SoDetectorTreeKit* Barrel = new SoDetectorTreeKit();
    if (sAlternateRep==TRUE) Barrel->generateAlternateRep(0);
    float z = (float)(32 * (j-1));
    addBarrel(Barrel,z);
    fullSeparator->addChild(Barrel);
    if (j==2) Barrel->setPreview(FALSE);
  }
}

void keystroke(void*,SoEventCallback*);

int main(int argc,char** argv) {

  SbArguments args(argc,argv);

  // Set up the main window
  
  SoGuiWidget myWindow=SoGui::init(argv[0]);
  if (!myWindow) exit(1);

  // You must initialize a new class

  SoTubs::initClass();
  SoBox::initClass();
  SoTrd::initClass();
  SoDetectorTreeKit::initClass();
  SoViewportRegion::initClass();
  HEPVis_SoMarkerSet::initClass();
  SoGL2PSAction::initClass(); //After nodes.
  SoCounterAction::initClass();
  SoAlternateRepAction::initClass();

  sAlternateRep = 
   (getenv("HEPVIS_ALTERNATEREP")||getenv("ALTERNATEREP"))?TRUE:FALSE;

  // Set up the root of the scene database and
  // reference it

  SoSeparator* root = new SoSeparator;
  root->ref();
  // Add a camera shared by the two viewers :
  SoOrthographicCamera* camera = new SoOrthographicCamera;
  root->addChild(camera);

  // In case only painter viewer is used :
  SoTransform* transform = new SoTransform;
  root->addChild(transform);

  SoDetectorTreeKit* SVX = new SoDetectorTreeKit();
  if (sAlternateRep==TRUE) SVX->generateAlternateRep(0);
  addDetector(SVX);
  SVX->setPreview(FALSE);
  root->addChild(SVX);

  SoTubs* beamPipe = new SoTubs();
  beamPipe->pRMin = 2.3F;
  beamPipe->pRMax = 2.4F;
  beamPipe->pDz = 70;
  if (sAlternateRep==TRUE) beamPipe->generateAlternateRep(0);
  root->addChild(beamPipe);

/*
  //Write an ascii out.iv file :
 {SoWriteAction action;
  action.getOutput()->openFile("out.iv");
  action.getOutput()->setBinary(FALSE);
  action.apply(root);
  action.getOutput()->closeFile();}
*/

  //G.Barrand :
  if (args.isAnArgument("-write")) {
    SbBool altRep = args.isAnArgument("-altrep")?TRUE:FALSE;
    SbString file;
    args.find("-file",file);
    if(file=="") file = "out.iv";
    SoTools::write(*root,file,"",FALSE,altRep);
  }


#ifdef HEPVIS_HAS_VRML2
  //Write a VRML2 file :
 {SoToVRML2Action vrmlAction;
  vrmlAction.apply(root);
  SoVRMLGroup* vrmlRoot = vrmlAction.getVRML2SceneGraph();
 {SoWriteAction action;
  action.getOutput()->openFile("out.wrl");
  action.getOutput()->setBinary(FALSE);
  action.getOutput()->setHeaderString("#VRML V2.0 utf8");
  action.apply(vrmlRoot);
  action.getOutput()->closeFile();}}
#endif

  SoEventCallback* eventCallback = new SoEventCallback;
  eventCallback->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                                  keystroke,0);
  root->addChild(eventCallback);                                  

 {SoCounterAction action;
  action.apply(root);
  unsigned int nodes = action.getCount();
  action.setLookFor(SoCounterAction::TYPE);
  action.setType(SoShape::getClassTypeId());
  action.apply(root);
  unsigned int shapes = action.getCount();
  SoDebugError::postInfo("DetectorTreeKit","Number of nodes : %u",nodes);
  SoDebugError::postInfo("DetectorTreeKit","Number of shapes : %u",shapes);}

  // View the new object in an Examiner Viewer

  examinerViewer = new SoGuiExaminerViewer(myWindow);
  const SbViewportRegion& vpRegion = examinerViewer->getViewportRegion();
  SoGL2PSAction* action = new SoGL2PSAction(vpRegion);
  action->setFileName("out.ps");
  examinerViewer->setGLRenderAction(action);
#ifdef HEPVisWin
  HEPVis_menuBar_set
#endif
  examinerViewer->setSize(SbVec2s(400,400));
  examinerViewer->setSceneGraph(root);
  examinerViewer->viewAll();
  examinerViewer->setViewing(FALSE);
  examinerViewer->show();

  SoDebugError::postInfo("DetectorTreeKit",
    "type P key in picking mode to produce a postscript file.");

  SoGui::show(myWindow);
  SoGui::mainLoop();

  root->unref();
  delete examinerViewer;
  delete action;

  return 0; 
}
void keystroke(void*,SoEventCallback* eventCB){
  if (eventCB->isHandled()) return;

  const SoKeyboardEvent* event = (SoKeyboardEvent*)eventCB->getEvent();

  if (SO_KEY_PRESS_EVENT(event,P)) {
    if(examinerViewer) {
      SoDebugError::postInfo("DetectorTreeKit","produce out.ps...");

      SoGL2PSAction* action = 
        (SoGL2PSAction*)examinerViewer->getGLRenderAction();
      action->enableFileWriting();
      examinerViewer->render();
      action->disableFileWriting();

      eventCB->setHandled();
    }
  }
}
