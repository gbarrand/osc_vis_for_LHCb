// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/upgrader>
#include <inlib/args>

#include <cstdlib>
#include <iostream>

int main (int a_argc,char** a_argv){

  //{// balance block :

  inlib::args args(a_argc,a_argv);

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::vector<std::string> options;
  options.push_back("-h");
  options.push_back("-verbose");
  options.push_back("-orders");
  options.push_back("-filter");

  if(!args.known_options(options)) {
    std::cout << "inlib_upgrade :"
        << " Some options are unknown." << std::endl;
    std::cout << "To have help, type :" << std::endl;
    std::cout << " OS> inlib_upgrade -h" << std::endl;
    return EXIT_SUCCESS;
  }

  if(args.is_arg("-h")) {
    std::cout << "inlib_upgrade :" << std::endl;
    std::cout << "Usage :" << std::endl;
    std::cout << " OS> inlib_upgrade <options> -orders=<file> <path>" << std::endl;
    std::cout << "Options :" << std::endl;
    std::cout << " -verbose" << std::endl;
    std::cout << " -filter=<string>" << std::endl;
    return EXIT_SUCCESS;
  }

  std::string orders;
  if(!args.find("-orders",orders)) {
    std::cout << "inlib_upgrade :"
        << " no order file given. Take $HOME/inlib.upgrade." 
        << std::endl;
    std::string home = inlib::dir::home();
    orders = home+inlib::sep()+"inlib.upgrade";
  }

  std::string filter;
  args.find("-filter",filter);

  std::string path;
  if(!args.file(path)) path = ".";

  bool is_dir;
  if(!inlib::dir::is_a(path,is_dir)) {}

  inlib::upgrader upgrader(std::cout,verbose,orders,filter);

  if(is_dir) {
    upgrader.upgrade(path);
  } else { //assume a file
    if(!upgrader.upgrade_file(path)) {}
  }

  //} //end balance block.

 //Lib::Debug::balance(std::cout);

  return EXIT_SUCCESS;
}
