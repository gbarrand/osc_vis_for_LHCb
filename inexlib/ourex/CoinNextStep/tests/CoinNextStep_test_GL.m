
#import <AppKit/AppKit.h>

#import <OpenGL/gl.h>

static void InitGL(int,int);
static void DrawBack();
static void DrawCube();

//#define EMULATE_FULL_SCREEN
//#define FULL_SCREEN

#ifdef FULL_SCREEN_AS_SDL
@interface MyView : NSView {
  NSOpenGLContext* m_gl_context;
}
- (id)initWithFrameAndCtx:(NSRect)rect ctx:(NSOpenGLContext*)ctx;
- (void)dealloc;
- (void)drawRect:(NSRect)rect;
@end
@implementation MyView
- (id)initWithFrameAndCtx:(NSRect)rect ctx:(NSOpenGLContext*)a_ctx {
  m_gl_context = 0;
  if (self = [super initWithFrame:rect]) {
    m_gl_context = a_ctx;
  }
  return self;
}
- (void)dealloc {
  [m_gl_context release];
  [super dealloc];
}
- (void)drawRect:(NSRect)rect {
  //printf("debug : draw 002\n");
  [m_gl_context setFullScreen];
  [m_gl_context makeCurrentContext];
  int w = rect.size.width;
  int h = rect.size.height;
  //printf("debug : drawRect : %d %d\n",w,h);
  InitGL(w,h);
  DrawBack();
  DrawCube();
  //glFinish();
  [m_gl_context flushBuffer];
}
- (void)reshape {
  //printf("debug : reshape\n");
  //[[self openGLContext] update];
}
- (void) keyDown:(NSEvent *)event {
  printf("debug : key down\n");
  //exit(0);
}
- (void)mouseDown:(NSEvent*)event {
  printf("debug : mouse down\n");
  exit(0);
  [[self nextResponder] rightMouseDown:event];
}
@end
#else
@interface GL_View : NSOpenGLView {
}
- (id)initWithFrame:(NSRect)rect display:(CGDirectDisplayID)a_display;
- (void)dealloc;
- (void)drawRect:(NSRect)rect;
- (void)keyDown:(NSEvent*)event;
//- (BOOL)enterFullScreenMode:(NSScreen*)screen withOptions:(NSDictionary*)options;
//- (void)exitFullScreenModeWithOptions:(NSDictionary*)options;
@end
@implementation GL_View
- (id) initWithFrame:(NSRect)rect display:(CGDirectDisplayID)a_display{
  int colorbits = 32;
  int depthbits = 32;
  NSOpenGLPixelFormatAttribute att[32];
  int i = 0;
  att[i++] = NSOpenGLPFADoubleBuffer;
  att[i++] = NSOpenGLPFAAccelerated;
  att[i++] = NSOpenGLPFAAccumSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)32;
  att[i++] = NSOpenGLPFAColorSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)colorbits;
  att[i++] = NSOpenGLPFADepthSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)depthbits;
  att[i++] = NSOpenGLPFAScreenMask;
  att[i++] = CGDisplayIDToOpenGLDisplayMask(a_display);
#ifdef FULL_SCREEN
  att[i++] = NSOpenGLPFAFullScreen;
#endif
  att[i] = (NSOpenGLPixelFormatAttribute)0;
  NSOpenGLPixelFormat *pixelFormat;
  pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:att];
  if (self = [super initWithFrame:rect pixelFormat:pixelFormat]) {
    // flush buffer only during the vertical retrace of the monitor
    const GLint vals[1] = {1};
    [[self openGLContext] setValues:vals forParameter:NSOpenGLCPSwapInterval];
#ifdef FULL_SCREEN
    [[self openGLContext] setFullScreen];
#endif
    [[self openGLContext] makeCurrentContext];
  }
  [pixelFormat release];
  return self;
}
- (void)dealloc {[super dealloc];}

- (void)drawRect:(NSRect)rect {
  //printf("debug : draw 000\n");
#ifdef FULL_SCREEN
  [[self openGLContext] setFullScreen];
#endif
  [[self openGLContext] makeCurrentContext];
  int w = rect.size.width;
  int h = rect.size.height;
  printf("debug : drawRect : %d %d\n",w,h);
  InitGL(w,h);
  DrawBack();
  DrawCube();
  //glFinish();
  [[self openGLContext] flushBuffer];
}
- (void)reshape {
  //printf("debug : reshape\n");
  [[self openGLContext] update];
}
- (void) keyDown:(NSEvent *)event {
  printf("debug : key down\n");
  //exit(0);
}
- (void)mouseDown:(NSEvent*)event {
  printf("debug : mouse down\n");
  exit(0);
  [[self nextResponder] rightMouseDown:event];
}
@end
#endif

