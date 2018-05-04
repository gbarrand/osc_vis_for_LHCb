#ifndef Rio_Arrays_h
#define Rio_Arrays_h

// Inheritance :
#include <Rio/Core/Array.h>

#include <vector>

namespace Rio {class IBuffer;}

#define RIO_ARRAY_HEADER(aArray,aType)\
class aArray : public Array {\
  RIO_META_HEADER(aArray)\
public:\
  virtual bool stream(IBuffer&);\
public:\
  aArray(const IDictionary&);\
  aArray(const IDictionary&,const std::vector<aType>&);\
  aArray(const aArray&);\
  virtual ~aArray();\
  aArray& operator=(const aArray&);\
  aType* array() const;\
  void resize(int,bool = false);\
private:\
  aType* fArray; /*[fN] Array of fN aTypes*/\
};

#endif
