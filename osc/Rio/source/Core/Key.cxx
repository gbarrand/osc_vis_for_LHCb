//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  The Key class includes functions to book space on a file,           //
//   to create I/O buffers, to fill these buffers                       //
//   to compress/uncompress data buffers.                               //
//                                                                      //
//  Before saving (making persistent) an object on a file, a key must   //
//  be created. The key structure contains all the information to       //
//  uniquely identify a persistent object on a file.                    //
//     fNbytes    = number of bytes for the compressed object+key       //
//     version of the Key class                                         //
//     fObjlen    = Length of uncompressed object                       //
//     fDatime    = Date/Time when the object was written               //
//     fKeylen    = number of bytes for the key structure               //
//     fCycle     = cycle number of the object                          //
//     fSeekKey   = Address of the object on file (points to fNbytes)   //
//                  This is a redundant information used to cross-check //
//                  the data base integrity.                            //
//     fSeekPdir  = Pointer to the directory supporting this object     //
//     fClassName = Object class name                                   //
//     fName      = name of the object                                  //
//     fTitle     = title of the object                                 //
//                                                                      //
//  The Key class is used by ROOT to:                                   //
//    - to write an object in the Current Directory                     //
//    - to write a new ntuple buffer                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// this :
#include <Rio/Core/Key.h>

#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Core/File.h>
#include <Rio/Core/FreeSegment.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Directory.h>

#include "tobuf.h"

#include <string.h> //memcpy

#define Rio_Key_Class_Version 2
#define MAXIMUM(a,b) ((a)>(b)?a:b)
const int Rio_kMAXBUF = 0xffffff;

extern "C" {
  void Rio_R__zip(int,int*,char*,int*,char*,int*);
  void Rio_R__unzip(int*,unsigned char*,int*,unsigned char*,int*);
}

//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(File& aFile)
:fFile(aFile)
,fOut(aFile.out())
,fLeft(0)
,fBufSize(0)
,fBuffer(0)
,fBufferRef(0)
// Record
,fNumberOfBytes(0)
,fVersion(Rio_Key_Class_Version)
,fObjectSize(0)
,fKeyLength(0)
,fCycle(0)
,fSeekKey(0)
,fSeekParentDirectory(0)
//////////////////////////////////////////////////////////////////////////////
// Used by Basket, Directory::readKeys, File::recover.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Key");

#ifdef RIO_LARGE_FILE
  if(aFile.end()>RIO_START_BIG_FILE) fVersion += 1000;
#endif

  fKeyLength = findRecordSize();
  fDate.setDate(0);
}

//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
,Seek aSeekParentDirectory
,const std::string& aObjectName
,const std::string& aObjectTitle
,const std::string& aObjectClassName
,int aNumberOfBytes
)
:fFile(aFile)
,fOut(aFile.out())
,fLeft(0)
,fBufSize(0)
,fBuffer(0)
,fBufferRef(0)
// Record
,fNumberOfBytes(0)
,fVersion(Rio_Key_Class_Version)
,fObjectSize(aNumberOfBytes)
,fKeyLength(0)
,fCycle(0)
,fSeekKey(0)
,fSeekParentDirectory(0)
,fObjectClassName(aObjectClassName)
,fObjectName(aObjectName)
,fObjectTitle(aObjectTitle)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Key");

#ifdef RIO_LARGE_FILE
  if(aFile.end()>RIO_START_BIG_FILE) fVersion += 1000;
#endif

  fKeyLength = findRecordSize();
  if(!initialize(aNumberOfBytes)) {
    fOut << "Rio::Key::Key : cannot initialize (1)." << std::endl;
    return;
  }
  fSeekParentDirectory = aSeekParentDirectory;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
,Seek aPointer
,int aNumberOfBytes
)
:fFile(aFile)
,fOut(aFile.out())
,fBufSize(0)
,fBuffer(0)
,fBufferRef(0)
// Record :
,fNumberOfBytes(aNumberOfBytes)
,fVersion(Rio_Key_Class_Version)
,fObjectSize(0)
,fKeyLength(0)
,fCycle(0)
,fSeekKey(aPointer)
,fSeekParentDirectory(0)
//////////////////////////////////////////////////////////////////////////////
// Create a TKey object to read keys :
//  Constructor called by TDirectory::ReadKeys and by TFile::TFile
//  A TKey object is created to read the keys structure itself
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Key");

