#ifndef Slash_Tools_osc_env_h
#define Slash_Tools_osc_env_h

#include <inlib/system>
#include <inlib/sep>

namespace Slash {
namespace osc {

// used in OnX/source/SWIG/Slash.i/def create_session

inline bool set_env(std::ostream& a_out){
  
  std::string OSC_HOME_DIR;
  if(!inlib::getenv("OSC_HOME_DIR",OSC_HOME_DIR)) {
    a_out << "Slash::osc::set_env :"
          << " Environment variable OSC_HOME_DIR not defined."
          << " You have to setup OpenScientist by sourcing "
          << " the setup script, found in the distribution,"
          << " corresponding to your kind of shell."
          << std::endl;
    return false;
  }

  std::string fs = inlib::sep();

  //NOTE : ONXROOT is used to find <ref> common .onx files.
  //       Also used in OnX/examples.
  std::string ONXROOT = OSC_HOME_DIR + fs + "Resources" + fs + "OnX";        
  if(!inlib::putenv("ONXROOT",ONXROOT)) return false;

  if(!inlib::env_append_path
    ("TTFPATH",OSC_HOME_DIR+fs+"Resources"+fs+"HEPVis"+fs+"fonts")) return false;
  
#ifdef WIN32
  //inlib::putenv("ONX_HAS_PYTHON","no")
#else
#ifdef __APPLE__
  inlib::putenv("ONX_HAS_PYTHON","yes");
#else  // Linux
  //inlib::putenv("ONX_HAS_PYTHON","no")
#endif
#endif

  if(!inlib::env_append_path("PYTHONPATH",
    OSC_HOME_DIR+fs+"Resources"+fs+"OnX"+fs+"scripts"+fs+"Python")) return false;
  if(!inlib::env_append_path("PYTHONPATH",
    OSC_HOME_DIR+fs+"Resources"+fs+"CoinPython"+fs+"scripts")) return false;
  if(!inlib::env_append_path("PYTHONPATH",
    OSC_HOME_DIR+fs+"Resources"+fs+"HEPVis"+fs+"scripts"+fs+"Python")) 
      return false;
  if(!inlib::env_append_path("PYTHONPATH",OSC_HOME_DIR+fs+"bin")) return false;

  return true;
}

}}

namespace Slash {
inline std::vector<std::string> to(int a_argc,char** a_argv) {
  std::vector<std::string> v;
  for(int index=0;index<a_argc;index++) v.push_back(a_argv[index]);
  return v;
}
}

#endif
