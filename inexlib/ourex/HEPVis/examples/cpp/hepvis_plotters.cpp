/*
  A more complex example of usage of SoPlotter.

  SoPlotter knows only the abstract classes :
   SbPlottableBins1D, SbPlottableCloud,...
 These classes permit SoPlotter to retreive
 informations about things to plot ; histogram,
 function,... The virtuality of the SbPlottable
 classes permits to plug many histogram packages
 to SoPlotter. 

  Given an histogram package <Xx> someone has to implement 
 the concrete class XxPlottableBins1D,... derived from
 SbPlottableBins1D...
 
  At run time someone create an histogram instance <Xx>Histogram
 of the package <Xx>, a corresponding instance <Xx>PlottableBins1D
 and give this instance to SoPlotter.

  Below is a dummy example with Histogram and
 PlottableHistogram classes.
*/
  

#include <HEPVis/Gui/SoGui.h>

// Inventor :
#include <Inventor/SbString.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/events/SoKeyboardEvent.h>

// Draggable SoCone :
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
//#include <Inventor/draggers/SoTranslate1Dragger.h>
//#include <Inventor/draggers/SoDragPointDragger.h>
//#include <Inventor/draggers/SoJackDragger.h>

// HEPVisUtils :
#include <HEPVis/SbMath.h>
#include <HEPVis/SbStyles.h>
#include <HEPVis/SbColors.h>
#include <HEPVis/SbSystem.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodes/SoImageWriter.h>
#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodes/SoStyle.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>
#include <HEPVis/actions/SoGL2PSAction.h>

// HEPVisGUI :
#include <HEPVis/nodekits/SoPushButton.h>
#include <HEPVis/nodekits/SoShell.h>
#include <HEPVis/nodekits/SoForm.h>

// HEPVisPlotter :
#include <HEPVis/SbPlottableThings.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoAxes2D.h>
#include <HEPVis/nodekits/SoAxis.h>
#include <HEPVis/nodekits/SoCanvas.h>
#include <HEPVis/nodekits/SoInfosRegion.h>
#include <HEPVis/nodekits/SoLegendRegion.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>

extern "C" {
  void HEPVisUtils_initClasses();
  void HEPVisGUI_initClasses();
  void HEPVisPlotter_initClasses();
}

#include <HEPVis/SbTestPlotter.h>

// Else :
#include <stdlib.h>
#include <stdio.h>

#include "SetRegion.icc"

static void demo();
static void setStyle();
static void producePostScript();
static void produceGIF();
static void produceJPEG();
static void activateCB (void*,SoNode*);
static void keystroke(void*,SoEventCallback*);

static SoCanvas* canvas = 0;
static int demoCount = 0;
//static int demoCount = 9; //To start with the draggable SoCone.
//static int demoCount = 10; // To start with 2D histos.
static SoPlotterRegion* demoPlotterRegion = NULL;
static SoNode* demoPostScript = NULL;

static SoGuiExaminerViewer* examinerViewer = NULL;
static SoImageWriter* imageWriter = NULL;

static SbTestHistogram2D* histo2D = 0;

static int styleCount = 0;

//#include "dsm.h"