#ifdef RIO_LARGE_FILE
  if(aPointer>RIO_START_BIG_FILE) fVersion += 1000;
#endif

  fBuffer = new char[aNumberOfBytes];
  fBufSize = aNumberOfBytes;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
,Directory& aDirectory
,IObject& aObject
,const std::string& aName
,int aBufferSize
)
:fFile(aFile)
,fOut(aFile.out())
,fBufSize(0)
,fBuffer(0)
,fBufferRef(0)
// Record :
,fNumberOfBytes(0)
,fVersion(Rio_Key_Class_Version)
,fObjectSize(0)
,fKeyLength(0)
,fSeekKey(0)
,fSeekParentDirectory(0)
,fObjectName(aName)
,fObjectTitle(aObject.title())
//////////////////////////////////////////////////////////////////////////////
// Create a Key object and fill output buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Key");

#ifdef RIO_LARGE_FILE
  if(aFile.end()>RIO_START_BIG_FILE) fVersion += 1000;
#endif

  fObjectClassName = aObject.isA().inStoreName();
  fBufferRef = new Buffer(fFile.dictionary(),fOut,
                          Buffer::WRITE,aBufferSize);
  fCycle = aDirectory.appendKey(this);
  //write key itself
  if(!_stream(*fBufferRef)) {
    fOut << "Rio::Key::Key : cannot stream key." << std::endl;
    return;
  }
  fKeyLength = fBufferRef->inline_length();
  //FIXME register obj in map in case of self reference
  if(!fBufferRef->mapObject(&aObject)) {
    fOut << "Rio::Key::Key : cannot map object." << std::endl;
    return;
  }
  //write object
  if(!aObject.stream(*fBufferRef)) {
    fOut << "Rio::Key::Key :"
         << " cannot stream object of class "
         << " \"" << fObjectClassName << "\"." 
         << std::endl;
    return;
  }
  int lbuf = fBufferRef->inline_length();
  fObjectSize = lbuf - fKeyLength;

  int cxlevel = fFile.compressionLevel();
  if (cxlevel && fObjectSize > 256) {
    if (cxlevel == 2) cxlevel--;
    int nbuffers = fObjectSize/Rio_kMAXBUF;
    //add 8 bytes in case object is placed in a deleted gap
    int buflen = MAXIMUM(512,fKeyLength + fObjectSize + 9*nbuffers + 8); 
    if(fBuffer) {
      fOut << "Rio::Key::Key : new char anomaly." << std::endl;
    }
    fBuffer = new char[buflen];
    fBufSize = buflen;
    char* objbuf = fBufferRef->buffer() + fKeyLength;
    char* bufcur = fBuffer+fKeyLength;
    int noutot = 0;
    int nzip = 0;
    int bufmax;
    for (int i=0;i<=nbuffers;i++) {
      bufmax = ((i == nbuffers) ? fObjectSize - nzip : Rio_kMAXBUF);
      int nout;
      Rio_R__zip(cxlevel, &bufmax, objbuf, &bufmax, bufcur, &nout);
      if (nout == 0) { //this happens when the buffer cannot be compressed
        if(noutot==0) delete [] fBuffer; //GB : not in ROOT.
        fBuffer = fBufferRef->buffer();
        fBufSize = fBufferRef->bufferSize();
        if(!initialize(fObjectSize)) {
          fOut << "Rio::Key::Key : cannot initialize (2)." << std::endl;
          return;
        }
        fSeekParentDirectory = aDirectory.seekDirectory();
        fBufferRef->inline_setBufferOffset(0);
        //write key itself again
        if(!_stream(*fBufferRef)) {
          fOut << "Rio::Key::Key : cannot stream key." << std::endl;
        }
        return;
      }
      bufcur += nout;
      noutot += nout;
      objbuf += Rio_kMAXBUF;
      nzip   += Rio_kMAXBUF;
    }
    if(!initialize(noutot)) {
      fOut << "Rio::Key::Key : cannot initialize (3)." << std::endl;
      return;
    }
    fSeekParentDirectory = aDirectory.seekDirectory();
    fBufferRef->inline_setBufferOffset(0);
    //write key itself again
    if(!_stream(*fBufferRef)) {
      fOut << "Rio::Key::Key : cannot stream key." << std::endl;
      return;
    }
    ::memcpy(fBuffer,fBufferRef->buffer(),fKeyLength);
    delete fBufferRef; 
    fBufferRef = 0;
  } else {
    fBuffer = fBufferRef->buffer();
    fBufSize = fBufferRef->bufferSize();
    if(!initialize(fObjectSize)) {
      fOut << "Rio::Key::Key : cannot initialize (4)." << std::endl;
      return;
    }
    fSeekParentDirectory = aDirectory.seekDirectory();
    fBufferRef->inline_setBufferOffset(0);
    //write key itself again
    if(!_stream(*fBufferRef)) {
      fOut << "Rio::Key::Key : cannot stream key." << std::endl;
      return;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 const Key& aKey
)
:IKey(aKey)
,fFile(aKey.fFile)
,fOut(aKey.fOut)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Key");
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::operator=(
 const Key&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::~Key(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deleteBuffer();
  Debug::decrement("Rio::Key");
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Key::className(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectClassName;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Key::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectName;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Key::title(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectTitle;
}
//////////////////////////////////////////////////////////////////////////////
short Rio::Key::cycle(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCycle;
}

char* Rio::Key::dataBuffer() const {return fBuffer + fKeyLength;}
const char* Rio::Key::eob() const {return fBuffer + fBufSize;}

//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::initialize(
 int aNumberOfBytes
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fDate.set();

  // Find free segment :
  int nsize = aNumberOfBytes + fKeyLength;
  FreeSegment* bestfree  = fFile.bestFreeSegment(nsize);
  if(!bestfree) {
    fOut << "Rio::Key::initialize : cannot allocate " 
         << nsize << " bytes for ID = \"" 
         << fObjectName << "\" Title = \"" << fObjectTitle << "\"" 
         << std::endl;
    return false;
  }
  fSeekKey = bestfree->first();

#ifdef RIO_LARGE_FILE
  if(fVersion>1000) {
  } else {
    if(fSeekKey>RIO_START_BIG_FILE) {
      fOut << "Rio::Key::initialize : " 
           << " fSeekKey anomaly : "
           << " key " << (unsigned long)this
           << " fSeeKey " << fSeekKey
           << " fFile.end() " << fFile.end()
           << " fVersion " << fVersion
           << " fKeyLength " << fKeyLength
           << " oclss " << fObjectClassName
           << " oname " << fObjectName
           << " otitle " << fObjectTitle
           << " buf " << (unsigned long)fBuffer
           << " fObjectSize " << fObjectSize
           << " aNumberOfBytes " << aNumberOfBytes    
           << " datalen " << fBufSize-fKeyLength
           << std::endl;
      return false;
    }
  }
#endif

  // Case Add at the end of the file :
  if (fSeekKey == fFile.end()) {
    fFile.setEnd(fSeekKey+nsize);
    bestfree->setFirst(fSeekKey+nsize);
    fLeft = -1;
    if (!fBuffer) {
      fBuffer = new char[nsize];
      fBufSize = nsize;
    }
  } else {
    fLeft = int(bestfree->last() - fSeekKey - nsize + 1);
  }

  fNumberOfBytes = nsize;

  // Case where new object fills exactly a deleted gap :
  if (fLeft == 0) {
    if (!fBuffer) {
      fBuffer = new char[nsize];
      fBufSize = nsize;
    }
    fFile.freeSegments().remove(bestfree);
    delete bestfree;
  }

  // Case where new object is placed in a deleted gap larger than itself :
  if (fLeft > 0) {    // found a bigger segment
    if (!fBuffer) {
      fBuffer = new char[nsize+sizeof(int)];
      fBufSize = nsize+sizeof(int);
    }
   {char* buffer = fBuffer+nsize;
    if((buffer+sizeof(int))>eob()){ 
      //GB logic to solve pb seen with Rio_jec.cxx.
      if(fBuffer==fBufferRef->buffer()) {
        int lack = nsize + sizeof(int) - fBufSize;
        if(!fBufferRef->expand(fBufferRef->bufferSize()+lack)) return false;
        fBuffer = fBufferRef->buffer();
        fBufSize = fBufferRef->bufferSize();
      } else {
        //Allocated with new char[] :
        delete [] fBuffer;
        fBuffer = new char[nsize+sizeof(int)];
        fBufSize = nsize+sizeof(int);
      }
    }}
    char* buffer  = fBuffer+nsize;
    int nbytesleft = -fLeft;  // set header of remaining record
    if(!tobuf(fOut,eob(),buffer, nbytesleft)) {
      fOut << "Rio::Key::initialize : cannot write buffer." << std::endl;
      return false;
    }
    bestfree->setFirst(fSeekKey+nsize);
  }

  // Done elsewhere : fSeekParentDirectory = fDirectory.seekDirectory();
  return true;
}

int Rio::Key::findRecordSize() const {
  // Return the size in bytes of the key header structure.
  int nbytes = sizeof fNumberOfBytes; //int=4
  nbytes += sizeof(short);            //2
  nbytes += sizeof fObjectSize;       //4
  nbytes += fDate.recordSize();       //4
  nbytes += sizeof fKeyLength;        //2
  //16
  nbytes += sizeof fCycle;            //2
  //18
#ifdef RIO_LARGE_FILE
  if(fVersion>1000) {
    nbytes += sizeof(Seek);           //8
    nbytes += sizeof(Seek);           //8
    //18+16=34
  } else {
    nbytes += sizeof(Seek32);         //4
    nbytes += sizeof(Seek32);         //4
    //18+8=26
  }
#else
  nbytes += sizeof fSeekKey;
  nbytes += sizeof fSeekParentDirectory;
#endif
  nbytes += Rio_std_string_recordSize(fObjectClassName);
  nbytes += Rio_std_string_recordSize(fObjectName);
  nbytes += Rio_std_string_recordSize(fObjectTitle);
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::fillBuffer(
 const char* aEOB
,char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Encode key header into output buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!tobuf(fOut,aEOB,aBuffer, fNumberOfBytes)) return false;
  short version = fVersion;
  if(!tobuf(fOut,aEOB,aBuffer, version)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fObjectSize)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fDate.date())) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fKeyLength)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fCycle)) return false;
