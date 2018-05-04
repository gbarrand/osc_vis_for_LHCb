#include <QtGui/qapplication.h>

#include <QtGui/qframe.h>
#include <QtGui/qsplitter.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qlayout.h>
#include <QtGui/qtoolbar.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qfiledialog.h>

//#define QT_TEST_1
//#define QT_TEST_2
#define QT_TEST_3

#if QT_VERSION < 0x040000
#include <qvbox.h>
#include <qgrid.h>
#include <qtextview.h>
#include <qlistview.h>
#include <qmultilineedit.h>
#include <qcanvas.h>
#include <qpopupmenu.h>
#else
#ifdef QT_TEST_3
#else
#include <q3vbox.h>
#include <q3grid.h>
#include <q3textview.h>
#include <q3listview.h>
#include <q3canvas.h>
#include <q3popupmenu.h>
#define QVBox Q3VBox
#define QGrid Q3Grid
#define QTextView Q3TextView
#define QListView Q3ListView
#define QCanvas Q3Canvas
#define QPopupMenu Q3PopupMenu
#define QCanvasRectangle Q3CanvasRectangle
#define QCanvasView Q3CanvasView
#endif
#endif

#ifdef QT_TEST_1
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
#endif

//////////////////////////////////////////////////////////////////////////////
int main (
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  QApplication* qapp = new QApplication(aArgc,aArgv);

#ifdef QT_TEST_3
  QMainWindow* mainWindow = new QMainWindow(0);
  //printf("debug : central widget %lu\n",mainWindow->centralWidget());
  QPushButton* button = new QPushButton(mainWindow);
  mainWindow->setCentralWidget(button);
  mainWindow->show();

  QFileDialog* fd = new QFileDialog((QWidget*)mainWindow,(Qt::WindowFlags)0);
  fd->show();
#endif

#ifdef QT_TEST_1
  QVBox* box = new QVBox(0,"main");
  qapp->setMainWidget(box);
  box->setMinimumSize(900,750);
  box->setCaption("main");

  /*
  QSplitter* box3 = new QSplitter(Qt::Horizontal,box);
  //printf("debug :%s %s\n",box3->className(),((QWidget*)box3)->className());

  QListView* button1 = new QListView(box3);
  button1->setResizeMode(QListView::AllColumns);
  //QPushButton* button1 = new QPushButton("Hello 1",box3);
  //QObject::connect(button1,SIGNAL(clicked()),qapp,SLOT(quit()));
  button1->show();

  QPushButton* button4 = new QPushButton("Hello 4",box3);
  QObject::connect(button4,SIGNAL(clicked()),qapp,SLOT(quit()));
  button4->show();

  QSplitter* box2 = new QSplitter(Qt::Vertical,box);

  QTextView* button2 = new QTextView(box2);
  //QPushButton* button2 = new QPushButton("Hello 2",box2);
  //QObject::connect(button2,SIGNAL(clicked()),qapp,SLOT(quit()));
  button2->show();

  //QPushButton* button3 = new QPushButton("Hello 3",box2);
  QMultiLineEdit* button3 = new QMultiLineEdit(box2);
  //QObject::connect(button3,SIGNAL(clicked()),qapp,SLOT(quit()));
  button3->show();
  */

  // Reproduce the "QMenuBar in QVBox" bad height problem

  QVBox* box0 = new QVBox(box,"box0");

  /// Grid begin
  //QGrid* box1 = new QGrid(2,Qt::Vertical,box0,"grid");
  QVBox* box1 = new QVBox(box0,"grid");
  box1->setPaletteBackgroundColor(QColor("red"));

  QMenuBar* menuBar = new MyMenuBar(box1,"menuBar");
  //QMenuBar* menuBar = new QMenuBar(box1,"menuBar");

 {QPopupMenu* popup = new QPopupMenu(menuBar,"popup");
  popup->setCheckable(TRUE);
  popup->insertItem("xxxxxxxx");
  menuBar->insertItem("xxxxxxxxx",popup);}
 {QPopupMenu* popup = new QPopupMenu(menuBar,"popup");
  popup->setCheckable(TRUE);
  popup->insertItem("xxxxxxxx");
  menuBar->insertItem("xxxxxxxxx",popup);}
 {QPopupMenu* popup = new QPopupMenu(menuBar,"popup");
  popup->setCheckable(TRUE);
  popup->insertItem("xxxxxxxx");
  menuBar->insertItem("xxxxxxxxx",popup);}
 {QPopupMenu* popup = new QPopupMenu(menuBar,"popup");
  popup->setCheckable(TRUE);
  popup->insertItem("xxxxxxxx");
  menuBar->insertItem("xxxxxxxxx",popup);}
 {QPopupMenu* popup = new QPopupMenu(menuBar,"popup");
  popup->setCheckable(TRUE);
  popup->insertItem("xxxxxxxx");
  menuBar->insertItem("xxxxxxxxx",popup);}

  QPushButton* button = new QPushButton(box1,"button");
  button->setText("button");

  /// Grid end
  QVBox* box2 = new QVBox(box0,"box2");
  QPushButton* b = new QPushButton(box2,"b");
  b->setText("b");
  QSizePolicy sp = b->sizePolicy();
  sp.setVerData(QSizePolicy::Expanding);
  b->setSizePolicy(sp);
  box->show();
#endif

#ifdef QT_TEST_2
  QVBox* box = new QVBox(0,"main");
  qapp->setMainWidget(box);
  box->setMinimumSize(900,750);
  box->setCaption("main");

  QCanvas* canvas = new QCanvas(500,500);
  //QCanvasEllipse* canvasEllipse = new QCanvasEllipse(canvas);
  QCanvasRectangle* canvasEllipse = new QCanvasRectangle(canvas);
  canvasEllipse->setSize(200,200);
  canvasEllipse->move(200,200);
  canvasEllipse->show();

  QCanvasRectangle* canvasRectangle = new QCanvasRectangle(canvas);
  canvasRectangle->setSize(200,200);
  canvasRectangle->show();

  QCanvasView* canvasView = new QCanvasView(box,"canvas");
  canvasView->setCanvas(canvas);

  box->show();
#endif

  //qapp->setMainWidget(box);
  qapp->exec();

  delete qapp;
  return 0;
}
