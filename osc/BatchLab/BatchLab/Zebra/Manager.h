#ifndef BatchLab_Zebra_Manager_h
#define BatchLab_Zebra_Manager_h

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Store/IStoreManager.h>

namespace Slash {namespace Core {class ISession;}}

namespace Slash {namespace Store {class ITree;}}
namespace Slash {namespace Store {class IObject;}}

namespace BatchLab {

class ZebraPrinter;

namespace Zebra {

class Manager 
:public virtual Slash::Core::IManager 
,public virtual Slash::Store::IStoreManager 
{
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
  int* get_pawc();
  int* get_quest();
  int* get_hcbits();
  int* get_hcbook();
  int* get_rzcl();
  void setWritingFlag(bool);
  bool writingFlag() const;
private:
  std::string fName;
  Slash::Core::ISession& fSession;
};

} //Zebra

} //BatchLab

#endif