#ifdef RIO_LARGE_FILE
  if(version>1000) {
    if(!tobuf(fOut,aEOB,aBuffer, fSeekKey)) return false;
    if(!tobuf(fOut,aEOB,aBuffer, fSeekParentDirectory)) return false;
  } else {
    if(fSeekKey>RIO_START_BIG_FILE) {
      fOut << "Rio::Key::fillBuffer :"
           << " attempt to write big Seek "
           << fSeekKey << " on 32 bits."
           << std::endl;
      return false;
    }

    if(!tobuf(fOut,aEOB,aBuffer, (Seek32)fSeekKey)) return false;

    if(fSeekParentDirectory>RIO_START_BIG_FILE) {
      fOut << "Rio::Key::fillBuffer :"
           << " (2) attempt to write big Seek "
           << fSeekParentDirectory << " on 32 bits."
           << std::endl;
      return false;
    }

    if(!tobuf(fOut,aEOB,aBuffer, (Seek32)fSeekParentDirectory)) return false;
  }
#else
  if(!tobuf(fOut,aEOB,aBuffer, fSeekKey)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fSeekParentDirectory)) return false;
#endif
  if(!tobuf(fOut,aEOB,aBuffer, fObjectClassName)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fObjectName)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fObjectTitle)) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::readBuffer(
 const char* aEOB
,char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Decode input buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!frombuf(fOut,aEOB,aBuffer, &fNumberOfBytes)) return false;
  short version;
  if(!frombuf(fOut,aEOB,aBuffer,&version)) return false;
  fVersion = version;
  if(!frombuf(fOut,aEOB,aBuffer, &fObjectSize)) return false;
  unsigned int date;
  if(!frombuf(fOut,aEOB,aBuffer, &date)) return false;
  fDate.setDate(date);
  if(!frombuf(fOut,aEOB,aBuffer, &fKeyLength)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, &fCycle)) return false;
