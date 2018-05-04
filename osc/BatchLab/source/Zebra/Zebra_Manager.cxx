
#include <BatchLab/Zebra/Manager.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/Manager.h>

#include <inlib/cast>

// AIDA :
#include <AIDA/ITreeFactory.h>

// BatchLab :
#include <BatchLab/Zebra/CHBOOK.h>
#include <BatchLab/Zebra/Histogram.h>
#include <BatchLab/Zebra/Tuple.h>

#ifdef WIN32
#define pawc PAWC
#define quest QUEST
#define hcbits HCBITS
#define hcbook HCBOOK
#define rzcl RZCL
#define setpawc SETPAWC
#else
#define pawc pawc_
#define quest quest_
#define hcbits hcbits_
#define hcbook hcbook_
#define rzcl rzcl_
#define setpawc setpawc_
#endif

extern "C" int pawc[1];
extern "C" int quest[100];
extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];
extern "C" int setpawc(); //BatchLab

static bool sHbookInited = false; //FIXME

static int sID = 100; //FIXME : use HIDALL

BatchLab::Zebra::Manager::Manager(Slash::Core::ISession& aSession)
:fName("Zebra_Manager")
,fSession(aSession)
{
  ///////////////////////////////////
  // Initialize HBOOK :
  ///////////////////////////////////
  if(!sHbookInited) {
#if defined(_HIUX_SOURCE) && !defined(__GNUC__)
    hf_fint((char*)NULL);
#endif
    CHLIMIT(setpawc());
    sHbookInited = true;
  }
}

BatchLab::Zebra::Manager::~Manager(){}

std::string BatchLab::Zebra::Manager::name() const {return fName;}

void* BatchLab::Zebra::Manager::cast(const std::string& a_class) const {
  INLIB_IF_CAST(BatchLab::Zebra::Manager)
  else INLIB_IF_CAST(Slash::Store::IStoreManager)
  else INLIB_IF_CAST(Slash::Core::IManager)
  else return 0;
}

#include <BatchLab/Zebra/Tree.h>

Slash::Store::ITree* BatchLab::Zebra::Manager::createTree(
 const std::string& aName
,bool aReadOnly
,bool aCreateNew
,const std::string& aOptions
){
  Zebra::Tree* tree = 
    new Zebra::Tree(fSession,aName,aReadOnly,aCreateNew,aOptions,
                                  *this,fSession.verboseLevel());
  if(!tree) return 0;
  if(!tree->isValid()) {
    delete tree;
    return 0;
  }
  fSession.addManager(tree);
  return tree;
}

#define UNPACK_INT(aString,aNumber) \
    int aNumber;\
    if(!inlib::to<int>(aString,aNumber)) {\
      std::ostream& out = fSession.cout();\
      out << "BatchLab::Zebra::Manager::createObject :" \
          << " arg is not an int " << inlib::sout(aString)\
          << std::endl;\
      return 0;\
    }

#define UNPACK_DOUBLE(aString,aNumber) \
    double aNumber;\
    if(!inlib::to<double>(aString,aNumber)) {\
      std::ostream& out = fSession.cout();\
      out << "BatchLab::Zebra::Manager::createObject :" \
          << " arg is not a double " << inlib::sout(aString)\
          << std::endl;\
      return 0;\
    }

Slash::Store::IObject* BatchLab::Zebra::Manager::createObject(
 Slash::Store::ITree& aTree
,const std::string& aClass
,const std::string& aPath
,const std::string& aName
,const std::string& aTitle
,const std::vector<std::string>& aArgs
,const std::string& //aOptions
){
  BatchLab::Zebra::Tree* zebraTree = INLIB_CAST(aTree,BatchLab::Zebra::Tree);
  if(!zebraTree) return 0;
  int uid;
  if(!inlib::to<int>(aName,uid)) {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Zebra::Manager::createObject :" 
        << " can't convert " << inlib::sout(aName) << " to an int (HBOOK ID)." 
        << " Take ID " << sID <<"."
        << std::endl;
    uid = sID;
    sID++;
  }

  if(aClass=="AIDA::ITuple") {
    if(aArgs.size()!=1) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Zebra::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad argument number. One expected."
          << std::endl;  
      return 0;
    }

    std::string pwd = CHPWD();
    if(!zebraTree->hcd(aPath)) return 0;

    BatchLab::Zebra::Tuple* tuple = 
      new BatchLab::Zebra::Tuple(fSession.cout(),
                                 CHPWD(),uid,aTitle,aArgs[0],
                                 fSession.verboseLevel());

    CHCDIR(pwd," ");

    if(!tuple->isValid()) {
      delete tuple;
      return 0;
    }

    return tuple;

  } else {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Zebra::Manager::createObject :" 
        << " don't know how to create object of class " 
        << inlib::sout(aClass) << "."
        << std::endl;  
    return 0;
  }
}

