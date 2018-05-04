#ifndef Slash_Store_IObjectIterator_h
#define Slash_Store_IObjectIterator_h

namespace Slash {namespace Store {class IObject;}}

namespace Slash {

namespace Store {

class IObjectIterator {
public:
  virtual ~IObjectIterator() {}
  virtual IObject* object() = 0;
  virtual void next() = 0;
};

} //Store

} //Slash

#endif
