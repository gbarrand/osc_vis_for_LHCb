
#include <OnX/Qt/Callbacks.h>

#include <Slash/Core/ISession.h>

#include <inlib/sprintf>

#include <inlib/xml/tree>

#include <Lib/Debug.h>

#include <OnX/Qt/QtTools.h>
#include <OnX/Qt/QCallbackData.h>
#include <OnX/Qt/QCommandLine.h>

#include <OnX/Core/BaseUI.h>

#include <OnX/Qt/QTk.h>

//#define DEBUG_DESTROY

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qcolordialog.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qslider.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qscrollbar.h>
#include <QtGui/qlistwidget.h>
#include <QtGui/qtreewidget.h>
#include <QtGui/qmenudata.h> // for QMenuItem definition
#else
#include <QtWidgets/qcolordialog.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qscrollbar.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qtreewidget.h>
#endif

#include <QtGui/qevent.h>

#define S_QListWidget  "QListWidget"
#define S_QTreeWidget  "QTreeWidget"
#define S_QDialog      "QDialog"
#define S_QGLWidget    "QGLWidget"
#define S_QTabWidget   "QTabWidget"
#define S_QSlider      "QSlider"
#define S_QLineEdit    "QLineEdit"
#define S_QComboBox    "QComboBox"
#define S_QScrollBar   "QScrollBar"
#define S_QCheckBox    "QCheckBox"
#define S_QLabel       "QLabel"
#define S_QComboBox    "QComboBox"
#define S_QCheckBox    "QCheckBox"
#define S_QSplitter    "QSplitter"
#define S_QMenuBar     "QMenuBar"
#define S_QMenu        "QMenu"
#define S_QToolBar     "QToolBar"
#define S_QPushButton  "QPushButton"
#define S_QLineEdit    "QLineEdit"

#define S_OnX_QFileDialog     "OnX_QFileDialog"
#define S_OnX_QToggleMenuItem "OnX_QToggleMenuItem"
#define S_OnX_QMenuItem       "OnX_QMenuItem"
#define S_OnX_QMenu           "OnX_QMenu"
#define S_OnX_QColorDialog    "OnX_QColorDialog"
#define S_OnX_QOpenGLArea     "OnX_QOpenGLArea"
#define S_OnX_QSplitter       "OnX_QSplitter"

//FIXME : OnX_ -> OnX::

QtCallback::QtCallback(
 Slash::UI::IUI& aUI
,Slash::UI::IWidget* aWidget
,OnX::Action::NativeWidget aComponent
,const std::string& aInterp
,const std::string& aScript
,const Slash::UI::IUI::Options& aParams
,const std::string& aName
)
: OnX::Action(aUI,aWidget,aComponent,aInterp,aScript,aParams,aName,0)
{
  Lib::Debug::increment("QtCallback");
}
QtCallback::~QtCallback() {
  Lib::Debug::decrement("QtCallback");
}

QtCallback::QtCallback(const QtCallback& aFrom): OnX::Action(aFrom){}
QtCallback& QtCallback::operator=(const QtCallback&){return *this;}

//QObject* QtCallback::cast() const {
//  return (QObject*)static_cast<const QObject*>(this);
//}
//void QtCallback::destroy(){}
void QtCallback::execute(){
  std::string svalue;
  // Retreive "value" :
  QWidget* widget = (QWidget*)nativeWidget();
  //printf("debug : QtCallback::execute : %s\n",widget->className());
  //WARNING : have QCommandLine before QLineEdit.
  if(widget->inherits("OnX::QCommandLine")) {
    svalue = ((OnX::QCommandLine*)widget)->getValue();
  } else if(widget->inherits(S_QLineEdit)) {
    svalue = ((QLineEdit*)widget)->text().toStdString();
  } else if(widget->inherits(S_OnX_QFileDialog)) {
    svalue = ((OnX_QFileDialog*)widget)->value();
  } else if(widget->inherits(S_QCheckBox)) {
    svalue = ((QCheckBox*)widget)->isChecked()?"true":"false";
  } else if(widget->inherits(S_QListWidget)) {
    QListWidgetItem* item = ((QListWidget*)widget)->currentItem();
    svalue = item?item->text().toStdString():"";
  } else if(widget->inherits(S_QTreeWidget)) {
    if (OnX::Action::name()=="select") {
      QTreeWidgetItem* item = ((QTreeWidget*)widget)->currentItem();
      if(!item) return;
      if(item->childCount()) return; //branch
      svalue = OnX::QTreeWidgetItemGetPath(*item);
    } else if (OnX::Action::name()=="selectBranch") {
      QTreeWidgetItem* item = ((QTreeWidget*)widget)->currentItem();
      if(!item) return;
      if(!item->childCount()) return; //leaf
      svalue = OnX::QTreeWidgetItemGetPath(*item);
    } else {
      return;
    }
  } else if(widget->inherits(S_QScrollBar)) {
    inlib::sprintf(svalue,64,"%d",((QScrollBar*)widget)->value());
  } else if(widget->inherits(S_QSlider)) {
    inlib::sprintf(svalue,64,"%d",((QSlider*)widget)->value());
  } else if(widget->inherits(S_OnX_QToggleMenuItem)) {
    bool value;
    if(!((OnX_QToggleMenuItem*)widget)->isChecked(value)) return;
    svalue = value?"true":"false";
  } else if(widget->inherits(S_QComboBox)) {
    svalue = ((QComboBox*)widget)->currentText().toStdString();
  } else if(widget->inherits(S_OnX_QColorDialog)) {
    svalue = ((OnX_QColorDialog*)widget)->value();
  } else if(widget->inherits(S_QTabWidget)) {
    QTabWidget* w = (QTabWidget*)widget;
    if(w->currentWidget()) {
      svalue = w->tabText(w->indexOf(w->currentWidget())).toStdString();
    }
  }

  if(ui().session().verboseLevel()) {
    std::ostream& out = ui().session().out();
    out << "OnX::QtCallback::execute :"
        << " widget " << widget
        << " action " << this
        << " value " << inlib::sout(svalue)
        << " script " << inlib::sout(script()) << "." 
        << std::endl;
  }

  ui().setCallbackData(OnX::CallbackData(OnX::Action::widget(),
    widget->objectName().toStdString(),
    svalue,"",0,0));

  // Execute script :
  OnX::Action::execute();

  ui().setCallbackData(OnX::CallbackData());
}
void QtCallback::execute2(QCallbackData& aData){
  QWidget* widget = (QWidget*)nativeWidget();
  std::string svalue = aData.value;

  if(ui().session().verboseLevel()) {
    std::ostream& out = ui().session().out();
    out << "OnX::QtCallback::execute2 :"
        << " value " << inlib::sout(svalue) << "." 
        << std::endl;
  }
  ui().setCallbackData(OnX::CallbackData(OnX::Action::widget(),
    widget->objectName().toStdString(),
    svalue,aData.event,aData.x,aData.y));
  OnX::Action::execute();
  ui().setCallbackData(OnX::CallbackData());
}

