// this :
#include <BatchLab/MySQL/MySQLManager.h>

// Lib :
#include <inlib/cast>
#include <Slash/Tools/Manager.h>

// BatchLab :
#include <BatchLab/MySQL/MySQLTree.h>

//////////////////////////////////////////////////////////////////////////////
BatchLab::MySQLManager::MySQLManager(
 Slash::Core::ISession& aSession
)
:fName("MySQLManager")
,fSession(aSession)
,fMySQL(0)
,fMemoryTree(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fMemoryTree = Slash_findManager(fSession,"MemoryTree",AIDA::ITree);
  if(!fMemoryTree) {
    std::ostream& out = fSession.cout();
    f_out << "BatchLab::MySQLManager::MySQLManager :"
        << " MemoryTree not found." << std::endl;
    return;
  }

  fMySQL = new MYSQL;
  mysql_init(fMySQL);
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::MySQLManager::~MySQLManager(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete fMySQL;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& BatchLab::MySQLManager::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::MySQLManager::cast(
 const std::string& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aClass=="BatchLab::MySQLManager") {
    return INLIB_SCAST(MySQLManager);
  } else if(aClass=="Slash::Store::IStoreManager") {
    return INLIB_SCAST(Slash::Store::IStoreManager);
  } else if(aClass=="Slash::Core::IManager") {
    return INLIB_SCAST(Slash::Core::IManager);
  } else {
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
MYSQL* BatchLab::MySQLManager::mysql(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fMySQL;
}
//////////////////////////////////////////////////////////////////////////////
AIDA::ITree* BatchLab::MySQLManager::createTree(
 const std::string& aName
,bool //aReadOnly
,bool //aCreateNew
,const std::string&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fMySQL) return 0;
  if(!fMemoryTree) return 0;
  MySQLTree* tree = new MySQLTree(fSession,aName,false,false,
                                  *this,*fMemoryTree,fSession.verboseLevel());
  fSession.addManager(tree);
  return tree;
}
//////////////////////////////////////////////////////////////////////////////
AIDA::ITuple* BatchLab::MySQLManager::createTuple(
 AIDA::ITree&
,const std::string&
,const std::string&
,const std::string&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
