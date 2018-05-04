// this :
#import <OnX/NextStep/NSPageViewer.h>

#import <AppKit/NSApplication.h>
#import <AppKit/NSWindow.h>
#import <SCExaminerController.h>
#import <SCView.h>

#import <Inventor/SoSceneManager.h>
#import <HEPVis/misc/SoTools.h>
#import <HEPVis/nodekits/SoPage.h>
#import <HEPVis/nodekits/SoRegion.h>
#import <HEPVis/actions/SoGL2PSAction.h>

#import <Lib/Debug.h>

#import <Onx/NextStep/NSAction.h>

class PageSetRegionCallback : public virtual SbSetRegionCallback {
public:
  PageSetRegionCallback(SCExaminerController* aExaminerController)
   :fExaminerController(aExaminerController){}
  virtual void setCamera(SoCamera* aCamera) {
    [fExaminerController setCamera:aCamera];
  }
private:
  SCExaminerController* fExaminerController;
};

@implementation NSPageViewer
- (id)initWithFrame:(NSRect)aRect {
  fPage = 0;
  if(self = [super initWithFrame:aRect]) {
    Lib::Debug::increment("OnX::NSPageViewer");

    SoSceneManager* sceneManager = [fExaminerController sceneManager];
    if(sceneManager) {
      const SbViewportRegion& vpRegion = sceneManager->getViewportRegion();
      SoGL2PSAction* action = new SoGL2PSAction(vpRegion);
      sceneManager->setGLRenderAction(action);
    }
 
    fPage = new SoPage;
    fPage->ref();
    // SetRegionCallback object deleted by the SoPage.
    fPage->setSetRegionCallback
      (new PageSetRegionCallback(fExaminerController));
    
    [fExaminerController setSceneGraph:(SoGroup*)fPage];

    fPage->highlightRegion(fPage->getRootRegion(0));
  }
  return self;
}
- (void) dealloc {
  Lib::Debug::decrement("OnX::NSPageViewer");
  [super dealloc];  
}
- (SoPage*)page {return fPage;}
- (SoNode*)sceneGraph {return fPage;}
@end
