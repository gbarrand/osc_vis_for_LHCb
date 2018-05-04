// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/args>
#include <inlib/sys/plugin>

#include <cstdlib>
#include <iostream>

int main (int a_argc,char** a_argv) {
  inlib::args args(a_argc,a_argv);

  if(args.is_arg("-h")) {
    std::cout << "inlib_plug :" << std::endl;
    std::cout << "Usage :" << std::endl;
    std::cout << " OS> inlib_plug <options>" << std::endl;
    std::cout << "Options :" << std::endl;
    std::cout << " -plugin=<string> : plugin to open." << std::endl;
    std::cout << " -symbol=<string> : symbol to search in the plugin." << std::endl;
    std::cout << " -verbose." << std::endl;
    std::cout << " -h : to have help." << std::endl;
    std::cout << "For example :" << std::endl;
    std::cout << " OS> inlib_plug -plugin=plugin -symbol=test_init" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::string splg;
  if(!args.find("-plugin",splg)) {
    std::cout << "inlib_plug :" << std::endl;
    std::cout << " give a plugin with -plugin=<file>" << std::endl;
    return EXIT_FAILURE;
  }

 {// Put in a block to check the destruction.
  inlib::plugin plg(splg,std::cout,verbose);
  if(!plg.is_opened()) return EXIT_FAILURE;

  std::string symbol;
  args.find("-symbol",symbol);

  if(symbol.size()) {
    if(!plg.find(symbol)) {
      return EXIT_FAILURE;
    }
  }}

  return EXIT_SUCCESS;
}
