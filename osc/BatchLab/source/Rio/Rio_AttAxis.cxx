// this :
#include <BatchLab/Rio/AttAxis.h>

// Rio :
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(BatchLab::Rio::AttAxis)
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::AttAxis::AttAxis(
 const ::Rio::IDictionary& aDictionary
)
:fClass(BatchLab::Rio::AttAxis::is(aDictionary))
,fNdivisions(510)
,fAxisColor(1)
,fLabelColor(1)
,fLabelFont(62)
,fLabelOffset(0.005F)
,fLabelSize(0.04F)
,fTickLength(0.03F)
,fTitleOffset(1)
,fTitleSize(0.04F)
,fTitleColor(1)
,fTitleFont(62)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::AttAxis::~AttAxis(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::AttAxis::stream(
 ::Rio::IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Version 4 streaming (ROOT/v3-00-6).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;

    if(!aBuffer.read(fNdivisions)) return false;
    if(!aBuffer.read(fAxisColor)) return false;
    if(!aBuffer.read(fLabelColor)) return false;
    if(!aBuffer.read(fLabelFont)) return false;
    if(!aBuffer.read(fLabelOffset)) return false;
    if(!aBuffer.read(fLabelSize)) return false;
    if(!aBuffer.read(fTickLength)) return false;
    if(!aBuffer.read(fTitleOffset)) return false;
    if(!aBuffer.read(fTitleSize)) return false;
    if(!aBuffer.read(fTitleColor)) return false;
    if(!aBuffer.read(fTitleFont)) return false;

    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!aBuffer.write(fNdivisions)) return false;
    if(!aBuffer.write(fAxisColor)) return false;
    if(!aBuffer.write(fLabelColor)) return false;
    if(!aBuffer.write(fLabelFont)) return false;
    if(!aBuffer.write(fLabelOffset)) return false;
    if(!aBuffer.write(fLabelSize)) return false;
    if(!aBuffer.write(fTickLength)) return false;
    if(!aBuffer.write(fTitleOffset)) return false;
    if(!aBuffer.write(fTitleSize)) return false;
    if(!aBuffer.write(fTitleColor)) return false;
    if(!aBuffer.write(fTitleFont)) return false;

    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}
