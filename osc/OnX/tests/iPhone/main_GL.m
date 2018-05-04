
#import <UIKit/UIKit.h>

extern int test_inet(const char*);

#define HAS_OPENGL

#ifdef HAS_OPENGL
#import "GLView.h"
#import "ConstantsAndMacros.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_OS_EMBEDDED
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#else
#import <OpenGL/OpenGL.h>
#endif

#define glBegin(a_mode) \
 {float coords[24];\
  unsigned int coordi = 0;\
  unsigned int mode = a_mode

#define glVertex3d(a_x,a_y,a_z) \
  coords[coordi] = a_x;coordi++;\
  coords[coordi] = a_y;coordi++;\
  coords[coordi] = a_z;coordi++

#define glEnd(a_dummy) \
  glEnableClientState(GL_VERTEX_ARRAY);\
  glVertexPointer(3,GL_FLOAT,0,coords);\
  glDrawArrays(mode,0,coordi/3);\
  glDisableClientState(GL_VERTEX_ARRAY);}if(true)

static void draw_cube() {
  glLineWidth(4);

  // Front :
  glColor4f(1,0,0,0);    //red
  glNormal3f(0,0,1);
  // GL_POLYGON does not exist !
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d( 0.5F,-0.5F,0.5F);
  glVertex3d( 0.5F, 0.5F,0.5F);
  glVertex3d(-0.5F, 0.5F,0.5F);
  glVertex3d(-0.5F,-0.5F,0.5F);
  glEnd();

  // Back :
  glColor4f(0,1,0,0);   //green
  glNormal3f(0,0,-1);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3d( 0.5F,-0.5F,-0.5F);
  glVertex3d(-0.5F,-0.5F,-0.5F);
  glVertex3d(-0.5F, 0.5F,-0.5F);
  glVertex3d( 0.5F, 0.5F,-0.5F);
  glEnd();

  glColor4f(1,1,0,0);   //yellow
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

static void opengl_cube_paint(UIView* a_view) {

  UIDeviceOrientation orient = [[UIDevice currentDevice] orientation];
  //NSLog(@"debug : device orientation %d\n",orientation);

  CGRect rect = [a_view bounds]; 

  unsigned int width = rect.size.width;
  unsigned int height = rect.size.height;  

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_SCISSOR_TEST);

  //FIXME glEnable(GL_LINE_STIPPLE);
  //FIXME glPolygonMode(GL_FRONT,GL_FILL);

  //  So that lightning is correctly computed 
  // when some scaling is applied.
  glEnable(GL_NORMALIZE);

  glShadeModel(GL_FLAT);
  //glEnable(GL_LIGHTING);
  //glDisable(GL_LIGHT0);

  glViewport(0,0,width,height);
  //glScissor(0,0,width,height);

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  float aspect = ((float)width)/((float)height);
  float half_height = 1;
  float l = -aspect * half_height;
  float r =  aspect * half_height;
  float b = -half_height; 
  float t =  half_height; 
  glOrthof(l,r,b,t,-100,100);

  if(orient==UIDeviceOrientationPortrait){
  } else if(orient==UIDeviceOrientationPortraitUpsideDown){
    glRotatef(180,0,0,1);
  } else if(orient==UIDeviceOrientationLandscapeLeft){ //home at right
    glRotatef(-90,0,0,1);
  } else if(orient==UIDeviceOrientationLandscapeRight){ //home at left
    glRotatef(90,0,0,1);
  } else {
    //UIDeviceOrientationFaceUp,  // Device oriented flat, face up
    //UIDeviceOrientationFaceDown // Device oriented flat, face down
  }

  // Better to clear after glViewport and glScissor (else problems with Mesa).
  glClearColor(0.8F,0.8F,0.8F,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glScaled(1,1,1);
  // Rotate then translate :
  glTranslatef(0,0,-10);
  glRotatef(30,0,1,0);
  glRotatef(30,1,0,0);

  draw_cube();

  glFinish();
}

@interface MyWindow_VC : UIViewController {
}
@end
@implementation MyWindow_VC
- (void)dealloc {[super dealloc];}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  //NSLog(@"debug : shouldAutorotateToInterfaceOrientation\n");
  return YES;
}

@end

@interface MyGL_VC : UIViewController <GLViewDelegate> {
}
@end
@implementation MyGL_VC
- (void)dealloc {[super dealloc];}
- (void)drawView:(UIView*)a_view {
  opengl_cube_paint(a_view);
}
-(void)setupView:(GLView*)view {
}
@end

