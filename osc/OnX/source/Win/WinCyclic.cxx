// this :
#include <OnX/Win/WinCyclic.h>

#include <Slash/Core/ISession.h>

#include <map>

static std::map<UINT_PTR,Slash::UI::ICyclic*>* sTimers = 0;

//////////////////////////////////////////////////////////////////////////////
OnX::WinCyclic::WinCyclic(
 Slash::Core::ISession& aSession
,const std::string& aName
,int aDelay
,const std::string& aInterpreter
,const std::string& aScript
)
:BaseCyclic(aSession,aName,aDelay,aInterpreter,aScript)
,fId(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnX::WinCyclic::~WinCyclic(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  stop();
}
//////////////////////////////////////////////////////////////////////////////
void OnX::WinCyclic::stop(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fId) {
    ::KillTimer(NULL,fId);
    if(sTimers) sTimers->erase(fId);
  }
  fId = 0;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::WinCyclic::start(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  stop();
  if(fDelay<=0) return;
  fId = ::SetTimer(NULL,0,
                   (UINT)fDelay,
                   (TIMERPROC)timerProc);
  if(!sTimers) sTimers = new std::map<UINT_PTR,Slash::UI::ICyclic*>;
  sTimers->insert(std::pair<UINT_PTR,Slash::UI::ICyclic*>(fId,this));
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::WinCyclic::started(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fId?true:false;
}
//////////////////////////////////////////////////////////////////////////////
void CALLBACK OnX::WinCyclic::timerProc(
 HWND
,UINT
,UINT_PTR aId
,DWORD               
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!sTimers) return;
  std::map<UINT_PTR,Slash::UI::ICyclic*>::iterator it = sTimers->find(aId);
  if(it==sTimers->end()) return;
  Slash::UI::ICyclic* cyclic = (*it).second;
  if(!cyclic) return;
  cyclic->execute();
}
