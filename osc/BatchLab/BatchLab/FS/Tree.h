#ifndef BatchLab_FS_Tree_h
#define BatchLab_FS_Tree_h

#include <BatchLab/Core/MemoryTree.h>

namespace exlib {namespace xml {class tree;}}

namespace BatchLab {namespace FS {class Manager;}}

#include <BatchLab/XML/WAXML.h>
#include <BatchLab/Version.h>

#include <amanip/write.h>

#include <inlib/system>
#include <inlib/sys/dir>

#include <inlib/xml/tree>

//#ifdef HAS_ZLIB
#include <exlib/gzip>
//#endif

#include <fstream>

namespace BatchLab {

namespace FS {

class Tree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::FS::Tree)
    else return MemoryTree::cast(a_class);
  }
public: //AIDA/ITree
  virtual bool commit(){
    if(fReadOnly) return false;
    std::string fileName = name(); //!!!
    if(fCreateNew) {
    } else {
      //Can't update for the moment.
      return false; //FIXME.
    }
  
    // The name may contain env variables.
    std::string fullName;
    inlib::file_name(fileName,fullName);
  
    std::string pwd;
    if(!inlib::dir::pwd(pwd)) {
      out() << "BatchLab::FS::Tree::commit :" 
          << " can't get current directory."
          << std::endl;
      return false;
    }
  
   {std::string path,name,suffix;
    inlib::path_name_suffix(fullName,path,name,suffix);
    if(path.size() && !inlib::dir::cd(path)) {
      out() << "BatchLab::FS::Tree::commit :" 
          << " can't go in " << inlib::sout(path)
          << std::endl;
      return false;
    }
    if(fCreateNew) {
      if(inlib::dir::in_fs(name)) {
        if(!inlib::dir::rmdir(name)) {
          out() << "BatchLab::FS::Tree::commit :" 
              << " can't remove " << inlib::sout(name)
              << std::endl;
          return false;
        }
      }
    }
    if(!inlib::dir::mkcd(name)) {
      out() << "BatchLab::FS::Tree::commit :" 
          << " can't mkcd " << inlib::sout(name)
          << std::endl;
      return false;
    }}
  
    inlib::xml::default_factory factory;
    FS_StoreVisitor visitor(*this,factory,out());
    rootFolder().visit(visitor);
  
    if(pwd.size() && !inlib::dir::cd(pwd)) {
      out() << "BatchLab::FS::Tree::commit :" 
          << " can't go in " << inlib::sout(pwd)
          << std::endl;
      return false;
    }
  
    return true;
  }

  virtual bool close(){return true;}

  virtual std::string storeName() const {return fStoreName;}
  virtual std::string storeType() const {return "BatchLab::FS::Tree";} //AIDA-v3r3p0
public:
  Tree(Slash::Core::ISession& aSession,
       const std::string& aStoreName,
       bool aReadOnly,bool aCreateNew,
       const std::string& aOptions,
       FS::Manager& aManager,int aVerboseLevel)
  :MemoryTree(aSession,aStoreName) //FIXME : have a name # storeName
  ,fManager(aManager)
  ,fStoreName(aStoreName)
  ,fNameCounter(0)
  ,fReadOnly(aReadOnly)
  ,fCreateNew(aCreateNew)
  ,fCompress(false)
  ,fIsValid(true)
  ,fStreaming(XML)
  ,fHeader(true)
  ,fTabSep(false){
    setVerboseLevel(aVerboseLevel);
  
    inlib::args args(aOptions,";",true);
    args.find("compress",fCompress);
  
    std::string value;
    if(args.find("streaming",value)) {
      if(value=="ascii") fStreaming = ASCII;
    }
  
    bool bval;
    if(args.find("header",bval)) fHeader = bval;
    if(args.find("tab",value)) fTabSep = bval;
  
    //printf("debug : FS::Tree :<-- \"%s\" \"%s\" %d -->\n",
    // aStoreName.c_str(),aOptions.c_str(),fCompress);
  
  }

  virtual ~Tree(){}
public:
  bool isValid() const {return fIsValid;}
  bool compress() const {return fCompress;}
  enum eStreaming {XML,ASCII};
  eStreaming streaming() const {return fStreaming;}
  bool header() const {return fHeader;}
  bool tabSep() const {return fTabSep;}
