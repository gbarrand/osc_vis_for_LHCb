
#include <QtGui/qapplication.h> //G.Barrand QtGui
#include <QtGui/qpushbutton.h> //G.Barrand QtGui
#include <QtGui/qboxlayout.h> //G.Barrand QtGui
#include <QtOpenGL/qgl.h> //G.Barrand QtOpenGL

#include "MyButton.h"

void MyButton::exitLoop(){qApp->exit(0);}

void MyOpenGLArea::initialieGL() {
  printf("debug : initializeGL\n");
}
void MyOpenGLArea::resizeGL(int,int){
  printf("debug : resize\n");
}
void MyOpenGLArea::paintGL() {
  printf("debug : paintGL\n");
}
void MyOpenGLArea::mousePressEvent(QMouseEvent* aEvent) {
}
void MyOpenGLArea::mouseReleaseEvent(QMouseEvent* aEvent) {
}
void MyOpenGLArea::mouseMoveEvent(QMouseEvent* aEvent) {
}


int main(int argc,char** argv) {

  QApplication* qapp = new QApplication(argc,argv);

  QWidget* top = new QWidget();

  QWidget* box = new QWidget();
  //QGLWidget* gl = new QGLWidget(box);
  MyOpenGLArea* gl = new MyOpenGLArea(box);

  QVBoxLayout* layout = new QVBoxLayout;
  MyButton* button = new MyButton();
  QObject::connect(button,SIGNAL(clicked()),button,SLOT(exitLoop()));
  layout->addWidget(box);
  layout->addWidget(button);
  top->setLayout(layout);

  top->show();

  qapp->exec();

  delete gl;
  delete top;

  delete qapp;

  printf("debug : exit...\n");

  return 0;
}

