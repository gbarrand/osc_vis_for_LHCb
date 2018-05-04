
#import <AppKit/AppKit.h>

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,const char* aArgv[]
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Initialize NS (NextStep) :
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSApplication* app = [NSApplication sharedApplication];
  //[app loadNibNamed:@"myMain" owner::app];

  // origin = bottom, left.
  NSRect rect = {200,100,400,300};
  unsigned int mask = NSResizableWindowMask | NSTitledWindowMask;
  NSWindow* window = [[NSWindow alloc] initWithContentRect:rect styleMask:mask backing:NSBackingStoreBuffered defer:NO];
  NSString* title = [NSString stringWithCString:"ExaminerViewer"];
  [window setTitle:title];
  //[title release];
  [window setShowsResizeIndicator:YES];
  //[window setContentView:view];
  [window makeKeyAndOrderFront:app];

  [app run];

  [window release];
  [app release];

  [pool release];

  //printf("debug : exiting...\n");

  return 0;
}
