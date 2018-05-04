//////////////////////////////////////////////////////////////////////////
// Buffer base class used for serializing objects.                      //
//////////////////////////////////////////////////////////////////////////

// this :
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Platform.h>

#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Core/Lib.h>
//#include <Rio/Core/Map.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/realloc.h>
#include <Rio/Core/smanip.h>

#include "tobuf.h"

#include <string.h> //memcpy

#define Rio_assert(e) \
  if (!(e)) {\
    fOut << "Rio_assert : " << #e << " violated at line " \
         << __LINE__ << " of \"" << __FILE__ << "\"." << std::endl;	\
    return;\
  }

#define Rio_assert_return(e,r) \
  if (!(e)) {\
    fOut << "Rio_assert : " << #e << " violated at line " \
         << __LINE__ << " of \"" << __FILE__ << "\"." << std::endl;	\
    return r;\
  }

#define RIO_CHECK_EOB(n) \
  if((fBufCur+n)>fBufMax) {\
    fOut << "Rio::Buffer::[read,write] :"\
         << " try to access out of buffer " << n << " bytes." << std::endl;\
    return false;}

#define MAXIMUM(a,b) ((a)>(b)?(a):(b)) 
#define MINIMUM(a,b) ((a)<(b)?(a):(b)) 

const unsigned int Rio_kNullTag = 0;
const int Rio_kExtraSpace = 8;
const int Rio_kMaxInt = 2147483647;       
const unsigned int Rio_kByteCountMask = 0x40000000; 
const short Rio_kByteCountVMask = 0x4000;
const short Rio_kMaxVersion = 0x3FFF;
// last valid fMapCount and byte count
const unsigned int Rio_kMaxMapCount = 0x3FFFFFFE;  
const unsigned int Rio_kNewClassTag = 0xFFFFFFFF;
// OR the class index with this
const unsigned int Rio_kClassMask = 0x80000000;  

// first 2 map entries are taken by null obj and self obj :
const int Rio_kMapOffset = 2;   
const int Rio_kMapSize = 503;

//#define BUFFER_VERBOSE 1
#define BUFFER_VERBOSE 0

//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::Buffer(
 const Buffer& aBuffer
)
:IBuffer()
,fDictionary(aBuffer.fDictionary)
,fOut(aBuffer.fOut)
,fVerboseLevel(BUFFER_VERBOSE)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Buffer");
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::operator=(
 const Buffer&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::Buffer(
 const IDictionary& aDictionary
,std::ostream& aOut
,Mode aMode
,int aBufferSize
,char* aBuffer
)
:fDictionary(aDictionary)
,fOut(aOut)
,fVerboseLevel(BUFFER_VERBOSE)
,fMode(aMode)
,fVersion(0)
,fBufSize(0)
,fBuffer(0)
,fBufCur(0)
,fBufMax(0)
,fMapCount(0)
,fMapSize(Rio_kMapSize)
,fDisplacement(0)
,fReadMap(0)
//////////////////////////////////////////////////////////////////////////////
// Create an I/O buffer object. Mode should be either TBuffer::kRead or
// TBuffer::kWrite. By default the I/O buffer has a size of
// TBuffer::kInitialSize (1024) bytes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Buffer");
  // Before using the buffer make sure some assumptions are true
  Rio_assert(sizeof(short) == 2);
  Rio_assert(sizeof(int) == 4);
// commented lines below in view of support for longlong on 32 bits machines
//#ifdef R__B64
//   Rio_assert(sizeof(long) == 8);
//#else
//   Rio_assert(sizeof(long) == 4);
//#endif
  Rio_assert(sizeof(float) == 4);
  Rio_assert(sizeof(double) == 8);

  if (aBuffer) {
    fBufSize  = aBufferSize;
    fBuffer = aBuffer;
  } else {
    if(fBuffer) {
      fOut << "Rio::Buffer::Buffer : new char anomaly." << std::endl;
    }
    fBufSize  = (aBufferSize < MINIMAL) ? MINIMAL : aBufferSize;   
    fBuffer = new char[fBufSize + Rio_kExtraSpace];
    if(!fBuffer) {
      fOut << "Rio::Buffer::Buffer : can't alloc " 
	   << fBufSize + Rio_kExtraSpace << " bytes." << std::endl;
      fBufSize = 0;
    }
  }

  fBufCur = fBuffer;
  fBufMax = fBuffer + fBufSize;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::~Buffer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete [] fBuffer;
  if(inline_isReading()) delete fReadMap;
  else delete fWriteMap;
  fReadMap = 0;
  f_cls_mapped.clear(); //GB
  f_obj_mapped.clear(); //GB
  Debug::decrement("Rio::Buffer");
}

std::ostream& Rio::Buffer::out() const {return fOut;}

