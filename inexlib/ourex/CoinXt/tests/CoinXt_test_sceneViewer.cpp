
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>

int main (int aArgc,char** aArgv) {
 {//  Put things in a block so that all SbName are cleaned up
  // when we do a SoXt::clear (needed for SunOS/CC compiler ) :

  if(aArgc==1) {
    printf("Usage :\n\
[path]SceneViewer.exe <iv file> [-bbox]\n");
    return EXIT_FAILURE;
  }

  Widget myWindow=SoXt::init(aArgv[0]);
  if (myWindow==NULL) exit(1);

  SoInput input;
  if(input.openFile(aArgv[1])==FALSE) {
    printf("Can't open file %s\n",aArgv[1]);
    return EXIT_FAILURE;
  }

  SoSeparator* root = SoDB::readAll(&input);
  if(root==NULL) {
    printf("Can't read file %s\n",aArgv[1]);
  } else {
    root->ref();
  }
  input.closeFile();

  if((aArgc==3)&&(strcmp(aArgv[2],"-bbox")==0)) {
    SbViewportRegion vpRegion;
    SoGetBoundingBoxAction* getBBAction = new SoGetBoundingBoxAction(vpRegion);
    getBBAction->apply(root);
    SbVec3f center = getBBAction->getCenter();
    SbVec3f bbmin = getBBAction->getBoundingBox().getMin();
    SbVec3f bbmax = getBBAction->getBoundingBox().getMax();
    printf("center : %g %g %g\n",center[0],center[1],center[2]);
    printf("min    : %g %g %g\n",bbmin[0],bbmin[1],bbmin[2]);
    printf("max    : %g %g %g\n",bbmax[0],bbmax[1],bbmax[2]);
    delete getBBAction;
  }

  // View the new object in an Examiner Viewer
  //SoXtExaminerViewer* viewer=new SoXtExaminerViewer(NULL);
  SoXtExaminerViewer* viewer = new SoXtExaminerViewer(myWindow);
    
  //viewer->setBackgroundColor(SbColor(1.0,1.0,1.0));
  if(root!=NULL) viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(600,400));
  viewer->show();
  viewer->setTitle("CoinXt/SceneViewer");
  //viewer->setAutoClipping(FALSE);

  SoXt::show(myWindow);
  SoXt::mainLoop();

  //Cleanup
  if(root!=NULL) root->unref();

  delete viewer;}

#ifdef SoFreePackage
  SoXt::clear();  
#endif

  return EXIT_SUCCESS; 
}