/*
@interface AppDelegate : NSObject<NSApplicationDelegate> {
}
- (void)applicationDidFinishLaunching:(NSNotification*)notification;
@end
@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  printf("debug : finish lauch 000\n");
  NSApplication* app = (NSApplication*)[notification object];
 {NSApplicationPresentationOptions opts = 0;
   //opts |= NSApplicationPresentationHideDock;
   //opts |= NSApplicationPresentationHideMenuBar;
  opts += NSApplicationPresentationAutoHideDock;
  opts += NSApplicationPresentationAutoHideMenuBar;
  [app setPresentationOptions:opts];}
}
@end
*/

int main(int aArgc,const char* aArgv[]) {
  printf("debug : xxxxxxx 006\n");

  int monitor = 1;
#ifdef FULL_SCREEN
  CGDirectDisplayID display_id = kCGDirectMainDisplay;
  CGDirectDisplayID activeDspys[3];
  CGDisplayCount dspyCnt;
  CGGetActiveDisplayList(3,activeDspys,&dspyCnt);
  printf("debug : displays : %d\n",dspyCnt);
  if((monitor>=0)&&(monitor<dspyCnt)) {
    display_id = activeDspys[monitor];
    printf("debug : 000 display : %d\n",display_id);
  }
  //CGDisplayCapture(display_id);
#else
  CGDirectDisplayID display_id = kCGDirectMainDisplay;
#endif

  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  NSApplication* app = [NSApplication sharedApplication];

  //AppDelegate* appDel = [[AppDelegate alloc] init];
  //[app setDelegate:appDel];

  NSString* ns = [[NSString alloc] initWithString:@"CoinNextStep_test_GL"];
  BOOL status = [NSBundle loadNibNamed:ns owner:app];
  [ns release];

  NSScreen* screen = 0;
 {NSArray* scrs = [NSScreen screens];
  int number = [scrs count];
  //printf("debug : screens %d\n",number);
  if((monitor>=0)&&(monitor<number)) {
    screen = (NSScreen*)[scrs objectAtIndex:monitor];
    //printf("debug : screen n %lu\n",(NSScreen*)[scrs objectAtIndex:0]);
  }}

 {NSRect rect = [screen frame];
  printf("debug : screen : %g %g %g %g\n",
    rect.origin.x,
    rect.origin.y,
    rect.size.width,
    rect.size.height);}

  // Create window :
  // origin = bottom, left.
#ifdef FULL_SCREEN 
  NSRect rect = [screen frame];
  NSRect gl_rect = rect;
#else
#ifdef EMULATE_FULL_SCREEN
  NSRect rect = [screen frame];
  rect.origin.x = 0;
  rect.origin.y = 0;
  NSRect gl_rect = rect;
#else
  NSRect rect = {20,10,500,400}; /*x,y,w,h*/
  NSRect gl_rect = rect;
#endif
#endif

#ifdef FULL_SCREEN 
  unsigned int mask = 0;
#else
#ifdef EMULATE_FULL_SCREEN
  unsigned int mask = NSBorderlessWindowMask;
#else
  unsigned int mask = NSResizableWindowMask | NSTitledWindowMask;
#endif
#endif
  NSWindow* window = [[NSWindow alloc] initWithContentRect:rect
                                       styleMask:mask
                                       backing:NSBackingStoreBuffered
                                       defer:NO
#ifdef FULL_SCREEN
				       ];
#else
                                       screen:screen];
#endif

#ifdef FULL_SCREEN 
#else
#ifdef EMULATE_FULL_SCREEN
  [window setLevel: NSStatusWindowLevel];
#endif
  NSString* title = [NSString stringWithUTF8String:"CoinNextStep_test_GL"];
  [window setTitle:title];
  [title release];
  [window setShowsResizeIndicator:YES];
#endif

