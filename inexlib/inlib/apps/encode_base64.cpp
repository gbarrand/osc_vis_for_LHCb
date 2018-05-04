// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/args>

#include <inlib/file>
#include <inlib/base64>
#include <inlib/vdata>

#include <cstdlib>
#include <iostream>

int main(int argc,char* argv[]) {

  inlib::args args(argc,argv);

  std::string file;
  if(!args.file(file)) {
    std::cout << "inlib_encode_base64 :" 
              << " no file given."
              << std::endl;
    return EXIT_FAILURE;
  }

  char* buffer;
  long length;
  if(!inlib::file::read_bytes(file,buffer,length)) {
    std::cout << "inlib_encode_base64 :" 
              << " can't read file \"" << file << "\"."
              << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<char> vc;
  inlib::base64::encode((unsigned int)length,buffer,vc);

  delete [] buffer;

  if(!inlib::file::write_bytes("out.b64",inlib::vec_data(vc),vc.size())) {
    std::cout << "inlib_encode_base64 :" 
              << " can't write file."
              << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
