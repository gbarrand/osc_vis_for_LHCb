
#include <Inventor/Gtk/SoGtkBasic.h> //SOGTK_VERSION

#include "Lib_embed.h"

#include <stdio.h>   //printf
#include <stdlib.h>  //EXIT_SUCCESS

#include "find_config.icxx"

int main(int aArgn,char** aArgs) {
  if(aArgn==1) {
  
    ::printf("\
Usage: osc-coin-gtk-config [OPTION]\n\
Options:\n\
        [--version]\n\
        [--incs,--include]\n\
        [--cflags,--cppflags]\n\
        [--libs,--lib]\n\
        [--includedir]\n\
        [--compiler]\n\
        [--builder]\n\
        [--have-feature]\n\
");
    return EXIT_SUCCESS;

  } else if(aArgn==2) {

    std::string arg1 = aArgs[1];
  
    if(arg1=="--version") {
      ::printf("%s\n",SOGTK_VERSION);
      return EXIT_SUCCESS;
  
    } else if(arg1=="--builder") {
  
      ::printf("obuild\n");
      return EXIT_SUCCESS;
  
    } else if(arg1=="--compiler") {
  
      if(!find_cxx(aArgs[0],"CoinGtk_config_save.save")) return EXIT_FAILURE;
      return EXIT_SUCCESS;
  
    } else if( (arg1=="--incs") || 
               (arg1=="--include") ) {
  
      if(!find_incs(aArgs[0],"CoinGtk_config_save.save")) return EXIT_FAILURE;
      return EXIT_SUCCESS;
  
    } else if( (arg1=="--cflags") || 
               (arg1=="--cppflags") ) {
  
      if(!find_flags(aArgs[0],"CoinGtk_config_save.save")) return EXIT_FAILURE;
      return EXIT_SUCCESS;
  
    } else if( (arg1=="--libs") || 
               (arg1=="--lib") ) {
  
      if(!find_libs(aArgs[0],"CoinGtk_config_save.save")) return EXIT_FAILURE;
      return EXIT_SUCCESS;
  
    } else if( (arg1=="--includedir") ) {

      if(!find_incdir(aArgs[0],"CoinGtk_config_save.save")) 
        return EXIT_FAILURE;
      return EXIT_SUCCESS;
  
    }

  } else if(aArgn==3) {

    std::string arg1 = aArgs[1];
    std::string arg2 = aArgs[2];
    if(arg1=="--have-feature") {

      ::printf("no\n");
      return EXIT_SUCCESS;

    } else if( (arg1=="--ldflags") && (arg2=="--libs") ) {

      // For Pivy-0.3.0 setup.py.
      //FIXME : loop on args.

      if(!find_libs(aArgs[0],"CoinGtk_config_save.save")) return EXIT_FAILURE;
      return EXIT_SUCCESS;

    }

  }

  ::fprintf(stderr,"osc-coin-gtk-config : unknown option.\n");
  
 {::fprintf(stderr,"osc-coin-gtk-config : passed arguments had been :\n");
  for(int index=0;index<aArgn;index++) {
    ::fprintf(stderr,"%s\n",aArgs[index]);
  }}
  
  return EXIT_FAILURE;
}

// embed Lib code :
#include "Lib_embed.icxx"
