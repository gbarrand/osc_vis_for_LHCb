
#include <inlib/net/ftp>
#include <inlib/system>

extern "C" {
  int test_inet(const char*); //to be called from ObjC.
}

#include <iostream>

int test_inet(const char* a_doc_dir) {
  
  std::string url("ftp://ftp.lal.in2p3.fr/pub/OpenScientist");
  std::string host;
  std::string path;
  if(!inlib::net::ftp::parse(url,host,path)) return false;
  std::cout << "host " << inlib::sout(host) << std::endl;
  std::cout << "path " << inlib::sout(path) << std::endl;

  inlib::net::ftp ftp(std::cout,0);
  if(!ftp.start(host)) return false;

  //HOME /Users/barrand/Library/Application Support/iPhone Simulator/3.2/Applications/.
  // TMPDIR is HOME/tmp
  //std::string home;
  //inlib::getenv("HOME",home);
  //std::cout << "home " << inlib::sout(home) << std::endl;

  std::string home(a_doc_dir);
  std::cout << "doc_dir " << inlib::sout(home) << std::endl;

  if(!ftp.fetch_dir("/pub/OpenScientist",home+"/dir_1")) 
    return false;

  std::string dir = "/pub/OpenScientist/16.11";

  if(!ftp.fetch_dir(dir,home+"/dir_2")) return false;

 {std::string file = "osc_obuild_source_16.11.zip";
  if(!ftp.fetch_file(dir+"/"+file,home+"/"+file)) return false;}

 {std::string file = "osc_batch_source_16.11.zip";
  if(!ftp.fetch_file(dir+"/"+file,home+"/"+file)) return false;}

  return true;

}
