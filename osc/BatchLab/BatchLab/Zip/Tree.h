#ifndef BatchLab_Zip_Tree_h
#define BatchLab_Zip_Tree_h

#include <BatchLab/Core/MemoryTree.h>

namespace BatchLab {namespace Zip {class Manager;}}

#include <BatchLab/XML/AIDA_ML.h>
#include <BatchLab/XML/WAXML.h>
#include <BatchLab/Version.h>

#include <inlib/xml/tree>
#include <exlib/zip>

namespace BatchLab {

namespace Zip {

class Tree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::Zip::Tree)
    else return MemoryTree::cast(a_class);
  }
public: //AIDA/ITree
  virtual bool commit() {
    if(fReadOnly) return false;
    std::string fileName = name(); //!!!
    if(fCreateNew) {
      ::remove(fileName.c_str());
    } else {
      //Can't update a file !
      return false;
    }
  
    // The name may contain env variables.
    std::string fullName;
    inlib::file_name(fileName,fullName);

//#ifdef HAS_ZIP
    exlib::ozip zp(fullName,out());
    if(!zp.opened()) return false;
  
    inlib::xml::default_factory factory; //FIXME
    Zip_StoreVisitor visitor(*this,factory,zp,out());
    rootFolder().visit(visitor);
  
    zp.close();
//#else
//    out() << "BatchLab::Zip::Tree::commit() :"
//          << " package not built with zip."
//          << std::endl;
 //#endif
  
    return true;
   }


  virtual bool close() {return true;}
  virtual std::string storeName() const {return fStoreName;}
  virtual std::string storeType() const {return "BatchLab::Zip::Tree";}
public:
  Tree(Slash::Core::ISession& aSession,const std::string& aStoreName,
       bool aReadOnly,bool aCreateNew,
       const std::string& aOptions,
       Zip::Manager& aManager,int aVerboseLevel)
  :MemoryTree(aSession,aStoreName) //FIXME : have a name # storeName
  ,fManager(aManager)
  ,fStoreName(aStoreName)
  ,fNameCounter(0)
  ,fReadOnly(aReadOnly)
  ,fCreateNew(aCreateNew)
  ,fCompress(false)
  ,fIsValid(true)
  {
    setVerboseLevel(aVerboseLevel);
  
    inlib::args args(aOptions,";",true);
    args.find("compress",fCompress);
  
    //printf("debug : Zip::Tree :<-- \"%s\" \"%s\" %d -->\n",
    // aStoreName.c_str(),aOptions.c_str(),fCompress);
  
//#ifdef HAS_ZIP
    if(fCreateNew) {
      // Handled in the commit.
    } else {
      // Read the existing file :
      if(verboseLevel()) {
        out() << "BatchLab::Zip::Tree::Zip::Tree :"
            << " open " << inlib::sout(aStoreName) 
            << " with compression=" 
            << std::string(fCompress?"yes":"no") << "..." 
            << std::endl;
      }
      AIDA_Zip aidaZip(*this);
      if(!aidaZip.load_file(aStoreName)) {
        out() << "BatchLab::Zip::Tree::Zip::Tree :"
            << " problem loading file " << inlib::sout(aStoreName)
            << std::endl;
        fIsValid = false;
        return;
      }
      if(verboseLevel()) {
        out() << "BatchLab::Zip::Tree::Zip::Tree :"
            << " load of " << inlib::sout(aStoreName) << " done."
            << std::endl;
      }
    }
//#else
//  out() << "BatchLab::Zip::Tree::Zip::Tree :"
//        << " built without zip." 
//        << std::endl;
//  fCompress = false;
//#endif
  }

  virtual ~Tree(){}
public:
  bool isValid() const {return fIsValid;}
  bool compress() const {return fCompress;}
