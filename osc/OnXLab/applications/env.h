#ifndef OnXLab_env_h
#define OnXLab_env_h

#include <inlib/system>

namespace OnXLab {

inline bool set_env(std::ostream& a_out) {

  std::string OSC_HOME_DIR;
  if(!inlib::getenv("OSC_HOME_DIR",OSC_HOME_DIR)) {
    a_out << "OnXLab::set_env :"
          << " environment variable OSC_HOME_DIR not defined."
          << std::endl;
    return false;
  }

  std::string fs = inlib::sep();
  std::string ONXLABROOT = OSC_HOME_DIR + fs + "Resources" + fs + "OnXLab";
        
  //if(!inlib::putenv("ONXLABROOT",ONXLABROOT)) return false;

  if(!inlib::env_append_path("PYTHONPATH",
       ONXLABROOT+fs+"scripts"+fs+"Python")) return false;

  return true;
}

}

#endif
