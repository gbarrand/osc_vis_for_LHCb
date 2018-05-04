#ifndef Slash_Core_IFileGetter_h
#define Slash_Core_IFileGetter_h 

#include <string>
#include <vector>

namespace Slash {

namespace Core {

class IFileGetter {
public:
  virtual ~IFileGetter() {}
public:
  typedef std::pair<std::string,std::string> Argument;
  typedef std::vector<Argument> Arguments;
public:
  virtual bool fetch(const std::string&,const Arguments&,std::string&) = 0;
  virtual bool exists(const std::string&,bool&) = 0;
};

} //Core

} //Slash

#endif
