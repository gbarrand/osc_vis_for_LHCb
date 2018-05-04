// this :
#include <Rio/Core/StreamerElement.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <cstdio> //sprintf

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerElement)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerElement::StreamerElement(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,int aType
,const std::string& aTypeName
)
:Named(aDictionary,aName,aTitle)
,fClass(StreamerElement::is(aDictionary))
,fType(aType)
,fSize(0)
,fArrayLength(0)
,fArrayDim(0)
,fOffset(aOffset)
,fTypeName(aTypeName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*FIXME
    fNewType     = fType;
    fStreamer    = 0;
    fMethod      = 0;
  */
  for (int i=0;i<5;i++) fMaxIndex[i] = 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerElement::~StreamerElement(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME delete fMethod;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerElement::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerElement);
  else 
    return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
std::string Rio::StreamerElement::fullName(
) const
//////////////////////////////////////////////////////////////////////////////
// return element name including dimensions, if any
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s = name();
  for (int i=0;i<fArrayDim;i++) {
    char cdim[32];
    ::sprintf(cdim,"[%d]",fMaxIndex[i]);
    s += cdim;
  }
  return s;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerElement::setArrayDimension(
 int aDimension
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fArrayDim = aDimension;
  if(aDimension) fType += Streamer_Info::ARRAY;
  //fNewType = fType;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerElement::setMaxIndex(
 int aDimension
,int aMaximum
)
//////////////////////////////////////////////////////////////////////////////
//set maximum index for array with dimension dim
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aDimension < 0 || aDimension > 4) return;
  fMaxIndex[aDimension] = aMaximum;
  if (fArrayLength == 0)  fArrayLength  = aMaximum;
  else                    fArrayLength *= aMaximum;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerElement::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!Named::stream(aBuffer)) return false;
    if(!aBuffer.read(fType)) return false;
    if(!aBuffer.read(fSize)) return false;
    if(!aBuffer.read(fArrayLength)) return false;
    if(!aBuffer.read(fArrayDim)) return false;
    if(!aBuffer.readFastArray(fMaxIndex,5)) return false;
    if(!aBuffer.read(fTypeName)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!Named::stream(aBuffer)) return false;
    if(!aBuffer.write(fType)) return false;
    if(!aBuffer.write(fSize)) return false;
    if(!aBuffer.write(fArrayLength)) return false;
    if(!aBuffer.write(fArrayDim)) return false;
    if(!aBuffer.writeFastArray(fMaxIndex,5)) return false;
    if(!aBuffer.write(fTypeName)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}

