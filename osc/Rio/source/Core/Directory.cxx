//
// A ROOT file is structured in Directories (like a file system).
// Each Directory has a list of Keys 
// and a list of objects in memory.
// A Key is a small object that describes the type and location
// of a persistent data in a file. The persistent data may be a directory.
//

// this :
#include <Rio/Core/Directory.h>

#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IPrinter.h>
#include <Rio/Core/File.h>
#include <Rio/Core/Key.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/smanip.h>

#include "tobuf.h"

#define BIT(n)       (1 << (n))

//----- Write() options
enum {
  Rio_kSingleKey = BIT(0),        // write collection with single key
  Rio_kOverWrite = BIT(1)         // overwrite existing object with same name
};

RIO_META_SOURCE(Rio::Directory)

//////////////////////////////////////////////////////////////////////////////
Rio::Directory::Directory(
 File& aFile
,Directory* aParent
)
:Named(aFile.dictionary(),"","")
,fClass(aFile.dictionary().opt_Directory_Class())
,fIDirectoryClass(*aFile.dictionary().findClass("Rio::IDirectory"))
,fFile(aFile)
,fOut(aFile.out())
,fParent(aParent)
,fIsValid(false)
,fIsClosing(false)
,fModified(false)
,fWritable(false)
// Record :
,fNbytesKeys(0)
,fNbytesName(0)
,fSeekDirectory(0)
,fSeekParent(0)
,fSeekKeys(0)
//////////////////////////////////////////////////////////////////////////////
// Used when reading a file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(file().verboseLevel()) {
    fOut << "Rio::Directory::Directory1 : called for " 
         << this << " \"" << name() << "\"." << std::endl;
  }

  aParent->appendObject(this);
  fIsValid = true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Directory::Directory(
 File& aFile
,Directory* aParent
,const std::string& aName
,const std::string& aTitle
)
:Named(aFile.dictionary(),aName,aTitle)
,fClass(aFile.dictionary().opt_Directory_Class())
,fIDirectoryClass(*aFile.dictionary().findClass("Rio::IDirectory"))
,fFile(aFile)
,fOut(aFile.out())
,fParent(aParent)
,fIsValid(false)
,fIsClosing(false)
,fModified(false)
,fWritable(false)
// Record :
,fNbytesKeys(0)
,fNbytesName(0)
,fSeekDirectory(0)
,fSeekParent(0)
,fSeekKeys(0)
//////////////////////////////////////////////////////////////////////////////
//  A new directory with name,title is created in the parent directory
//  The directory header information is immediatly saved on the file
//  A new key is added in the parent directory
//
//  FIXME : When this constructor is called from a class directly derived
//  from TDirectory, the third argument classname MUST be specified.
//  In this case, classname must be the name of the derived class.
//
//  Note that the directory name cannot contain slashes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(file().verboseLevel()) {
    fOut << "Rio::Directory::Directory : called for " 
         << this << " \"" << name() << "\"." << std::endl;
  }

  if(fName.empty()) {
    fOut << "Rio::Directory::Directory : "
         << "directory name cannot be \"\"." << std::endl;
    return; //FIXME ? what to do.
  }
  if(fName.find("/")!=std::string::npos) {
    fOut << "Rio::Directory::Directory : "
         << "directory name \"" << name() 
         << "\" cannot contain a slash." << std::endl;
    return; //FIXME ? what to do.
  }

  if(fTitle.empty()) fTitle = fName;

  initialize();

  if(!aParent) {
    fWritable = true; //root directory of fFile.
    fIsValid = true;
    return;
  }

  if(!fFile.isWritable()) {
    // In case of a directory in memory
    fIsValid = true;
    return;
  }
  if(aParent->getKey(fName)) {
    fOut << "Rio::Directory::Directory : directory \"" 
         << name() << "\" exists already." << std::endl;
    return;
  }

  fWritable = true;
  fSeekParent= aParent->seekDirectory();
  int nbytes = recordSize();
  Key* key = new Key(fFile,aParent->seekDirectory(),
                     fName,fTitle,fClass.inStoreName(),nbytes);
  fNbytesName = key->keyLength();
  fSeekDirectory = key->seekKey();
  if(fSeekDirectory == 0) {
    fOut << "Rio::Directory::Directory : bad key." << std::endl;
    delete key;
    return;
  }
 {char* buffer = key->dataBuffer();
  if(!fillBuffer(key->eob(),buffer)) {
    fOut << "Rio::Directory::Directory : "
         << "directory name \"" << name() 
         << "\" cannot fill buffer." << std::endl;
    return;
  }}
  int cycle = aParent->appendKey(key);
  int n;
  if(!key->writeFile(cycle,n)) {
    fOut << "Rio::Directory::Directory : "
         << "directory name \"" << name() 
         << "\" cannot write key to file." << std::endl;
    return;
  }

  fModified = false;
  fIsValid = true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Directory::Directory(
 const Directory& aDirectory
)
:IObject(aDirectory)
,IDirectory(aDirectory)
,Named(aDirectory.fFile.dictionary(),"","")
,fClass(aDirectory.fClass)
,fIDirectoryClass(*aDirectory.fFile.dictionary().findClass("Rio::IDirectory"))
,fFile(aDirectory.fFile)
,fOut(aDirectory.fOut)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Directory::~Directory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fFile.verboseLevel()) {
    fOut << "Rio::Directory::~Directory : called for \"" 
         << name() << "\" : begin." << std::endl;
  }

  //TCollection::StartGarbageCollection();

  fIsClosing = true;

  std::list<IObject*>::iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();ito=fObjects.erase(ito)) 
    delete (*ito);

  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();itk=fKeys.erase(itk)) delete (*itk);

  //TCollection::EmptyGarbageCollection();

  if (fFile.verboseLevel()) {
    fOut << "Rio::Directory::~Directory : called for \"" 
         << name() << "\" : end." << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::operator=(
 const Directory& 
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::initialize(
)
//////////////////////////////////////////////////////////////////////////////
// Initialise directory to defaults :
// If directory is created via default ctor (when dir is read from file)
// don't add it here to the directory since its name is not yet known.
// It will be added to the directory in TKey::ReadObj().
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fParent && fName.size()) fParent->appendObject(this);
  fModified = true;
  fWritable = false;
  fDateC.set();
  fDateM.set();
  fNbytesKeys = 0;
  fSeekDirectory = 0;
  fSeekParent = 0;
  fSeekKeys = 0;
  //FIXME SetBit(kCanDelete);
}

