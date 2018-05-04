#ifndef Slash_Store_ITree_h
#define Slash_Store_ITree_h

#include <string>

namespace Slash {namespace Store {class IObject;}}

namespace Slash {

namespace Store {

class ITree {
public:
  virtual ~ITree() {}
  virtual void* cast(const std::string&) const = 0;
public:
  virtual std::string storeName() const = 0;
  virtual std::string storeType() const = 0;
  virtual bool manageObject(IObject*,const std::string&) = 0;
  virtual void emitUpdate() = 0;
  virtual bool isObjectValid(const IObject*) const = 0;
  virtual IObject* findObject(const std::string&) = 0;
  virtual bool addObject(IObject*,const std::string&) = 0;
};

} //Store

} //Slash

#endif
