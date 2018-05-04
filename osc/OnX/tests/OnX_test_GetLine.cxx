#include <OnX/Core/Getline.h>

#include <cstdio>
#include <string>

#define TEST_IS_INPUT

int main(int aArgn,char* argv[]) {
  if(aArgn==1) {

    OnX_Term_Gl_histinit((char*)"OnX_test_GetLine.hist");
    ::printf("type ^D to exit\n");
    while(true) {
      char* p = OnX_Term_Getline("PROMPT>>>> ");
      if(!(*p)) break;
      OnX_Term_Gl_histadd(p);
      std::string s(p);
      std::string cmd = s.substr(0,s.size()-1); //Remove last \n
      ::printf("%s\n",cmd.c_str());
      if(cmd=="exit") break;
    }
  
  } else {

    OnX_Term_Gl_histinit((char*)"OnX_test_GetLine.hist");
    OnX_Term_Getlinem(-1, "GetLine> ");
    while(true) {
#ifdef TEST_IS_INPUT
      int is;
      if(!OnX_Gl_is_there_stdin_input(&is)){
        ::printf("select error\n\n");
      }
      if(is) {
#endif
        char* p = OnX_Term_Getlinem(1,0);
        if(p) {
          if(!(*p)) { //^D
            OnX_Term_Getlinem(-1, "GetLine> ");
            break;
          } else {
            OnX_Term_Gl_histadd(p);
            std::string s(p);
            std::string cmd = s.substr(0,s.size()-1); //Remove last \n
            ::printf("%s\n",cmd.c_str());
            OnX_Term_Getlinem(-1, "GetLine> ");
            if(cmd=="exit") break;
          }
        }
#ifdef TEST_IS_INPUT
      //} else {
      //  ::printf("no input\n");
      }
#endif
    }
    OnX_Term_Gl_erase_line();
    OnX_Term_Getlinem(2, 0);
    printf("Exiting program.\n");  

  }
  return 0;
}


#ifdef TEST_IS_INPUT

#ifdef WIN32
bool is_there_input(bool& aIs) {
  aIs = false;
  return false;
}
#else
#include <unistd.h>
#include <sys/time.h>

#define MAXIMUM(a,b) ((a)>(b)?(a):(b))

bool is_there_input(bool& aIs) {
  int fd_in = fileno(stdin);

  fd_set mask;
  FD_ZERO(&mask);
  FD_SET(fd_in,&mask);

  static struct timeval timeout;
  //timeout.tv_sec = 0;
  timeout.tv_sec = 1;
  timeout.tv_usec = 10; //microsec

  int nfds = 0;
  nfds = MAXIMUM(nfds,fd_in);
  nfds++;
  int i = select(nfds,&mask,0,0,&timeout);
  if(i==(-1)) {
    aIs = false;
    return false;
  }

  aIs = FD_ISSET(fd_in,&mask)?true:false;
  return true;
}

#endif

#endif
