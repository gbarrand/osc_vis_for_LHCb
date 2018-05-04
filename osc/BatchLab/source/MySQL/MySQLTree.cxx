// this :
#include <BatchLab/MySQL/MySQLTree.h>

// Lib :
#include <Lib/smanip.h>
#include <inlib/cast>

#include <BatchLab/MySQL/MySQLStore.h>

static std::string c2std(const char* aString) {
  if(!aString) return "";
  return std::string(aString);
}

//////////////////////////////////////////////////////////////////////////////
BatchLab::MySQLTree::MySQLTree(
 Slash::Core::ISession& aSession
,const std::string& aStoreName
,bool //aReadOnly
,bool //aCreateNew
,MySQLManager& aManager
,AIDA::ITree& aMemoryTree
,int aVerboseLevel
)
:BaseTree(aSession,aStoreName) //FIXME : have a name # storeName
,fStoreName(aStoreName)
,fStore(0)
,fMemoryTree(aMemoryTree) //FIXME
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  setVerboseLevel(aVerboseLevel);
  // Store deleted by the tree.
  fStore = 
    new BatchLab::MySQLStore(out(),aManager,aStoreName,"",verboseLevel());
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::MySQLTree::~MySQLTree(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(verboseLevel()) {
    f_out << "BatchLab::MySQLTree::~MySQLTree : cleanup..." << std::endl;
  }
  
  //emitDelete();
  //fListeners.clear();

  delete fStore;

  if(verboseLevel()) {
    f_out << "BatchLab::MySQLTree::~MySQLTree : end." << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::MySQLTree::cast(
 const std::string& aTo
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aTo=="BatchLab::MySQLTree")  {
    return INLIB_SCAST(MySQLTree);
  } else {
    return BaseTree::cast(aTo);
  }
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::MySQLTree::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  BaseTree::setVerboseLevel(aVerboseLevel);
  if(!fStore) return;
  fStore->setVerboseLevel(aVerboseLevel);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLTree::storeName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStoreName;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::mkdir(
 const std::string& aFolder
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;
  return fStore->mkdir(aFolder);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::ls(
 const std::string&
,bool
,std::ostream&
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;
  fStore->ls();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::commit(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;
  /*
  std::string currentPath = pwd();
  std::list< std::pair< std::string,AIDA::IManagedObject*> >::iterator it;
  for(it=fList.begin();it!=fList.end();++it) {
    const std::string& path = (*it).first;
    AIDA::IManagedObject* object = (*it).second;
    //printf("debug : commit : \"%s\"\n",path.c_str());
    cd(path);
    fStore->store(*object,object->name());
    //printf("debug : commit : end\n");
  }
  cd(currentPath);
  return fStore->commit();
  */
  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLTree::findPath(
 const AIDA::IManagedObject& aObject
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //return aObject.name();
  return "";
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLTree::pwd(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if(!fStore) return fEmpty;
  return fStore->path();
  */
  return fEmpty;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::rmdir(
 const std::string& /*aFolder*/
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::cd(
 const std::string& //aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::addObject(
 BaseObject* //aObject
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if(!fStore) return;
  if(aObject->name().empty()) {
    f_out << "BatchLab::MySQLTree::addObject :"
        << " WARNING : add object with empty name." << std::endl;
  }
  if(verboseLevel()) {
    f_out << "BatchLab::MySQLTree::addObject :" 
        <<  " \"" << aObject->name() << "\"."
        << std::endl;
  }
  // fList holds the things related to the store (tuple, histograms).
  fList.push_back(std::pair<std::string,AIDA::IManagedObject*>(pwd(),aObject));
  */
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::rm(
 const std::string&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  std::list< std::pair< std::string,AIDA::IManagedObject*> >::iterator it;
  for(it=fList.begin();it!=fList.end();++it) {
    AIDA::IManagedObject* object = (*it).second;
    if(object==aObject) {
      if(verboseLevel()) {
        f_out << "BatchLab::MySQLTree::removeObject :" 
            << " \"" << aObject->name() << "\"."
            << std::endl;
      }
      fList.erase(it);
      return;
    }
  }
  */
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::isObjectValid(
 const AIDA::IManagedObject* //aObject
) const
//////////////////////////////////////////////////////////////////////////////
// Should never pass here.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  std::list< std::pair< std::string,AIDA::IManagedObject*> >::const_iterator it;
  for(it=fList.begin();it!=fList.end();++it) {
    AIDA::IManagedObject* object = (*it).second;
    if(object==aObject) return true;
  }
  */
  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::MySQLTree::hierarchy(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  /*
  if(!fStore) return "";
  return fStore->keys();
  */
  return "";
}

bool BatchLab::MySQLTree::mkdirs ( const std::string&) {
  return false;
}
bool BatchLab::MySQLTree::mv(const std::string&,const std::string&) {
  return false;
}
bool BatchLab::MySQLTree::cp(const std::string&,const std::string&,bool) {
  return false;
}
bool BatchLab::MySQLTree::symlink(const std::string&,const std::string&) {
  return false;
}
bool BatchLab::MySQLTree::mount(const std::string&,AIDA::ITree&,const std::string&) {
  return false;
}
bool BatchLab::MySQLTree::unmount(const std::string&) {
  return false;
}
bool BatchLab::MySQLTree::close() {
   return false;
}
std::vector<std::string> BatchLab::MySQLTree::listObjectNames(
 const std::string&
,bool
) const {
  return std::vector<std::string>();
}
std::vector<std::string> BatchLab::MySQLTree::listObjectTypes(
 const std::string&
,bool
) const {
  return std::vector<std::string>();
}
std::string BatchLab::MySQLTree::findPath(
 const AIDA::IManagedObject&
) const
{
  return "";
}
void BatchLab::MySQLTree::setOverwrite(
 bool
)
{
}


// Data specific area :
#include <inlib/histo/all>
#include <BatchLab/Core/Histogram.h>
//////////////////////////////////////////////////////////////////////////////
AIDA::IManagedObject* BatchLab::MySQLTree::find(
 const std::string& aSID
)
//////////////////////////////////////////////////////////////////////////////
// SID = storage identifier
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  inlib::histo::base_histo hclHistogram;

  if(!getHistogram(aSID,hclHistogram)) return 0;

  BatchLab::Histogram1D* object = 
    new BatchLab::Histogram1D(out(),aSID,hclHistogram);

  BaseTree* batchlabTree = INLIB_CAST(fMemoryTree,BatchLab::BaseTree);
  if(batchlabTree) {
    if(!batchlabTree->manageObject(object)) {
      delete object;
      return 0;
    }
  }

  return object;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::MySQLTree::getHistogram(
 const std::string& aSID
,inlib::histo::base_histo& aHistogram
)
//////////////////////////////////////////////////////////////////////////////
// SID = storage identifier
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;
  MYSQL* mysql;
  MYSQL* connection;
  fStore->connection(mysql,connection);
  if(!mysql || !connection) {
    f_out << "BatchLab::MySQLTree::getHistogram  :"
        << " no database open." << std::endl;
    return false;
  }

  std::string query;
  query  = " SELECT bins,min,binWidth,allEntries,";
  query += " sumOfWeights,sumOfWeightsSquared";
  query += " FROM FixedAxis";
  query += " LEFT JOIN Histogram1D";
  query += " ON (FixedAxis.LIDOID = Histogram1D.xAxis)";
  query += " WHERE (Histogram1D.name = '"  + aSID  + "')";

  if(mysql_query(connection,query.c_str())) {
    f_out << "BatchLab::MySQLTree::getHistogram :"
        << " " << std::string(mysql_error(mysql)) << std::endl;
    return false;
  }

  // Must call mysql_store_result before we can issue any other query calls.
  MYSQL_RES* result = mysql_store_result(connection);
  unsigned int num_fields = mysql_num_fields(result);
  unsigned long rows = (unsigned long)mysql_num_rows(result);
  if( (num_fields!=6) || (rows<=0))  {
    f_out << "BatchLab::MySQLTree::getHistogram :"
        << " unexpected result for query." << std::endl;
    mysql_free_result(result);
    return false;
  }

  MYSQL_ROW row = mysql_fetch_row(result);
  if(!row) {
    mysql_free_result(result);
    return false;
  }


  int binn;
  if(!Lib::smanip::toint(c2std(row[0]),binn)) {
    mysql_free_result(result);
    return false;
  }
  
  double lowerEdge;
  if(!Lib::smanip::todouble(c2std(row[1]),lowerEdge)) {
    mysql_free_result(result);
    return false;
  }

  double binWidth;
  if(!Lib::smanip::todouble(c2std(row[2]),binWidth)) {
    mysql_free_result(result);
    return false;
  }

  int allEntries;
  if(!Lib::smanip::toint(c2std(row[3]),allEntries)) {
    mysql_free_result(result);
    return false;
  }
  
  double sumOfWeights;
  if(!Lib::smanip::todouble(c2std(row[4]),sumOfWeights)) {
    mysql_free_result(result);
    return false;
  }

  double sumOfWeightsSquared;
  if(!Lib::smanip::todouble(c2std(row[5]),sumOfWeightsSquared)) {
    mysql_free_result(result);
    return false;
  }
  mysql_free_result(result);

  inlib::histo::histo_data& data = aHistogram.getData();
  data.fLabel = aSID;
  data.fDimension = 1;
  data.fAxis.resize(1);
  data.fAxis[0].fOffset = 1;

  data.fAxis[0].configure(binn,lowerEdge,lowerEdge + binn * binWidth);
  data.fAxis[0].fSxw = 0;
  data.fAxis[0].fSx2w = 0;
  data.fAxis[0].fUnderFlow = 0;
  data.fAxis[0].fOverFlow = 0;

  data.fEntries = allEntries;//FIXME : should remove outflow.
  data.fSw = sumOfWeights;
  data.fSw2 = sumOfWeightsSquared;
  // Underflow/overflow :
  data.fOutFlow = 0;
  data.fOutSw = 0;

  // Get bins :
  query  = " SELECT LIDOVALUE";
  query += " FROM BasicBinner1D_entries";
  query += " LEFT JOIN Histogram1D";
  query += " ON (BasicBinner1D_entries.LIDOFK = Histogram1D.binner1D)";
  query += " WHERE (Histogram1D.name = '"  + aSID  + "')";
  query += " ORDER BY LIDOPOS";

  if(mysql_query(connection,query.c_str())) {
    f_out << "BatchLab::MySQLTree::getHistogram :"
        << " " << std::string(mysql_error(mysql)) << std::endl;
    return false;
  }

  result = mysql_store_result(connection);
  num_fields = mysql_num_fields(result);
  rows = (unsigned long)mysql_num_rows(result);
  if( (num_fields!=1) || (rows<=0))  {
    f_out << "BatchLab::MySQLTree::getHistogram :"
        << " unexpected results from query." << std::endl;
    mysql_free_result(result);
    return false;
  }

  data.fBinNumber = binn;
  data.fBinEntries.resize(binn);
  data.fBinSw.resize(binn);
  data.fBinSw2.resize(binn);
  data.fBinSwx.resize(binn);

  int index = 0;
  while( ( row = mysql_fetch_row(result) ) != NULL ) {
    double dvalue;
    if(!Lib::smanip::todouble(c2std(row[0]),dvalue)) {
      mysql_free_result(result);
      return false;
    }
    if(index>=binn) break;
    data.fBinEntries[index] = (int)dvalue;; //FIXME : w = 1 only !
    data.fBinSw[index] = dvalue;
    data.fBinSw2[index] = 0; //FIXME
    index++;
  }
  mysql_free_result(result);

  if(verboseLevel()) {
    f_out << "BatchLab::MySQLTree::getHistogram :"
        << " object \"" << aSID << "\" found :" 
        << " " << binn << " " << lowerEdge << " " << binWidth
        << std::endl;
  }
  
  return true;
}
AIDA::ITree* BatchLab::MySQLTree::findTree(const std::string&) {
  return 0; //FIXME
}