#ifdef RIO_LARGE_FILE
  if(version>1000) {
    if(!frombuf(fOut,aEOB,aBuffer, &fSeekKey)) return false;
    if(!frombuf(fOut,aEOB,aBuffer, &fSeekParentDirectory)) return false;
  } else {
   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer, &i)) return false;
    fSeekKey = i;}    
   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer, &i)) return false;
    fSeekParentDirectory = i;}
  }
#else
  if(!frombuf(fOut,aEOB,aBuffer, &fSeekKey)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, &fSeekParentDirectory)) return false;
#endif
  if(!frombuf(fOut,aEOB,aBuffer, fObjectClassName)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, fObjectName)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, fObjectTitle)) return false;
  /*FIXME : TBranch does a gROOT->SetReadingObject.
  if (!gROOT->ReadingObject()) {
    if (fSeekPdir != gDirectory->GetSeekDir()) gDirectory->AppendKey(this);
  }
  */
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::_stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream a class object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    if(!aBuffer.read(fNumberOfBytes)) return false;
    short version;
    if(!aBuffer.readVersion(version)) return false;
    fVersion = version;
    if(!aBuffer.read(fObjectSize)) return false;
    unsigned int date;
    if(!aBuffer.read(date)) return false;
    fDate.setDate(date);
    if(!aBuffer.read(fKeyLength)) return false;
    if(!aBuffer.read(fCycle)) return false;
