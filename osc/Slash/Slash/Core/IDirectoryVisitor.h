#ifndef Slash_Core_IDirectoryVisitor_h
#define Slash_Core_IDirectoryVisitor_h

#include <string>

namespace Slash {

namespace Core {

class IDirectoryVisitor {
public:
  virtual ~IDirectoryVisitor() {}
public:
  virtual bool directory(const std::string&,bool&) = 0;
  virtual bool file(const std::string&) = 0;
};

} //Core

} //Slash

#endif
