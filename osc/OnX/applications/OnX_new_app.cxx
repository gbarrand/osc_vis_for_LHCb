
#include <inlib/args>
#include <inlib/system>

#include <inlib/cloner>

#include <cstdlib>
#include <iostream>

// Embedded code :
#include "osc_new_app_help.h"

int main(int a_argc,char** a_argv){
  if(a_argc<=0) return EXIT_FAILURE;

  inlib::args args(a_argc,a_argv);

  if(args.is_arg("-h")) {
    std::vector<std::string> text;
    text.resize(osc_new_app_help_linen);
    for(unsigned int index=0;index<osc_new_app_help_linen;index++) {
      std::string s = osc_new_app_help_lines[index];
      inlib::replace(s,"@@double_quote@@","\"");
      inlib::replace(s,"@@back_slash@@","\\");
      text[index] = s;
    }
    for(unsigned int index=0;index<text.size();index++) {
      std::cout << text[index] << std::endl;
    }
    return EXIT_SUCCESS;
  }

  std::string name;
  args.find("-name",name);
  if(name.empty()) name="MyApp";

  std::string path;
  std::string from;
  if(args.is_arg("-gl")) {
    from="OnXTemplateThree";
    path="$OSC_HOME_DIR/Resources/OnX/templates/" + from;   
  } else if(args.is_arg("-iv")) {
    from="OnXTemplateFour";
    path="$OSC_HOME_DIR/Resources/OnX/templates/" + from;   
  } else if(args.is_arg("-py")) {
    from="OnXTemplateTwo";
    path="$OSC_HOME_DIR/Resources/OnX/templates/" + from;   
  } else if(args.is_arg("-g4")) {
    from="G4LabTemplateOne";
    path="$OSC_HOME_DIR/Resources/G4Lab/templates/" + from;   
  } else {
    std::string s;
    args.find("-template",s);
    if(s.empty()) {
      from="OnXTemplateOne";
      path="$OSC_HOME_DIR/Resources/OnX/templates/" + from;   
    } else {
      path = s;
      from = inlib::base_name(s);
    }
  }


  std::string p;
  inlib::file_name(path,p);
  bool debug = args.is_arg("-debug");
  bool verbose = args.is_arg("-verbose");

  if(!inlib::cloner::clone(p,from,name,std::cout,verbose,debug)) {
    return EXIT_FAILURE;
  }

  // Some libraries (for exa Coin, gtk) may have declared 
  // C functions with ::atexit. To ease debugging we call
  // explicitly ::exit here.
  ::exit(EXIT_SUCCESS);

  return EXIT_SUCCESS;
}
