
#include <Slash/Core/ISession.h>

#include <Slash/Tools/UI.h>
#include <Slash/Tools/Managers.h>

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoDetectorExample.h>

static void detector_visualize(Slash::Core::ISession& a_session,std::ostream& a_out,const std::string& a_viewer,bool a_ui) {

  Slash::UI::ISceneHandler* sh = Slash::sceneHandler(a_session);
  if(!sh) {
    a_out << "detector_visualize :"
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
      a_out << "detector_visualize :"
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

int main(int,char*[]) {

  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "create_ui :"
              << " unable to set OSC environment." << std::endl;
    return 1;
  }

  int argc = 1;
  static char* argv[] = {(char*)"-onx_file=detector.onx"};  

  Slash::Core::ISession* session = Slash_create_session(inlib::to(argc,argv));
  if(!session) {
    std::cout << "detector_visualize :"
              << " session not found." << std::endl;
    return 1;
  }

  detector_visualize(*session,std::cout,"@current@",true);
  delete session;

  return 0;
}
