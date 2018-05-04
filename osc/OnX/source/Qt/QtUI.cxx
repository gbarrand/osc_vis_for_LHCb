// this :
#include <OnX/Qt/QtUI.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IWidgetClass.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/UI/IWidget.h>

#include "../Core/inlib"

#include <inlib/system>
#include <inlib/cast>

#include <exlib/xml/parser>

#include <Lib/Debug.h>
#include <Lib/Messages.h>

#include <OnX/Core/Widgets.h>

//#define DEBUG_DESTROY

#include <OnX/Qt/QtCyclic.h>
#include <OnX/Qt/QCommandLine.h>
#include <OnX/Qt/QtTools.h>
#include <OnX/Qt/QTk.h>
#include <OnX/Qt/Callbacks.h>

namespace OnX {
class QCustomEvent : public QEvent {
public:
  QCustomEvent(BaseUI& aUI,Slash::Core::IMessage* aMess)
  :QEvent(QEvent::User)
  ,fUI(aUI)
  ,fMessage(aMess){
    Lib::Debug::increment("OnX::QCustomEvent");
  }
  virtual ~QCustomEvent() {
    Lib::Debug::decrement("OnX::QCustomEvent");
  }
public:
  BaseUI& fUI;
  Slash::Core::IMessage* fMessage;
};
}
class OnX_QMessageWidget : public QWidget {
protected:
  void customEvent(QEvent* aEvent) {
    OnX::QCustomEvent* event = dynamic_cast<OnX::QCustomEvent*>(aEvent);
    if(!event) return;
    Slash::Core::IMessage* message = event->fMessage;
    //delete guiMessage;
    event->fUI.notification(message);
  }
};

#define ONX_HAS_INVENTOR
#ifdef ONX_HAS_INVENTOR
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
//#include <Inventor/Qt/viewers/SoQtWalkViewer.h>
#include <Inventor/Qt/viewers/SoQtFlyViewer.h>
#include <Slash/Tools/RegionSceneHandler.h>
#include <OnX/Inventor/Inventor.h>
#include <OnX/Inventor/HEPVis.h>
#include <OnX/Inventor/GuiViewer.h>
#include <HEPVis/nodekits/SoPage.h>
#include <OnX/Qt/QPageViewer.h>
#define S_OnX_QPageViewer "OnX::QPageViewer"
#endif

#include <QtGui/qpainter.h>
#include <QtOpenGL/qgl.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qobject.h>

#include <QtCore/qglobal.h>

#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qtoolbutton.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qslider.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qprogressbar.h>
#include <QtGui/qfiledialog.h>
#include <QtGui/qabstractbutton.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qscrollbar.h>
#include <QtGui/qlistwidget.h>
#include <QtGui/qtextedit.h>
#include <QtGui/qheaderview.h>
#include <QtGui/qtreewidget.h>
#include <QtGui/qmenudata.h> // for QMenuItem definition
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qsplitter.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qtooltip.h>
#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qtabwidget.h>
#include <QtWidgets/qslider.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qabstractbutton.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qscrollbar.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qtreewidget.h>
#endif

#include "q3_hbox.h"
#include "q3_vbox.h"

#define S_QAbstractButton "QAbstractButton"
#define S_QMainWindow  "QMainWindow"
#define S_QProgressBar "QProgressBar"
#define S_QListWidget  "QListWidget"
#define S_QTextEdit    "QTextEdit"
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

#define S_OnX_QToggleMenuItem "OnX_QToggleMenuItem"
#define S_OnX_QMenuItem       "OnX_QMenuItem"
#define S_OnX_QMenu           "OnX_QMenu"
#define S_OnX_QFileDialog     "OnX_QFileDialog"
#define S_OnX_QColorDialog    "OnX_QColorDialog"
#define S_OnX_QOpenGLArea     "OnX_QOpenGLArea"
#define S_OnX_QCommandLine    "OnX::QCommandLine"
#define S_OnX_Q3VBox          "OnX_Q3VBox"
#define S_OnX_Q3HBox          "OnX_Q3HBox"

static QObject* find_parent(QObject*,const std::string&);

#include "proxy.icxx"

#if QT_VERSION < 0x050000
#ifdef __APPLE__
extern void qt_mac_set_native_menubar(bool);
#endif
#endif

bool openFileCB(const QString& aFile,void* aTag){
  OnX::BaseUI* ui = (OnX::BaseUI*)aTag;
  ui->openFile(aFile.toStdString());
  return true;
}

inline QString s2q(const std::string& a_s) {
#if QT_VERSION < 0x050000
  return QString::fromAscii(a_s.c_str());
#else
  return QString::fromLatin1(a_s.c_str());
#endif
}

