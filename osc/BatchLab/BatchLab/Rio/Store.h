#ifndef BatchLab_Rio_Store_h
#define BatchLab_Rio_Store_h

#include <string>

namespace Rio {class IFile;}
namespace Rio {class IDirectory;}

namespace BatchLab {namespace Rio {class Manager;}}

namespace BatchLab {

namespace Rio {

class Store {
public: //IStore
  //virtual void* cast(const std::string&) const;
  virtual bool commit();
  virtual bool mkdir(const std::string&);
  virtual bool mkdirs(const std::string&);
  virtual bool cd(const std::string&);
  virtual void ls() const;
  virtual std::string path();
  virtual std::string hierarchy(const std::string& = ""); 
public:
  static std::string path(const ::Rio::IDirectory&);
public:
  Store(std::ostream&,
        Rio::Manager&,const std::string&,
        const std::string& = "READ",int = 0);
  virtual ~Store();
public:
  ::Rio::IFile* rioFile();
  ::Rio::IDirectory* directory();
  ::Rio::IDirectory* findDirectory(const std::string&);
  bool isADirectory(const std::string&);
  ::Rio::IDirectory* makeDirectories(const std::string&); 
  void setVerboseLevel(int);
  int verboseLevel() const;
  std::string storeName() const;
private:
  void visitTree(::Rio::IDirectory&,bool,std::string&); 
private:
  std::ostream& f_out;
  ::Rio::IFile* fRioFile;
  ::Rio::IDirectory* fDirectory;
  std::string fFile;
  int fVerboseLevel;
};

} //Rio

} //BatchLab

#endif
