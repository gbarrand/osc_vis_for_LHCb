#ifndef BatchLab_MySQLStore_h
#define BatchLab_MySQLStore_h


#include <BatchLab/Core/BaseStore.h>

#include <mysql.h>

namespace BatchLab {

class MySQLManager;

class MySQLStore : public BaseStore {
public: //IStore
  virtual void* cast(const std::string&) const;
  virtual bool commit();
  virtual bool mkdir(const std::string&);
  virtual bool cd(const std::string&);
  virtual void ls() const;
  virtual std::string path();
  virtual std::string keys(); 
  virtual std::string rootDirectoryName();
public:
  virtual ~MySQLStore();
  MySQLStore(std::ostream&,MySQLManager&,const std::string&,const std::string&,int);
  void setVerboseLevel(int);
  void connection(MYSQL*&,MYSQL*&);
private:
  int fVerboseLevel;
  MYSQL* fMySQL;
  MYSQL* fConnection;
};

}

#endif
