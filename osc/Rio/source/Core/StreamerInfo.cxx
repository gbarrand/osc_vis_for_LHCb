// this :
#include <Rio/Core/StreamerInfo.h>

// Rio :
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/StreamerElement.h>
#include <Rio/Core/ObjArray.h>
#include <Rio/Core/Arguments.h>

//int Rio::StreamerInfo::fgCount = 0;

RIO_META_SOURCE(Rio::StreamerInfo)

//////////////////////////////////////////////////////////////////////////////
Rio::StreamerInfo::StreamerInfo(
 const IDictionary& aDictionary
,const IClass& aStreamedClass
,const std::string& aTitle
)
:Named(aDictionary,aStreamedClass.inStoreName(),aTitle)
,fClass(StreamerInfo::is(aDictionary))
,fStreamedClass(aStreamedClass)
,fCheckSum(aStreamedClass.checkSum())
,fStreamedClassVersion(aStreamedClass.version())
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fgCount++;  // Then start at 1.
  //fNumber   = fgCount;

  fElements  = new ObjArray(aDictionary);

  /*
  fType     = 0;
  fNewType  = 0;
  fOffset   = 0;
  fLength   = 0;
  fElem     = 0;
  fMethod   = 0;
  fNdata    = 0;
  fOptimized = kFALSE;
  */
 
  //FIXME if (info) BuildUserInfo(info);
}
//////////////////////////////////////////////////////////////////////////////
Rio::StreamerInfo::~StreamerInfo(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if (fNdata) {
    delete [] fType;
    delete [] fNewType;
    delete [] fOffset;
    delete [] fLength;
    delete [] fElem;
    delete [] fMethod;
  }
  */
  if(fElements) {
   {unsigned int sz = fElements->size();
    for(unsigned int index=0;index<sz;index++) delete (*fElements)[index];
    fElements->clear();}
    delete fElements;
  }
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::StreamerInfo::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(StreamerInfo);
  else 
    return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::StreamerInfo::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    /*FIXME
    if (v > 1) {
      TStreamerInfo::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
      return;
      }*/
    //====process old versions before automatic schema evolution
    if(!Named::stream(aBuffer)) return false;
    if(!aBuffer.read(fCheckSum)) return false;
    if(!aBuffer.read(fStreamedClassVersion)) return false;

    delete fElements;
    fElements = 0;

    IObject* obj = 0;
    Arguments args;
    Args_init(args);
    if(!aBuffer.readObject(args,obj)) {
      aBuffer.out() << "Rio::StreamerInfo::stream : can't read object." 
                    << std::endl;
      return false;
    } 

    fElements = obj?Rio::cast<ObjArray>(*obj):0;

    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    //FIXME TStreamerInfo::Class()->WriteBuffer(R__b,this);

    if(!Named::stream(aBuffer)) return false;
    if(!aBuffer.write(fCheckSum)) return false;
    if(!aBuffer.write(fStreamedClassVersion)) return false;

    if(!aBuffer.writeObject(fElements)) return false;

    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::StreamerInfo::add(
 StreamerElement* aElement
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fElements) return;
  fElements->push_back(aElement);
}

void Rio::StreamerInfo::out(std::ostream& aOut) const {
  aOut << std::endl;
  aOut << "StreamerInfo for class: " 
       << name() << ", version=" << fStreamedClassVersion << std::endl;

  if(fElements) {
    std::vector<IObject*>::const_iterator it;
    for(it=fElements->begin();it!=fElements->end();++it) { 
      (*it)->out(aOut);
    }
  }
  /*FIXME
  for (Int_t i=0;i<fNdata;i++) {
    TStreamerElement *element = (TStreamerElement*)fElem[i];
    printf("   i=%2d, %-15s type=%3d, offset=%3d, len=%d, method=%ld\n",i,element->GetName(),fType[i],fOffset[i],fLength[i],fMethod[i]);
  }
  */
}