//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::Mode Rio::Buffer::mode(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fMode; 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::isReading(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return inline_isReading();
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::isWriting(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return inline_isWriting();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setBuffer(
 void* aBuffer
,unsigned int aNewSize
)
//////////////////////////////////////////////////////////////////////////////
// Set buffer address
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBuffer = (char*)aBuffer;
  fBufCur = fBuffer;
  if (aNewSize > 0) fBufSize = aNewSize;
  fBufMax = fBuffer + fBufSize;
  //GB : ?
  //f_cls_mapped.clear();
  //f_obj_mapped.clear();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setReadMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set buffer in read mode.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fMode = READ;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setWriteMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set buffer in write mode.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fMode = WRITE;
}
//////////////////////////////////////////////////////////////////////////////
char* Rio::Buffer::buffer(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fBuffer;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::displacement(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fDisplacement;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::bufferSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fBufSize;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::length(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return inline_length();
}
//////////////////////////////////////////////////////////////////////////////
char*& Rio::Buffer::currentPosition(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBufCur;
}
//////////////////////////////////////////////////////////////////////////////
char* Rio::Buffer::maximumPosition(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBufMax;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setBufferOffset(
 int aOffset
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  inline_setBufferOffset(aOffset);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::expand(
 int aNewSize
)
//////////////////////////////////////////////////////////////////////////////
// Expand the I/O buffer to newsize bytes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int len = inline_length();

  fBuffer = Rio::realloc<char>(fBuffer,
                               (aNewSize+Rio_kExtraSpace)*sizeof(char),
                               (fBufSize+Rio_kExtraSpace)*sizeof(char));
  if(!fBuffer) {
    fOut << "Rio::Buffer::expand : can't realloc "
         << (aNewSize+Rio_kExtraSpace) << " bytes." << std::endl;
    fBufSize = 0;
    fBufCur = 0;
    fBufMax = 0;
    return false;
  }
  fBufSize = aNewSize;
  fBufCur  = fBuffer + len;
  fBufMax  = fBuffer + fBufSize;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::read(unsigned char& x){
  return frombuf(fOut,fBufMax,fBufCur,&x);
}
bool Rio::Buffer::read(unsigned short& x){
  return frombuf(fOut,fBufMax,fBufCur,&x);
}
bool Rio::Buffer::read(unsigned int& x){
  return frombuf(fOut,fBufMax,fBufCur,&x);
}
bool Rio::Buffer::read(uint64& x){
  return frombuf(fOut,fBufMax,fBufCur,&x);
}
bool Rio::Buffer::read(float& x){
  return frombuf(fOut,fBufMax,fBufCur,&x);
}
bool Rio::Buffer::read(double& x){
  return frombuf(fOut,fBufMax,fBufCur,&x);
}
bool Rio::Buffer::read(char& x){ 
  unsigned char uc = 0;
  bool status = frombuf(fOut,fBufMax,fBufCur,&uc); 
  x = (char)uc;
  return status;
}
bool Rio::Buffer::read(bool& x){ 
  unsigned char uc = 0;
  bool status = frombuf(fOut,fBufMax,fBufCur,&uc); 
  x = uc?true:false;
  return status;
}
bool Rio::Buffer::read(short& x){ 
  return frombuf(fOut,fBufMax,fBufCur,&x); 
}
bool Rio::Buffer::read(int& x){ 
  return frombuf(fOut,fBufMax,fBufCur,&x); 
}
bool Rio::Buffer::read(int64& x){ 
  return frombuf(fOut,fBufMax,fBufCur,&x); 
}
bool Rio::Buffer::read(std::string& x) {
  return frombuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::read(std::vector<std::string>& aArray) {
  int n;
  if(!read(n)) return false;
  aArray.resize(n);
  for(int index=0;index<n;index++) {
    if(!read(aArray[index])) return false;
  }
  return true;
}
bool Rio::Buffer::write(unsigned char x) {
  if (fBufCur + sizeof(unsigned char) > fBufMax) 
    if(!expand(2*fBufSize)) return false;
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(unsigned short x){
  if (fBufCur + sizeof(unsigned short) > fBufMax) 
    if(!expand(2*fBufSize)) return false;
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(unsigned int x){
  if (fBufCur + sizeof(unsigned int) > fBufMax) 
    if(!expand(2*fBufSize)) return false;
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(uint64 x){
  if (fBufCur + sizeof(uint64) > fBufMax) 
    if(!expand(2*fBufSize)) return false;
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(float x){
  if (fBufCur + sizeof(float) > fBufMax) 
    if(!expand(2*fBufSize)) return false;
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(double x) {
  if (fBufCur + sizeof(double) > fBufMax) 
    if(!expand(2*fBufSize)) return false;
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(char x){ 
  return write((unsigned char)x); 
}
bool Rio::Buffer::write(bool x){ 
  return write((unsigned char)x); 
}
bool Rio::Buffer::write(short x){ 
  return write((unsigned short)x);
}
bool Rio::Buffer::write(int x){ 
  return write((unsigned int)x); 
}
bool Rio::Buffer::write(int64 x){ 
  return write((uint64)x); 
}
bool Rio::Buffer::write(const std::string& x) {
  if ((fBufCur + x.size() + sizeof(int) + 1)> fBufMax) {
    int sz = (int)x.size() + (int)sizeof(int) + 1;
    if(!expand(MAXIMUM(2*fBufSize,fBufSize+sz))) return false;
  }
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(const char* x) {
  size_t lx = ::strlen(x);
  if ((fBufCur + lx + sizeof(int) + 1)> fBufMax) {
    int sz = (int)lx + (int)sizeof(int) + 1;
    if(!expand(MAXIMUM(2*fBufSize,fBufSize+sz))) return false;
  }
  return tobuf(fOut,fBufMax,fBufCur,x);
}
bool Rio::Buffer::write(const std::vector<std::string>& aArray) {
  if(!write((int)aArray.size())) return false;
  for(unsigned int index=0;index<aArray.size();index++) {
    if(!write(aArray[index])) return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 unsigned char*& c
,int& n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);

  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;

  int l = n * sizeof(unsigned char);
  RIO_CHECK_EOB(l);

  if (!c) {
    c = new unsigned char[n];
    if(!c) return false;
  }
  
  ::memcpy(c, fBufCur, l);
  fBufCur += l;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 char*& c
,int& n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of characters from the I/O buffer. Returns the number of
// characters read. If argument is a 0 pointer then space will be
// allocated for the array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);

  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;

  int l = n * sizeof(char);
  RIO_CHECK_EOB(l);

  if (!c) {
    c = new char[n];
    if(!c) return false;
  }
  
  ::memcpy(c, fBufCur, l);
  fBufCur += l;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 short*& ii
,int& n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of shorts from the I/O buffer. Returns the number of shorts
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);
  
  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;
  
  int l = n * sizeof(short);
  RIO_CHECK_EOB(l);

  if (!ii) {
    ii = new short[n];
    if(!ii) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 int*& ii
,int& n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of ints from the I/O buffer. Returns the number of ints
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);
  
  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;
  
  int l = n * sizeof(int);
  RIO_CHECK_EOB(l);

  if (!ii) {
    ii = new int[n];
    if(!ii) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 unsigned int*& ii
,int& n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of ints from the I/O buffer. Returns the number of ints
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);
  
  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;
  
  int l = n * sizeof(unsigned int);
  RIO_CHECK_EOB(l);

  if (!ii) {
    ii = new unsigned int[n];
    if(!ii) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 int64*& ii
,int& n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);
  
  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;
  
  int l = n * sizeof(int64);
  RIO_CHECK_EOB(l);

  if (!ii) {
    ii = new int64[n];
    if(!ii) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 uint64*& ii
,int& n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);
  
  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;
  
  int l = n * sizeof(uint64);
  RIO_CHECK_EOB(l);

  if (!ii) {
    ii = new uint64[n];
    if(!ii) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 float*& f
,int& n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of floats from the I/O buffer. Returns the number of floats
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);

  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;

  int l = n * sizeof(float);
  RIO_CHECK_EOB(l);

  if (!f) {
    f = new float[n];
    if(!f) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &f[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(f, fBufCur, l);
  fBufCur += l;
#endif

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readArray(
 double*& d
,int& n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of doubles from the I/O buffer. Returns the number of doubles
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  n = 0;
  Rio_assert_return(inline_isReading(),false);

  if(fBufCur==fBufMax) {n = 0;return true;}
  if(!read(n)) {n = 0;return false;}
  if(!n) return true;

  int l = n * sizeof(double);
  RIO_CHECK_EOB(l);

  if (!d) {
    d = new double[n];
    if(!d) return false;
  }

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &d[i])) {
      n = 0;
      return false;
    }
  }
#else
  ::memcpy(d, fBufCur, l);
  fBufCur += l;
#endif

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const char* cs
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n chars into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);

  if(!write(n)) return false;

  if (!n) return true;

  Rio_assert_return(cs,false);

  int l = n * sizeof(char);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

  ::memcpy(fBufCur, cs, l);
  fBufCur += l;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const unsigned char* cs
,int n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);

  if(!write(n)) return false;

  if (!n) return true;

  Rio_assert_return(cs,false);

  int l = n * sizeof(unsigned char);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

  ::memcpy(fBufCur, cs, l);
  fBufCur += l;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const short* ii
,int n
) 
//////////////////////////////////////////////////////////////////////////////
// Write array of n short into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);
  
  if(!write(n)) return false;
  
  if (!n) return true;
  
  Rio_assert_return(ii,false);

  int l = n * sizeof(short);
  if (fBufCur + l > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const int* ii
,int n
) 
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);
  
  if(!write(n)) return false;
  
  if (!n) return true;
  
  Rio_assert_return(ii,false);

  int l = n * sizeof(int);
  if (fBufCur + l > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const unsigned int* ii
,int n
) 
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);
  
  if(!write(n)) return false;
  
  if (!n) return true;
  
  Rio_assert_return(ii,false);

  int l = n * sizeof(unsigned int);
  if (fBufCur + l > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const int64* ii
,int n
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);
  
  if(!write(n)) return false;
  
  if (!n) return true;
  
  Rio_assert_return(ii,false);

  int l = n * sizeof(int64);
  if (fBufCur + l > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const uint64* ii
,int n
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);
  
  if(!write(n)) return false;
  
  if (!n) return true;
  
  Rio_assert_return(ii,false);

  int l = n * sizeof(uint64);
  if (fBufCur + l > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const float* f
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n floats into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);

  if(!write(n)) return false;

  if (!n) return true;

  Rio_assert_return(f,false);

  int l = n * sizeof(float);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, f[i])) return false;
  }
#else
  ::memcpy(fBufCur, f, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeArray(
 const double* d
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n doubles into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);

  if(!write(n)) return false;

  if (!n) return true;

  Rio_assert_return(d,false);

  int l = n * sizeof(double);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, d[i])) return false;
  }
#else
  ::memcpy(fBufCur, d, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 bool* b
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n characters from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;
  int l = n * sizeof(unsigned char);
  RIO_CHECK_EOB(l);
  for (int i = 0; i < n; i++) {
    unsigned char uc;
    if(!frombuf(fOut,fBufMax,fBufCur,&uc)) return false;
    b[i] = uc;
  }
  return true;
}
/*
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 std::string* a
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n std::string from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur,a[i])) return false;
  }
  return true;
}
*/
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 char* c
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n characters from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;
  int l = n * sizeof(char);
  RIO_CHECK_EOB(l);
  ::memcpy(c, fBufCur, l);
  fBufCur += l;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 unsigned char* c
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n characters from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;
  int l = n * sizeof(unsigned char);
  RIO_CHECK_EOB(l);
  ::memcpy(c, fBufCur, l);
  fBufCur += l;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 int* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n ints from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(int);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) return false;
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 unsigned int* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n ints from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(unsigned int);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) return false;
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 int64* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(int64);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) return false;
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 uint64* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n ints from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(uint64);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &ii[i])) return false;
  }
