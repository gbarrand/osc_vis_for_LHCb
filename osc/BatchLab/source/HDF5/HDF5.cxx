#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <inlib/cast>

#include <BatchLab/HDF5/Manager.h>

extern "C" {
void BatchLabHDF5Initialize(Slash::Core::ISession&);
void BatchLabHDF5Finalize(Slash::Core::ISession&);
}

//////////////////////////////////////////////////////////////////////////////
void BatchLabHDF5Initialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aSession.findManager("HDF5_Manager")) {
    aSession.addManager(new BatchLab::HDF5::Manager(aSession));
  }
}
//////////////////////////////////////////////////////////////////////////////
void BatchLabHDF5Finalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aSession.destroyManager("HDF5_Manager");
}
