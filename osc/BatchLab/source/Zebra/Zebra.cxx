#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <inlib/cast>

#include <BatchLab/Zebra/Manager.h>

extern "C" {
 void BatchLabZebraInitialize(Slash::Core::ISession&);
 void BatchLabZebraFinalize(Slash::Core::ISession&);
}

//////////////////////////////////////////////////////////////////////////////
void BatchLabZebraInitialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aSession.findManager("Zebra_Manager")) {
    aSession.addManager(new BatchLab::Zebra::Manager(aSession));
  }
}
//////////////////////////////////////////////////////////////////////////////
void BatchLabZebraFinalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aSession.destroyManager("Zebra_Manager");
}
