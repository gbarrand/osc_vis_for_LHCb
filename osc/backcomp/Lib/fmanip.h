#ifndef Lib_fmanip_h
#define Lib_fmanip_h

#include <sys/stat.h>
#include <cstdio>

#include "smanip.h"

namespace Lib {

class fmanip {
public:
  static bool tmpname(std::string& aName,
                      const std::string& aDir,
                      const std::string& aPrefix,
                      const std::string& aSuffix) {
    //  For UNIX aDir must be a string ended with '/'. Exa : /tmp/ .
    for(int count=0;count<1000;count++) {
      std::string name;
      smanip::printf(name,3,"%03d",count);
      std::string tmpnm;
      if(aDir==".") {
        tmpnm += aPrefix;
        tmpnm += name;
        tmpnm += aSuffix;
      } else if(aDir.size()) {
        tmpnm += aDir;
        tmpnm += aPrefix;
        tmpnm += name;
        tmpnm += aSuffix;
      } else {
#ifdef WIN32
        std::string env = System::getenv("HOMEDRIVE");
        std::string drive = (env!="" ? env : std::string("C:"));
        tmpnm += drive;
        tmpnm += "\\";
        tmpnm += aPrefix;
        tmpnm += name;
        tmpnm += aSuffix;
#else
        tmpnm += "/tmp/";
        tmpnm += aPrefix;
        tmpnm += name;
        tmpnm += aSuffix;
#endif
      }
      struct stat finfo;
      if(::stat(tmpnm.c_str(),&finfo) < 0) {
        // No directory or file found :
        aName = tmpnm; // Good file name.
        return true; 
      }
    }
    // No good name found :
    aName = "";
    return false;
  }

  static bool read(const std::string& aName,std::vector<std::string>& aText) {
    aText.clear();
    //FIXME : should read without a limited buffer.
    std::string name = fmanip::name(aName);
    FILE* file = ::fopen(name.c_str(),"rb");
    if(!file) return false;
    //#define BUFSIZE 65536 
    char buffer[65536+1];
    while(true) {
      if(::fgets(buffer,65536,(FILE*)file)==NULL) break; // EOF.
      int l = ::strlen(buffer);
      //  On Windows, editors when saving binary files,
      // put \r\n at place of \n ; we then look for \r\n.
      if( (l>=2) && (buffer[l-2]=='\r') && (buffer[l-1]=='\n') ) {
        buffer[l-2] = '\0';
      } else if( (l>=1) && (buffer[l-1]=='\n') ) {
        buffer[l-1] = '\0';
      }
      aText.push_back(buffer);
    }
    ::fclose(file);
    return true;
  }

  static std::string name(const std::string& aString) {
    std::string fname = aString;
    // Check for env variable in name :
    smanip::getenv(fname);
    return fname;
  }

public: //BaseML
  static bool exists(const std::string& aString) {
    std::string s = name(aString);
    FILE* file = ::fopen(s.c_str(),"rb");
    if(!file) return false;
    ::fclose(file);
    return true;
  }

};
  
}

#endif
