//
//  A ROOT file is a suite of consecutive data records with the following
//    format (see also the TKey class);
// TKey ---------------------
//      byte 1->4  Nbytes    = Length of compressed object (in bytes)
//           5->6  Version   = TKey version identifier
//           7->10 ObjLen    = Length of uncompressed object
//          11->14 Datime    = Date and time when object was written to file
//          15->16 KeyLen    = Length of the key structure (in bytes)
//          17->18 Cycle     = Cycle of key
//          19->22 SeekKey   = Pointer to record itself (consistency check)
//          23->26 SeekPdir  = Pointer to directory header
//          27->27 lname     = Number of bytes in the class name
//          28->.. ClassName = Object Class Name
//          ..->.. lname     = Number of bytes in the object name
//          ..->.. Name      = lName bytes with the name of the object
//          ..->.. lTitle    = Number of bytes in the object title
//          ..->.. Title     = Title of the object
//          -----> DATA      = Data bytes associated to the object
//
//  The first data record starts at byte fBEGIN (currently set to kBegin)
//  Bytes 1->kBegin contain the file description:
//       byte  1->4  "root"      = Root file identifier
//             5->8  fVersion    = File format version
//             9->12 fBEGIN      = Pointer to first data record
//            13->16 fEND        = Pointer to first free word at the EOF
//            17->20 fSeekFree   = Pointer to FREE data record
//            21->24 fNbytesFree = Number of bytes in FREE data record
//            25->28 nfree       = Number of free data records
//            29->32 fNbytesName = Number of bytes in TNamed at creation time
//            33->33 fUnits      = Number of bytes for file pointers
//            34->37 fCompress   = Zip compression level
//

// this :
#include <Rio/Core/File.h>

#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Core/Key.h>
#include <Rio/Core/Directory.h>
#include <Rio/Core/FreeSegment.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Platform.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/List.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/Debug.h>

#include "tobuf.h"

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

// Various mess :
#ifdef WIN32
#include <direct.h>
#define ssize_t int
#include <io.h>
#include <sys/types.h>
#else
#include <unistd.h>
#endif
#if defined(__alpha) && !defined(linux) // OSF1 & cxx.
#define EINTR                4                /* Interrupted system call */
#endif

static const int kBegin = 64;
static const char kUnits = 4;
enum EAccessMode {
  kFileExists        = 0,
  kExecutePermission = 1,
  kWritePermission   = 2,
  kReadPermission    = 4
};

static int Rio_open(const char*,int,unsigned int);
static bool Rio_fileStatus(const std::string&,long&,long&,long&,long&);
static bool Rio_accessPathName(const std::string&,EAccessMode);
static bool Rio_unlink(const std::string&);

#define NOT_OPEN (-1)

