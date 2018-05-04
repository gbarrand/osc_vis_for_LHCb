#include <HEPVis/Gui/SoGui.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include <HEPVis/SbArguments.h>
#include <HEPVis/SbSystem.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoTextTTF.h>
#include <HEPVis/actions/SoGL2PSAction.h>
#include <HEPVis/actions/SoAlternateRepAction.h>

#include <stdlib.h>

static void keystroke(void*,SoEventCallback*);

static SoGuiExaminerViewer* examinerViewer = NULL;

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

  SoGuiWidget widget = SoGui::init(aArgv[0]);
  if (widget==NULL) exit(1);

  SbString font;
  args.find("-font",font);
  SbBool smooth = args.isAnArgument("-smooth");

#ifdef __APPLE__
 {SbString arg0 = (aArgc>=1?aArgv[0]:"");
  SbString dir_root;
  if(SbSystemIsMacApp(arg0,dir_root)==TRUE) {    
    SbString TTFPATH = dir_root;
    TTFPATH += SbString("/Resources/HEPVis/fonts");
    SbSystemPutenv("TTFPATH",TTFPATH);
  }}
#endif

  SoTextTTF::initClass();
  SoGL2PSAction::initClass(); //After nodes.
  SoAlternateRepAction::initClass(); //For SoTools::write.

  SoSeparator* sceneGraph = new SoSeparator;
  sceneGraph->ref();

  // Set up the material for the object to be drawn

 {SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(1, 1, 1);
  material->transparency = 0.2F;
  sceneGraph->addChild(material);

  SoCube* cube = new SoCube;
  sceneGraph->addChild(cube);}

 {SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(1, 0, 0);
  sceneGraph->addChild(material);

  SoTransform* transform = new SoTransform;
  transform->translation.setValue(1.1F,1.1F,0);
  sceneGraph->addChild(transform);

  SoTextTTF* textTTF = new SoTextTTF();
  textTTF->rescale.setValue(200);
  textTTF->size.setValue(10); // 13 pixels.
  //textTTF->size.setValue(30); // 13 pixels.
  textTTF->horizontalJustification.setValue(SoTextTTF::CENTER);
  //textTTF->horizontalJustification.setValue(SoTextTTF::RIGHT);
  //textTTF->align.setValue(SoTextTTF::RIGHT);
  //textTTF->align.setValue(SoTextTTF::CENTER);
  if(font!="") textTTF->fontName.setValue(font);
  textTTF->smoothing.setValue(smooth);
  textTTF->string.set1Value(0,"abcdefghijklmnopqrstuvwxyz");
  textTTF->string.set1Value(1,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  textTTF->string.set1Value(2,"0123456789");
  textTTF->string.set1Value(3,"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{}~|");
  sceneGraph->addChild(textTTF);}

 {SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(0, 1, 1);
  sceneGraph->addChild(material);

  SoTransform* transform = new SoTransform;
  transform->translation.setValue(0,-10.0F,1);
  sceneGraph->addChild(transform);

  SoTextTTF* textTTF = new SoTextTTF();
  textTTF->fontName.setValue("symbol");
  textTTF->rescale.setValue(200);
  textTTF->size.setValue(10); // 13 pixels.
  //textTTF->size.setValue(30); // 13 pixels.
  textTTF->horizontalJustification.setValue(SoTextTTF::CENTER);
  //textTTF->horizontalJustification.setValue(SoTextTTF::RIGHT);
  //textTTF->align.setValue(SoTextTTF::RIGHT);
  //textTTF->align.setValue(SoTextTTF::CENTER);
  if(font!="") textTTF->fontName.setValue(font);
  textTTF->smoothing.setValue(smooth);
  textTTF->string.set1Value(0,"abcdefghijklmnopqrstuvwxyz");
  textTTF->string.set1Value(1,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  textTTF->string.set1Value(2,"0123456789");
  textTTF->string.set1Value(3,"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{}~|");
  sceneGraph->addChild(textTTF);}

  // Formula :
 {SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(0, 1, 0);
  sceneGraph->addChild(material);

  SoTransform* transform = new SoTransform;
  transform->translation.setValue(0,-1.1F,0);
  sceneGraph->addChild(transform);

  SoTextTTF* formulas = new SoTextTTF();
  formulas->rescale.setValue(200);
  formulas->size.setValue(10); // 13 pixels.
  //formulas->horizontalJustification.setValue(SoTextTTF::CENTER);
  //formulas->horizontalJustification.setValue(SoTextTTF::RIGHT);
  //formulas->align.setValue(SoTextTTF::RIGHT);
  //formulas->align.setValue(SoTextTTF::CENTER);
  if(font!="") formulas->fontName.setValue(font);
  formulas->smoothing.setValue(smooth);
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
  formulas->string.set1Value(6,"[h]^2! / ndf");
  sceneGraph->addChild(formulas);}

  // Formula :
 {SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(1, 1, 0);
  sceneGraph->addChild(material);

  SoTransform* transform = new SoTransform;
  transform->translation.setValue(-3,-2,0);
  sceneGraph->addChild(transform);

  SoTextTTF* formulas = new SoTextTTF();
  formulas->encoding.setValue("PAW");
  formulas->rescale.setValue(200);
  formulas->size.setValue(10); // 13 pixels.
  formulas->rotated.setValue(TRUE); // 13 pixels.
  if(font!="") formulas->fontName.setValue(font);
  formulas->smoothing.setValue(smooth);
  formulas->string.set1Value(0,"W^-! -> [m]^-! |[n]?[m]!");
  formulas->string.set1Value(1,"p?T!&^1! (GeV)");
  sceneGraph->addChild(formulas);}

  SoEventCallback* eventCallback = new SoEventCallback;
  eventCallback->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                                  keystroke,0);
  sceneGraph->addChild(eventCallback);                                  

  //G.Barrand :
  if(args.isAnArgument("-write")) {
    SbBool altRep = args.isAnArgument("-altrep")?TRUE:FALSE;
    SbString file;
    args.find("-file",file);
    if(file=="") file = "out.iv";
    SoTools::write(*sceneGraph,file,"",FALSE,altRep);
  }

  examinerViewer = new SoGuiExaminerViewer(widget);
  const SbViewportRegion& vpRegion = examinerViewer->getViewportRegion();
  SoGL2PSAction* action = new SoGL2PSAction(vpRegion);
  action->setFileFormat(SoGL2PSAction::EPS);
  action->setFileName("out.ps");
  //action->setFileFormat(SoGL2PSAction::SVG);
  //action->setFileName("out.svg");
#ifdef HEPVisWin
  HEPVis_menuBar_set
#endif
  examinerViewer->setSize(SbVec2s(400,400));
  examinerViewer->setGLRenderAction(action);
  examinerViewer->setSceneGraph(sceneGraph);
  examinerViewer->viewAll();
  examinerViewer->show();

  SoDebugError::postInfo("TextTTF","\
\n\
type P key in picking mode to produce a out.ps postscript file.\n\
");

  SoGui::show(widget);
  SoGui::mainLoop();

  sceneGraph->unref();
  delete examinerViewer;
  examinerViewer = 0;
  delete action;

#ifdef SoFreePackage //Cleanup
  SoTextTTF::clearClass();
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
      SoDebugError::postInfo("TextTTF","produce out.[ps,svg]...");

      SoGL2PSAction* action = 
        (SoGL2PSAction*)examinerViewer->getGLRenderAction();
      action->enableFileWriting();
      examinerViewer->render();
      action->disableFileWriting();

      eventCB->setHandled();
    }
  }
}

