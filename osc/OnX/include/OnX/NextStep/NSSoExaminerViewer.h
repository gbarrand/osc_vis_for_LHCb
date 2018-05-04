#ifndef OnX_NSSoExaminerViewer_h
#define OnX_NSSoExaminerViewer_h 

// Inheritance :
#import <Foundation/NSObject.h>

#import <Foundation/NSGeometry.h>

@class NSView;
@class SCExaminerController;
@class NSMutableArray;
@class SCView;
@class OnX_NSAction;
@class NSColor;

class SoNode;
class SoGLRenderAction;
class SoSceneManager;

@interface NSSoExaminerViewer : NSObject {
  SCExaminerController* fExaminerController;
  SCView* fView;
  NSMutableArray* fDestroyActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)setSceneGraph:(SoNode*)aNode;
- (SoNode*)sceneGraph;
- (NSView*)view;
- (void)addDestroyAction:(OnX_NSAction*)aAction;
- (void)viewAll;
- (void)render;
- (void)setBackgroundColor:(float)aR:(float)aG:(float)aB;
- (NSColor*)backgroundColor;
- (BOOL)isHeadlight;
- (void)setHeadlight:(BOOL)yn;
- (SoGLRenderAction*)renderAction;
- (SoSceneManager*)sceneManager;
- (void)setRenderAction:(SoGLRenderAction*)aAction;
@end

#endif
