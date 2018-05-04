#ifndef OnX_QtThread_h
#define OnX_QtThread_h

#include <QtCore/qthread.h>

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x050000
#include <QtGui/qapplication.h>
#else
#include <QtWidgets/qapplication.h>
#endif

#include <iostream> //FIXME

/** A derived class of QThread that runs the QApplication object. */

namespace OnX {

class QtThread : public QThread {
  //FIXME Q_OBJECT
public: //QThread
  /** Creates the application object and runs it. */
  virtual void run() {
    if (!qApp) {    
      static int argc = 1;
      static char* argv[1] = {(char*)"OnX"};
      new QApplication (argc, argv); //FIXME : delete.
    }
    // Create the GUI ?
    try {
      qApp->exec ();
    }
    catch ( std::exception & e ) {
      std::cout << e.what()
	        << std::endl;
    }
  }
public:
  QtThread(){}
  virtual ~QtThread(){}
};

}

#endif