void QtCallback::execute3(QTreeWidgetItem* aItem){
  //printf("debug : execute3 \"%s\"\n",OnX::Action::name().c_str());
  std::string svalue = OnX::QTreeWidgetItemGetPath(*aItem);
  if (OnX::Action::name()=="open") {
    setBackground(true);
  } else if (OnX::Action::name()=="close") {
    setBackground(true);
  } else {
    return;
  }
  QWidget* widget = (QWidget*)nativeWidget();
  if(ui().session().verboseLevel()) {
    std::ostream& out = ui().session().out();
    out << "OnX::QtCallback::execute3 :"
        << " value " << inlib::sout(svalue) << "." 
        << std::endl;
  }
  ui().setCallbackData(OnX::CallbackData(OnX::Action::widget(),
    widget->objectName().toStdString(),
    svalue,"",0,0));
  OnX::Action::execute();
  ui().setCallbackData(OnX::CallbackData());
}

OnX::CloseCallback::CloseCallback(BaseUI& aUI,inlib::xml::tree* aItemML)
: OnX::Action(aUI)
,fBaseUI(aUI)
,fItemML(aItemML)
{
  Lib::Debug::increment("OnX::CloseCallback");
}
OnX::CloseCallback::~CloseCallback() {
  Lib::Debug::decrement("OnX::CloseCallback");
}

OnX::CloseCallback::CloseCallback(const CloseCallback& aFrom)
: OnX::Action(aFrom),fBaseUI(aFrom.fBaseUI){}
OnX::CloseCallback& OnX::CloseCallback::operator=(const CloseCallback&){return *this;}


//QObject* OnX::CloseCallback::cast() const {
//  return (QObject*)static_cast<const QObject*>(this);
//}
void OnX::CloseCallback::execute(){
#ifdef DEBUG_DESTROY
  ::printf("debug : closeCallback : begin\n");
#endif
  inlib::xml::tree* top = fBaseUI.topItemML();
  if(top && fBaseUI.is_in(*top,fItemML)) {
#ifdef DEBUG_DESTROY
    ::printf("debug : closeCallback : exec destroy cbks\n");
#endif
    Execute tag(fBaseUI,"destroy");
    fItemML->post_execute_backward(BaseUI::executeItemMLCallbacks,&tag);

    fBaseUI.removeInRelations(*fItemML);
    fItemML->invalidate();

    inlib::xml::tree* parent = fItemML->parent();
    if(parent) parent->remove_child(fItemML,true);

  }
  fItemML = 0;
}

/* NOT : note needed.
OnX::DestroyCallback::DestroyCallback(
 BaseUI& aUI,inlib::xml::tree* aItemML,QWidget* aWidget)
:fItemML(aItemML)
,fWidget(aWidget)
{
  Lib::Debug::increment("OnX::DestroyCallback");
}
OnX::DestroyCallback::~DestroyCallback() {
  Lib::Debug::decrement("OnX::DestroyCallback");
}
//QObject* OnX::DestroyCallback::cast() const {
//  return (QObject*)static_cast<const QObject*>(this);
//}
void OnX::DestroyCallback::execute(){
#ifdef DEBUG_DESTROY
  ::printf("debug : DestroyCallback\n");
#endif
  //FIXME : fUI.removeQWidget(fWidget);
  inlib::xml::tree* top = fUI.topItemML();
  if(top && fUI.is_in(*top,fItemML)) {
#ifdef DEBUG_DESTROY
    ::printf("debug : invalidate ItemML\n");
#endif
    fUI.removeInRelations(*fItemML);
    fItemML->invalidate();
  }
  //fUI.removeActions(aWidget);
  fWidget = 0;
  fItemML = 0;
}
*/
