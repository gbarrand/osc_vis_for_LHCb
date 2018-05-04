#ifndef BatchLab_XML_Tree_h
#define BatchLab_XML_Tree_h

#include <BatchLab/Core/MemoryTree.h>

namespace BatchLab {namespace XML {class Manager;}}

#include <BatchLab/XML/AIDA_ML.h>
#include <BatchLab/XML/WAXML.h>
#include <BatchLab/Version.h>

#include <fstream>

namespace BatchLab {

namespace XML {

class Tree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::XML::Tree)
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
    if(fCompress) {
//#ifdef HAS_ZLIB
    exlib::ogzip fout(fullName,out());
    if(!fout.opened()) return false;
    file_write(fout);
    fout.close();
//#else
//    std::ofstream fout(fullName.c_str());
//    if(fout.fail()) return false;
//    file_write(fout);
//    fout.close();
//#endif
    } else {
      std::ofstream fout(fullName.c_str());
      if(fout.fail()) return false;
      file_write(fout);
      fout.close();
    }
    
    return true;
  }

  virtual bool close(){return true;}

  virtual std::string storeName() const {return fStoreName;}
  virtual std::string storeType() const {return "BatchLab::XML::Tree";} //AIDA-v3r3p0
public:
  Tree(Slash::Core::ISession& aSession,
       const std::string& aStoreName,
       bool aReadOnly,bool aCreateNew,
       const std::string& aOptions,
       XML::Manager& aManager,int aVerboseLevel)
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
  
    //printf("debug : XML::Tree :<-- \"%s\" \"%s\" %d -->\n",
    // aStoreName.c_str(),aOptions.c_str(),fCompress);
  
    //if(fCompress && !GZIP_AVAILABLE) {
    //  out() << "BatchLab::XML::Tree::Tree :"
    //      << " compression not available. " 
    //      << " check installation of Lib package."
    //      << std::endl;
    //  fCompress = false;
    //}
    
    if(fCreateNew) {
      // Handled in the commit.
    } else {
      // Read the existing file :
      if(verboseLevel()) {
        out() << "BatchLab::XML::Tree::Tree :"
            << " open " << inlib::sout(aStoreName) 
            << " with compression=" 
            << std::string(fCompress?"yes":"no") << "..." << std::endl;
      }
      inlib::xml::default_factory factory; //FIXME

      AIDA_ML aidaML(factory,*this);
      aidaML.set_read_tag(&aSession);

      if(!aidaML.load_file(aStoreName,fCompress) || aidaML.errors()) {
        out() << "BatchLab::XML::Tree::Tree :"
            << " problem loading file " << inlib::sout(aStoreName)
            << std::endl;
        fIsValid = false;
        return;
      }
      if(verboseLevel()) {
        out() << "BatchLab::XML::Tree::Tree :"
            << " load of " << inlib::sout(aStoreName) << " done."
            << std::endl;
      }

      std::vector<exlib::raxml_out>::iterator it;
      for(it=aidaML.objects().begin();it!=aidaML.objects().end();++it) {
        exlib::raxml_out& ro = (*it);
        Slash::Store::IObject* object = (Slash::Store::IObject*)ro.object();
        const std::string& spath = ro.path();        
        if(object) {
          if(!mkdirs(spath)) {
          } else {
            if(!manageObject(object,spath)) {
            } else {
              ro.disown();
            }
          }
        }
      }  
  
    }
  }

  virtual ~Tree(){}
public:
  bool isValid() const {return fIsValid;}
private:
  void file_write(std::ostream& a_out){
    // Header :
    a_out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    a_out << "<!DOCTYPE aida SYSTEM \"http://aida.freehep.org/schemas/3.0/aida.dtd\">" << std::endl;
    a_out << "<aida version=" << inlib::sout(AIDA_VERSION) << ">" << std::endl;
    a_out << "  <implementation package=" 
          << inlib::sout("BatchLab") 
          << " version=" << inlib::sout(BATCHLAB_VERSION) << "/>" 
          << std::endl;
  
    inlib::xml::default_factory factory; //FIXME
    XML_StoreVisitor visitor(*this,factory,a_out,out());
    rootFolder().visit(visitor);
  
    // Footer :
    a_out << "</aida>" << std::endl;
  }

private:
  class XML_StoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    XML_StoreVisitor(XML::Tree& aTree,
                     inlib::xml::factory& aFactory,
                     std::ostream& a_writer,
                     std::ostream& a_out)
    :fTree(aTree)
    ,fWAXML(a_out)
    ,f_writer(a_writer),f_out(a_out) {
    }
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fTree.verboseLevel()) {
        f_out << "BatchLab::XML_StoreVisitor::visit :" 
            << " path : " << inlib::sout(aFolder.path())
            << " name : " << inlib::sout(aObject.name())
            << std::endl;
      }
      if(!fWAXML.write(f_writer,aFolder.path(),aObject)) {
        //continue anyway.
      }
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder&){ return true;}
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ return true;}
    virtual bool isRecursive() const { return true;}
  private:
    XML::Tree& fTree;  
    WAXML fWAXML;
    std::ostream& f_writer;
    std::ostream& f_out;
  };
private:
  XML::Manager& fManager;
  std::string fStoreName;
  int fNameCounter;
  bool fReadOnly;
  bool fCreateNew;
  bool fCompress;
  bool fIsValid;
};

} //XML

} //BatchLab

#endif
