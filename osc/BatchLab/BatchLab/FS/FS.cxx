#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <inlib/cast>

#include <BatchLab/FS/Manager.h>

extern "C" {
 void BatchLabFSInitialize(Slash::Core::ISession&);
 void BatchLabFSFinalize(Slash::Core::ISession&);
}

void BatchLabFSInitialize(Slash::Core::ISession& aSession) {
  if(!aSession.findManager("FS_Manager")) {
    aSession.addManager(new BatchLab::FS::Manager(aSession));
  }
}

void BatchLabFSFinalize(Slash::Core::ISession& aSession) {
  aSession.destroyManager("FS_Manager");
}
