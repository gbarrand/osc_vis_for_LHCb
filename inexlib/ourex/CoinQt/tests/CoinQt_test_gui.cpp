
// Now over Qt4. 2 March 2006.

#include <QtGui/qpushbutton.h> //G.Barrand QtGui
#include <QtGui/qboxlayout.h> //G.Barrand QtGui

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>

int main(int,char** aArgv) {
  QWidget* top = SoQt::init(aArgv[0]);
  if (top==NULL) exit(1);

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

  QVBoxLayout* box_layout = new QVBoxLayout;

  QWidget* box1 = new QWidget();
  box_layout->addWidget(box1);
  SoQtExaminerViewer* viewer = new SoQtExaminerViewer(box1);
  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(700,500));
  viewer->setTitle("CoinQt");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);
  viewer->show();

 {QWidget* box2 = new QWidget();
  QHBoxLayout* box2_layout = new QHBoxLayout;
  box2_layout->addWidget(new QPushButton("Button 1"));
  box2_layout->addWidget(new QPushButton("Button 2"));
  box2->setLayout(box2_layout);
  box_layout->addWidget(box2);}

  top->setLayout(box_layout);

  SoQt::show(top);

  SoQt::mainLoop();

  //Cleanup
  root->unref();

  delete viewer;

  //SoQt::clear();  

  return 0;
}
