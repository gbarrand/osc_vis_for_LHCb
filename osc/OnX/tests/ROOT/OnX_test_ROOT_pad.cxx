
// InventorXt :
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

// HEPVis :
#include <HEPVis/nodes/SoTextTTF.h>
#include <HEPVis/nodes/SoViewportRegion.h>

// RioGrande :
#include <TROOT.h>
#include <TRandom.h>

// RootAnalysis :
#include <TH1.h>

// RootGraphics :
#include <THistPainter.h>
#include <TEllipse.h>

// OnX :
#include <OnX/ROOT/SoRootPad.h>

TROOT root("OnX_ROOT_pad","OnX_ROOT_pad");

int main (int,char** aArgv) {

  Widget top = SoXt::init(aArgv[0]);
  if (top==NULL) exit(1);

  SoViewportRegion::initClass();
  SoTextTTF::initClass();

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  // Create a scene :
  SoSeparator* root = new SoSeparator;
  root->ref();

  SoViewportRegion* viewportRegion = new SoViewportRegion;
  //viewportRegion->origin.setValue(SbVec2f(0.25F,0.25F));
  //viewportRegion->size.setValue(SbVec2f(0.5F,0.5F));
  root->addChild(viewportRegion);

  SoOrthographicCamera* camera = new SoOrthographicCamera;
  camera->viewportMapping.setValue(SoCamera::LEAVE_ALONE);
  camera->position.setValue(0,0,4);
  camera->nearDistance.setValue(1);
  camera->farDistance.setValue(100);
  root->addChild(camera);

  //SoDrawStyle* drawStyle = new SoDrawStyle;
  //drawStyle->style.setValue(SoDrawStyle::POINTS);
  //root->addChild(drawStyle);

  SoLightModel* lightModel = new SoLightModel;
  lightModel->model.setValue(SoLightModel::BASE_COLOR);
  root->addChild(lightModel);

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue(1,1,1);
  root->addChild(material);

  SoSeparator* separator = new SoSeparator;
  root->addChild(separator);

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  gPad = new RootLab::Pad(separator);
  
  TH1D* histogram = new TH1D("name","title",100,-5,5);
  for(int count=0;count<10000;count++)  
    histogram->Fill(gRandom->Gaus(0,1));

  new THistPainter();
  
  // Add the histogram in the pad list :
  histogram->Draw(); 

  // Add the histogram in the pad list :
  //TEllipse* ellipse = new TEllipse(0,0,1,10);
  //ellipse->Draw(); 

  // Create the iv scene graph for the objects in the pad list :
  gPad->Paint(""); 
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  // Create a viewer :
  SoXtExaminerViewer* viewer = new SoXtExaminerViewer(top);
  //viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->setAutoClipping(FALSE);
  viewer->setViewing(FALSE);
  viewer->setHeadlight(FALSE);
  viewer->setDecoration(FALSE);
  viewer->setTitle("RootLab");
  viewer->setSize(SbVec2s(400,450));
  viewer->show();

  SoXt::show(top);
  SoXt::mainLoop();

  //Cleanup
  root->unref();
  delete viewer;
#ifdef SoFreePackage
  SoViewportRegion::clearClass();
  SoTextTTF::clearClass();
  SoXt::clear();  
#endif
  return 0;
}
