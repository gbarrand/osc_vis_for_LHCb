// this :
#include <Rio/Core/Named.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(Rio::Named)

//////////////////////////////////////////////////////////////////////////////
Rio::Named::Named(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
)
:Object(aDictionary)
,fClass(aDictionary.opt_Named_Class())
,fName(aName)
,fTitle(aTitle)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Named::Named(
 const Named& aFrom
)
:Object(aFrom)
,fClass(aFrom.fClass)
,fName(aFrom.fName)
,fTitle(aFrom.fTitle)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Named::~Named(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Named& Rio::Named::operator=(
 const Named& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Object::operator=(aFrom);
  //fClass
  fName = aFrom.fName;
  fTitle = aFrom.fTitle;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Named::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Named::title(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fTitle;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::Named::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(Named);
  else 
    return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Named::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME if (IsA()->CanIgnoreTObjectStreamer()) return;
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!Object::stream(aBuffer)) return false;
    if(!aBuffer.read(fName)) return false;
    if(!aBuffer.read(fTitle)) return false;
    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!Object::stream(aBuffer)) return false;
    if(!aBuffer.write(fName)) return false;
    if(!aBuffer.write(fTitle)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Named::setName(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fName = aName;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Named::setTitle(
 const std::string& aTitle
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTitle = aTitle;
}
