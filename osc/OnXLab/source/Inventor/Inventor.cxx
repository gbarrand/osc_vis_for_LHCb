#include <Slash/Core/ISession.h>

#include <OnXLab/Inventor/PlotterManager.h>

extern "C" {

void OnXLabInventorInitialize(Slash::Core::ISession& aSession) {
  if(!aSession.findManager("OnXLab_PlotterManager")) {
    aSession.addManager(new OnXLab::PlotterManager(aSession));
  }
}

void OnXLabInventorFinalize(Slash::Core::ISession& aSession) {
  aSession.destroyManager("OnXLab_InventorManager");
}

}
