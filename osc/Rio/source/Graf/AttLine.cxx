// this :
#include <Rio/Graf/AttLine.h>

// Rio :
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(Rio::AttLine)
//////////////////////////////////////////////////////////////////////////////
Rio::AttLine::AttLine(
 const IDictionary& aDictionary
)
:fClass(aDictionary.opt_AttLine_Class())
,fLineColor(1)
,fLineStyle(1)
,fLineWidth(1)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttLine::AttLine(
 const AttLine& aFrom
)
:fClass(aFrom.fClass)
,fLineColor(aFrom.fLineColor)
,fLineStyle(aFrom.fLineStyle)
,fLineWidth(aFrom.fLineWidth)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttLine::~AttLine(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttLine& Rio::AttLine::operator=(
 const AttLine& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fClass
  fLineColor = aFrom.fLineColor;
  fLineStyle = aFrom.fLineStyle;
  fLineWidth = aFrom.fLineWidth;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::AttLine::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;

    if(!aBuffer.read(fLineColor)) return false;
    if(!aBuffer.read(fLineStyle)) return false;
    if(!aBuffer.read(fLineWidth)) return false;

    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!aBuffer.write(fLineColor)) return false;
    if(!aBuffer.write(fLineStyle)) return false;
    if(!aBuffer.write(fLineWidth)) return false;

    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}