#ifdef RIO_LARGE_FILE
    if(version>1000) {
      if(!aBuffer.read(fSeekKey)) return false;
      if(!aBuffer.read(fSeekParentDirectory)) return false;
    } else {
     {Seek32 i;
      if(!aBuffer.read(i)) return false;
      fSeekKey = i;}    
     {Seek32 i;
      if(!aBuffer.read(i)) return false;
      fSeekParentDirectory = i;}
    }
#else
    if(!aBuffer.read(fSeekKey)) return false;
    if(!aBuffer.read(fSeekParentDirectory)) return false;
#endif
    if(!aBuffer.read(fObjectClassName)) return false;
    if(!aBuffer.read(fObjectName)) return false;
    if(!aBuffer.read(fObjectTitle)) return false;
  } else {
    if(!aBuffer.write(fNumberOfBytes)) return false;
    short version = fVersion;
    if(!aBuffer.writeVersion(version)) return false;
    if(!aBuffer.write(fObjectSize)) return false;
    if (fDate.date() == 0) fDate.set();
    if(!aBuffer.write(fDate.date())) return false;
    if(!aBuffer.write(fKeyLength)) return false;
    if(!aBuffer.write(fCycle)) return false;
#ifdef RIO_LARGE_FILE
    if(version>1000) {
      if(!aBuffer.write(fSeekKey)) return false;
      if(!aBuffer.write(fSeekParentDirectory)) return false;
    } else {
      if(fSeekKey>RIO_START_BIG_FILE) { //GB
        fOut << "Rio::Key::_stream :"
             << " attempt to write big Seek "
             << fSeekKey << " on 32 bits."
             << std::endl;
        return false;
      }

      if(!aBuffer.write((Seek32)fSeekKey)) return false;

      if(fSeekParentDirectory>RIO_START_BIG_FILE) { //GB
        fOut << "Rio::Key::_stream :"
             << " (2) attempt to write big Seek "
             << fSeekParentDirectory << " on 32 bits."
             << std::endl;
        return false;
      }

      if(!aBuffer.write((Seek32)fSeekParentDirectory)) return false;
    }
#else
    if(!aBuffer.write(fSeekKey)) return false;
    if(!aBuffer.write(fSeekParentDirectory)) return false;
#endif
    if(!aBuffer.write(fObjectClassName)) return false;
    if(!aBuffer.write(fObjectName)) return false;
    if(!aBuffer.write(fObjectTitle)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::writeFile(
 int aCycle
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Write the encoded object supported by this key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  int nsize  = fNumberOfBytes;
  char* buffer = fBuffer;
  if (aCycle) {
    fCycle = aCycle;
    if(!fillBuffer(eob(),buffer)) return false;
    buffer = fBuffer;
  }

  if (fLeft > 0) nsize += sizeof(int);

  if(!fFile.setPosition(fSeekKey)) return false;

  if(!fFile.writeBuffer(buffer,nsize)) return false;

  //  gFile->Flush(); Flushing takes too much time.
  //                  Let user flush the file when he wants.
  if (fFile.verboseLevel()) {
    fOut << "Rio::Key::writeFile :"
         << " writing " << nsize << " bytes"
         << " at address " << fSeekKey
         << " for ID=\"" 
         << fObjectName << "\" Title=\"" << fObjectTitle << "\"." << std::endl;
  }

  deleteBuffer();
  aNbytes = nsize;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::readFile(
)
//////////////////////////////////////////////////////////////////////////////
// Read the key structure from the file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nsize = fNumberOfBytes;
  if(!fFile.setPosition(fSeekKey)) return false;
  if(!fFile.readBuffer(fBuffer,nsize)) return false;
  if(fFile.verboseLevel()) {
    fOut << "Rio::Key::readFile :"
         << " Reading " << nsize << " bytes"
         << " at address " << fSeekKey
         << "." 
         << std::endl;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::deleteBuffer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fBufferRef) {
    // fBuffer may come from a : fBuffer = fBufferRef->buffer();
    delete fBufferRef;
    fBufferRef = 0;
  } else {
    if(fBuffer) delete [] fBuffer;
  }
  fBuffer = 0;
  fBufSize = 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Key::readObject(
 Directory& aDirectory
)
//////////////////////////////////////////////////////////////////////////////
// To read an object from the file :
//  The object associated to this key is read from the file into memory
//  Once the key structure is read (via Streamer) the class identifier
//  of the object is known.
//  Using the class identifier we find the TClass object for this class.
//  A TClass object contains a full description (i.e. dictionary) of the
//  associated class. In particular the TClass object can create a new
//  object of the class type it describes. This new object now calls its
//  Streamer function to rebuilt itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBufferRef = new Buffer(fFile.dictionary(),fOut,
                          Buffer::READ,fObjectSize+fKeyLength);
  if(!fBufferRef) {
    fOut << "Rio::Key::readObject : cannot allocate buffer: " 
         << "fObjectSize = " << fObjectSize << std::endl;
    delete fBufferRef;
    fBufferRef = 0;
    return 0;
  }
  if(fObjectSize<=0) {
    fOut << "Rio::Key::readObject :" 
         << " fObjectSize = " << fObjectSize << " !!!" << std::endl;
  }
  if (fObjectSize > (fNumberOfBytes-fKeyLength)) {
    if(fBuffer) {
      fOut << "Rio::Key::readObject : new char anomaly." << std::endl; 
    }
    fBuffer = new char[fNumberOfBytes];
    fBufSize = fNumberOfBytes;
    if(!readFile()) {
      delete fBufferRef;
      fBufferRef = 0;
      delete [] fBuffer;
      fBuffer = 0;
      fBufSize = 0;
      return 0; //read object structure from file
    }
    ::memcpy(fBufferRef->buffer(),fBuffer,fKeyLength);
  } else {
    fBuffer = fBufferRef->buffer();
    fBufSize = fBufferRef->bufferSize();
    if(!readFile()) {
      delete fBufferRef;
      fBufferRef = 0;
      fBuffer = 0;
      fBufSize = 0;
      return 0; //Read object structure from file
    }
  }
  // get version of key
  fBufferRef->inline_setBufferOffset(sizeof(fNumberOfBytes));
  short kvers;
  if(!fBufferRef->readVersion(kvers)) {
    fOut << "Rio::Key::readObject : can't read key version." << std::endl;
    delete fBufferRef;
    fBufferRef = 0;
    //FIXME : delete [] fBuffer;
    fBuffer = 0;
    fBufSize = 0;
    return 0;
  }

  fBufferRef->inline_setBufferOffset(fKeyLength);

  Arguments args;
  Args_init(args);
  Args_insertDirectory(args,(IDirectory*)&aDirectory);

  IClass* cl = fFile.dictionary().findInStoreClass(fObjectClassName);
  if (!cl) {
    fOut << "Rio::Key::readObject : unknown class \"" 
         << fObjectClassName << "\"." << std::endl;
    delete fBufferRef;
    fBufferRef = 0;
    //FIXME delete [] fBuffer;
    fBuffer = 0;
    fBufSize = 0;
    return 0;
  }

  //NOTE : the create may have added the returned obj in the aDirectory.
  IObject* obj = cl->create(args); 
  if (!obj) {
    fOut << "Rio::Key::readObject :"
         << " cannot create new object of class"
         << " \"" << fObjectClassName << "\"." 
         << std::endl;
    delete fBufferRef;
    fBufferRef = 0;
    //FIXME delete [] fBuffer;
    fBuffer = 0;
    fBufSize = 0;
    return 0;
  }

  if (kvers > 1) {
    //register obj in map to handle self reference
    if(!fBufferRef->mapObject(obj)) return 0;
  }

  if (fObjectSize > (fNumberOfBytes-fKeyLength)) {
    unsigned char* objbuf = (unsigned char*)(fBufferRef->buffer()+fKeyLength);
    unsigned char* bufcur = (unsigned char*)(fBuffer+fKeyLength);
    int nout = 0;
    int noutot = 0;
    while(true) {
      int nin  = 
        9 + ((int)bufcur[3] | ((int)bufcur[4] << 8) | ((int)bufcur[5] << 16));
      int nbuf = 
        (int)bufcur[6] | ((int)bufcur[7] << 8) | ((int)bufcur[8] << 16);
      Rio_R__unzip(&nin, bufcur, &nbuf, objbuf, &nout);
      if (!nout) break;
      noutot += nout;
      if (noutot >= fObjectSize) break;
      bufcur += nin;
      objbuf += nout;
    }
    if (nout) {
      if(!obj->stream(*fBufferRef)) {
        fOut << "Rio::Key::readObject (0) : cannot stream object of class \""
             << fObjectClassName << "\"." << std::endl;
        aDirectory.removeObject(obj);
        delete obj;
        delete fBufferRef;
        fBufferRef = 0;
        delete [] fBuffer;
        fBuffer = 0;
        fBufSize = 0;
        return 0;
      }
      delete [] fBuffer;
      fBuffer = 0;
      fBufSize = 0;
    } else {
      fOut << "Rio::Key::readObject : nothing from unzip." << std::endl;
      aDirectory.removeObject(obj);
      delete obj;
      delete fBufferRef;
      fBufferRef = 0;
      delete [] fBuffer;
      fBuffer = 0;
      fBufSize = 0;
      return 0;
    }
  } else {
    if(!obj->stream(*fBufferRef)) {
      fOut << "Rio::Key::readObject (1) : cannot stream object of class \""
           << fObjectClassName << "\"." << std::endl;
      aDirectory.removeObject(obj);
      delete obj;
      delete fBufferRef;
      fBufferRef = 0;
      fBuffer = 0;
      fBufSize = 0;
      return 0;
    }
  }
  
  //FIXME if (gROOT->GetForceStyle()) obj->UseCurrentStyle();
  
  Directory* dir = Rio::cast<Directory>(*obj);
  if(dir) {
    if(fFile.verboseLevel()) {
      fOut << "Rio::Key::readObject : directory : \"" 
           << name() << "\"" << std::endl;
    }
    dir->setName(name());
    dir->setTitle(title());
  }

  delete fBufferRef;
  fBufferRef = 0;
  fBuffer = 0;
  fBufSize = 0;
  
  return obj;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Key::destroy(
)
//////////////////////////////////////////////////////////////////////////////
// Delete an object from the file :
// Note: the key is not deleted. You still have to call "delete key".
// This is different from the behaviour of TObject::Delete()!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFile.verboseLevel()) {
    fOut << "Rio::Key::destroy : " 
         << fObjectName << " at address " 
         << fSeekKey << ", nbytes = " << fNumberOfBytes << std::endl;
  }

  Seek first = fSeekKey;
  Seek last  = fSeekKey + fNumberOfBytes - 1;
  // release space used by this key
  if(!fFile.makeFreeSegment(first, last)) return false;  
  //Done elsewhere : if(fDirectory) fDirectory->removeKey(this);
  return true;
}