OnX::QtUI::QtUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs)
:BaseUI(aSession)
,fQApplication(0)
,fQApplication_owner(false)
,fConsoleText(0)
,fWarningDialog(0)
,fMessageWidget(0)
,fTermCyclic(0)
{
  if (!qApp) {    
#if QT_VERSION < 0x050000
#ifdef __APPLE__
    if(inlib::isenv("OSC_QT_MAC_MENUBAR")) {
    } else {
      qt_mac_set_native_menubar(false);
    }
#endif
#endif
    //QApplication::setColorSpec( QApplication::ManyColor );
    int* argc = new int;
    typedef char** argv_t;
    argv_t* argv = new argv_t;
    new_argcv(aArgs,*argc,*argv);
    OnX_QApplication* onx_qapp = new OnX_QApplication(*argc,*argv);
    onx_qapp->setOpenFileCallback(openFileCB,(OnX::BaseUI*)this);
    fQApplication = onx_qapp;
    fQApplication_owner = true;
  } else {
    fQApplication = qApp;
    fQApplication_owner = false;
  }  

  if(!fQApplication) {
    std::ostream& out = fSession.cout();
    out << "OnX::QtUI::QtUI :"
        << " null QApplication."
        << std::endl;
    return;
  }

  std::string value;
  if(aSession.parameterValue("UI_Manager.style",value) &&(value.size())) {
    // The string must be one of the QStyleFactory::keys(),
    // typically one of :
    //  "windows", "motif", "cde", "motifplus", "platinum", "sgi"
    //  "compact", "windowsxp", "aqua", "macintosh"
    QApplication::setStyle(value.c_str());
  }

  fMessageWidget = new OnX_QMessageWidget();

#ifdef ONX_HAS_INVENTOR
 {QWidget* qWidget = new OnX_Q3VBox();
  SoQt::init(qWidget);
  HEPVis::initClasses();
  OnX::Inventor::initialize(aSession);}
  if(!aSession.findManager(Slash_SceneHandler))  {
    aSession.addManager
      (new Slash::RegionSceneHandler(aSession,Slash_SceneHandler));
  }
#endif
}
//////////////////////////////////////////////////////////////////////////////
OnX::QtUI::QtUI(
 const QtUI& aFrom
)
:BaseUI(aFrom.fSession)
,fQApplication(0)
,fQApplication_owner(false)
,fConsoleText(0)
,fWarningDialog(0)
,fMessageWidget(0)
,fTermCyclic(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnX::QtUI::~QtUI(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fBeingDestroyed = true;

  // Treat messages emitted between the quiting of the steering
  // and here. They may come from the destruction of various
  // managers in the ~Session.
  synchronize();

  removeCyclics();

  while(!fWidgets.empty()) {
    QWidget* widget = *(fWidgets.rbegin());
    if(fVerboseLevel) {
      std::string name = widget->objectName().toStdString();
      std::string className = widget->metaObject()->className();
      std::ostream& out = fSession.cout();
      out << "OnX::QtUI::~QtUI :"
          << " delete "
          << "\"" << name << "\" of class " 
          << "\"" << className << "\"..." 
          << std::endl;
    }
    delete widget; //may induce a fWidget.remove(widget) !
    fWidgets.remove(widget);
  }

  fConsoleText = 0;
  fWarningDialog = 0;

  delete fMessageWidget;
  if(fQApplication_owner) delete fQApplication;
  
#ifdef ONX_HAS_INVENTOR
  OnX::Inventor::finalize(fSession); 
#endif
}
OnX::QtUI& OnX::QtUI::operator=(const QtUI&){ return *this;}

void* OnX::QtUI::cast(const std::string& a_class) const {
  INLIB_IF_CAST(OnX::QtUI)
  else return BaseUI::cast(a_class);
}

QApplication* OnX::QtUI::getQApplication() const {return fQApplication;}

void OnX::QtUI::put(const std::string& aString){
  // Append line at end of report region.
  if(!fConsoleText) {
    fConsoleText = (QWidget*)findNativeWidget("consoleText");
    if(fConsoleText) {
      fConsoleText->setFont(QFont("courier"));
      QScrollBar* vbar = ((QTextEdit*)fConsoleText)->verticalScrollBar();
      vbar->setValue(vbar->maximum());
    }
  }
  if(!fConsoleText) {
    ::printf("%s",aString.c_str());
    return;
  }
  ((QTextEdit*)fConsoleText)->insertPlainText(aString.c_str());
  QScrollBar* vbar = ((QTextEdit*)fConsoleText)->verticalScrollBar();
  vbar->setValue(vbar->maximum());
}

void OnX::QtUI::warn(const std::string& aString){
  // Modal method.
  if(!fWarningDialog) {
    fWarningDialog = (QMessageBox*)findNativeWidget("warningDialog");
  }
  if(!fWarningDialog) {
    echo("WARNING : "+aString);
    return;
  }
  fWarningDialog->setText(aString.c_str());
  fWarningDialog->exec();
}

int OnX::QtUI::nativeSteer(){
  if(!fQApplication) return 0;
  fSteerLevel++;

  QEventLoop* eventLoop = new QEventLoop();
  fEventLoopStack.push_front(eventLoop);
  int code = eventLoop->exec();

 {QEventLoop* eloop = fEventLoopStack.front();
  if(eloop!=eventLoop) {
    std::ostream& out = fSession.cout();
    out << "OnX::QtUI::nativeSteer :"
        << " problem with QEventLoops. fSteerLevel " << fSteerLevel << "."
        << std::endl;
    return 0;
  }}

  fEventLoopStack.pop_front();
  delete eventLoop;

  //int code = fQApplication->exec();
  fSteerLevel--;
  return code;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QtUI::synchronize(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //if(!fQApplication) return;
  //fQApplication->processEvents(); 

  if(fEventLoopStack.empty()) return;
  QEventLoop* eventLoop = fEventLoopStack.front();
  eventLoop->processEvents();
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QtUI::exit(
 int aExitCode
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //if(!fQApplication) return;
  //fQApplication->exit(aExitCode);

  if(fEventLoopStack.empty()) return;
  QEventLoop* eventLoop = fEventLoopStack.front();
  eventLoop->exit(aExitCode);
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::lock(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //SoQt::lock();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::unlock(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //SoQt::unlock();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::notify(
 Slash::Core::IMessage* aMessage
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fMessageWidget) return false;

  //lock();

  //FIXME : or done with fEventLoopStack ?
  if(!fQApplication) return false;
  fQApplication->postEvent(fMessageWidget,
    new OnX::QCustomEvent(*this,aMessage));

  //unlock();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::parameterValue(
 const std::string& aWhat
,std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::QtUI::parameterValue :"
        << " widget \"" << aWidget << "\""
        << " not found." << std::endl;
    return false;
  }

  QWidget* widget = (QWidget*)itemML->get_data1();
  if(!widget) return false;


    if(aResource=="background") {
      QColor color = widget->palette().color(widget->backgroundRole());
      aValue = inlib::to<double>(((double)color.red())/255.)+" " +
               inlib::to<double>(((double)color.green())/255.)+" "+
               inlib::to<double>(((double)color.blue())/255.);
      return true;
    } else if(aResource=="foreground") {
      QColor color = widget->palette().color(widget->foregroundRole());
      aValue = inlib::to<double>(((double)color.red())/255.)+" " +
               inlib::to<double>(((double)color.green())/255.)+" "+
               inlib::to<double>(((double)color.blue())/255.);
      return true;

    } else if(aResource=="items") {
      if(widget->inherits(S_QTreeWidget)) {
        QTreeWidget* tw = (QTreeWidget*)widget;
        QTreeWidgetItem* header = tw->invisibleRootItem();
        if(header && header->childCount()) {
          aValue = "<tree>"+
                   QTreeWidgetItemGetXML(*header)+
                   "</tree>";
        } else {
          aValue = "";
        }
        return true;
      } else if(widget->inherits(S_QListWidget)) {
        aValue = "";
        int number = ((QListWidget*)widget)->count();
        for(unsigned int index=0;index<number;index++){
          if(index) aValue += "\n";
          QListWidgetItem* item = ((QListWidget*)widget)->item(index);
          aValue += item->text().toStdString();          
        }
        return true;
      } else if(widget->inherits(S_QComboBox)) {
        std::vector<std::string> v;
        if(((QComboBox*)widget)->isEditable()) {
          v = QComboBoxGetItems(*((QComboBox*)widget));
        } else {
          v = QOptionMenuGetItems(*((QComboBox*)widget));
        }
        aValue = inlib::tos<std::string>(v,"\n");
        return true;
      } 

    } else if(aResource=="selection") {
      if(widget->inherits(S_QTabWidget)) {
        QTabWidget* w = (QTabWidget*)widget;
        if(w->currentWidget()) {
          int idx = w->indexOf(w->currentWidget());
          aValue = w->tabText(idx).toStdString();
        }
        return true;
      } else if(widget->inherits(S_QListWidget)) {
        QListWidgetItem* item = ((QListWidget*)widget)->currentItem();
        aValue = item?item->text().toStdString():"";
        return true;
      } else if(widget->inherits(S_QTreeWidget)) {
        aValue = QTreeWidgetGetSelection(*((QTreeWidget*)widget));
        return true;
      }
    } else if(aResource=="itemCount") {
      if(widget->inherits(S_QTabWidget)) {
        QTabWidget* w = (QTabWidget*)widget;
        inlib::sprintf(aValue,64,"%d",w->count());
        return true;
      }
    } else if(aResource=="value") {
      if(widget->inherits(S_QLineEdit)) {
        aValue = ((QLineEdit*)widget)->text().toStdString();
        return true;
      } else if(widget->inherits(S_QComboBox)) {
        if(((QComboBox*)widget)->isEditable()) {
          aValue = QComboBoxGetValue(*((QComboBox*)widget));
        } else {
          aValue = QOptionMenuGetValue(*((QComboBox*)widget));
        }
        return true;
      } else if(widget->inherits(S_QScrollBar)) {
        inlib::sprintf(aValue,64,"%d",((QScrollBar*)widget)->value());
        return true;
      } else if(widget->inherits(S_QSlider)) {
        inlib::sprintf(aValue,64,"%d",((QSlider*)widget)->value());
        return true;
      } else if(widget->inherits(S_QTextEdit)) {
        aValue = ((QTextEdit*)widget)->toPlainText().toStdString();
        return true;
      } else if(widget->inherits(S_QProgressBar)) {
        aValue = inlib::to<int>(((QProgressBar*)widget)->value());
        return true;
      }

    } else if(aResource=="minimum") {
      if(widget->inherits(S_QScrollBar)) {
        inlib::sprintf(aValue,64,"%d",((QScrollBar*)widget)->minimum());
        return true;
      } else if(widget->inherits(S_QSlider)) {
        inlib::sprintf(aValue,64,"%d",((QSlider*)widget)->minimum());
        return true;
      }

    } else if(aResource=="maximum") {
      if(widget->inherits(S_QScrollBar)) {
        inlib::sprintf(aValue,64,"%d",((QScrollBar*)widget)->maximum());
        return true;
      } else if(widget->inherits(S_QSlider)) {
        inlib::sprintf(aValue,64,"%d",((QSlider*)widget)->maximum());
        return true;
      }

    } else if(aResource=="set") {
      if(widget->inherits(S_QCheckBox)) {
        aValue = ((QCheckBox*)widget)->isChecked()?"true":"false";
        return true;
      } else if(widget->inherits(S_OnX_QToggleMenuItem)) {
        bool value;
        if(((OnX_QToggleMenuItem*)widget)->isChecked(value)) {
          aValue = value?"true":"false";
          return true;
        }
      }
    } else if(aResource=="label") {
      if(widget->inherits(S_QLabel)) {
        aValue = ((QLabel*)widget)->text().toStdString();
        return true;
      } else if(widget->inherits(S_QAbstractButton)) {
        aValue = ((QAbstractButton*)widget)->text().toStdString();
        return true;
      } else if(widget->inherits(S_QMenu)) {
        aValue = ((QMenu*)widget)->title().toStdString();
        return true;
      } else if(widget->inherits(S_OnX_QMenuItem)) {
        if(((OnX_QMenuItem*)widget)->label(aValue)) return true;
      }
    } else if(aResource=="tabLabel") {
      //Constraint resource, we have to work on the parent.
      //NOTE : widget->parentWidget() is not the QTabWidget !
      inlib::xml::tree* p = parentItemML(*itemML);
      if(p) {
        QWidget* parent = (QWidget*)(p->get_data1()); 
        if(parent && parent->inherits(S_QTabWidget)) {
          int idx = ((QTabWidget*)parent)->indexOf(widget);
          aValue = ((QTabWidget*)parent)->tabText(idx).toStdString();
          return true;
        }
      }
    }

  // Not found, then look in XML elements :
  if(aResource=="class") {
    return itemML->attribute_value(aResource,aValue);
  }
  return itemML->element_value(aResource,aValue);
}

// from Qt-4.5.3/qwidget.h :
static void reparent(QWidget& w,QWidget *parent, Qt::WindowFlags f, const QPoint &p, bool showIt=false) { 
  w.setParent(parent, f);
  w.setGeometry(p.x(),p.y(),w.width(),w.height());
  if (showIt) w.show();
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::setParameter(
 const std::string& aWhat
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string aWidget;
  std::string aResource;
  if(!toWidgetAndResource(aWhat,aWidget,aResource)) return false;

  if(fVerboseLevel) {
    std::ostream& out = fSession.cout();
    out << "OnX::QtUI::setParameter :"
        << " widget \"" << aWidget << "\""
        << " resource \"" << aResource << "\""
        << " value \"" << aValue << "\"."
        << std::endl;
  }

  inlib::xml::tree* itemML = findItemML(aWidget);
  if(!itemML) {
    std::ostream& out = fSession.cout();
    out << "OnX::QtUI::setParameter :"
        << " widget \"" << aWidget << "\""
        << " not found." << std::endl;
    return false;
  }

  QWidget* widget = (QWidget*)itemML->get_data1();
  if(!widget) return false;

  std::string svalue;

  if(aResource=="background") {
    if(widget->inherits(S_QAbstractButton)) {
      QButtonSetBackground(*((QAbstractButton*)widget),aValue);
    } else if(widget->inherits(S_QComboBox)) {
      QComboBoxSetBackground(*((QComboBox*)widget),aValue);
    } else {
      QWidgetSetBackground(*widget,aValue);
      //widget->setStyleSheet("QWidget { background: FF00AA }");
    }
    return true;
  } else if(aResource=="foreground") {
    QWidgetSetForeground(*widget,aValue);
    return true;
  } else if(aResource=="decoration") {
    QWidget* top = widget->topLevelWidget();
    int sx = top->x();
    int sy = top->y();
#ifdef __APPLE__
    if(sy<=23) sy = 23;  //To be under Apple menubar.
#endif
    Qt::WindowFlags flags = decorationFlags(parseDecorations(aValue));
    if(top->inherits(S_QDialog)) flags |= Qt::Dialog;
    reparent(*top,top->parentWidget(),flags,top->pos(),top->isVisible());
    top->move(sx,sy);
    top->resize(top->size());
    return true;
  } else if(aResource=="label") {
    if(widget->inherits(S_QLabel)) {
      ((QLabel*)widget)->setText(aValue.c_str());
      return true;
    } else if(widget->inherits(S_QMenu)) {
      ((QMenu*)widget)->setTitle(aValue.c_str());
      return true;
    } else if(widget->inherits(S_OnX_QMenuItem)) {
      if(((OnX_QMenuItem*)widget)->setLabel(aValue)) return true;
    } else if(widget->inherits(S_QAbstractButton)) {
      ((QAbstractButton*)widget)->setText(aValue.c_str());
      return true;
    }
  } else if(aResource=="expand") {
    // Should be in that order because QVBos herited OnX_Q3HBox
    if(widget->inherits(S_OnX_Q3VBox)) {
      return QVBoxSetExpand(*((OnX_Q3VBox*)widget),aValue);
    } else if(widget->inherits(S_OnX_Q3HBox)) {
      return QHBoxSetExpand(*((OnX_Q3HBox*)widget),aValue);
    }
  } else if(aResource=="dirMask") {
    if(widget->inherits(S_OnX_QFileDialog)) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      ((OnX_QFileDialog*)widget)->setFilter(svalue);
      return true;
    }
  } else if(aResource=="directory") {
    if(widget->inherits(S_OnX_QFileDialog)) {
      svalue = aValue;
      inlib::expand_vars(svalue);
      ((OnX_QFileDialog*)widget)->setDirectory(svalue);
      return true;
    }
  } else if(aResource=="mode") {
    if(widget->inherits(S_OnX_QFileDialog)) {
      if(aValue=="save") {
        ((OnX_QFileDialog*)widget)->setAcceptMode(OnX_QFileDialog::AcceptSave);
      } else {
        ((OnX_QFileDialog*)widget)->setAcceptMode(OnX_QFileDialog::AcceptOpen); 
      }
      return true;
    }
  } else if(aResource=="selection") {
    if(widget->inherits(S_QTabWidget)) {
      QTabWidget* w = (QTabWidget*)widget;
      if(QTabWidgetSetSelection(*w,aValue)) return true;
    } else if(widget->inherits(S_QListWidget)) {
      int number = ((QListWidget*)widget)->count();
      for(unsigned int index=0;index<number;index++){
        QListWidgetItem* item = ((QListWidget*)widget)->item(index);
        std::string s = item->text().toStdString();
        if(s==aValue) {
          ((QListWidget*)widget)->setCurrentItem(item);
          break;
        } 
      } 
      return true;
    } else if(widget->inherits(S_QTreeWidget)) {
      if(QTreeWidgetSetSelection(*((QTreeWidget*)widget),aValue)) return true;
    } 
  } else if(aResource=="set") {
    if(widget->inherits(S_QCheckBox)) {
      bool value;
      if(inlib::to(aValue,value)) {
        ((QCheckBox*)widget)->setChecked(value);
        return true;
      }
    } else if(widget->inherits(S_OnX_QToggleMenuItem)) {
      bool value;
      if(inlib::to(aValue,value)) {
        if(((OnX_QToggleMenuItem*)widget)->setChecked(value)) return true;
        return true;
      }
    }
  } else if(aResource=="value") {
    if(widget->inherits(S_QScrollBar)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QScrollBar*)widget)->setValue(value);
        return true;
      }
    } else if(widget->inherits(S_QSlider)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QSlider*)widget)->setValue(value);
        return true;
      }
    } else if(widget->inherits(S_QLineEdit)) {
      ((QLineEdit*)widget)->setText(aValue.c_str());
      return true;
    } else if(widget->inherits(S_QTextEdit)) {
      ((QTextEdit*)widget)->setText(aValue.c_str());
      return true;
    } else if(widget->inherits(S_QProgressBar)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QProgressBar*)widget)->setValue(value);
        return true;
      }
    } else if(widget->inherits(S_QComboBox)) {
      if(((QComboBox*)widget)->isEditable()) {
        QComboBoxSetValue(*((QComboBox*)widget),aValue);
        return true;
      } else {
        if(QOptionMenuSetDefault(*((QComboBox*)widget),aValue)) return true;
      }
    }
  } else if(aResource=="minimum") {
    if(widget->inherits(S_QScrollBar)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QScrollBar*)widget)->setMinimum(value);
        return true;
      }
    } else if(widget->inherits(S_QSlider)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QSlider*)widget)->setMinimum(value);
        return true;
      }
    }
  } else if(aResource=="maximum") {
    if(widget->inherits(S_QScrollBar)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QScrollBar*)widget)->setMaximum(value);
        return true;
      }
    } else if(widget->inherits(S_QSlider)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        ((QSlider*)widget)->setMaximum(value);
        return true;
      }
    }
  } else if(aResource=="default") {
    if(widget->inherits(S_QComboBox)) {
      if(((QComboBox*)widget)->isEditable()) {
        QComboBoxSetValue(*((QComboBox*)widget),aValue);
        return true;
      } else {
        return QOptionMenuSetDefault(*((QComboBox*)widget),aValue);
      }
    }
  } else if(aResource=="items") {
    if(widget->inherits(S_QListWidget)) {
      ((QListWidget*)widget)->clear();
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      unsigned int itemn = items.size();
      for(unsigned int index=0;index<itemn;index++){
        ((QListWidget*)widget)->addItem((char*)items[index].c_str());
      }
      return true;
    } else if(widget->inherits(S_QTreeWidget)) {
      //WARNING : could be executed from a execute3 callback.
      //          The below clear() induces a crash.

      // disable the scripts of the callback to avoid recursive loop 
      // if someone has define the same callback in the script.
      std::vector<std::string> scripts = 
        disableCallbackScript(widget,"select");
      // retrieve the old tree in a string
      QTreeWidget* tw = (QTreeWidget*)widget;
      std::string oldXMLTree;
     {QTreeWidgetItem* header = tw->invisibleRootItem();
      if(header && header->childCount()) {
        oldXMLTree = QTreeWidgetItemGetXML(*header);
      }}
      //retrieve the old selection
      std::string selection = QTreeWidgetGetSelection(*tw);
      // clear the old tree in memory
      QTreeWidgetClear(*tw);
      if(aValue.find("<tree>")!=std::string::npos) {
        //if(aValue==oldXMLTree) return true;
        inlib::xml::default_factory factory;
        exlib::xml::parser treeML(factory,out(),fSession.verboseLevel());
        std::vector<std::string> tags;
        tags.push_back("tree");
        tags.push_back("treeItem");
        treeML.set_tags(tags);
        if(treeML.load_string(aValue)) {
          inlib::xml::tree* top = treeML.top_item();
          //top->dump();
          if(top) {
            if (oldXMLTree.size()) {
              oldXMLTree = "<tree>"+oldXMLTree+"</tree>";
	      exlib::xml::parser oldTreeML(factory,out(),fSession.verboseLevel());
              std::vector<std::string> tags2;
              tags2.push_back("tree");
              tags2.push_back("treeItem");
              oldTreeML.set_tags(tags2);
              if(oldTreeML.load_string(oldXMLTree)) {
                inlib::xml::tree* old = oldTreeML.top_item();
                if (old) {
                  top->update_tree(*old);
                }
              }
            }
            createTree(widget,top);
          }
        }
      } else {
        std::vector<std::string> items;
        smanip_lines(aValue,items);
        unsigned int itemn = items.size();
        for(unsigned int index=0;index<itemn;index++){
          QTreeWidgetItem* item = new QTreeWidgetItem(tw,0);
          item->setText(0,(char*)items[index].c_str());
        }
      }
      //restore old selection
      QTreeWidgetSetSelection(*tw,selection);
      // enable callback script
      enableCallbackScript(widget,"select",scripts);
      return true;
    } else if(widget->inherits(S_QComboBox)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      if(((QComboBox*)widget)->isEditable()) {
        QComboBoxSetItems(*((QComboBox*)widget),items);
      } else {
        QOptionMenuInitialize(*((QComboBox*)widget),items,"");
      }
      return true;
    }
  } else if(aResource=="splitPosition") {
    if(widget->inherits(S_QSplitter)) {
      int value;
      if(inlib::to<int>(aValue,value)) {
        QSplitter* w = (QSplitter*)widget;
        QList<int> sizes = w->sizes();
        if(sizes.size()==2) {
          sizes[0] = value;
          sizes[1] = (w->orientation()==Qt::Vertical ? 
                      w->height() : w->width());
          sizes[1] -= value;
          w->setSizes(sizes);
        }
        return true;
      }
    }
  } else if(aResource=="popupItems") {
#ifdef ONX_HAS_INVENTOR
    if(widget->inherits(S_OnX_QPageViewer)) {
      std::vector<std::string> items;
      smanip_lines(aValue,items);
      for(unsigned int index=0;index<items.size();index++)
        ((QPageViewer*)widget)->addPopupEntry(items[index]);
      return true;
    }
#endif
  } else if(aResource=="tabLabel") {
    //Constraint resource, we have to work on the parent.
    //NOTE : widget->parentWidget() is not the QTabWidget !
    inlib::xml::tree* p = parentItemML(*itemML);
    if(p) {
      QWidget* parent = (QWidget*)(p->get_data1()); 
      if(parent && parent->inherits(S_QTabWidget)) {
        //int index = ((QTabWidget*)parent)->indexOf(widget);
        //((QTabWidget*)parent)->setTabText(index,aValue.c_str());
        int idx = ((QTabWidget*)parent)->indexOf(widget);
        ((QTabWidget*)parent)->setTabText(idx,aValue.c_str());
        return true;
      }
    }
  } else if(aResource=="title") {
    widget->topLevelWidget()->setWindowTitle(aValue.c_str());
    return true;
  } else if(aResource=="geometry") {
    int x,y,w,h;
    if(!to_geometry(aValue.c_str(),x,y,w,h)) {
      std::ostream& out = fSession.cout();
      out << "QtUI::setParameter :"
          << " \"" << aValue << "\" : bad value for geometry." 
          << std::endl;
    } else {
#ifdef __APPLE__
      if(y<=23) y = 23;  //To be under Apple menubar.
#endif
      //WARNING : setGeometry positions client area.
      // for mainWindow it's a qtWidget and for QtDialog it'a a qtContainer
      // but qtWidget = qtContainer ...so...
      widget->topLevelWidget()->move(QPoint(x,y));
      widget->topLevelWidget()->resize(w,h);
      return true;
    }
  }

  std::ostream& out = fSession.cout();
  out << "OnX::QtUI::setParameter :"
      << " unable to set parameter for :"
      << " widget \"" << aWidget << "\""
      << " resource \"" << aResource << "\""
      << " value \"" << aValue << "\"."
      << std::endl;

  return false;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::createTree(
 QWidget* aTree
,inlib::xml::tree* aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<inlib::xml::tree*> list;
  aItemML->sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    std::string slabel;
    (*it)->element_value("label",slabel);

    std::string sopened;
    bool opened = false;
    if((*it)->element_value("opened",sopened)) {
      inlib::to(sopened,opened);
    }

    void* data1;
    void* data2;
    int dataInt;
    (*it)->parent()->get_data(data1,data2,dataInt);

    QTreeWidgetItem* item = 0;
    if(!data1) {
      item = new QTreeWidgetItem((QTreeWidget*)aTree,0);
      item->setText(0,slabel.c_str());
    } else {
      item = new QTreeWidgetItem((QTreeWidgetItem*)data1,0);
      item->setText(0,slabel.c_str());
    }
    aTree->blockSignals(true);
    item->setExpanded(opened);
    aTree->blockSignals(false);

    (*it)->set_data(item,0,0);
    if(!createTree(aTree,*it)) return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::createNativeWidget(
 inlib::xml::tree& aItemML                      
,const std::string& aClass
,const std::string& aName
,inlib::xml::tree* aParent
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
    OnX::Widget* owidget = INLIB_CAST(aItemML,OnX::Widget);
    if(!owidget) {
      std::ostream& out = fSession.cout();
      out << "QtUI::createNativeWidget :"
          << " ItemML not a OnX::Widget." 
          << aClass << std::endl;
      return false;
    }

    std::string qtClass = getType(aClass);
    if(qtClass.empty()) {
      std::ostream& out = fSession.cout();
      out << "QtUI::createNativeWidget :"
          << " unknown widget type : " 
          << aClass << std::endl;
      return false;
    }
    
    // Get parent :
    QWidget* qtContainer = 0;
    QWidget* qtParent = 0;
    std::string qtParentClass;
    std::string sparentclass;
    if(aParent) {
      qtParent = (QWidget*)aParent->get_data1();
      aParent->attribute_value("class",sparentclass);
      qtParentClass = getType(sparentclass);
    } 

#define QT_PARENT (qtParent->inherits(S_QTabWidget)?0:qtParent)

    // Check parent number of children :
    if (((qtParentClass=="HBox") ||
         (qtParentClass=="VBox") ||
         (qtParentClass=="HPaned") ||
         (qtParentClass=="VPaned")) && 
         (QWidgetGetNumberOfChildren(*qtParent)>2) ) {
      std::ostream& out = fSession.cout();
      out << "QtUI::createNativeWidget :"
          << " could not create a third widget in  : " 
          << sparentclass
          << std::endl;
      return false;
    }
    
#define CHECK_PARENT \
      if(!qtParent) {\
        std::ostream& out = fSession.cout();\
        out << "QtUI::createNativeWidget : " << aClass \
            << " needs a parent." << std::endl;\
        return false;\
      }  

    // Create the Qt widget :
    std::string svalue;
    std::string sdef;
    QWidget* qtWidget = 0;

    //FIXME: Use QMetaObject to create widget in a generic way ?

    if(qtClass=="Window") {
      qtParent = 0;
      aItemML.element_value("decoration",svalue);
      Qt::WindowFlags flags = decorationFlags(parseDecorations(svalue));
      qtWidget = new OnX_QMainWindow(qtParent,flags);
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget);

      //QObject::connect(fQApplication,SIGNAL(lastWindowClosed()),
      //                 fQApplication,SLOT(quit()));

    } else if(qtClass=="MenuBar") {
      QMainWindow* mainWindow = 
        (QMainWindow*)find_parent(qtParent,S_QMainWindow);
      if(!mainWindow) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " MenuBar needs a Window as parent." 
            << std::endl;
        return false;
      }
      qtWidget = mainWindow->menuBar();
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="CascadeButton") {
      CHECK_PARENT
      QWidget* parent = 0;
      if(qtParent->inherits(S_QMenuBar)) {
        parent = ((QMenuBar*)qtParent)->parentWidget();
        qtContainer = qtParent; //Used by setParameter.
      } else if(qtParent->inherits(S_OnX_QMenu)) {
        parent = &((OnX_QMenu*)qtParent)->popupMenu();
        qtContainer = parent; //Used by setParameter.
      } else {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " CascadeButton needs a MenuBar as parent " 
            << "(" << std::string(qtParent->metaObject()->className()) 
            << ") found."
            << std::endl;
        return false;
      }
      aItemML.element_value("label",svalue);
      QMenu* menu = new QMenu(parent);
      QAction* act = menu->menuAction();
      act->setText(svalue.c_str());
      qtWidget = menu;
      qtWidget->setObjectName(s2q(aName));
      // add in parent :
      if(qtParent->inherits(S_QMenuBar)) {
	((QMenuBar*)qtParent)->addAction(act);
      } else if(qtParent->inherits(S_OnX_QMenu)) {
	((OnX_QMenu*)qtParent)->popupMenu().addAction(act);
      } else {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " problem when creating CascadeButton in " 
            << "(" << std::string(qtParent->metaObject()->className()) << ")."
            << " " << std::string(parent->metaObject()->className())
            << std::endl;
        return false;
      }

    } else if(qtClass=="Menu") {
      CHECK_PARENT
      if(!qtParent->inherits(S_QMenu)) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " Menu needs a CascadeButton as parent."
            << " Parent is a " 
            << std::string(qtParent->metaObject()->className()) << "."
            << std::endl;
        return false;
      }
      qtWidget = new OnX_QMenu(*((QMenu*)qtParent));
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget); //FIXME : should have a parent !
    } else if(qtClass=="MenuItem") {
      CHECK_PARENT
      if(!qtParent->inherits(S_OnX_QMenu)) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " MenuItem needs a Menu as parent." << std::endl;
        return false;
      }
      QMenu& popup = ((OnX_QMenu*)qtParent)->popupMenu();
      aItemML.element_value("label",svalue);
      qtWidget = new OnX_QMenuItem(popup,svalue);
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget); //FIXME : should have a parent !
    } else if(qtClass=="ToggleMenuItem") {
      CHECK_PARENT
      if(!qtParent->inherits(S_OnX_QMenu)) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " ToggleMenuItem needs a Menu as parent." << std::endl;
        return false;
      }
      QMenu& popup = ((OnX_QMenu*)qtParent)->popupMenu();
      //popup.setCheckable(TRUE);
      aItemML.element_value("label",svalue);
      qtWidget = new OnX_QToggleMenuItem(popup,svalue);
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget); //FIXME : should have a parent !
    } else if(qtClass=="MenuSeparator") {
      CHECK_PARENT
      if(!qtParent->inherits(S_OnX_QMenu)) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " MenuSeparator needs a Menu as parent." << std::endl;
        return false;
      }
      QMenu& popup = ((OnX_QMenu*)qtParent)->popupMenu();
      qtWidget = new OnX_QMenuSeparator(popup);
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget); //FIXME : should have a parent !

    } else if(qtClass=="Dialog") {
      aItemML.element_value("decoration",svalue);
      Qt::WindowFlags flags = decorationFlags(parseDecorations(svalue));
      flags |= Qt::Dialog;
      QDialog* qDialog = new QDialog(qtParent,flags);
      qDialog->setObjectName(s2q(aName));
      qDialog->setModal(false);
      qtContainer = qDialog;
      QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom,qtContainer);
      std::string name = aName + "_vbox";
      qtWidget = new OnX_Q3VBox(qtContainer,name.c_str());
      layout->addWidget(qtWidget);
    } else if(qtClass=="FileSelection") {
      //aItemML.element_value("decoration",svalue);
      //Qt::WindowFlags flags = decorationFlags(parseDecorations(svalue));
      std::string dirMask;
      if(!aItemML.element_value("dirMask",dirMask)) dirMask = "*";
      inlib::expand_vars(dirMask);
      std::string directory;
      if(!aItemML.element_value("directory",directory)) directory = "";
      inlib::expand_vars(directory);
      qtWidget = new OnX_QFileDialog();      
      //qtWidget = new OnX_QFileDialog(qtParent,flags);      
      //qtWidget = new OnX_QFileDialog((QWidget*)0,(Qt::WindowFlags)0);      
      qtWidget->setObjectName(s2q(aName));
      ((OnX_QFileDialog*)qtWidget)->setAcceptMode(OnX_QFileDialog::AcceptOpen);
      ((OnX_QFileDialog*)qtWidget)->setFilter(dirMask);
      ((OnX_QFileDialog*)qtWidget)->setDirectory(directory);
      fWidgets.push_back(qtWidget);
    } else if(qtClass=="ColorSelection") {
      qtWidget = new OnX_QColorDialog();      
      ((OnX_QColorDialog*)qtWidget)->setOkHide(false);
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget);
    } else if(qtClass=="WarningDialog") {
      /*FIXME
      aItemML.element_value("decoration",svalue);
      Qt::WindowFlags flags = decorationFlags(parseDecorations(svalue));
      flags |= Qt::WType_Dialog;
      */
      qtWidget = new QMessageBox(qtParent);      
      qtWidget->setObjectName(s2q(aName));
      fWidgets.push_back(qtWidget);
    } else if(qtClass=="ToolBar") {
      QMainWindow* mainWindow = 
        (QMainWindow*)find_parent(qtParent,S_QMainWindow);
      if(!mainWindow) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " ToolBar needs a Window as parent." 
            << std::endl;
        return false;
      }
      qtWidget = new QToolBar(aName.c_str(),(QWidget*)mainWindow);
      mainWindow->addToolBar((QToolBar*)qtWidget);
    } else if(qtClass=="ToolBarItem") {
      CHECK_PARENT
      if(!qtParent->inherits(S_QToolBar)) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget :"
            << " ToolBarItem needs a QToolBar as parent." << std::endl;
        return false;
      }
      qtWidget = new QToolButton(0);
      qtWidget->setObjectName(s2q(aName));
      ((QToolBar*)qtParent)->addWidget(qtWidget);
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    } else if(qtClass=="ScrolledTree") {
      qtWidget = new QTreeWidget(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      ((QTreeWidget*)qtWidget)->header()->hide();
    } else if(qtClass=="ScrolledList") {
      qtWidget = new QListWidget(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="VBox") {
      qtWidget = new OnX_Q3VBox(QT_PARENT,aName.c_str());
    } else if(qtClass=="HBox") {
      qtWidget = new OnX_Q3HBox(QT_PARENT,aName.c_str());
    } else if(qtClass=="VContainer") {
      qtWidget = new OnX_Q3VBox(QT_PARENT,aName.c_str());
    } else if(qtClass=="HContainer") {
      qtWidget = new OnX_Q3HBox(QT_PARENT,aName.c_str());
    } else if(qtClass=="VPaned") {
      qtWidget = new OnX_QSplitter(Qt::Vertical,QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((OnX_QSplitter*)qtWidget)->setSplitPosition(value);
        }
      }
    } else if(qtClass=="HPaned") {
      qtWidget = new OnX_QSplitter(Qt::Horizontal,QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      if(aItemML.element_value("splitPosition",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          ((OnX_QSplitter*)qtWidget)->setSplitPosition(value);
        }
      }      
    } else if(qtClass=="Label") {
      qtWidget = new QLabel(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="PushButton") {
      qtWidget = new QPushButton(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="ToggleButton") {
      qtWidget = new QCheckBox(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="ScrolledText") {
      qtWidget = new QTextEdit(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      //((QTextEdit*)qtWidget)->setTextFormat(Qt::PlainText);
      ((QTextEdit*)qtWidget)->setReadOnly(false);
    } else if(qtClass=="Entry") {
      qtWidget = new QLineEdit(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="Password") {
      qtWidget = new QLineEdit(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      ((QLineEdit*)qtWidget)->setEchoMode(QLineEdit::Password);
    } else if(qtClass=="CommandLine") {
      qtWidget = new OnX::QCommandLine(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="Tree") {
      qtWidget = new QTreeWidget(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="OptionMenu") {
      qtWidget = new QComboBox(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      ((QComboBox*)qtWidget)->setEditable(false);
    } else if(qtClass=="ComboBox") {

      qtWidget = new QComboBox(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
      ((QComboBox*)qtWidget)->setEditable(true);

    } else if(qtClass=="HScrollBar") {

      qtWidget = new QScrollBar(Qt::Horizontal,QT_PARENT);
      qtWidget->setObjectName(s2q(aName));

    } else if(qtClass=="VScrollBar") {

      qtWidget = new QScrollBar(Qt::Vertical,QT_PARENT);
      qtWidget->setObjectName(s2q(aName));

    } else if(qtClass=="HScale") {

      qtWidget = new QSlider(Qt::Horizontal,QT_PARENT);
      qtWidget->setObjectName(s2q(aName));

    } else if(qtClass=="VScale") {

      qtWidget = new QSlider(Qt::Vertical,QT_PARENT);
      qtWidget->setObjectName(s2q(aName));

    } else if(qtClass=="ProgressBar") {
      qtWidget = new QProgressBar(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));
    } else if(qtClass=="TabStack") {

      qtWidget = new QTabWidget(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));

    } else if(qtClass=="OpenGLArea") {
      qtWidget = new OnX_QOpenGLArea(QT_PARENT);
      qtWidget->setObjectName(s2q(aName));

#ifdef ONX_HAS_INVENTOR
    } else if(qtClass=="SoExaminerViewer") {
      std::string name = aName + "_frame";

      qtContainer =  new QFrame(QT_PARENT);
      qtContainer->setObjectName(s2q(name));

      SoQtExaminerViewer* viewer = 
        new SoQtExaminerViewer(qtContainer,aName.c_str(),TRUE);
      qtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy
        (viewer,viewer_proxy<SoQtFullViewer,SoQtExaminerViewer>);
    } else if(qtClass=="SoPlaneViewer") {
      std::string name = aName + "_frame";

      qtContainer =  new QFrame(QT_PARENT);
      qtContainer->setObjectName(s2q(name));

      SoQtPlaneViewer* viewer = 
        new SoQtPlaneViewer(qtContainer,aName.c_str(),TRUE);
      qtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoQtFullViewer>);
/*FIXME
    } else if(qtClass=="SoWalkViewer") {
      std::string name = aName + "_frame";
      qtContainer =  new QFrame(QT_PARENT,name.c_str());
      SoQtWalkViewer* viewer = 
        new SoQtWalkViewer(qtContainer,aName.c_str(),TRUE);
      qtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoQtFullViewer>);
*/
    } else if(qtClass=="SoFlyViewer") {
      std::string name = aName + "_frame";

      qtContainer =  new QFrame(QT_PARENT);
      qtContainer->setObjectName(s2q(name));

      SoQtFlyViewer* viewer = 
        new SoQtFlyViewer(qtContainer,aName.c_str(),TRUE);
      qtWidget = viewer->getWidget();
      SoSeparator* sg = new SoSeparator;
      viewer->setSceneGraph(sg);
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) return false;
      soViewer->setSceneGraph(sg);
      soViewer->setViewerProxy(viewer,viewer_proxy<SoQtFullViewer>);
    } else if( (qtClass=="PageViewer") ||
               (qtClass=="PlanePageViewer") ){
      std::string vclass = "Examiner";
      if(qtClass=="PlanePageViewer") vclass = "Plane";
      std::string name = aName + "_frame";

      qtContainer =  new QFrame(QT_PARENT);
      qtContainer->setObjectName(s2q(name));

      qtWidget = new QPageViewer(qtContainer,vclass.c_str());
      qtWidget->setObjectName(s2q(aName));
      SoNode* soNode = ((QPageViewer*)qtWidget)->sceneGraph();
      SoQtFullViewer* viewer = ((QPageViewer*)qtWidget)->soViewer();
      OnX::SoViewer* soViewer = INLIB_CAST(aItemML,OnX::SoViewer);
      if(!soViewer) {
        std::ostream& out = fSession.cout();
        out << "QtUI::createNativeWidget : ItemML not a OnX::SoViewer."
            << std::endl;
        return false;
      }
      soViewer->setSceneGraph(soNode);
      if(viewer->isOfType(SoQtExaminerViewer::getClassTypeId())) {    
        soViewer->setViewerProxy
          (viewer,viewer_proxy<SoQtFullViewer,SoQtExaminerViewer>);
      } else {
        soViewer->setViewerProxy(viewer,viewer_proxy<SoQtFullViewer>);
      }
#endif
    }
    
    if(!qtWidget) return false;
    if(!qtContainer) qtContainer = qtWidget;

/* NOTE : since a delete widget would call immediatly the below
          which would do an ItemML.invalidate, and that it is done
          in the destroyNativeWidget, then there is no need of a
          DestroyCallback.
   {DestroyCallback* qtCbk = new DestroyCallback(*this,&aItemML,qtWidget);
    addAction(qtCbk);
    QObject::connect(qtWidget,SIGNAL(destroyed()),
                     qtCbk->cast(),SLOT(execute()));}
    if(qtContainer!=qtWidget) {
      DestroyCallback* qtCbk = new DestroyCallback(*this,&aItemML,qtContainer);
      addAction(qtCbk);
      QObject::connect(qtContainer,SIGNAL(destroyed()),
                       qtCbk->cast(),SLOT(execute()));
    }
*/

    aItemML.set_data(qtWidget,qtContainer?qtContainer:qtWidget,0);
    owidget->setProxy(qtWidget,qt_proxy);
    
    // Manage :
    //qtWidget->show();

    // Put in parent :
    if(qtParent) {

      if(qtParent->inherits(S_QTabWidget)) {
        if(!aItemML.element_value("tabLabel",svalue)) svalue = aName;
        ((QTabWidget*)qtParent)->addTab(qtContainer,svalue.c_str());
      } else if(qtParentClass=="Window") {
        if(qtClass=="ToolBar") {
        } else if(qtClass=="MenuBar") {
        } else if(qtContainer->inherits(S_OnX_QFileDialog)) {
        } else if(qtContainer->inherits(S_OnX_QColorDialog)) {
        } else if(qtContainer->inherits(S_QDialog)) {
        } else if(((QMainWindow*)qtParent)->centralWidget()) {
          std::ostream& out = fSession.cout();
          out << "QtUI::createNativeWidget :"
              << " A Window can have only one non-dialog child." 
              << std::endl;
        } else {
          ((QMainWindow*)qtParent)->setCentralWidget(qtContainer);
        }
      } else if(qtParentClass=="Dialog") {
        ((OnX_Q3VBox*)qtParent)->layout()->addWidget(qtContainer);
      } else if(qtParentClass=="HBox") {
        ((OnX_Q3HBox*)qtParent)->layout()->addWidget(qtContainer);
        std::string sexpand;
        if(!aParent->element_value("expand",sexpand)) sexpand = "second";
        QHBoxSetExpand(*((OnX_Q3HBox*)qtParent),sexpand);
      } else if(qtParentClass=="VBox") {
        ((OnX_Q3VBox*)qtParent)->layout()->addWidget(qtContainer);
        std::string sexpand;
        if(!aParent->element_value("expand",sexpand)) sexpand = "second";
        QVBoxSetExpand(*((OnX_Q3VBox*)qtParent),sexpand);
      } else if(qtParentClass=="HContainer") {
        ((OnX_Q3HBox*)qtParent)->layout()->addWidget(qtContainer);
      } else if(qtParentClass=="VContainer") {
        ((OnX_Q3VBox*)qtParent)->layout()->addWidget(qtContainer);
      }
    }

    // Set resources :
    if(aItemML.element_value("background",svalue)) {
      if(qtWidget->inherits(S_QAbstractButton)) {
        QButtonSetBackground(*((QAbstractButton*)qtWidget),svalue);
      } else if(qtWidget->inherits(S_QComboBox)) {
        QComboBoxSetBackground(*((QComboBox*)qtWidget),svalue);
      } else {
        QWidgetSetBackground(*qtWidget,svalue);
      }
    }
    if(aItemML.element_value("foreground",svalue)) {
      QWidgetSetForeground(*qtWidget,svalue);
    }
    if( (qtClass=="Window") || 
        (qtClass=="Dialog") ||
        (qtClass=="WarningDialog") ) {
      if(aItemML.element_value("title",svalue)) {
        qtWidget->topLevelWidget()->setWindowTitle(svalue.c_str());
      }
      if(aItemML.element_value("geometry",svalue)) {
        int x,y,w,h;
        if(!to_geometry(svalue,x,y,w,h)) {
          std::ostream& out = fSession.cout();
          out << "QtUI::createNativeWidget :"
              << " \"" << svalue << "\" : bad value for geometry." 
              << std::endl;
        } else {
#ifdef __APPLE__
	  if(y<=0) y += 23;  //To be under Apple menubar.
#endif
 	  qtWidget->topLevelWidget()->move(QPoint(x,y));
	  qtWidget->topLevelWidget()->resize(w,h);
	}
      }

      if( (qtClass=="Window") ){

        CloseCallback* qtCbk = new CloseCallback(*this,&aItemML);
        addAction(qtCbk);
        QObject::connect
          (qtWidget,SIGNAL(close_window()),qtCbk,SLOT(execute()));
      }

    } else if(qtClass=="CascadeButton") {
      // Callbacks :
      addCallbacks(aItemML,qtWidget,"cascading",SIGNAL(aboutToShow()));
    } else if( (qtClass=="MenuItem") || 
               (qtClass=="ToggleMenuItem") ){

      if(qtClass=="ToggleMenuItem") {
        if(aItemML.element_value("set",svalue)) {
          bool value;
          if(inlib::to(svalue,value)) {
            ((OnX_QToggleMenuItem*)qtWidget)->setChecked(value);
          }
        }
      }

      if(aItemML.element_value("accelerator",svalue)) {
        ((OnX_QMenuItem*)qtWidget)->setAccel(svalue);
      }

      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(activated()));
    } else if(qtClass=="Label") {
      if(aItemML.element_value("label",svalue) || 
         aItemML.element_value("labelString",svalue) ) {
        ((QLabel*)qtWidget)->setText(svalue.c_str());
      }
    } else if(qtClass=="PushButton") {
      if(aItemML.element_value("label",svalue)) {
        ((QPushButton*)qtWidget)->setText(svalue.c_str());
      }
      // Callbacks :
      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(clicked()));
    } else if(qtClass=="TabStack") {
      addCallbacks(aItemML,qtWidget,
                   "select",SIGNAL(currentChanged(int)));
    } else if(qtClass=="ToolBarItem") {
      if(aItemML.element_value("label",svalue)) {
        ((QToolButton*)qtWidget)->setText(svalue.c_str());
      }
      if(aItemML.element_value("tooltip",svalue)) {
        qtWidget->setToolTip(svalue.c_str());
      }
      if(aItemML.element_value("pixmap",svalue)) {
        std::string fileName;
        inlib::file_name(svalue,fileName);
        QPixmap pixmap;
        pixmap.load(fileName.c_str());
        ((QToolButton*)qtWidget)->setIcon(QIcon(pixmap));
        ((QToolButton*)qtWidget)->setIconSize(pixmap.size());
      }
      // Callbacks :
      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(clicked()));
    } else if(qtClass=="ToggleButton") {
      if(aItemML.element_value("set",svalue)) {
        bool value;
        if(inlib::to(svalue,value)) {
          ((QCheckBox*)qtWidget)->setChecked(value);
        }
      }
      if(aItemML.element_value("label",svalue)) {
        ((QCheckBox*)qtWidget)->setText(svalue.c_str());
      }
      // Callbacks :
      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(toggled(bool)));
    } else if(qtClass=="FileSelection") {
      addCallbacks(aItemML,qtWidget,"ok",SIGNAL(ok()));
    } else if(qtClass=="ColorSelection") {
      addCallbacks(aItemML,qtWidget,"ok",SIGNAL(ok()));
    } else if(qtClass=="CommandLine") {
      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(returnPressed()));
      addCallbacks(aItemML,qtWidget,"complete",SIGNAL(complete()));
    } else if(qtClass=="ProgressBar") {
      ((QProgressBar*)qtWidget)->setRange(0,100);
      ((QProgressBar*)qtWidget)->setValue(0);
    } else if(qtClass=="ScrolledText") {
      if(aItemML.element_value("value",svalue)) {
        bool as_it = false;
        std::string s;
        if(aItemML.element_atb_value("value","as_it",s)) {
          if(!inlib::to(s,as_it)) as_it = false;
        }
        if(!as_it) inlib::replace(svalue,"\\n","\n");
        //((QTextEdit*)qtWidget)->setText(svalue.c_str());
        //to have cursor at end.
        ((QTextEdit*)qtWidget)->insertPlainText(svalue.c_str());
      }
    } else if(qtClass=="ScrolledList") {
      aItemML.element_value("selection",sdef);
      if(aItemML.element_value("items",svalue)) {
        ((QListWidget*)qtWidget)->clear();
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        unsigned int itemn = items.size();
        for(unsigned int index=0;index<itemn;index++){
          ((QListWidget*)qtWidget)->addItem((char*)items[index].c_str());
        }
      }
      addCallbacks(aItemML,qtWidget,"select",SIGNAL(itemSelectionChanged()));
    } else if(qtClass=="ScrolledTree") {
      addCallbacks(aItemML,qtWidget,"select",SIGNAL(itemSelectionChanged()));
      addCallbacks(aItemML,qtWidget,"selectBranch",SIGNAL(itemSelectionChanged()));
      addCallbacks(aItemML,qtWidget,
        "open",SIGNAL(itemExpanded(QTreeWidgetItem*)),
               SLOT(execute3(QTreeWidgetItem*)));
      addCallbacks(aItemML,qtWidget,
        "close",SIGNAL(itemCollapsed(QTreeWidgetItem*)),
                SLOT(execute3(QTreeWidgetItem*)));
    } else if(qtClass=="Password") {
      std::string svalue;
      if(aItemML.element_value("value",svalue)) {
        ((QLineEdit*)qtWidget)->setText(svalue.c_str());
      }
      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(returnPressed()));
    } else if(qtClass=="Entry") {
      std::string svalue;
      if(aItemML.element_value("value",svalue)) {
        ((QLineEdit*)qtWidget)->setText(svalue.c_str());
      }
      addCallbacks(aItemML,qtWidget,"activate",SIGNAL(returnPressed()));
    } else if( (qtClass=="HScale") || (qtClass=="VScale") ||
               (qtClass=="HScrollBar") || (qtClass=="VScrollBar") ) {
      int mn = 0;
      if(aItemML.element_value("minimum",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          mn = value;
        }
      }
      int mx = 1000;
      if(aItemML.element_value("maximum",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          mx = value;
        }
      }
      int val = 0;
      if(aItemML.element_value("value",svalue)) {
        int value;
        if(inlib::to<int>(svalue,value)) {
          val = value;
        }
      }
      if( (qtClass=="HScale") || (qtClass=="VScale") ) {
        ((QSlider*)qtWidget)->setMinimum(mn);
        ((QSlider*)qtWidget)->setMaximum(mx);
        ((QSlider*)qtWidget)->setValue(val);
      } else {
        ((QScrollBar*)qtWidget)->setMinimum(mn);
        ((QScrollBar*)qtWidget)->setMaximum(mx);
        ((QScrollBar*)qtWidget)->setValue(val);
      }
      // Callbacks :
      addCallbacks(aItemML,qtWidget,"drag",SIGNAL(sliderMoved(int)));
    } else if( (qtClass=="ComboBox") || (qtClass=="OptionMenu")) {
      //if(aItemML.element_value("label",svalue)) {
        //((QComboBox*)qtWidget)->
      //}
      if(((QComboBox*)qtWidget)->isEditable()) {
        if(aItemML.element_value("items",svalue)) {
          std::vector<std::string> items;
          smanip_lines(svalue,items);
          QComboBoxSetItems(*((QComboBox*)qtWidget),items);
        }
        if(aItemML.element_value("default",svalue)) {
          QComboBoxSetValue(*((QComboBox*)qtWidget),svalue);
        }
      } else {
        aItemML.element_value("default",sdef);
        if(aItemML.element_value("items",svalue)) {
          std::vector<std::string> items;
          smanip_lines(svalue,items);
          QOptionMenuInitialize(*((QComboBox*)qtWidget),items,sdef);
        }
      }

      addCallbacks(aItemML,qtWidget,"valueChanged",SIGNAL(activated(int)));
    } else if(qtClass=="OpenGLArea") {
      addCallbacks(aItemML,qtWidget,"paint",
                   SIGNAL(paint(QCallbackData&)),
                   SLOT(execute2(QCallbackData&)));
      addCallbacks(aItemML,qtWidget,"event",
                   SIGNAL(eventHandler(QCallbackData&)),
                   SLOT(execute2(QCallbackData&)));
#ifdef ONX_HAS_INVENTOR
    } else if( (qtClass=="PageViewer") ||
               (qtClass=="PlanePageViewer") ){
      if(aItemML.element_value("popupItems",svalue)) {
        std::vector<std::string> items;
        smanip_lines(svalue,items);
        for(unsigned int index=0;index<items.size();index++)
          ((QPageViewer*)qtWidget)->addPopupEntry(items[index]);
      }

      addCallbacks(aItemML,qtWidget,"rectangularCollect",
                   SIGNAL(rectangularCollect(QCallbackData&)),
                   SLOT(execute2(QCallbackData&)));
      addCallbacks(aItemML,qtWidget,"collect",
                   SIGNAL(collect(QCallbackData&)),
                   SLOT(execute2(QCallbackData&)));
      addCallbacks(aItemML,qtWidget,"popup",
                   SIGNAL(popup(QCallbackData&)),
                   SLOT(execute2(QCallbackData&)));
      addCallbacks(aItemML,qtWidget,"update","","");
#endif
    }      

    // Constraint resources

    bool status = createChildren(aItemML);

    // Post children create resources :
    if(qtClass=="Dialog") {
      QDialogDisableEnter(*((QDialog*)qtWidget));
    }

    return status;

}

bool OnX::QtUI::destroyNativeWidget(inlib::xml::tree& aItemML) {
  // Destroy sub widgets :
  std::list<inlib::xml::tree*> list;
  aItemML.sub_trees(list);
  std::list<inlib::xml::tree*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    if(!destroyNativeWidget(*(*it))) return false;
  }
  // Destroy this widget :
  return destroyOneWidget(aItemML);
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::destroyOneWidget(
 inlib::xml::tree& aItemML                      
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::QtUI::destryOneWidget : begin\n");
#endif
  QWidget* widget = (QWidget*)aItemML.get_data1();
  if(widget) {
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "QtUI::destroyWidget :"
          << " destroy Qt widget \"" 
          << std::string(widget->objectName().toStdString())
          << "\"." << std::endl;
    }
    fWidgets.remove(widget);
    delete widget;
  }
  QWidget* widgetContainer = (QWidget*)aItemML.get_data2();
  if(widgetContainer && (widgetContainer!=widget) ) {
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "QtUI::destroyWidget :"
          << " destroy Qt widget \"" 
          << std::string(widgetContainer->objectName().toStdString()) 
          << "\"." << std::endl;
    }
    fWidgets.remove(widgetContainer);
    delete widgetContainer;
  }
