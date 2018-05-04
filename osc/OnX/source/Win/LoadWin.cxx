#include <OnX/Win/WinUI.h>

extern "C" {
Slash::UI::IUI* OnX_createWinUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs){
  return new OnX::WinUI(aSession,aArgs);
}
}