Slash::Store::IObject* BatchLab::Zebra::Manager::createObject(
 Slash::Store::ITree& aTree
,const std::string& aClass
,const std::string& aPath
,const std::string& aName
,const std::string& aTitle
,const std::vector<Slash::Store::IStoreManager::Range>& aArgs
,const std::string& //aOptions
){
  BatchLab::Zebra::Tree* zebraTree = INLIB_CAST(aTree,BatchLab::Zebra::Tree);
  if(!zebraTree) return 0;
  int uid;
  if(!inlib::to<int>(aName,uid)) {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Zebra::Manager::createObject :" 
        << " can't convert " << inlib::sout(aName) << " to an int (HBOOK ID)." 
        << " Take ID " << sID <<"."
        << std::endl;
    uid = sID;
    sID++;
  }

  if(aClass=="AIDA::IHistogram1D") {
    if(aArgs.size()==0) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Zebra::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad booking argument number. More than one expected."
          << std::endl;  
      return 0;
    } else if(aArgs.size()==1) {
      std::string pwd = CHPWD();
      if(!zebraTree->hcd(aPath)) return 0;
      int xn = aArgs[0].first;
      double xmn = aArgs[0].second.first;
      double xmx = aArgs[0].second.second;
      Slash::Store::IObject* obj = 
        new BatchLab::Zebra::Histogram1D(fSession.cout(),
                                         CHPWD(),uid,aTitle,
                                         xn,xmn,xmx);
      CHCDIR(pwd," ");
      return obj;
    } else { //Assume variable bins histogram.
      std::string pwd = CHPWD();
      if(!zebraTree->hcd(aPath)) return 0;
      // Args must be in agreement with what is done in
      // Core/HistogramFactory/createHistogram1D.
      unsigned int edgen = aArgs.size();
      std::vector<double> edges(edgen);
      for(unsigned int index=0;index<edgen;index++) {
        edges[index] = aArgs[index].second.first;
      }
      Slash::Store::IObject* obj = 
        new BatchLab::Zebra::Histogram1D(fSession.cout(),
                                         CHPWD(),uid,aTitle,
                                         edges);
      CHCDIR(pwd," ");
      return obj;
    }
  } else if(aClass=="AIDA::IHistogram2D") {
    if(aArgs.size()!=2) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Zebra::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad booking argument number. Two expected."
          << std::endl;  
      return 0;
    }
    std::string pwd = CHPWD();
    if(!zebraTree->hcd(aPath)) return 0;
    int xn = aArgs[0].first;
    double xmn = aArgs[0].second.first;
    double xmx = aArgs[0].second.second;
    int yn = aArgs[1].first;
    double ymn = aArgs[1].second.first;
    double ymx = aArgs[1].second.second;
    Slash::Store::IObject* obj = 
      new BatchLab::Zebra::Histogram2D(fSession.cout(),
                                       CHPWD(),uid,aTitle,
                                       xn,xmn,xmx,
                                       yn,ymn,ymx);
    CHCDIR(pwd," ");
    return obj;
  } else if(aClass=="AIDA::IProfile1D") {
    if(aArgs.size()!=2) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::Zebra::Manager::createObject :" 
          << " for class " << inlib::sout(aClass)
          << " bad booking argument number. Two expected."
          << std::endl;  
      return 0;
    }
    std::string pwd = CHPWD();
    if(!zebraTree->hcd(aPath)) return 0;
    int xn = aArgs[0].first;
    double xmn = aArgs[0].second.first;
    double xmx = aArgs[0].second.second;
    //int yn = aArgs[1].first;
    double ymn = aArgs[1].second.first;
    double ymx = aArgs[1].second.second;
    Slash::Store::IObject* obj = 
      new BatchLab::Zebra::Profile1D(fSession.cout(),
                                     CHPWD(),uid,aTitle,
                                     xn,xmn,xmx,
                                     ymn,ymx);
    CHCDIR(pwd," ");
    return obj;
  } else {
    std::ostream& out = fSession.cout();
    out << "BatchLab::Zebra::Manager::createObject :" 
        << " don't know how to create object of class " 
        << inlib::sout(aClass) << "."
        << std::endl;  
    return 0;
  }
}
int* BatchLab::Zebra::Manager::get_pawc() {return pawc;}
int* BatchLab::Zebra::Manager::get_quest() { return quest;}
int* BatchLab::Zebra::Manager::get_hcbits() {return hcbits;}
int* BatchLab::Zebra::Manager::get_hcbook() {return hcbook;}
int* BatchLab::Zebra::Manager::get_rzcl() { return rzcl;}
