// this :
#include <Rio/Graf/AttMarker.h>

// Rio :
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(Rio::AttMarker)
//////////////////////////////////////////////////////////////////////////////
Rio::AttMarker::AttMarker(
 const IDictionary& aDictionary
)
:fClass(aDictionary.opt_AttMarker_Class())
,fMarkerColor(1)
,fMarkerStyle(1)
,fMarkerWidth(1)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttMarker::AttMarker(
 const AttMarker& aFrom
)
:fClass(aFrom.fClass)
,fMarkerColor(aFrom.fMarkerColor)
,fMarkerStyle(aFrom.fMarkerStyle)
,fMarkerWidth(aFrom.fMarkerWidth)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttMarker::~AttMarker(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::AttMarker& Rio::AttMarker::operator=(
 const AttMarker& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fClass
  fMarkerColor = aFrom.fMarkerColor;
  fMarkerStyle = aFrom.fMarkerStyle;
  fMarkerWidth = aFrom.fMarkerWidth;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::AttMarker::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;

    if(!aBuffer.read(fMarkerColor)) return false;
    if(!aBuffer.read(fMarkerStyle)) return false;
    if(!aBuffer.read(fMarkerWidth)) return false;

    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!aBuffer.write(fMarkerColor)) return false;
    if(!aBuffer.write(fMarkerStyle)) return false;
    if(!aBuffer.write(fMarkerWidth)) return false;

    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}