int main(int aArgc,char** aArgv){
  //LookDSM_Problem(); //G.Barrand

  // Do user data analysis (here very simple !) :
  SbTestHistogram1D* histo1D = new SbTestHistogram1D;
  histo2D = new SbTestHistogram2D;

  SoGuiWidget widget=SoGui::init(aArgv[0]);
  if (widget==NULL) exit(1);

#ifdef __APPLE__
 {SbString arg0 = (aArgc>=1?aArgv[0]:"");
  SbString dir_root;
  if(SbSystemIsMacApp(arg0,dir_root)==TRUE) {    
    SbString TTFPATH = dir_root;
    TTFPATH += SbString("/Resources/HEPVis/fonts");
    SbSystemPutenv("TTFPATH",TTFPATH);
  }}
#endif

  HEPVisUtils_initClasses();
  HEPVisGUI_initClasses();
  HEPVisPlotter_initClasses();

  SbBool plotter1 = TRUE;
  SbBool plotter2 = TRUE;
  SbBool plotter3 = TRUE;

  SoSeparator* sceneGraph = new SoSeparator;
  sceneGraph->ref();

  float menu_height = 0.05F;

 {////////////// Menu //////////////////////
  SoSeparator* region = new SoSeparator;
  sceneGraph->addChild(region);

  SoViewportRegion* viewportRegion = new SoViewportRegion;
  viewportRegion->setPositionPercent(0,1-menu_height);
  viewportRegion->setSizePercent(1,menu_height);
  viewportRegion->sensitiveBorder.setValue(FALSE);
  region->addChild(viewportRegion);

  SoLightModel* lightModel = new SoLightModel;
  lightModel->model.setValue(SoLightModel::PHONG);
  region->addChild(lightModel);

  SoDirectionalLight* light = new SoDirectionalLight;
  light->color.setValue(1,1,1);
  light->direction.setValue(SbVec3f(1,-1,-1));
  region->addChild(light);

  SoOrthographicCamera* camera = new SoOrthographicCamera;
  camera->viewportMapping.setValue(SoCamera::ADJUST_CAMERA);
  camera->position.setValue(0,0,4);
  camera->orientation.setValue(SbRotation(SbVec3f(0,1,0),0));
  camera->height.setValue(2);
  camera->nearDistance.setValue(1);
  camera->farDistance.setValue(100);
  camera->focalDistance.setValue(4);
  region->addChild(camera);

  SoShell* shell = new SoShell;
  //shell->background.setValue(0.5F,0.5F,0.5F);
  region->addChild(shell);

  SoForm* form = new SoForm;
  shell->addChild(form);

  SoPushButton* prevNode; 
  // Exit :
#define BUTTON_Exit 1
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0,0);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("Exit");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_Exit);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // Next :
#define BUTTON_Next 2
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("Next");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_Next);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // PS :
#define BUTTON_PS 3
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("PS");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_PS);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // GIF :
#define BUTTON_GIF 4
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("GIF");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_GIF);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // JPEG :
#define BUTTON_JPEG 5
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("JPEG");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_JPEG);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // On top :
#define BUTTON_ON_TOP 6
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("On top");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_ON_TOP);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // Delete region :
#define BUTTON_DELETE_REGION 7
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("Delete region");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_DELETE_REGION);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

  // Style :
