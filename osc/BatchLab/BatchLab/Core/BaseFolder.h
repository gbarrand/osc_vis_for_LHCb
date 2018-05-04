#ifndef BatchLab_BaseFolder_h
#define BatchLab_BaseFolder_h

// Inheritance :
#include <Slash/Store/IFolder.h>

#include <Slash/Store/IObject.h>
#include <Slash/Store/ITree.h>
#include <Slash/Store/IObjectIterator.h>
#include <Slash/Store/IFolderVisitor.h>
#include <Slash/Store/IFolderIterator.h>
#include <Slash/Data/IArray.h>

#include <AIDA/IBaseHistogram.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/IConstants.h>

#include <amanip/mo.h>

#include <Lib/Debug.h>

#include <inlib/srep>
#include <inlib/sto>
#include <inlib/cast>

#include <string>
#include <list>

namespace BatchLab {

class BaseFolder : public virtual Slash::Store::IFolder {
public: //Slash::Store::IFolder
  virtual Slash::Store::IFolder* parent() const {return fParent;}
  virtual std::string name() const {return fName;}
  virtual void setName(const std::string& aName) {fName = aName;}

  virtual Slash::Store::IObject* findInTree(const std::string& aName,
                                       Slash::Store::IFolder*& aFolder) const {
    // Look in folder and descent in sub folders.
    Slash::Store::IObject* object = findObject(aName);
    if(object) {
      aFolder = 
       (Slash::Store::IFolder*)static_cast<const Slash::Store::IFolder*>(this);
      return object;
    }
  
    // children :
    Slash::Store::IFolderIterator* itf = folders();
    if(itf) {
      for(;itf->folder();itf->next()) {
        object = itf->folder()->findInTree(aName,aFolder);
        if(object) {
          delete itf;
          return object;
        }
      }
      delete itf;
    }
    aFolder = 0;
    return 0;
  }
  
  virtual Slash::Store::IObject* findObject(const std::string& aName) const {
    Slash::Store::IObjectIterator* it = objects();
    if(it) {
      for(;it->object();it->next()) {
        Slash::Store::IObject* object = it->object();
        if(aName==object->name()) {
          delete it;
          return object;
        }
      }
      delete it;
    }
    return 0;
  }

  virtual bool isValid(const Slash::Store::IObject* aObject) const {
    Slash::Store::IObjectIterator* it = objects();
    if(it) {
      for(;it->object();it->next()) {
        if(it->object()==aObject) {
          delete it;
          return true;
        }
      }
      delete it;
    }
  
    Slash::Store::IFolderIterator* itf = folders();
    if(itf) {
      for(;itf->folder();itf->next()) {
        if(itf->folder()->isValid(aObject)) {
          delete itf;
          return true;
        }
      }
      delete itf;
    }
    return false;
  }
  
  virtual Slash::Store::IFolder* findFolder(const std::string& aName) const {
    Slash::Store::IFolderIterator* itf = folders();
    if(itf) {
      for(;itf->folder();itf->next()) {
        Slash::Store::IFolder* folder = itf->folder();
        if(folder->name()==aName) {
          delete itf;
          return folder;
        }
      }
      delete itf;
    }
    return 0;
  }

  virtual unsigned int numberOfObjects() const {
    Slash::Store::IObjectIterator* it = objects();
    if(!it) return 0;
    unsigned int number = 0;
    for(;it->object();it->next()) {
      number++;
    }
    return number;
  }

  virtual unsigned int numberOfFolders() const {
    Slash::Store::IFolderIterator* it = folders();
    if(!it) return 0;
    unsigned int number = 0;
    for(;it->folder();it->next()) {
      number++;
    }
    delete it;
    return number;
  }

  virtual void ls(bool aRecursive) const {

    f_out << " ************** Directory ===> "
        << fName
        << " <===" 
        << std::endl;
  
    if(numberOfFolders()) {
      f_out << " ===> List of subdirectories" << std::endl;
      Slash::Store::IFolderIterator* itf = folders();
      if(itf) {
        for(;itf->folder();itf->next()) {
          f_out << "  " << itf->folder()->name() << std::endl;
        }
        delete itf;
      }
    }
  
    f_out << " ===> List of objects" << std::endl;
    Slash::Store::IObjectIterator* it = objects();
    if(it) {
      for(;it->object();it->next()) {
        f_out << "  " << object_ls(*(it->object())) << std::endl;
      }
      delete it;
    }
  
    if(aRecursive) {
      Slash::Store::IFolderIterator* itf = folders();
      if(itf) {
        for(;itf->folder();itf->next()) {
          itf->folder()->ls(aRecursive);
        }
        delete itf;
      }
    }
  }
  
  virtual std::string path() const {
    std::string path = "";
    const Slash::Store::IFolder* folder = 
      static_cast<const Slash::Store::IFolder*>(this);
    while(folder) {
      std::string tmp = path;
      path = "/";
      path += folder->name();
      path += tmp;
      folder = folder->parent();
    }
    inlib::replace(path,"//","/");
    return path;
  }