#endif

@interface MyApp : UIApplication {
  UIProgressView* m_progress;
}
- (id)init;
- (void)dealloc;
- (void)create_ui;
@end

@implementation MyApp
- (id)init {
  m_progress = 0;
  if(self = [super init]) {
  }
  return self;
}
- (void)dealloc {[super dealloc];}
- (void)applicationDidFinishLaunching:(UIApplication *)application {
  [self create_ui];    
}
- (void)create_ui {
  //{UIDevice* device = [UIDevice currentDevice];
  // NSLog(@"debug : device model %@\n",[device model]);
  // NSLog(@"debug : device systemName %@\n",[device systemName]);
  // NSLog(@"debug : device systemVersion %@\n",[device systemVersion]);}

  //{CGRect rect = [[UIScreen mainScreen] bounds];
  //iPhone : 0 0 320 480
  //iPad : 0 0 768 1024
  //NSLog(@"debug : create_ui : bounds : %g %g %g %g\n",
  //  rect.origin.x,rect.origin.y,
  //  rect.size.width,rect.size.height);}

  CGRect rect = [[UIScreen mainScreen] applicationFrame];
  //iPhone : 0 20 320 460
  //iPad : 0 20 768 1004
  //NSLog(@"debug : create_ui : applicationFrame : %g %g %g %g\n",
  //  rect.origin.x,rect.origin.y,
  //  rect.size.width,rect.size.height);

  float aw = rect.size.width;
  float ah = rect.size.height;

  float xmiddle = rect.size.width*0.5F;

  UIWindow* window = [[UIWindow alloc] initWithFrame:rect];
 {MyWindow_VC* vc = [[MyWindow_VC alloc] init];
  [window addSubview:[vc view]];}
  [window setBackgroundColor:[UIColor whiteColor]];
 
  float yw = 10;

#ifdef HAS_OPENGL
 {float w = aw*0.75F;
  float h = ah*0.75F;
  CGRect rect = CGRectMake(xmiddle-w*0.5F,yw,w,h);
  yw += h;  
  GLView* widget = [[GLView alloc] initWithFrame:rect];  
 {MyGL_VC* vc = [[MyGL_VC alloc] init];
 [widget setDelegate:vc];}

  widget.animationInterval = 1.0 / kRenderingFrequency;
  [widget startAnimation];

  [window addSubview:widget];
 }
#endif

  {    
  UIButton* widget = [UIButton buttonWithType:UIButtonTypeRoundedRect];
  float w = 100;  
  float h = 50;  
  CGRect rect = CGRectMake(xmiddle-w*0.5F,yw+10,w,h);
  yw += h+10;  
  [widget setFrame:rect];  
  [widget setTitle:@"iOnX_GL_18" forState:UIControlStateNormal];  
  [widget addTarget:(id)self action:@selector(button_action:) forControlEvents:UIControlEventTouchDown];
  [window addSubview:widget];
  }
  
 {UIProgressView* widget = 
    [[UIProgressView alloc] 
     initWithProgressViewStyle:UIProgressViewStyleDefault];  
  float w = 100;  
  float h = 50;  
  CGRect rect = CGRectMake(xmiddle-w*0.5F,yw+10,w,50);
  yw += h+10;  
  [widget setFrame:rect];  
  [widget setProgress:0.75F];  
  m_progress = widget;
  [window addSubview:widget];
 }
   
  [window makeKeyAndVisible];
}
- (void)button_action:(id)aSender {
  //NSLog(@"debug : button_exit\n");
  //exit(0);
  //system("pwd");
  //system("ls");
  //system("curl");
  //system("uname -a");
  //system("printenv");
  //system("printenv 1> /Users/barrand/out.log");
  //system("echo ${HOME} 1&2> /Users/barrand/out.log");
  float value = [m_progress progress];
  value += 0.2F;
  if(value>1) value = 0;
  [m_progress setProgress:value];

  NSArray* paths = 
    NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);
  NSString* doc_dir = [paths objectAtIndex:0];
  NSFileManager* fm = [NSFileManager defaultManager];    
  [fm createDirectoryAtPath:doc_dir attributes:nil];
  test_inet([doc_dir UTF8String]);
  //[paths release];
}
@end

int main(int argc, char *argv[]) {
  //NSLog(@"debug : main : begin : 001\n");
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  int retVal = UIApplicationMain(argc, argv, @"MyApp", nil);
  //NOTE : it seems that we never pass here.
  [pool release];
  return retVal;
}
