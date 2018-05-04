#include <OnX/Qt/QtUI.h>

extern "C" {

Slash::UI::IUI* OnX_createQtUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs){
  return new OnX::QtUI(aSession,aArgs);
}

}
