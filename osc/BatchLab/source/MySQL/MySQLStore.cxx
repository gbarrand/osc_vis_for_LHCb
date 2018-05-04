// this :
#include <BatchLab/MySQL/MySQLStore.h>

// Lib :
#include <Lib/smanip.h>
#include <inlib/cast>

// BatchLab :
#include <BatchLab/MySQL/MySQLManager.h>

// MySQL :
#include <mysql.h>

//////////////////////////////////////////////////////////////////////////////
BatchLab::MySQLStore::MySQLStore(
 std::ostream& a_out
,MySQLManager& aManager
,const std::string& aDataBase
,const std::string&
,int aVerboseLevel
)
:BatchLab::BaseStore(aPrinter)
,fVerboseLevel(aVerboseLevel)
,fMySQL(0)
,fConnection(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //if(aDataBase.empty()) return;

  fMySQL = aManager.mysql();
  if(!fMySQL) {
    f_out << "BatchLab::MySQLStore::MySQLStore :"
        << " MySQL not inited." << std::endl;
    return;
  }

  std::vector<std::string> words;
  Lib::smanip::words(aDataBase," ",words);
  if(words.size()<3) {
    f_out << "BatchLab::MySQLStore::MySQLStore :"
        << " for database name, at least threee fields expected " 
        << "\"host database user [passowrd]\""
        << " (Given \"" << aDataBase << "\")."
        << std::endl;
    return;
  }

  std::string host = words[0];
  std::string db = words[1];
  std::string user = words[2];
  std::string passwd = (words.size()==4 ? words[3] : "");
    
  unsigned int port = 3306;
    
  // Connect to the MySQL Db at host :
  fConnection = mysql_real_connect(fMySQL,
                                   host.c_str(),
                                   user.c_str(),
                                   passwd.c_str(),
                                   db.c_str(),
                                   port,0,0);
  if(!fConnection) {
    std::string s = mysql_error(fMySQL);
    f_out << "BatchLab::MySQLStore::MySQLStore :"
        << " " << s << std::endl;
    return;
  }

  if(fVerboseLevel) {
    f_out << "BatchLab::MySQLStore::MySQLStore :"
        << " connected to host \"" << host << "\""
        << " base \"" << db << "\""
        << " user \"" << user << "\""
        << std::endl;
  }
  
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::MySQLStore::~MySQLStore(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fConnection) mysql_close(fConnection);
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::MySQLStore::connection(
 MYSQL*& aMYSQL
,MYSQL*& aConnection
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aMYSQL = fMySQL;
  aConnection = fConnection;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::MySQLStore::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fVerboseLevel = aVerboseLevel;
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::MySQLStore::cast(
 const std::string& aTo
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aTo=="BatchLab::MySQLStore") {
    return INLIB_SCAST(BatchLab::MySQLStore);
  } else {
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLStore::commit(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if(!fMySQLFile) {
    f_out << "BatchLab::MySQLStore::commit : no storage open." << std::endl;
    return false;
  }
  int n;
  return fMySQLFile->write(n);
  */
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLStore::mkdir(
 const std::string& //aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if(!fDirectory) {
    f_out << "BatchLab::MySQLStore::mkdir : no container open." << std::endl;
    return false;
  }
  if(fDirectory->findInMemory(aName)) return true; //Done.
  return fDirectory->mkdir(aName,"") ? true : false;
  */
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLStore::cd(
 const std::string& //aName
)
//////////////////////////////////////////////////////////////////////////////
// aName is a relative name.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if(!fDirectory) {
    f_out << "BatchLab::MySQLStore::cd : no container open." << std::endl;
    return false;
  }
  if(aName.empty()) return true;
  if(aName=="..") {
    if(fDirectory->parent()) fDirectory = fDirectory->parent();
    return true;
  }
  MySQL::IDirectory* dir = fDirectory->findDirectory(aName);
  if(!dir) return false;
  fDirectory = dir;
  */
  return true;
}
  /*
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLStore::setRootDirectory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fMySQLFile) {
    f_out << "BatchLab::MySQLStore::setRootDirectory : no file open." << std::endl;
    fDirectory = 0;
    return false;
  }
  fDirectory = &(fMySQLFile->directory());
  return true;
}
  */
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLStore::rootDirectoryName(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "";
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLStore::path(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "";
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::MySQLStore::ls(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fConnection) {
    f_out << "BatchLab::MySQLStore::ls : no database open." << std::endl;
    return;
  }
  // Get the list of the db tables
  MYSQL_RES* result = mysql_list_tables(fConnection,NULL);
  if(!result) {
    std::string s = mysql_error(fMySQL);
    f_out << "BatchLab::MySQLStore::ls :"
        << " " << s << std::endl;
    return;
  }

  f_out << "BatchLab::MySQLStore::ls :"
      << " Number of tables : " << (unsigned long)mysql_num_rows(result)
      << std::endl;

  unsigned int num_fields = mysql_num_fields(result);
  MYSQL_ROW  row;
  while( ( row = mysql_fetch_row(result) ) != NULL )  {
    //unsigned long* lengths = mysql_fetch_lengths(result);
    f_out << "BatchLab::MySQLStore::ls :";
    for( unsigned int i = 0; i < num_fields; i++ ) {
      f_out << " " << std::string(row[i] ? row[i] : "NULL");
    }
    f_out << std::endl;
  }
  
  mysql_free_result(result);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLStore::keys(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  std::string sout;
  MySQL::IDirectory* cursav = fDirectory;
  setRootDirectory();
  if(fDirectory) {
    sout += "<tree>";
    visitTree("",sout);
    sout += "</tree>";
  } 
  fDirectory = cursav;
  return sout;
  */
  return "";
}
