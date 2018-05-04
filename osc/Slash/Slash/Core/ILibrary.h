#ifndef Slash_Core_ILibrary_h
#define Slash_Core_ILibrary_h

#include <string>

namespace Slash {

namespace Core {

class ILibrary {
public:
  virtual ~ILibrary() {};
public:
  typedef void (*Procedure)();
public:
  virtual bool isInitialized() const = 0;
  virtual std::string name() const = 0;
  virtual Procedure findProcedure(const std::string&,bool quiet = false) const = 0;
};

} //Core

} //Slash

#endif
