#ifndef Lib_System_h
#define Lib_System_h

#include <cstdlib>

namespace Lib {

class System {
public:
  static int execute(const std::string& aString) {
    return ::system(aString.c_str());
  }
  static bool getenv(const std::string& aString,std::string& aValue) {
    const char* env = ::getenv(aString.c_str());
    if(env) {
      aValue = std::string(env?env:"");
      return true;
    } else {
      aValue = "";
      return false;
    }
  }
  static std::string getenv(const std::string& aString) {
    const char* env = ::getenv(aString.c_str());
    return std::string(env?env:"");
  }
};

}

#endif

