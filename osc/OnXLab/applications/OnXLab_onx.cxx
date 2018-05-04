
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>

#include <OnXLab/Version.h>

#include "env.h"

// Embedded code :
#include "osc_plot_help.h"

#include <inlib/srep>
#include <inlib/app>
#include <inlib/osc_env>
#include <inlib/args>

#include <iostream>
#include <cstdlib>

int main(int argc,char** argv){

  inlib::args args(argc,argv);

  if(args.is_arg("-version")) {
    std::cout << ONXLAB_VERSION << std::endl;
    return EXIT_SUCCESS;
  }

  if(args.is_arg("-h")) {
    std::vector<std::string> text;
    text.resize(osc_plot_help_linen);
    for(unsigned int index=0;index<osc_plot_help_linen;index++) {
      std::string s = osc_plot_help_lines[index];
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
    home_dirs.push_back("OnXLab");
    home_dirs.push_back("session");
    if(!inlib::if_mouse_startup(arg0,"OSC_HOME_DIR",home_dirs,std::cout)) { //for osc-plot.app
      std::cout << "OnXLab_onx :"
                << " unable to set mouse startup." << std::endl;
      return EXIT_FAILURE;
    }
  }
  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "OnXLab_onx :"
              << " unable to set OnX environment." << std::endl;
    return EXIT_FAILURE;
  }
  if(!OnXLab::set_env(std::cout)) {
    std::cout << "OnXLab_onx :"
              << " unable to set OnXLab environment." << std::endl;
    return EXIT_FAILURE;
  }

 {std::vector<std::string> files = args.files();
  for(std::vector<std::string>::const_iterator it = files.begin();
      it!=files.end();++it) {
    args.add("-data_file",*it,false);
    args.remove(*it); //else OnX::Main will take it as a .onx file !
  }}

 {std::string OSC_HOME_DIR;
  if(!inlib::getenv("OSC_HOME_DIR",OSC_HOME_DIR)) {
    std::cout << "OnXLab_onx :"
              << " environment variable OSC_HOME_DIR not defined."
              << std::endl;
    return EXIT_FAILURE;
  }
  std::string file = OSC_HOME_DIR + "/Resources/OnXLab/scripts/OnX/main.onx";
  args.add("-onx_file",file);}

  Slash::Core::ISession* session = Slash_create_session(args.tovector());
  if(!session) {
    std::cout << "OnXLab_onx :"
              << " session not found." << std::endl;
    return EXIT_FAILURE;
  }

  Slash::UI::IUI* ui = Slash::find_UI(*session);
  if(!ui) {
    std::cout << "OnXLab_onx :"
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
