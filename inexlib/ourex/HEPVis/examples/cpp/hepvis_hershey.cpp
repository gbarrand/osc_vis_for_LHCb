#include <HEPVis/Gui/SoGui.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include <HEPVis/SbArguments.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoTextHershey.h>
#include <HEPVis/actions/SoGL2PSAction.h>
#include <HEPVis/actions/SoAlternateRepAction.h>

#include <stdlib.h>
#include <iostream>

void keystroke(void*,SoEventCallback*);

SoGuiExaminerViewer* examinerViewer = NULL;

#ifdef HEPVisWin
#include "MenuBar.ic"
HEPVis_menuBar_def
#endif

//////////////////////////////////////////////////////////////////////////////
int main (
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  SbArguments args(aArgc,aArgv);
  if(args.isAnArgument("-h")) {
    std::cout << "HEPVis::TextHershey :" << std::endl;
    std::cout << "Usage :" << std::endl;
    std::cout << " OS> TextHershey<GUI> <options>" << std::endl;
    std::cout << "Options :" << std::endl;
    std::cout << " -h : to get help." << std::endl;
    std::cout << " -altrep : to produce an alternate representation." 
              << std::endl;
    std::cout << " -write : to write scene in a file (default is out.iv)." 
              << std::endl;
    std::cout << " -file : name of the file (with .iv or .wrl extension)." 
              << std::endl;
    std::cout << " -what=[1,2,3] : what to put in the scene." << std::endl;
    std::cout << "For example :" << std::endl;
    std::cout << " OS> TextHersheyXt -altrep -write" << std::endl;
    std::cout << " UNIX> ivview out.iv" << std::endl;
    return 0;
  }

  SoGuiWidget widget = SoGui::init(aArgv[0]);
  if (widget==NULL) exit(1);

  SbString what;
  args.find("-what",what);

  float HEIGHT = 1;

  SoTextHershey::initClass();
  SoGL2PSAction::initClass(); //After nodes.
  SoAlternateRepAction::initClass(); //For SoTools::write.

  SoSeparator* sceneGraph = new SoSeparator;
  sceneGraph->ref();

  // Set up the material for the object to be drawn

 {SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(1, 1, 1);
  material->emissiveColor.setValue(1, 1, 1); //For VRML2.
  material->transparency = 0.2F;
  sceneGraph->addChild(material);

  SoCube* cube = new SoCube;
  sceneGraph->addChild(cube);}

  if( (what=="1") || (what=="") ) {
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(1, 0, 0);
    material->emissiveColor.setValue(1, 0, 0); //For VRML2.
    sceneGraph->addChild(material);

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(1.1F,1.1F,0);
    sceneGraph->addChild(transform);

    SoTextHershey* textHershey = new SoTextHershey();
    textHershey->height.setValue(HEIGHT);
    textHershey->horizontalJustification.setValue(SoTextHershey::CENTER);
    //textHershey->horizontalJustification.setValue(SoTextHershey::RIGHT);
    //textHershey->align.setValue(SoTextHershey::RIGHT);
    //textHershey->align.setValue(SoTextHershey::CENTER);
    textHershey->string.set1Value(0,"abcdefghijklmnopqrstuvwxyz");
    textHershey->string.set1Value(1,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    textHershey->string.set1Value(2,"0123456789");
    textHershey->string.set1Value(3,"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{}~");
    sceneGraph->addChild(textHershey);

   {SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(0, 1, 1);
    material->emissiveColor.setValue(0, 1, 1); //For VRML2.
    sceneGraph->addChild(material);

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(0,-10.0F,0);
    sceneGraph->addChild(transform);

    SoTextHershey* textHershey = new SoTextHershey();
    textHershey->font.setValue(SoTextHershey::GREEK);
    textHershey->height.setValue(HEIGHT);
    textHershey->horizontalJustification.setValue(SoTextHershey::CENTER);
    //textHershey->horizontalJustification.setValue(SoTextHershey::RIGHT);
    //textHershey->align.setValue(SoTextHershey::RIGHT);
    //textHershey->align.setValue(SoTextHershey::CENTER);
    textHershey->string.set1Value(0,"abcdefghijklmnopqrstuvwxyz");
    textHershey->string.set1Value(1,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    textHershey->string.set1Value(2,"0123456789");
    textHershey->string.set1Value(3,"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{}~");
    sceneGraph->addChild(textHershey);}

  }

  // Formula :
  if(what=="2") {
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(0, 1, 0);
    material->emissiveColor.setValue(0, 1, 0); //For VRML2.
    sceneGraph->addChild(material);

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(0,-1.1F,0);
    sceneGraph->addChild(transform);

    SoTextHershey* formulas = new SoTextHershey();
    formulas->height.setValue(HEIGHT);
    //formulas->horizontalJustification.setValue(SoTextHershey::CENTER);
    //formulas->horizontalJustification.setValue(SoTextHershey::RIGHT);
    //formulas->align.setValue(SoTextHershey::RIGHT);
    //formulas->align.setValue(SoTextHershey::CENTER);
    formulas->encoding.setValue("PAW");
    // PAW escape sequences :
    //  [ go to greek (roman = default)
    //  ] end of greek
    //  ! go to normal level of script
    //  ^ go to superscript
    //  ? go to subscript
    //  & backscpace one charachter
    // Extension :
    //  | draw a bar over one character
    formulas->string.set1Value(0,"[g] -> e^+! e^-");
    formulas->string.set1Value(1,"Z^0! -> e^+! e^-");
    formulas->string.set1Value(2,"W^+! -> e^+! [n]?e!");
    formulas->string.set1Value(3,"W^-! -> [m]^-! |[n]?[m]!");
    //Latex : "p_{T} (GeV)"
    formulas->string.set1Value(4,"p?T!&^1! (GeV)");
    //Latex : "E d^{3}#sigma/dp^{3} (b/GeV^{2})"
    formulas->string.set1Value(5,"E d^3![s]/dp^3! (b/GeV^2!)");
    formulas->string.set1Value(6,"[v]^2! / ndf");
    sceneGraph->addChild(formulas);
  }

  // Formula :
  if(what=="3") {
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(1, 1, 0);
    material->emissiveColor.setValue(1, 1, 0); //For VRML2.
    sceneGraph->addChild(material);

    SoTransform* transform = new SoTransform;
    transform->translation.setValue(-3,-2,0);
    sceneGraph->addChild(transform);

    SoTextHershey* formulas = new SoTextHershey();
    formulas->encoding.setValue("PAW");
    formulas->height.setValue(HEIGHT);
    //formulas->rotated.setValue(TRUE); // 13 pixels.
    formulas->string.set1Value(0,"W^-! -> [m]^-! |[n]?[m]!");
    formulas->string.set1Value(1,"p?T!&^1! (GeV)");
    sceneGraph->addChild(formulas);
  }

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
#ifdef HEPVisWin
  HEPVis_menuBar_set
#endif
  examinerViewer->setSize(SbVec2s(400,400));
  examinerViewer->setGLRenderAction(action);
  examinerViewer->setSceneGraph(sceneGraph);
  examinerViewer->viewAll();
  examinerViewer->show();

  SoDebugError::postInfo("TextHershey",
    "type P key in picking mode to produce a postscript file.");

  SoGui::show(widget);
  SoGui::mainLoop();

  sceneGraph->unref();
  delete examinerViewer;
  delete action;

#ifdef SoFreePackage //Cleanup
  SoTextHershey::clearClass();
  SoGL2PSAction::clearClass();

  SoGui::clear();  
#endif

  return 0; 
}
void keystroke(void*,SoEventCallback* eventCB){
  if (eventCB->isHandled()) return;

  const SoKeyboardEvent* event = (SoKeyboardEvent*)eventCB->getEvent();

  if (SO_KEY_PRESS_EVENT(event,P)) {
    if(examinerViewer) {
      SoDebugError::postInfo("TextHershey","produce out.ps...");

      SoGL2PSAction* action = 
        (SoGL2PSAction*)examinerViewer->getGLRenderAction();
      action->enableFileWriting();
      examinerViewer->render();
      action->disableFileWriting();

      eventCB->setHandled();
    }
  }
}

