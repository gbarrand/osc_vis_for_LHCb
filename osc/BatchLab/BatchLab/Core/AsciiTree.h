#ifndef BatchLab_AsciiTree_h
#define BatchLab_AsciiTree_h

// Inheritance :
#include <BatchLab/Core/MemoryTree.h>

#include <BatchLab/Core/AsciiTuple.h> //FIXME

#include <amanip/tuple.h>
#include <inlib/system>
#include <fstream>

namespace BatchLab {

class AsciiTree : public MemoryTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::AsciiTree)
    else return MemoryTree::cast(a_class);
  }

public: //AIDA/ITree
  virtual bool commit(){
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
    std::ofstream fout(fullName.c_str());
    if(fout.fail()) return false;
    AsciiStoreVisitor visitor(*this,fout,out());
    rootFolder().visit(visitor);
    fout.close();
    return true;
  }

  virtual std::string storeName() const {return fStoreName;}
  //AIDA-v3r3p0
  virtual std::string storeType() const {return "BatchLab::AsciiTree";}
  virtual bool isReadOnly() const {return fReadOnly;}
public:
  AsciiTree(Slash::Core::ISession& aSession,
            const std::string& aName,const std::string& aStoreName,
            bool aReadOnly,bool aCreateNew,
            const std::string& /*aOptions*/)
  :MemoryTree(aSession,aName)
  ,fStoreName(aStoreName)
  ,fReadOnly(aReadOnly)
  ,fCreateNew(aCreateNew)
  ,fHippoStyle(false)
  {
    if(fCreateNew) {
      // Handled in the commit.
    } else {
      // Read the existing file :
      if(verboseLevel()) {
        out() << "BatchLab::AsciiTree::AsciiTree :"
              << " open " << inlib::sout(aStoreName) 
              << "..." << std::endl;
      }
  
      std::string path,name,suffix;
      if(!inlib::path_name_suffix(aStoreName,path,name,suffix)) return;
      std::string name_only = inlib::nosuffix(name,false);
      
      BatchLab::AsciiTuple* object = 
        new BatchLab::AsciiTuple(out(),name_only,"",aStoreName);
    //if(!MemoryTree::mkdirs("/")) {
    //  delete object;
    //}
      if(!manageObject(object,"/")) {
        delete object;
      }
  
      if(verboseLevel()) {
        out() << "BatchLab::AsciiTree::AsciiTree :"
              << " load of " << inlib::sout(aStoreName) << " done."
              << std::endl;
      }
  
    }
  }
  
  virtual ~AsciiTree(){}
public:
  bool createNew() const {return fCreateNew;}
  void setHippoStyle(bool aValue){fHippoStyle = aValue;}
  bool hippoStyle() const {return fHippoStyle;}
private:
  class AsciiStoreVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    AsciiStoreVisitor(AsciiTree& aTree,
                      std::ostream& a_io,
                      std::ostream& a_out)
    :fTree(aTree),f_io(a_io),f_out(a_out),f_done(false) {}
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fTree.verboseLevel()) {
        f_out << "BatchLab::AsciiStoreVisitor::visit :" 
              << " path : " << inlib::sout(aFolder.path())
              << " name : " << inlib::sout(aObject.name())
              << std::endl;
      }

      if(const AIDA::ITuple* object =
           INLIB_CONST_CAST(aObject,AIDA::ITuple)) {

        if(f_done) {
          f_out << "BatchLab::AsciiStoreVisitor::visit :" 
                << " path : " << inlib::sout(aFolder.path())
                << " name : " << inlib::sout(aObject.name())
                << " object not written. A tuple had already been written."
                << std::endl;
          return false;
        }

        if(fTree.hippoStyle()) {
          if(!amanip::write(*(const_cast<AIDA::ITuple*>(object)),
                            f_io,true,true,
                            f_out)) return false;
        } else {
          if(!amanip::write(*(const_cast<AIDA::ITuple*>(object)),
                            f_io,false,false,
                            f_out)) return false;
        }

        f_done = true;

      } else {

        f_out << "BatchLab::AsciiStoreVisitor::visit :" 
              << " path : " << inlib::sout(aFolder.path())
              << " name : " << inlib::sout(aObject.name())
              << " object not written (only tuples are supported)."
              << std::endl;

      }
        
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder&){ return true;}
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ return true;}
    virtual bool isRecursive() const { return true;}
  private:
    AsciiTree& fTree;
    std::ostream& f_io;
    std::ostream& f_out;
    bool f_done;
  };

private:
  std::string fStoreName;
  bool fReadOnly;
  bool fCreateNew;
  bool fHippoStyle;
};

}

#endif
