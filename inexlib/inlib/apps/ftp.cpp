// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/args>

#include <inlib/path>
#include <inlib/net/ftp>

#include <cstdlib>
#include <iostream>

int main(int argc,char* argv[]) {

  inlib::args args(argc,argv);

  if(args.is_arg("-h")) {
    std::cout << "inlib_ftp ftp://ftp.lal.in2p3.fr/pub/OpenScientist/16.11/osc_obuild_source_16.11.zip" 
              << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  bool dir = false;
  if(args.is_arg("-dir")) dir = true;

  std::string url;

  if(!args.file(url)) {
    std::cout << "inlib_ftp :"
              << " give an url as last argument."
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string host,path;
  if(!inlib::net::ftp::parse(url,host,path)) {
    std::cout << "inlib_ftp :"
              << " inlib::net::ftp::parse failed."
              << std::endl;
    return EXIT_FAILURE;
  }

  inlib::net::ftp ftp(std::cout,verbose);
  if(!ftp.start(host,inlib::net::ftp::s_anonymous(),"")) return EXIT_FAILURE;

  std::string file = inlib::base_name(path);

  if(dir) {
    if(!ftp.fetch_dir(path,file)) return EXIT_FAILURE;
  } else {
    if(!ftp.fetch_file(path,file)) return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
