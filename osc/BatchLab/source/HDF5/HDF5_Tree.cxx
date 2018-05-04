// this :
#include <BatchLab/HDF5/Tree.h>

// Slash :
#include <Slash/Tools/Managers.h>
#include <Slash/Meta/IClass.h>
#include <Slash/Meta/IObject.h>
#include <Slash/Store/IObject.h>
#include <Slash/Store/IStorable.h>
#include <Slash/Store/IFolderVisitor.h>

#include <inlib/system>
#include <inlib/file>
#include <inlib/args>
#include <inlib/sys/atime>
#include <inlib/cast>

#include <Lib/ObjectReader.h>

#include <BatchLab/Core/Header.h>

#include <BatchLab/HDF5/HDF5O.h>
#include <BatchLab/HDF5/IVisitor.h>
#include <BatchLab/HDF5/Writer.h>
//#include <BatchLab/HDF5/ReadContext.h>

#include <AIDA/IManagedObject.h>

// HDF5 :
#include <H5LT.h>

#include <cstdio> //::remove

#define NotFound (-1)
#define Failure (-1)

#ifdef WIN32
// FIXME :
//  The below is not exported in the DLL (internal methods).
//  We copy the code here and make them static.
static herr_t find_attr( hid_t loc_id, const char *name, void *op_data) {
 int ret = 0;
 char *attr_name = (char*)op_data;
 loc_id=loc_id;
 if( strcmp( name, attr_name ) == 0 ) ret = 1;
 return ret;
}
static herr_t H5LT_find_attribute( hid_t loc_id, const char* attr_name ) {
 unsigned int attr_num;
 attr_num = 0;
 return H5Aiterate( loc_id, &attr_num, find_attr, (void *)attr_name );
}
#endif

BatchLab::HDF5::Tree::Tree(
 Slash::Core::ISession& aSession
,const std::string& aStoreName
,bool aReadOnly
,bool aCreateNew
,const std::string& aOptions
,HDF5::Manager& aManager
,int aVerboseLevel
)
:MemoryTree(aSession,aStoreName) //FIXME : have a name # storeName
,fManager(aManager)
,fStoreName(aStoreName)
,fNameCounter(0)
,fReadOnly(aReadOnly)
,fCreateNew(aCreateNew)
,fCompress(false)
,fIsValid(false)
,fFileID(Failure)
,fHDF5O(0)
{
  setVerboseLevel(aVerboseLevel);

  inlib::args args(aOptions,";",true);
  args.find("compress",fCompress);

  if(fCompress) { //Compression wanted. Check it is available.
    if(H5Zfilter_avail(H5Z_FILTER_DEFLATE)!=1) {
      out() << "BatchLab::HDF5::Tree::Tree :" 
          << " zip filter not available."
          << std::endl;  
      fCompress = false;
    } else if(verboseLevel()) {
      out() << "BatchLab::HDF5::Tree::Tree :" 
          << " zip filter is available."
          << std::endl;  
    }
  }

  fHDF5O = new HDF5O(fCompress);

  //printf("debug : HDF5::Tree :<-- \"%s\" \"%s\" %d -->\n",
  // aStoreName.c_str(),aOptions.c_str(),fCompress);

  if(fCreateNew) {

    if(fReadOnly) {
      out() << "BatchLab::HDF5::Tree::commit :"
          << " createNew and readOnly make no sens."
          << std::endl;
      return;
    }

    std::string name;
    inlib::file_name(aStoreName,name);
    ::remove(name.c_str());
  
    fFileID = 
      H5Fcreate(name.c_str(),H5F_ACC_TRUNC,H5P_DEFAULT,H5P_DEFAULT);
    if(fFileID<0) {
      out() << "BatchLab::HDF5::Tree::commit :"
          << " can't create file."
          << std::endl;
      fFileID = Failure;
      return;
    }

    fIsValid = true;

  } else {
    // Read the existing file :
    if(verboseLevel()) {
      out() << "BatchLab::HDF5::Tree::HDF5::Tree :"
          << " open " << inlib::sout(aStoreName) 
          << " with compression=" 
          << std::string(fCompress?"yes":"no") << "..." << std::endl;
    }

    std::string name;
    inlib::file_name(aStoreName,name);
    fFileID = H5Fopen(name.c_str(),H5F_ACC_RDONLY, H5P_DEFAULT);
    if(fFileID<0) {
      out() << "BatchLab::HDF5::Tree::Tree :" 
          << " can't open file " << inlib::sout(name)
          << std::endl;
      fFileID = Failure;
      return;
    }

/*
    if(!loadFile()) {
      out() << "BatchLab::HDF5::Tree::HDF5::Tree :"
          << " problem loading file " << inlib::sout(aStoreName)
          << std::endl;
      return;
    }
*/

    if(verboseLevel()) {
      out() << "BatchLab::HDF5::Tree::HDF5::Tree :"
          << " load of " << inlib::sout(aStoreName) << " done."
          << std::endl;
    }

    fIsValid = true;

  }
}

