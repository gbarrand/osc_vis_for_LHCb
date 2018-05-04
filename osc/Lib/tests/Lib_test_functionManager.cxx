
// To test the function manager :

#include <Lib/Session.h>
#include <Lib/LibraryManager.h>
#include <Lib/FunctionManager.h>
#include <Lib/Debug.h>

#include <inlib/args>
#include <inlib/system>

#include <iostream>

#include <Slash/Tools/Core.h>

int main(int a_argc,char** a_argv) {

 {inlib::args args(a_argc,a_argv);

  if(!inlib::isenv("LIBROOT")){
    std::cout << "LIBROOT not defined."
              << std::endl;
    return EXIT_FAILURE;
  }

  int verbose = 0;
  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verbose = 1;
  }

  Slash::Printer printer;
  Lib::Session session(printer);
  session.setVerboseLevel(verbose);
  session.addManager(new Lib::LibraryManager(session));

  Lib::FunctionManager functionManager(session);

  std::cout << "Lib_functionManager :" 
      << " load $LIBROOT/data/htfun1.f"
      << std::endl;
  
  functionManager.load("$LIBROOT/data/htfun1.f");

  Slash::Data::IFunction* htfun1 = functionManager.findFunction("HTFUN1");
  if(htfun1) {
    std::vector<double> args(1);
    args[0] = 0.3;
    double value;
    htfun1->value(args,value);
    std::cout << "Lib_functionManager :" 
        << " HTFUN1(0.3) = " << value << " (1.00193 expected)." 
        << std::endl;
  } else {
    std::cout << "Lib_functionManager :" 
        << " function HTFUN1 not found."
        << std::endl;
  }

  Slash::Data::IFunction* htfun2 = functionManager.findFunction("HTFUN2");
  if(htfun2) {
    std::vector<double> args(2);
    args[0] = 0.3;
    args[1] = 0.3;
    double value;
    htfun2->value(args,value);
    std::cout << "Lib_functionManager :" 
        << " HTFUN2(0.3,0.3) = " << value << " (100.387 expected)." 
        << std::endl;
  } else {
    std::cout << "Lib_functionManager :" 
        << " function HTFUN2 not found."
        << std::endl;
  }

  std::cout << "Lib_functionManager :" 
      << " load $LIBROOT/data/chtfun1.c"
      << std::endl;
  
  functionManager.load("$LIBROOT/data/chtfun1.c");
  Slash::Data::IFunction* chtfun1 = functionManager.findFunction("chtfun1");
  if(chtfun1) {
    std::vector<double> args(1);
    args[0] = 0.3;
    double value;
    chtfun1->value(args,value);
    std::cout << "Lib_functionManager :" 
        << " chtfun1(0.3) = " << value << " (1.00193 expected)." 
        << std::endl;
  } else {
    std::cout << "Lib_functionManager :" 
        << " function chtfun1 not found."
        << std::endl;
  }

  Slash::Data::IFunction* CHTFUN2 = functionManager.findFunction("CHTFUN2");
  if(CHTFUN2) {
    std::vector<double> args(2);
    args[0] = 0.3;
    args[1] = 0.3;
    double value;
    CHTFUN2->value(args,value);
    std::cout << "Lib_functionManager :" 
        << " CHTFUN2(0.3,0.3) = " << value << " (100.387 expected)." 
        << std::endl;
  } else {
    std::cout << "Lib_functionManager :" 
        << " function CHTFUN2 not found."
        << std::endl;
  }


 }

  Lib::Debug::balance(std::cout);

  return 0;
}
