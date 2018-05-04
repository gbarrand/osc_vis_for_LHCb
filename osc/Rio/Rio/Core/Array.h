#ifndef Rio_Array_h
#define Rio_Array_h

#include <Rio/Core/Meta.h>

#include <vector>

namespace Rio {

class Array {
  RIO_META_HEADER(Array)
public:
  Array(const IDictionary&);
  Array(const Array&);
  virtual ~Array();
  Array& operator=(const Array&);
  int size() const;
  virtual bool stream(IBuffer&);
protected:
  int fN;            //Number of array elements
};

}

#endif
