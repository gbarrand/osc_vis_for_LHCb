// this :
#include <OnX/Xt/XtCyclic.h>

#include <Slash/Core/ISession.h>

OnX::XtCyclic::XtCyclic(
 Slash::Core::ISession& aSession
,const std::string& aName
,int aDelay
,const std::string& aInterpreter
,const std::string& aScript
,XtAppContext aXtAppContext
)
:BaseCyclic(aSession,aName,aDelay,aInterpreter,aScript)
,fXtAppContext(aXtAppContext)
,fIntervalId(0L)
,fStopIt(false)
{}

OnX::XtCyclic::~XtCyclic(){stop();}

void OnX::XtCyclic::stop(){
  if(fIntervalId!=0L) XtRemoveTimeOut(fIntervalId);
  fIntervalId = 0L;
  fStopIt = true;
}

void OnX::XtCyclic::start(){
  stop();
  fStopIt = false;
  startTimer(this,0);
}

bool OnX::XtCyclic::started() const {return fIntervalId?true:false;}

void OnX::XtCyclic::startTimer(XtCyclic* aCyclic,XtIntervalId*){
  if(!aCyclic) return;
  aCyclic->execute();

  if(aCyclic->fDelay<=0) return;
  if(!aCyclic->fXtAppContext) return;
  if(aCyclic->fStopIt) {
    aCyclic->fIntervalId = 0L;
    return;
  }

  // Have to check if XtCyclic is still valid !
  // The XtCyclic could have been deleted by some script.
  aCyclic->fIntervalId = XtAppAddTimeOut(aCyclic->fXtAppContext,
                                (unsigned long)aCyclic->fDelay,
                                (XtTimerCallbackProc)XtCyclic::startTimer,
                                (XtPointer)aCyclic);
}
