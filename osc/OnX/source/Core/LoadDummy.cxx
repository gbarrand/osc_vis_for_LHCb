#include <OnX/Core/DummyUI.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
Slash::UI::IUI* OnX_createDummyUI(
 Slash::Core::ISession& aSession
,const std::string& aFile
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return new OnX::DummyUI(aSession,aFile,aArgs);
}

}
