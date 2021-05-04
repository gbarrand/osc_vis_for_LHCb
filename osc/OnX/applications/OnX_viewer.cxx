
#include <Slash/Core/ISession.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>

#include <inlib/srep>
#include <inlib/args>
#include <inlib/osc_env>
#include <inlib/app>

#include "iv_viewer_onx.h"

#include <iostream>

int main(int argc,char* argv[]) {

  if(argc>0) {
    std::string arg0(argv[0]);
    //for osc-viewer.app :
    std::vector<std::string> home_dirs;
    home_dirs.push_back("OpenScientist");
    home_dirs.push_back("OnX");
    home_dirs.push_back("session");
    if(!inlib::if_mouse_startup(arg0,"OSC_HOME_DIR",home_dirs,std::cout)) {
      std::cout << "OnX_viewer :" << " unable to set mouse startup." << std::endl;
      return EXIT_FAILURE;
    }
  }

  inlib::args args(argc,argv);

  args.add("-onx_file","@empty@");

  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "OnX_viewer :" << " unable to set OSC environment." << std::endl;
    return EXIT_FAILURE;
  }
 
  std::vector<std::string> vargs = args.tovector();
  Slash::Core::ISession* session = Slash_create_session(vargs);
  if(!session) {
    std::cout << "OnX_viewer :" << " session not found." << std::endl;
    return 1;
  }

  Slash::UI::IUI* ui = Slash::find_UI(*session);     
  if(!ui) {
    std::cout << "OnX_viewer :" << "Slash::UI::IUI not found." << std::endl;
    return 1;
  }

  std::string s = 
    inlib::tos(iv_viewer_onx_linen,iv_viewer_onx_lines);
  inlib::replace(s,"@@double_quote@@","\"");
  inlib::replace(s,"@@back_slash@@","\\");

  ui->load("",s,false);

  ui->steer();

  delete session;

  return 0;
}
