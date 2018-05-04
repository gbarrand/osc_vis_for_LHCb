// CINT :
#include <G__ci.h>

#include <CintWrap.ic>

extern "C" {
AIDA::IAnalysisFactory* Slash::Core::IManager_to_IAnalysisFactory(Slash::Core::IManager* aManager) {
  if(!aManager) return 0;
  return dynamic_cast<AIDA::IAnalysisFactory*>(aManager);
}
AIDA::ITree* Slash::Core::IManager_to_ITree(Slash::Core::IManager* aManager) {
  if(!aManager) return 0;
  return dynamic_cast<AIDA::ITree*>(aManager);
}
AIDA::IHistogram1D* IManagedObject_to_IHistogram1D(
 AIDA::IManagedObject* aObject
) {
  if(!aObject) return 0;
  return dynamic_cast<AIDA::IHistogram1D*>(aObject);
}
AIDA::IHistogram2D* IManagedObject_to_IHistogram2D(
 AIDA::IManagedObject* aObject
) {
  if(!aObject) return 0;
  return dynamic_cast<AIDA::IHistogram2D*>(aObject);
}
}
