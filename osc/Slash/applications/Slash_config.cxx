
#include <Slash/Version.h>

#include <inlib/config>

#include <cstdio>   //printf
#include <cstdlib>  //EXIT_SUCCESS

int main(int aArgn,char** aArgs) {
  if(aArgn==1) {
  
    ::printf("\
Usage: slash-config [OPTION]\n\
Options:\n\
        [--version]\n\
        [--incs,--include]\n\
        [--cflags,--cppflags]\n\
        [--compiler]\n\
        [--builder]\n\
");
    return EXIT_SUCCESS;

  } else if(aArgn==2) {

    std::string arg1 = aArgs[1];
  
    if(arg1=="--version") {
      ::printf("%s\n",SLASH_VERSION);
      return EXIT_SUCCESS;
  
    } else if(arg1=="--builder") {
  
      ::printf("obuild\n");
      return EXIT_SUCCESS;
  
    } else if(arg1=="--compiler") {
  
      std::string value;
      if(!inlib::config::find_cxx(aArgs[0],"Slash_config_save.save",value)) return EXIT_FAILURE;
      ::printf("%s\n",value.c_str());
      return EXIT_SUCCESS;
  
    } else if( (arg1=="--incs") || 
               (arg1=="--include") ) {
  
      std::string value;
      if(!inlib::config::find_incs(aArgs[0],"Slash_config_save.save",value)) return EXIT_FAILURE;
      ::printf("%s\n",value.c_str());
      return EXIT_SUCCESS;
  
    } else if( (arg1=="--cflags") || 
               (arg1=="--cppflags") ) {
  
      std::string value;
      if(!inlib::config::find_flags(aArgs[0],"Slash_config_save.save",value)) return EXIT_FAILURE;
      ::printf("%s\n",value.c_str());
      return EXIT_SUCCESS;
  
    }

  }

  ::fprintf(stderr,"slash-config : unknown option.\n");
  
 {::fprintf(stderr,"slash-config : passed arguments had been :\n");
  for(int index=0;index<aArgn;index++) {
    ::fprintf(stderr,"%s\n",aArgs[index]);
  }}
  
  return EXIT_FAILURE;
}
