
// Lib :
#include <Slash/Core/ISession.h>

// OnX :
#include <OnX/Xt/XtUI.h>

extern "C" {

Slash::UI::IUI* OnX_createXtUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs){
  return new OnX::XtUI(aSession,aArgs);
}

}
