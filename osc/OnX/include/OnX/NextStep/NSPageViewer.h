#ifndef OnX_NSPageViewer_h
#define OnX_NSPageViewer_h 

// Inheritance :
#import <OnX/NextStep/NSSoExaminerViewer.h>

class SoPage;

@interface NSPageViewer : NSSoExaminerViewer {
  SoPage* fPage;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (SoPage*)page;
- (SoNode*)sceneGraph;
@end

#endif