BatchLab::HDF5::Tree::~Tree() {
  if(fFileID>=0) H5Fclose(fFileID);
  delete fHDF5O;
}

void* BatchLab::HDF5::Tree::cast(const std::string& a_class) const {
  INLIB_IF_CAST(BatchLab::HDF5::Tree)
  else return MemoryTree::cast(a_class);
}

std::string BatchLab::HDF5::Tree::storeName() const {return fStoreName;}

std::string BatchLab::HDF5::Tree::storeType() const {
  return "BatchLab::HDF5::Tree";
}

bool BatchLab::HDF5::Tree::isValid() const {return fIsValid;}

//////////////////////////////////////////////////////////////////////////////
/// Data specific area ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

namespace BatchLab {
namespace HDF5 {
  class StoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    StoreVisitor(HDF5::Manager& aManager,
                 HDF5::Tree& aTree,
                 hid_t aFile,
                 std::ostream& a_out,
                 HDF5O& aHDF5O,
                 int aVerboseLevel)
    :fManager(aManager)
    ,fTree(aTree)
    ,fFile(aFile)
    ,fDirectory(aFile)
    ,f_out(a_out)
    ,fHDF5O(aHDF5O)
    ,fVerboseLevel(aVerboseLevel) {
      fStack.push_front(aFile);
    }
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fVerboseLevel) {
        f_out << "BatchLab::HDF5::StoreVisitor::visit :" 
            << " path : " << inlib::sout(aFolder.path())
            << " name : " << inlib::sout(aObject.name())
            << std::endl;
      }

      if(!writeObject(fDirectory,aObject,fHDF5O)) {
        f_out << "BatchLab::HDF5::StoreVisitor::visit :" 
            << " writing failed for " << inlib::sout(aObject.name())
            << std::endl;
      }

      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder& aFolder){ 
      hid_t directory = H5Gcreate(fDirectory,aFolder.name().c_str(),0);
      if(directory<0) {
        f_out << "HDF5::StoreVisitor::beginVisit folder :"
            << " can't create group " << inlib::sout(aFolder.name())
            << std::endl;
        return false;
      }
      if(!fHDF5O.write_atb(directory,"type","BatchLab::Directory")) {
        f_out << "BatchLab::HDF5::StoreVisitor::beginGroup :" 
            << " can't write " << inlib::sout(aFolder.name()) << " type."
            << std::endl;
        H5Gclose(directory);
        return false;
      }
      fStack.push_front(directory);
      fDirectory = directory;
      return true;
    }
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ 
      hid_t directory = fStack.front();
      H5Gclose(directory);
      fStack.pop_front();
      fDirectory = fStack.front();
      //fprintf(stderr,"debug : endVisit : %d : %d %d\n",
      //      fFile,fDirectory,directory);
      return true;
    }
    virtual bool isRecursive() const { return true;}
  private:
    bool writeObject(hid_t aDirectory,
                     const Slash::Store::IObject& aObject,
                     BatchLab::HDF5::HDF5O& aHDF5O) {
      hid_t object = H5Gcreate(aDirectory,aObject.name().c_str(),0);
      if(object<0) return false;
      //FIXME : use some Slash::Meta::IObject.isA ?
      if(!aHDF5O.write_atb(object,"type",aObject.storeClassName())) {
        H5Gclose(object);
        return false;
      }
      int version = 1; //FIXME
      if(!aHDF5O.write_atb(object,"version",version)) {
        H5Gclose(object);
        return false;
      }
      if(!aHDF5O.write_atb(object,"title",aObject.title())) {
        H5Gclose(object);
        return false;
      }
      //FIXME : have to store a uint64.
      if(!aHDF5O.write_atb
            (object,"date",(unsigned int)inlib::atime::now().seconds())) {
        H5Gclose(object);
        return false;
      }
      const Slash::Store::IStorable* visited = 
        INLIB_CONST_CAST(aObject,Slash::Store::IStorable);
      if(!visited) {
        f_out << "BatchLab::HDF5::StoreVisitor::writeObject :" 
            << " Slash::Store::IObject " << inlib::sout(aObject.name())
            << " not a Slash::Store::IStorable."
            << std::endl;
        H5Gclose(object);
        return false;
      }
      if(!hdf5_writer(object,aObject.name(),*visited,aHDF5O)) {
        H5Gclose(object);
        return false;
      }
      H5Gclose(object);
      return true;  
    }
  private:
    HDF5::Manager& fManager;
    HDF5::Tree& fTree;  
    hid_t fFile;
    hid_t fDirectory;
    std::list<hid_t> fStack;
    std::ostream& f_out;
    HDF5O& fHDF5O;
    int fVerboseLevel;
};
}}