std::ostream& Rio::Directory::out() const {return fOut;}

//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory* Rio::Directory::parent(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fParent;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::IObject*> Rio::Directory::objects(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Rio::IObject*> objects;
  std::list<IObject*>::const_iterator it;
  for(it=fObjects.begin();it!=fObjects.end();++it) objects.push_back(*it);
  return objects;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::IKey*> Rio::Directory::keys(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Rio::IKey*> keys;
  std::list<Key*>::const_iterator it;
  for(it=fKeys.begin();it!=fKeys.end();++it) keys.push_back(*it);
  return keys;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::Directory::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) { 
    return Rio_SCast(Directory);
  } else if(&aClass==&fIDirectoryClass) {
    return Rio_SCast(IDirectory);
  } else {
    return Named::cast(aClass);
  }
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Directory::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return Named::name();
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Directory::title(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return Named::title();
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::isWritable(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fWritable; 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::isClosing(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fIsClosing;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::isValid(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fIsValid;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IFile& Rio::Directory::file(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFile;
}
//////////////////////////////////////////////////////////////////////////////
std::string Rio::Directory::path(
) const
//////////////////////////////////////////////////////////////////////////////
// WARNING : returned path starts with /<file name> 
//           which is the name of the file top directory.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string path = "";
  const IDirectory* directory = this;
  while(directory) {
    std::string s = path;
    path = "/";
    path += directory->name();
    path += s;
    directory = directory->parent();
  }
  return path;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::setSeekDirectory(
 Seek aSeek
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fSeekDirectory = aSeek;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::Directory::seekKeys(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fSeekKeys;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::Directory::seekDirectory(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fSeekDirectory;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::nBytesName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fNbytesName;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::setNbytesName(
 int aNbytesName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNbytesName = aNbytesName;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key* Rio::Directory::getKey(
 const std::string& aName
,short aCycle
)
//////////////////////////////////////////////////////////////////////////////
// Return pointer to key with name, cycle
//  if aCycle = 9999 returns highest cycle
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==aName) {
      if(aCycle==9999) return *itk;
      if(aCycle>=(*itk)->cycle()) return *itk;
    }
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key* Rio::Directory::findKey(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Key*>::const_iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==aName) return *itk;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::removeKey(
 Rio::Key* aKey
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fKeys.remove(aKey);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::appendObject(
 IObject* aObject
)
//////////////////////////////////////////////////////////////////////////////
// Append object to current directory.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aObject) return;
  if (fFile.verboseLevel()) {
    fOut << "Rio::Directory::appendObject : directory " 
         << this << " \"" << name() << "\" ; appending " 
         << aObject << " \"" << aObject->name() << "\" of class \""
         << aObject->isA().name() << "\"." << std::endl;
  }

  fObjects.push_back(aObject);

  /*FIXME
  if (!fMother) return;
  if (fMother->IsA() == TMapFile::Class()) {
    TMapFile *mfile = (TMapFile*)fMother;
    mfile->Add(obj);
    }*/
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::removeObject(
 IObject* aObject
)
//////////////////////////////////////////////////////////////////////////////
// It does not delete the object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aObject) return;
  if (fFile.verboseLevel()) {
    fOut << "Rio::Directory::removeObject : directory "
         << this << " \"" << name() << "\" ; removing "
         << aObject << " \"" << aObject->name() << "\" of class \""
         << aObject->isA().name() << "\"." << std::endl;
  }

  fObjects.remove(aObject);

}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::appendKey(
 Key* aKey
)
//////////////////////////////////////////////////////////////////////////////
// Insert key in the list of keys of this directory.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fModified = true;

  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==aKey->name()) {
      fKeys.insert(itk,aKey); //aKey will be before *itk.
      return ((*itk)->cycle() + 1);
    }
  }

  // Not found :
  fKeys.push_back(aKey);
  return 1;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Directory::recordSize(
) const
//////////////////////////////////////////////////////////////////////////////
// (Name, title) are stored in the (name, title) of the associated key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nbytes = sizeof(short);
  nbytes += fDateC.recordSize();
  nbytes += fDateM.recordSize();
  nbytes += sizeof fNbytesKeys;
  nbytes += sizeof fNbytesName;
#ifdef RIO_LARGE_FILE
  if(fFile.version()>=40000) {
    nbytes += sizeof(Seek);
    nbytes += sizeof(Seek);
    nbytes += sizeof(Seek);
  } else {
    nbytes += sizeof(Seek32);
    nbytes += sizeof(Seek32);
    nbytes += sizeof(Seek32);
  }
#else
  nbytes += sizeof fSeekDirectory;
  nbytes += sizeof fSeekParent;
  nbytes += sizeof fSeekKeys;
#endif
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::fillBuffer(
 const char* aEOB
,char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Encode directory header into output buffer.
// (Name, title) are stored in the (name, title) of the associated key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version = fClass.version();
#ifdef RIO_LARGE_FILE
  if((fSeekDirectory>RIO_START_BIG_FILE) || //GB
     (fSeekParent>RIO_START_BIG_FILE)    || //GB
     (fSeekKeys>RIO_START_BIG_FILE) ) version += 1000;
#endif
  if(!tobuf(fOut,aEOB,aBuffer, version)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fDateC.date())) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fDateM.date())) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fNbytesKeys)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fNbytesName)) return false;
