// this :
#include <BatchLab/Rio/Manager.h>

// Slash :
#include <Slash/Tools/Managers.h>
#include <Slash/Store/IStorable.h>

// Lib:
#include <Lib/Debug.h>
#include <inlib/cast>

// Rio :
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>
#include <Rio/Core/Debug.h>

#include <BatchLab/Rio/Dictionary.h>
#include <BatchLab/Rio/Data.h>
#include <BatchLab/Rio/Tree.h>
#include <BatchLab/Rio/Tuple.h>

// Readers :
#include <BatchLab/Rio/T_Reader.h>
#include <BatchLab/Rio/THistogram.h>
#include <BatchLab/Rio/DataReader.h>

// Automatic writting :
#include <BatchLab/Core/Histogram.h> //FIXME

BatchLab::Rio::Manager::Manager(Slash::Core::ISession& aSession
)
:fName("Rio_Manager")
,fSession(aSession)
,fDictionary(0)
{
  if(Lib::Debug::checkByClass()) ::Rio::Debug::checkByClass(true);

  fDictionary = 
    new BatchLab::Rio::Dictionary(fSession.cout());

  if(!fDictionary->initialize()) {
    aSession.cout() << "BatchLab::Rio::Manager::Manager :"
                    << " can't initialize dictionary."
                    << std::endl;
  }

  addDefaultAllocators();

  addDefaultReaders();
}

BatchLab::Rio::Manager::~Manager() {
  delete fDictionary;
  ::Rio::Debug::check(fSession.cout());
}

std::string BatchLab::Rio::Manager::name() const {return fName;}

void* BatchLab::Rio::Manager::cast(const std::string& a_class) const {
  INLIB_IF_CAST(BatchLab::Rio::Manager)
  else INLIB_IF_CAST(Slash::Store::IStoreManager)
  else INLIB_IF_CAST(Slash::Core::IManager)
  else return 0;
}

::Rio::IDictionary& BatchLab::Rio::Manager::dictionary() const {
  return *fDictionary;
}

std::ostream& BatchLab::Rio::Manager::out() const {return fSession.cout();}

Slash::Store::ITree* BatchLab::Rio::Manager::createTree(
 const std::string& aName
,bool aReadOnly
,bool aCreateNew
,const std::string& aOption
)
{
  BatchLab::Rio::Tree* tree = 
    new BatchLab::Rio::Tree(fSession,aName,aReadOnly,aCreateNew,
                            *this,fSession.verboseLevel(),
                            aOption);
  if(!tree) return 0;
  if(!tree->isValid()) {
    delete tree;
    return 0;
  }
  fSession.addManager(tree);
  return tree;
}

Slash::Store::IObject* BatchLab::Rio::Manager::createObject(
 Slash::Store::ITree& aTree
,const std::string& aClass
,const std::string& aPath
,const std::string& aName
,const std::string& aTitle
,const std::vector<std::string>& aArgs
,const std::string& //aOptions
)
{
  if(aClass=="AIDA::ITuple") {
    if(aArgs.size()!=1) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Rio::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. One expected."
          << std::endl;  
      return 0;
    }
    return createTuple(aTree,aPath,aName,aTitle,aArgs[0]);
  } else {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Rio::Manager::createObject :" 
        << " don't know how to create object of class " 
        << inlib::sout(aClass) << "."
        << std::endl;  
    return 0;
  }
}

Slash::Store::IObject* BatchLab::Rio::Manager::createObject(
 Slash::Store::ITree& aTree
,const std::string& aClass
,const std::string& aPath
,const std::string& aName
,const std::string& aTitle
,const std::vector<Slash::Store::IStoreManager::Range>& aArgs
,const std::string& //aOptions
)
{
  if(aClass=="AIDA::IHistogram1D") {
    if(aArgs.size()!=1) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Rio::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. One range expected."
          << std::endl;  
      return 0;
    }
    int xn = aArgs[0].first;
    double xmn = aArgs[0].second.first;
    double xmx = aArgs[0].second.second;
    return createHistogram1D(aTree,aPath,aName,aTitle,xn,xmn,xmx);
  } else if(aClass=="AIDA::IHistogram2D") {
    if(aArgs.size()!=2) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Rio::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. Two ranges expected."
          << std::endl;  
      return 0;
    }
    int xn = aArgs[0].first;
    double xmn = aArgs[0].second.first;
    double xmx = aArgs[0].second.second;
    int yn = aArgs[1].first;
    double ymn = aArgs[1].second.first;
    double ymx = aArgs[1].second.second;
    return createHistogram2D(aTree,aPath,aName,aTitle,xn,xmn,xmx,yn,ymn,ymx);
  } else {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Rio::Manager::createObject :" 
        << " don't know how to create object of class " 
        << inlib::sout(aClass) << "."
        << std::endl;  
    return 0;
  }
}

