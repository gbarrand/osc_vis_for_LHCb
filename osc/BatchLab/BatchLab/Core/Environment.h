// Used in FunctionCatalog::loadAll.

#ifndef BatchLab_Environment_h
#define BatchLab_Environment_h

#include <inlib/system>
#include <inlib/sep>

#include <ostream>

namespace BatchLab {

inline bool set_env(std::ostream& a_out){

  std::string dir_osc;
  if(!inlib::getenv("OSC_HOME_DIR",dir_osc)) {
    // look for AIDA_HOME_DIR :
    if(!inlib::getenv("AIDA_HOME_DIR",dir_osc)) {
      a_out << "BatchLab::set_env :"
            << " environment variable" 
            << " OSC_HOME_DIR and AIDA_HOME_DIR not defined."
            << " Can't guess where OpenScientist had been installed."
            << std::endl;
      return false;
    }
  }


  std::string fs = inlib::sep();
  std::string BATCHLABROOT = dir_osc + fs + "Resources" + fs + "BatchLab";

  if(!inlib::putenv
        ("BATCHLABROOT",dir_osc+fs+"Resources"+fs+"BatchLab")) return false;

  // For the BatchLab/source/Core/FunctionCatalog.cxx and
  // BatchLab/scripts/[sh,DOS]/mak[c,f77]dll[.bat] :
  if(!inlib::putenv("INLIBROOT",dir_osc+fs+"Resources"+fs+"inlib")) return false;
  if(!inlib::putenv("SLASHROOT",dir_osc+fs+"Resources"+fs+"Slash")) return false;
  if(!inlib::putenv("EXLIBROOT",dir_osc+fs+"Resources"+fs+"exlib")) return false;
  if(!inlib::putenv("LIBROOT",dir_osc+fs+"Resources"+fs+"Lib")) return false;
  if(!inlib::putenv("LIBDIRLIB",dir_osc+fs+"lib")) return false;
  if(!inlib::putenv("BATCHLABDIRLIB",dir_osc+fs+"lib")) return false;

  return true;
}

}

#endif
