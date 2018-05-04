#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

#include <inlib/cast>

#include <BatchLab/Zip/Manager.h>

extern "C" {
 void BatchLabZipInitialize(Slash::Core::ISession&);
 void BatchLabZipFinalize(Slash::Core::ISession&);
}

void BatchLabZipInitialize(Slash::Core::ISession& aSession) {
  if(!aSession.findManager("Zip_Manager")) {
    aSession.addManager(new BatchLab::Zip::Manager(aSession));
  }
}

void BatchLabZipFinalize(Slash::Core::ISession& aSession) {
  aSession.destroyManager("Zip_Manager");
}