bool BatchLab::HDF5::Tree::commit() {
  if(fFileID<0) return false;
  if(fReadOnly) return false;

  if(!writeHeader()) return false;

  StoreVisitor visitor
    (fManager,*this,fFileID,out(),*fHDF5O,verboseLevel());

  rootFolder().visit(visitor);

  return true;
}

bool BatchLab::HDF5::Tree::close() {
  if(fFileID<0) return true; //done or not opened.
  H5Fclose(fFileID);
  fFileID = Failure;
  return true;
}

bool BatchLab::HDF5::Tree::writeHeader() {
  if(fFileID<0) return false;

  // Create a Header group :
  std::string name = "header";
  hid_t id = H5Gcreate(fFileID,name.c_str(),0);
  if(id<0) {
    out() << "BatchLab::HDF5::Tree::writeHeader :"
        << " can't create group " << inlib::sout(name)
        << std::endl;
    return false;
  }

  if(!fHDF5O->write_atb(id,"type","BatchLab::Header")) {
    H5Gclose(id);
    return false;
  }

  BatchLab::Header header;
  if(!hdf5_writer(id,name,header,*fHDF5O)) {
    H5Gclose(id);
    return false;
  }

  H5Gclose(id);
  return true;
}

herr_t file_info(hid_t aID,const char* aName,void* aTag){
  BatchLab::HDF5::IVisitor* visitor = (BatchLab::HDF5::IVisitor*)aTag;

  H5G_stat_t statbuf;
  H5Gget_objinfo(aID,aName,0,&statbuf);
  switch(statbuf.type) {
  case H5G_GROUP:{
    visitor->beginGroup(aID,aName);
    //printf(" Object with name %s is a group : begin\n", aName);
    //int retval = 
    H5Giterate(aID,aName, NULL,file_info,visitor);
    visitor->endGroup(aID,aName);
    //printf(" Object with name %s is a group : end\n", aName);
  }break;
  case H5G_DATASET: 
    visitor->dataSet(aID,aName);
    //printf(" Object with name %s is a dataset \n", aName);
    break;
  case H5G_TYPE: 
    //printf(" Object with name %s is a named datatype \n", aName);
    break;
  default:
    printf(" Unable to identify an object ");
  }
  return 0;
}