RIO_META_SOURCE(Rio::File)
//////////////////////////////////////////////////////////////////////////////
Rio::File::File(
 IDictionary& aDictionary
,std::ostream& aOut
,const std::string& aPath
,Mode aMode
,int aVerboseLevel
)
:fClass(aDictionary.opt_File_Class())
,fName(aPath)
,fTitle("")
,fMode(aMode)
,fDictionary(aDictionary)
,fOut(aOut)
,fRootDirectory(0)
,fVerboseLevel(aVerboseLevel)
,fWritable(false)
,fFileDescriptor(NOT_OPEN)
,fBytesWrite(0)
,fBytesRead(0)
,fWritten(0)
,fSumBuffer(0)
,fSum2Buffer(0)
// Begin of record :
,fVersion(0)
,fBEGIN(0)
,fEND(0)
,fSeekFree(0)
,fNbytesFree(0)
,fNbytesName(0)
,fUnits(kUnits)
,fCompress(1)
,fSeekInfo(0)
,fNbytesInfo(0)
//////////////////////////////////////////////////////////////////////////////
//  If Mode   = CREATE          create a new file and open it for writing,
//                              if the file already exists the file is
//                              not opened.
//            = RECREATE        create a new file, if the file already
//                              exists it will be overwritten.
//            = UPDATE          open an existing file for writing.
//                              if no file exists, it is created.
//            = READ            open an existing file for reading.
//  If the constructor failed in any way IsZombie() will return true.
//  Use IsOpen() to check if the file is (still) open.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::File");

  fVersion = version();
  // Root directory (should have the same name, title than the file) :

  // Extract file name :
  std::string directory;
  std::string name;
  std::string suffix;
  smanip::pathNameSuffix(fName,directory,name,suffix);

  fRootDirectory = new Directory(*this,0,name,fTitle);
  if(!fRootDirectory->isValid()) {
    fOut << "Rio::File::File : file \"" 
         << fName << "\" root directory badly created." << std::endl;
    //WARNING : do not delete fRootDirectory due to the directory() method
    // that return a reference.
    return;
  }

  bool create = (aMode==CREATE ? true : false);
  bool recreate = (aMode==RECREATE ? true : false);
  bool update = (aMode==UPDATE ? true : false);
  bool read = (aMode==READ ? true : false);

  if (!create && !recreate && !update && !read) {
    read = true;
  }

  if(recreate) {
    if(Rio_accessPathName(fName,kFileExists)) Rio_unlink(fName);
    recreate = false;
    create = true;
  }
   
  if(create && Rio_accessPathName(fName, kFileExists)) {
    fOut << "Rio::File::File : file \"" << fName << "\" already exists." 
         << std::endl;
    return;
  }

  if(update) {
    if(!Rio_accessPathName(fName, kFileExists)) {
      update = false;
      create = true;
    }
    if(update && !Rio_accessPathName(fName,kWritePermission)) {
      fOut << "Rio::File::File : no write permission, could not open file \""
           << fName << "\"." << std::endl;
      return;
    }
  }
  if(read) {
    if(!Rio_accessPathName(fName,kFileExists)) {
      fOut << "Rio::File::File : file \"" << fName << "\" does not exist." 
           << std::endl;
      return;
    }
    if(!Rio_accessPathName(fName,kReadPermission)) {
      fOut << "Rio::File::File : no read permission, could not open file \"" << fName << "\"." << std::endl;
      return;
    }
  }


  if(create || update) {
#ifdef WIN32
    fFileDescriptor = Rio_open(fName.c_str(),
                               O_RDWR | O_CREAT | O_BINARY,
                               S_IREAD | S_IWRITE);
#else
    fFileDescriptor = Rio_open(fName.c_str(), 
                               O_RDWR | O_CREAT, 
                               0644);
#endif
    if(fFileDescriptor == NOT_OPEN) {
      fOut << "Rio::File::File : file \"" << fName 
           << "\" can not be opened." << std::endl;
      return;
    }
    fWritable = true;
  } else {
#ifdef WIN32
    fFileDescriptor = Rio_open(fName.c_str(),
                               O_RDONLY | O_BINARY, 
                               S_IREAD | S_IWRITE);
#else
    fFileDescriptor = Rio_open(fName.c_str(),
                               O_RDONLY, 
                               0644);
#endif
    if(fFileDescriptor == NOT_OPEN) {
      fOut << "Rio::File::File : file \"" << fName
           << "\" can not be opened for reading." << std::endl;
      return;
    }
    fWritable = false;
  }

  if(!(create ? initializeInCreateMode():initializeInUpdateMode())) {
    ::close(fFileDescriptor);
    fFileDescriptor = NOT_OPEN;
    return;
  }

  // Create StreamerInfo index
  /*FIXME
  int lenIndex = gROOT->GetListOfStreamerInfo()->GetSize()+1;
  if (lenIndex < 5000) lenIndex = 5000;
  fClassIndex = new TArrayC(lenIndex);
  if (fSeekFree > fBEGIN) readStreamerInfo();*/

}
//////////////////////////////////////////////////////////////////////////////
Rio::File::File(
 const File& aFile
)
:IFile(aFile)
,fClass(aFile.fClass)
,fDictionary(aFile.fDictionary)
,fOut(aFile.fOut)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::File");
}
//////////////////////////////////////////////////////////////////////////////
Rio::File::~File(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  close();
  delete fRootDirectory;
  if(verboseLevel()) {
    fOut << "Rio::File::~File : called for \"" << fName << "\"." << std::endl;
  }
  Debug::decrement("Rio::File");
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::operator=(
 const File&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::File::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(File);
  else 
    return 0;
}

std::ostream& Rio::File::out() const {return fOut;}

//////////////////////////////////////////////////////////////////////////////
const Rio::IDictionary& Rio::File::dictionary(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDictionary;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory& Rio::File::directory(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *fRootDirectory;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::isOpen(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFileDescriptor == NOT_OPEN ? false : true;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::verboseLevel(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fVerboseLevel;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fVerboseLevel = aVerboseLevel;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::File::name(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::isWritable(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fWritable; 
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::File::end(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fEND; 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::setEnd(
 Seek aLast
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fEND = aLast; 
}
//////////////////////////////////////////////////////////////////////////////
std::list<Rio::FreeSegment*>& Rio::File::freeSegments(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFreeSegments;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::compressionLevel(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCompress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::setCompressionLevel(
 int aLevel
)
//////////////////////////////////////////////////////////////////////////////
// Set level of compression for this file
// ======================================
//
//  level = 0 objects written to this file will not be compressed.
//  level = 1 minimal compression level but fast.
//  ....
//  level = 9 maximal compression level but slow.
//
//  Note that the compression level may be changed at any time.
//  The new compression level will only apply to newly written objects.
//  The function TFile::Map shows the compression factor
//  for each object written to this file.
//  The function TFile::GetCompressionFactor returns the global
//  compression factor for this file.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aLevel < 0) aLevel = 0;
  if (aLevel > 9) aLevel = 9;
  fCompress = aLevel;
  if(fVerboseLevel) {
    fOut << "Rio::File::setCompressionLevel :"
         << " set compression level " << fCompress
	 << " for file \"" << fName << "\"."
         << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::initializeInCreateMode(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBEGIN = kBegin;  // First used word in file following the file header.
  fEND = fBEGIN;    // Pointer to end of file.

  fFreeSegments.push_back(new FreeSegment(fOut,fBEGIN,RIO_START_BIG_FILE));

  // Write Directory info :
  int namelen = 
    Rio_std_string_recordSize(fName) + 
    Rio_std_string_recordSize(fTitle);
  int nbytes = namelen + fRootDirectory->recordSize();
  Key key(*this,fRootDirectory->seekDirectory(),
          fName,fTitle,"TFile",nbytes);
  // fNbytesName = start point of directory info from key head.
  fNbytesName = key.keyLength() + namelen; 
  fRootDirectory->setNbytesName(fNbytesName);
  fRootDirectory->setSeekDirectory(key.seekKey());
  fSeekFree = 0;
  fNbytesFree = 0;

  if(!writeHeader()) {
    fOut << "Rio::File::initialize : can't write file header." << std::endl;
    return false;
  }

  char* buffer = key.dataBuffer();
  if(!tobuf(fOut,key.eob(),buffer,fName)) return false;
  if(!tobuf(fOut,key.eob(),buffer,fTitle)) return false;
  if(!fRootDirectory->fillBuffer(key.eob(),buffer)) return false;

  if(verboseLevel()) {
    fOut << "Rio::File::initialize : write key (" << namelen 
         << ", " << fRootDirectory->recordSize() << ", " 
         << nbytes << ", " << fNbytesName << ", " << key.seekKey() 
         << "):" << std::endl;
  }

  int n;
  if(!key.writeFile(1,n)) {
    fOut << "Rio::File::initialize : can't write key in file." << std::endl;
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::initializeInUpdateMode(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!readHeader()) {
    fOut << "Rio::File::initalize : Cannot read header." << std::endl;
    return false;
  }
  fRootDirectory->setSeekDirectory(fBEGIN);
  // Read Free segments structure if file is writable :
  if (fWritable) {
    if (fSeekFree > fBEGIN) {
      if(!readFreeSegments()) {
        fOut << "Rio::File::initalize : Cannot read free segments." 
             << std::endl;
        return false;
      }
    } else {
      fOut << "Rio::File::initialize : file \"" << fName 
           << "\" probably not closed, cannot read free segments" << std::endl;
    }
  }
  // Read Directory info :
  int nbytes = fNbytesName + fRootDirectory->recordSize();
  char* header = new char[nbytes];
  char* buffer = header;
  if(!setPosition(fBEGIN)) {
    fOut << "Rio::File::initalize : Cannot set position." << std::endl;
    delete [] header;
    return false;
  }
  if(!readBuffer(buffer,nbytes)) {
    fOut << "Rio::File::initalize : Cannot read buffer." << std::endl;
    delete [] header;
    return false;
  }
  buffer = header+fNbytesName;
  const char* eob = header+nbytes;
  if(!fRootDirectory->readBuffer(eob,buffer)) {
    fOut << "Rio::File::initalize : Cannot read buffer (2)." << std::endl;
    delete [] header;
    return false;
  }
  int nk =             //size of Key
    sizeof(int) +      //Key::fNumberOfBytes
    sizeof(short) +    //Key::fVersion
    2*sizeof(int) +    //Key::fObjectSize, Date
    2*sizeof(short) +  //Key::fKeyLength,fCycle
#ifdef RIO_LARGE_FILE
    2*sizeof(Seek32);  //Key::fSeekKey,fSeekParentDirectory
                       //WARNING : the upper is Seek32 since at begin of file.
#else
    2*sizeof(Seek);    //Key::fSeekKey,fSeekParentDirectory
#endif
  buffer = header+nk;
  std::string cname;
  // Should be "TFile".
  if(!frombuf(fOut,eob,buffer,cname)) {
    fOut << "Rio::File::initalize : Cannot read buffer (3)." << std::endl;
    delete [] header;
    return false;
  }
  if(!frombuf(fOut,eob,buffer,cname)) {
    fOut << "Rio::File::initalize : Cannot read buffer (4)." << std::endl;
    delete [] header;
    return false;
  }
  if(!frombuf(fOut,eob,buffer,fTitle)) {
    fOut << "Rio::File::initalize : Cannot read buffer (5)." << std::endl;
    delete [] header;
    return false;
  }
  delete [] header;
  int dirNbytesName = fRootDirectory->nBytesName();
  if (dirNbytesName < 10 || dirNbytesName > 1000) {
    fOut << "Rio::File::initalize : Cannot read directory info" << std::endl;
    return false;
  }
  // Read keys of the top directory :
  if (fRootDirectory->seekKeys() > fBEGIN) {
    int n;
    if(!fRootDirectory->readKeys(n)) {
      fOut << "Rio::File::initalize : can't read keys." << std::endl;
      return false;
    }
  } else {
    fOut << "Rio::File::initialize : file \"" << fName 
         << "\" probably not closed, trying to recover" << std::endl;
    if(!recover()) {
      fOut << "Rio::File::initalize : Cannot recover." << std::endl;
      return false;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::close(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!isOpen()) return false;

  if (isWritable()) if(!writeStreamerInfo()) return false;
  /*FIXME
  delete fClassIndex;
  fClassIndex = 0;
  */
  
  //FIXME : TCollection::StartGarbageCollection();

  if(!fRootDirectory->close()) return false;

  if (isWritable()) {
    if(!fFreeSegments.empty()) {
      FreeSegment* f1 = *(fFreeSegments.begin());
      if(f1) {
        if(verboseLevel()) {
          fOut << "Rio::File::close : writeFreeSegments :" << std::endl;
        }

        // Write free segments linked list
        if(!writeFreeSegments()) return false; 

        if(!writeHeader())  { // Now write file header
          fOut << "Rio::File::close : can't write file header." << std::endl;
        }
      }
    }
    //FIXME if (fCache) fCache->Flush();
  }

  // Delete free segments from free list.
  std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();
      it!=fFreeSegments.end();
      it = fFreeSegments.erase(it)) 
    delete (*it);

  ::close(fFileDescriptor);
  fFileDescriptor = NOT_OPEN;
  
  fWritable = false;

  //FIXME TCollection::EmptyGarbageCollection();
  return true;
}
#include <string.h> //memcpy
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::writeHeader(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  FreeSegment* lastfree = (!fFreeSegments.empty() ? fFreeSegments.back() : 0);
  if (lastfree) fEND  = lastfree->first();
  const char* root = "root";
  char psave[kBegin];
  const char* eob = psave + kBegin;
  char* buffer = psave;
  ::memcpy(buffer, root, 4); buffer += 4;
  int version = fVersion;
#ifdef RIO_LARGE_FILE
  //GB : begin
  //if(fEND>RIO_START_BIG_FILE) {version += 1000000; fUnits = 8;}
  if((fEND>RIO_START_BIG_FILE)       ||
     (fSeekFree>RIO_START_BIG_FILE)  ||
     (fSeekInfo>RIO_START_BIG_FILE)  ){
    version += 1000000;
    fUnits = 8;
  }
  //GB : end
#endif
  if(!tobuf(fOut,eob,buffer, version)) return false;
#ifdef RIO_LARGE_FILE
  if(fBEGIN>RIO_START_BIG_FILE) { //GB
    fOut << "Rio::Branch::writeHeader :"
         << " attempt to write big Seek "
         << fBEGIN << " on 32 bits."
         << std::endl;
    return false;
  }
  if(!tobuf(fOut,eob,buffer, (Seek32)fBEGIN)) return false;
  if(version>1000000) {
    if(!tobuf(fOut,eob,buffer, fEND)) return false;
    if(!tobuf(fOut,eob,buffer, fSeekFree)) return false;
  } else {
    if(fEND>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::Branch::writeHeader :"
           << " attempt to write big Seek "
           << fEND << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,eob,buffer, (Seek32)fEND)) return false;
    if(fSeekFree>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::Branch::writeHeader :"
           << " attempt to write big Seek "
           << fSeekFree << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,eob,buffer, (Seek32)fSeekFree)) return false;
  }
#else
  if(!tobuf(fOut,eob,buffer, fBEGIN)) return false;
  if(!tobuf(fOut,eob,buffer, fEND)) return false;
  if(!tobuf(fOut,eob,buffer, fSeekFree)) return false;
#endif
  if(!tobuf(fOut,eob,buffer, fNbytesFree)) return false;
  int nfree  = fFreeSegments.size();
  if(!tobuf(fOut,eob,buffer, nfree)) return false;
  if(!tobuf(fOut,eob,buffer, fNbytesName)) return false;
  if(!tobuf(fOut,eob,buffer, fUnits)) return false;
  if(!tobuf(fOut,eob,buffer, fCompress)) return false;
#ifdef RIO_LARGE_FILE
  if(version>1000000) {
    if(!tobuf(fOut,eob,buffer, fSeekInfo)) return false;
  } else {
    if(fSeekInfo>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::Branch::writeHeader :"
           << " attempt to write big Seek "
           << fSeekInfo << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,eob,buffer, (Seek32)fSeekInfo)) return false;
  }
#else
  if(!tobuf(fOut,eob,buffer, fSeekInfo)) return false;
#endif
  if(!tobuf(fOut,eob,buffer, fNbytesInfo)) return false;
  int nbytes  = buffer - psave;
  if(!setPosition()) return false;
  if(!writeBuffer(psave, nbytes)) {
    return false;
  }
  if(!synchronize()) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::readHeader(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  char header[kBegin];
  if(!setPosition()) return false;
  if(!readBuffer(header,kBegin)) return false;
  // make sure this is a root file
  if (strncmp(header, "root", 4)) {
    fOut << "Rio::File::readHeader : \"" << fName 
         << "\" not a ROOT file" << std::endl;
    return false;
  }
  char *buffer = header + 4;    // skip the "root" file identifier
  const char* eob = header + kBegin;
  if(!frombuf(fOut,eob,buffer, &fVersion)) return false;
#ifdef RIO_LARGE_FILE
 {Seek32 i;
  if(!frombuf(fOut,eob,buffer, &i)) return false;
  fBEGIN = i;}
  if(fVersion>1000000) {
    if(!frombuf(fOut,eob,buffer, &fEND)) return false;
    if(!frombuf(fOut,eob,buffer, &fSeekFree)) return false;
  } else {
   {Seek32 i;
    if(!frombuf(fOut,eob,buffer, &i)) return false;
    fEND = i;}

   {Seek32 i;
    if(!frombuf(fOut,eob,buffer, &i)) return false;
    fSeekFree = i;}
  }
#else
  if(!frombuf(fOut,eob,buffer, &fBEGIN)) return false;
  if(!frombuf(fOut,eob,buffer, &fEND)) return false;
  if(!frombuf(fOut,eob,buffer, &fSeekFree)) return false;
#endif
  if(!frombuf(fOut,eob,buffer, &fNbytesFree)) return false;
  int nfree = 0;
  if(!frombuf(fOut,eob,buffer, &nfree)) return false;
  if(!frombuf(fOut,eob,buffer, &fNbytesName)) return false;
  if(!frombuf(fOut,eob,buffer, &fUnits )) return false;
  if(!frombuf(fOut,eob,buffer, &fCompress)) return false;
#ifdef RIO_LARGE_FILE
  if(fVersion>1000000) {
    if(!frombuf(fOut,eob,buffer, &fSeekInfo)) return false;
  } else {
   {Seek32 i;
    if(!frombuf(fOut,eob,buffer, &i)) return false;
    fSeekInfo = i;}
  }
#else
  if(!frombuf(fOut,eob,buffer, &fSeekInfo)) return false;
#endif
  if(!frombuf(fOut,eob,buffer, &fNbytesInfo)) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::version(
)
//////////////////////////////////////////////////////////////////////////////
// Return version id as an integer, i.e. "2.22/04" -> 22204.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
//#define ROOT_RELEASE "3.00/06"   

#ifdef RIO_LARGE_FILE
#define ROOT_MAJOR_VERSION 4
#define ROOT_MINOR_VERSION 0
#define ROOT_PATCH_VERSION 0
#else
#define ROOT_MAJOR_VERSION 3
#define ROOT_MINOR_VERSION 0
#define ROOT_PATCH_VERSION 6
#endif

  return 
    10000 * ROOT_MAJOR_VERSION + 
    100 * ROOT_MINOR_VERSION + 
    ROOT_PATCH_VERSION;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::setPosition(
 Seek aOffset
,From aFrom
)
//////////////////////////////////////////////////////////////////////////////
// Seek to a specific position in the file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int whence = 0;
  switch (aFrom) {
  case BEGIN:
    whence = SEEK_SET;
    break;
  case CURRENT:
    whence = SEEK_CUR;
    break;
  case END:
    whence = SEEK_END;
    break;
  }

#ifdef RIO_LARGE_FILE

#if defined(RIO_LSEEK64)
  if (::lseek64(fFileDescriptor, aOffset, whence) < 0) {
#elif defined(WIN32)
  if (::_lseeki64(fFileDescriptor, aOffset, whence) < 0) {
#else
  if (::lseek(fFileDescriptor, aOffset, whence) < 0) {
#endif

#else
  if (::lseek(fFileDescriptor, aOffset, whence) < 0) {
#endif
    fOut << "Rio::File::setPosition : cannot set position " 
         << aOffset << " in file \"" << fName << "\"." << std::endl;
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::writeBuffer(
 const char* aBuffer
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
// Write a buffer to the file. This is the basic low level write operation.
// Returns false in case of failure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!isOpen() || !fWritable) return false;
  ssize_t siz;
  //FIXME : gSystem->IgnoreInterrupt();
  while ((siz = ::write(fFileDescriptor,aBuffer,aLength)) < 0 && 
         errorNumber() == EINTR)
    resetErrorNumber();
  //FIXME : gSystem->IgnoreInterrupt(kFALSE);
  if (siz < 0) {
    fOut << "Rio::File::writeBuffer : error writing to file \"" 
         << fName << "\"." << std::endl;
    return false;
  }
  if (siz != aLength) {
    fOut << "Rio::File::writeBuffer : " 
         << "error writing all requested bytes to file \"" << fName 
         << "\", wrote " << long_tos(siz) << " of " << aLength << std::endl;
    return false;
  }
  fBytesWrite  += siz;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::readBuffer(
 char* aBuffer
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
// Read a buffer from the file. This is the basic low level read operation.
// Returns kTRUE in case of failure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(isOpen()) {
    ssize_t siz;
    while ((siz = ::read(fFileDescriptor, aBuffer, aLength)) < 0 && 
           errorNumber() == EINTR)
      resetErrorNumber();
    if (siz < 0) {
      fOut << "Rio::File::readBuffer : error reading from file \"" 
           << fName << "\"." << std::endl;
      return false;
    }
    if (siz != aLength) {
      fOut << "Rio::File::readBuffer : " 
           << "error reading all requested bytes from file " 
           << fName << ", got " << long_tos(siz) << " of " << aLength 
           << std::endl;
      return false;
    }
    fBytesRead  += siz;
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::getRecordHeader(
 const char* aEOB
,char* aBuffer
,Seek aFirst
,int aMaxbytes
,int& aNbytes
,short& aVersKey
,int& aObjectSize
,short& aKeyLength
,int& aNread
)
//////////////////////////////////////////////////////////////////////////////
// Read the logical record header starting at position first :
// maxbytes bytes are read into buf
// the function reads nread bytes where nread is the minimum of maxbytes
// and the number of bytes before the end of file.
// the function returns nread.
// In output arguments:
//    nbytes : number of bytes in record
//             if negative, this is a deleted record
//             if 0, cannot read record, wrong value of argument first
//    objlen : uncompressed object size
//    keylen : length of logical record header
// Note that the arguments objlen and keylen are returned only if maxbytes >=16
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //GB : begin
  aNbytes = 0;
  aVersKey = 0;
  aObjectSize = 0;
  aKeyLength = 0;
  //GB : end
  aNread = 0;
  if (aFirst < fBEGIN) {
    fOut << "Rio::File::getRecordHeader : error : " 
         << aFirst << " < fBEGIN." << std::endl;
    return false;
  }
  if (aFirst > fEND) {
    fOut << "Rio::File::getRecordHeader : error : " 
         << aFirst << " > fEND." << std::endl;
    return false;
  }
  if(!setPosition(aFirst)) return false;
  int nread = aMaxbytes;
  if (aFirst+aMaxbytes > fEND) nread = fEND-aMaxbytes;
  if (nread < 4) {
    fOut << "Rio::File::getRecordHeader : parameter maxbytes=" 
         << nread << " must be >= 4" << std::endl;
    return false;
  }
  if(!readBuffer(aBuffer,nread)) {
    fOut << "Rio::File::getRecordHeader : can't read buffer." << std::endl;
    return false;
  }
  char* buffer = aBuffer;
  int nb;
  if(!frombuf(fOut,aEOB,buffer,&nb)) {
    fOut << "Rio::File::getRecordHeader : frombuf error." << std::endl;
    return false;
  }
  aNbytes = nb;
  if (nb < 0) {
    aNread = nread;
    fOut << "Rio::File::getRecordHeader : WARNING : nb<0." << std::endl;
    return true; //GB : return false ?
  }
//   const Int_t headerSize = Int_t(sizeof(nb) +sizeof(versionkey) +sizeof(olen) +sizeof(datime) +sizeof(klen));
  const int headerSize = 16;
  if (nread < headerSize) {
    aNread = nread;
    fOut << "Rio::File::getRecordHeader : WARNING nread < headerSize."
         << std::endl;
    return true; //GB : return false ?
  }
  short versionkey;
  if(!frombuf(fOut,aEOB,buffer, &versionkey)) {
    fOut << "Rio::File::getRecordHeader : frombuf error." << std::endl;
    return false;
  }
  int olen;
  if(!frombuf(fOut,aEOB,buffer, &olen)) {
    fOut << "Rio::File::getRecordHeader : frombuf error." << std::endl;
    return false;
  }
  unsigned int datime;
  if(!frombuf(fOut,aEOB,buffer, &datime)) {
    fOut << "Rio::File::getRecordHeader : frombuf error." << std::endl;
    return false;
  }
  short klen;
  if(!frombuf(fOut,aEOB,buffer, &klen)) {
    fOut << "Rio::File::getRecordHeader : frombuf error." << std::endl;
    return false;
  }
  if (!olen) olen = aNbytes-klen;
  aVersKey = versionkey;
  aObjectSize = olen;
  aKeyLength = klen;
  aNread = nread;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::errorNumber(
)
//////////////////////////////////////////////////////////////////////////////
// Return system error number.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_SOLARIS_CC50
  return ::errno;
#else
  return errno;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::resetErrorNumber(
)
//////////////////////////////////////////////////////////////////////////////
// Reset system error number.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_SOLARIS_CC50
   ::errno = 0;
#else
   errno = 0;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::synchronize(
)
//////////////////////////////////////////////////////////////////////////////
// Synchornize a file's in-core and on-disk states.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!isOpen() || !fWritable) return false;
#ifdef WIN32
  return true;
#else
  if (::fsync(fFileDescriptor) < 0) {
    fOut << "Rio::File::synchronize : error flushing file \"" 
         << fName << "\"." << std::endl;
    return false;
  } 
  return true;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::writeFreeSegments(
)
//////////////////////////////////////////////////////////////////////////////
//  The linked list of FREE segments (fFree) is written as a single data
//  record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Delete old record if it exists :
  if (fSeekFree != 0){
    if(!makeFreeSegment(fSeekFree, fSeekFree + fNbytesFree -1)) 
      return false;
  }

  int nbytes = 0;
 {std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();it!=fFreeSegments.end();++it) {
    nbytes += (*it)->recordSize();
  }}
  if(!nbytes) return true;

  Key key(*this,fRootDirectory->seekDirectory(),fName,fTitle,"TFile",nbytes);
  if (key.seekKey() == 0) {
    return true;
  }
  char* buffer = key.dataBuffer();
  
 {std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();it!=fFreeSegments.end();++it) {
    if(!(*it)->fillBuffer(key.eob(),buffer)) return false;
  }}

  fNbytesFree = key.numberOfBytes();
  fSeekFree   = key.seekKey();
  if(verboseLevel()) {
    fOut << "Rio::File::writeFreeSegments : write key :" << std::endl;
  }

  int n;
  return key.writeFile(1,n);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::readFreeSegments(
)
//////////////////////////////////////////////////////////////////////////////
// Read the FREE list.
//  Every file has a linked list (fFree) of free segments
//  This linked list has been written on the file via WriteFree
//  as a single data record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Key headerfree(*this,fSeekFree,fNbytesFree);
  if(!headerfree.readFile()) return false;
  char* buffer = headerfree.dataBuffer();
  if(!headerfree.readBuffer(headerfree.eob(),buffer)) return false;
  buffer = headerfree.dataBuffer();
  while(true) {
    FreeSegment* afree = new FreeSegment(fOut);
    if(!afree->readBuffer(headerfree.eob(),buffer)) {
      delete afree;
      return false;
    }
    fFreeSegments.push_back(afree);
    if (afree->last() > fEND) break;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::makeFreeSegment(
 Seek aFirst
,Seek aLast
)
//////////////////////////////////////////////////////////////////////////////
// Mark unused bytes on the file :
//  The list of free segments is in the fFree list
//  When an object is deleted from the file, the freed space is added
//  into the FREE linked list (fFree). The FREE list consists of a chain
//  of consecutive free segments on the file. At the same time, the first
//  4 bytes of the freed record on the file are overwritten by GAPSIZE
//  where GAPSIZE = -(Number of bytes occupied by the record).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFreeSegments.empty()) return false; //FIXME : is the return value ok ?
  FreeSegment* newfree = new FreeSegment(fOut,aFirst,aLast);
  if(!newfree) return false;
  fFreeSegments.insert(fFreeSegments.begin(),newfree);
  Seek nfirst = newfree->first();
  Seek nlast = newfree->last();
#ifdef RIO_LARGE_FILE
  Seek nbytesl = nlast-nfirst+1;
  if(nbytesl>RIO_START_BIG_FILE) nbytesl = RIO_START_BIG_FILE;
  int nbytes = -int(nbytesl);
#else
  int nbytes = (int)(nfirst - nlast -1);
#endif
  int nb = sizeof(int);
  char* buffer = new char[nb];
  char* psave = buffer;
  if(!tobuf(fOut,buffer+nb,buffer,nbytes)) {delete [] psave;return false;}
  if (nlast == fEND-1) fEND = nfirst;
  if(!setPosition(nfirst)) {delete [] psave;return false;}
  if(!writeBuffer(psave, nb)) {delete [] psave;return false;}
  synchronize();
  delete [] psave;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment* Rio::File::bestFreeSegment(
 int aNumberOfBytes
)
//////////////////////////////////////////////////////////////////////////////
// Return the best free segment where to store nbytes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();it!=fFreeSegments.end();++it) {
    FreeSegment* idcur = *it;
#ifdef RIO_LARGE_FILE
    Seek nleft = idcur->last() - idcur->first() +1;
    if (nleft == (Seek)aNumberOfBytes) return idcur;  // Found an exact match.
    if(nleft > (Seek)(aNumberOfBytes+3)) return idcur;
#else
    int nleft = int(idcur->last() - idcur->first() +1);
    if (nleft == aNumberOfBytes) return idcur;  // Found an exact match.
    if(nleft > aNumberOfBytes+3) return idcur;
#endif
  }
#ifdef RIO_LARGE_FILE
  if(fFreeSegments.empty()) return 0;
  FreeSegment* lastfree = fFreeSegments.back();
  lastfree->setLast(lastfree->last()+1000000000);
  return lastfree;
#else
  return 0;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::recover(
)
//////////////////////////////////////////////////////////////////////////////
// Attempt to recover file if not correctly closed.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {long id, size, flags, modtime;
  if (!Rio_fileStatus(fName,id,size,flags,modtime)) {
    fOut << "Rio::File::recover : cannot stat the file \"" 
         << fName << "\"." << std::endl;
    return false;
  }
  fEND = Seek(size);}

  char header[kBegin];
  const char* eob = header + kBegin;

  int nrecov = 0;
  int nwheader = 64;
  int nread = nwheader;
  Seek idcur = fBEGIN;
  while (idcur < fEND) {
    if(!setPosition(idcur)) return false;
    if (idcur+nread >= fEND) nread = fEND-idcur-1;
    if(!readBuffer(header, nread)) return false;
    char* buffer = header;
    char* bufread = header;
    int nbytes;
    if(!frombuf(fOut,eob,buffer, &nbytes)) return false;
    if (!nbytes) {
      fOut << "Rio::File::recover : Address = " << idcur << "\t" 
           << "Nbytes = " << nbytes << "\t" << "=====E R R O R=======" 
           << std::endl;
      break;
    }
    if (nbytes < 0) {
      idcur -= nbytes;
      if (fWritable) {
        fFreeSegments.push_back
          (new FreeSegment(fOut,idcur,idcur-nbytes-1));
      }
      if(!setPosition(idcur)) return false;
      continue;
    }
    short versionkey;
    if(!frombuf(fOut,eob,buffer, &versionkey)) return false;
    int objlen;
    if(!frombuf(fOut,eob,buffer, &objlen)) return false;
    unsigned int datime;
    if(!frombuf(fOut,eob,buffer, &datime)) return false;
    short keylen;
    if(!frombuf(fOut,eob,buffer, &keylen)) return false;
    short cycle;
    if(!frombuf(fOut,eob,buffer, &cycle)) return false;
    Seek seekkey,seekpdir;
#ifdef RIO_LARGE_FILE
    if(versionkey>1000) {
      if(!frombuf(fOut,eob,buffer, &seekkey)) return false;
      if(!frombuf(fOut,eob,buffer, &seekpdir)) return false;
    } else {
     {Seek32 i;
      if(!frombuf(fOut,eob,buffer, &i)) return false;
      seekkey = i;}

     {Seek32 i;
      if(!frombuf(fOut,eob,buffer, &i)) return false;
      seekpdir = i;}
    }
#else
    if(!frombuf(fOut,eob,buffer, &seekkey)) return false;
    if(!frombuf(fOut,eob,buffer, &seekpdir)) return false;
#endif
    char nwhc;
    if(!frombuf(fOut,eob,buffer, &nwhc)) return false;
    char* classname = new char[nwhc+1];
    int i;
    for (i = 0;i < nwhc; i++) {
      if(!frombuf(fOut,eob,buffer, &classname[i])) {
        delete [] classname;
        return false;
      }
    }
    classname[nwhc] = '\0';
    if (!strcmp(classname,"TBasket")) {
      idcur += nbytes; 
      delete [] classname;
      continue;
    }
    if (seekpdir != fRootDirectory->seekDirectory()) {
      idcur += nbytes; continue;
    }
    Key* key = new Key(*this);
    if(!key->readBuffer(bufread+kBegin,bufread)) {
      delete key;
      delete [] classname;
      return false;
    }
    fRootDirectory->appendKey(key);
    nrecov++;
    delete [] classname;
    idcur += nbytes;
  }
  if (fWritable) {
    Seek max_file_size = RIO_START_BIG_FILE;
#ifdef RIO_LARGE_FILE
    if(max_file_size<fEND) max_file_size = fEND+1000000000;
#endif
    fFreeSegments.push_back(new FreeSegment(fOut,fEND,max_file_size));
    if (nrecov) {
      int n;
      if(!write(n)) return false;
    }
  }
  if (nrecov) {
    fOut << "Rio::File::recover : successfully recovered " 
         << nrecov << " keys." << std::endl;
  } else {
    fOut << "Fio::File::recover : no keys recovered." << std::endl;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::write(
 int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Write memory objects to this file :
//  Loop on all objects in memory (including subdirectories).
//  A new key is created in the KEYS linked list for each object.
//  The list of keys is then saved on the file (via WriteKeys)
//  as a single data record.
//  The directory header info is rewritten on the directory header record.
//  The linked list of FREE segments is written.
//  The file header is written (bytes 1->fBEGIN).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  if (!isWritable()) {
    fOut << "Rio::File::write : file not opened in write mode." << std::endl;
    return false;
  }

  if(verboseLevel()) {
    fOut << "Rio::File::write : writing Name=\"" 
	 << fName << "\" Title=\"" << fTitle << "\"." << std::endl;
  }

  int nbytes;
  if(!fRootDirectory->write(nbytes)) return false; // Write directory tree

  if(!writeStreamerInfo()) return false;

  if(verboseLevel()) {
    fOut << "Rio::File::write : writeFreeSegments :" << std::endl;
  }

  if(!writeFreeSegments()) return false; // Write free segments.

  if(!writeHeader()) { // Now write file header
    fOut << "Rio::File::write : can't write file header." << std::endl;
    return false;
  }

  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::writeStreamerInfo(
)
//////////////////////////////////////////////////////////////////////////////
//  Write the list of TStreamerInfo as a single object in this file
//  The class Streamer description for all classes written to this file
//  is saved.
//  see class TStreamerInfo
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fWritable) return false;

  /*FIXME
  if (!fClassIndex) return;
  //no need to update the index if no new classes added to the file
  if (fClassIndex->fArray[0] == 0) return;
  if (gDebug > 0) printf("Calling WriteStreamerInfo for file: %s\n",GetName());
  */

  // build a temporary list with the marked files
  List list(fDictionary);

  // ROOT code : TIter next(gROOT->GetListOfStreamerInfo());
  const std::list<IObject*>& infos = fDictionary.streamerInfos();
  std::list<IObject*>::const_iterator it;
  for(it=infos.begin();it!=infos.end();++it) {
    /*FIXME
    if (info->IsA() != TStreamerInfo::Class()) {
      Warning("WriteStreamerInfo"," not a TStreamerInfo object");
      continue;
    }
    */
    //int uid = (*it)->identifier();
    int uid = 0; //FIXME
    //FIXME if (fClassIndex->fArray[uid]) list.Add(info);
    list.push_back(*it);
    if(verboseLevel()) {
      fOut << "Rio::File::writeStreamerInfo : -class: \""
	   << (*it)->name() << "\" info number " << uid << " saved." 
           << std::endl;
    }
  }
  if (list.size() == 0) return false;
  //FIXME fClassIndex->fArray[0] = 0;

  // always write with compression on
  int compress = fCompress;
  fCompress = 1;

  //free previous StreamerInfo record
  if(fSeekInfo) {
    if(!makeFreeSegment(fSeekInfo,fSeekInfo+fNbytesInfo-1)) 
      return false;
  }
  //Create new key
  //FIXME : &list will be put in the buffer map !
  Key key(*this,*fRootDirectory,list,"StreamerInfo",bestBuffer());
  fRootDirectory->removeKey(&key);
  fSeekInfo = key.seekKey();
  fNbytesInfo = key.numberOfBytes();
  sumBuffer(key.objectSize());
  int nbytes;
  if(!key.writeFile(0,nbytes)) return false;
  if(!nbytes) return false;

  fCompress = compress;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::readStreamerInfo(
)
//////////////////////////////////////////////////////////////////////////////
// Read the list of StreamerInfo from this file
// The key with name holding the list of TStreamerInfo objects is read.
// The corresponding TClass objects are updated.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // FIXME : dummy for the moment...
  /*
  if (fSeekInfo > 0 && fSeekInfo < fEND) {
    Key key(fFile);
    char* buffer = new char[fNbytesInfo+1];
    char* buf = buffer;
    if(!setPosition(fSeekInfo)) return false;
    if(!readBuffer(buf,fNbytesInfo)) return false;
    if(!key.readBuffer(buf)) return false;
    //FIXME list = (TList*)key->ReadObj();
    delete [] buffer;
  }
  */
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::sumBuffer(
 int aBufferSize
)
//////////////////////////////////////////////////////////////////////////////
// Increment statistics for buffer sizes of objects in this file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fWritten++;
  fSumBuffer  += aBufferSize;
  fSum2Buffer += aBufferSize * aBufferSize;
}
#include <math.h> //fabs,sqrt
//////////////////////////////////////////////////////////////////////////////
int Rio::File::bestBuffer(
) const
//////////////////////////////////////////////////////////////////////////////
// Return the best buffer size of objects on this file.
//  The best buffer size is estimated based on the current mean value
//  and standard deviation of all objects written so far to this file.
//  Returns mean value + one standard deviation.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fWritten) return Buffer::INITIAL;
  double mean = fSumBuffer/fWritten;
  double rms2 = ::fabs(fSum2Buffer/fSumBuffer -mean*mean);
  return (int)(mean + ::sqrt(rms2));
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool Rio_fileStatus(
 const std::string& aPath
,long& aID
,long& aSize
,long& aFlags
,long& aModtime
)
//////////////////////////////////////////////////////////////////////////////
// Get info about a file: id, size, flags, modification time.
// Id      is (statbuf.st_dev << 24) + statbuf.st_ino
// Size    is the file size
// Flags   is file type: 0 is regular file, bit 0 set executable,
//                       bit 1 set directory, bit 2 set special file
//                       (socket, fifo, pipe, etc.)
// Modtime is modification time
// The function returns 0 in case of success and 1 if the file could
// not be stat'ed.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aID = 0;
  aSize = 0;
  aFlags = 0;
  aModtime = 0;
  struct stat statbuf;
  if(::stat(aPath.c_str(),&statbuf) >= 0) {
#ifdef WIN32
    aID = (statbuf.st_dev << 24) + statbuf.st_ino;
#else
#if defined(RIO_KCC)
    aID = (statbuf.st_dev.__val[0] << 24) + statbuf.st_ino;
#else
    aID = (statbuf.st_dev << 24) + statbuf.st_ino;
#endif
#endif
    aSize = statbuf.st_size;
    aModtime = statbuf.st_mtime;
    if (statbuf.st_mode & ((S_IEXEC)|(S_IEXEC>>3)|(S_IEXEC>>6))) aFlags |= 1;
    if ((statbuf.st_mode & S_IFMT) == S_IFDIR) aFlags |= 2;
    if ((statbuf.st_mode & S_IFMT) != S_IFREG &&
        (statbuf.st_mode & S_IFMT) != S_IFDIR) aFlags |= 4;
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio_accessPathName(
 const std::string& aPath
,EAccessMode aMode
)
//////////////////////////////////////////////////////////////////////////////
// Returns true if one can access a file using the specified access mode.
// Mode is the same as for the WinNT access(2) function.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef WIN32
  return (::_access(aPath.c_str(), aMode) == 0) ? true : false;
#else
  return (::access(aPath.c_str(), aMode) == 0) ? true : false;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio_unlink(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// Unlink, i.e. remove, a file or directory. Returns true when succesfull,
// false in case of failure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  struct stat finfo;
  if (::stat(aPath.c_str(), &finfo) < 0) return false;
#ifdef WIN32 
  if (finfo.st_mode & S_IFDIR)
    return (::_rmdir(aPath.c_str())==-1 ? false : true);
  else
    return (::unlink(aPath.c_str())==-1 ? false : true);
#else
  if (S_ISDIR(finfo.st_mode))
    return (::rmdir(aPath.c_str())==-1 ? false : true);
  else
    return (::unlink(aPath.c_str())==-1 ? false : true);
#endif
}
int Rio_open(const char* aName,int aFlags,unsigned int aMode) {
#ifdef RIO_LARGE_FILE
#if defined(RIO_LSEEK64)
   return ::open64(aName,aFlags,aMode);
#else
   return ::open(aName,aFlags,aMode);
#endif
#else
   return ::open(aName,aFlags,aMode);
#endif
}
