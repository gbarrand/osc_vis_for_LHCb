// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/args>

#include <inlib/mail_file>

#include <cstdlib>
#include <iostream>

int main(int argc,char* argv[]) {

  inlib::args args(argc,argv);

  std::string file;
  if(!args.file(file)) {
    std::cout << " no file given." << std::endl;
    return EXIT_FAILURE;
  }

  bool verbose = false;
  if(args.is_arg("-verbose")) verbose = true;

  std::string smtp; //exa : smtp.lal.in2p3.fr
  if(!args.find("-smtp",smtp)) {
    std::cout << " no -smtp=<string> given." << std::endl;
    return EXIT_FAILURE;
  }
  std::string from; //exa : user@lal.in2p3.fr
  if(!args.find("-from",from)) {
    std::cout << " no -from=<string> given." << std::endl;
    return EXIT_FAILURE;
  }
  std::string to; //exa : user@lal.in2p3.fr
  if(!args.find("-to",to)) {
    std::cout << " no -to=<string> given." << std::endl;
    return EXIT_FAILURE;
  }

  bool AUTH = true;
  if(args.is_arg("-no_auth")) AUTH = false;
  std::string user;
  args.find("-user",user);
  std::string pass;
  args.find("-pass",pass);

  int port = 0;
  if(!args.find("-port",port)) port = inlib::net::mail::mail_port();

  std::string subject;
  args.find("-subject",subject);
  std::string msg;
  args.find("-msg",msg);

  if(!inlib::mail_file(std::cout,file,
                       smtp,port,
                       from,to,
                       AUTH,user,pass,
                       subject,msg,verbose)) {
    std::cout << "inlib::mail_file failed." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