namespace BatchLab {
namespace HDF5 {
  class KeysVisitor : public virtual IVisitor {
  public:
    KeysVisitor(Manager& aManager,
                std::string& aOut,std::ostream& a_out)
    :fManager(aManager),fOut(aOut),f_out(a_out){}
    virtual ~KeysVisitor() {}
  public:
    virtual bool dataSet(hid_t,const std::string&) { return true;}
    virtual bool beginGroup(hid_t aID,const std::string& aName) {
      hid_t id = H5Gopen(aID,aName.c_str());
      if(id<0) {
        f_out << "BatchLab::HDF5::KeysVisitor::beginGroup :" 
            << " can't open group " << inlib::sout(aName)
            << std::endl;
        return true;
      }
      std::string stype;
      if(!BatchLab::HDF5::HDF5O::read_atb(id,"type",stype)) {
        f_out << "BatchLab::HDF5::KeysVisitor::beginGroup :" 
            << " can't read group " << inlib::sout(aName) << " type."
            << std::endl;
        H5Gclose(id);
        return true;
      }
      std::string title;
      if(H5LT_find_attribute(id,"title")==1) {
      if(!BatchLab::HDF5::HDF5O::read_atb(id,"title",title)) {
        f_out << "BatchLab::HDF5::KeysVisitor::beginGroup :" 
            << " can't read group " << inlib::sout(aName) << " title."
            << std::endl;
        H5Gclose(id);
        return true;
      }
      }

      //printf("debug : BatchLab::HDF5::KeysVisitor::beginGroup : class \"%s\" name \"%s\"\n",stype.c_str(),aName.c_str());

      if(stype=="BatchLab::Directory") {
        fStack.push_front(aName);
        fOut += 
          "<treeItem><label>" + inlib::to_xml(aName) + "</label>";
 
      } else {
        std::string s;
        s += inlib::to_xml(aName);
        //s += ";";
        //std::string cs;
        //inlib::sprintf(cs,32,"%d",key->cycle());
        //s += cs;
        //if(aAll) {
          s += " class=";
          s += stype;
          s += " title=";
          s += "\""+title+"\"";
        //}
        fOut += "<treeItem><label>" + s + "</label></treeItem>";
      }

      H5Gclose(id);
      return true;
    }
    virtual bool endGroup(hid_t aID,const std::string& aName) { 
      hid_t id = H5Gopen(aID,aName.c_str());
      if(id<0) {
        f_out << "BatchLab::HDF5::KeysVisitor::endGroup :" 
            << " can't open group " << inlib::sout(aName)
            << std::endl;
        return true;
      }
      std::string stype;
      if(!BatchLab::HDF5::HDF5O::read_atb(id,"type",stype)) {
        f_out << "BatchLab::HDF5::KeysVisitor::endGroup :" 
            << " can't read group " << inlib::sout(aName) << " type."
            << std::endl;
        H5Gclose(id);
        return true;
      }
      if(stype=="BatchLab::Directory") {
        fStack.pop_front();
        fOut += "</treeItem>";
      }
      H5Gclose(id);
      return true;
    }
  private:
    Manager& fManager;
    std::string& fOut;
    std::ostream& f_out;
    std::list<std::string> fStack;
};
}}

std::string BatchLab::HDF5::Tree::hierarchy(const std::string& aWhat) const {
  if(aWhat=="memory") return MemoryTree::hierarchy();

  if(fFileID<0) return "";

  std::string sout;
  sout += "<tree>";

  KeysVisitor visitor(fManager,sout,out());
  H5Giterate(fFileID,"/",NULL,file_info,(BatchLab::HDF5::IVisitor*)&visitor);
  sout += "</tree>";

  return sout;
}

AIDA::IManagedObject* BatchLab::HDF5::Tree::find(const std::string& aPath) {
  // aPath = /dir1/dir2/.../SID
  //       = SID   (where SID = storage identifier)
  // Could bring in memory two histos with same SID.
  if(aPath.empty()) return 0;

  // Look first in memory :
 {AIDA::IManagedObject* object = MemoryTree::find(aPath);
  if(object) {
    if(verboseLevel()) {
      out() << "BatchLab::HDF5::Tree::find :" 
          << " find " << inlib::sout(aPath) << " in memory."
          << std::endl;
    } 
    return object;
  }}

  // Not found in memory, get in file :
  if(fFileID<0) return 0;

  std::string path,SID,suffix;
  inlib::path_name_suffix(aPath,path,SID,suffix);
 
  hid_t id = H5Gopen(fFileID,aPath.c_str());
  if(id<0) {
    out() << "BatchLab::HDF5::Tree::find :"
        << " can't find container " << inlib::sout(path) 
        << " in store " << inlib::sout(storeName()) << "." 
        << std::endl;
    return 0;
  }

  Slash::Store::IObject* object = retreiveObject(id,SID);
  if(!object) {
    out() << "BatchLab::HDF5::Tree::find :"
        << " can't retreive object " << inlib::sout(SID) 
        << " in store " << inlib::sout(storeName()) << "." 
        << std::endl;
    H5Gclose(id);
    return 0;
  } else {
    if(!MemoryTree::mkdirs(path)) {
      delete object;
      H5Gclose(id);
      return 0;
    }
    if(!manageObject(object,path)) {
      delete object;
      H5Gclose(id);
      return 0;
    }
  }

  AIDA::IManagedObject* aobject = INLIB_CAST(*object,AIDA::IManagedObject);
  if(!aobject) {
    out() << "BatchLab::HDF5::Tree::find :"
        << " for " << inlib::sout(aPath) 
        << ",Slash::Store::IObject not a AIDA::IManagedObject."
        << " in store " << inlib::sout(storeName()) << "." 
        << std::endl;
    H5Gclose(id);
    return 0;
  }

  H5Gclose(id);
  return aobject;
}

