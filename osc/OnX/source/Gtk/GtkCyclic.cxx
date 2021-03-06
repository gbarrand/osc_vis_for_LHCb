// this :
#include <OnX/Gtk/GtkCyclic.h>

#include <Slash/Core/ISession.h>

extern "C" {
  static gint timerProc(gpointer);
}

//////////////////////////////////////////////////////////////////////////////
OnX::GtkCyclic::GtkCyclic(
 Slash::Core::ISession& aSession
,const std::string& aName
,int aDelay
,const std::string& aInterpreter
,const std::string& aScript
)
:BaseCyclic(aSession,aName,aDelay,aInterpreter,aScript)
,fId(0)
,fStopIt(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnX::GtkCyclic::~GtkCyclic(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  stop();
}
//////////////////////////////////////////////////////////////////////////////
void OnX::GtkCyclic::stop(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fId) {
    gtk_timeout_remove(fId);
  }
  fId = 0;
  fStopIt = true;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::GtkCyclic::start(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  stop();
  fStopIt = false;
  fId = gtk_timeout_add(fDelay,timerProc,this);
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::GtkCyclic::started(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fId?true:false;
}
//////////////////////////////////////////////////////////////////////////////
void OnX::GtkCyclic::setID(
 int aId
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fId = aId;
}
//////////////////////////////////////////////////////////////////////////////
bool OnX::GtkCyclic::stopIt(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStopIt;
}
//////////////////////////////////////////////////////////////////////////////
gint timerProc(
 gpointer aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  OnX::GtkCyclic* cyclic = (OnX::GtkCyclic*)aData;
  if(!cyclic) return false;
  cyclic->execute();
  if(cyclic->delay()<=0) return false;
  if(cyclic->stopIt()) {
    cyclic->setID(0);
  } else {
    cyclic->setID(gtk_timeout_add(cyclic->delay(),timerProc,cyclic));
  }
  return false;
}
