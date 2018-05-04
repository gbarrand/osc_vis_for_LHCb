#ifndef Rio_AttFill_h
#define Rio_AttFill_h

#include <Rio/Core/Meta.h>

namespace Rio {

class IBuffer;

class AttFill {
  RIO_META_HEADER(AttFill)
public:
  virtual bool stream(IBuffer&);
public:
  AttFill(const IDictionary&);
  AttFill(const AttFill&);
  virtual ~AttFill();
  AttFill& operator=(const AttFill&);
private:
  short fFillColor;
  short fFillStyle;
};

}

#endif
