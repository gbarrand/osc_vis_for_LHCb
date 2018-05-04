// this :
#import <OnX/NextStep/NSSoExaminerViewer.h>

#import <AppKit/NSApplication.h>
#import <AppKit/NSWindow.h>
#import <SCExaminerController.h>
#import <SCView.h>

#import <Inventor/SoSceneManager.h>
#import <Inventor/nodes/SoNode.h>
#import <Lib/Debug.h>

#import <OnX/NextStep/NSAction.h>

@implementation NSSoExaminerViewer
- (id)initWithFrame:(NSRect)aRect {
  fExaminerController = 0;
  fView = 0;
  fDestroyActions = 0;
  if(self = [super init]) {
    Lib::Debug::increment("OnX::NSSoExaminerViewer");

    fDestroyActions = [[NSMutableArray alloc] init];

    fView = [[SCView alloc] initWithFrame:aRect];
    //NOTE : inheriting SCExaminerController crashes at exit.
    fExaminerController = [[SCExaminerController alloc] init];
    // Create the view<->controller connection
    [fExaminerController setView:fView];
    [fView setController:fExaminerController];
    // The magic activate
    [fExaminerController activate];
    [fView reshape];

    //FIXME [fExaminerController setHeadlight:FALSE];
  }
  return self;
}
- (void) dealloc {
  int number = [fDestroyActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fDestroyActions objectAtIndex:index] execute];
  }
  [fDestroyActions release];
  Lib::Debug::decrement("OnX::NSSoExaminerViewer");
  SoNode* node = (SoNode*)[fExaminerController userSceneGraph];
  if(node) node->ref();
  [fExaminerController setSceneGraph:nil];
  //FIXME : pb in SC21 : node refcount is not 1 !
  if(node) {
    while(true) {
      if(node->getRefCount()==1) {    
        node->unref();
        break;
      } else {
        node->unref();
      }
    }
  } 
  [fView release];
  [fExaminerController release];
  [super dealloc];  
}
- (void)addDestroyAction:(OnX_NSAction*)aAction {
  [fDestroyActions addObject:aAction];
}
- (void)setSceneGraph:(SoNode*)aNode {
  [fExaminerController setSceneGraph:(SoGroup*)aNode];
}
- (SoNode*)sceneGraph {
  return (SoNode*)[fExaminerController userSceneGraph];
}
- (NSView*)view {
  return fView;
}
- (void)viewAll {
  [fExaminerController viewAll];
}
- (void)render {
  [fExaminerController render];
}
- (BOOL)isHeadlight {
  return [fExaminerController headlightIsOn];
}
- (void)setHeadlight:(BOOL)aFlag {
  [fExaminerController setHeadlightIsOn:aFlag];
}
- (void)setBackgroundColor:(float)aR:(float)aG:(float)aB {
  NSColor* color = [NSColor colorWithDeviceRed:aR
                                        green:aG
                                         blue:aB
                                        alpha:1];
  [fExaminerController setBackgroundColor:color];
}
- (NSColor*)backgroundColor {
  return [fExaminerController backgroundColor];
}
- (SoGLRenderAction*)renderAction {
  SoSceneManager* sm = [fExaminerController sceneManager];
  if(!sm) return 0;
  return sm->getGLRenderAction();
}
- (SoSceneManager*)sceneManager {
  return [fExaminerController sceneManager];
}
- (void)setRenderAction:(SoGLRenderAction*)aAction {
  SoSceneManager* sm = [fExaminerController sceneManager];
  if(!sm) return;
  sm->setGLRenderAction(aAction);
}
@end