#else
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 float* f
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n floats from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(float);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &f[i])) return false;
  }
#else
  ::memcpy(f, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 double* d
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n doubles from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(double);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &d[i])) return false;
  }
#else
  ::memcpy(d, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 short* s
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n shorts from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(short);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &s[i])) return false;
  }
#else
  ::memcpy(s, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readFastArray(
 unsigned short* s
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n shorts from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(unsigned short);
  RIO_CHECK_EOB(l);

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!frombuf(fOut,fBufMax,fBufCur, &s[i])) return false;
  }
#else
  ::memcpy(s, fBufCur, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const bool* b
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n bool into the I/O buffer.
// A bool is mapped on an unsigned char (to follow ROOT).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;
  int l = n * sizeof(unsigned char);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

  for (int i = 0; i < n; i++) {
    unsigned char uc = b[i];
    if(!tobuf(fOut,fBufMax,fBufCur,uc)) return false;
  }

  return true;
}
/*
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const std::string* a
,int n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  for (int i = 0; i < n; i++) {
    int l = (a[i].size()+1) * sizeof(char);
    if ((fBufCur + l) > fBufMax) 
      if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
    if(!tobuf(fOut,fBufMax,fBufCur,a[i])) return false;
  }
  return true;
}
*/
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const char* c
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n characters into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;
  int l = n * sizeof(char);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  ::memcpy(fBufCur, c, l);
  fBufCur += l;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const unsigned char* c
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n characters into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;
  int l = n * sizeof(unsigned char);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  ::memcpy(fBufCur, c, l);
  fBufCur += l;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const int* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(int);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const unsigned int* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(unsigned int);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const int64* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(int64);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const uint64* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(uint64);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, ii[i])) return false;
  }
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const float* f
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n floats into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(float);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, f[i])) return false;
  }