void Rio::StreamerElement::out(std::ostream& aOut) const {
  /*FIXME  sprintf(includeName,GetTypeName());
  if (IsaPointer() && !fTypeName.Contains("*")) strcat(includeName,"*");
  printf("  %-14s%-15s offset=%3d type=%2d %-20s\n",includeName,GetFullName(),fOffset,fType,GetTitle());
  */

  char s[128];
  ::sprintf(s,"  %-14s%-15s offset=%3d type=%2d %-20s",fTypeName.c_str(),fullName().c_str(),fOffset,fType,title().c_str());
  aOut << s << std::endl;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerBase)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBase::StreamerBase(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,int aBaseVersion
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,Streamer_Info::BASE,"BASE")
,fClass(StreamerBase::is(aDictionary))
,fBaseVersion(aBaseVersion)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aName=="TObject") fType = Streamer_Info::TOBJECT;
  if (aName=="TNamed") fType = Streamer_Info::TNAMED;
  /*FIXME
    fNewType = fType;
    fBaseClass = gROOT->GetClass(GetName());
    Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBase::~StreamerBase(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerBase::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerBase);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerBase::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.read(fBaseVersion)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.write(fBaseVersion)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerBasicType)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicType::StreamerBasicType(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,int aType
,const std::string& aTypeName
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,aType,aTypeName)
,fClass(StreamerBasicType::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicType::~StreamerBasicType(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerBasicType::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerBasicType);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerBasicType::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerString)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerString::StreamerString(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,Streamer_Info::TSTRING,"TString")
,fClass(StreamerString::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerString::~StreamerString(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerString::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerString);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerString::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerObject)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObject::StreamerObject(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,const std::string& aTypeName
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,0,aTypeName)
,fClass(StreamerObject::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fType = Streamer_Info::OBJECT;
  if (aName=="TObject") fType = Streamer_Info::TOBJECT;
  if (aName=="TNamed") fType = Streamer_Info::TNAMED;
  /*
  fNewType = fType;
  Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObject::~StreamerObject(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerObject::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerObject);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerObject::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerObjectPointer)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectPointer::StreamerObjectPointer(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,const std::string& aTypeName
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,Streamer_Info::OBJECT_POINTER,aTypeName)
,fClass(StreamerObjectPointer::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aTitle.substr(0,2)=="->") fType = Streamer_Info::OBJECT_ARROW;
  /*FIXME
    fNewType = fType;
    Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectPointer::~StreamerObjectPointer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerObjectPointer::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerObjectPointer);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerObjectPointer::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerBasicPointer)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicPointer::StreamerBasicPointer(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,int aType
,const std::string& aCountName
,const std::string& aCountClass
,int aCountVersion
,const std::string& aTypeName
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,aType,aTypeName)
,fClass(StreamerBasicPointer::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fType += Streamer_Info::POINTER;
  fCountName    = aCountName;
  fCountClass   = aCountClass;
  fCountVersion = aCountVersion;
  //   Init();
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerBasicPointer::~StreamerBasicPointer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerBasicPointer::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerBasicPointer);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerBasicPointer::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.read(fCountVersion)) return false;
    if(!aBuffer.read(fCountName)) return false;
    if(!aBuffer.read(fCountClass)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.write(fCountVersion)) return false;
    if(!aBuffer.write(fCountName)) return false;
    if(!aBuffer.write(fCountClass)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerObjectAny)
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectAny::StreamerObjectAny(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,const std::string& aTypeName
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,Streamer_Info::OBJECT_ANY,aTypeName)
,fClass(StreamerObjectAny::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*FIXME
    Init();
  */
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerObjectAny::~StreamerObjectAny(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerObjectAny::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerObjectAny);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerObjectAny::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(Rio::StreamerSTL)

enum ESTLtype { kSTL       = 300, kSTLstring  =365,   kSTLvector = 1,
                kSTLlist   =  2,  kSTLdeque   =  3,   kSTLmap    = 4,
                kSTLset    =  5,  kSTLmultimap=6,     kSTLmultiset=7};

// Instead of EDataType, we use the Streamer_Info::Type.
//enum EDataType {
//   kChar_t  = 1, kUChar_t  = 11, kShort_t = 2,  kUShort_t = 12,
//   kInt_t   = 3, kUInt_t   = 13, kLong_t  = 4,  kULong_t  = 14,
//   kFloat_t = 5, kDouble_t = 8,  kchar  = 10, kOther_t  = -1
//};

//////////////////////////////////////////////////////////////////////////////
Rio::StreamerSTL::StreamerSTL(
 const IDictionary& aDictionary
,const std::string& aName
,const std::string& aTitle
,int aOffset
,Streamer_Info::Type aType //Must match TDataType/EDataType
,const std::string& aTypeName
)
:StreamerElement(aDictionary,aName,aTitle,aOffset,kSTL,aTypeName)
,fClass(StreamerSTL::is(aDictionary))
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fSTLtype = kSTLvector;
  fCtype   = aType;
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerSTL::~StreamerSTL(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerSTL::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerSTL);
  else 
    return StreamerElement::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerSTL::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.read(fSTLtype)) return false;
    if(!aBuffer.read(fCtype)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!StreamerElement::stream(aBuffer)) return false;
    if(!aBuffer.write(fSTLtype)) return false;
    if(!aBuffer.write(fCtype)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
