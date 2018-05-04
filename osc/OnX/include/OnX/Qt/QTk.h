#ifndef OnX_QTk_h
#define OnX_QTk_h 

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qdialog.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qcolordialog.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qaction.h>
#include <QtGui/qmenu.h>
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qaction.h>
#include <QtWidgets/qmenu.h>
#endif

#include <QtGui/qevent.h>
#include <QtOpenGL/qgl.h>

#include <inlib/S_STRING>
#include <Lib/Debug.h>

#include <OnX/Qt/QCallbackData.h>

// to handle QFileOpenEvent :
class OnX_QApplication : public QApplication {
  Q_OBJECT
public:
  OnX_QApplication(int& a_argc,char** a_argv)
  :QApplication(a_argc,a_argv)
  ,fCB(0),fTag(0)
  {}
  virtual ~OnX_QApplication(){}
public:
  typedef bool(*OpenFileCB)(const QString&,void*);
  void setOpenFileCallback(OpenFileCB aCB,void* aTag){
    fCB = aCB;
    fTag = aTag;
  }
protected:
  virtual bool event(QEvent* a_event) {
    if(a_event->type()==QEvent::FileOpen) {
      if(fCB) {
        return fCB(static_cast<QFileOpenEvent*>(a_event)->file(),fTag);
      }
    }
    return QApplication::event(a_event);
  }
private:
  OpenFileCB fCB;
  void* fTag;
};

class OnX_QOpenGLArea : public QGLWidget {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QOpenGLArea)
public:
  OnX_QOpenGLArea(QWidget* aParent):QGLWidget(aParent){
    Lib::Debug::increment(s_class());
  }
  virtual ~OnX_QOpenGLArea(){
    Lib::Debug::decrement(s_class());
  }
public:
  virtual void initialieGL() {}
  virtual void resizeGL(int,int){}
  virtual void paintGL() {
    QCallbackData data;
    emit paint(data);
  }
  virtual void mousePressEvent(QMouseEvent* aEvent) {
    QCallbackData data;
    data.event = "ButtonPress";
    data.x = aEvent->x();
    data.y = aEvent->y();
    emit eventHandler(data);
  }
  virtual void mouseReleaseEvent(QMouseEvent* aEvent) {
    QCallbackData data;
    data.event = "ButtonRelease";
    data.x = aEvent->x();
    data.y = aEvent->y();
    emit eventHandler(data);
  }
  virtual void mouseMoveEvent(QMouseEvent* aEvent) {
    QCallbackData data;
    data.event = "MotionNotify";
    data.x = aEvent->x();
    data.y = aEvent->y();
    emit eventHandler(data);
  }
signals:
  void paint(QCallbackData&);
  void eventHandler(QCallbackData&);
public:
  bool write_gl2ps(const std::string&,const std::string&); 
};

// To handle splitPosition at startup.
class OnX_QSplitter : public QSplitter {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QSplitter)
public:
  OnX_QSplitter(Qt::Orientation aOrientation,QWidget* aParent)
  :QSplitter(aOrientation,aParent)
  ,fSplitPosition(0){
    Lib::Debug::increment(s_class());
  }
  virtual ~OnX_QSplitter(){
    Lib::Debug::decrement(s_class());
  }
public:
  void setSplitPosition(int aPosition) {fSplitPosition = aPosition;}
  virtual void showEvent(QShowEvent* aEvent){
    QList<int> vals = sizes();
    if(vals.size()==2) {
      vals[0] = fSplitPosition;
      vals[1] = (orientation()==Qt::Vertical ? height() : width());
      vals[1] -= fSplitPosition;
      setSizes(vals);
    }
    QSplitter::showEvent(aEvent);
  }
private:
  int fSplitPosition;
};

#if QT_VERSION < 0x040600
class OnX_QColorDialog_Base : public QWidget {
#else
class OnX_QColorDialog_Base : public QColorDialog {
#endif
public:
  OnX_QColorDialog_Base();
  virtual ~OnX_QColorDialog_Base();
};

class OnX_QColorDialog : public OnX_QColorDialog_Base {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QColorDialog)
public:
  OnX_QColorDialog();
  virtual ~OnX_QColorDialog(){
    Lib::Debug::decrement(s_class());
  }
public:
  std::string value() const {return fValue;}
  void setOkHide(bool);
signals:
  void ok();
private slots:
  void color_selected();
#if QT_VERSION < 0x040600
public:
  void map();
#else
private:
  virtual void done(int);
  virtual void setVisible(bool);
private:
  bool f_ok_hide;
  bool f_do_done;
#endif
private:
  std::string fValue;
};