#else
  ::memcpy(fBufCur, f, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const double* d
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n doubles into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(double);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, d[i])) return false;
  }
#else
  ::memcpy(fBufCur, d, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const short* s
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n shorts into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(short);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, s[i])) return false;
  }
#else
  ::memcpy(fBufCur, s, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeFastArray(
 const unsigned short* s
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n shorts into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return true;

  int l = n * sizeof(unsigned short);
  if ((fBufCur + l) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+l))) return false;
  
#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++) {
    if(!tobuf(fOut,fBufMax,fBufCur, s[i])) return false;
  }
#else
  ::memcpy(fBufCur, s, l);
  fBufCur += l;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
char* Rio::Buffer::readString(
 char* aString
,int aMax
)
//////////////////////////////////////////////////////////////////////////////
// Read string from I/O buffer. String is read till 0 character is
// found or till max-1 characters are read (i.e. string s has max
// bytes allocated). If max = -1 no check on number of character is
// made, reading continues till 0 character is found.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inline_isReading()) return 0;
  if (aMax == -1) aMax = Rio_kMaxInt;
  char  ch;
  int nr = 0;
  while (nr < aMax-1) {
    if(!read(ch)) return 0;
    // stop when 0 read
    if (ch == 0) break;
    aString[nr++] = ch;
  }
  aString[nr] = 0;
  return aString;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeString(
 const char* aString
)
//////////////////////////////////////////////////////////////////////////////
// Write string to I/O buffer. Writes string upto and including the
// terminating 0.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return writeBuffer(aString,(::strlen(aString)+1)*sizeof(char));
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeBuffer(
 const void* aBuffer
,int aMax
)
//////////////////////////////////////////////////////////////////////////////
// Write max bytes from buf into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inline_isWriting()) return false;
  if (aMax <= 0) return true;
  if ((fBufCur + aMax) > fBufMax) 
    if(!expand(MAXIMUM(2*fBufSize, fBufSize+aMax))) return false;
  ::memcpy(fBufCur,aBuffer,aMax);
  fBufCur += aMax;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readVersion(
 short& aVersion
)
//////////////////////////////////////////////////////////////////////////////
// Read class version from I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aVersion = 0;
  short version = 0;
  // not interested in byte count
  if(!read(version)) return false;
    
  // if this is a byte count, then skip next short and read version
  if (version & Rio_kByteCountVMask) {
    if(!read(version)) return false;
    if(!read(version)) return false;
  }
  
  aVersion = version;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readVersion(
 short& aVersion
,unsigned int& aStartPosition
,unsigned int& aByteCount
)
//////////////////////////////////////////////////////////////////////////////
// Read class version from I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aVersion = 0;
  aStartPosition = 0;
  aByteCount = 0;

  short version = 0;

  // before reading object save start position
  //GB : the below should be unsigned long.
  unsigned int startpos = (unsigned int)(fBufCur-fBuffer);
    
  // read byte count (older files don't have byte count)
  // byte count is packed in two individual shorts, this to be
  // backward compatible with old files that have at this location
  // only a single short (i.e. the version)
  union {
    unsigned int cnt;
    short vers[2];
  } v;
#ifdef RIO_BYTESWAP
  if(!read(v.vers[1])) return false;
  if(!read(v.vers[0])) return false;
#else
  if(!read(v.vers[0])) return false;
  if(!read(v.vers[1])) return false;
