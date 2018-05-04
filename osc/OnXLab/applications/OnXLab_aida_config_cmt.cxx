
#include <AIDA/IConstants.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgn
,char** aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  if(aArgn<=1) {
  
    // Similar to : UNIX> /usr/bin/glib-config

    printf("\
Usage: aida-config [OPTIONS]\n\
Options:\n\
        [--version]\n\
        [--libs,--lib]\n\
        [--incs,--cflags,--include]\n\
        [--dll]\n\
        [--implementation]\n\
        [--builder]\n\
");
    return EXIT_SUCCESS;

  } 

  if(!strcmp(aArgs[1],"--version")) {
    printf("%s\n",AIDA_VERSION);
  } else if( !strcmp(aArgs[1],"--incs") || 
             !strcmp(aArgs[1],"--cflags") || 
             !strcmp(aArgs[1],"--include") ) {
    if(char* env = getenv("ONXLAB_AIDA_CONFIG_INCS")) {
      printf("%s\n",env);
    } else {
      printf("OnXLab setup not executed.\n");
      return EXIT_FAILURE;
    }
  } else if( !strcmp(aArgs[1],"--libs") ||
             !strcmp(aArgs[1],"--lib") ) {
    if(char* env = getenv("ONXLAB_AIDA_CONFIG_LIBS")) {
      printf("%s\n",env);
    } else {
      printf("OnXLab setup not executed.\n");
      return EXIT_FAILURE;
    }
  } else if(!strcmp(aArgs[1],"--dll")) {
    if(char* env = getenv("ONXLAB_AIDA_CONFIG_DLL")) {
      printf("%s\n",env);
    } else {
      printf("OnXLab setup not executed.\n");
      return EXIT_FAILURE;
    }
  } else if(!strcmp(aArgs[1],"--implementation")) {
    printf("OnXLab\n");
  } else if(!strcmp(aArgs[1],"--builder")) {
    ::printf("CMT\n");
    return EXIT_SUCCESS;
  } else {
    printf("Unknown option.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