#ifdef FULL_SCREEN_AS_SDL
  int colorbits = 32;
  int depthbits = 32;
  NSOpenGLPixelFormatAttribute att[32];
  int i = 0;
  att[i++] = NSOpenGLPFADoubleBuffer;
  att[i++] = NSOpenGLPFAAccelerated;
  att[i++] = NSOpenGLPFAAccumSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)32;
  att[i++] = NSOpenGLPFAColorSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)colorbits;
  att[i++] = NSOpenGLPFADepthSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)depthbits;

  att[i++] = NSOpenGLPFAScreenMask;
  att[i++] = CGDisplayIDToOpenGLDisplayMask(display_id);
  att[i++] = NSOpenGLPFAFullScreen;

  att[i] = (NSOpenGLPixelFormatAttribute)0;
  NSOpenGLPixelFormat* pixelFormat = 
    [[NSOpenGLPixelFormat alloc] initWithAttributes:att];

  NSOpenGLContext* gl_context = 
    [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
  [pixelFormat release];

  NSView* view = [[MyView alloc] initWithFrameAndCtx:gl_rect ctx:gl_context];
  [window setContentView:view];
  [view release ];
#else
  GL_View* view = [[GL_View alloc] initWithFrame:gl_rect display:display_id];
  [window setContentView:view];
  [view release];
#endif

  //[window setAcceptsMouseMovedEvents:YES];
  [window makeKeyAndOrderFront:app];
  [app run];

  // if doing [app run], we never pass here !
  [window release];
  [app release];
  [pool release];

//[app setDelegate:nil];
//[appDel release];

  //printf("debug : exiting...\n");

  return 0;
}

/* Some GL */
static void InitGL(int width,int height) {
  //glClearColor(1.0,1.0,1.0,0.);
  glClearColor(1.0,1.0,0.6,0.);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_LIGHTING);

  //glDisable(GL_SCISSOR_TEST);
  //glShadeModel(GL_FLAT);
  glViewport(0,0,width,height);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  double aspect = ((double)viewport[2])/((double)viewport[3]);
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  double h = 1;
  //glFrustum(-aspect,aspect,-1.,1.,1.,4.);
  glFrustum(-aspect*h/2,aspect*h/2,-h/2,h/2,1.,4.);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  gluLookAt(3.,0.,0.,  /*eye*/
            0.,0.,0.,  /*center*/
            0.,-1.,0.); /*up*/

}
static void DrawBack () {
  glColor3d(0.7,0.7,0.7);   
  glBegin(GL_POLYGON);
  glVertex3d( 1.0,-1.0, 1.0);
  glVertex3d( 1.0,-1.0,-1.0);
  glVertex3d(-1.0,-1.0,-1.0);
  glVertex3d(-1.0,-1.0, 1.0);
  glEnd();
}
static GLubyte plus_5_5[] = {
  0x20,
  0x20,
  0xf8,
  0x20,
  0x20,
};
static void DrawCube () {
  glLineWidth(4.0);

  glColor3d(1.0,0.0,0.);   
  glBegin(GL_POLYGON);
  glVertex3d( 0.5,-0.5,0.5);
  glVertex3d( 0.5, 0.5,0.5);
  glVertex3d(-0.5, 0.5,0.5);
  glVertex3d(-0.5,-0.5,0.5);
  glEnd();

  glColor3d(0.0,0.0,1.0);   
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glRasterPos3f(0.5,-0.5,0.5);
  glBitmap(5,5,2.,2.,0.,0.,plus_5_5);
  glRasterPos3f( 0.5, 0.5,0.5);
  glBitmap(5,5,2.,2.,0.,0.,plus_5_5);
  glRasterPos3f(-0.5, 0.5,0.5);
  glBitmap(5,5,2.,2.,0.,0.,plus_5_5);
  glRasterPos3f(-0.5,-0.5,0.5);
  glBitmap(5,5,2.,2.,0.,0.,plus_5_5);

  glColor3d(0.0,1.0,0.);   
  glBegin(GL_POLYGON);
  glVertex3d( 0.5,-0.5,-0.5);
  glVertex3d( 0.5, 0.5,-0.5);
  glVertex3d(-0.5, 0.5,-0.5);
  glVertex3d(-0.5,-0.5,-0.5);
  glEnd();

  glColor3d (1.0,1.0,0.);   
  glBegin(GL_LINES);
  glVertex3d( 0.5,-0.5, 0.5);
  glVertex3d( 0.5,-0.5,-0.5);
  glVertex3d( 0.5, 0.5, 0.5);
  glVertex3d( 0.5, 0.5,-0.5);
  glVertex3d(-0.5, 0.5, 0.5);
  glVertex3d(-0.5, 0.5,-0.5);
  glVertex3d(-0.5,-0.5, 0.5);
  glVertex3d(-0.5,-0.5,-0.5);
  glEnd();
}
