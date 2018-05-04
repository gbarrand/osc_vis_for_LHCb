
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/Tools/UI.h>

#include <inlib/srep>
#include <inlib/args>
#include <inlib/app>
#include <inlib/osc_env>

#include <OnX/Version.h>

// Embedded code :
#include "osc_gui_viewer_help.h"

#include <iostream>
#include <cstdlib>

int main(int argc,char** argv){

  inlib::args args(argc,argv);

  /////////////////////////////////
  // some non-interactive options :
  /////////////////////////////////
  if(args.is_arg("-version")) {
    std::cout << ONX_VERSION << std::endl;
    return EXIT_SUCCESS;
  }

  if(args.is_arg("-h")) {
    std::vector<std::string> text;
    text.resize(osc_gui_viewer_help_linen);
    for(unsigned int index=0;index<osc_gui_viewer_help_linen;index++) {
      std::string s = osc_gui_viewer_help_lines[index];
      inlib::replace(s,"@@double_quote@@","\"");
      inlib::replace(s,"@@back_slash@@","\\");
      text[index] = s;
    }
    for(unsigned int index=0;index<text.size();index++) {
      std::cout << text[index] << std::endl;
    }
    return EXIT_SUCCESS;
  }

  if(argc>0) {
    std::string arg0(argv[0]);
    std::vector<std::string> home_dirs;
    home_dirs.push_back("OpenScientist");
    home_dirs.push_back("OnX");
    home_dirs.push_back("session");
    if(!inlib::if_mouse_startup(arg0,"OSC_HOME_DIR",home_dirs,std::cout)) {
      std::cout << "OnX_onx :"
                << " unable to set mouse startup." << std::endl;
      return EXIT_FAILURE;
    }
    if(inlib::is_mac_app(argv[0])) {
      std::string file;
      if(!inlib::getenv("OSC_GUI_FILE",file)) {
        // started with a .app but no .onx file given.
        inlib::putenv("OSC_GUI_FILE",
          "$OSC_HOME_DIR/Resources/OnX/scripts/OnX/osc_gui_viewer_app.onx");
      }
    }
  }
  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "OnX_onx :"
              << " unable to set OSC environment." << std::endl;
    return EXIT_FAILURE;
  }

  Slash::Core::ISession* session = Slash_create_session(args.tovector());
  if(!session) {
    std::cout << "OnX_onx :"
              << " session not found." << std::endl;
    return EXIT_FAILURE;
  }

  Slash::UI::IUI* ui = Slash::find_UI(*session);
  if(!ui) {
    std::cout << "OnX_onx :"
              << " UI not found." << std::endl;
    delete session;
    return EXIT_FAILURE;
  }

  ui->steer();

  delete session;

  // Some libraries (for exa Coin, gtk) may have declared 
  // C functions with ::atexit. To ease debugging we call
  // explicitly ::exit here.
  ::exit(EXIT_SUCCESS);

  return EXIT_SUCCESS;
}
