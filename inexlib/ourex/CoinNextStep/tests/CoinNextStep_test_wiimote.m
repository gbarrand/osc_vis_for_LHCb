
#import <AppKit/AppKit.h>

#import "MyWiiRemote.h"

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
@interface AppDelegate : NSObject {
#else
@interface AppDelegate : NSObject <NSApplicationDelegate> {
#endif
  MyWiiRemote* m_my_wiimote;
}
- (id)initWithMyWiiRemote:(MyWiiRemote*)a_my_wiimote;
- (void)dealloc;
- (void)applicationWillTerminate:(NSNotification*)a_notification;
- (void)applicationDidFinishLaunching:(NSNotification*)notification;
@end
@implementation AppDelegate
- (id)initWithMyWiiRemote:(MyWiiRemote*)a_my_wiimote {
  m_my_wiimote = 0;
  if(self = [super init]) {
    m_my_wiimote = a_my_wiimote;
  }
  return self;
}
- (void)dealloc{
  [m_my_wiimote release];
  [super dealloc];
}
- (void)applicationWillTerminate:(NSNotification*)a_notification {
  [m_my_wiimote close];
}
- (void)applicationDidFinishLaunching:(NSNotification*)a_notification {
  printf("debug : applicationDidFinishLaunching\n");
}
@end

@interface PushButton : NSButton {
  MyWiiRemote* m_my_wiiremote;
}
- (id)initWithFrame:(NSRect)aRect myWiiRemote:(MyWiiRemote*)a_my_wiiremote;
- (void)dealloc;
- (void)action:(id)aSender;
@end
@implementation PushButton
- (id)initWithFrame:(NSRect)aRect myWiiRemote:(MyWiiRemote*)a_my_wiiremote {
  m_my_wiiremote = 0;
  if(self = [super initWithFrame:aRect]) {
    [self setTarget:self];
    [self setAction:@selector(action:)];
    m_my_wiiremote = a_my_wiiremote;
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)action:(id)aSender {
  printf("debug : PushButton\n");
  [[m_my_wiiremote discovery] start];
  printf("Please press 1 button and 2 button simultaneously\n");
}
@end

int main(int aArgc,const char* aArgv[]) {
  printf("debug : wii 000\n");

  // Initialize NS (NextStep) :
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSApplication* app = [NSApplication sharedApplication];

  MyWiiRemote* my_wiimote = [[MyWiiRemote alloc] initWithApp:app];

  AppDelegate* appDel = [[AppDelegate alloc] initWithMyWiiRemote:my_wiimote];
  [app setDelegate:appDel];

  NSString* ns = [[NSString alloc] initWithString:@"CoinNextStep_test_wii"];
  BOOL status = [NSBundle loadNibNamed:ns owner:app];
  [ns release];

  NSRect rect = {20,10,500,400}; /*x,y,w,h*/

  unsigned int mask = NSResizableWindowMask;
  NSWindow* window = [[NSWindow alloc] initWithContentRect:rect 
                                       styleMask:mask 
                                       backing:NSBackingStoreBuffered
                                       defer:NO];

  printf("debug : create push button...\n");
  rect.origin.x = 0;
  rect.origin.y = 0;
  PushButton* button = 
    [[PushButton alloc] initWithFrame:rect myWiiRemote:my_wiimote];
  //[button setAutoresizingMask:autoResizeMask];
  [window setContentView:button];
  [button release];
  //[[window contentView] addSubview:button];
  [window makeKeyAndOrderFront:app];

  printf("debug : run...\n");
  [app run];
  printf("debug : run : end\n");

  // in fact we never pass here.
  [window release];
  [app release];
  [pool release];

  [app setDelegate:nil];
  [appDel release];

  printf("debug : exiting...\n");

  return 0;
}
