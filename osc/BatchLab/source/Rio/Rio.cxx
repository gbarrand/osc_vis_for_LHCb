#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <inlib/cast>

#include <BatchLab/Rio/Manager.h>

extern "C" {
  void BatchLabRioInitialize(Slash::Core::ISession&);
  void BatchLabRioFinalize(Slash::Core::ISession&);
}

//////////////////////////////////////////////////////////////////////////////
void BatchLabRioInitialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aSession.findManager("Rio_Manager")) {
    aSession.addManager(new BatchLab::Rio::Manager(aSession));
  }
}
//////////////////////////////////////////////////////////////////////////////
void BatchLabRioFinalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aSession.destroyManager("Rio_Manager");
}
