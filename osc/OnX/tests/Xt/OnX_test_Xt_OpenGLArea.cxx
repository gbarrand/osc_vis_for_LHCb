#include <stdio.h>
#include <stdlib.h>

#include <X11/StringDefs.h>

//#include <Xm/DrawingA.h>
#include <Xm/Form.h>

#include <Xm/PushB.h>

#include <OnX/Xt/OpenGLArea.h>

#include <GL/gl.h>

static void ButtonCallback(Widget,XtPointer a_data,XtPointer){
  Widget top = (Widget)a_data;
  XtUnmapWidget(top);
  XtMapWidget(top);  
}

static void paint_cbk(Widget,XtPointer,XtPointer);
static void DrawCube();

#if defined(__APPLE__) && (XmVERSION>=2) && (XmREVISION>=3)
#include <Xm/Display.h>
#include <Xm/DialogS.h>
#include <Xm/DragOverS.h>
#include <Xm/GrabShell.h>
#include <Xm/Print.h>
#include <X11/IntrinsicP.h>
#include <iostream>
static void ResetVendorField(WidgetClass aWidgetClass){
  WidgetClass wc = aWidgetClass;
  while(wc) {
    WidgetClass super = wc->core_class.superclass;
    if(super) {
      char* name = (char*)super->core_class.class_name;
      if( !strcmp(name,"VendorShell") && (super!=vendorShellWidgetClass) ) {
        // The Vendor super class is not the Motif one, override :
        wc->core_class.superclass = vendorShellWidgetClass;
        return;
      }
    }
    wc = super;
  }
}
static void LookDSM_Problem(){
  if(!vendorShellWidgetClass->core_class.class_part_initialize) {
    // The vendorShellWidgetClass we get is the Xt one.
    std::cout << "OnX::XtUI::LookDSM_Problem :" << std::endl;
    std::cout << " Can't solve the dsm problem." << std::endl;
    std::cout << " Bad linking order of -lXm and -lXt " << std::endl;
    std::cout << " or you try to load a DLL linked to X11 prior to" << std::endl;
    std::cout << " load a DLL linked with Motif." << std::endl;
    return;
  }
  ResetVendorField(xmDisplayClass);
  ResetVendorField(xmDialogShellWidgetClass);
  ResetVendorField(xmDragOverShellWidgetClass);
  ResetVendorField(xmGrabShellWidgetClass);
  ResetVendorField(xmPrintShellWidgetClass);
}
#else
static void LookDSM_Problem(){}
#endif

int main(int a_argn,char* a_args[]){
  LookDSM_Problem();

  //printf("debug : main : 0\n");

#if XtSpecificationRelease == 4
  Cardinal argn = (Cardinal)a_argn;
#else
  int argn = a_argn;
#endif
  XtAppContext appContext = NULL;
  Arg args[7];
  XtSetArg(args[0],XtNgeometry,XtNewString("200x200"));
  XtSetArg(args[1],XtNborderWidth,0);
  Widget top = XtAppInitialize(&appContext,"test",
                               NULL,(Cardinal)0,
                               &argn,a_args,NULL,
                               args,2);

  Widget dform = XmCreateForm(top,(char*)"dform",NULL,0);
  XtManageChild(dform);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_NONE);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_FORM);
  Widget button = XmCreatePushButton(dform,(char*)"hide_shoe",args,4);
  XtManageChild(button);
  XtAddCallback(button,XmNactivateCallback,ButtonCallback,(XtPointer)top);

  XtSetArg(args[0],XmNtopAttachment   ,XmATTACH_FORM);
  XtSetArg(args[1],XmNleftAttachment  ,XmATTACH_FORM);
  XtSetArg(args[2],XmNrightAttachment ,XmATTACH_FORM);
  XtSetArg(args[3],XmNbottomAttachment ,XmATTACH_WIDGET);
  XtSetArg(args[4],XmNbottomWidget,button);
  XtSetArg(args[5],XmNwidth,200);
  XtSetArg(args[6],XmNheight,200);
  //XtCreateManagedWidget("da",xmDrawingAreaWidgetClass,dform,args,7);
 {Widget glArea = XtCreateManagedWidget
    ("gl",openGLAreaWidgetClass,dform,args,7);
  XtAddCallback(glArea,XoNpaintCallback,paint_cbk,(XtPointer)0);}


  XtRealizeWidget(top);

  XtAppMainLoop(appContext);

  return EXIT_SUCCESS;
}

static void paint_cbk(Widget a_widget,XtPointer,XtPointer){

  unsigned int width,height;
  //printf("debug : paint_cbk\n");
  width = a_widget->core.width;
  height = a_widget->core.height;

  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_SCISSOR_TEST);

  glEnable(GL_LINE_STIPPLE);
  glPolygonMode(GL_FRONT,GL_FILL);
  //  So that lightning is correctly computed 
  // when some scaling is applied.
  glEnable(GL_NORMALIZE);

  glShadeModel(GL_FLAT);
  //glEnable(GL_LIGHTING);
  //glDisable(GL_LIGHT0);

  glViewport(0,0,width,height);
  //glScissor(0,0,width,height);

  // In OpenGL, by default,
  // camera is placed at origin, looking at -z, up along y.
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  double aspect = ((double)viewport[2])/((double)viewport[3]);
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();

  double half_height = 1;
  glOrtho(-aspect * half_height,aspect*half_height,
          -half_height,half_height,-100,100);

  // Better to clear after glViewport and glScissor (else problems with Mesa).
  glClearColor(0.8F,0.8F,0.8F,0);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glScaled(1,1,1);
  // Rotate then translate :
  glTranslated(0,0,-10);
  glRotated(30,0,1,0);
  glRotated(30,1,0,0);

  DrawCube();

  glFinish();
}
//////////////////////////////////////////////////////////////////////////////
static void DrawCube(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  glLineWidth(4);

  // Front :
  glColor3d(1,0,0);    //red
  glBegin(GL_POLYGON);
  glNormal3d(0,0,1);
  glVertex3d( 0.5,-0.5,0.5);
  glVertex3d( 0.5, 0.5,0.5);
  glVertex3d(-0.5, 0.5,0.5);
  glVertex3d(-0.5,-0.5,0.5);
  glEnd();

  // Back :
  glColor3d(0,1,0);   //green
  glBegin(GL_POLYGON);
  glNormal3d(0,0,-1);
  glVertex3d( 0.5,-0.5,-0.5);
  glVertex3d(-0.5,-0.5,-0.5);
  glVertex3d(-0.5, 0.5,-0.5);
  glVertex3d( 0.5, 0.5,-0.5);
  glEnd();

  glColor3d(1,1,0);   //yellow
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

#include <OpenGLArea.c>
#include <gl2ps.c>

