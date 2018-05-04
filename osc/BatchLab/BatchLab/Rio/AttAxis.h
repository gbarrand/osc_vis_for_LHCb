#ifndef BatchLab_Rio_AttAxis_h
#define BatchLab_Rio_AttAxis_h

#include <Rio/Core/Meta.h>

namespace Rio { class IBuffer;}

namespace BatchLab {

namespace Rio {

class AttAxis {
  RIO_META_HEADER(AttAxis)
public:
  virtual bool stream(::Rio::IBuffer&);
public:
  AttAxis(const ::Rio::IDictionary&);
  virtual ~AttAxis();
private:
  int fNdivisions;   //Number of divisions(10000*n3 + 100*n2 + n1)
  short fAxisColor;    //color of the line axis
  short fLabelColor;   //color of labels
  short fLabelFont;    //font for labels
  float fLabelOffset;  //offset of labels
  float fLabelSize;    //size of labels
  float fTickLength;   //length of tick marks
  float fTitleOffset;  //offset of axis title
  float fTitleSize;    //size of axis title
  short fTitleColor;   //color of axis title
  short fTitleFont;    //font for axis title
};

} //Rio

} //BatchLab

#endif
