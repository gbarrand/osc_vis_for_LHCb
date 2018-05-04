#ifndef Rio_Directory_h
#define Rio_Directory_h

// Inheritance :
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Core/Named.h>

#include <list>
#include <Rio/Core/Date.h>
#include <Rio/Core/Key.h>

namespace Rio {

class IPrinter;
class File;

class Directory : public Named, public virtual IDirectory {
  RIO_META_HEADER(Directory)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
  virtual void print(IPrinter&) const;
public: //Rio::IDirectory
  virtual IDirectory* mkdir(const std::string&,const std::string& = "");
  virtual void appendObject(IObject*);
  virtual void removeObject(IObject*);
  virtual IObject* find(const std::string&);
  virtual IDirectory* findDirectory(const std::string&);
  virtual bool isADirectory(const std::string&);
  virtual IFile& file() const;
  virtual std::ostream& out() const;
  virtual Seek seekDirectory() const;
  virtual bool isWritable() const;
  virtual const std::string& name() const;
  virtual const std::string& title() const;
  virtual bool isClosing() const;
  virtual bool saveObject(IObject&);
  virtual bool remove(const std::string&);
  virtual IObject* findInMemory(const std::string&);
  virtual IDirectory* parent() const;
  virtual std::string path() const;
  virtual std::vector<IObject*> objects() const;
  virtual std::vector<IKey*> keys() const;
  virtual bool isValid() const;
public:
  Directory(File&,Directory*);
  Directory(File&,Directory*,
            const std::string&,
            const std::string& = "");
  virtual ~Directory();
  // Get/set :
  Seek seekKeys() const;
  int nBytesName() const;
  void setNbytesName(int);
  // Else :
  virtual int recordSize() const;
  virtual bool fillBuffer(const char*,char*&);
  virtual bool readBuffer(const char*,char*&);
  virtual void setSeekDirectory(Seek);
  virtual bool writeKeys();
  virtual bool readKeys(int&);
  virtual int appendKey(Key*);
  virtual void removeKey(Key*);
  virtual Key* findKey(const std::string&) const;
  virtual bool write(int&);
  virtual bool close();
  virtual bool writeObject(IObject&,int&);
private:
  void initialize();
  Directory(const Directory&);
  void operator=(const Directory &);
  bool writeHeader();
  bool saveSelf(bool=false);
  bool save();
  Key* getKey(const std::string&,short = 9999);
  void decodeNameCycle(const std::string&,std::string&,short&);
private:
  const IClass& fIDirectoryClass;
  File& fFile;
  std::ostream& fOut;
  Directory* fParent;
  bool fIsValid;
  bool fIsClosing;
  //
  std::list<IObject*> fObjects; //Pointer to objects list in memory
  std::list<Key*> fKeys;        //Pointer to keys list in memory
  bool fModified;        //true if directory has been modified
  bool fWritable;        //true if directory is writable
  // Record (stored in file):
  Date fDateC;           //Date and time when directory is created
  Date fDateM;           //Date and time of last modification
  int  fNbytesKeys;      //Number of bytes for the keys
  int  fNbytesName;      //Number of bytes in TNamed at creation time
  Seek fSeekDirectory;   //Location of directory on file
  Seek fSeekParent;      //Location of parent directory on file
  Seek fSeekKeys;        //Location of Keys record on file
};

}

#endif
