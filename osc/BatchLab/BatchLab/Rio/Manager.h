#ifndef BatchLab_Rio_Manager_h
#define BatchLab_Rio_Manager_h

//Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Store/IStoreManager.h>

#include <vector>

namespace Slash {namespace Core {class ISession;}}

namespace Rio {class IDictionary;}

namespace Slash {namespace Store {class ITree;}}
namespace Slash {namespace Store {class IObject;}}

namespace BatchLab {namespace Rio {class ReadContext;}}
namespace BatchLab {namespace Rio {class WriteContext;}}

namespace BatchLab {

namespace Rio {

typedef bool (*Writer)(void*,WriteContext&);
typedef Slash::Store::IObject* (*Reader)(ReadContext&,bool&);

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
  ::Rio::IDictionary& dictionary() const;
  std::ostream& out() const;

  void addWriter(const std::string&,Writer,bool=false);
  Writer findWriter(const std::string&);
  void addReader(const std::string&,Reader,bool=false);
  Reader findReader(const std::string&);
private:
  void addDefaultReaders();
  bool addDefaultAllocators();
private:
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
private:
  std::string fName;
  Slash::Core::ISession& fSession;
  ::Rio::IDictionary* fDictionary;
  std::vector< std::pair<std::string,Writer> > fWriters;
  std::vector< std::pair<std::string,Reader> > fReaders;
};

} //Rio

} //BatchLab

#endif


