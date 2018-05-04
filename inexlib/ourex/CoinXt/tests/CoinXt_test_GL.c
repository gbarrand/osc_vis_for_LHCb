/*
 *  This is a program to demonstrate a problem
 * on a MacOSX X11 and OpenGL context when drawing an elastic
 * rubber band. The XOR mode is ok on a pure X11 window
 * but not on an OpenGL windows. 
 *  This program works fine when displaying on other X11 server
 * like XFree86 on a Linux or an Exceed3D on a Windows.
 *  
 *  Two windows should appear, a white one which is a pure X11 one
 * and on which someone can drag and draw an elastic rubber band and
 * an OpenGL one (drawing some cubes) on which the same dragging
 * does not work for me on my Mac (but work well when displaying 
 * on other X11 server).
 * 
 *  Things does not work on a Panther and a Tiger for me.
 * 
 * Someone can reconstruct and run this program with :
 *    Darwin> gcc -I/usr/X11R6/include CoinXt_GL.c -L/usr/X11R6/lib -lGL -lX11
 *    Darwin> ./a.out
 * 
 *    Guy Barrand
 *
 *  26 Oct 2005
 * 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#ifdef APPLE_GL
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <GL/glx.h>

#ifdef __cplusplus
extern "C"{
#endif
static void Initialize();
static Window CreateSimpleWindow(char*,int,int,unsigned int,unsigned int);
static Window CreateGL_Window(char*,int,int,unsigned int,unsigned int);
static Bool WaitForNotify(Display*,XEvent*,char*);
static void DispatchRubber(Display*,XEvent*);
static void DrawBack();
static void DrawCube();
#ifdef __cplusplus
}
#endif

/*
void check_glGetError() {
  GLenum glerr;
  glerr = glGetError();
  printf("check glGetError...\n");
  while (glerr != GL_NO_ERROR) {
    printf("Error when setting up the GL context.\n");
    glerr = glGetError();
    if (glerr == GL_INVALID_OPERATION) break;
  }
}
*/

static struct {
  Display* display;
  Colormap colormap;
  XVisualInfo* vinfo;
  GLXContext ctx;
  int privateColormap;
} Global = {NULL,0L,NULL,NULL,1};

