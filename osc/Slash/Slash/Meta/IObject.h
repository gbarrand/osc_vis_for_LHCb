#ifndef Slash_Meta_IObject_h
#define Slash_Meta_IObject_h

#include <string>

namespace Slash {

namespace Meta {

class IObject {
public:
  virtual ~IObject() {}
  virtual void* cast(const std::string&) const = 0;
public:
  /** @return The class describer of the IObject. */
  //virtual const IClass& isA() const = 0;
};

} //Meta

} //Slash

#endif
