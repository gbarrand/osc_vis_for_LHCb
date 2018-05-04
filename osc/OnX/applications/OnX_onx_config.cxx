
#include <OnX/Version.h>

#include <inlib/config>

#include <cstdio>   //printf
#include <cstdlib>  //EXIT_SUCCESS

int main(int aArgn,char** aArgs) {
  if(aArgn!=2) {
  
    ::printf("\
Usage: onx-config [OPTION]\n\
Options:\n\
        [--version]\n\
        [--incs,--include]\n\
        [--cflags,--cppflags]\n\
        [--libs,--lib]\n\
        [--gl_cflags,--gl_cppflags]\n\
        [--gl_incs,--gl_include]\n\
        [--gl_libs,--gl_lib]\n\
        [--iv_cflags,--iv_cppflags]\n\
        [--iv_incs,--iv_include]\n\
        [--iv_libs,--iv_lib]\n\
        [--py_cflags,--py_cppflags]\n\
        [--py_incs,--py_include]\n\
        [--py_libs,--py_lib]\n\
        [--core_libs,--core_lib]\n\
        \n\
        [--g4_cflags,--g4_cppflags]\n\
        [--g4_incs,--g4_include]\n\
        [--g4_libs,--g4_lib]\n\
        [--g4_pl_cflags,--g4_pl_cppflags]\n\
        [--g4_pl_incs,--g4_pl_include]\n\
        [--g4_pl_libs,--g4_pl_lib]\n\
        [--g4lab_cflags,--g4lab_cppflags]\n\
        [--g4lab_incs,--g4lab_include]\n\
        [--g4lab_libs,--g4lab_lib]\n\
        \n\
        [--g3lab_cflags,--g3lab_cppflags]\n\
        [--g3lab_incs,--g3lab_include]\n\
        [--g3lab_libs,--g3lab_lib]\n\
        \n\
        [--compiler]\n\
        [--builder]\n\
");
    return EXIT_SUCCESS;
  } 

  std::string arg1 = aArgs[1];

  if(arg1=="--version") {
    ::printf("%s\n",ONX_VERSION);
    return EXIT_SUCCESS;

  } else if(arg1=="--builder") {

    ::printf("obuild\n");
    return EXIT_SUCCESS;

  } else if(arg1=="--compiler") {

    std::string value;
    if(!inlib::config::find_cxx(aArgs[0],"OnX_config_Lib.save",value)) 
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // Lib :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--incs") || 
             (arg1=="--include") ) {

    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"OnX_config_Lib.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  } else if( (arg1=="--cflags") || 
             (arg1=="--cppflags") ) {
  
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"OnX_config_Lib.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  
  } else if( (arg1=="--libs") || 
             (arg1=="--lib") ) {

    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"OnX_config_Lib.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // OpenGL :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--gl_incs") || 
             (arg1=="--gl_include") ) {

    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"OnX_config_OpenGL.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  } else if( (arg1=="--gl_cflags") || 
             (arg1=="--gl_cppflags") ) {
  
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"OnX_config_OpenGL.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  
  } else if( (arg1=="--gl_libs") || 
             (arg1=="--gl_lib") ) {

    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"OnX_config_OpenGL.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // Inventor :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--iv_incs") || 
             (arg1=="--iv_cflags") || 
             (arg1=="--iv_include") ) {

    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"OnX_config_Inventor.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  } else if( (arg1=="--iv_cflags") || 
             (arg1=="--iv_cppflags") ) {
  
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"OnX_config_Inventor.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  } else if( (arg1=="--iv_libs") || 
             (arg1=="--iv_lib") ) {

    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"OnX_config_Inventor.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // Python :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--py_incs") || 
             (arg1=="--py_cflags") || 
             (arg1=="--py_include") ) {

    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"OnX_config_Python.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  } else if( (arg1=="--py_cflags") || 
             (arg1=="--py_cppflags") ) {
  
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"OnX_config_Python.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  } else if( (arg1=="--py_libs") || 
             (arg1=="--py_lib") ) {

    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"OnX_config_Python.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // OnX :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--core_libs") || 
             (arg1=="--core_lib") ) {

    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"OnX_onx.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // Geant4 :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--g4_incs") || 
             (arg1=="--g4_include") ) {
    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"G4Lab_config_Geant4.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g4_cflags") || 
             (arg1=="--g4_cppflags") ) {
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"G4Lab_config_Geant4.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g4_libs") || 
             (arg1=="--g4_lib") ) {
    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"G4Lab_config_Geant4.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // Geant4 physics lists :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--g4_pl_incs") || 
             (arg1=="--g4_pl_include") ) {
    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"G4Lab_config_physics_lists.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g4_pl_cflags") || 
             (arg1=="--g4_pl_cppflags") ) {
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"G4Lab_config_physics_lists.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g4_pl_libs") || 
             (arg1=="--g4_pl_lib") ) {
    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"G4Lab_config_physics_lists.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // G4Lab :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--g4lab_incs") || 
             (arg1=="--g4lab_include") ) {
    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"G4Lab_config.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g4lab_cflags") || 
             (arg1=="--g4lab_cppflags") ) {
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"G4Lab_config.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g4lab_libs") || 
             (arg1=="--g4lab_lib") ) {
    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"G4Lab_config.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;

  ////////////////////////////////////////////////////////////////////////////
  // G3Lab :
  ////////////////////////////////////////////////////////////////////////////
  } else if( (arg1=="--g3lab_incs") || 
             (arg1=="--g3lab_include") ) {
    std::string value;
    if(!inlib::config::find_incs(aArgs[0],"G3Lab_config.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g3lab_cflags") || 
             (arg1=="--g3lab_cppflags") ) {
    std::string value;
    if(!inlib::config::find_flags(aArgs[0],"G3Lab_config.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  } else if( (arg1=="--g3lab_libs") || 
             (arg1=="--g3lab_lib") ) {
    std::string value;
    if(!inlib::config::find_libs(aArgs[0],"G3Lab_config.save",value))
      return EXIT_FAILURE;
    ::printf("%s\n",value.c_str());
    return EXIT_SUCCESS;
  }

  ::fprintf(stderr,"onx-config : unknown option.\n");
  
 {::fprintf(stderr,"onx-config : passed arguments had been :\n");
  for(int index=0;index<aArgn;index++) {
    ::fprintf(stderr,"%s\n",aArgs[index]);
  }}
  
  return EXIT_FAILURE;
}
