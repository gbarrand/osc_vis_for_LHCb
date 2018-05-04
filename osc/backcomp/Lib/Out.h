#ifndef Lib_Out_h
#define Lib_Out_h

// For Panoramix backward compatibility.

#include <Slash/Core/IWriter.h>

#include <string>

#include <Lib/typedefs.h>
#include <cstdio>

#include "sout.h"

namespace Lib {

class Out {
public:
  typedef Out& (*OutFunc)(Out&); //For Lib::endl;
public:
  Out(Slash::Core::IWriter& aWriter):fWriter(aWriter){}
  virtual ~Out(){}
private:
  Out(const Out& aOut):fWriter(aOut.fWriter){}
  Out& operator=(const Out&){return *this;}
public:
  Out& operator<<(const char* aValue) {
    fWriter.write(std::string(aValue));
    return *this;
  }
  Out& operator<<(const std::string& aValue) {
    fWriter.write(aValue);
    return *this;
  }
  Out& operator<<(OutFunc aFunc) {return aFunc(*this);}

  Out& operator<<(bool aValue) {
    if(aValue) fWriter.write("true");
    else       fWriter.write("false");
    return *this;
  }

  Out& operator<<(int aValue) {
    char s[32];
    ::sprintf(s,"%d",aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(float aValue) {
    char s[32];
    ::sprintf(s,"%f",aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(double aValue) {
    char s[32];
    ::sprintf(s,"%g",aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(unsigned int aValue) {
    char s[32];
    ::sprintf(s,"%u",aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(Lib::int64 aValue) {
    char s[32];
    ::sprintf(s,LIB_INT64_FORMAT,aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(Lib::uint64 aValue) {
    char s[32];
    ::sprintf(s,LIB_UINT64_FORMAT,aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(const void* aValue) {
    char s[32];
    ::sprintf(s,"%lx",(unsigned long)aValue);
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(char aValue) {
    char s[32];
    if(  
       ( (aValue>='0')&&(aValue<='9') ) ||
       ( (aValue>='a')&&(aValue<='z') ) ||
       ( (aValue>='A')&&(aValue<='Z') ) 
       ) {
      ::sprintf(s,"%c",aValue);
    } else {
      ::sprintf(s,"%d",aValue);
    }
    fWriter.write(s);
    return *this;
  }
  Out& operator<<(const Lib::sout& aValue) {
    fWriter.write("\"");
    fWriter.write(aValue);
    fWriter.write("\"");
    return *this;
  }
private:
  Slash::Core::IWriter& fWriter;
};

inline Out& dot_endl(Out& aOut) {return aOut << ".\n";}
inline Out& endl(Out& aOut) {return aOut << "\n";}
inline Out& tab(Out& aOut) {return aOut << "\t";}

}

#endif
