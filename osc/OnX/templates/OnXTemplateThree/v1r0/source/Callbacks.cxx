//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/UI/IUI.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ICallbackData.h>

#include <exlib/OpenGL>

extern "C" {

static void OpenGL_DrawCube();

void OnXTemplateThree_paint_cube(Slash::UI::IUI& aUI){
  // Should be put on the paint callback of a OpenGLArea widget.
  // See OnXTemplateThree/<vers>/scripts/OnX/OnXTemplateThree.onx file.

  Slash::UI::IWidget* widget = aUI.callbackData().widget();
  if(!widget) return;
  unsigned int width,height;
  if(!widget->size(width,height)) return;
  //printf("debug : %s : %d %d\n",component.c_str(),width,height);

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

  OpenGL_DrawCube();

  glFinish();
}


} // extern "C"

static void OpenGL_DrawCube(){
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
