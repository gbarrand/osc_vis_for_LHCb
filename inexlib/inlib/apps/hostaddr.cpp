// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#ifdef WIN32
#include <io.h>
#include <winsock.h>
typedef unsigned long in_addr_t;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h> //gethostname
#endif

#include <cerrno>
#include <cstring>

#include <sstream>

inline std::string serror() {
   std::ostringstream strm;
#ifdef WIN32
   strm << (int)WSAGetLastError();
   return std::string(" error : ")+strm.str()+".";
#else
   strm << (int)errno;
   return std::string(" error : ")+strm.str()
          + " : " +std::string(strerror(errno))+".";
#endif
}
inline std::string herror() {
   std::ostringstream strm;
#ifdef WIN32
   strm << (int)WSAGetLastError();
   return std::string(" error : ")+strm.str()+".";
#else
   strm << (int)h_errno;
   return std::string(" error : ")+strm.str()
          + " : " +std::string(hstrerror(h_errno))+".";
#endif
}

#include <iostream>
#include <cstdlib>

int main(){

  char host[512];
  if(::gethostname(host,sizeof(host))<0) {
    std::cout << "main :"
              << " gethostname() :" << serror()
              << std::endl;
    return EXIT_FAILURE;
  }
  in_addr_t addr = 0; //int32

  std::cout << "host = \"" << std::string(host) << "\"." << std::endl;

  //perhaps not numeric form.
  hostent* host_p = ::gethostbyname(host);
  if(!host_p) {
    std::cout << "main :"
              << " for " << std::string(host)
              << " gethostbyname() : " << herror()
              << std::endl;
    return EXIT_FAILURE;
  }
  if(host_p->h_length!=sizeof(in_addr_t)) {
    std::cout << "main :"
              << " for " << std::string(host)
              << " gethostbyname() : bad length."
              << std::endl;
    return EXIT_FAILURE;
  }
  ::memcpy((char*)&addr,(char*)(host_p->h_addr),host_p->h_length);

  std::cout << "addr = \"" << ::inet_ntoa(*(struct in_addr*)&addr) << "\"."
            << std::endl;

  return EXIT_SUCCESS;
}
