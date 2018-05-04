// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/args>
#include <inlib/file>

#include <cstdlib>
#include <iostream>

int main(int a_argc,char** a_argv) {

  inlib::args args(a_argc,a_argv);

  std::string file;
  if(!args.file(file)) {
    std::cout << "no file given." << std::endl;
    return EXIT_FAILURE;
  }
  
  //char sep;
  bool is;
  if(!inlib::file::is_hippo(file,is)) {
    std::cout << "inlib::file::is_hippo failed." << std::endl;
    return EXIT_FAILURE;
  }

  if(is) {
    std::cout << "it is a hippo file." << std::endl;
  } else {
    std::cout << "it is not a hippo file." << std::endl;
  }

  return EXIT_SUCCESS;
}
