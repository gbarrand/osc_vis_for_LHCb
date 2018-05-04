#ifndef Slash_Core_IHierarchy_h
#define Slash_Core_IHierarchy_h

#include <string>

namespace Slash {

namespace Core {

class IHierarchy {
public:
  virtual ~IHierarchy() {};
public:
  virtual std::string hierarchy(const std::string& opts = "") const = 0;
};

} //Core

} //Slash

#endif
