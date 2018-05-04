
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/NetManager.h>
#include <Slash/Net/protocol>

#include <inlib/args>
#include <inlib/app>
#include <inlib/osc_env>
#include <inlib/net/inet_socket>

#include <iostream>
#include <cstdlib>

int main(int argc,char** argv){

  if(argc>0) {
    std::string arg0(argv[0]);
    std::vector<std::string> home_dirs;
    home_dirs.push_back("OpenScientist");
    home_dirs.push_back("OnX");
    home_dirs.push_back("session");
    if(!inlib::if_mouse_startup(arg0,"OSC_HOME_DIR",home_dirs,std::cout)) {
      std::cout << "unable to set mouse startup." << std::endl;
      return EXIT_FAILURE;
    }
  }

  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "OnX_server :"
              << " unable to set OnX environment." << std::endl;
    return EXIT_FAILURE;
  }

  inlib::args args(argc,argv);

  int verboseLevel = 0;
 {std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }}

  std::string host;
  if(!inlib::net::inet_host(std::cout,host)){
    std::cout << "can't get host name." << std::endl;
    return EXIT_FAILURE;
  }

  inlib::net::inet_socket* server = 
    new inlib::net::inet_socket(std::cout,verboseLevel);

  unsigned int port;
  if(!args.find<unsigned int>("-port",port)) port = 50800;

  if(!server->bind_accept(host,port,10,1,true,true)) {
    delete server;
    return EXIT_FAILURE;
  }

  //////////////////////////////////////////////////
  /// startup protocol /////////////////////////////
  //////////////////////////////////////////////////
  if(!server->send_string(inlib_net_protocol_hello)) {
    delete server;
    return EXIT_FAILURE;
  }

  std::string driver;
  if(!server->fetch_string(driver)) {
    delete server;
    return EXIT_FAILURE;
  }

  std::string onx_file;
  if(!server->fetch_string(onx_file)) {
    delete server;
    return EXIT_FAILURE;
  }

  //////////////////////////////////////////////////
  //////////////////////////////////////////////////
  //////////////////////////////////////////////////

  Slash::Core::ISession* session = Slash_create_session(args.tovector());
  if(!session) {
    std::cout << "OnX_server :"
              << " session not found." << std::endl;
    delete server;
    return EXIT_FAILURE;
  }

  if(!session->findManager(Slash_NetManager)) {
    Slash::NetManager* netManager =
      new Slash::NetManager(Slash_NetManager,session->cout(),*server);
    session->addManager(netManager);
  }

  Slash::UI::IUI_Manager* uim = Slash::uiManager(*session);
  if(!uim) {
    std::cout << "OnX_server :"
              << " UI_Manager not found." << std::endl;
    delete session;
    delete server;
    return EXIT_FAILURE;
  }

  Slash::UI::IUI* ui = 
    uim->create(driver,onx_file,std::vector<std::string>(),true);
  if(!ui) {
    delete session;
    delete server;
    return EXIT_FAILURE;
  } 

  ui->steer();

 {server->send_string(inlib_net_protocol_exit);
  server->send_string(inlib::to<int>(0));}

  delete session;
  delete server; //Must come after session.

  // Some libraries (for exa Coin, gtk) may have declared 
  // C functions with ::atexit. To ease debugging we call
  // explicitly ::exit here.
  ::exit(EXIT_SUCCESS);

  return EXIT_SUCCESS;
}
