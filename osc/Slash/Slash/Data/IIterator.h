#ifndef Slash_Data_IIterator_h
#define Slash_Data_IIterator_h

namespace Slash {

namespace Data {

class IIterator {
public:
  virtual ~IIterator() {}
public:
  virtual void* object() = 0;
  virtual void* tag() = 0;
  virtual void next() = 0;
};

} //Data

}

#endif
