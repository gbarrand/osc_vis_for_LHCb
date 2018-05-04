// glut :
#ifdef APPLE_GLUT
#include <glut.h>
#else
#include <GL/glut.h>
#endif

void DrawCube()
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

void SetCamera(void)
{
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1,1,-1,1,0,100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Reshape (int w, int h)
{
  glDisable (GL_LINE_SMOOTH);
  glMatrixMode (GL_MODELVIEW);
  glViewport (0, 0, w, h);
  glLoadIdentity();
  
  SetCamera();
}

void Display (void)
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  
  SetCamera();
  
  DrawCube();
  
  glutSwapBuffers();
}
int main(int argc, char **argv) {
  glutInit( &argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize( 600, 500);
  glutCreateWindow("OnX glut test");

  glShadeModel(GL_FLAT);
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);

  glutMainLoop ();

  return 0; //EXIT_SUCCESS;
}
