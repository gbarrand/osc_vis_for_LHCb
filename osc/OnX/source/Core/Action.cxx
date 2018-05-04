// this :
#include <OnX/Core/Action.h>

#include <Lib/Debug.h>

#include <Slash/UI/IUI.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

OnX::Action::Action(Slash::UI::IUI& aUI)
:fUI(aUI)
,fWidget(0)
,fNativeWidget(0)
,fID(0)
{
  Lib::Debug::increment("OnX::Action");
}

OnX::Action::Action(
 Slash::UI::IUI& aUI
,Slash::UI::IWidget* aWidget
,NativeWidget aComponent
,const std::string& aInterpreter
,const std::string& aScript
,const Parameters& aParams
,const std::string& aName
,int aID
)
:fUI(aUI)
,fWidget(aWidget)
,fNativeWidget(aComponent)
,fName(aName)
,fID(aID)
,fInterpreter(aInterpreter)
,fScript(aScript)
,fParameters(aParams)
{
  Lib::Debug::increment("OnX::Action");
}

OnX::Action::Action(
 const Action& aAction
)
:fUI(aAction.fUI)
,fWidget(aAction.fWidget)
,fNativeWidget(aAction.fNativeWidget)
,fName(aAction.fName)
,fID(aAction.fID)
,fInterpreter(aAction.fInterpreter)
,fScript(aAction.fScript)
,fParameters(aAction.fParameters)
,fCallbackData(aAction.fCallbackData)
{
  Lib::Debug::increment("OnX::Action");
}
OnX::Action::~Action() {
  Lib::Debug::decrement("OnX::Action");
}

OnX::Action& OnX::Action::operator=(const Action& aAction) {
  //IRef : fUI = aAction.fUI;
  fWidget = aAction.fWidget;
  fNativeWidget = aAction.fNativeWidget;
  fName = aAction.fName;
  fID = aAction.fID;
  fInterpreter = aAction.fInterpreter;
  fScript = aAction.fScript;
  fParameters = aAction.fParameters;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
bool OnX::Action::equal(const Action& aAction) const {
  if (fName != aAction.fName) return false;
  if (fWidget != aAction.fWidget) return false;
  if (fNativeWidget != aAction.fNativeWidget) return false;
  if (fID != aAction.fID) return false;
  if (fInterpreter != aAction.fInterpreter) return false;
  if (fScript != aAction.fScript) return false;
  if (fParameters != aAction.fParameters) return false;
  return true;
}
*/
bool OnX::Action::execute() {
  return fUI.executeScript(fInterpreter,fScript,fParameters);
}

#define S_BACK "background"

bool OnX::Action::isBackground() const{
  for(Parameters::const_iterator it = fParameters.begin();
      it!=fParameters.end();++it) {
    if((*it).first==S_BACK) return true;
  }
  return false;
}

void OnX::Action::setBackground(bool aValue) {
  if(aValue) { 
    if(isBackground()) return; //Done.
    fParameters.push_back(Parameter(S_BACK,"true"));
  } else {
    if(!isBackground()) return; //Done.
    // brute force.
    Parameters params;
    for(Parameters::const_iterator it = fParameters.begin();
        it!=fParameters.end();++it) {
      if((*it).first!=S_BACK) params.push_back(*it);
    }
    fParameters = params;
  }
}

Slash::UI::IWidget* OnX::Action::widget() const { return fWidget;}
OnX::Action::NativeWidget OnX::Action::nativeWidget() const { 
 return fNativeWidget;
}
void OnX::Action::setID(int aID) {fID = aID;}
std::string OnX::Action::name() const { return fName;}

int OnX::Action::getID() const {return fID;}
Slash::UI::IUI& OnX::Action::ui() const {return fUI;}
void OnX::Action::setScript(const std::string& aScript){fScript = aScript;}
void OnX::Action::setInterpreter(const std::string& aInterpreter){ 
 fInterpreter = aInterpreter;
}

void OnX::Action::setCallbackData(const OnX::CallbackData& aData) {
  fCallbackData = aData;
}
OnX::CallbackData OnX::Action::callbackData() const {return fCallbackData;}
std::string OnX::Action::script() const {return fScript;}
std::string OnX::Action::interpreter() const {return fInterpreter;}
OnX::Action::Parameters OnX::Action::parameters() const {return fParameters;}
