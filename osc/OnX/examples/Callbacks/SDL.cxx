//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

//
// SDL examples related callbacks.
//

#include <Slash/Core/ISession.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/Tools/Inventor.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoSearchAction.h>

#include <HEPVis/SbMath.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoGrid.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

#include <HEPVis/nodekits/SoDetectorTreeKit.h>
#include <HEPVis/nodekits/SoDetectorExample.h>

static void detPickCB(void*,void*);

extern "C" {

namespace OnX {
class add_pickCB : public virtual SbVisitor {
public:
  virtual ~add_pickCB(){}
public:
  virtual SbBool visit(SoFullPath& aPath) {
    SoNode* node = aPath.getTail();
    if(!node) return TRUE;
    SoDetectorTreeKit* kit = (SoDetectorTreeKit*)node;
    kit->addPickCallback(fCB,fTag);
    return TRUE;
  }
public:
  SoCallbackListCB* fCB;
  void* fTag;
};
}

void OnX_SDL_default_scene(Slash::UI::IUI& aUI) {
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;

  SoViewportRegion* vpr = soRegion->getViewportRegion();
  if(!vpr) return;
  vpr->horizontalBorder.setValue(0);
  vpr->verticalBorder.setValue(0);
  vpr->horizontalHighlightBorder.setValue(0);
  vpr->verticalHighlightBorder.setValue(0);

  if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) return;
  SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;

  displayRegion->setCameraType(SoPerspectiveCamera::getClassTypeId());
  SoPerspectiveCamera* soCamera = 
    (SoPerspectiveCamera*)displayRegion->getCamera();
  soCamera->viewportMapping.setValue(SoCamera::ADJUST_CAMERA);
  float d = 100;
  soCamera->position.setValue(0,2,d); //y = 2 (meter)
  soCamera->focalDistance.setValue(d);
  soCamera->orientation.setValue(SbRotation(SbVec3f(0,1,0),0));
  soCamera->nearDistance.setValue(d*0.01F);
  soCamera->farDistance.setValue(100*d);
  soCamera->heightAngle.setValue(FM_PI/4.0F);

 {Slash::UI::IWidget* widget = aUI.currentWidget();
  if(!widget) return;
  float dr = 2.0F * 3.1416F/180.0F;
  widget->setParameter("deltaRotation",inlib::to<float>(dr));
  widget->setParameter("deltaTranslation","3");} //default is 1.

  SoSeparator* scene = (SoSeparator*)displayRegion->getStaticScene();
  if(!scene) return;
  scene->removeAllChildren();

  soRegion->color.setValue(0.8F,0.8F,0.8F);

  // Light at ceil :
  SoPointLight* light = new SoPointLight;
  light->location.setValue(0,100,0); //y = 100.
  scene->addChild(light);

  ///////////////////////////////////////////////
  // Floor :
  ///////////////////////////////////////////////
  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(0.8F,0.8F,0.8F);
  scene->addChild(material);

  SoGrid* floor = new SoGrid;
  float halfSize = 1000;
  floor->position.setValue(SbVec3f(-halfSize,0,halfSize)); //y = 0
  floor->X.setValue(SbVec3f(1,0,0));
  floor->Y.setValue(SbVec3f(0,0,-1));
  floor->width.setValue(2.0F*halfSize);
  floor->height.setValue(2.0F*halfSize);
  scene->addChild(floor);

  ///////////////////////////////////////////////
  // Detector :
  ///////////////////////////////////////////////
 {SoSeparator* soSepDet = new SoSeparator;
  scene->addChild(soSepDet);

  SoTranslation* trans = new SoTranslation;
  trans->translation.setValue(SbVec3f(0,20,0)); //y = 20
  soSepDet->addChild(trans);

  SoRotationXYZ* soRotation = new SoRotationXYZ; //For animation.
  soRotation->axis.setValue(SoRotationXYZ::Y);
  //soRotation->angle.setValue(FM_PI_2);
  soSepDet->addChild(soRotation);

  SoDetectorExample* soDetExa = new SoDetectorExample;
  soSepDet->addChild(soDetExa);

  OnX::add_pickCB v;
  v.fCB = detPickCB;
  v.fTag = &aUI;
  SoTools::visit(*soDetExa,SoDetectorTreeKit::getClassTypeId(),v);
  }

}

// used by the OnX_SDL_cyclic (see SDL.onx).
void OnX_SDL_rotate_update(Slash::UI::IUI& aUI){
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) return;
  SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;
  SoSeparator* scene = (SoSeparator*)displayRegion->getStaticScene();
  if(!scene) return;

  SoSearchAction searchAction;
  searchAction.setFind(SoSearchAction::TYPE);
  searchAction.setType(SoRotationXYZ::getClassTypeId());
  searchAction.apply(scene);

  SoPath* path = searchAction.getPath();
  if(!path) return;

  SoRotationXYZ* soRotation = (SoRotationXYZ*)path->getTail();
  float angle = soRotation->angle.getValue();
  float one_degree = 3.14/360.0;
  soRotation->angle.setValue(angle+one_degree);

}

} // extern "C"

static void detPickCB(void* aTag,void* aNode){
  SoDetectorTreeKit* soDet = (SoDetectorTreeKit*)((SoNode*)aNode);
  ::printf("OnX_SDL_default_scene::detPickCB : %s\n",
    soDet->getName().getString());

  SoNode* node = soDet->getPart("transform",TRUE);
  if(node) {
    Slash::UI::IUI* ui = (Slash::UI::IUI*)aTag;

   {Slash::UI::IWidget* widget = ui->currentWidget();
    if(widget) widget->setParameter("showCursor","true");

    Slash::UI::ISoViewer* soViewer = Slash::cast_SoViewer(*widget);
    if(soViewer) {
      SoNode* sceneGraph = soViewer->sceneGraph();
      SoTools::setManip(sceneGraph,(SoTransform*)node);
    }}
  }

}
