#ifndef Rio_ILeaf_h
#define Rio_ILeaf_h

#include <ostream>

namespace Rio {

class IBuffer;

/**
 * @class ILeaf
 *
 *  ILeaf is the interface to a Rio "leaf".
 *  Internal usage only.
 *
 * @author Guy Barrand
 * @date 2002
 */

class ILeaf {
public:
  virtual ~ILeaf(){}
public:
  virtual int number() const = 0;
  virtual void* valuePointer() const = 0;
  virtual bool fillBasket(IBuffer&) = 0;
  virtual bool readBasket(IBuffer&) = 0;
  virtual bool setAddress(void*) = 0;
  virtual void printValue(std::ostream&,int = 0) const = 0;
};

}

#endif
