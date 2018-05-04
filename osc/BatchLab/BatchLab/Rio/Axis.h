#ifndef BatchLab_Rio_Axis_h
#define BatchLab_Rio_Axis_h

#include <Rio/Core/Meta.h>
#include <Rio/Core/Named.h>

#include <inlib/histo/axis>

namespace BatchLab {

namespace Rio {

class Axis : public ::Rio::Named {
  RIO_META_HEADER(Axis)
public:
  virtual bool stream(::Rio::IBuffer&);
public:
  Axis(const ::Rio::IDictionary&,inlib::histo::axis<double>&);
  virtual ~Axis();
private:
  inlib::histo::axis<double>& fAxis;
  /*
  int fNbins;          //Number of bins
  double fXmin;           //low edge of first bin
  double fXmax;           //upper edge of last bin
  TArrayD      fXbins;          //Bin edges array in X
  int fFirst;          //first bin to display
  int fLast;           //last bin to display
  unsigned char fTimeDisplay;    //on/off displaying time values instead of numerics
  std::string fTimeFormat;     //Date&time format, ex: 09/12/99 12:34:00
  */
};

} //Rio

} //BatchLab

#endif
