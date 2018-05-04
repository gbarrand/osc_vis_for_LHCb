// this :
#include <Rio/Core/Arrays.h>

// Rio :
#include <Rio/Interfaces/IBuffer.h>


#define RIO_ARRAY_SOURCE(aArray,aType)\
RIO_META_SOURCE(Rio::aArray)\
Rio::aArray::aArray(const IDictionary& aDictionary)\
:Array(aDictionary)\
,fClass(aDictionary.opt_##aArray##_Class())\
,fArray(0)\
{\
}\
Rio::aArray::aArray(\
 const IDictionary& aDictionary\
,const std::vector<aType>& aArray\
)\
:Array(aDictionary)\
,fClass(aDictionary.opt_##aArray##_Class())\
,fArray(0)\
{\
  fN = aArray.size();\
  if(!fN) return;\
  fArray = new aType[fN];\
  for(int index=0;index<fN;index++) fArray[index] = aArray[index];\
}\
Rio::aArray::aArray(\
 const aArray& aFrom\
)\
:Array(aFrom)\
,fClass(aFrom.fClass)\
,fArray(0)\
{\
  fN = aFrom.fN;\
  if(!fN) return;\
  fArray = new aType[fN];\
  for(int index=0;index<fN;index++) fArray[index] = aFrom.fArray[index];\
}\
Rio::aArray::~aArray(){ delete [] fArray;}\
Rio::aArray& Rio::aArray::operator=(\
 const aArray& aFrom\
)\
{\
  delete [] fArray;\
  fArray = 0;\
  fN = aFrom.fN;\
  if(!fN) return *this;\
  fArray = new aType[fN];\
  for(int index=0;index<fN;index++) fArray[index] = aFrom.fArray[index];\
  return *this;\
}\
void Rio::aArray::resize(int aNumber,bool aInit){\
  if(aNumber==fN) return;\
  delete [] fArray;\
  fArray = 0;\
  fN = aNumber;\
  if(!fN) return;\
  fArray = new aType[fN];\
  if(aInit) for(int index=0;index<fN;index++) fArray[index] = 0;\
}\
aType* Rio::aArray::array() const { return fArray;}\
bool Rio::aArray::stream(IBuffer& aBuffer) {\
  if (aBuffer.isReading()) {\
    delete [] fArray;\
    fArray = 0;\
    if(!aBuffer.read(fN)) return false;\
    if(fN) {\
      /*FIXME : check fN*sizeof() against aBuffer max pos.*/\
      fArray = new aType[fN];\
      if(!aBuffer.readFastArray(fArray,fN)) return false;\
    }\
  } else {\
    if(!aBuffer.write(fN)) return false;\
    if(!aBuffer.writeFastArray(fArray,fN)) return false;\
  }\
  return true;\
}

namespace Rio {
RIO_ARRAY_HEADER(Array_int,int)
RIO_ARRAY_HEADER(Array_float,float)
RIO_ARRAY_HEADER(Array_double,double)
}

RIO_ARRAY_SOURCE(Array_int,int)
RIO_ARRAY_SOURCE(Array_float,float)
RIO_ARRAY_SOURCE(Array_double,double)
