

#include <gtk/gtk.h>

#include <gtkgl/gtkglarea.h>

#include <stdio.h>

static void expose_callback(GtkWidget*,GdkEventExpose*,void*);
static void OpenGL_DrawCube();

static void destroy_callback(GtkObject*,gpointer aTag) {
  printf("debug : destroy glarea\n");
}
static void exit_callback(GtkObject*,gpointer aTag) {
  gtk_main_quit();
}

int main(int aArgc,char** aArgv) {

  gtk_init(&aArgc, &aArgv);
  
  GtkWidget* topWidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(topWidget),600,600);

  GtkWidget* vbox = gtk_vbox_new(FALSE,0);

  GtkWidget* button = gtk_button_new_with_label("xxx");
  gtk_signal_connect(GTK_OBJECT(button),"clicked",
                     GTK_SIGNAL_FUNC(exit_callback), 
                     (gpointer)0);

  int glAttributes[16];
  int i = 0;
  glAttributes[i] = GDK_GL_RGBA; i++;
  glAttributes[i] = GDK_GL_DOUBLEBUFFER; i++;
  glAttributes[i] = GDK_GL_DEPTH_SIZE; i++;
  glAttributes[i] = 1; i++;
  /*
  glAttributes[i] = GDK_GL_STENCIL_SIZE; i++;
  glAttributes[i] = 1; i++;
  */
  glAttributes[i] = GDK_GL_NONE; i++;
  GtkWidget* glarea = gtk_gl_area_new(glAttributes);    
  if(!glarea) {
    ::printf("can't create a gtk_gl_area widget.\n");
    return 1;
  }
  //NOTE : gtk2 : the below avoid a crash in the last gtk_widget_destroy.
  GdkGLContext* glcontext = GTK_GL_AREA(glarea)->glcontext;
  gdk_gl_context_ref(glcontext);

  gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
                     GTK_SIGNAL_FUNC(expose_callback),(gpointer)0);
  gtk_signal_connect(GTK_OBJECT(glarea), 
                     "destroy",
                     GTK_SIGNAL_FUNC(destroy_callback),
                     0);

  //GtkWidget* glarea = gtk_drawing_area_new();    

  gtk_container_add(GTK_CONTAINER(vbox),button);
  gtk_container_add(GTK_CONTAINER(vbox),glarea);
  gtk_container_add(GTK_CONTAINER(topWidget),vbox);

  gtk_widget_show_all(topWidget);

  gtk_main ();

  gtk_widget_destroy(topWidget);

  //FIXME : gdk_gl_context_unref(glcontext);

  ::printf("main : return...\n");
  return 0;
}

#ifdef WIN32
#include <windows.h>
#endif

#ifdef APPLE_GL
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////
void expose_callback(
 GtkWidget* aWidget
,GdkEventExpose*
,void*
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int width = aWidget->allocation.width;
  unsigned int height = aWidget->allocation.height;

  //printf("debug : %d %d\n",width,height);

  gtk_gl_area_make_current(GTK_GL_AREA(aWidget));

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
  // height 2.
  glOrtho(-aspect,aspect,-1,1,-100,100);

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

  gtk_gl_area_swapbuffers(GTK_GL_AREA(aWidget));
}
//////////////////////////////////////////////////////////////////////////////
static void OpenGL_DrawCube(
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
