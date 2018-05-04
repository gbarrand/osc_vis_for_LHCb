/*
 * Example of an OnX client program.
 */

#include <inlib/system>
#include <inlib/args>
#include <inlib/net/inet_socket>
#include <Slash/Net/protocol>

#include <iostream>
#include <cstdlib>

int main(int a_argc,char** a_argv){

  int ret_status = EXIT_SUCCESS;

 {inlib::args args(a_argc,a_argv);

  int verbose = 0;
  if(args.is_arg("-verbose")) verbose = 1;

  std::string server;
  if(!args.find("-server",server)) {
    std::cout << "argument -server=<string> not found."
              << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int port;
  if(!args.find<unsigned int>("-port",port)) port = 50701;
  
  inlib::net::inet_socket client(std::cout,verbose);

  if(!client.connect(server,port,10,1)) {
    std::cout << "OnX_client : "
              << " can't connect to server."
              << std::endl;
    return EXIT_FAILURE;
  }

 {std::string OSC_HOME_DIR;
  if(!inlib::getenv("OSC_HOME_DIR",OSC_HOME_DIR)) {
    std::cout << "OnX_client :"
          << " Environment variable OSC_HOME_DIR not defined."
          << " You have to setup OpenScientist by sourcing "
          << " the setup script, found in the distribution,"
          << " corresponding to your kind of shell."
          << std::endl;
    return EXIT_FAILURE;
  }
  std::string fs = inlib::sep();
  std::string ONXROOT = OSC_HOME_DIR + fs + "Resources" + fs + "OnX";        
  if(!inlib::putenv("ONXROOT",ONXROOT)) {
    std::cout << "OnX_client : "
              << " can't putenv ONXROOT."
              << std::endl;
    return EXIT_FAILURE;
  }


  if(!client.send_string("")) { //default GUI driver.
    std::cout << "OnX_client : "
              << " can't send empty string to server."
              << std::endl;
    return EXIT_FAILURE;
  }

  std::string GUI = ONXROOT+fs+"tests"+fs+"onx_tests"+fs+"main.onx";        
  if(!client.send_string(GUI)) {
    std::cout << "OnX_client : "
              << " can't send string to server."
              << std::endl;
    return EXIT_FAILURE;
  }}

  // Treat events coming from the OnX server :
  while(true) {
    //std::cout << "debug : go to waitString..." << std::endl;

    std::string prot;

    if(!client.fetch_string(prot)) {
      return EXIT_FAILURE;
    }

    //std::cout << "debug : from server protocol " << inlib::sout(prot) 
    //          << std::endl;

    if(prot==inlib_net_protocol_file) {
      std::string file;
      if(!client.fetch_string(file)) return EXIT_FAILURE;

      //std::cout << "OnX_client : "
      //          << " want file " << inlib::sout(file) << " ..."
      //          << std::endl;

      std::string p;
      inlib::file_name(file,p);

      //std::cout << "OnX_client : "
      //          << " send file " << inlib::sout(p) << " ..."
      //          << std::endl;

      if(!client.send_file(p)) {
	std::cout << "OnX_client : "
                  << " can't send file " << inlib::sout(file) << "."
                  << std::endl;
        return EXIT_FAILURE;
      }

    } else if(prot==inlib_net_protocol_exit) {
      std::string scode;
      if(!client.fetch_string(scode)) return EXIT_FAILURE;
      int exit_code;
      if(!inlib::to<int>(scode,exit_code)) return EXIT_FAILURE;
      return exit_code;

    } else {
       std::cout << "OnX_client :"
                 << " protocol : " << inlib::sout(prot) 
                 << " not treated."
                 << std::endl;
    }

  }

  }

  return ret_status;
}
