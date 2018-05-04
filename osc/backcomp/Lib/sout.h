#ifndef Lib_sout_h
#define Lib_sout_h

#include <string>

#include <complex>
#include <cstdio>

namespace Lib {

class sout : public std::string {
public:
  sout(const std::string& aString):std::string(aString){}
  sout(int aValue) {
    char s[64];
    ::sprintf(s,"%d",aValue);
    *this = std::string(s);
  }
  sout(double aValue) {
    char s[64];
    ::sprintf(s,"%g",aValue);
    *this = std::string(s);
  }
  sout(const std::complex<double> &aValue) {
    char s[64];
    ::sprintf(s,"(%g,%g)",aValue.real(),aValue.imag());
    *this = std::string(s);
  }
};

}

#endif
