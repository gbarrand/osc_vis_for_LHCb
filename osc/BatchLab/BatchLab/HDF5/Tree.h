#ifndef BatchLab_HDF5_Tree_h
#define BatchLab_HDF5_Tree_h

// Inheritance :
#include <BatchLab/Core/MemoryTree.h>

#include <exlib/hdf5/hdf5_h>

namespace Lib {class ItemML;}

namespace BatchLab {namespace HDF5 {class Manager;}}
namespace BatchLab {namespace HDF5 {class HDF5O;}}

namespace BatchLab {

namespace HDF5 {

class Tree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string&) const;
public: //AIDA/ITree
  virtual AIDA::IManagedObject* find(const std::string&);
  virtual bool commit();
  virtual bool close();
  virtual std::string storeName() const;
  virtual std::string storeType() const; //AIDA-v3r3p0
public: //Slash::Core::IHierarchy
  virtual std::string hierarchy(const std::string& = "") const;
public:
  Tree(Slash::Core::ISession&,const std::string&,bool,bool,const std::string&,
       HDF5::Manager&,int);
  virtual ~Tree();
  bool isValid() const;
private:
  bool writeHeader();
  Slash::Store::IObject* retreiveObject(hid_t,const std::string&);
private:
  HDF5::Manager& fManager;
  std::string fStoreName;
  int fNameCounter;
  bool fReadOnly;
  bool fCreateNew;
  bool fCompress;
  bool fIsValid;
  hid_t fFileID;
  HDF5O* fHDF5O;
};

} //HDF5

} //BatchLab

#endif
