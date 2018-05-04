
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>

int main(int argc,char** argv) {

  QWidget* top = SoQt::init(argv[0]);
  if (top==NULL) exit(1);

  // Create a scene :

  SoSeparator* root = new SoSeparator;
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

  SoQtExaminerViewer* viewer = new SoQtExaminerViewer(top);
  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(700,500));
  viewer->setTitle("CoinQt");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);
  viewer->show();

  SoQt::show(top);

  SoQt::mainLoop();

  //Cleanup
  root->unref();

  delete viewer;

  return 0;
}