class OnX_QFileDialog : public QWidget {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QFileDialog)
public:
  virtual void map(){
    if(fMode==AcceptSave) {
      QString s = QFileDialog::getSaveFileName(0,
                        "Choose a filename to save under",
                        fDirectory.c_str(),fFilter.c_str());
      fValue = s.toStdString();
    } else { //AcceptOpen
      QString s = QFileDialog::getOpenFileName(0,
                        "Choose a filename to open",
                        fDirectory.c_str(),fFilter.c_str());
      fValue = s.toStdString();
    }
    emit ok();
    fValue = "";
  }
public:
  OnX_QFileDialog():QWidget(0),fMode(AcceptOpen){
    Lib::Debug::increment(s_class());
  }
  virtual ~OnX_QFileDialog(){
    Lib::Debug::decrement(s_class());
  }
public:
  std::string value() const {return fValue;}
  enum AcceptMode { AcceptOpen, AcceptSave };
  void setAcceptMode(AcceptMode aMode) {fMode = aMode;}
  void setFilter(const std::string& aFilter) {fFilter = aFilter;}
  void setDirectory(const std::string& aDirectory) {fDirectory = aDirectory;}
signals:
  void ok();
private:
  AcceptMode fMode;
  std::string fFilter;
  std::string fDirectory;
  std::string fValue;
};

class OnX_QMenu : public QWidget {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QMenu)
public:
  OnX_QMenu(QMenu& aPopup):QWidget(0),fPopup(aPopup){
    Lib::Debug::increment(s_class());
  }
  virtual ~OnX_QMenu(){Lib::Debug::decrement(s_class());}
public:
  QMenu& popupMenu() const{ return fPopup;}
private:
  QMenu& fPopup;
};

class OnX_QMenuItem : public QWidget {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QMenuItem)
public:
  OnX_QMenuItem(QMenu& aPopup,const std::string& aLabel)
  :QWidget(0),fPopup(aPopup),fAction(0) {
    fAction = fPopup.addAction(aLabel.c_str(),this,SLOT(execute()));
    Lib::Debug::increment(s_class());
  }
  virtual ~OnX_QMenuItem(){
    fPopup.removeAction(fAction);
    delete fAction;
    Lib::Debug::decrement(s_class());
  }
public:
  void setAccel(const std::string& aValue) {
    //FIXME
  }
  QMenu& popupMenu() const{ return fPopup;}

  //int id() const{ return fID;}
  bool label(std::string& aValue) const{ 
    aValue = fAction->text().toStdString();
    return true;
  }
  bool setLabel(const std::string& aValue){ 
    fAction->setText(aValue.c_str());
    return true;
  }
signals:
  void activated();
public slots:
  void execute(){ emit activated();} //the popup activates it.
protected:
  QMenu& fPopup;
  QAction* fAction;
};

class OnX_QToggleMenuItem : public OnX_QMenuItem {
  Q_OBJECT
public:
  OnX_QToggleMenuItem(QMenu& aPopup,const std::string& aLabel)
  :OnX_QMenuItem(aPopup,aLabel){
    fAction->setCheckable(true);
  }
  virtual ~OnX_QToggleMenuItem(){}
public:
  bool isChecked(bool& aValue) const{ 
    aValue = fAction->isChecked()?true:false;
    return true;
  }
  bool setChecked(bool aValue){ 
    fAction->setChecked(aValue);
    return true;
  }
};

class OnX_QMenuSeparator : public QWidget {
  Q_OBJECT
private:
  INLIB_SCLASS(OnX_QMenuSeparator)
public:
  OnX_QMenuSeparator(QMenu& aPopup):QWidget(0),fPopup(aPopup),fAction(0) {
    fAction = new QAction(&fPopup);
    fAction->setSeparator(true);
    fPopup.addAction(fAction);
    Lib::Debug::increment(s_class());
  }
  virtual ~OnX_QMenuSeparator(){
    fPopup.removeAction(fAction);
    delete fAction;  
    Lib::Debug::decrement(s_class());
  }
protected:
  QMenu& fPopup;
  QAction* fAction;
};

class OnX_QMainWindow : public QMainWindow {
  Q_OBJECT
public:
  OnX_QMainWindow(QWidget* aParent,Qt::WindowFlags aFlags)
  :QMainWindow(aParent,aFlags)
  {}
  virtual ~OnX_QMainWindow(){}
protected:
  virtual void closeEvent(QCloseEvent* aEvent) {
    emit close_window();
    QMainWindow::closeEvent(aEvent);
  }
signals:
  void close_window(); //can't be close. It clashes with QWidget::close.
};

#endif
