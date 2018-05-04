#ifndef Slash_Store_IObject_h
#define Slash_Store_IObject_h

#include <string>

namespace Slash {namespace Store {class IFolder;}}

namespace Slash {

namespace Store {

class IObject {
public:
  virtual ~IObject() {}
  virtual void* cast(const std::string&) const = 0;
public:
  virtual std::string storeClassName() const = 0;
  virtual std::string name() const = 0;
  virtual void setFolder(IFolder*) = 0;
  virtual IObject* copy() const = 0;
  virtual void setName(const std::string&) = 0;
  virtual std::string title() const = 0; //Used by storage keys.
};

} //Store

} //Slash

#endif