#endif
    
  // no bytecount, backup and read version
  if (!(v.cnt & Rio_kByteCountMask)) {
    fBufCur -= sizeof(unsigned int);
    v.cnt = 0;
  }
  unsigned int bcnt = (v.cnt & ~Rio_kByteCountMask);
  if(!read(version)) return false;
  //printf("Reading version=%d at pos=%d, bytecount=%d\n",
  //version,*startpos,*bcnt);
  
  aVersion = version;
  aStartPosition = startpos;
  aByteCount = bcnt;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeVersion(
 short aVersion
)
//////////////////////////////////////////////////////////////////////////////
// Write class version to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aVersion > Rio_kMaxVersion) {
    fOut << "Rio::Buffer::writeVersion : "
         << "version number cannot be larger than " << Rio_kMaxVersion 
         << std::endl;
    aVersion = Rio_kMaxVersion;
  }
  return write(aVersion);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeVersion(
 short aVersion
,unsigned int& aPos
)
//////////////////////////////////////////////////////////////////////////////
// Write class version to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // reserve space for leading byte count
  aPos = (unsigned int)(fBufCur-fBuffer);
  fBufCur += sizeof(unsigned int);
  if (aVersion > Rio_kMaxVersion) {
    fOut << "Rio::Buffer::writeVersion : "
         << "version number cannot be larger than " << Rio_kMaxVersion 
         << std::endl;
    aVersion = Rio_kMaxVersion;
  }
  return write(aVersion);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::setByteCount(
 unsigned int aCountPos
,bool aPackInVersion
)
//////////////////////////////////////////////////////////////////////////////
// Set byte count at position cntpos in the buffer. Generate warning if
// count larger than kMaxMapCount. The count is excluded its own size.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int cnt = 
    (unsigned int)(fBufCur - fBuffer) - aCountPos - sizeof(unsigned int);
  char  *buf = (char *)(fBuffer + aCountPos);

  // if true, pack byte count in two consecutive shorts, so it can
  // be read by ReadVersion()
  if (aPackInVersion) {
    union {
      unsigned int cnt;
      short vers[2];
    } v;
    v.cnt = cnt;
#ifdef RIO_BYTESWAP
    if(!tobuf(fOut,fBufMax,buf, short(v.vers[1] | Rio_kByteCountVMask))) 
      return false;
    if(!tobuf(fOut,fBufMax,buf, v.vers[0])) return false;
#else
    if(!tobuf(fOut,fBufMax,buf, short(v.vers[0] | Rio_kByteCountVMask))) 
      return false;
    if(!tobuf(fOut,fBufMax,buf, v.vers[1])) return false;
#endif
  } else {
    if(!tobuf(fOut,fBufMax,buf, cnt | Rio_kByteCountMask)) return false;
  }

  if (cnt >= Rio_kMaxMapCount) {
    fOut << "Rio::Buffer::setByteCount : bytecount too large (more than "
         << Rio_kMaxMapCount << ")." << std::endl;
    // exception
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::checkByteCount(
 unsigned int aStartPos
,unsigned int aByteCount
,const std::string& aClassName
)
//////////////////////////////////////////////////////////////////////////////
// Check byte count with current buffer position. They should
// match. If not print warning and position buffer in correct
// place determined by the byte count. Startpos is position of
// first byte where the byte count is written in buffer.
// Returns 0 if everything is ok, otherwise the bytecount offset
// (< 0 when read too little, >0 when read too much).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aByteCount) return true;

  unsigned long len = aStartPos + aByteCount + sizeof(unsigned int);

  unsigned long diff = fBufCur-fBuffer;  

  if(diff==len) return true;

  if(diff<len) {
    fOut << "Rio::Buffer::checkByteCount : object of class \""
         << aClassName << "\" read too few bytes (" 
         << long_tos(len-diff) << " missing)." << std::endl;
  }
  if(diff>len) {
    fOut << "Rio::Buffer::checkByteCount : object of class \""
         << aClassName << "\" read too many bytes ("
         << long_tos(diff-len) << " in excess)." << std::endl;
  }
  fOut << "Rio::Buffer::checkByteCount : \"" << aClassName 
       << "\"::Streamer() not in sync with data on file, fix Streamer()"
       << std::endl;
    
  //int offset = int(long(fBufCur) - endpos);
    
  fBufCur = fBuffer+len;

  return false;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::checkCount(
 unsigned int aOffset
)
//////////////////////////////////////////////////////////////////////////////
// Check if offset is not too large (< kMaxMapCount) when writing.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (inline_isWriting()) {
    if (aOffset >= Rio_kMaxMapCount) {
      fOut << "Rio::Buffer::checkCount : " 
           << "buffer offset too large (larger than " 
           << Rio_kMaxMapCount << ")." << std::endl;
      // exception
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::initMap(
)
//////////////////////////////////////////////////////////////////////////////
// Create the fWriteMap or fReadMap containers and initialize them
// with the null object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (inline_isWriting()) {
    if (!fWriteMap) {
      fWriteMap = new Map(fOut,fMapSize);
      fMapCount = 0;
    }
  } else {
    if (!fReadMap) {
      fReadMap = new Map(fOut,fMapSize);
      // put kNullTag in slot 0
      if(!fReadMap->add(0,Rio_kNullTag)) return false;
      fMapCount = 1;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::mapObject(
 const void* aObject
,unsigned int aOffset
)
//////////////////////////////////////////////////////////////////////////////
// Add object to the fWriteMap or fReadMap containers (depending on the mode).
// If obj is not 0 add object to the map (in read mode also add 0 objects to
// the map). This method may only be called outside this class just before
// calling obj->Streamer() to prevent self reference of obj, in case obj
// contains (via via) a pointer to itself. In that case offset must be 1
// (default value for offset).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fVerboseLevel) {
    fOut << "Rio::Buffer::mapObject :"
         << " " << this << " :"
         << " obj " << aObject 
         << " offset : " << aOffset << std::endl;
  }

  if (inline_isWriting()) {
    if (!fWriteMap) if(!initMap()) return false;
    if (aObject) {
      checkCount(aOffset);
      const void* ptr = aObject;
      if(!fWriteMap->add(hash(&ptr,sizeof(void*)),(long)aObject,aOffset))
        return false;
      fMapCount++;
    }
  } else {
    if (!fReadMap) if(!initMap()) return false;
    if(!fReadMap->add(aOffset,(long)aObject)) return false;
    fMapCount++;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeClass(
 const IClass& aClass
)
//////////////////////////////////////////////////////////////////////////////
// Write class description to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);

  const void* ptr = &aClass;
  unsigned long idx;

  if ((idx = (unsigned long)
              fWriteMap->findValue(hash(&ptr,sizeof(void*)),(long)&aClass)
              ) != 0) {
    
    // truncation is OK the value we did put in the map is an 30-bit offset
    // and not a pointer
    unsigned int clIdx = (unsigned int)idx;
    
    unsigned int offset = (unsigned int)(fBufCur-fBuffer); //GB

    // save index of already stored class
    if(!write((clIdx | Rio_kClassMask))) return false;

    f_cls_mapped.push_back(std::pair<bufpos,bufpos>(offset,clIdx));

  } else {

    // offset in buffer where class info is written
    unsigned int offset = (unsigned int)(fBufCur-fBuffer);
    
    // save new class tag
    if(!write(Rio_kNewClassTag)) return false;
    
    // write class name
    //cl->Store(*this);
    
    if(!writeString(aClass.inStoreName().c_str())) return false;
    
    // store new class reference in fWriteMap (+Rio_kMapOffset so it's != kNullTag)
    if(!mapObject(&aClass, offset + Rio_kMapOffset)) return false;

  }
  return true;
}

bool Rio::Buffer::checkClass(unsigned int aOffset,const IClass* aClass){
  // in position 0 we always have the reference to the null object
  if (!aOffset) return true;

  long cli = fReadMap->findValue(aOffset);

  if (cli==0) {
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::checkClass :"
           << " " << this << " :"
           << " no class found in map for offset " 
           << aOffset << ". Attempt to read class..."
           << std::endl; 
    }

    // No class found at this location in map. It might have been skipped
    // as part of a skipped object. Try to explicitely read the class.
    // save fBufCur and set to place specified 
    // by aOffset (-kMapOffset-sizeof(bytecount))
    char* bufsav = fBufCur;
    fBufCur = 
      (char*)(fBuffer + aOffset - Rio_kMapOffset-sizeof(unsigned int));
    
    unsigned int tag;
    IClass* c;
    if(!readClass(aClass,tag,c)) {
      fOut << "Rio::Buffer::checkClass : " << "can't read class. " 
           << std::endl;
      fBufCur = bufsav; //GB
      return false; //GB
    }
    if (c == (IClass*)-1) {
      // mark class as really not available
      fReadMap->remove(aOffset);
      if(!fReadMap->add(aOffset, -1)) {
        fOut << "Rio::Buffer::checkClass : " << "can't add key. " 
             << std::endl;
        fBufCur = bufsav; //GB
        return false;
      }
      fOut << "Rio::Buffer::checkClass : " 
           << "reference to unavailable class \"" 
           << (aClass ? aClass->name() : std::string(""))
           << "\", pointers of this type will be 0." << std::endl;
      fBufCur = bufsav; //GB
      return false; //GB
    } else {
      if(fVerboseLevel) {
        fOut << "Rio::Buffer::checkClass :"
             << " " << this << " :"
             << " class at offset " << aOffset
             << " was " << (c?c->name():std::string("null")) << "."
             << " aClass was "
             << (aClass ? aClass->name() : std::string("null"))
             << std::endl; 
      }
    }
    
    fBufCur = bufsav;
    
  } else if (cli == -1) {
    
     fOut << "Rio::Buffer::checkClass : class does not exist." << std::endl;
     // class really does not exist
     return false;

  } else {
     if(fVerboseLevel) {
      fOut << "Rio::Buffer::checkClass : "
           << " " << this << " :"
           << " class seems ok " 
           << long_tos(cli)
           << std::endl; 
    }
  }
    
  return true;
}

bool Rio::Buffer::checkObject(unsigned int aOffset,const IClass* aClass) {
  // Check for object in the read map. If the object is 0 it still has to be
  // read. Try to read it from the buffer starting at location offset. If the
  // object is -1 then it really does not exist and we return 0. If the object
  // exists just return the offset.

  // in position 0 we always have the reference to the null object
  if (!aOffset) return true;

  long cli = fReadMap->findValue(aOffset);

  if (cli==0) {

    fOut << "Rio::Buffer::checkObject :"
         << " try to read object at aOffset "
         << aOffset 
         << std::endl;

    // No object found at this location in map. It might have been skipped
    // as part of a skipped object. Try to explicitely read the object.
      
    // save fBufCur and set to place specified by aOffset (-kMapOffset)
    char *bufsav = fBufCur;
    fBufCur = (char *)(fBuffer + aOffset - Rio_kMapOffset);
      
    //FIXME IObject* obj = readObject(cl);
    IObject* obj;
    Arguments args;
    Args_init(args);
    if(!readObject(args,obj)) {
      fOut << "Rio::Buffer::checkObject : " << "can't read object. " 
           << std::endl;
      fBufCur = bufsav; //GB
      return false; //GB
    }
    //IObject* obj = 0;
    if (!obj) {
      // mark object as really not available
      fReadMap->remove(aOffset);
      if(!fReadMap->add(aOffset, -1)) {
        fOut << "Rio::Buffer::checkObject : " << "can't add key. " 
             << std::endl;
        fBufCur = bufsav; //GB
        return false;
      }
      fOut << "Rio::Buffer::checkObject : "
           << "reference to object of unavailable class \"" 
           << (aClass ? aClass->name() : std::string(""))
           << "\", pointer will be 0." << std::endl;
      fBufCur = bufsav; //GB
      return false; //GB
    }
    
    fBufCur = bufsav;
    
  } else if (cli == -1) {
    
    fOut << "Rio::Buffer::checkObject :"
         << " object does not exist. aOffset " << aOffset
         << std::endl;
    // object really does not exist
    return false;

  } else {

    if(fVerboseLevel) {
      fOut << "Rio::Buffer::checkObject :"
           << " " << this << " :"
           << " ok " 
           << long_tos(cli)
           << " aOffset "
           << aOffset
           << std::endl; 
    }

  }
   
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readClass(
 const IClass* aRequiredClass
,unsigned int& aObjectTag
,IClass*& aClass
)
//////////////////////////////////////////////////////////////////////////////
// Read class definition from I/O buffer. clReq can be used to cross check
// if the actually read object is of the requested class. objTag is
// set in case the object is a reference to an already read object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aClass = 0;
  Rio_assert_return(inline_isReading(),false);

  if(fVerboseLevel) {
    fOut << "Rio::Buffer::readClass :"
         << " " << this << " :"
         << " Required class is "
         << (aRequiredClass?aRequiredClass->name():std::string("null")) << "."
         << std::endl;
  }

  // read byte count and/or tag (older files don't have byte count)
  unsigned int bcnt = 0;
  unsigned int tag;
  unsigned int startpos = 0;
  if(!read(bcnt)) {
    return false;
  }

  if(fVerboseLevel) {
    fOut << "Rio::Buffer::readClass :"
         << " " << this << " :"
         << " bcnt " << smanip::tostringx(bcnt) << "."
         << " buf size " << fBufSize << "."
         << std::endl;
  }
  if(bcnt==Rio_kNullTag) { //GB
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " bcnt is Rio_kNullTag."
           << std::endl;
    }
    tag  = bcnt;
    bcnt = 0;
  } else if(bcnt == Rio_kNewClassTag) { // class desc follows.
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " bcnt is Rio_kNewClassTag."
           << std::endl;
    }
    tag  = bcnt;
    bcnt = 0;
  } else if(bcnt & Rio_kByteCountMask) {
    fVersion = 1;
    startpos = (unsigned int)(fBufCur-fBuffer);
    if(!read(tag)) return false;
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " tag " << smanip::tostringx(tag)
           << std::endl;
    }
  } else {
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " (?) byte count " << bcnt
           << std::endl;
    }
    //Old version ?
    tag  = bcnt;
    bcnt = 0;
    //return false; //GB ?
  }
  
  if(tag & Rio_kClassMask) {
  } else {
    // in case tag is object tag return tag
    aObjectTag = tag;
    aClass = 0;
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " tag " << smanip::tostringx(tag)
           << ". tag is object tag (vers " << fVersion << ")."
           << std::endl;
    }
    return true;
  }
  
  IClass* cl = 0;
  if (tag == Rio_kNewClassTag) {
    
    char s[80];
    if(!readString(s, 80)) return false;
    std::string className = s;

    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " read className \"" << className << "\"."
           << std::endl;
    }

    // got a new class description followed by a new object
    // (class can be 0 if class dictionary is not found, in that
    // case object of this class must be skipped)
    // ROOT code : cl = TClass::Load(*this);
    cl = fDictionary.findInStoreClass(className);
    if(!cl) {
      fOut << "Rio::Buffer::readClass : could not create Class of name \""
           << className << "\"." << std::endl;
      return false;
    }

    // add class to fReadMap for later reference
    if (fVersion > 0) {
      // check if class was already read
      IClass* cl1 = (IClass*)fReadMap->findValue(startpos + Rio_kMapOffset);
      if (cl1 != cl) {
        if(fVerboseLevel) {
          fOut << "Rio::Buffer::readClass :"
               << " " << this << " :"
               << " mapObject className \"" << className << "\""
               << " at pos " << startpos << "."
               << std::endl;
        }
        if(!mapObject(cl ? cl : (void*)-1, startpos + Rio_kMapOffset)) 
          return false;
      }
    } else {
      fOut << "Rio::Buffer::readClass : mapObject fMapCound case "
           << fMapCount << std::endl;
      if(!mapObject(cl, fMapCount)) return false;
    }

  } else {
    
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readClass :"
           << " " << this << " :"
           << " got a tag to an already seen class (vers " 
           << fVersion << ")."
           << std::endl;
    }

    // got a tag to an already seen class
    unsigned int clTag = (tag & ~Rio_kClassMask);
    
    if (fVersion > 0) {
      clTag += fDisplacement;
      if(!checkClass(clTag,aRequiredClass)) {//GB : treat bool ret value.
        fOut << "Rio::Buffer::readClass : checkClass failed."
             << std::endl;
        return false;
      }
    } else {
      if (clTag == 0 || clTag > (unsigned int)fReadMap->size()) {
        fOut << "Rio::Buffer::readClass : illegal class tag="
             << clTag << " (0<tag<=" << fReadMap->size() 
             << "), I/O buffer corrupted." << std::endl;
        // exception
        return false;
      }
    }
    
    // class can be 0 if dictionary was not found
    cl = (IClass*)fReadMap->findValue(clTag);
  }
  
  /*FIXME
  if (cl && clReq && !cl->InheritsFrom(clReq)) {
    Error("ReadClass", "got wrong class: %s",cl->GetName());
    // exception
  }
  */

  // return bytecount in objTag
  aObjectTag = (bcnt & ~Rio_kByteCountMask);
  
  // case of unknown class
  if (!cl) cl = (IClass*)-1;
  
  if(fVerboseLevel) {
    fOut << "Rio::Buffer::readClass :"
         << " " << this << " :"
         << " end : "
         << (const void*)cl
         << std::endl;
  }

  aClass = cl;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::readObject(
 const Arguments& aArgs
,IObject*& aObject
)
//////////////////////////////////////////////////////////////////////////////
// Read object from I/O buffer. clReq can be used to cross check
// if the actually read object is of the requested class.
// WARNING : readObject can return true with aObject = 0 !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
    In buffer :  
      unsigned int kNullTag (0) (to write null pointer object)
    or if object was in the map :
      unsigned int objIdx.
    or if not in the map :
      class infos written by writeClass :
        unsigned int : 
            ( bcnt | Rio_kByteCountMask) (mask = 0x40000000)
          or packed in version :
            short : (bcnt | kByteCountVMask)  (mask = 0x4000
            short : 0
          (reversed if byte swap)
        unsigned int : tag which is :             
          if class in the map :
            (clIdx | kClassMask) if class in the map (mask = 0x80000000).
          or if class not in the map :
             unsigned int : kNewClassTag   (0xFFFFFFFF)
             string : className : WriteBuf(s, (strlen(s)+1)*sizeof(Text_t));
      object streaming.
   */

  aObject = 0;
  Rio_assert_return(inline_isReading(),false);

  // make sure fReadMap is initialized
  if(!initMap()) return false;
  
  // before reading object save start position
  unsigned int startpos = (unsigned int)(fBufCur-fBuffer);

  // attempt to load next object as TClass clReq
  unsigned int tag;       // either tag or byte count
  //FIXME TClass *clRef = ReadClass(clReq, &tag);

  if(fVerboseLevel) {
    fOut << "Rio::Buffer::readObject :"
         << " " << this << " :"
         << " begin."
         << std::endl;
  }

  IClass* clRef;
  if(!readClass(0,tag,clRef)) {
    fOut << "Rio::Buffer::readObject : can't read class." << std::endl;
    return false;
  }

  // check if object has not already been read
  // (this can only happen when called via checkObject())
  IObject* obj = 0;
  if (fVersion > 0) {
    obj = (IObject*)fReadMap->findValue(startpos + Rio_kMapOffset);
    if (obj) {
      fOut << "Rio::Buffer::readObject :"
           << " object already read " 
           << (const void*)obj
           << std::endl;
      if (obj == (IObject*) -1) {
        obj = 0;
      /*FIXME
      } else if (clReq && !obj->IsA()->InheritsFrom(clReq)) {
        out << "Rio::Buffer::readObject : got object of wrong class." << endl;
        // exception
      */
      }
      if(!checkByteCount(startpos,tag,obj->isA().inStoreName())) {
        return false;
      }
      aObject = obj;
      return true;
    } else {
      if(fVerboseLevel) {
        fOut << "Rio::Buffer::readObject :"
             << " " << this << " :"
             << " object not found in map."
             << std::endl;
      }
    }
  }
  
  // unknown class, skip to next object and return 0 obj
  if (clRef == (IClass*) -1) {
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readObject :"
           << " " << this << " :"
           << " unknown class, skip to next object and return 0 obj." 
           << std::endl;
    }
    if (fVersion > 0) {
      if(!mapObject((void*)-1, startpos + Rio_kMapOffset)) return false;
    } else {
      if(!mapObject(0, fMapCount)) return false;
    }

    if(!checkByteCount(startpos,tag,"unknown")) {
      fOut << "Rio::Buffer::readObject :"
           << " unknown class, checkByteCount failed."
           << std::endl;
      return false;
    }
    return false;
  }
  
  if (!clRef) {

    // got a reference to an already read object
    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readObject :"
           << " " << this << " :"
           << " got a reference to an already read object (vers " 
           << fVersion << ")." << std::endl;
    }

    if (fVersion > 0) {
      tag += fDisplacement;
      if(!checkObject(tag,0)) { //GB : treat bool ret value.
        fOut << "Rio::Buffer::readClass : checkObject failed."
             << std::endl;
        return false;
      }
    } else {
      if (tag > (unsigned int)fReadMap->size()) {
        fOut << "Rio::Buffer::readObject : " 
             << "object tag " << tag 
             << " too large (>" << (unsigned int)fReadMap->size()
             << "), I/O buffer corrupted." << std::endl;
        // exception
        return false;
      }
    }
    
    obj = (IObject*)fReadMap->findValue(tag);

    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readObject :"
           << " " << this << " :"
           << " get object in map " 
           << (const void*)obj
           << std::endl;
    }

    /*FIXME
    if (obj && clReq && !obj->IsA()->InheritsFrom(clReq)) {
      Error("ReadObject", "got object of wrong class");
      // exception
    }
    */
    
  } else {
    
    // allocate a new object based on the class found

    if(fVerboseLevel) {
      fOut << "Rio::Buffer::readObject :"
           << " " << this << " :"
           << " create a new object (vers " << fVersion << ")." 
           << std::endl;
    }

    /* obj = (TObject *)clRef->New(); */
    obj = clRef->create(aArgs); 
    if(!obj) {
      fOut << "Rio::Buffer::readObject : could not create object of class \"" 
           << clRef->name() << "\"." << std::endl;
      return false;
    }
   
    // add to fReadMap before reading rest of object
    if (fVersion > 0) {
      if(fVerboseLevel) {
        fOut << "Rio::Buffer::readObject :"
             << " " << this << " :"
             << " map object at " << (startpos + Rio_kMapOffset) 
             << std::endl;
      }
      if(!mapObject(obj, startpos + Rio_kMapOffset)) return false;
    } else {
      if(fVerboseLevel) {
        fOut << "Rio::Buffer::readObject :"
             << " " << this << " :"
             << " map object fMapCound case " << fMapCount
             << std::endl;
      }
      if(!mapObject(obj, fMapCount)) return false;
    }

    if(!obj->stream(*this)) {
      fOut << "Rio::Buffer::readObject : could not stream object of class \"" 
           << clRef->name() << "\"." << std::endl;
      //FIXME delete obj;
      return false;
    }

    if(!checkByteCount(startpos, tag, clRef->inStoreName())) {
      fOut << "Rio::Buffer::readObject : checkByteCount failed "
           << "for object of class \"" 
           << clRef->name() << "\"." << std::endl;
      //FIXME delete obj;
      return false;
    }

  }
  
  if(fVerboseLevel) {
    fOut << "Rio::Buffer::readObject :"
         << " " << this << " :"
         << " end : "
         << (const void*)obj
         << std::endl;
  }

  aObject = obj;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::writeObject(
 IObject* aObject
)
//////////////////////////////////////////////////////////////////////////////
// Write object to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(inline_isWriting(),false);

  // make sure fWriteMap is initialized
  if(!initMap()) return false;

  const void* ptr = aObject;
  unsigned long idx;

  if (!aObject) {

    // save kNullTag to represent NULL pointer
    if(!write(Rio_kNullTag)) return false;

  } else if ((idx = (unsigned long)
              fWriteMap->findValue(hash(&ptr,sizeof(void*)),(long)aObject)
              ) != 0) {
    
    // truncation is OK the value we did put in the map is an 30-bit offset
    // and not a pointer
    unsigned int objIdx = (unsigned int)idx;
    
    unsigned int offset = (unsigned int)(fBufCur-fBuffer); //GB

    // save index of already stored object
    if(!write(objIdx)) return false;
    
    f_obj_mapped.push_back(std::pair<bufpos,bufpos>(offset,objIdx));

  } else {

    // reserve space for leading byte count
    unsigned int cntpos = (unsigned int)(fBufCur-fBuffer);
    fBufCur += sizeof(unsigned int);
    
    // write class of object first
    if(!writeClass(aObject->isA())) return false;
    
    // add to map before writing rest of object (to handle self reference)
    // (+Rio_kMapOffset so it's != kNullTag)
    if(!mapObject(aObject, cntpos + Rio_kMapOffset)) return false;
    
    // let the object write itself :
    if(!aObject->stream(*this)) return false;
    
    // write byte count
    if(!setByteCount(cntpos)) return false;
  
  }
  return true;
}

