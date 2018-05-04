#ifndef Slash_Data_IHandler_h
#define Slash_Data_IHandler_h

namespace Slash {namespace Data { class IAccessor;}}

namespace Slash {

namespace Data {

class IHandler {
public:
  virtual ~IHandler(){}
public:
  virtual IAccessor* type() const = 0;
  virtual void* object() const = 0;
  virtual void* tag() const = 0;
  virtual bool isEqual(const IHandler&) const = 0;
};

} //Data

} //Slash

#endif



