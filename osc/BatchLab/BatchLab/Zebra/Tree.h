#ifndef BatchLab_Zebra_Tree_h
#define BatchLab_Zebra_Tree_h

#include <BatchLab/Core/MemoryTree.h>

#include <list>

namespace Zebra {class IDirectory;}

namespace BatchLab {namespace Zebra {class Manager;}}

namespace BatchLab {

namespace Zebra {

// Need a MemoryTree to manage read objects.
class Tree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string&) const;
public: //AIDA/ITree
  virtual std::string storeName() const;
  virtual std::string storeType() const;
  virtual bool commit();
  virtual bool close();
  virtual bool mkdir(const std::string&);
  virtual bool mkdirs(const std::string&);
  virtual bool rmdir(const std::string&);
  virtual bool cd(const std::string&);
  virtual AIDA::IManagedObject* find(const std::string&);
  virtual bool ls(const std::string & = ".", bool = false,std::ostream& = std::cout) const;
public: //Slash::Core::IHierarchy
  virtual std::string hierarchy(const std::string& = "") const; 
public:
  Tree(Slash::Core::ISession&,
       const std::string&,bool,bool,const std::string&,Zebra::Manager&,int);
  virtual ~Tree();
  bool isValid() const;
  void toLUN();
  bool hcd(const std::string&);
private:
  Slash::Store::IObject* convertHistogram1D(int);
  Slash::Store::IObject* convertHistogram2D(int);
  Slash::Store::IObject* convertProfile(int);
  Slash::Store::IObject* convertColumnWiseTuple(int);
  Slash::Store::IObject* convertRowWiseTuple(int);
  void ls_dir(bool) const;
  void clone_tree(const std::string&);
  Slash::Store::IObject* retreiveObject(const std::string&);
  void visitTree(std::string&) const; 
private:
  std::string fStoreName;
  Zebra::Manager& fManager;
  bool fIsValid;
  std::string fDirectory;
  int fUnit;
  bool fWriting;
};

} //Zebra

} //BatchLab

#endif
