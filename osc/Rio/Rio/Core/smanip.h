#ifndef Rio_smanip_h
#define Rio_smanip_h

#include <string>
#include <vector>

namespace Rio {

class smanip {
public:
  enum what { leading, trailing, both };
  static void strip(std::string&,what = both,char = ' ');
  static bool toint(const std::string&,int&);
  static std::string tostringx(unsigned int);
  static void words(const std::string&,const std::string&,
                    std::vector<std::string>&);
  static bool pathNameSuffix(const std::string&,
                             std::string&,std::string&,std::string&);
  static bool match(const std::string&,const std::string&);
  static bool printf(std::string&,int,const char*,...);
};

}

#endif