bool Rio::Buffer::displaceMapped(unsigned int a_num){ //GB
  char* old = fBufCur;

 {if(fVerboseLevel) {
    fOut << "Rio::Buffer::displaceMapped :"
         << " cls num " << f_cls_mapped.size()
         << std::endl;
  }
  std::vector< std::pair<bufpos,bufpos> >::const_iterator it;
  for(it=f_cls_mapped.begin();it!=f_cls_mapped.end();++it) {
    unsigned int offset = (*it).first;
    unsigned int id = (*it).second;
    //fOut << "   displace " << offset << " " << id << std::endl;
    fBufCur = fBuffer+offset+a_num;
    unsigned int clIdx = id+a_num;
    if(!write((clIdx | Rio_kClassMask))) {fBufCur = old;return false;}
  }}

 {if(fVerboseLevel) {
    fOut << "Rio::Buffer::displaceMapped :"
         << " obj_num " << f_obj_mapped.size()
         << std::endl;
  }
  std::vector< std::pair<bufpos,bufpos> >::const_iterator it;
  for(it=f_obj_mapped.begin();it!=f_obj_mapped.end();++it) {
    unsigned int offset = (*it).first;
    unsigned int id = (*it).second;
    //fOut << "   displace " << offset << " " << id << std::endl;
    fBufCur = fBuffer+offset+a_num;
    unsigned int objIdx = id+a_num;
    if(!write(objIdx)) {fBufCur = old;return false;}
  }}

  fBufCur = old;
  return true;
}