Slash::Store::IObject* BatchLab::HDF5::Tree::retreiveObject(
 hid_t aID //group
,const std::string& aSID
){
  std::string sclass;
  if(!BatchLab::HDF5::HDF5O::read_atb(aID,"type",sclass)) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " can't read group " << inlib::sout(aSID) << " type."
        << std::endl;
    return 0;
  }

  int version = NotFound;
  if(H5LT_find_attribute(aID,"version")==1) {
  if(!BatchLab::HDF5::HDF5O::read_atb(aID,"version",version)) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " can't read " << inlib::sout(aSID) << " version."
        << std::endl;
    return 0;
  }}

  if(verboseLevel()) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " name " << inlib::sout(aSID)
        << "..." 
        << std::endl;
  }

  Slash::Meta::IDictionary* manager = Slash::dictionary(session());
  if(!manager) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " Lib::Dictionary manager not found." 
        << std::endl;		 
    return 0;
  }
  Slash::Meta::IClass* cls = manager->find(sclass);    
  if(!cls) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " no entry in Lib::Dictionary found for class " 
        << inlib::sout(sclass) << "."
        << std::endl;		 
    return 0;
  }
  Slash::Meta::IObject* mobj = cls->create();
  if(!mobj) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " can't create an object for class " << inlib::sout(sclass) << "."
        << std::endl;		 
    return 0;
  }
  Slash::Store::IObject* sobj = INLIB_CAST(*mobj,Slash::Store::IObject);
  if(!sobj) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " can't cast Slash::Meta::IObject to Slash::Store::IObject"
        << " for class \"" << sclass << "\"."
        << std::endl;		 
    delete mobj;
    return 0;
  }
  sobj->setName(aSID);

  Slash::Store::IStorable* visited = INLIB_CAST(*mobj,Slash::Store::IStorable);
  if(!visited) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " can't cast Slash::Meta::IObject to Slash::Store::IStorable"
        << " for class \"" << sclass << "\"."
        << std::endl;		 
    delete mobj;
    return 0;
  }

 {unsigned int sz = 0;
  char* buffer = 0;
  if(!fHDF5O->read_object(aID,aSID,sz,buffer)) {
    delete mobj;
    return 0;
  }
  char* p = buffer;
  char* end = buffer+sz;
  Lib::ObjectReader mReader(session(),p,end);
  visited->read(mReader);
  delete [] buffer;
  if(p!=end) {
    delete mobj;
    return 0;
  }}

  if(verboseLevel()) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " name " << inlib::sout(aSID)
        << " done." << std::endl;
  }

  return sobj;

/*
  BatchLab::HDF5::Reader reader = fManager.findReader(sclass);
  if(!reader) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " reader not found for class " << inlib::sout(stype)
        << std::endl;
    return 0;
  } 
  BatchLab::HDF5::ReadContext ctx
    (verboseLevel(),out(),aID,aSID,version,*fHDF5O);

  Slash::Store::IObject* object = reader(ctx);
  if(!object) {
    out() << "BatchLab::HDF5::Tree::retreiveObject :" 
        << " can't read object " << inlib::sout(aSID)
        << std::endl;
    return 0;
  }

  return object;
*/
}