#ifdef DEBUG_DESTROY
  ::printf("debug : OnX::QtUI::destryOneWidget : end\n");
#endif
  removeInRelations(aItemML);
  aItemML.invalidate();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::QtUI::getCallbackName(
 const QObject& aMeta
,const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aName == "activate") {
    if(aMeta.inherits(S_QPushButton)) {
      return SIGNAL(clicked());
    } else if(aMeta.inherits(S_QMenu)) {
      return SIGNAL(clicked());
    } else if(aMeta.inherits(S_QCheckBox)) {
      return SIGNAL(toggled(bool));
    } else if(aMeta.inherits(S_QLineEdit)) {
      return SIGNAL(returnPressed());
    } else if(aMeta.inherits(S_OnX_QMenuItem)) {
      return SIGNAL(activated());
    } else if(aMeta.inherits(S_OnX_QCommandLine)) {
      return SIGNAL(returnPressed());
    }
  } else if (aName == "valueChanged") {
    if(aMeta.inherits(S_QScrollBar)) {
      return SIGNAL(activated(int));
    } else if(aMeta.inherits(S_QComboBox)) {
      return SIGNAL(activated(int));
    }
  } else if (aName == "select") {
    if(aMeta.inherits(S_QTabWidget)) {
      return SIGNAL(currentChanged(int));
    } else if(aMeta.inherits(S_QTreeWidget)) {
      return SIGNAL(itemSelectionChanged());
    }
  } else if (aName == "ok") {
    if(aMeta.inherits(S_OnX_QFileDialog)) {
      return SIGNAL(ok());
    } else if(aMeta.inherits(S_OnX_QColorDialog)) {
      return SIGNAL(ok());
    }
  }
  // unique callback name
  else if (aName =="complete") return SIGNAL(complete());
  else if (aName =="collect" ) return SIGNAL(collect(QCallbackData&));
  else if (aName =="popup")    return SIGNAL(popup(QCallbackData&));
  else if (aName =="paint")    return SIGNAL(paint(QCallbackData&));
  else if (aName =="event")    return SIGNAL(eventHandler(QCallbackData&));
  else if (aName =="drag")     return SIGNAL(sliderMoved(int));
  else if (aName =="open")     return SIGNAL(itemExpanded(QTreeWidgetItem*));
  else if (aName =="close")    return SIGNAL(itemCollapsed(QTreeWidgetItem*));
  else if (aName =="selectBranch") return SIGNAL(itemSelectionChanged());

  // FIXME
  //  else if (aName =="cancel") return ;
  //  else if (aName =="increment") return ;
  //  else if (aName =="decrement") return ;
  
  return "";
}

