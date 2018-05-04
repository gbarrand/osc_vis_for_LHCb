#ifndef Rio_realloc_h
#define Rio_realloc_h

#include <string.h>

namespace Rio {

template <class T>
inline T* realloc(T* aPointer,size_t aNewSize,size_t aOldSize)
{
  if (!aPointer) return new T[aNewSize];
  if (aOldSize == aNewSize) return aPointer;
  T* pointer = new T[aNewSize];
  if (!pointer) return 0;
  if (aNewSize > aOldSize) {
    ::memcpy(pointer,aPointer,aOldSize);
    ::memset(((char*)pointer)+aOldSize,0,aNewSize-aOldSize);
  } else {
    ::memcpy(pointer,aPointer,aNewSize);
  }
  delete [] aPointer;
  return pointer;
}

}

#endif
