
#include <inlib/args>

#include <Lib/calc.h>

#include <cstdlib>
#include <iostream>

int main(int a_argc,char** a_argv){

  inlib::args args(a_argc,a_argv);

  if(args.is_arg("-h")) {
    std::cout << "Lib_calc :" << std::endl;
    std::cout << "Usage :" << std::endl;
    std::cout << "  OS> Lib_calc -h" << std::endl;
    std::cout << "  OS> Lib_calc -formula=<formula> [variable values]" << std::endl;
    std::cout << "For example :" << std::endl;
    std::cout << " UNIX> Lib_calc '-formula=sqrt(2*x+2*y)' 2 3" << std::endl;
    return EXIT_SUCCESS;
  }

  std::string formula;
  if(!args.find("-formula",formula)) {
    std::cout << "Lib_calc : -formula argument missing." << std::endl;
    return EXIT_FAILURE;
  }

  args.remove_first();
  args.remove("-formula");
  std::vector<std::string> vals = args.tovector();
  std::string value;

  if(!Lib::calc(std::cout,formula,vals,value)) {
    std::cout << "Lib_calc : failed." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << value << std::endl;

  return EXIT_SUCCESS;
}

// embed code :

#include <Debug.cxx>
#include <Processor.cxx>
//Do the below to avoid to have YYSTYPE doubly defined.
#define Lib_ProcessorTokens_h
#include <ProcessorLexer.cxx>
