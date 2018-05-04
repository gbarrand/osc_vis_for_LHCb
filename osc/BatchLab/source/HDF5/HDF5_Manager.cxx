// this :
#include <BatchLab/HDF5/Manager.h>

// Lib:
#include <Slash/Core/ISession.h>
#include <Slash/Tools/Manager.h>

#include <inlib/cast>

#include <Lib/Debug.h>

// HDF5 :
#include <BatchLab/HDF5/Tree.h>
#include <BatchLab/HDF5/HDF5O.h>

BatchLab::HDF5::Manager::Manager(Slash::Core::ISession& aSession)
:fName("HDF5_Manager")
,fSession(aSession)
{
  if(!HDF5O::checkSizes()) {
    std::ostream& out = aSession.cout();
    out << "BatchLab::HDF5::Manager::Manager :" 
        << " strange machine..."
        << std::endl;  
  }
}

BatchLab::HDF5::Manager::~Manager(){}
std::string BatchLab::HDF5::Manager::name() const {return fName;}

void* BatchLab::HDF5::Manager::cast(const std::string& a_class) const {
  INLIB_IF_CAST(BatchLab::HDF5::Manager)
  else INLIB_IF_CAST(Slash::Store::IStoreManager)
  else INLIB_IF_CAST(Slash::Core::IManager)
  else return 0;
}

Slash::Store::ITree* BatchLab::HDF5::Manager::createTree(
 const std::string& aName
,bool aReadOnly
,bool aCreateNew
,const std::string& aOptions
){
  HDF5::Tree* tree = 
    new HDF5::Tree(fSession,aName,aReadOnly,aCreateNew,aOptions,
                   *this,fSession.verboseLevel());
  if(!tree) return 0;
  if(!tree->isValid()) {
    delete tree;
    return 0;
  }
  fSession.addManager(tree);
  return tree;
}

Slash::Store::IObject* BatchLab::HDF5::Manager::createObject(
 Slash::Store::ITree& //aTree
,const std::string& aClass
,const std::string& //aPath
,const std::string& //aName
,const std::string& //aTitle
,const std::vector<std::string>& aArgs
,const std::string& //aOptions
){
  if(aClass=="AIDA::ITuple") {
    if(aArgs.size()!=1) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::HDF5::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. One expected."
          << std::endl;  
      return 0;
    }
    //return createTuple(aTree,aPath,aName,aTitle,aArgs[0]);
    return 0;
  } else {
    std::ostream& out = fSession.cout();
    out << "BatchLab::HDF5::Manager::createObject :" 
        << " don't know how to create object of class " 
        << inlib::sout(aClass) << "."
        << std::endl;  
    return 0;
  }
}

Slash::Store::IObject* BatchLab::HDF5::Manager::createObject(
 Slash::Store::ITree& //aTree
,const std::string& aClass
,const std::string& //aPath
,const std::string& //aName
,const std::string& //aTitle
,const std::vector<Slash::Store::IStoreManager::Range>& aArgs
,const std::string& //aOptions
){
  if(aClass=="AIDA::IHistogram1D") {
    if(aArgs.size()!=1) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::HDF5::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. One range expected."
          << std::endl;  
      return 0;
    }
    //int xn = aArgs[0].first;
    //double xmn = aArgs[0].second.first;
    //double xmx = aArgs[0].second.second;
    //return createHistogram1D(aTree,aPath,aName,aTitle,xn,xmn,xmx);
    return 0;
  } else if(aClass=="AIDA::IHistogram2D") {
    if(aArgs.size()!=2) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::HDF5::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. Two ranges expected."
          << std::endl;  
      return 0;
    }
    //int xn = aArgs[0].first;
    //double xmn = aArgs[0].second.first;
    //double xmx = aArgs[0].second.second;
    //int yn = aArgs[1].first;
    //double ymn = aArgs[1].second.first;
    //double ymx = aArgs[1].second.second;
    //return createHistogram2D(aTree,aPath,aName,aTitle,xn,xmn,xmx,yn,ymn,ymx);
    return 0;
  } else {
    std::ostream& out = fSession.cout();
    out << "BatchLab::HDF5::Manager::createObject :" 
        << " don't know how to create object of class " 
        << inlib::sout(aClass) << "."
        << std::endl;  
    return 0;
  }
}
  /*
Slash::Store::IObject* BatchLab::HDF5::Manager::createTuple(
 Slash::Store::ITree& aTree
,const std::string& aPath
,const std::string& aName
,const std::string& aTitle
,const std::string& aBooking
){
  BatchLab::RioTree* rioTree = INLIB_CAST(aTree,BatchLab::RioTree);
  if(!rioTree) return 0;
  Rio::IDirectory* directory = rioTree->makeDirectories(aPath);
  if(!directory) return 0;

  if(fSession.verboseLevel()) {
    std::ostream& out = fSession.cout();
    out << "BatchLab::HDF5::Manager::createTuple :" 
        <<  " path " << inlib::sout(aPath)
        <<  " name " << inlib::sout(aName)
        <<  " label " << inlib::sout(aTitle)
        <<  ". Path in store is " 
        << inlib::sout(rioTree->pathInStore(aPath))
        << std::endl;
  }

  BatchLab::RioTuple* tuple = 
    new BatchLab::RioTuple(fSession.out(),*directory,
                      aName,aTitle,aBooking,
		      rioTree->verboseLevel());
  if(!tuple->isValid()) {
    // This may happen if the object on disk is not a Rio::Tree.
    delete tuple;
    return 0;
  }
  return tuple;
}
  */

void BatchLab::HDF5::Manager::addWriter(
 const std::string& aClass
,BatchLab::HDF5::Writer aWriter
,bool aCheck
){
  if(aCheck==true) {if(findWriter(aClass)) return;}
  fWriters.push_back(std::pair<std::string,HDF5::Writer>(aClass,aWriter));
}

BatchLab::HDF5::Writer BatchLab::HDF5::Manager::findWriter(const std::string& aClass){
  unsigned int number = fWriters.size();
  for(unsigned int index=0;index<number;index++) {
    if(aClass==fWriters[index].first) return fWriters[index].second;
  }
  return 0;
}

void BatchLab::HDF5::Manager::addReader(
 const std::string& aClass
,BatchLab::HDF5::Reader aReader
,bool aCheck
){
  if(aCheck==true) {if(findReader(aClass)) return;}
  fReaders.push_back(std::pair<std::string,HDF5::Reader>(aClass,aReader));
}

BatchLab::HDF5::Reader BatchLab::HDF5::Manager::findReader(const std::string& aClass) {
  unsigned int number = fReaders.size();
  for(unsigned int index=0;index<number;index++) {
    if(aClass==fReaders[index].first) return fReaders[index].second;
  }
  return 0;
}
