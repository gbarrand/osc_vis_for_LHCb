//////////////////////////////////////////////////////////////////////////////
/// OpenGL cube example //////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <exlib/OpenGL>

static void draw_cube() {
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

#ifdef OnXExas_include
//FIXME : arrange so that the below be local.

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ICallbackData.h>

extern "C" {

void opengl_cube_paint(Slash::UI::IUI& aUI) {
  Slash::UI::IWidget* widget = aUI.callbackData().widget();
  if(!widget) return;
  unsigned int width,height;
  if(!widget->size(width,height)) return;
  //printf("debug : %s : %d %d\n",widget->name().c_str(),width,height);

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

  draw_cube();

  glFinish();
}

void opengl_event(Slash::UI::IUI& aUI){
  aUI.session().cout() << "opengl_event :"
                       << " " << aUI.callbackData().event()
                       << " x = " << aUI.callbackData().x()
                       << " y = " << aUI.callbackData().y()
                       << std::endl;
}

}

#endif //OnXExas_include

#include <Slash/Tools/UI.h>

// this method is reused in examples/Callbacks/Included.cxx.

static void opengl_cube(Slash::Core::ISession& a_session,std::ostream& a_out,const std::string& a_viewer,bool a_ui) {

  Slash::UI::IUI* ui = Slash::find_UI(a_session);     
  if(!ui) {
    a_out << "Slash::UI::IUI not found." << std::endl;
    return;
  }

  if(!ui->setCallback
        ("opengl_area","paint","DLD","OnXExas opengl_cube_paint")) {
    ui->session().cout() << "opengl_cube :" 
                         << " Can't set paint callback "
                         << " on widget opengl_area."
                         << std::endl;
  }
  if(!ui->setCallback
        ("opengl_area","event","DLD","OnXExas opengl_event")) {
    ui->session().cout() << "opengl_cube :" 
                         << " Can't set event callback "
                         << " on widget opengl_area."
                         << std::endl;
  }

  if(a_ui) ui->steer();
}


#include <Slash/Core/ISession.h>

#include <iostream>

int main(int,char*[]) {

  int argc = 1;
  static char* argv[] = {(char*)"-onx_file=opengl_area.onx"};  

  Slash::Core::ISession* session = Slash_create_session(inlib::to(argc,argv));
  if(!session) {
    std::cout << "opengl_cube :"
              << " session not found." << std::endl;
    return 1;
  }

  opengl_cube(*session,std::cout,"@current@",true);

  delete session;

  return 0;
}
