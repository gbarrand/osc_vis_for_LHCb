#include <Slash/Core/ISession.h>

#include <BatchLab/XML/Manager.h>

extern "C" {

void BatchLabXMLInitialize(Slash::Core::ISession& aSession) {
  if(!aSession.findManager("XML_Manager")) {
    aSession.addManager(new BatchLab::XML::Manager(aSession));
  }
}
void BatchLabXMLFinalize(Slash::Core::ISession& aSession) {
  aSession.destroyManager("XML_Manager");
}

}
