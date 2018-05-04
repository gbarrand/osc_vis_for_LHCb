// this :
#include <Rio/Tree/Basket.h>

// Core :
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/File.h> //Hard to avoid.
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Lib.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/realloc.h>

//
#define RIO_MEMPHYS

#ifdef RIO_MEMPHYS
#ifdef RIO_LARGE_FILE
#include <Rio/Core/FreeSegment.h>
#endif
#endif

// Tree :
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

extern "C" {
  void Rio_R__zip(int,int*,char*,int*,char*,int*);
  void Rio_R__unzip(int*,unsigned char*,int*,unsigned char*,int*);
}

#define MAXIMUM(a,b) ((a)>(b)?a:b) 

const unsigned int Rio_kDisplacementMask = 0xFF000000;
const int Rio_kMAXBUF = 0xffffff;

RIO_META_SOURCE(Rio::Basket)

//////////////////////////////////////////////////////////////////////////////
Rio::Basket::Basket(
 const IDictionary& aDictionary
,IDirectory& aDirectory
,Branch& aBranch
)
:Object(aDictionary)
,Key(*Rio::cast<File>(aDirectory.file()))
,fClass(aDictionary.opt_Basket_Class())
,fDirectory(aDirectory)
,fBranch(aBranch)
,fFile_end(fFile.end()) //GB
,fBufferSize(0)
,fNevBufSize(0)
,fNevBuf(0)
,fLast(0)
,fHeaderOnly(false)
,fDisplacement(0)
,fEntryOffset(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Basket");
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket::Basket(
 const IDictionary& aDictionary
,IDirectory& aDirectory
,Branch& aBranch
,const std::string& aName
,const std::string& aTitle
)
:Object(aDictionary)
,Key(*Rio::cast<File>(aDirectory.file()))
,fClass(aDictionary.opt_Basket_Class())
,fDirectory(aDirectory)
,fBranch(aBranch)
,fFile_end(fFile.end()) //GB
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Basket");
  fObjectName = aName;
  fObjectTitle = aTitle;
  fObjectClassName = fClass.inStoreName();
  fBufferSize  = fBranch.basketSize();
  fNevBufSize  = fBranch.entryOffsetLength();
  fNevBuf      = 0;
  fEntryOffset = 0;  //Must be set to 0 before calling Sizeof
  fDisplacement= 0;  //Must be set to 0 before calling Sizeof
  fBuffer      = 0;  //Must be set to 0 before calling Sizeof
  fBufSize     = 0;
  fBufferRef = new Buffer(fFile.dictionary(),aDirectory.out(),
                          Buffer::WRITE, fBufferSize);

#ifdef RIO_LARGE_FILE
  if(fFile.end()>RIO_START_BIG_FILE){
    if(fVersion>1000) {
    } else {
      fVersion += 1000;
    }
  }
#endif

  fHeaderOnly  = true;
  fLast        = 0; // RDK: Must initialize before calling Streamer()
  if(!stream(*fBufferRef)) return;
  fKeyLength   = fBufferRef->inline_length();
  //GB : ??? fObjectSize  = fBufferRef->bufferSize() - fKeyLength;
  fObjectSize  = fBufferSize - fKeyLength;
  fLast        = fKeyLength;
  fBuffer      = 0;
  fBufSize     = 0;
  fHeaderOnly  = false;
  if (fNevBufSize) fEntryOffset = new int[fNevBufSize];
  fBranch.tree().incrementTotalBuffers(fBufferSize);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket::Basket(
 const Basket& aFrom
)
:Object(aFrom)
,Key(*Rio::cast<File>(aFrom.fDirectory.file()))
,fClass(aFrom.fClass)
,fDirectory(aFrom.fDirectory)
,fBranch(aFrom.fBranch)
,fFile_end(fFile.end()) //GB
,fBufferSize(0)
,fNevBufSize(0)
,fNevBuf(0)
,fLast(0)
,fHeaderOnly(false)
,fDisplacement(0)
,fEntryOffset(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Basket");
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket::~Basket(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fDisplacement) delete [] fDisplacement;
  if (fEntryOffset)  delete [] fEntryOffset;
  Debug::decrement("Rio::Basket");
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket& Rio::Basket::operator=(
 const Basket&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::setReadMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set read mode of basket.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fLast = fBufferRef->inline_length();
  fBufferRef->setReadMode();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::setWriteMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set write mode of basket.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBufferRef->setWriteMode();
  fBufferRef->inline_setBufferOffset(fLast);
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::nevBuf(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fNevBuf;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::setNevBufSize(
 int aNumber
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNevBufSize = aNumber;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void* Rio::Basket::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(Basket);
  else 
    return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Basket::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    if(!Key::_stream(aBuffer)) return false; //this must be first
    short v;
    if(!aBuffer.readVersion(v)) return false;
    if(!aBuffer.read(fBufferSize)) return false;
    if(!aBuffer.read(fNevBufSize)) return false;
    if(!aBuffer.read(fNevBuf)) return false;
    if(!aBuffer.read(fLast)) return false;
    char flag;
    if(!aBuffer.read(flag)) return false;
    if (fLast > fBufferSize) fBufferSize = fLast;
    if (!flag) return true;

    //G.Barrand : add the below test.
    if( (flag!=1) &&(flag!=2)  &&
        (flag!=11)&&(flag!=12) &&
        (flag!=41)&&(flag!=42) &&
        (flag!=51)&&(flag!=52) ) {
      fBranch.out() << "Rio::Basket::stream : bad flag " 
                    << (int)flag << std::endl; 
      return false;
    }

    if (flag%10 != 2) {
      delete [] fEntryOffset;
      fEntryOffset = new int[fNevBufSize];
      if (fNevBuf) {
        //Int[fNevBuf]
        int n;
        if(!aBuffer.readArray(fEntryOffset,n)) return false;
      }
      if (20<flag && flag<40) {
        for(int i=0; i<fNevBuf; i++){
          fEntryOffset[i] &= ~Rio_kDisplacementMask;
        }
      }
      if (flag>40) {
        fDisplacement = new int[fNevBufSize];
        int n;
        if(!aBuffer.readArray(fDisplacement,n)) return false;
      }
    }
    if (flag == 1 || flag > 10) {
      fBufferRef = new Buffer(fFile.dictionary(),fFile.out(),
                              Buffer::READ,fBufferSize);
      char *buf  = fBufferRef->buffer();
      if (v > 1) {
        if(!aBuffer.readFastArray(buf,fLast)) return false;
      } else {
        int n;
        if(!aBuffer.readArray(buf,n)) return false;
      }
      fBufferRef->inline_setBufferOffset(fLast);
      fBranch.tree().incrementTotalBuffers(fBufferSize);
    }
  } else {
    if(!Key::_stream(aBuffer)) return false;   //this must be first
    if(!aBuffer.writeVersion(fClass.version())) return false;

    int curLast = fBufferRef->inline_length();
    if (fBufferRef && !fHeaderOnly && 
        !fSeekKey && (curLast > fLast)) fLast = curLast;
    //if (fBufferRef && !fHeaderOnly) fLast = curLast;
    if (fLast > fBufferSize) fBufferSize = fLast;
        
    if(!aBuffer.write(fBufferSize)) return false;
    if(!aBuffer.write(fNevBufSize)) return false;
    if(!aBuffer.write(fNevBuf)) return false;
    if(!aBuffer.write(fLast)) return false;
    char flag = 1;
    if (!fEntryOffset)  flag  = 2;
    if (fBufferRef)     flag += 10;
    if (fDisplacement)  flag += 40;
    if (fHeaderOnly)    flag  = 0;
    if(!aBuffer.write(flag)) return false;
    if (fHeaderOnly) return true;; //sizeof = sizeof(Key)+19

    if (fEntryOffset && fNevBuf) {
      if(!aBuffer.writeArray(fEntryOffset, fNevBuf)) return false;
      if (fDisplacement) {
        if(!aBuffer.writeArray(fDisplacement, fNevBuf)) return false;
      }
    }

    if(fBufferRef) { //GB : ??? why that ?
      char* buf = fBufferRef->buffer();
      if(!aBuffer.writeFastArray(buf, fLast)) return false;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::update(
 int offset
,int skipped
)
//////////////////////////////////////////////////////////////////////////////
// Update basket header and EntryOffset table.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fEntryOffset) {
    if (fNevBuf+1 >= fNevBufSize) {
      int newsize = MAXIMUM(10,2*fNevBufSize);
      fEntryOffset = Rio::realloc<int>(fEntryOffset, 
                                       newsize * sizeof(int),
                                       fNevBufSize * sizeof(int));
      if (fDisplacement) {
        fDisplacement = Rio::realloc<int>(fDisplacement, 
                                          newsize * sizeof(int),
                                          fNevBufSize * sizeof(int));
      }
      fNevBufSize   = newsize;
      // Update branch only for the first 10 baskets
      if (fBranch.writeBasket() < 10) 
        fBranch.setEntryOffsetLength(newsize);
      
    }
    fEntryOffset[fNevBuf] = offset;
    
    if (skipped!=offset && !fDisplacement){
      fDisplacement = new int[fNevBufSize];
      for (int i = 0; i<fNevBufSize; i++) fDisplacement[i] = fEntryOffset[i];
    }
    if (fDisplacement) {
      fDisplacement[fNevBuf] = skipped;
      fBufferRef->setDisplacement(skipped);
    }
  }
  fNevBuf++;
}

#include <cstring> //memcpy

//////////////////////////////////////////////////////////////////////////////
bool Rio::Basket::writeBuffer(
 int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Write buffer of this basket on the current file
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  if (!fFile.isWritable()) {
    fBranch.out() << "Rio::Basket::writeBuffer : not writable." << std::endl;
    return false;
  }

  //GB : begin
#ifdef RIO_MEMPHYS
#ifdef RIO_LARGE_FILE
  // Change logic to enforce "on the fly" fVersion>1000 if
  // fVersion is still 2 but Key::fSeekKey>RIO_START_BIG_FILE.
  // If not doing that we shall write a big fSeekKey on a Seek32
  // and then losing data. It permits to pass the MEMPHYS/merge
  // program that creates a file > 2Gbytes. 

  if(fVersion>1000) {
  } else {
    int lref = fBufferRef->length();
    FreeSegment* bestfree = fFile.bestFreeSegment(lref+8);
    if(!bestfree) {
      fOut << "Rio::Basket::writeBuffer :" 
           << " fFile.bestFreeSegment failed."
           << std::endl;
      return false;
    }
    Seek seekKey = bestfree->first();
    if(seekKey>RIO_START_BIG_FILE) {
      fOut << "Rio::Basket::writeBuffer : " 
           << " WARNING : fSeekKey anomaly :"
           << " seeKey " << seekKey
           << ". Continue by trying to correct the data buffer."
           << std::endl;

      char* bref = fBufferRef->buffer();
      if((lref+8)>fBufferRef->bufferSize()) {
        if(!fBufferRef->expand(lref+8)) {
          fOut << "Rio::Basket::writeBuffer : can't expand buffer."
               << std::endl; 
          return false;
        }
      }
      int datalen = lref-fKeyLength;
      if(datalen) {
        char* b = new char[datalen];
        ::memcpy(b,bref+fKeyLength,datalen);
        ::memcpy(bref+fKeyLength+8,b,datalen);
        delete [] b;
      }
      fBufferRef->inline_setBufferOffset(lref+8);

      if(!fBufferRef->displaceMapped(8)) {
        fOut << "Rio::Basket::writeBuffer :" 
             << " displaceMapped failed."
             << std::endl;
        return false;
      }

      fVersion += 1000;
      fKeyLength += 8;

      if(fEntryOffset) {
        for(int i=0;i<fNevBuf;i++) fEntryOffset[i] += 8;
        if(fDisplacement) {
          //??? Do we have to shift them ?
          fOut << "Rio::Basket::writeBuffer :" 
               << " displace logic : fDisplacement not null."
               << std::endl;
        }
      }

    }
  }
#endif
#endif
  //GB : end

  // Transfer fEntryOffset table at the end of fBuffer. Offsets to fBuffer
  // are transformed in entry length to optimize compression algorithm.
  fLast = fBufferRef->inline_length();
  if (fEntryOffset) {
    if(!fBufferRef->writeArray(fEntryOffset,fNevBuf+1)) {
      delete [] fEntryOffset; 
      fEntryOffset = 0;
      return false;
    }
    delete [] fEntryOffset; 
    fEntryOffset = 0;
    if (fDisplacement) {
      if(!fBufferRef->writeArray(fDisplacement,fNevBuf+1)) {
        delete [] fDisplacement; 
        fDisplacement = 0;
        return false;
      }
      delete [] fDisplacement; 
      fDisplacement = 0;
    }
  }
  
 {int lbuf = fBufferRef->inline_length();
  fObjectSize = lbuf - fKeyLength;}

  int nout = 0;
  fHeaderOnly = true;

  fCycle = fBranch.writeBasket();
  int cxlevel = fBranch.compressionLevel();
  if (cxlevel > 0) {
    if (cxlevel == 2) cxlevel--;
    int nbuffers = fObjectSize/Rio_kMAXBUF;
    //add 28 bytes in case object is placed in a deleted gap
    int buflen = fKeyLength + fObjectSize + 28;
    if(fBuffer) {
      fBranch.out() << "Rio::Basket::writeBuffer : new char anomaly." 
                    << std::endl;
    }
    fBuffer = new char[buflen];
    fBufSize = buflen;
    char* objbuf = fBufferRef->buffer() + fKeyLength;
    char* bufcur = fBuffer+fKeyLength;
    int noutot = 0;
    int nzip   = 0;
    for (int i=0;i<=nbuffers;i++) {
      int bufmax;
      if (i == nbuffers) bufmax = fObjectSize -nzip;
      else               bufmax = Rio_kMAXBUF;
      Rio_R__zip(cxlevel, &bufmax, objbuf, &bufmax, bufcur, &nout);
      //GB if (nout == 0) { //this happens when the buffer cannot be compressed
      if((nout==0)||(nout>=fObjectSize)) { //GB

        //fBranch.out() << "Rio::Basket::writeBuffer :"
        //              << " can't compress."
        //              << std::endl;

        if(noutot==0) delete [] fBuffer; //GB : not in ROOT.

        //same code as for no compression (cxlevel==0) :

        fBuffer = fBufferRef->buffer();
        fBufSize = fBufferRef->bufferSize();
        if(!initialize(fObjectSize)) {
          fBranch.out() << "Rio::Basket::writeBuffer : cannot initialize (1)."
                        << std::endl;
          return false;
        }
        fSeekParentDirectory = fDirectory.seekDirectory();
        fBufferRef->inline_setBufferOffset(0);
        //write key itself again
        if(!stream(*fBufferRef)) return false; //fHeaderOnly=true
        nout = fObjectSize; //GB
        int nbytes;
        if(!Key::writeFile(0,nbytes)) return false;
        fHeaderOnly = false;
        aNbytes = fKeyLength + nout;
        return true;
      }
      bufcur += nout;
      noutot += nout;
      objbuf += Rio_kMAXBUF;
      nzip   += Rio_kMAXBUF;
    }
    if(noutot>fObjectSize) {
      fBranch.out() << "Rio::Basket::writeBuffer :" 
                    << " compression anomaly "
                    << " fObjectSize " << fObjectSize
                    << " noutot " << noutot
                    << std::endl;
      return false;
    }
    nout = noutot;
    if(!initialize(noutot)) {
      fBranch.out() << "Rio::Basket::writeBuffer : cannot initialize (2)." 
                    << " fFile_end " << fFile_end
                    << std::endl;
      return false;
    }
    fSeekParentDirectory = fDirectory.seekDirectory();
    fBufferRef->inline_setBufferOffset(0);
    //write key itself again
    if(!stream(*fBufferRef)) return false; //fHeaderOnly=true         

    if(fBufferRef->length()!=fKeyLength) {
      fBranch.out() << "Rio::Basket::writeBuffer :"
                    << " key len anomaly " << fBufferRef->length()
                    << " fKeyLength " << fKeyLength
                    << std::endl;
      return false;
    }

    ::memcpy(fBuffer,fBufferRef->buffer(),fKeyLength);
    delete fBufferRef; 
    fBufferRef = 0;
  } else {
    fBuffer = fBufferRef->buffer();
    fBufSize = fBufferRef->bufferSize();
    if(!initialize(fObjectSize)) {
      fBranch.out() << "Rio::Basket::writeBuffer : cannot initialize (3)." 
                    << std::endl;
      return false;
    }
    fSeekParentDirectory = fDirectory.seekDirectory();
    fBufferRef->inline_setBufferOffset(0);
    //write key itself again
    if(!stream(*fBufferRef)) return false; //fHeaderOnly=true
    nout = fObjectSize;
  }
  
  //  TKey::WriteFile calls FillBuffer. TBasket inherits from TKey, hence
  //  TBasket::FillBuffer is called.
  int nbytes;
  if(!Key::writeFile(0,nbytes)) return false;

  fHeaderOnly = false;
  
  aNbytes = fKeyLength + nout;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Basket::readBasketBytes(
 Seek aPosition
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Read basket buffers in memory and cleanup :
//       Read first bytes of a logical record starting at position pos
//       return record length (first 4 bytes of record)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  const int len = 128;
  char buffer[len];
  short verskey;
  short keylen;
  int nread;
  if(!fFile.getRecordHeader(buffer+len,buffer, aPosition,len, 
                            fNumberOfBytes,verskey,fObjectSize,keylen,nread)) {
    aNbytes = 0;
    return false;
  }
  if(verskey!=fVersion) {
    fBranch.out() << "Rio::Basket::readBasketBytes :" 
                  << " key version mismatch."
                  << std::endl;
    aNbytes = 0;
    return false;
  }
  //GB : check aPosition versus seekkey ?
  fKeyLength = keylen;
  aNbytes = fNumberOfBytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Basket::readBasketBuffers(
 Seek aPosition
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
// Read basket buffers in memory and cleanup :
//  Read a basket buffer. Check if buffers of previous ReadBasket
//  should not be dropped. Remember, we keep buffers
//  in memory up to fMaxVirtualSize.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fBranch.tree().memoryFull(fBufferSize)) fBranch.dropBaskets();
  
  fBufferRef = new Buffer(fFile.dictionary(),fFile.out(),Buffer::READ,aLength);
  char* buffer = fBufferRef->buffer();
  if(!fFile.setPosition(aPosition)) {
    fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                  << " File::setPosition failed for " << aPosition << "."
                  << std::endl;
    return false;
  }
  if(!fFile.readBuffer(buffer,aLength)) {
    fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                  << " File::readBuffer failed for " << aLength << "."
                  << std::endl;
    return false;
  }
  if(!stream(*fBufferRef)) {
    fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                  << " stream failed."
                  << std::endl;
    return false;
  }

  //GB : begin
  if(fSeekKey!=aPosition) {
    fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                  << " seek anomaly."
                  << " aPosition " << aPosition
                  << " fSeekKey " << fSeekKey
                  << std::endl;
  }
#ifdef RIO_LARGE_FILE 
  if(fVersion>1000) {
  } else {
    if(fSeekKey>RIO_START_BIG_FILE) {
      //fBranch.out() << "Rio::Basket::readBasketBuffers :" 
      //              << " seek anomaly. Expect version>1000."
      //              << std::endl;
    }
  }
#endif
  //GB : end

  bool status = true;
  if(fObjectSize<=0) {
    fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                  << " fObjectSize = " << fObjectSize << " !!!" 
                  << std::endl;
    status = false; //G.Barrand. (To protect the JEC BAOJEC.root case).
  }
  if (fObjectSize > (fNumberOfBytes-fKeyLength)) {
    if(fBuffer) {
      std::ostream& out = fBranch.out();
      out << "Rio::Basket::readBasketBuffers : new char anomaly." << std::endl;
    }
    fBuffer = new char[fObjectSize+fKeyLength];
    fBufSize = fObjectSize+fKeyLength;
    ::memcpy(fBuffer,buffer,fKeyLength);
    unsigned char* objbuf = (unsigned char*)(fBuffer + fKeyLength);
    int nin = fNumberOfBytes-fKeyLength;
    int nout;
    Rio_R__unzip
      (&nin, (unsigned char*)&buffer[fKeyLength], &fObjectSize, objbuf, &nout);
    if (nout != fObjectSize) {
      std::ostream& out = fBranch.out();
      out << "Rio::Basket::readBasketBuffers : fObjlen = " 
          << fObjectSize << ", nout = " << nout << std::endl;
      status = false;
    }
    delete [] buffer;
    fBufferRef->setBuffer(fBuffer, fObjectSize+fKeyLength );
  } else {
    fBuffer = fBufferRef->buffer();
    fBufSize = fBufferRef->bufferSize();
  }

  fBranch.tree().incrementTotalBuffers(fBufferSize);
  
  // read offsets table
  if (!fBranch.entryOffsetLength()) return status;
  delete [] fEntryOffset;
  fEntryOffset = 0;
  fBufferRef->inline_setBufferOffset(fLast);
  int n;
  if(!fBufferRef->readArray(fEntryOffset,n)) {
    fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                  << " readArray failed."
                  << std::endl;
    return false;
  }
  delete [] fDisplacement;
  fDisplacement = 0;
  if (fBufferRef->inline_length() != fBufferRef->bufferSize()) {
    // There is more data in the buffer!  It is the diplacement
    // array.
    if(!fBufferRef->readArray(fDisplacement,n)) {
      fBranch.out() << "Rio::Basket::readBasketBuffers :" 
                    << " readArray(2) failed."
                    << std::endl;
      return false;
    }
  }

  return status;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::dropBuffers(
)
//////////////////////////////////////////////////////////////////////////////
// Drop buffers of this basket if it is not the current basket
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fBuffer && !fBufferRef) return 0;
  //   delete [] fBuffer;
  if (fDisplacement) delete [] fDisplacement;
  if (fEntryOffset)  delete [] fEntryOffset;
  if (fBufferRef)    delete fBufferRef;
  fBufferRef   = 0;
  fBuffer      = 0;
  fDisplacement= 0;
  fEntryOffset = 0;
  fBranch.tree().incrementTotalBuffers(-fBufferSize);
  return fBufferSize;
}
