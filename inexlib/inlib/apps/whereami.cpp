// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/app>

#include <cstdlib>
#include <iostream>

int main(int a_argc,char** a_args) {
  if(a_argc<=0) return EXIT_FAILURE;
  std::string arg0 = a_args[0];
  std::string path;
  if(!inlib::program_path(arg0,path)) {
    std::cout << "inlib_whereami :"
        << " can't get binary path of " << arg0
        << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << path << std::endl;
  return EXIT_SUCCESS;
}