#ifdef RIO_LARGE_FILE
  if(version>1000) {
    if(!tobuf(fOut,aEOB,aBuffer, fSeekDirectory)) return false;
    if(!tobuf(fOut,aEOB,aBuffer, fSeekParent)) return false;
    if(!tobuf(fOut,aEOB,aBuffer, fSeekKeys)) return false;
  } else {
    if(fSeekDirectory>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::Directory::fillBuffer :"
           << " attempt to write big Seek "
           << fSeekDirectory << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,aEOB,aBuffer, (Seek32)fSeekDirectory)) return false;
    if(fSeekParent>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::Directory::fillBuffer :"
           << " attempt to write big Seek "
           << fSeekParent << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,aEOB,aBuffer, (Seek32)fSeekParent)) return false;
    if(fSeekKeys>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::Directory::fillBuffer :"
           << " attempt to write big Seek "
           << fSeekKeys << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,aEOB,aBuffer, (Seek32)fSeekKeys)) return false;
  }
#else
  if(!tobuf(fOut,aEOB,aBuffer, fSeekDirectory)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fSeekParent)) return false;
  if(!tobuf(fOut,aEOB,aBuffer, fSeekKeys)) return false;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::readBuffer(
 const char* aEOB
,char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Decode input buffer.
// (Name, title) are stored in the (name, title) of the associated key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short versiondir;
  if(!frombuf(fOut,aEOB,aBuffer,&versiondir)) return false;
  unsigned int date;
  if(!frombuf(fOut,aEOB,aBuffer,&date)) return false;
  fDateC.setDate(date);
  if(!frombuf(fOut,aEOB,aBuffer,&date)) return false;
  fDateM.setDate(date);
  if(!frombuf(fOut,aEOB,aBuffer, &fNbytesKeys)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, &fNbytesName)) return false;
#ifdef RIO_LARGE_FILE
  if(versiondir>1000) {
    if(!frombuf(fOut,aEOB,aBuffer, &fSeekDirectory)) return false;
    if(!frombuf(fOut,aEOB,aBuffer, &fSeekParent)) return false;
    if(!frombuf(fOut,aEOB,aBuffer, &fSeekKeys)) return false;
  } else {
   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer, &i)) return false;
    fSeekDirectory = i;}

   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer, &i)) return false;
    fSeekParent = i;}

   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer, &i)) return false;
    fSeekKeys = i;}
  }
