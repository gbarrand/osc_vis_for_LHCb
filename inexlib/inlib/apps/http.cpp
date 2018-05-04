// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/args>

#include <inlib/path>
#include <inlib/net/http>

#include <cstdlib>
#include <iostream>

int main(int argc,char* argv[]) {

  inlib::args args(argc,argv);

  if(args.is_arg("-h")) {
    //Content-Length : <size>
    std::cout << "inlib_http http://OpenScientist.lal.in2p3.fr/download/16.11/A_RELEASE_NOTES_16.11" << std::endl;
    //Transfer-Encoding: chunked :
    std::cout << "inlib_http http://lsst-web.ncsa.illinois.edu//lsstdata/dr-s2012/sdss/coadd/rr1000/s4S/r/" << std::endl;
    return EXIT_SUCCESS;
  }

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::string url;

  if(!args.file(url)) {
    std::cout << "give an url as last argument." << std::endl;
    return EXIT_FAILURE;
  }

  std::string host,path;
  if(!inlib::net::http::parse(url,host,path)) {
    std::cout << "inlib::net::http::parse failed." << std::endl;
    return EXIT_FAILURE;
  }

  inlib::net::http http(std::cout,verbose);
  if(!http.start(host)) return EXIT_FAILURE;

  std::string file = inlib::base_name(path);
  if(file.empty()) file = "http_content";

  if(verbose) {
    std::cout << "host " << inlib::sout(host) << std::endl;
    std::cout << "path " << inlib::sout(path) << std::endl;
    std::cout << "file " << inlib::sout(file) << std::endl;
  }

  if(!http.fetch(path,file)) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
