
#include <stdlib.h> //KCC : to define NULL before glib.h.

#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/engines/SoTimeCounter.h>

int main(int,char** aArgv) {
  GtkWidget* top = SoGtk::init(aArgv[0]);
  if (top==NULL) exit(1);

  SoGtkExaminerViewer* viewer = new SoGtkExaminerViewer(top);

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

  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(700,500));
  viewer->show();
  viewer->setTitle("CoinGtk++");
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);

  SoGtk::show(top);

  // FIXME : compell the viewing cursor (ok if done after show) :
  //viewer->setViewing(TRUE); 

  SoGtk::mainLoop();

  //Cleanup
  cube->width.disconnect();
  root->unref();

  delete viewer;

  //SoGtk::clear();  

  return 0;
}