#define BUTTON_Style 8
 {SoPushButton* pushButton = new SoPushButton;
  pushButton->background.setValue(0.9F,0.9F,0.9F);
  pushButton->shadowThickness.setValue(0.1F);
  pushButton->marginWidth.setValue(0.1F);
  pushButton->marginHeight.setValue(0.1F);
  pushButton->labelString.setValue("Style");
  pushButton->addActivateCallback(activateCB,(void*)BUTTON_Style);
  pushButton->leftAttachment.setValue(SoPrimitive::ATTACH_NODE);
  pushButton->leftNode.setValue(prevNode);
  form->addChild(pushButton);
  prevNode = pushButton;}

 } //End menu 

  canvas = new SoCanvas;
  SoSeparator* plotters = canvas->getTopSeparator();
  sceneGraph->addChild(canvas);

  float marginx = 0.025F;
  float marginy = 0.025F;
  float width = 0.5F * (1 - 3 * marginx); 
  float height = 0.5F * (1 - menu_height - 3 * marginy); 

  if(plotter1) { // region up
    SoPlotterRegion* plotterRegion = new SoPlotterRegion;
    //plotterRegion->verbose.setValue(TRUE);
    SoViewportRegion* viewportRegion = plotterRegion->getViewportRegion();
    viewportRegion->setPositionPercent(marginx,height+2*marginy);
    viewportRegion->setSizePercent(1-2*marginx,height);
    plotterRegion->infosRegionVisible.setValue(TRUE);
    plotterRegion->legendRegionVisible.setValue(TRUE);
    plotters->addChild(plotterRegion);

   {SoInfosRegion* infosRegion = plotterRegion->getInfosRegion();
    viewportRegion = infosRegion->getViewportRegion();
    viewportRegion->setSizePercent(0.25F,0.1F);
    //viewportRegion->backgroundColor.setValue(SbColor(0.5F,0.5F,0.5F));
    //SoTextStyle* soStyle = infosRegion->getTextStyle();
    //soStyle->fontName.setValue("Hershey");
    }

    
    SoLegendRegion* legendRegion = plotterRegion->getLegendRegion();
    viewportRegion = legendRegion->getViewportRegion();
    viewportRegion->setSizePercent(0.25F,0.05F);

    SoPlotter* plotter = plotterRegion->getPlotter();
    plotter->setName("plotter_1");
    plotter->title.setValue("Title");
    plotter->setPlottableObject(new SbTestPlottableBins1D(*histo1D));
   {SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_points);
    style->color.setValue(SbColor_blue);
    style->markerStyle.setValue(SbMarkerPlus);
    style->markerSize.setValue(9);}

    plotter->getXAxis()->title.setValue("x axis title");
    plotter->getYAxis()->title.setValue("y axis title");
    plotter->getZAxis()->title.setValue("z axis title");

    demoPlotterRegion = plotterRegion;
    demoPostScript = plotters;
  }

  if(plotter2) {  // region bottom left
    SoDisplayRegion* displayRegion = new SoDisplayRegion;
    SoViewportRegion* viewportRegion = displayRegion->getViewportRegion();
    viewportRegion->setPositionPercent(marginx,marginy);
    viewportRegion->setSizePercent(width,height);
    plotters->addChild(displayRegion);

    SoSeparator* soStatic = displayRegion->getStaticScene();
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(SbColor(1,0,0));
    soStatic->addChild(material);
    SoTransform* transform = new SoTransform;
    transform->rotation.setValue(SbRotation(SbVec3f(1,1,0),1.5));
    soStatic->addChild(transform);
    soStatic->addChild(new SoCylinder);
  }

  if(plotter3) { // region bottom right
    SoPlotterRegion* plotterRegion = new SoPlotterRegion;
    SoViewportRegion* viewportRegion = plotterRegion->getViewportRegion();
    viewportRegion->setPositionPercent(2*marginx+width,marginy);
    viewportRegion->setSizePercent(width,height);
    viewportRegion->borderColor.setValue(SbColor(0,1,0));
    viewportRegion->backgroundColor.setValue(SbColor(0.8F,0.8F,0.8F));
    plotters->addChild(plotterRegion);

    SoPlotter* plotter = plotterRegion->getPlotter();
    plotter->setName("plotter_3");
    plotter->setPlottableObject(new SbTestPlottableBins1D(*histo1D));

   {SoStyle* style = plotter->getWallStyle();
    style->modeling.setValue(SbModeling_boxes);
    style->color.setValue(SbColor_lightsteelblue);}

   {SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_boxes);
    style->color.setValue(SbColor_navyblue);
    style->hatching.setValue(1);}

   {SoStyle* style = plotter->getRightHatchStyle(0);
    style->color.setValue(SbColor_white);}

    plotter->titleUp.setValue(FALSE);

   {SoTextStyle* style = plotter->getTitleStyle();
    style->color.setValue(SbColor_black);
    style->fontName.setValue("TTF/times");}

   {SoTextStyle* style = plotter->getXAxis()->getLabelsStyle();
    style->color.setValue(SbColor_black);
    style->fontName.setValue("TTF/times");}
   {SoTextStyle* style = plotter->getYAxis()->getLabelsStyle();
    style->color.setValue(SbColor_black);
    style->fontName.setValue("TTF/times");}
  }

  imageWriter = new SoImageWriter();
  sceneGraph->addChild(imageWriter);

  SoEventCallback* eventCallback = new SoEventCallback;
  eventCallback->addEventCallback(SoKeyboardEvent::getClassTypeId(),
                                  keystroke,0);
  sceneGraph->addChild(eventCallback);                                  

  ////////////////////////////////////////////////////////////

  examinerViewer = new SoGuiExaminerViewer(widget);
  const SbViewportRegion& vpRegion = examinerViewer->getViewportRegion();
  SoGL2PSAction* action = new SoGL2PSAction(vpRegion);
  action->setFileName("out.ps");
  examinerViewer->setGLRenderAction(action);
  examinerViewer->setSize(SbVec2s(700,450));
  examinerViewer->setAutoClipping(FALSE);
  examinerViewer->setViewing(FALSE);
  examinerViewer->setHeadlight(FALSE);
  examinerViewer->setDecoration(FALSE);
  examinerViewer->setBackgroundColor(SbColor(0.5F,0.5F,0.5F));
  canvas->setSetRegionCallback(new SetRegionCallback(examinerViewer));
  examinerViewer->setSceneGraph(sceneGraph);
  if(demoPlotterRegion) {
    examinerViewer->setCamera(demoPlotterRegion->getCamera()); // After setSceneGraph.
  }
  examinerViewer->show();

  SoGui::show(widget);

  SoDebugError::postInfo("Plotter",
"\
 - Next button : go to the next demo page.\n\
 - PS button : produce postscript file (out.ps) of the current page.\n\
 - P key in picking mode : produce vector PS (out.ps) of the current page.\n\
 - esc : pass in viewing mode (hand cursor).\n\
 - button3 in viewing mode : popup menu.\n\
 - Q in viewing mode : exit\n\
\n\
 In picking mode over a border of a region :\n\
 - click border + pointer move : move the visualization region.\n\
 - click corner + pointer move : resize the visualization region.");

  SoGui::mainLoop();

  //Cleanup
  canvas->setSetRegionCallback(0);
  sceneGraph->unref();
  delete examinerViewer;

  delete histo1D;
  delete histo2D;

  return EXIT_SUCCESS;
}

