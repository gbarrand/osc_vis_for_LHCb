
#import <AppKit/AppKit.h>

#import <SCController.h>
#import <SCView.h>
#import <SCExaminerController.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/nodes/SoOrthographicCamera.h>
//#include <Inventor/nodes/SoLightModel.h>
//#include <Inventor/nodes/SoDirectionalLight.h>
//#include <Inventor/nodes/SoDrawStyle.h>

#include <string>

int main(int aArgc,const char* aArgv[]) {
  // Initialize NS (NextStep) :
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSApplication* app = [NSApplication sharedApplication];
  if(aArgc) {
    //NSLog(@"debug : \"%s\"\n",aArgv[0]);
    std::string arg0 = aArgv[0];
    std::string::size_type pos = arg0.rfind('/');
    if(pos==std::string::npos) pos = 0;
    else pos++;
    std::string name = arg0.substr(pos,arg0.size()-pos);
    NSString* ns = [NSString stringWithUTF8String:name.c_str()];
    BOOL status = [NSBundle loadNibNamed:ns owner:app];
    [ns release];
  }

  // Initialize Inventor :
  [SCController initCoin];

  // Create a scene :
  SoSeparator* root=new SoSeparator;
  root->ref();

  SoOrthographicCamera* soCamera = new SoOrthographicCamera();
  soCamera->viewportMapping.setValue(SoCamera::LEAVE_ALONE);
  //camera->aspectRatio.setValue(10);
  soCamera->position.setValue(0,0,10);
  soCamera->orientation.setValue(SbRotation(SbVec3f(0,1,0),0));
  soCamera->height.setValue(2);
  soCamera->nearDistance.setValue(1);
  soCamera->farDistance.setValue(100);
  soCamera->focalDistance.setValue(10);
  root->addChild(soCamera);

/*
  SoDirectionalLight* soDirLight = new SoDirectionalLight;
  soDirLight->on.setValue(FALSE);
  //soDirLight->on.setValue(TRUE); //FIXME
  soDirLight->color.setValue(1,1,1);
  // Same as Coin<Gui>/So<Gui>Viewer :
  soDirLight->direction.setValue(SbVec3f(1,-1,-10));
  root->addChild(soDirLight);

  SoLightModel* soLightModel = new SoLightModel;
  soLightModel->model.setValue(SoLightModel::BASE_COLOR);
  root->addChild(soLightModel);

  SoDrawStyle* drawStyle = new SoDrawStyle;
  drawStyle->style.setValue(SoDrawStyle::FILLED);
  root->addChild(drawStyle);
*/

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue (SbColor(1,1,0));
  //material->emissiveColor.setValue (SbColor(1,1,0));
  root->addChild(material);

  SoRotationXYZ* rot = new SoRotationXYZ;
  rot->axis = SoRotationXYZ::Y;
  rot->angle = 1.57F/2.0F;
  root->addChild(rot);

  SoScale* scale = new SoScale;
  scale->scaleFactor.setValue(0.1F,0.1F,0.1F);
  //scale->scaleFactor.setValue(0.5F,0.5F,0.5F);
  root->addChild(scale);

  SoCube* cube = new SoCube;
  root->addChild(cube);

  SCView* view = [[SCView alloc] init];
  // Create an new SCExaminerController
  SCExaminerController* examinerController = [[SCExaminerController alloc] init];
  // Create the view<->controller connection
  [examinerController setView:view];
  [view release];
  [view setController:examinerController];
  [examinerController release];
  // Set the scene graph
  [examinerController setSceneGraph:root];
  // The magic activate
  [examinerController activate];
  [view reshape];
  //[examinerController viewAll];

  [examinerController setCamera:soCamera];
  //[examinerController setHeadlightIsOn:FALSE];

  // origin = bottom, left.
  NSRect rect = {200,100,400,300};
  unsigned int mask = NSResizableWindowMask | NSTitledWindowMask;
  NSWindow* window = [[NSWindow alloc] initWithContentRect:rect styleMask:mask backing:NSBackingStoreBuffered defer:NO];
  NSString* title = [NSString stringWithUTF8String:"ExaminerViewer"];
  [window setTitle:title];
  //[title release];
  [window setShowsResizeIndicator:YES];
  [window setContentView:view];
  [window makeKeyAndOrderFront:app];

/*
  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSize(SbVec2s(700,500));
  //viewer->show();
  viewer->setTitle("CoinXt");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);
*/

  //printf("debug : mainloop.... 005\n");

  [app run];

  //Cleanup
  root->unref();

  [window release];
  [app release];

  [pool release];

  //printf("debug : exiting...\n");

  return 0;
}
