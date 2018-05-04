#ifndef Rio_AttLine_h
#define Rio_AttLine_h

#include <Rio/Core/Meta.h>

namespace Rio {

class IBuffer;

class AttLine {
  RIO_META_HEADER(AttLine)
public:
  virtual bool stream(IBuffer&);
public:
  AttLine(const IDictionary&);
  AttLine(const AttLine&);
  virtual ~AttLine();
  AttLine& operator=(const AttLine&);
private:
  short fLineColor;           //line color
  short fLineStyle;           //line style
  short fLineWidth;           //line width
};

}

#endif