private:
  class Zip_StoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    Zip_StoreVisitor(Zip::Tree& aTree,
                     inlib::xml::factory& aFactory,
                     exlib::ozip& a_ozip,
                     std::ostream& a_out)
    :fTree(aTree)
    ,fWAXML(a_out)
    ,f_ozip(a_ozip),f_out(a_out)
    ,fVerboseLevel(fTree.verboseLevel())
    //,fVerboseLevel(1)
    {}
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fVerboseLevel) {
        f_out << "BatchLab::Zip_StoreVisitor::visit :" 
            << " path : " << inlib::sout(aFolder.path())
            << " name : " << inlib::sout(aObject.name())
            << std::endl;
      }

      // Strip the first slash if any.
      std::string path = aFolder.path()+"/"+aObject.name();
      if(path.size()&&(path[0]=='/')) path = path.substr(1,path.size()-1);

      // Append the type/class of object :
      path += ".";
      std::string sclass;
      if(const AIDA::IManagedObject* amo = 
          INLIB_CONST_CAST(aObject,AIDA::IManagedObject)) {
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
        if(!amanip::type(*amo,sclass)) {
          if(INLIB_SAFE_CAST(*amo,Slash::Data::IArray)) {
            sclass = "Slash::Data::IArray";;
          }
        }
#else
        sclass = amo->type();
#endif
        inlib::replace(sclass,"AIDA::",""); //IHistogram1D
      } else {
        sclass = aObject.storeClassName();
      }
      //Windows does not like file name with "::".
      inlib::replace(sclass,"::","_");    //Slash_Data_IArray
      path += sclass;

      if(!f_ozip.open_entry(path)) return false;

      // Header :
      f_ozip << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
      f_ozip << "<!DOCTYPE aida SYSTEM \"http://aida.freehep.org/schemas/3.0/aida.dtd\">" << std::endl;
      f_ozip << "<aida version=" << inlib::sout(AIDA_VERSION) << ">" << std::endl;
      f_ozip << "  <implementation package=" << inlib::sout("BatchLab") 
          << " version=" << inlib::sout(BATCHLAB_VERSION) << "/>" << std::endl;

      if(!fWAXML.write(f_ozip,aFolder.path(),aObject)) {
        //continue anyway.
      }

      // Footer :
      f_ozip << "</aida>" << std::endl;

      return f_ozip.close_entry();
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder& aFolder){ 
      if(fVerboseLevel) {
        f_out << "BatchLab::Zip_StoreVisitor::beginVisit :" 
            << " path : " << inlib::sout(aFolder.path())
            << std::endl;
      }
      // Strip the first slash if any and append one if needed.
      std::string path = aFolder.path();
      if(path.size()&&(path[0]=='/')) path = path.substr(1,path.size()-1);
      if(path.size()&&(path[path.size()-1]!='/')) path += "/";
      if(!f_ozip.open_entry(path)) return false;
      return f_ozip.close_entry();
    }
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder& aFolder){ 
      if(fVerboseLevel) {
        f_out << "BatchLab::Zip_StoreVisitor::endVisit :" 
            << " path : " << inlib::sout(aFolder.path())
            << std::endl;
      }
      return true;
    }
    virtual bool isRecursive() const { return true;}
  private:
    Zip::Tree& fTree;  
    WAXML fWAXML;
    exlib::ozip& f_ozip;
    std::ostream& f_out;
    int fVerboseLevel;
  };

  class AIDA_Zip : public exlib::base_zip {
  public:
    AIDA_Zip(BaseTree& aTree)
    : exlib::base_zip(aTree.out())
    ,fTree(aTree)
    ,fVerbose(fTree.verboseLevel())
    //,fVerbose(1)
    {}
  public:
    virtual bool visit_entry(const std::string& aEntry,
                             unsigned int aSize,
                             const char* aBuffer) {
      if(fVerbose) {
        fTree.out() << "AIDA_Zip::visit_entry :" 
            << " entry " << inlib::sout(aEntry)
            << " (size " << aSize << ")..."
            << std::endl;
      }
  
      inlib::xml::default_factory factory;
      AIDA_ML aidaML(factory,fTree);
      if(!aidaML.load_buffer(aSize,aBuffer) || aidaML.errors()) {
        fTree.out() << "AIDA_Zip::visit_entry :" 
            << " problem loading entry " << inlib::sout(aEntry)
            << ". Continue anyway."
            << std::endl;
        return true;
      }
      if(fVerbose) {
        fTree.out() << "AIDA_Zip::visit_entry :" 
            << " load of " << inlib::sout(aEntry) << " done."
            << std::endl;
      }
      return true;
    }
  private:
    BaseTree& fTree;
    int fVerbose;
  };
private:
  Zip::Manager& fManager;
  std::string fStoreName;
  int fNameCounter;
  bool fReadOnly;
  bool fCreateNew;
  bool fCompress;
  bool fIsValid;
};

} //Zip

} //BatchLab

#endif
