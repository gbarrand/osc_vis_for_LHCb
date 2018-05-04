#ifndef Rio_AttMarker_h
#define Rio_AttMarker_h

#include <Rio/Core/Meta.h>

namespace Rio {

class IBuffer;

class AttMarker {
  RIO_META_HEADER(AttMarker)
public:
  virtual bool stream(IBuffer&);
public:
  AttMarker(const IDictionary&);
  AttMarker(const AttMarker&);
  virtual ~AttMarker();
  AttMarker& operator=(const AttMarker&);
private:
  short fMarkerColor;           //line color
  short fMarkerStyle;           //line style
  float fMarkerWidth;           //line width
};

}

#endif