#else
  if(!frombuf(fOut,aEOB,aBuffer, &fSeekDirectory)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, &fSeekParent)) return false;
  if(!frombuf(fOut,aEOB,aBuffer, &fSeekKeys)) return false;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream a class object
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    initialize();
    if (fFile.isWritable()) fWritable = true;
    short version;
    if(!aBuffer.readVersion(version)) return false;
    unsigned int date;
    if(!aBuffer.read(date)) return false;
    fDateC.setDate(date);
    if(!aBuffer.read(date)) return false;
    fDateM.setDate(date);
    if(!aBuffer.read(fNbytesKeys)) return false;
    if(!aBuffer.read(fNbytesName)) return false;
#ifdef RIO_LARGE_FILE
    if(version>1000) {
      if(!aBuffer.read(fSeekDirectory)) return false;
      if(!aBuffer.read(fSeekParent)) return false;
      if(!aBuffer.read(fSeekKeys)) return false;
    } else {
     {Seek32 i;
      if(!aBuffer.read(i)) return false;
      fSeekDirectory = i;}

     {Seek32 i;
      if(!aBuffer.read(i)) return false;
      fSeekParent = i;}

     {Seek32 i;
      if(!aBuffer.read(i)) return false;
      fSeekKeys = i;}
    }
    //short v = version%1000;
#else
    if(!aBuffer.read(fSeekDirectory)) return false;
    if(!aBuffer.read(fSeekParent)) return false;
    if(!aBuffer.read(fSeekKeys)) return false;
