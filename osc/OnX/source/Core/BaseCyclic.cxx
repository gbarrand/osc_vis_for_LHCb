// this :
#include <OnX/Core/BaseCyclic.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/Managers.h>

#include <Lib/Debug.h>

OnX::BaseCyclic::BaseCyclic(
 Slash::Core::ISession& aSession
,const std::string& aName
,int aDelay
,const std::string& aInterpreter
,const std::string& aScript
)
:fSession(aSession)
,fName(aName)
,fDelay(aDelay>=0?aDelay:0)
,fCallback(0)
,fInterpreter(aInterpreter)
,fScript(aScript)
,fScriptManager(0)
,fCounter(0)
{
  Lib::Debug::increment("OnX::BaseCyclic");
  fScriptManager = Slash::scriptManager(fSession);
}
OnX::BaseCyclic::~BaseCyclic(){
  delete fCallback;
  Lib::Debug::decrement("OnX::BaseCyclic");
}

std::string OnX::BaseCyclic::name() const {return fName;}

void OnX::BaseCyclic::setDelay(int aDelay){
  if(aDelay<=0) return;
  fDelay = aDelay;
}

int OnX::BaseCyclic::iterations() const {return fCounter;}
int OnX::BaseCyclic::delay() const {return fDelay;}

void OnX::BaseCyclic::setCallback(Slash::UI::ICyclicCallback* aCallback) {
  fCallback = aCallback;
}

void OnX::BaseCyclic::setScript(const std::string& aInterpreter,const std::string& aScript) {
  fInterpreter = aInterpreter;
  fScript = aScript;
}

std::string OnX::BaseCyclic::interpreter() const {return fInterpreter;}
std::string OnX::BaseCyclic::script() const {return fScript;}

void OnX::BaseCyclic::execute(){
  if(fCallback) fCallback->execute();
  if(fScriptManager && fScript.size()) {
    Slash::UI::IInterpreter::Options params;
    fScriptManager->executeScript(fInterpreter,fScript,params);
  }
  fCounter++;
}
