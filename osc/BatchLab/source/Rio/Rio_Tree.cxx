// this :
#include <BatchLab/Rio/Tree.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Store/IStorable.h>
#include <Slash/Store/IObject.h>
#include <Slash/Store/IFolderVisitor.h>

#include <AIDA/IManagedObject.h>

#include <amanip/annotation.h>

#include <inlib/args>

#include <Lib/Debug.h>
#include <inlib/cast>
#include <Slash/Tools/Manager.h>

#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IFile.h>
#include <Rio/Interfaces/IKey.h>

#include <BatchLab/Rio/Store.h>
#include <BatchLab/Rio/Manager.h>
#include <BatchLab/Rio/Data.h>

#include <BatchLab/Rio/ReadContext.h>
#include <BatchLab/Core/Histogram.h> //FIXME
#include <BatchLab/Core/BaseTuple.h> //FIXME : for close()
#include <BatchLab/Rio/ROOT.h> //FIXME

static void Rio_Tree_keys(::Rio::IDirectory&,std::vector<std::string>&);

//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tree::Tree(
 Slash::Core::ISession& aSession
,const std::string& aStoreName
,bool aReadOnly
,bool aCreateNew
,Rio::Manager& aManager
,int aVerboseLevel
,const std::string& aOptions
)
:MemoryTree(aSession,aStoreName) //FIXME : have a name # storeName
,fManager(aManager)
,fStoreName(aStoreName)
,fStore(0)
,f2ROOT(true) //ROOT streaming is then the default.
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  inlib::args args(aOptions,";",true);

  int verb = aVerboseLevel;

  std::string value;
  if(args.find("verbose",value)) {
    bool bval;
    if(inlib::to(value,bval)) {
      verb = bval?1:0;
    } else {
      int ival;
      if(inlib::to<int>(value,ival)) verb = ival;
    }
  }
  BaseTree::setVerboseLevel(verb);

  // Store deleted by the tree.
  if(aCreateNew) {
    fStore = 
      new BatchLab::Rio::Store(out(),fManager,aStoreName,"RECREATE",verb);
  } else {
    if(aReadOnly) {
      fStore = 
        new BatchLab::Rio::Store(out(),fManager,aStoreName,"READ",verb);
    } else {
      fStore = 
        new BatchLab::Rio::Store(out(),fManager,aStoreName,"UPDATE",verb);
    }
  }
  if(!fStore->rioFile()) {
    delete fStore;
    fStore = 0;
  }

  //export=root no more supported. It is now the default.
  if(args.find("export",value)) {
    if(value=="root") {
      out() << "BatchLab::Rio::Tree::Tree :" 
          << "export=root option no more supported. It is now the default."
          << std::endl
          << " Use streaming=native option to have native BatchLab streaming."
          << std::endl;
    }
  }

  if(args.find("streaming",value)) {
    if(value=="native") f2ROOT = false;
  }
  if(fStore && fStore->rioFile()) {
    int level = 1; //Default is compressed.
    if(args.find("compress",value)) {
      bool bval;
      if(inlib::to(value,bval)) {
        level = bval?1:0;
      } else {
        int ival;
        if(inlib::to<int>(value,ival)) level = ival;
      }
    }
    fStore->rioFile()->setCompressionLevel(level);
  }
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Tree::~Tree(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(verboseLevel()) {
    out() << "BatchLab::Rio::Tree::~Rio::Tree : cleanup..." << std::endl;
  }

  rootFolder().clear();
  
  delete fStore;

  if(verboseLevel()) {
    out() << "BatchLab::Rio::Tree::~Rio::Tree : end." << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Rio::Tree::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  INLIB_IF_CAST(BatchLab::Rio::Tree)
  else return MemoryTree::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Tree::storeName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStoreName;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Tree::storeType(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Rio::Tree";
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::isValid(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStore?true:false;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Tree::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  BaseTree::setVerboseLevel(aVerboseLevel);
  if(!fStore) return;
  fStore->setVerboseLevel(aVerboseLevel);
}
/*
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Store* BatchLab::Rio::Tree::rioStore(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStore;
}
*/
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::cd(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;

  // MemoryTree part :

  // Need to cd in MemoryTree for the factory::create(object,"relative path")
  // It does a addObject that needs to be in correct position in MemoryTree.
  if(!MemoryTree::isADirectory(aPath)) {
    // When reading a file, the directories may be not created. Create them.
    Slash::Store::IFolder* folder = MemoryTree::makeFolders(aPath);
    if(!folder) return false;    
    MemoryTree::setCurrentFolder(folder);
  } else {
    if(!MemoryTree::cd(aPath)) return false;
  }

  // Store part :

  //FIXME : twice cd ???
  // Need to cd in store for the creation of Rio::Tuples.
  if(!fStore->cd(aPath)) {
    // When reading a file, the directory may be not in memory. Try to read it.
    //if(!fStore->cd(aPath)) {
      out() << "BatchLab::Rio::Tree::cd :" 
          << " input path " << inlib::sout(aPath) << " :"
          << " can't cd in store."
          << std::endl;
      return false;
    //}
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::mkdir(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;
  // Create in MemoryTree so that manage(object,"relative path") is ok.
  if(!MemoryTree::mkdir(aPath)) return false;
  // Need to create in store for the creation of Rio::Tuples.
  return fStore->mkdir(aPath);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::mkdirs(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return false;
  // Create in MemoryTree so that manage(object,"relative path") is ok.
  if(!MemoryTree::mkdirs(aPath)) return false;
  // Need to create in store for the creation of Rio::Tuples.
  return fStore->mkdirs(aPath);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::ls(
 const std::string&
,bool
,std::ostream&
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : ls what ? It seems more usefull to dump store keys.
  //  But could be interesting to see objects in MemoryTree 
  // ready to be committed.
  if(!fStore) return false;
  fStore->ls();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::cp(
 const std::string& aFrom
,const std::string& aTo
,bool aRecursive
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fStore->isADirectory(aFrom)) {
    ::Rio::IDirectory* directory = fStore->findDirectory(aFrom);
    if(directory) import(*directory,aFrom);
  }
  return MemoryTree::cp(aFrom,aTo,aRecursive);
}
namespace BatchLab {
  class StoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    StoreVisitor(BatchLab::Rio::Tree& aTree,Rio::Store& aStore,std::ostream& a_out)
    :fTree(aTree),fStore(aStore),f_out(a_out) {}
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fTree.verboseLevel()) {
        f_out << "BatchLab::StoreVisitor::visit :" 
            << " path : " << inlib::sout(aFolder.path())
            << " name : " << inlib::sout(aObject.name())
            << std::endl;
      }
      ::Rio::IDirectory* directory = 
        fStore.makeDirectories(aFolder.path());
      if(!directory) return false; //Stop visiting.
      fTree.store(*directory,aObject);
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,const Slash::Store::IFolder&){ return true;}
    virtual bool endVisit(const Slash::Store::IFolder&,const Slash::Store::IFolder&){ return true;}
    virtual bool isRecursive() const { return true;}
  private:
    BatchLab::Rio::Tree& fTree;
    Rio::Store& fStore;
    std::ostream& f_out;
  };
}

bool BatchLab::Rio::Tree::commit() {
  if(!fStore) return false;
  StoreVisitor visitor(*this,*fStore,out());
  rootFolder().visit(visitor);
  return fStore->commit();
}

namespace BatchLab {
  class CloseStoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    CloseStoreVisitor(){}
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      const BatchLab::BaseTuple* tuple = 
        INLIB_CONST_CAST(aObject,BatchLab::BaseTuple);
      if(tuple) const_cast<BatchLab::BaseTuple*>(tuple)->close();          
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,const Slash::Store::IFolder&){ return true;}
    virtual bool endVisit(const Slash::Store::IFolder&,const Slash::Store::IFolder&){ return true;}
    virtual bool isRecursive() const { return true;}
  };
}

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::close(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return true; //done or not opened.

  // Have to deconnect the tuple from the store.
  CloseStoreVisitor visitor;
  rootFolder().visit(visitor);

  delete fStore;
  fStore = 0;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::rmdir(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  MemoryTree::rmdir(aPath);
  //FIXME : and on fStore ?
  return true;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Tree::findPath(
 const AIDA::IManagedObject&
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return ""; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::rm(
 const std::string&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return false; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Tree::pwd(
) const
//////////////////////////////////////////////////////////////////////////////
// WARNING : does not return a path starting with /<file name>.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return "";
  return fStore->path();
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Tree::hierarchy(
 const std::string& aWhat
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aWhat=="memory") return MemoryTree::hierarchy();
  if(!fStore) return "";
  return fStore->hierarchy("all");
}
//////////////////////////////////////////////////////////////////////////////
std::vector<std::string> BatchLab::Rio::Tree::listObjectNames(
 const std::string& aPath
,bool /*aRecursive*/
) const 
//////////////////////////////////////////////////////////////////////////////
// Return object keys in file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> vect;
  if(!fStore) return vect;

  if(!fStore->isADirectory(aPath)) {
    out() << "BatchLab::Rio::Tree::listObjectNames :" 
        <<  " path " << inlib::sout(aPath)
        <<  " not a directory of " << inlib::sout(fStoreName) 
        << "."
        << std::endl;
    return vect;
  }

  ::Rio::IDirectory* directory = fStore->findDirectory(aPath);
  if(!directory) {
    out() << "BatchLab::Rio::Tree::listObjectNames :" 
        <<  " directory " << inlib::sout(aPath)
        <<  " not found in  " << inlib::sout(fStoreName) 
        << "."
        << std::endl;
    return vect;
  }

  Rio_Tree_keys(*directory,vect);

  return vect;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<std::string> BatchLab::Rio::Tree::listObjectTypes(
 const std::string& /*aPath*/
,bool /*aRecursive*/
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> vect;
  out() << "BatchLab::Rio::Tree::listObjectTypes : dummy. " << std::endl;
  //FIXME
  return vect;
}
//////////////////////////////////////////////////////////////////////////////
::Rio::IDirectory* BatchLab::Rio::Tree::makeDirectories(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fStore) return 0;
  return fStore->makeDirectories(aPath);
}
//////////////////////////////////////////////////////////////////////////////
static void Rio_Tree_keys(
 ::Rio::IDirectory& aDirectory
,std::vector<std::string>& aList
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 /*
 {std::vector< ::Rio::IObject*> objs = aDirectory.objects();
  std::vector< ::Rio::IObject*>::const_iterator it;
  for(it=objs.begin();it!=objs.end();++it) {
    aList.push_back((*it)->name());
  }}
  */

 {std::vector< ::Rio::IKey*> keys = aDirectory.keys();
  std::vector< ::Rio::IKey*>::const_iterator it;
  for(it=keys.begin();it!=keys.end();++it) {
    if((*it)->className()=="TDirectory") {
      aList.push_back((*it)->name()+"/"); //AIDA : add a / at end.
      ::Rio::IDirectory* dir = aDirectory.findDirectory((*it)->name());
      if(dir) Rio_Tree_keys(*dir,aList);
    } else {
      aList.push_back((*it)->name());
    }
  }}
}

// Data specific area :

//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Rio::Tree::retreiveObject(
 ::Rio::IDirectory& aDirectory
,const std::string& aSID
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //  The retreived ::Rio::IObject is transient, it is deleted
  // after the creation of the BatchLab::Histogram.
  ::Rio::IObject* rioObject = aDirectory.find(aSID);
  if(!rioObject) return 0;

  if(verboseLevel()) {
    out() << "BatchLab::Rio::Tree::retreiveObject :" 
        << " found " << inlib::sout(aSID)
        //        << " in store " << inlib::sout(storeName()) 
        << "."
        << std::endl;
  } 

  Slash::Store::IObject* object = 0;

  BatchLab::Rio::Data* rdata = ::Rio::cast<BatchLab::Rio::Data>(*rioObject);
  if(rdata) {

    // Generic way :
    Slash::Store::IStorable* visited = rdata->data();
    if(!visited) {
      out() << "BatchLab::Rio::Tree::retreiveObject :" 
          << " BatchLab::Rio::Data has no data."
          << std::endl;
    } else {
      object = INLIB_CAST(*visited,Slash::Store::IObject);
      if(!object) {
        out() << "BatchLab::Rio::Tree::retreiveObject :" 
            << " can't cast Slash::Store::IStorable to Slash::Store::IObject."
            << std::endl;
      } else {
        object->setName(aSID);
      }
    }

  } else {

    bool toDelete = true;
    // Try to find a reader :
    std::string sclass = rioObject->isA().name();
    BatchLab::Rio::Reader reader = fManager.findReader(sclass);
    if(!reader) {
      out() << "BatchLab::Rio::Tree::retreiveObject :" 
          << " reader not found for class " << inlib::sout(sclass)
          << std::endl;
    } else {
      Rio::ReadContext ctx(out(),*rioObject,aSID,verboseLevel());
      object = reader(ctx,toDelete);
      if(!object) {
        out() << "BatchLab::Rio::Tree::retreiveObject :" 
            << " can't retreive object " << inlib::sout(aSID) << "."
            << std::endl;
      }
    }
    if(toDelete) {
      aDirectory.removeObject(rioObject);
      delete rioObject;
    }

  }

  return object;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::import(
 ::Rio::IDirectory& aDirectory
,const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// aPath = /dir1/dir2/...
// Bring in memory objects in a directory of a file (from their keys).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector< ::Rio::IKey*> keys = aDirectory.keys();
  for(unsigned int index=0;index<keys.size();index++) {
    std::string SID = keys[index]->name();
    if(verboseLevel()) {
      out() << "BatchLab::Rio::Tree::import :" 
          << " find " << inlib::sout(SID) << "..."
          << std::endl;
    } 
    Slash::Store::IObject* object = retreiveObject(aDirectory,SID);
    if(!object) {
      out() << "BatchLab::Rio::Tree::import :" 
          << " object " << inlib::sout(SID)
          << " not found." << std::endl;
      return false;
    }
    if(!MemoryTree::mkdirs(aPath)) {
      delete object;
      return false;
    }
    if(!manageObject(object,aPath)) {
      delete object;
      return false;
    }
    if(verboseLevel()) {
      out() << "BatchLab::Rio::Tree::import :" 
          << " ok."
          << std::endl;
    } 
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
AIDA::IManagedObject* BatchLab::Rio::Tree::find(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// aPath = /dir1/dir2/.../SID
//       = SID
// where SID = storage identifier. 
// The SID could be of the form <string>;<uint>
// Could bring in memory two histos with same SID.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aPath.empty()) return 0;

  // Look first in memory :
 {AIDA::IManagedObject* object = 0;
  // look for case <string>;<int> :
  std::string::size_type pos = aPath.rfind(';');
  if(pos==std::string::npos) {
    object = MemoryTree::find(aPath);
  } else {
    //foo;1
    //01234
    object = MemoryTree::find(aPath.substr(0,pos));
  }
  if(object) {
    if(verboseLevel()) {
      out() << "BatchLab::Rio::Tree::find :" 
          << " find " << inlib::sout(aPath) << " in memory."
          << std::endl;
    } 
    return object;
  }}

  // Not found in memory, get in file :
  if(!fStore) return 0;

  std::string wpath,SID,suffix; //wpath is the wanted path.
  inlib::path_name_suffix(aPath,wpath,SID,suffix);
  ::Rio::IDirectory* directory = fStore->findDirectory(wpath);
  if(!directory) {
    out() << "BatchLab::Rio::Tree::find :"
        << " can't find container " << inlib::sout(wpath) 
        << " in store " << inlib::sout(fStore->storeName()) << "." 
        << std::endl;
    return 0;
  }

  Slash::Store::IObject* object = retreiveObject(*directory,SID);
  if(!object) {
    if(verboseLevel()) {
      out() << "BatchLab::Rio::Tree::find :"
          << " can't retreive object " << inlib::sout(SID) 
          << " in store " << inlib::sout(fStore->storeName()) << "." 
          << std::endl;
    }
    return 0;
  } 

  AIDA::IManagedObject* aobject = INLIB_CAST(*object,AIDA::IManagedObject);
  if(!aobject) {
    out() << "BatchLab::Rio::Tree::find :"
        << " for " << inlib::sout(aPath) 
        << ",Slash::Store::IObject not a AIDA::IManagedObject."
        << " in store " << inlib::sout(fStore->storeName()) << "." 
        << std::endl;
    delete object;
    return 0;
  }

 {std::string path = Store::path(*directory); //true path
  if(!MemoryTree::mkdirs(path)) {
    delete object;
    return 0;
  }
  if(!manageObject(object,path)) {
    delete object;
    return 0;
  }}

  return aobject;
}

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Tree::store(
 ::Rio::IDirectory& aDirectory
,const Slash::Store::IObject& aObject
)
//////////////////////////////////////////////////////////////////////////////
// SID = storage identifier (it is not a path).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s = aObject.storeClassName();

  if(s=="BatchLab::Rio::Tuple") return true;

  bool exprt = f2ROOT;
  if(exprt) {
    if(s=="BatchLab::Histogram1D") {

      std::string SID = aObject.name();
      aDirectory.remove(std::string(SID + ";*"));
      const ::Rio::IDictionary& dico = aDirectory.file().dictionary();
      const BatchLab::Histogram1D* bobject = 
        INLIB_CONST_CAST(aObject,BatchLab::Histogram1D);  
      std::string sa = amanip::to(bobject->annotation());
      ::Rio::IObject* rioObject = 
        new BatchLab::Rio::TH1D(dico,
                                (inlib::histo::h1d*)&(bobject->hclHistogram()),
                                sa,SID);
      aDirectory.appendObject(rioObject);
      return true;

    } else if(s=="BatchLab::Histogram2D") {

      std::string SID = aObject.name();
      aDirectory.remove(std::string(SID + ";*"));
      const ::Rio::IDictionary& dico = aDirectory.file().dictionary();
      const BatchLab::Histogram2D* bobject = 
        INLIB_CONST_CAST(aObject,BatchLab::Histogram2D);  
      std::string sa = amanip::to(bobject->annotation());
      ::Rio::IObject* rioObject = 
        new BatchLab::Rio::TH2D(dico,
                                (inlib::histo::h2d*)&(bobject->hclHistogram()),
                                sa,SID);
      aDirectory.appendObject(rioObject);
      return true;

    } else if(s=="BatchLab::Profile1D") {

      std::string SID = aObject.name();
      aDirectory.remove(std::string(SID + ";*"));
      const ::Rio::IDictionary& dico = aDirectory.file().dictionary();
      const BatchLab::Profile1D* bobject = 
        INLIB_CONST_CAST(aObject,BatchLab::Profile1D);  
      std::string sa = amanip::to(bobject->annotation());
      ::Rio::IObject* rioObject = 
        new BatchLab::Rio::TProfile(dico,
                                   (inlib::histo::p1d*)&(bobject->hclHistogram()),
                                    sa,SID);
      aDirectory.appendObject(rioObject);
      return true;

    } else if(s=="BatchLab::Profile2D") {
      out() << "BatchLab::Rio::Tree::store :"
          << " export of BatchLab::Profile2D to TProfile2D not yet supported." 
          << " Write as a BatchLab::Profile2D." 
          << std::endl;
      exprt = false;

    }
  }

  const Slash::Store::IStorable* visited = 
    INLIB_CONST_CAST(aObject,Slash::Store::IStorable);
  if(!visited) {
    out() << "BatchLab::Rio::Tree::store :"
        << " object " << inlib::sout(aObject.name()) 
        << " not a Slash::Store::IStorable ; can't store it." 
        << " Write as a BatchLab::Profile2D." 
        << std::endl;
    return false;
  }

  // Generic way :
  std::string SID = aObject.name();
  aDirectory.remove(std::string(SID + ";*"));

  ::Rio::IDictionary& dico = 
    const_cast< ::Rio::IDictionary& >(aDirectory.file().dictionary());

  ::Rio::IClass* rioClass = dico.findClass(s);
  if(!rioClass) {
    //dico.addClass(rioClass);
    out() << "BatchLab::Rio::Tree::store :"
        << " Rio class not found for " << inlib::sout(s) 
        << " ." 
        << std::endl;
    return false;
  }

  aDirectory.appendObject
    (new BatchLab::Rio::Data(dico,*rioClass,SID,aObject.title(),*visited));

  return true;
}