private:
  class FS_StoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    FS_StoreVisitor(FS::Tree& aTree,
                     inlib::xml::factory& aFactory,
                     //Lib::FS_Writer& aWriter,
                     std::ostream& a_out)
    :fTree(aTree)
    ,fWAXML(a_out)
    //,fWriter(aWriter)
    ,f_out(a_out)
    ,fVerboseLevel(fTree.verboseLevel())
    //,fVerboseLevel(1)
    {}
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fVerboseLevel) {
        f_out << "BatchLab::FS_StoreVisitor::visit :" 
            << " path : " << inlib::sout(aFolder.path())
            << " name : " << inlib::sout(aObject.name())
            << std::endl;
      }

      std::string name = aObject.name();

      // Append the type/class of object :
      name += ".";
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
      name += sclass;

      if(fTree.compress()) {
//#ifdef HAS_ZLIB
	exlib::ogzip fout(name,f_out);
        if(!fout.opened()) return false;
        file_write(fout,aObject,aFolder.path());
        fout.close();
//#else
//        std::ofstream fout(name.c_str());
//        if(fout.fail()) return false;
//        file_write(fout,aObject,aFolder.path());
//        fout.close();
//#endif
      } else {
        std::ofstream fout(name.c_str());
        if(fout.fail()) return false;
        file_write(fout,aObject,aFolder.path());
        fout.close();
      }

      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder& aFolder){ 
      if(fVerboseLevel) {
        f_out << "BatchLab::FS_StoreVisitor::beginVisit :" 
            << " path : " << inlib::sout(aFolder.path())
            << std::endl;
      }
      return inlib::dir::mkcd(aFolder.name());
    }
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder& aFolder){ 
      if(fVerboseLevel) {
        f_out << "BatchLab::FS_StoreVisitor::endVisit :" 
            << " path : " << inlib::sout(aFolder.path())
            << std::endl;
      }
      return inlib::dir::cd("..");
    }
    virtual bool isRecursive() const { return true;}
  private:
    void file_write(std::ostream& a_wout,const Slash::Store::IObject& aObject,const std::string& a_path){
      if(fTree.streaming()==FS::Tree::ASCII) {

        if(const AIDA::ITuple* object = 
             INLIB_CONST_CAST(aObject,AIDA::ITuple)) {
          if(!amanip::write(*(const_cast<AIDA::ITuple*>(object)),
                            a_wout,
                            fTree.header(),fTree.tabSep(),
                            f_out)) {
            //continue anyway.
          }
        } else if(const AIDA::IHistogram1D* object = 
                    INLIB_CONST_CAST(aObject,AIDA::IHistogram1D)) {
          if(!amanip::write(*object,a_wout,
                            fTree.header(),fTree.tabSep())) {
            //continue anyway.
          }
        } else if(const AIDA::IHistogram2D* object = 
                    INLIB_CONST_CAST(aObject,AIDA::IHistogram2D)) {

          if(!amanip::write(*object,a_wout,
                            fTree.header(),fTree.tabSep())) {
            //continue anyway.
          }
        } else {

          f_out << "BatchLab::FS_StoreVisitor::visit :" 
              << " path : " << inlib::sout(a_path)
              << " name : " << inlib::sout(aObject.name())
              << " : stream in XML."
              << std::endl;

          // Header :
          a_wout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
          a_wout << "<!DOCTYPE aida SYSTEM \"http://aida.freehep.org/schemas/3.0/aida.dtd\">" << std::endl;
          a_wout << "<aida version=" << inlib::sout(AIDA_VERSION) << ">" << std::endl;
          a_wout << "  <implementation package=" << inlib::sout("BatchLab") 
               << " version=" << inlib::sout(BATCHLAB_VERSION) << "/>" << std::endl;
          if(!fWAXML.write(a_wout,a_path,aObject)) {
            //continue anyway.
          }
          // Footer :
          a_wout << "</aida>" << std::endl;

        }

      } else { //XML

        // Header :
        a_wout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        a_wout << "<!DOCTYPE aida SYSTEM \"http://aida.freehep.org/schemas/3.0/aida.dtd\">" << std::endl;
        a_wout << "<aida version=" << inlib::sout(AIDA_VERSION) << ">" << std::endl;
        a_wout << "  <implementation package=" << inlib::sout("BatchLab") 
             << " version=" << inlib::sout(BATCHLAB_VERSION) << "/>" << std::endl;
        if(!fWAXML.write(a_wout,a_path,aObject)) {
          //continue anyway.
        }
        // Footer :
        a_wout << "</aida>" << std::endl;

      }
    }
  private:
    FS::Tree& fTree;  
    WAXML fWAXML;
    //Lib::FS_Writer& fWriter;
    std::ostream& f_out;
    int fVerboseLevel;
};
private:
  FS::Manager& fManager;
  std::string fStoreName;
  int fNameCounter;
  bool fReadOnly;
  bool fCreateNew;
  bool fCompress;
  bool fIsValid;
  eStreaming fStreaming;
  bool fHeader;
  bool fTabSep;
};

} //FS

} //BatchLab

#endif