#endif
    if (fSeekKeys) {
      int n;
      if(!readKeys(n)) {
        fOut << "Rio::Directory::stream : cannot read keys." << std::endl;
        return false;
      }
    }
  } else {
    short version = fClass.version();
#ifdef RIO_LARGE_FILE
    //GB if(fFile.end()>RIO_START_BIG_FILE) version += 1000;
    if((fSeekDirectory>RIO_START_BIG_FILE) ||
       (fSeekParent>RIO_START_BIG_FILE)    ||
       (fSeekKeys>RIO_START_BIG_FILE) ) version += 1000; //GB
#endif
    if(!aBuffer.writeVersion(version)) return false;
    if(!aBuffer.write(fDateC.date())) return false;
    if(!aBuffer.write(fDateM.date())) return false;
    if(!aBuffer.write(fNbytesKeys)) return false;
    if(!aBuffer.write(fNbytesName)) return false;
#ifdef RIO_LARGE_FILE
    if(version>1000) {
      if(!aBuffer.write(fSeekDirectory)) return false;
      if(!aBuffer.write(fSeekParent)) return false;
      if(!aBuffer.write(fSeekKeys)) return false;
    } else {
      if(fSeekDirectory>RIO_START_BIG_FILE) { //GB
        fOut << "Rio::Directory::stream :"
             << " attempt to write big Seek "
             << fSeekDirectory << " on 32 bits."
             << std::endl;
        return false;
      }
      if(!aBuffer.write((Seek32)fSeekDirectory)) return false;
      if(fSeekParent>RIO_START_BIG_FILE) { //GB
        fOut << "Rio::Directory::stream :"
             << " attempt to write big Seek "
             << fSeekParent << " on 32 bits."
             << std::endl;
        return false;
      }
      if(!aBuffer.write((Seek32)fSeekParent)) return false;
      if(fSeekKeys>RIO_START_BIG_FILE) { //GB
        fOut << "Rio::Directory::stream :"
             << " attempt to write big Seek "
             << fSeekKeys << " on 32 bits."
             << std::endl;
        return false;
      }
      if(!aBuffer.write((Seek32)fSeekKeys)) return false;
    }
#else
    if(!aBuffer.write(fSeekDirectory)) return false;
    if(!aBuffer.write(fSeekParent)) return false;
    if(!aBuffer.write(fSeekKeys)) return false;
#endif
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::writeKeys(
)
//////////////////////////////////////////////////////////////////////////////
// Write KEYS list on the file :
//  The list of keys (fKeys) is written as a single data record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Delete the old keys structure if it exists
  if(fSeekKeys) {
    if(!fFile.makeFreeSegment
       (fSeekKeys, fSeekKeys + fNbytesKeys -1)) return false;
  }
  // Write new keys record :
  int nkeys  = fKeys.size();
  int nbytes = sizeof nkeys;          // Compute size of all keys

//FIXME : the below (found in ROOT) is not understood.
#ifdef RIO_LARGE_FILE
  // 8 = 2*sizeof(Seek)-2*sizeof(Seek32)
  if(fFile.end()>RIO_START_BIG_FILE) nbytes += 8;
#endif

 {std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    nbytes += (*itk)->keyLength();
  }}

  Key headerkey(fFile,seekDirectory(),
                fName,fTitle,fClass.inStoreName(),nbytes);
  if (headerkey.seekKey() == 0) return false;
  char* buffer = headerkey.dataBuffer();
  if(!tobuf(fOut,headerkey.eob(),buffer,nkeys)) return false;

 {std::list<Key*>::iterator it;
  for(it=fKeys.begin();it!=fKeys.end();++it) {
    if(!((*it)->fillBuffer(headerkey.eob(),buffer))) return false;
  }}
  
  fSeekKeys = headerkey.seekKey();
  fNbytesKeys = headerkey.numberOfBytes();

  if(fFile.verboseLevel()) {
    fOut << "Rio::Directory::writeKeys : write header key \""
         << name() << "\" \"" << fTitle << "\" \"" 
         << fClass.name() << "\" (" << nkeys << ", " 
         << long_tos(fKeys.size()) << ", " << nbytes << ", " 
         << fSeekKeys << ", " << fNbytesKeys << "):" << std::endl;
  }

  int n;
  return headerkey.writeFile(1,n);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::readKeys(
 int& aNumber
)
//////////////////////////////////////////////////////////////////////////////
// Read the KEYS :
//  Every directory has a list (fKeys). This list has been
//  written on the file via writeKeys as a single data record.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNumber = 0;
  std::list<Key*>::iterator it;
  for(it=fKeys.begin();it!=fKeys.end();it=fKeys.erase(it)) delete (*it);

  //In case directory was updated by another process, read new
  //position for the keys
 {int nbytes = fNbytesName + recordSize();
  char* header = new char[nbytes];
  char* buffer = header;
  if(!fFile.setPosition(fSeekDirectory)) {delete [] header;return false;}
  if(!fFile.readBuffer(buffer,nbytes)) {delete [] header;return false;}
  buffer += fNbytesName;
  if(!readBuffer(header+nbytes,buffer)) {delete [] header;return false;}
  delete [] header;}

 {Key headerkey(fFile,fSeekKeys,fNbytesKeys);
  if(!headerkey.readFile()) return false;
  char* buffer = headerkey.dataBuffer();
  if(!headerkey.readBuffer(headerkey.eob(),buffer)) return false;
  int nkeys = 0;
  if(!frombuf(fOut,headerkey.eob(),buffer,&nkeys)) return false;
  for (int i = 0; i < nkeys; i++) {
    Key* key = new Key(fFile);
    if(!key->readBuffer(headerkey.eob(),buffer)) {
      delete key;
      return false;
    }
    fKeys.push_back(key);
  }
  aNumber = nkeys;}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::write(
 int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Write all objects in memory to disk.
// Loop on all objects in memory (including subdirectories).
// A new key is created in the KEYS linked list for each object.
// For allowed options see TObject::Write().
// The directory header info is rewritten on the directory header record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  if(fFile.verboseLevel()) {
    fOut << "Rio::Directory::write : \"" << name() << "\"(" 
         << isWritable() << ") : " 
         << long_tos(fObjects.size()) << " objects." << std::endl;
  }

  if (!isWritable()) {
    fOut << "Rio::Directory::write : \"" 
         << name() << "\" is not writable." << std::endl;
    return false;
  }

  int nbytes = 0;

  // Loop on all objects (including subdirs)
  std::list<IObject*>::iterator it;
  for(it=fObjects.begin();it!=fObjects.end();++it) {
    Directory* dir = Rio::cast<Directory>(*(*it));
    if(dir) {
      int n;
      if(!dir->write(n)) return false;;
      nbytes += n;
    } else {
      int n;
      if(!writeObject(*(*it),n)) return false;
      nbytes += n;
    }
  }

  if(!saveSelf(true)) return false;   // force save itself
  
  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::saveObject(
 IObject& aObject
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Key* key = findKey(aObject.name());
  //wOK will be 0 if Write failed (disk space exceeded)
  int wOK;
  if(!writeObject(aObject,wOK)) return false; 
  if(wOK && key) {
    if(!key->destroy()) return false;
    removeKey(key);
    delete key;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::saveSelf(
 bool aForce
)
//////////////////////////////////////////////////////////////////////////////
// Save Directory keys and header :
//  If the directory has been modified (fModified set), write the keys
//  and the directory header. This function assumes the cd is correctly set.
//
//  It is recommended to use this function in the following situation:
//  Assume a process1 using a directory in Update mode
//    -New objects or modified objects have been written to the directory
//    -You do not want to close the file
//    -You want your changes be visible from another process2 already connected
//     to this directory in read mode
//    -Call this function
//    -In process2, use TDirectory::ReadKeys to refresh the directory
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (isWritable() && (fModified || aForce)) {
    if(!fFile.freeSegments().empty()) {
      if(!writeKeys()) return false; // Write keys record.
      if(!writeHeader()) return false; // Update directory record.
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::writeHeader(
)
//////////////////////////////////////////////////////////////////////////////
// Overwrite the Directory header record.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nbytes = recordSize();
  char* header = new char[nbytes];
  char* buffer = header;
  fDateM.set();
  if(!fillBuffer(header+nbytes,buffer)) {
    delete [] header;
    return false;
  }
  // do not overwrite the name/title part
  Seek pointer = fSeekDirectory + fNbytesName; 
  fModified = false;
  if(!fFile.setPosition(pointer)) {
    delete [] header;
    return false;
  }
  if(!fFile.writeBuffer(header, nbytes)) {
    delete [] header;
    return false;
  }
  fFile.synchronize();
  delete [] header;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::close(
)
//////////////////////////////////////////////////////////////////////////////
// Delete all objects from memory and directory structure itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME TCollection::StartGarbageCollection();

  // Save the directory key list and header
  if(!save()) return false;

  // Delete objects from directory list, this in turn, recursively closes all
  // sub-directories (that were allocated on the heap)
  fIsClosing = true;

  std::list<IObject*>::iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();ito=fObjects.erase(ito)) {
    if (fFile.verboseLevel()) {
      fOut << "Rio::Directory::close : directory \"" << name()
           << "\" ; deleting \"" << (*ito)->name() 
           << "\" of class \"" << (*ito)->isA().name() << "\"." << std::endl;
    }

    delete (*ito);

    if (fFile.verboseLevel()) {
      fOut << "Rio::Directory::close : directory \"" 
           << name() << "\" ; deleting end." << std::endl;
    }
  }

  // Delete keys from key list (but don't delete the list header)
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();itk=fKeys.erase(itk)) delete (*itk);

  //FIXME : TCollection::EmptyGarbageCollection();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::save(
)
//////////////////////////////////////////////////////////////////////////////
// Save recursively all directory keys and headers.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!saveSelf()) return false;

  // recursively save all sub-directories
  std::list<IObject*>::iterator it;
  for(it=fObjects.begin();it!=fObjects.end();++it) {
    Directory* dir = Rio::cast<Directory>(*(*it));
    if(dir) if(!dir->save()) return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::writeObject(
 IObject& aObject
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Write this object to the current directory
// The data structure corresponding to this object is serialized.
// The corresponding buffer is written to the current directory
// with an associated key with name "name".
//
// Writing an object to a file involves the following steps:
//
//  -Creation of a support TKey object in the current directory.
//   The TKey object creates a TBuffer object.
//
//  -The TBuffer object is filled via the class::Streamer function.
//
//  -If the file is compressed (default) a second buffer is created to
//   hold the compressed buffer.
//
//  -Reservation of the corresponding space in the file by looking
//   in the TFree list of free blocks of the file.
//
//  -The buffer is written to the file.
//
//  Bufsize can be given to force a given buffer size to write this object.
//  By default, the buffersize will be taken from the average buffer size
//  of all objects written to the current file so far.
//
//  If a name is specified, it will be the name of the key.
//  If name is not given, the name of the key will be the name as returned
//  by GetName().
//
//  The option can be a combination of:
//    kSingleKey and kOverwrite
//  Using the kOverwrite option a previous key with the same name is
//  overwritten.
//  The kSingleKey option is only used by TCollection::Write() to write
//  a container with a single key instead of each object in the container
//  with its own key.
//
//  An object is read from the file into memory via TKey::Read() or
//  via TObject::Read().
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  if(!fFile.isWritable()) {
    fOut << "Rio::Directory::writeObject : file \"" 
         << fFile.name() << "\" is not writable." << std::endl;
    return false;
  }

  int bsize = fFile.bestBuffer();

  std::string name = aObject.name();
  smanip::strip(name);

  /*
  if ((aOption & Rio_kOverWrite)) {
    std::list<Key*>::iterator it;
    for(it=fKeys.begin();it!=fKeys.end();++it) {
      if((*it)->name()==name) {
        delete *it;
        fKeys.erase(it);
        break;
      }
    }
  }
  */

  Key* key = new Key(fFile,*this,aObject,name,bsize);
  if (!key->seekKey()) {
    fKeys.remove(key);
    delete key;
    return false;
  }

  fFile.sumBuffer(key->objectSize());

  if(fFile.verboseLevel()) {
    fOut << "Rio::Directory::writeObject : \"" 
         << aObject.name() << "\"." << std::endl;
  }

  return key->writeFile(1,aNbytes);
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory* Rio::Directory::mkdir(
 const std::string& aName
,const std::string& aTitle
)
//////////////////////////////////////////////////////////////////////////////
// Create a sub-directory and return a pointer to the created directory.
// Returns 0 in case of error.
// Note that the directory name cannot contain slashes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aName.empty()) {
    fOut << "Rio::Directory::mkdir : directory name cannot be \"\"." 
         << std::endl;
    return 0;
  }
  if(aName.find("/")!=std::string::npos) {
    fOut << "Rio::Directory::mkdir : directory name \"" 
         << aName << "\" cannot contain a slash." << std::endl;
    return 0;
  }
  IDirectory* dir = 
    new Directory(fFile,this,aName,aTitle==""?aName:aTitle);
  if(!dir->isValid()) {
    fOut << "Rio::Directory::mkdir : directory badly created." << std::endl;
    delete dir;
    return 0;
  }
  return dir;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory* Rio::Directory::findDirectory(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  IObject* object = find(aName);
  if(!object) {
    fOut << "Rio::Directory::findDirectory : object \"" 
         << aName << "\" not found ." << std::endl; 
    return 0;
  }
  IDirectory* dir = Rio::cast<Directory>(*object);
  if(!dir) {
    fOut << "Rio::Directory::findDirectory : object \"" 
         << aName << "\" not a directory." << std::endl; 
    return 0;
  }
  return dir;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::isADirectory(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  IObject* object = find(aName);
  if(!object) return false;
  IDirectory* dir = Rio::cast<Directory>(*object);
  if(!dir) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Directory::find(
 const std::string& aNameCycle
)
//////////////////////////////////////////////////////////////////////////////
// Return pointer to object identified by namecycle.
//   namecycle has the format name;cycle
//   name  = * is illegal, cycle = * is illegal
//   cycle = "" or cycle = 9999 ==> apply to a memory object
//
//   examples:
//     foo   : get object named foo in memory
//             if object is not in memory, try with highest cycle from file
//     foo;1 : get cycle 1 of foo on file
//
// WARNING: Never use Directory::find when namecycle is a directory itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short cycle;
  std::string name;
  decodeNameCycle(aNameCycle,name,cycle);

  if(fFile.verboseLevel()) {
    fOut << "Rio::Directory::find : namecycle \"" 
         << aNameCycle << "\" : \"" << name << "\" " << cycle << std::endl;
  }

  // Case of Object in memory :
  std::list<IObject*>::const_iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();++ito) {
    if((*ito)->name()==name) {
      if (cycle == 9999) return *ito;
      /*FIXME
        if (idcur->InheritsFrom(TCollection::Class()))
        idcur->Delete();  // delete also list elements
        delete idcur; //FIXME ?
      */
      break;
    }
  }

  // Case of Key :
  std::list<Key*>::iterator itk;
  for(itk=fKeys.begin();itk!=fKeys.end();++itk) {
    if((*itk)->name()==name) {
      if ((cycle == 9999) || (cycle == (*itk)->cycle())) {
        return (*itk)->readObject(*this);
      }
    }
  }

  return 0;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Directory::remove(
 const std::string& aNameCycle
)
//////////////////////////////////////////////////////////////////////////////
// Delete Objects or/and keys in a directory :
//   namecycle has the format name;cycle
//   namecycle = "" same as namecycle ="T*"
//   name  = * means all
//   cycle = * means all cycles (memory and keys)
//   cycle = "" or cycle = 9999 ==> apply to a memory object
//   When name=* use T* to delete subdirectories also
//
//   To delete one directory, you must specify the directory cycle,
//      eg.  file.Delete("dir1;1");
//
//   examples:
//     foo   : delete object named foo in memory
//     foo*  : delete all objects with a name starting with foo
//     foo;1 : delete cycle 1 of foo on file
//     foo;* : delete all cycles of foo on file and also from memory
//     *;2   : delete all objects on file having the cycle 2
//     *;*   : delete all objects from memory and file
//    T*;*   : delete all objects from memory and file and all subdirectories
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short cycle;
  std::string pattern;
  decodeNameCycle(aNameCycle,pattern,cycle);
  //printf("debug : Rio::remove : \"%s\" \"%s\" %d\n",
  // aNameCycle.c_str(),pattern.c_str(),cycle);

  bool deleteall = false;
  bool deletetree = false;
  if(pattern=="*") deleteall = true;
  if(pattern=="*T") { 
    deleteall = true; 
    deletetree = true;
  }
  if(pattern=="T*") { 
    deleteall = true; 
    deletetree = true;
  }
  if(aNameCycle.size() == 0) { 
    deleteall = true; 
    deletetree = true;
  }

  //---------------------Case of Object in memory---------------------
  //                     ========================
  if (cycle >= 9999 ) {
    std::list<IObject*>::iterator it;
    for(it=fObjects.begin();it!=fObjects.end();) {
      bool deleteOK = false;
      if (deleteall || smanip::match((*it)->name(),pattern) ) {
        deleteOK = true;
        Directory* dir = Rio::cast<Directory>(*(*it));
        if(dir) {
          if (!deletetree && deleteall) deleteOK = false;
        }
      }
      if (deleteOK) {
        if(fFile.verboseLevel() && (*it)) {
          fOut << "Rio::Directory::remove : delete object "
               << *it << " of class \"" << (*it)->isA().name()
               << "\" and name \"" << (*it)->name() << "\"." << std::endl;
        }

        delete (*it);
        it = fObjects.erase(it);
      } else {
        ++it;
      }
    }
  }

  //---------------------Case of Key---------------------
  //                     ===========
  if (cycle != 9999 ) {
    if (isWritable()) {
      std::list<Key*>::iterator itk;
      for(itk=fKeys.begin();itk!=fKeys.end();) {
        bool deleteOK = false;
        if (deleteall || smanip::match((*itk)->name(),pattern) ) {
          if (cycle == (*itk)->cycle()) deleteOK = true;
          if (cycle > 9999) deleteOK = true;
          if((*itk)->className()=="TDirectory") {
            if (!deletetree && deleteall) deleteOK = false;
            if (cycle == (*itk)->cycle()) deleteOK = true;
          }
        }
        if (deleteOK) {
          if(fFile.verboseLevel() && (*itk)) {
            fOut << "Rio::Directory::remove : delete key "
                 << *itk << " for object class \"" 
                 << (*itk)->className()
                 << "\" and name \"" << (*itk)->name() << "\"." << std::endl;
          }

          Key* key = *itk;
          itk = fKeys.erase(itk);
          if(!key->destroy()) return false; // Do a fDirectory.removeKey(key) !
          removeKey(key);
          delete key;
          fModified = true;
        } else {
          ++itk;
        }
      }
      if (fModified) {
        if(!writeKeys()) return false;   // Write new keys structure
        if(!writeHeader()) return false; // Write new directory header
        // Write new free segments list
        if(!fFile.writeFreeSegments()) return false;
        // Write new file header
        if(!fFile.writeHeader()) return false;
      }
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Directory::findInMemory(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<IObject*>::const_iterator ito;
  for(ito=fObjects.begin();ito!=fObjects.end();++ito) {
    if((*ito)->name()==aName) return *ito;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::print(
 IPrinter& aPrinter
) const
//////////////////////////////////////////////////////////////////////////////
// List Directory contents.
//  Indentation is used to identify the directory tree
//  Subdirectories are listed first, 
//   then objects in memory, then objects on the file
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aPrinter.indent();
  std::ostream& out = aPrinter.out();
  /*FIXME  if(fClassName=="TFile")
  out << fClass.rioname() << "**" << tab << tab << name() 
      << tab << fTitle << std::endl;
    else */
  out << fClass.name() << "*" << "\t\t"  << name() 
      << "\t" << fTitle << std::endl;
  aPrinter.increment();

  bool memobj  = true;
  bool diskobj = true;
  
  if(memobj) {
    std::list<IObject*>::const_iterator it;
    for(it=fObjects.begin();it!=fObjects.end();++it) {
      aPrinter.indent();
      (*it)->out(out);
    }
  }
  
  if(diskobj) {
    std::list<Key*>::const_iterator it;
    for(it=fKeys.begin();it!=fKeys.end();++it) {
      aPrinter.indent();
      out << "KEY: " << (*it)->className() 
          << "\t" << (*it)->name() << ";" << (*it)->cycle() 
          << "\t" << (*it)->title() << std::endl;
    }
  }
  
  aPrinter.decrement();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Directory::decodeNameCycle(
 const std::string& aString
,std::string& aName
,short& aCycle
)
//////////////////////////////////////////////////////////////////////////////
// Decode a namecycle "aap;2" into name "aap" and cycle "2".
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aCycle = 9999;
  int nch = aString.size();
  for (int i = 0; i < nch; i++) {
    if (aString[i] != ';')
      aName += aString[i];
    else {
      if( (i < nch-1) && (aString[i+1] == '*') ) {
        aCycle = 10000;
      } else {
        std::string s = aString.substr(i+1,aString.size()-(i+1));
        int v;
        //::sscanf(aString.c_str()+i+1, "%hd",&aCycle);
        if(!smanip::toint(s,v)) {
          fOut << "Rio::Directory::decodeNameCycle : in \""
               << aString << "\", cycle is not a number." << std::endl;
          aCycle = 10000;
        } else {
          aCycle = (short)v;
        }
      }
      return;
    }
  }
}