Slash::Store::IObject* BatchLab::Rio::Manager::createTuple(
 Slash::Store::ITree& aTree
,const std::string& aPath
,const std::string& aName
,const std::string& aTitle
,const std::string& aBooking
)
{
  BatchLab::Rio::Tree* rioTree = INLIB_CAST(aTree,BatchLab::Rio::Tree);
  if(!rioTree) return 0;
  ::Rio::IDirectory* directory = rioTree->makeDirectories(aPath);
  if(!directory) return 0;

  if(fSession.verboseLevel()) {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Rio::Manager::createTuple :" 
        <<  " path " << inlib::sout(aPath)
        <<  " name " << inlib::sout(aName)
        <<  " label " << inlib::sout(aTitle)
        <<  ". Path in store is " 
        << inlib::sout(aPath)
        << std::endl;
  }

  BatchLab::Rio::Tuple* tuple = 
    new BatchLab::Rio::Tuple(fSession.cout(),*directory,
                      aName,aTitle,aBooking,
		      rioTree->verboseLevel());
  if(!tuple->isValid()) {
    // This may happen if the object on disk is not a ::Rio::Tree.
    delete tuple;
    return 0;
  }
  return tuple;
}

Slash::Store::IObject* BatchLab::Rio::Manager::createHistogram1D(
 Slash::Store::ITree& //aTree
,const std::string& /*aPath*/
,const std::string& /*aName*/
,const std::string& /*aTitle*/
,int /*aXnumber*/
,double /*aXmin*/
,double /*aXmax*/
)
{
  std::ostream& out = fSession.cout();
  out << "BatchLab::RioaManager::createHistogram1D :" 
      << " dummy."
      << std::endl;
  return 0;
}

Slash::Store::IObject* BatchLab::Rio::Manager::createHistogram2D(
 Slash::Store::ITree& //aTree
,const std::string& /*aPath*/
,const std::string& /*aName*/
,const std::string& /*aTitle*/
,int /*aXnumber*/
,double /*aXmin*/
,double /*aXmax*/
,int /*aYnumber*/
,double /*aYmin*/
,double /*aYmax*/
)
{
  std::ostream& out = fSession.cout();
  out << "BatchLab::RioaManager::createHistogram1D :" 
      << " dummy."
      << std::endl;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Manager::addWriter(
 const std::string& aClass
,BatchLab::Rio::Writer aWriter
,bool aCheck
){
  if(aCheck==true) {if(findWriter(aClass)) return;}
  fWriters.push_back(std::pair<std::string,Rio::Writer>(aClass,aWriter));
}

BatchLab::Rio::Writer BatchLab::Rio::Manager::findWriter(const std::string& aClass)
{
  unsigned int number = fWriters.size();
  for(unsigned int index=0;index<number;index++) {
    if(aClass==fWriters[index].first) return fWriters[index].second;
  }
  return 0;
}

void BatchLab::Rio::Manager::addReader(
 const std::string& aClass
,BatchLab::Rio::Reader aReader
,bool aCheck
){
  if(aCheck==true) {if(findReader(aClass)) return;}
  fReaders.push_back(std::pair<std::string,Rio::Reader>(aClass,aReader));
}

BatchLab::Rio::Reader BatchLab::Rio::Manager::findReader(
 const std::string& aClass
){
  unsigned int number = fReaders.size();
  for(unsigned int index=0;index<number;index++) {
    if(aClass==fReaders[index].first) return fReaders[index].second;
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define ADD_READER(aDataClass,aIO_Class) \
 {typedef Slash::Store::IObject* (*F)(BatchLab::Rio::ReadContext&,bool&);\
  F f = BatchLab::Rio::reader<aIO_Class,aDataClass>;\
  addReader(#aIO_Class,(Rio::Reader)f);}


void BatchLab::Rio::Manager::addDefaultReaders() {
  ADD_READER(BatchLab::BaseHistogram,BatchLab::Rio::THistogram)

  // Extra readers for some ROOT classes :
  addReader("BatchLab::Rio::TH1F",(Rio::Reader)BatchLab::Rio::read_TH1F);
  addReader("BatchLab::Rio::TH2F",(Rio::Reader)BatchLab::Rio::read_TH2F);
  addReader("BatchLab::Rio::TH1D",(Rio::Reader)BatchLab::Rio::read_TH1D);
  addReader("BatchLab::Rio::TH2D",(Rio::Reader)BatchLab::Rio::read_TH2D);
  addReader("BatchLab::Rio::TProfile",(Rio::Reader)BatchLab::Rio::read_TProfile);
  addReader("Rio::Tree",(Rio::Reader)BatchLab::Rio::read_Tree);

}

bool BatchLab::Rio::Manager::addDefaultAllocators() {
  Slash::Meta::IDictionary* manager = Slash::dictionary(fSession);
  if(!manager) {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Rio::Manager::addDefaultAllocators :" 
        << " Lib::Dictionary manager not found." 
        << std::endl;		 
    return false;
  }

  unsigned int number = manager->numberOfClasses();
  //printf("debug : addDefaultAllocators : %d\n",number);
  for(unsigned int index=0;index<number;index++) {
    const Slash::Meta::IClass* cls = manager->getClass(index);
    //printf("debug :  %d : \"%s\"\n",index,cls->name().c_str());
    fDictionary->addClass(new Data_Class(cls->name(),
                                         cls->version(),
                                         fSession,fSession.cout()));
  }

  return true;
}
