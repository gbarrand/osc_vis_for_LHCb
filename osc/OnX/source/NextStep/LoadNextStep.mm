#import <OnX/NextStep/NS_UI.h>

extern "C" {

Slash::UI::IUI* OnX_createNextStepUI(Slash::Core::ISession& aSession,const std::vector<std::string>& aArgs){
  return new OnX::NS_UI(aSession,aArgs);
}

}
