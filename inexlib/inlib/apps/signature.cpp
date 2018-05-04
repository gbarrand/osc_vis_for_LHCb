// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

// dump the first four "signature" characters of a file.

#include <inlib/args>
#include <inlib/file>
#include <inlib/charmanip>

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

  unsigned int wnum = 4; //wanted num.
  if(!args.find("-num",wnum)) wnum = 4;
  if(!wnum) return EXIT_SUCCESS;

  unsigned char* header = new unsigned char[wnum];
  unsigned int num = wnum;
  if(!inlib::file::signature(file,header,num)||(num!=wnum)) {
    std::cout << "inlib::file::signature failed." << std::endl;
    delete [] header;
    return EXIT_FAILURE;
  }

  for(unsigned int index=0;index<num;index++) {
    unsigned char c = header[index];
    if(inlib::is_printable(c)) {
      ::printf("char %d is %d (0x%x) \'%c\'\n",index,c,c,c);
    } else {
      ::printf("char %d is %d (0x%x) (not printable)\n",index,c,c);
    }
  }

  delete [] header;

  return EXIT_SUCCESS;
}
