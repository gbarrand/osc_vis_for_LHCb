
#include <QtGui/qpushbutton.h> //G.Barrand QtGui

#include <cstdio>

class MyButton : public QPushButton {
  Q_OBJECT
public:
  MyButton(QWidget* aParent = 0):QPushButton(aParent){}
  virtual ~MyButton(){
    printf("debug : ~MyButton\n");
  }
public slots:
  void exitLoop();
};

#include <QtOpenGL/qgl.h> //G.Barrand QtOpenGL

class MyOpenGLArea : public QGLWidget {
  Q_OBJECT
public:
  MyOpenGLArea(QWidget* aParent = 0):QGLWidget(aParent){}
public:
  virtual ~MyOpenGLArea(){
    ::printf("debug : ~MyOpenGLArea\n");
  }
  virtual void initialieGL();
  virtual void resizeGL(int,int);
  virtual void paintGL();
  virtual void mousePressEvent(QMouseEvent*);
  virtual void mouseReleaseEvent(QMouseEvent*);
  virtual void mouseMoveEvent(QMouseEvent*);
};

