// this :
#include <OnX/Inventor/Inventor.h>

// Slash :
#include <Slash/Core/ISession.h>

// Inventor :
#include <Inventor/errors/SoDebugError.h>

//NOTE : we could get rid of the below statics,
//       but the SoDebugError::setHandlerCallback
//       is anyway static.

static SoErrorCB* sOld_HandlerCallback = 0;
static void* sOld_HandlerData = 0;

static void SoErrorCB_function(const SoError* aError,void* aData){
  Slash::Core::ISession* session = (Slash::Core::ISession*)aData;
  std::ostream& out = session->out();
  out << aError->getDebugString().getString() << std::endl;
}

void OnX::Inventor::initialize(Slash::Core::ISession& aSession){
  sOld_HandlerCallback = SoDebugError::getHandlerCallback();
  sOld_HandlerData = SoDebugError::getHandlerData();

  SoDebugError::setHandlerCallback(SoErrorCB_function,&(aSession));
}

void OnX::Inventor::finalize(Slash::Core::ISession&){
  if(sOld_HandlerCallback) {
    SoDebugError::setHandlerCallback(sOld_HandlerCallback,sOld_HandlerData);
  }
}

