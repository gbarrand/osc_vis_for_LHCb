#ifndef Rio_Object_h
#define Rio_Object_h

#include <Rio/Interfaces/IObject.h>
#include <Rio/Core/Meta.h>

namespace Rio {

class IDictionary;

class Object : public virtual IObject {
  RIO_META_HEADER(Object)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual const std::string& name() const;
  virtual const std::string& title() const;
  virtual bool stream(IBuffer&);
  virtual void out(std::ostream&) const;
  virtual const IDictionary& dictionary() const;
public:
  Object(const IDictionary&);
  Object(const Object&);
  virtual ~Object();
  Object& operator=(const Object&);
private:
  const IDictionary& fDictionary;
};

}

#endif
