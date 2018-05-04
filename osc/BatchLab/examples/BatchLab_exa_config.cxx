//
//  Example to get the main AIDA entry point.
// 
//  It can be used to check that aida-config
// is operational to build an AIDA application
// by using the BatchLab implementation :
//   UNIX> <source aida-setup>
//   UNIX> c++ `aida-config --incs` BatchLab_exa_build.cxx `aida-config --libs`
//   UNIX> ./a.out
//      
//

#include <AIDA/IAnalysisFactory.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "BatchLab_exa_build::main :"
              << " Can't get AIDA BatchLab implementation." << std::endl;
    return 1;
  } else {
    std::cout << "BatchLab_exa_build::main :"
              << " AIDA BatchLab implementation found." << std::endl;
  }
  delete aida;
  return 0;
}