static void activateCB(void* aUserData,SoNode*){
  unsigned long what = (unsigned long)aUserData;
  if(what==BUTTON_Exit) {
#ifdef SoFreePackage
    SoGui::sendExit();  
#else
    exit(EXIT_SUCCESS);
#endif
  } else if(what==BUTTON_Next) {
    demo();
  } else if(what==BUTTON_PS) {
    producePostScript();
  } else if(what==BUTTON_GIF) {
    produceGIF();
  } else if(what==BUTTON_JPEG) {
    produceJPEG();
  } else if(what==BUTTON_ON_TOP) {
    SoRegion* soRegion = canvas->getHighlightedRegion();
    if(soRegion) canvas->setRegionOnTop(soRegion);
  } else if(what==BUTTON_DELETE_REGION) {
    canvas->deleteHighlightedRegion();
  } else if(what==BUTTON_Style) {
    setStyle();
  }
}

void demo(){
  // Text with SoText3 & SoText2 are really hugly !
  if(!demoPlotterRegion) return;
  SoPlotter* plotter = demoPlotterRegion->getPlotter();
  if(demoCount==0) {
    demoCount++;
    SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_boxes);
    style->color.setValue(SbColor_forestgreen);
    printf("Histogram in forestgreen.\n");
  } else if(demoCount==1) {
    demoCount++;
    SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_top_lines);
    style->color.setValue(SbColor_black);
    printf("Histogram in black and wire frame.\n");
  } else if(demoCount==2) {
    demoCount++;
    SoStyle* style = plotter->getGridStyle();
    style->visible.setValue(FALSE);
    printf("Remove grid.\n");
  } else if(demoCount==3) {
    demoCount++;
    SoStyle* style = plotter->getWallStyle();
    style->visible.setValue(FALSE);
    printf("Remove wall.\n");
  } else if(demoCount==4) {
    demoCount++;
   {SoStyle* style = plotter->getWallStyle();
    style->visible.setValue(TRUE);}
   {SoTextStyle* style = plotter->getTitleStyle();
    style->visible.setValue(FALSE);}
    printf("Remove title.\n");
  } else if(demoCount==5) {
    demoCount++;
    SoTextStyle* style = plotter->getTitleStyle();
    style->visible.setValue(TRUE);
    plotter->titleUp.setValue(FALSE);
    printf("Title down.\n");
  } else if(demoCount==6) {
    demoCount++;
    plotter->title.setValue("My title");
    plotter->titleUp.setValue(TRUE);
    printf("My title up.\n");
  } else if(demoCount==7) {
    demoCount++;
  } else if(demoCount==8) {
    demoCount++;
    SoStyle* style = plotter->getErrorsStyle(0);
    style->visible.setValue(TRUE);
    printf("Errors\n");
  } else if(demoCount==9) {
    demoCount++;

    // Draggable SoCone :
   {SoSeparator* etcSep = plotter->getEtcDataSeparator();
  
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(SbColor(1,0,0));
    etcSep->addChild(material);
  
    SoTransform* transform = new SoTransform;
    transform->scaleFactor.setValue(SbVec3f(0.1f,0.1f,0.1f));
    transform->translation.setValue(SbVec3f(0.5f,0.5f,0.5f));
    etcSep->addChild(transform); 
  
    //SoDragPointDragger* soDragger;
    SoTranslate2Dragger* soDragger;
   {SoSeparator* sep = new SoSeparator;
    etcSep->addChild(sep);
    SoTransform* transform = new SoTransform;
    transform->scaleFactor.setValue(SbVec3f(2,2,2));
    sep->addChild(transform); 
    soDragger = new SoTranslate2Dragger;
    sep->addChild(soDragger);}
  
    SoSeparator* sep = new SoSeparator;
    etcSep->addChild(sep);
    SoTranslation* trans = new SoTranslation;
    sep->addChild(trans); 
    sep->addChild(new SoCone());
  
    //trans->translation.connectFrom(&(soDragger->translation));
  
    SoCalculator* soCalc = new SoCalculator; 
    soCalc->A.connectFrom(&(soDragger->translation)); 
    soCalc->expression.setValue("oA = 2*A"); 
    //soCalc->expression.setValue("oA = vec3f(A[0],A[1],0)"); 
    trans->translation.connectFrom(&(soCalc->oA));
    } //end SoCone.

    printf("Put extra objects.\n");
  } else if(demoCount==10) {
    demoCount++;
    SoSeparator* etcSep = plotter->getEtcDataSeparator();
    etcSep->removeAllChildren();
    // Give a 2D histogram :
    plotter->setPlottableObject(new SbTestPlottableBins2D(*histo2D));
    SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_points);
    style->markerStyle.setValue(SbMarkerDot);
    printf("Histo 2D ; shape XY ; XY random.\n");
  } else if(demoCount==11) {
    demoCount++;
    SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_boxes);
    style->color.setValue(SbColor_darkslategrey);
    printf("XY box.\n");
  } else if(demoCount==12) {
    demoCount++;
    SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_wire_boxes);
    printf("XY box lines.\n");
  } else if(demoCount==13) {
    demoCount++;
    SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_texts);
    printf("XY text.\n");
  } else if(demoCount==14) {
    demoCount++;
    //FIXME : does not work anymore !
    SoStyle* style = plotter->getBinsStyle(0);
    style->painting.setValue(SbPaintingByValue);
    style->colorMapping.setValue("black 1 cyan 2 green 4 orange");
    printf("XY color.\n");
  } else if(demoCount==15) {
    demoCount++;
    //FIXME : does not work anymore !
    SoStyle* style = plotter->getBinsStyle(0);
    style->colorMapping.setValue("black 1 blue 2 green 4 red");
    printf("Another color mapping.\n");
  } else if(demoCount==16) {
    demoCount++;
    // Previous 2D (XY) manipulations may have changed 
    // automatically SoPlotter height.
    plotter->shapeAutomated.setValue(FALSE);
    plotter->shape.setValue(SoPlotter::XYZ);
    plotter->height.setValue(1);
    plotter->width.setValue(1);
    plotter->depth.setValue(1);
   {SoStyle* style = plotter->getBinsStyle(0);
    style->modeling.setValue(SbModeling_boxes);}
   {SoStyle* style = plotter->getWallStyle();
    style->visible.setValue(FALSE);}
   {SoStyle* style = plotter->getGridStyle();
    style->visible.setValue(TRUE);}
    examinerViewer->setDecoration(TRUE);
    printf("Shape XYV. Set camera to 'lego' position.\n");
  } else if(demoCount==17) {
    demoCount++;
    SoSeparator* etcSep = plotter->getEtcDataSeparator();
    SoMaterial* material = new SoMaterial;
    material->diffuseColor.setValue(SbColor(1,0,0));
    etcSep->addChild(material);
    SoTransform* transform = new SoTransform;
    transform->scaleFactor.setValue(SbVec3f(0.1f,0.1f,0.1f));
    transform->translation.setValue(SbVec3f(0.5f,0.5f,0.5f));
    etcSep->addChild(transform); 
    etcSep->addChild(new SoCone());
    printf("Put objects in extraScene.\n");
  }
}

