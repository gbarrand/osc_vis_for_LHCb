#ifndef BatchLab_HDF5_Manager_h
#define BatchLab_HDF5_Manager_h

//Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Store/IStoreManager.h>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace Store {class ITree;}}
namespace Slash {namespace Store {class IObject;}}

#include <vector>

namespace BatchLab {namespace HDF5 {class WriteContext;}}
namespace BatchLab {namespace HDF5 {class ReadContext;}}

namespace BatchLab {

namespace HDF5 {

typedef bool (*Writer)(void*,WriteContext&);
typedef Slash::Store::IObject* (*Reader)(ReadContext&);

class Manager 
:public virtual Slash::Core::IManager
,public virtual Slash::Store::IStoreManager {
public: //Slash::Core::IManager
  virtual std::string name() const;
  virtual void* cast(const std::string&) const;
public: //Slash::Store::IStoreManager
  virtual Slash::Store::ITree* createTree(const std::string&,
                                  bool = false, 
                                  bool = false, 
                                  const std::string& = "" );
  virtual Slash::Store::IObject* createObject(Slash::Store::ITree&,
                                  const std::string&,
                                  const std::string&,
                                  const std::string&,
                                  const std::string&,
 			          const std::vector<std::string>&,
                                  const std::string& = "");
  virtual Slash::Store::IObject* createObject(Slash::Store::ITree&,
                                  const std::string&,
                                  const std::string&,
                                  const std::string&,
                                  const std::string&,
   			          const std::vector<Slash::Store::IStoreManager::Range>&,
                                  const std::string& = "");
public:
  Manager(Slash::Core::ISession&);
  virtual ~Manager();
  void addWriter(const std::string&,Writer,bool=false);
  Writer findWriter(const std::string&);
  void addReader(const std::string&,Reader,bool=false);
  Reader findReader(const std::string&);
private:
  /*
  Slash::Store::IObject* createTuple(Slash::Store::ITree&,
                                     const std::string&,
                                     const std::string&,
                                     const std::string&,
                                     const std::string&);
  Slash::Store::IObject* createHistogram1D(Slash::Store::ITree&,
                                    const std::string&,
                                    const std::string&,
                                    const std::string&,
                                    int,double,double);
  Slash::Store::IObject* createHistogram2D(Slash::Store::ITree&,
                                    const std::string&,
                                    const std::string&,
                                    const std::string&,
                                    int,double,double,
                                    int,double,double);
  */
private:
  std::string fName;
  Slash::Core::ISession& fSession;
  std::vector< std::pair<std::string,Writer> > fWriters;
  std::vector< std::pair<std::string,Reader> > fReaders;
};

} //HDF5

} //BatchLab

#endif


