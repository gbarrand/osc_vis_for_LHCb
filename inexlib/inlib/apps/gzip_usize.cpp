// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

// dump the first four "signature" characters of a file.

#include <inlib/args>
#include <inlib/file>

#include <cstdlib>
#include <cstdio>
#include <iostream>

int main(int a_argc,char** a_argv) {

  inlib::args args(a_argc,a_argv);

  std::string file;
  if(!args.file(file)) {
    std::cout << "no file given." << std::endl;
    return EXIT_FAILURE;
  }

  bool is;
  if(!inlib::file::is_gzip(file,is)) {
    std::cout << "inlib::file::is_gzip failed." << std::endl;
    return EXIT_FAILURE;
  }
  if(!is) {
    std::cout << "file is not at gzip format." << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int usz;
  if(!inlib::file::gzip_usize(file,usz)) {
    std::cout << "inlib::file::gzip_usize failed." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << usz << std::endl;

  return EXIT_SUCCESS;
}
