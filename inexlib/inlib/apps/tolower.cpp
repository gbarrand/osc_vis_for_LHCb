// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#ifdef INLIB_MEM
#include "../inlib/mem"
#endif

#include <inlib/args>
#include <inlib/file>
#include <inlib/touplow>

#include <cstdlib>
#include <iostream>

int main (int a_argc,char** a_argv){

#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif

  inlib::args args(a_argc,a_argv);

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::string file;
  if(!args.file(file)) {
    std::cout << "no file given." << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> txt;
  if(!inlib::file::read(file,txt)) {
    std::cout << " can't read input file \"" << file << "\"."
              << std::endl;
    return EXIT_FAILURE;
  }

 {std::vector<std::string>::iterator it;
  for(it=txt.begin();it!=txt.end();++it) {
    inlib::tolowercase(*it);
  }}

  if(!inlib::file::write(file,txt)) {
    std::cout << " can't write file \"" << file << "\"."
              << std::endl;
    return EXIT_FAILURE;
  }

#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
#endif

  return EXIT_SUCCESS;
}
