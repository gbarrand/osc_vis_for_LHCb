#ifndef BatchLab_MySQLTree_h
#define BatchLab_MySQLTree_h

#include <BatchLab/Core/BaseTree.h>

namespace HCL {
  class Histogram;
}

namespace BatchLab {

class MySQLManager;
class MySQLStore;

// ITree should come first due to BatchLab::castTree function.

class MySQLTree : public BaseTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string&) const;
public: //AIDA/ITree
  virtual AIDA::IManagedObject* find(const std::string&);
  virtual bool cd(const std::string&);
  virtual bool mkdir(const std::string&);
  virtual bool commit();
  virtual std::string findPath(const AIDA::IManagedObject&);
  virtual std::string storeName() const;
  virtual bool rm(const std::string&);

  // To implement :
  virtual std::string pwd() const;
  virtual bool ls(const std::string & = ".", bool = false,std::ostream& = std::cout) const;
  virtual std::vector<std::string> listObjectNames(const std::string& = ".", bool = false) const;
  virtual std::vector<std::string> listObjectTypes(const std::string& = ".", bool = false) const;
  virtual std::string findPath(const AIDA::IManagedObject&) const;
  virtual void setOverwrite(bool = true);
  virtual bool mkdirs ( const std::string&);
  virtual bool rmdir ( const std::string&);
  virtual bool mv(const std::string&,const std::string&);
  virtual bool cp(const std::string&,const std::string&,bool = false );
  virtual bool symlink(const std::string&,const std::string&);
  virtual bool mount(const std::string&,AIDA::ITree&,const std::string&);
  virtual bool unmount(const std::string&);
  virtual bool close();
  virtual AIDA::ITree* findTree(const std::string&);
public: //Slash::Core::IHierarchy
  virtual std::string hierarchy() const; 
public: //BatchLab::BaseTree.
  virtual bool addObject(BaseObject*);
  virtual bool isObjectValid(const AIDA::IManagedObject*) const;
public:
  MySQLTree(Slash::Core::ISession&,const std::string&,bool,bool,
            MySQLManager&,AIDA::ITree&,int);
  virtual ~MySQLTree();
  virtual void setVerboseLevel(int);
private:
  bool getHistogram(const std::string&,inlib::histo::base_histo&);
private:
  std::string fStoreName;
  MySQLStore* fStore;
  std::string fEmpty;
  AIDA::ITree& fMemoryTree;
};

}

#endif
