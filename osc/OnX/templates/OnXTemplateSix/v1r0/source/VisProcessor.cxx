// this :
#include <OnXTemplateSix/VisProcessor.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/osc_env.h>

// this :
#include <OnXTemplateSix/FrameworkManager.h>

#include <iostream>

namespace OnXTemplateSix {

VisProcessor::VisProcessor(User::Framework& aFramework,const std::string& aGUI)
:User::Processor(aFramework)
,fSession(0)
,fUI(0)
{
  Slash::osc::set_env(std::cout);

  fSession = Slash_create_session(std::vector<std::string>());
  if(!fSession) {
    std::cout << "OnXTemplateSix::VisProcessor::VisProcessor :"
              << " Slash_create_session failed."
              << std::endl;
    return;
  }

  // Declare our Manager to the OnX session.
  // It must be done prior the creation of the UI in order 
  // that the OnXTemplateSix_initialize callbacks execute properly).
  FrameworkManager* frameworkManager = new FrameworkManager(&aFramework,false);
  fSession->addManager(frameworkManager);

  Slash::UI::IUI_Manager* uim = Slash::uiManager(*fSession);
  if(!uim) {
    std::cout << "OnXTemplateSix::VisProcessor::VisProcessor :"
              << " UI_Manager not found." << std::endl;
    return;
  }

  fUI = uim->create("",aGUI,std::vector<std::string>(),true);
  if(!fUI) {
    std::cout << "OnXTemplateSix::VisProcessor::VisProcessor :"
              << " UI not created."
              << std::endl;
  }

}

VisProcessor::~VisProcessor() {delete fSession;}

//private:
VisProcessor::VisProcessor(const VisProcessor& aFrom)
:User::Processor(aFrom)
,fSession(0)
,fUI(0)
{}

//private:
VisProcessor& VisProcessor::operator=(const VisProcessor&) {return *this;}

Slash::Core::ISession* VisProcessor::session(){return fSession;}

bool VisProcessor::isValid(){
  if(!fUI) return false;
  return fUI->isValid();
}

void VisProcessor::processEvent(const User::Event&) {
  // Someone get out of the below steer() by doing a UI.exit()
  // from some "Next event" button in the GUI.
  if(!fUI) return;
  fUI->executeScript("DLD","OnX viewer_clear Viewer dynamicScene");
  fUI->executeScript("DLD","OnXTemplateSix OnXTemplateSix_vis_tracks");
  fUI->steer();
}

}
