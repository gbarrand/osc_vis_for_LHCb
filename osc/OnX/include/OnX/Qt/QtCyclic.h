#ifndef OnX_QtCyclic_h
#define OnX_QtCyclic_h 

#include <OnX/Core/BaseCyclic.h>

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x050000
#include <QtGui/qwidget.h>
#else
#include <QtWidgets/qwidget.h>
#endif

#include <Slash/Core/ISession.h>

namespace OnX {

class QtCyclic : public BaseCyclic {
public: //Slash::UI::ICyclic
  virtual void stop() {
    delete fTimer;
    fTimer = 0;
  }

  virtual void start() {
    stop();
    fTimer = new Timer(*this);
    fTimer->startTimer(fDelay);
  }

  virtual bool started() const {return fTimer?true:false;}
public:
  QtCyclic(Slash::Core::ISession& aSession,
           const std::string& aName,int aDelay,
           const std::string& aInterpreter,const std::string& aScript)
  :BaseCyclic(aSession,aName,aDelay,aInterpreter,aScript)
  ,fTimer(0)
  {}
  virtual ~QtCyclic() {stop();}

private:
  class Timer : public QObject {
  public:
    Timer(QtCyclic& aCyclic):fCyclic(aCyclic){}
    virtual ~Timer(){}
  protected:
    void timerEvent(QTimerEvent*) {fCyclic.execute();}
  private:
    QtCyclic& fCyclic;
  };

private:
  Timer* fTimer;
};

}

#endif

