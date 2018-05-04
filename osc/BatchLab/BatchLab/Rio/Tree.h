#ifndef BatchLab_Rio_Tree_h
#define BatchLab_Rio_Tree_h

// Inheritance :
#include <BatchLab/Core/MemoryTree.h>

#include <list>

namespace Rio {class IDirectory;}

namespace BatchLab {namespace Rio {class Manager;}}
namespace BatchLab {namespace Rio {class Store;}}

namespace BatchLab {

namespace Rio {

//class HistogramAllocator;

// Need a MemoryTree to manage read objects.
class Tree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string&) const;
public: //AIDA/ITree
  virtual AIDA::IManagedObject* find(const std::string&);
  virtual bool cd(const std::string&);
  virtual bool mkdir(const std::string&);
  virtual bool mkdirs(const std::string&);
  virtual bool commit();
  virtual bool close();
  virtual std::string findPath(const AIDA::IManagedObject&) const;
  virtual bool ls(const std::string & = ".", bool = false,std::ostream& = std::cout) const;
  virtual bool rm(const std::string&);
  virtual std::string pwd() const;
  virtual bool rmdir ( const std::string&);
  virtual std::string storeName() const;
  virtual std::string storeType() const;
  virtual bool cp(const std::string&,const std::string&,bool = false );
  virtual std::vector<std::string> listObjectNames(const std::string& = ".", bool = false) const;
  virtual std::vector<std::string> listObjectTypes(const std::string& = ".", bool = false) const;
//public: //Slash::Store::ITree.
//  virtual bool addObject(Slash::Store::IObject*,const std::string&);
public: //Slash::Core::IHierarchy
  virtual std::string hierarchy(const std::string& = "") const; 
public: //BatchLab::BaseTree.
  virtual void setVerboseLevel(int);
public:
  Tree(Slash::Core::ISession&,const std::string&,bool,bool,Rio::Manager&,int,
       const std::string&);
  virtual ~Tree();
  //Rio::Store* rioStore() const;
  bool store(::Rio::IDirectory&,const Slash::Store::IObject&);
  ::Rio::IDirectory* makeDirectories(const std::string&);
  bool isValid() const;
private:
  bool import(::Rio::IDirectory&,const std::string&);
  Slash::Store::IObject* retreiveObject(::Rio::IDirectory&,const std::string&);
private:
  Rio::Manager& fManager;
  std::string fStoreName;
  Rio::Store* fStore;
  bool f2ROOT;
  //HistogramAllocator* fAllocator;
};

} //Rio

} //BatchLab

#endif