//////////////////////////////////////////////////////////////////////////////
void OnX::QtUI::deleteCallback(
 inlib::xml::tree& aItemML
,const std::string& aNameXML
,OnX::Action* aAction
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QWidget* widget = (QWidget*)aItemML.get_data1();
  if(!widget) return;

  std::string qtCbkName = getCallbackName(*widget,aNameXML);
  if(qtCbkName.empty()) return;

  std::string slot;
  if(widget->inherits(S_OnX_QOpenGLArea)) {
    slot = SLOT(execute2(QCallbackData&));    
#ifdef ONX_HAS_INVENTOR
  } else if(widget->inherits(S_OnX_QPageViewer)) {
    slot = SLOT(execute2(QCallbackData&));    
#endif
  } else {
    slot = SLOT(execute());
  }

 {QObject* qobj = dynamic_cast<QObject*>(aAction);
  if(qobj) {
    QObject::disconnect(widget,qtCbkName.c_str(),qobj,slot.c_str());
  } else {
    std::ostream& out = fSession.cout();
    out << "OnX::QtUI::OnX::QtUI::deleteCallback :"
        << " Action not a QObject !"
        << std::endl;
  }}
}
//////////////////////////////////////////////////////////////////////////////
OnX::Action* OnX::QtUI::addCallback(
 inlib::xml::tree& aItemML
,const std::string& aNameXML
,const std::string& aInterp
,const std::string& aScript
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  if(!widget) return 0;

  QWidget* qtWidget = (QWidget*)aItemML.get_data1();
  if(!qtWidget) return 0;

  std::string qtCbkName = getCallbackName(*qtWidget,aNameXML);
  if(qtCbkName.empty()) return 0;

  Action::Parameters params;

  //do not use BaseUI::addAction.
  QtCallback* qtCbk = 
    new QtCallback(*this,widget,qtWidget,aInterp,aScript,params,aNameXML);
  addAction(qtCbk);

  std::string slot;
  if(qtWidget->inherits(S_OnX_QOpenGLArea)) {
    slot = SLOT(execute2(QCallbackData&));    
#ifdef ONX_HAS_INVENTOR
  } else if(qtWidget->inherits(S_OnX_QPageViewer)) {
    slot = SLOT(execute2(QCallbackData&));    
#endif
  } else {
    slot = SLOT(execute());
  }

  QObject::connect(qtWidget,qtCbkName.c_str(),qtCbk,slot.c_str());

  return qtCbk;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::QtUI::addCallbacks(
 inlib::xml::tree& aItemML
,QWidget* aWidget
,const std::string& aNameXML
,const std::string& aNameGUI
,const std::string& aSlot
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::UI::IWidget* widget = INLIB_CAST(aItemML,Slash::UI::IWidget);
  std::string slot = (aSlot==""?std::string(SLOT(execute())):aSlot);
  std::list<inlib::xml::element*> props;
  aItemML.sub_elems(props);
  std::list<inlib::xml::element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    if(aNameXML==(*it)->name()) {
      std::string sexec;
      (*it)->attribute_value("exec",sexec);
      //do not use BaseUI::addAction.
      QtCallback* qtCbk = 
        new QtCallback(*this,widget,aWidget,
                       sexec,(*it)->value(),
                       (*it)->attributes(),aNameXML);
      addAction(qtCbk);
      if(aNameGUI.size())
        QObject::connect(aWidget,aNameGUI.c_str(),qtCbk,slot.c_str());
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::QtUI::getType(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
       if(aName=="Window")         return aName;
  else if(aName=="VBox")           return aName;
  else if(aName=="HBox")           return aName;
  else if(aName=="VPaned")         return aName;
  else if(aName=="HPaned")         return aName;
  else if(aName=="MenuBar")        return aName;
  else if(aName=="Label")          return aName;
  else if(aName=="PushButton")     return aName;
  else if(aName=="ToggleButton")   return aName;
  else if(aName=="FileSelection")  return aName;
  else if(aName=="ColorSelection") return aName;
  else if(aName=="ScrolledList")   return aName;
  else if(aName=="ScrolledText")   return aName;
  else if(aName=="ScrolledTree")   return aName;
  else if(aName=="Entry")          return aName;
  else if(aName=="Password")       return aName;
  else if(aName=="CommandLine")    return aName;
  else if(aName=="Dialog")         return aName;
  else if(aName=="VContainer")     return aName;
  else if(aName=="HContainer")     return aName;
  else if(aName=="ComboBox")       return aName;
  else if(aName=="HScrollBar")     return aName;
  else if(aName=="VScrollBar")     return aName;
  else if(aName=="HScale")         return aName;
  else if(aName=="VScale")         return aName;
  else if(aName=="PageViewer")     return aName;
  else if(aName=="PlanePageViewer")return aName;
  else if(aName=="ToolBar")        return aName;
  else if(aName=="ToolBarItem")    return aName;
  else if(aName=="ProgressBar")    return aName;
  else if(aName=="TabStack")       return aName;

  else if(aName=="CascadeButton")  return aName;
  else if(aName=="Menu")           return aName;
  else if(aName=="MenuItem")       return aName;
  else if(aName=="ToggleMenuItem") return aName;
  else if(aName=="MenuSeparator")  return aName;
  else if(aName=="OptionMenu")     return aName;
  else if(aName=="OpenGLArea")     return aName;
  else if(aName=="WarningDialog")  return aName;

  // Inventor viewers :
  else if(aName=="SoExaminerViewer") return aName;
  else if(aName=="SoPlaneViewer")    return aName;
  //else if(aName=="SoFlyViewer")      return aName;
  //else if(aName=="SoWalkViewer")   return aName;

  return "";
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Slash::UI::ICyclic* OnX::QtUI::createCyclic(
 const std::string& aName
,int aDelay
,const std::string& aInterp
,const std::string& aScript
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::UI::ICyclic* cyclic = 
    new QtCyclic(fSession,aName,aDelay,aInterp,aScript);
  fCyclics.push_back(cyclic);
  return cyclic;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::QtUI::widgetName(
 inlib::xml::tree& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QWidget* widget = (QWidget*)aItemML.get_data1();
  if(!widget) return "";
  return widget->objectName().toStdString();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

QObject* find_parent(QObject* aObject,const std::string& aClass){
  QObject* object = aObject;
  while(true) {
    if(!object) return 0;
    if(object->inherits(aClass.c_str())) return object;
    object = object->parent();
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <OnX/Core/Term.h>

namespace OnX {
class TermCyclic 
:public QtCyclic
,public Term {
public: //Slash::UI::ICyclic
  virtual void execute() { cyclicExecute(fUI,fInterpreter); }
public:
  TermCyclic(Slash::UI::IUI& aUI,
             const std::string& aPrompt,
             const std::string& aInterp)
  // Passing 0 millisec will process when idling.
  :QtCyclic(aUI.session(),"onx_terminal",10,aInterp,"") //10 is millisec.
  ,Term(aPrompt),fUI(aUI){}
  virtual ~TermCyclic() {}
public:
  virtual std::string ask(const std::string& aPrompt) {
    return Term::ask(fUI,aPrompt);
  }
private:
  Slash::UI::IUI& fUI;
};
}

//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::enableTerminal(
 const std::string& aPrompt
,const std::string& aInterp
)
//////////////////////////////////////////////////////////////////////////////
//WARNING : it assumes that stdout is directed to the terminal !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fTermCyclic) return true;
  fTermCyclic = new TermCyclic(*this,aPrompt,aInterp);
  if(!fTermCyclic->isValid()) {
    delete fTermCyclic;
    fTermCyclic = 0;
    return false;
  }
  fTermCyclic->start();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::QtUI::disableTerminal(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fTermCyclic) return true;
  delete fTermCyclic;
  fTermCyclic = 0;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnX::QtUI::ask(
 const std::string& aPrompt
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fAskToTerminal && fTermCyclic) {
    return fTermCyclic->ask(aPrompt);
  } else {
    return BaseUI::ask(aPrompt);
  }
}

void OnX::QtUI::removeQWidget(QWidget* aWidget) {
  fWidgets.remove(aWidget);
}

#ifdef ONX_HAS_INVENTOR
#include "../Core/receive_scene.cxx"
#endif

bool OnX::QtUI::receiveScene(
 const std::string& aWhere
,const std::string& aPlacement
,const std::string& aFile
)
{
#ifdef ONX_HAS_INVENTOR
  return receive_scene(*this,aWhere,aPlacement,aFile);
#else
  return false;
#endif
}
/*
bool is_there_Qt_input(bool& aIs) {
  if(fEventLoopStack.empty()) return;
  QEventLoop* eventLoop = fEventLoopStack.front();
  eventLoop->processEvents();
  return true;
}
*/
