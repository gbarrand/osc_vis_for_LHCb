// this :
#include <Rio/Graf/AttFill.h>

// Rio :
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(Rio::AttFill)
//////////////////////////////////////////////////////////////////////////////
Rio::AttFill::AttFill(
 const IDictionary& aDictionary
)
:fClass(aDictionary.opt_AttFill_Class())
,fFillColor(0)
,fFillStyle(1001)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttFill::AttFill(
 const AttFill& aFrom
)
:fClass(aFrom.fClass)
,fFillColor(aFrom.fFillColor)
,fFillStyle(aFrom.fFillStyle)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttFill::~AttFill(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttFill& Rio::AttFill::operator=(
 const AttFill& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fClass
  fFillColor = aFrom.fFillColor;
  fFillStyle = aFrom.fFillStyle;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::AttFill::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;

    if(!aBuffer.read(fFillColor)) return false;
    if(!aBuffer.read(fFillStyle)) return false;

    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!aBuffer.write(fFillColor)) return false;
    if(!aBuffer.write(fFillStyle)) return false;

    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}
