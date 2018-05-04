
// OnX :
#include <OnX/Net/NetUI.h>

extern "C" {

Slash::UI::IUI* OnX_createNetUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs){
  return new OnX::NetUI(aSession,aArgs);
}

}
