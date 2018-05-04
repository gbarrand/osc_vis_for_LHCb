#ifndef BatchLab_MySQLManager_h
#define BatchLab_MySQLManager_h

#include <Slash/Core/IManager.h>

#include <Slash/Store/IStoreManager.h>

#include <mysql.h>

namespace Slash {namespace Core {class ISession;}}

namespace BatchLab {

class MySQLManager : public virtual Slash::Store::IStoreManager {
public: //Slash::Core::IManager
  virtual const std::string& name() const;
  virtual void* cast(const std::string&) const;
public: //Slash::Store::IStoreManager
  virtual AIDA::ITree* createTree(const std::string&,
                                  bool = false, 
                                  bool = false, 
                                  const std::string& = "" );
  virtual AIDA::ITuple* createTuple(AIDA::ITree&,
                                    const std::string&,
                                    const std::string&,
                                    const std::string&);
public:
  MySQLManager(Slash::Core::ISession&);
  virtual ~MySQLManager();
  MYSQL* mysql() const;
private:
  std::string fName;
  Slash::Core::ISession& fSession;
  MYSQL* fMySQL;
  AIDA::ITree* fMemoryTree;
};

}

#endif


