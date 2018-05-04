#ifndef Slash_Store_IArray_h
#define Slash_Store_IArray_h

#include <vector>

namespace Slash {

namespace Store {

class IArray {
public: 
  virtual ~IArray() {}
public: 
  typedef std::vector<unsigned int> Orders;
public: 
  virtual Orders orders() const = 0;
  virtual bool write(char*&,char*) const = 0;
  virtual bool read(const Orders&,char*&,char*) = 0;
};

} //Store

} //Slash

#endif
