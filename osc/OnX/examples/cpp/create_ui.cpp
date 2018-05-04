
#include <Slash/Core/ISession.h>

#include <Slash/Tools/UI.h>
#include <Slash/Tools/Managers.h>

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoDetectorExample.h>

static void create_ui(Slash::Core::ISession& a_session,std::ostream& a_out,const std::string& a_viewer,bool a_ui) {

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(a_session);
  if(!sh) {
    a_out << "create_ui :"
          << " scene handler not found." 
          << std::endl;
  } else {

    SoDetectorExample* det = new SoDetectorExample();
    det->ref();

    sh->setViewer(a_viewer);
    sh->addScene("staticScene",det);

    det->unref();

  }

  if(a_ui) {
    Slash::UI::IUI* ui = Slash::find_UI(a_session);     
    if(!ui) {
      a_out << "create_ui :"
            << "Slash::UI::IUI not found."
            << std::endl;
      return;
    }
    ui->steer();
  }

}

#include <Slash/Core/ISession.h>
#include <inlib/osc_env> //For TTFPATH

#include <iostream>

int main(int argc,char* argv[]) {

  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "create_ui :"
              << " unable to set OSC environment." << std::endl;
    return 1;
  }

  Slash::Core::ISession* session = 
    Slash_create_session(inlib::to(argc,argv));
  if(!session) {
    std::cout << "create_ui :"
              << " session not found." << std::endl;
    return 1;
  }

  Slash::UI::IUI_Manager* uim = Slash::uiManager(*session);
  if(!uim) {
    std::cout << "create_ui :"
              << " UI_Manager not found." << std::endl;
    return 1;
  }

  std::vector<std::string> args;
  uim->create("","detector.onx",args,true);

  create_ui(*session,std::cout,"@current@",true);
  delete session;

  return 0;
}