  virtual bool visit(Slash::Store::IFolderVisitor& aVisitor) const  {
    Slash::Store::IObjectIterator* it = objects();
    if(it) {
      for(;it->object();it->next()) {
        if(!aVisitor.visit(*this,*(it->object()))) {
          delete it;
          return false;
        }
      }
      delete it;
    }
    // Sub folders :
    bool status = true;
    Slash::Store::IFolderIterator* itf = folders();
    if(itf) {
      for(;itf->folder();itf->next()) {
        Slash::Store::IFolder* folder = itf->folder();
        if(!aVisitor.beginVisit(*this,*folder)) {
          delete itf;
          return false;
        }
        if(aVisitor.isRecursive()) status = folder->visit(aVisitor);
        if(!aVisitor.endVisit(*this,*folder)) {
          delete itf;
          return false;
        }
        if(!status) break;
      }
      delete itf;
    }
    return status;
  }

  virtual void setParent(Slash::Store::IFolder* aParent){fParent = aParent;}

  virtual std::string linkingPath() const {
    INLIB_SELF(BatchLab::BaseFolder);
    // Take care of links :
    std::string path = "";
    Slash::Store::IFolder* folder = &self;
    while(folder) {
      std::string tmp = path;
      path = "/";
      path += folder->name();
      path += tmp;
      if(Slash::Store::IFolder* p = folder->linkingFolder()) {
        if(&(p->tree())!=&(folder->tree())) { //Mouting point :
          folder = p;
        } else { //Symlink :
          folder = folder->parent();
        }
      } else {
        folder = folder->parent();
      }
    }
    inlib::replace(path,"//","/");
    return path;
  }

  virtual Slash::Store::ITree& tree(){return fTree;}
  virtual void setLinkingFolder(Slash::Store::IFolder* aFolder) {
    fLinkingFolder = aFolder;
  }
  virtual Slash::Store::IFolder* linkingFolder() const {return fLinkingFolder;}
public:
  BaseFolder(std::ostream& a_out,
             Slash::Store::IFolder* aParent,
             const std::string& aName,Slash::Store::ITree& aTree)
  :f_out(a_out),fParent(aParent),fName(aName),fTree(aTree),fLinkingFolder(0){
    Lib::Debug::increment("BatchLab::BaseFolder");
  }
  virtual ~BaseFolder(){
    Lib::Debug::decrement("BatchLab::BaseFolder");
  }

private:
  inline static std::string object_ls(const Slash::Store::IObject& aObject) {
    std::string s = aObject.name();
    if(const AIDA::IBaseHistogram* aobject = 
         INLIB_CONST_CAST(aObject,AIDA::IBaseHistogram)) {
      s += " ";
      s += "(" + inlib::to<int>(aobject->dimension())+")";
      s += " ";
      s += aobject->title();
    }
    return s;
  }

  class HierarchyVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    HierarchyVisitor(const std::string& aFormat,std::string& aOut)
    :fFormat(aFormat),fOut(aOut) {
    }
  public:
    virtual bool visit(const Slash::Store::IFolder&,
                       const Slash::Store::IObject& aObject){
      const AIDA::IManagedObject* aobject = 
        INLIB_CONST_CAST(aObject,AIDA::IManagedObject);
      if(!aobject) return true; //FIXME : warning.
      fOut += "<treeItem><label>";
      std::string name = inlib::to_xml(aObject.name());
      if(fFormat.empty()) {	
        fOut += name;
      } else {
        fOut += name;
        fOut += " class=";
        std::string sclass;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
        if(!amanip::type(*aobject,sclass)) {
          if(INLIB_SAFE_CAST(*aobject,Slash::Data::IArray)) {
            sclass = "Slash::Data::IArray";;
          }
        }
#else
        sclass = aobject->type();
#endif
        fOut += sclass;
        fOut += " title=";
       {std::string title;
        if(!amanip::title(*aobject,title)) {           
          if(Slash::Data::IArray* array = 
               INLIB_SAFE_CAST(*aobject,Slash::Data::IArray)) {
            title = array->title();
          }
        }
        inlib::to_xml(title);
        fOut += "\""+title+"\"";}
      }
      fOut += "</label></treeItem>";
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder& aFolder){
      std::string name = inlib::to_xml(aFolder.name());
      fOut += "<treeItem><label>" + name + "</label>";
      return true;
    }
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){
      fOut += "</treeItem>";
      return true;
    }
    virtual bool isRecursive() const { return true;}
  private:
    std::string fFormat;
    std::string& fOut;
  };

public:
  std::string hierarchy(const std::string& aFormat = "") const {
    std::string sout;
    HierarchyVisitor visitor(aFormat,sout);
    visit(visitor);
    return sout;
  }

protected:
  std::ostream& f_out;
  Slash::Store::IFolder* fParent;
  std::string fName;
  Slash::Store::ITree& fTree;
  Slash::Store::IFolder* fLinkingFolder;
};

}

#endif