void setStyle(){
  if(!demoPlotterRegion) return;
  if(styleCount==0) {
    styleCount++;
    demoPlotterRegion->setStylePAW_Default();
    printf("Style PAW/Default\n");
  } else if(styleCount==1) {
    styleCount++;
    demoPlotterRegion->setStyleROOT_Default();
    printf("Style ROOT/Default\n");
  } else if(styleCount==2) {
    styleCount++;
    demoPlotterRegion->setStyleROOT_Plain();
    printf("Style ROOT/Plain\n");
  } else if(styleCount==3) {
    styleCount++;
    demoPlotterRegion->setStyleROOT_Bold();
    printf("Style ROOT/Bold\n");
  } else if(styleCount==4) {
    styleCount++;
    demoPlotterRegion->setStyleROOT_Video();
    printf("Style ROOT/Video\n");
  } else if(styleCount==5) {
    styleCount++;
    demoPlotterRegion->setStyleROOT_Pub();
    printf("Style ROOT/Pub\n");
  } else if(styleCount==6) {
    styleCount++;
    demoPlotterRegion->setStyleROOT_LHCb();
    printf("Style ROOT/LHCb\n");
  } else {
    styleCount = 0;
  }
}

void producePostScript(){
  if(examinerViewer!=NULL) {
    if(imageWriter) {
      imageWriter->fileName.setValue("out.ps");
      imageWriter->format.setValue(SoImageWriter::POST_SCRIPT);
      imageWriter->enable();
      examinerViewer->render();
      imageWriter->disable();
      printf("out.ps produced.\n");
    }
  }
}

