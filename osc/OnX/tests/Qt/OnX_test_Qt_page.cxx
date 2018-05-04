#include <QtGui/qapplication.h>

#include <QtGui/qmenubar.h>

#include <Inventor/Qt/SoQt.h>
#include <OnX/Inventor/HEPVis.h>
#include <OnX/Qt/QPageViewer.h>

#if QT_VERSION < 0x040000
#include <qvbox.h>
#else
#include <q3vbox.h>
#define QVBox Q3VBox
#endif

class MyMenuBar : public QMenuBar {
public:
  MyMenuBar(QWidget* aParent,const char* aName)
  :QMenuBar(aParent,aName){
  }
  /*
  virtual QSize        sizeHint() const {
    //QSize sh = QMenuBar::sizeHint();
    QSize sh;
    printf("debug : QMenuBar::sizeHint ++ %d %d\n",sh.width(),sh.height());
    sh.setHeight(27);
    return sh.expandedTo( QApplication::globalStrut() );
  }
  */
  virtual int heightForWidth(int max_width) const {
    int h = QMenuBar::heightForWidth(max_width);
    printf("debug : heightForWidth : nchild %d : w %d : reth %d\n",
           count(),max_width,h);
    h = QMenuBar::heightForWidth(count() * 10000);
    return h;
  }
};

//////////////////////////////////////////////////////////////////////////////
int main (
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QApplication* qapp = new QApplication(aArgc,aArgv);

  QVBox* box = new QVBox(0,"main");
  qapp->setMainWidget(box);
  box->setMinimumSize(900,750);
  box->setCaption("main");

  SoQt::init(box);
  OnX::HEPVis::initClasses();

  //OnX::QPageViewer* pageViewer = 
  new OnX::QPageViewer(box,"");

  box->show();

  //qapp->setMainWidget(box);
  qapp->exec();

  delete qapp;
  return 0;
}
