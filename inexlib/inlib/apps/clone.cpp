// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.


#include <inlib/cloner>
#include <inlib/args>

#include <cstdlib>
#include <iostream>

int main (int a_argc,char** a_argv){

  inlib::args args(a_argc,a_argv);

  if(args.is_arg("-h")) {
    std::cout << "inlib_clone :" << std::endl;
    std::cout << "Usage :" << std::endl;
    std::cout << "  OS> inlib_clone -h" << std::endl;
    std::cout << "  OS> inlib_clone [-verbose] -path=<source path> -clone=<pack> -to=<name>" << std::endl;
    std::cout << "For example :" << std::endl;
    std::cout << "  OS> inlib_clone -path=/projects/OnX/v13r0/templates/OnXTemplateOne -clone=OnXTemplateOne -to=Xxx" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::string path;
  if(!args.find("-path",path)) {
    std::cout 
      << "  OS> inlib_clone -path=<source path> -clone=<pack> -to=<name>" 
      << std::endl;
    return EXIT_FAILURE;
  }

  std::string from;
  if(!args.find("-clone",from)) {
    std::cout 
      << "  OS> inlib_clone -path=<source path> -clone=<pack> -to=<name>" 
      << std::endl;
    return EXIT_FAILURE;
  }

  std::string to;
  if(!args.find("-to",to)) {
    std::cout 
      << "  OS> inlib_clone -path=<source path> -clone=<pack> -to=<name>" 
      << std::endl;
    return EXIT_FAILURE;
  }

  bool debug = args.is_arg("-debug");

  bool status = inlib::cloner::clone(path,from,to,std::cout,verbose,debug);

  return status?EXIT_SUCCESS:EXIT_FAILURE;
}