void produceGIF(){
  if(examinerViewer!=NULL) {
    if(imageWriter) {
      imageWriter->fileName.setValue("out.gif");
      imageWriter->format.setValue(SoImageWriter::GIF);
      imageWriter->enable();
      examinerViewer->render();
      imageWriter->disable();
      printf("out.gif produced.\n");
    }
  }
}

void produceJPEG(){
  if(examinerViewer!=NULL) {
    if(imageWriter) {
      imageWriter->fileName.setValue("out.jpg");
      imageWriter->format.setValue(SoImageWriter::JPEG);
      imageWriter->enable();
      examinerViewer->render();
      imageWriter->disable();
      printf("out.jpg produced.\n");
    }
  }
}

void keystroke(void*,SoEventCallback* eventCB){
  if (eventCB->isHandled()) return;

  const SoKeyboardEvent* event = (SoKeyboardEvent*)eventCB->getEvent();

  if (SO_KEY_PRESS_EVENT(event,P)) {
    if(examinerViewer) {
      SoDebugError::postInfo("Plotter","produce out.ps...");

      SoGL2PSAction* action = 
        (SoGL2PSAction*)examinerViewer->getGLRenderAction();
      action->enableFileWriting();
      examinerViewer->render();
      action->disableFileWriting();

      eventCB->setHandled();
    }
  }
}
