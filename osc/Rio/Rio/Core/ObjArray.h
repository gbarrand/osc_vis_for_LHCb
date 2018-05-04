#ifndef Rio_ObjArray_h
#define Rio_ObjArray_h

// Inheritance :
#include <Rio/Core/Object.h>
#include <vector>

#include <Rio/Core/Arguments.h>

namespace Rio {

class ObjArray : public Object, public std::vector<IObject*> {
  RIO_META_HEADER(ObjArray)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  ObjArray(const IDictionary&);
  ObjArray(const IDictionary&,int);
  ObjArray(const ObjArray&);
  virtual ~ObjArray();
  ObjArray& operator=(const ObjArray&);
  void setReadArguments(const Arguments&,bool = true);
private:
  Arguments fArgs;
  bool fNoNull;
};

}

#endif
