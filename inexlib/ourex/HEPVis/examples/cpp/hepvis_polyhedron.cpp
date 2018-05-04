
#include <HEPVis/Gui/SoGui.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
//#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include <HEPVis/SbArguments.h>
#include <HEPVis/SbPolyhedron.h>
#include <HEPVis/SbString.h>
#include <HEPVis/SbConvert.h>
#include <HEPVis/SbMath.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoPolyhedron.h>
#include <HEPVis/nodes/SoCoordinateAxis.h>
#include <HEPVis/actions/SoGL2PSAction.h>
#include <HEPVis/actions/SoAlternateRepAction.h>

void keystroke(void*,SoEventCallback*);

SoGuiExaminerViewer* examinerViewer = NULL;

int main(int aArgc, char** aArgv) {

  SbArguments args(aArgc,aArgv);

  // Set up the main window
 
  SoGuiWidget widget=SoGui::init(aArgv[0]);
  if (!widget) exit(1);

  SoSFPolyhedron::initClass();
  SoPolyhedron::initClass();
  SoCoordinateAxis::initClass();
  SoGL2PSAction::initClass(); //After nodes.
  SoAlternateRepAction::initClass();

  SbBool solid = TRUE;
  SbBool reducedWireFrame = TRUE; 
  if(args.isAnArgument("-wf")) solid = FALSE;
  if(args.isAnArgument("-no_rwf")) reducedWireFrame = FALSE;

 {SbString s;  
  if(args.find("-nstep",s)==TRUE) {
    int n;
    if(SbConvertToInt(s,n)==TRUE) {
      SbPolyhedron::SetNumberOfRotationSteps(n);
    }
  }}

  // You must initialize a new class

  SoSeparator* sceneGraph = new SoSeparator;
  sceneGraph->ref();

  if(solid) {
    SoLightModel* lightModel = new SoLightModel;
    lightModel->model.setValue(SoLightModel::PHONG);
    sceneGraph->addChild(lightModel);
  } else {
    SoLightModel* lightModel = new SoLightModel;
    lightModel->model.setValue(SoLightModel::BASE_COLOR);
    sceneGraph->addChild(lightModel);
  }

  //SoCoordinateAxis* frame = new SoCoordinateAxis;
  //sceneGraph->addChild(frame);

/*
  // a simple example :
  SbPolyhedronBox box(20,30,40);

  SoPolyhedron* polyhedron = new SoPolyhedron(box);
  polyhedron->solid.setValue(solid);
  polyhedron->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron);
*/

//#include "exa_rich2_mirror.cxx"
//#include "exa_hexagone.cxx"
//#include "exa_hexagone_2.cxx"
#include "exa_polyhedron.cxx"
/*
 {SbPolyhedronSphere sphere(9,10,0,2*M_PI,0,M_PI);
  SoPolyhedron* polyhedron = new SoPolyhedron(sphere);
  polyhedron->solid.setValue(solid);
  polyhedron->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron);}
*/
  SoEventCallback* eventCallback = new SoEventCallback;
  eventCallback->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                                  keystroke,0);
  sceneGraph->addChild(eventCallback);                                  

  if (args.isAnArgument("-write")) {
    SbBool altRep = args.isAnArgument("-altrep")?TRUE:FALSE;
    SbString file;
    args.find("-file",file);
    if(file=="") file = "out.iv";
    SoTools::write(*sceneGraph,file,"",FALSE,altRep);
  }

  examinerViewer = new SoGuiExaminerViewer(widget);
  const SbViewportRegion& vpRegion = examinerViewer->getViewportRegion();
  SoGL2PSAction* action = new SoGL2PSAction(vpRegion);
  action->setFileName("out.ps");
  examinerViewer->setGLRenderAction(action);
  examinerViewer->setCameraType(SoOrthographicCamera::getClassTypeId());
  examinerViewer->setSceneGraph(sceneGraph);
  examinerViewer->viewAll();
  examinerViewer->show();

  SoDebugError::postInfo("Polyhedron",
    "type P key in picking mode to produce a postscript file.");

  SoGui::show(widget);
  SoGui::mainLoop();

  sceneGraph->unref();
  delete examinerViewer;
  delete action;

#ifdef SoFreePackage //Cleanup
  SoPolyhedron::clearClass();
  SoCoordinateAxis::clearClass();
  SoGui::clear();  
  SoGL2PSAction::clearClass(); //After nodes.
#endif

  return 0; 
}

void keystroke(void*,SoEventCallback* eventCB){
  if (eventCB->isHandled()) return;

  const SoKeyboardEvent* event = (SoKeyboardEvent*)eventCB->getEvent();

  if (SO_KEY_PRESS_EVENT(event,P)) {
    if(examinerViewer) {
      SoDebugError::postInfo("Polyhedron","produce out.ps...");

      SoGL2PSAction* action = 
        (SoGL2PSAction*)examinerViewer->getGLRenderAction();
      action->enableFileWriting();
      examinerViewer->render();
      action->disableFileWriting();

      eventCB->setHandled();
    }
  }
}