/* The main */
int main(int a_argn,char* a_args[]){
  Window winGL;
  int width,height;
  GLint  viewport[4];
  double aspect;

  if(a_argn==2) Global.privateColormap = 0;

  Initialize();

  /* Create non-GL window */
  CreateSimpleWindow("win 0",500,500,400,200);

  /* Create GL window */
  winGL = CreateGL_Window("win 3",0    ,450,200,400);

  if(glXMakeCurrent(Global.display,winGL,Global.ctx)==False) {
    printf("glXMakeCurrent failed.\n");
  }

 {const char* str = glGetString(GL_VERSION);
  if(!str) printf("glGetString(GL_VERSION) returns 0\n");
  else printf("GL_VERSION \"%s\"\n",str);}
 {const char* str = glGetString(GL_VENDOR);
  if(!str) printf("glGetString(GL_VENDOR) returns 0\n");
  else printf("GL_VENDOR \"%s\"\n",str);}

/*
  check_glGetError();
  glDrawBuffer(GL_BACK);
  check_glGetError();
*/

  /* Get window size */
 {XWindowAttributes watbs;
  XGetWindowAttributes  (Global.display,winGL,&watbs);
  width = watbs.width;
  height = watbs.height;}

  /* Do some GL */
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glShadeModel(GL_FLAT);
  glViewport(0,0,width,height);
  glGetIntegerv(GL_VIEWPORT,viewport);
  aspect = ((double)viewport[2])/((double)viewport[3]);
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
  glFrustum(-aspect,aspect,-1.,1.,1.,4.);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  gluLookAt(3.,0.,0.,  /*eye*/
            0.,0.,0.,  /*center*/
            0.,-1.,0.); /*up*/

  glClearColor(0.3,0.3,0.3,0.);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  DrawBack();
  DrawCube();
  glFinish();
  glXSwapBuffers(Global.display,winGL);

  glXWaitX();

  /* X11 Steering */
  while(1) { 
    XEvent xevent;
    if(XPending(Global.display)!=0) {
      XNextEvent  (Global.display,&xevent);
      DispatchRubber(Global.display,&xevent);
    }
  }

  glXMakeCurrent(Global.display, None,NULL);

}
/* X11 part */
static void Initialize () {
  static int attributeList[] = { 
    GLX_RGBA,
    GLX_RED_SIZE, 2,
    GLX_GREEN_SIZE, 2,
    GLX_BLUE_SIZE, 2,
    GLX_DOUBLEBUFFER,
    GLX_DEPTH_SIZE, 1,
    None };

  Global.display  = XOpenDisplay(NULL);                                                         
  if(Global.display==NULL) {
    printf("Can't open display.\n");
    exit(EXIT_SUCCESS);
  }

  Global.vinfo = glXChooseVisual
    (Global.display,DefaultScreen(Global.display),attributeList);
  if(Global.vinfo==NULL) {
    printf("Can't choose a visual.\n");
    exit(EXIT_SUCCESS);
  }

  Global.ctx = glXCreateContext(Global.display,Global.vinfo,NULL,GL_FALSE);
  if(Global.ctx==NULL) {
    printf("Can't create a GLX context.\n");
    exit(EXIT_SUCCESS);
  }

  if(Global.privateColormap==1) {
    /* It is better to create a colormap adapted to the visual.*/
    Global.colormap = 
      XCreateColormap(Global.display,XDefaultRootWindow(Global.display),
      Global.vinfo->visual, AllocNone); 
  } else {
    /* Default colormap does not work on an SGI with SGI libGL.*/
    Global.colormap = 
      XDefaultColormap(Global.display,DefaultScreen(Global.display));
  }

  if(Global.colormap==0L) {
    printf("Can't create X colormap.\n");
    exit(EXIT_SUCCESS);
  }
}
/* Create a non GL window */
static Window CreateSimpleWindow(char* title,int a_x,int a_y
,unsigned int a_width,unsigned int a_height) {
  Window This;
  XSetWindowAttributes swa;

  if(Global.display==NULL) return 0L;
  if(Global.vinfo==NULL) return 0L;

  swa.border_pixel = 0L;
  swa.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask
    | ButtonReleaseMask | Button1MotionMask; /* for rubber */
  
  This = XCreateWindow (Global.display, 
    XDefaultRootWindow(Global.display),
    a_x,a_y,a_width,a_height,
    0,
    (int)CopyFromParent,
    InputOutput,
    (Visual*)CopyFromParent,
    CWBorderPixel|CWEventMask,&swa);
  if(This==0L) {
    printf ("Can't create an X window.\n");
    exit  (EXIT_SUCCESS);
  }

 {XSizeHints sh;
  XTextProperty tp;
  XStringListToTextProperty (&title, 1, &tp);
  sh.flags = USPosition | USSize;
  XSetWMProperties (Global.display, This, &tp, &tp, 0, 0, &sh, 0, 0);
  XFree(tp.value);}

  XMapWindow(Global.display, This);
  XRaiseWindow(Global.display, This);
 {XEvent event;
  XIfEvent(Global.display, &event, WaitForNotify, (char*)This);}

  return This;
}
/* Create A GL window */
static Window CreateGL_Window(char* title,int a_x,int a_y
,unsigned int a_width,unsigned int a_height){
  Window This;
  XSetWindowAttributes swa;

  if(Global.display==NULL) return 0L;
  if(Global.vinfo==NULL) return 0L;

  swa.colormap = Global.colormap;
  swa.border_pixel = 0L;
  swa.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask
    | ButtonReleaseMask | Button1MotionMask; /* for rubber */
  
  This = XCreateWindow (Global.display, 
    XDefaultRootWindow(Global.display),
    a_x,a_y,a_width,a_height,
    0,
    Global.vinfo->depth,
    InputOutput,
    Global.vinfo->visual,
    CWBorderPixel|CWColormap|CWEventMask,&swa);
  if(This==0L) {
    printf ("Can't create an X window.\n");
    exit  (EXIT_SUCCESS);
  }

 {XSizeHints sh;
  XTextProperty tp;
  XStringListToTextProperty (&title, 1, &tp);
  sh.flags = USPosition | USSize;
  XSetWMProperties (Global.display, This, &tp, &tp, 0, 0, &sh, 0, 0);
  XFree(tp.value);}

  XMapWindow(Global.display, This);
  XRaiseWindow(Global.display, This);
 {XEvent event;
  XIfEvent (Global.display, &event, WaitForNotify, (char*)This);}

  return This;
}
static Bool WaitForNotify(Display* d,XEvent* e,char* arg){
  d = NULL;
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}
static GC sRubberGC = 0;
static Window sRubberWindow = 0;
static int sRubberMove = 0;
static XPoint sBeginMove;
static XPoint sEndMove;
void DrawRectangle( Display* aDisplay,Drawable aDrawable
,GC aGC,XPoint* aBegin ,XPoint* aEnd) {
  XPoint points[5];
  points[0] = *aBegin;
  points[1].x = aEnd->x;
  points[1].y = aBegin->y;
  points[2] = *aEnd;
  points[3].x = aBegin->x;
  points[3].y = aEnd->y;
  points[4] = *aBegin;
  XDrawLines(aDisplay,aDrawable,aGC,points,5,CoordModeOrigin);
}
static void DispatchRubber(Display* aDisplay,XEvent* aEvent ) {
  if(aEvent->type==ButtonPress) {
    XButtonEvent* event = (XButtonEvent*)aEvent;
    if(event->button==Button1) {
      XGCValues arggc;
      arggc.function = GXxor;
      arggc.background = 
        XWhitePixelOfScreen(DefaultScreenOfDisplay(aDisplay));
      arggc.foreground = 0xffffffff;
      arggc.plane_mask = 
        XWhitePixelOfScreen(DefaultScreenOfDisplay(aDisplay)) ^ 
        XBlackPixelOfScreen(DefaultScreenOfDisplay(aDisplay));
      arggc.subwindow_mode = IncludeInferiors;
      sRubberGC = XCreateGC(aDisplay,event->window,
                       GCFunction|GCForeground|GCBackground|
                       GCPlaneMask|GCSubwindowMode,&arggc);

      sRubberMove = 1;

      printf("debug : button press\n");
      sEndMove.x  = sBeginMove.x  = event->x;
      sEndMove.y  = sBeginMove.y  = event->y;

      DrawRectangle(aDisplay,event->window,sRubberGC,&sBeginMove,&sEndMove);
    }

  } else if(aEvent->type==ButtonRelease) {
    XButtonEvent* event = (XButtonEvent*)aEvent;
    if(event->button==Button1) {
      if(sRubberMove) {
        sRubberMove = 0;
        DrawRectangle(aDisplay,event->window,sRubberGC,&sBeginMove,&sEndMove);
        printf("debug : button release\n");
        XFreeGC(aDisplay,sRubberGC);
        sRubberGC = 0;
      }
    }
  } else if(aEvent->type == MotionNotify) {

    XMotionEvent* event = (XMotionEvent*)aEvent;
    if((event->state & Button1Mask) == Button1Mask) {
      if(sRubberMove) {
        
        printf("debug : button move\n");
        // Erase previous box by redrawing it !!! 
        // (in mode COMP this is equivallent to a deletion )
        DrawRectangle(aDisplay,event->window,sRubberGC,&sBeginMove,&sEndMove);
        
        sEndMove.x = event->x;
        sEndMove.y = event->y;
        DrawRectangle(aDisplay,event->window,sRubberGC,&sBeginMove,&sEndMove);
      }
    }
  }
}
/* Some GL */
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
