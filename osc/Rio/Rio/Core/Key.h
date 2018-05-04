#ifndef Rio_Key_h
#define Rio_Key_h

#include <string>
#include <ostream>

#include <Rio/Interfaces/IKey.h>
#include <Rio/Interfaces/Seek.h>
#include <Rio/Core/Date.h>

namespace Rio {

class IObject;
class IBuffer;
class File;
class Directory;
class Buffer;

class Key : public virtual IKey {
public:
  virtual const std::string& name() const;
  virtual const std::string& title() const;
  virtual const std::string& className() const;
  virtual short cycle() const;
  //virtual int version() const {return fVersion;}
  virtual int keyLength() const {return fKeyLength;}
  virtual int numberOfBytes() const {return fNumberOfBytes;}
  virtual int objectSize() const {return fObjectSize;}
public:
  Key(File&);
  Key(File&,Seek,const std::string&,const std::string&,const std::string&,int);
  Key(File&,Seek,int);
  Key(File&,Directory&,IObject&,const std::string&,int);
  virtual ~Key();
public:
  // Get/set :
  Seek seekKey() const {return fSeekKey;}
  Seek seekParentDirectory() const {return fSeekParentDirectory;}
  Rio::Buffer* bufferRef() const { return fBufferRef;}
  // Else :
  virtual char* dataBuffer() const;
  virtual const char* eob() const;
  virtual bool writeFile(int,int&);
  virtual bool readFile();
  virtual bool readBuffer(const char*,char*&);
  virtual bool fillBuffer(const char*,char*&);
  virtual IObject* readObject(Directory&);
  virtual bool destroy();
private:
  Key(const Key&);
  void operator=(const Key&);
private:
  int findRecordSize() const;
  void deleteBuffer();
protected:  
  bool initialize(int);
  bool _stream(IBuffer&);
protected:
  File& fFile;
  std::ostream& fOut;
private:
  int fLeft;           //Number of bytes left in current segment
protected: // Basket inherits Key.
  unsigned int fBufSize;        //Buffer size.
  char* fBuffer;       //Object buffer
  Buffer* fBufferRef;  //Pointer to the Buffer object
  // Record (stored in file) :
  int fNumberOfBytes;        //Number of bytes for the object on file
  int fVersion;              //Key version identifier
  int fObjectSize;           //Length of uncompressed object in bytes
  Date fDate;                //Date/Time of insertion in file
  short fKeyLength;          //Number of bytes for the key itself
  short fCycle;              //Cycle number
  Seek fSeekKey;             //Location of object on file
  Seek fSeekParentDirectory; //Location of parent directory on file
  std::string fObjectClassName;   //Object Class name.
  std::string fObjectName;        //name of the object.
  std::string fObjectTitle;       //title of the object.
};

}

#endif
