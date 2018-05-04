#ifndef BatchLab_Folder_h
#define BatchLab_Folder_h

// Inheritance :
#include <BatchLab/Core/BaseFolder.h>

#include <BatchLab/Core/LinkFolder.h>

namespace BatchLab {

class Folder : public BaseFolder {
public: //Slash::Store::IFolder
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Store::IFolder)
    else INLIB_IF_CAST(BatchLab::BaseFolder)
    else INLIB_IF_CAST(BatchLab::Folder)
    else return 0;
  }

  virtual void add(Slash::Store::IObject* aObject){
    fObjects.push_back(aObject);
    fTree.emitUpdate(); //Notify the UI.
  }
  virtual void remove(Slash::Store::IObject* aObject){
    fObjects.remove(aObject);
    fTree.emitUpdate(); //Notify the UI.
  }
  virtual void add(Slash::Store::IFolder* aFolder){
    fFolders.push_back(aFolder);
    fTree.emitUpdate(); //Notify the UI.
  }
  virtual void remove(Slash::Store::IFolder* aFolder){
    fFolders.remove(aFolder);
    fTree.emitUpdate(); //Notify the UI.
  }

  virtual bool link(Slash::Store::IFolder* aFolder,
                    const std::string& aName){
    //FIXME : check that aFolder is not a parent of this.
    if(aFolder==this) {
      f_out << "BatchLab::Folder::link :" 
          << " can't link a folder on itself."
          << std::endl;
      return false;
    }
    if(aFolder->linkingFolder()) {
      f_out << "BatchLab::Folder::link :" 
          << " can't link an already linked folder."
          << std::endl;
      return false;
    }
    if(findFolder(aName)) {
      f_out << "BatchLab::Folder::link :" 
          << " a folder with name " << inlib::sout(aName)
          << " already exists."
          << std::endl;
      return false;
    }
    //FIXME : have to install a notification so that
    //  deleting aFolder will notify the mouting point and "this".
    fFolders.push_back(new LinkFolder(f_out,this,aName,fTree,aFolder));
    return true;
  }

  virtual bool unlink(Slash::Store::IFolder* aFolder){
    BatchLab::LinkFolder* link = INLIB_CAST(*aFolder,BatchLab::LinkFolder);
    if(!link) {
      f_out << "BatchLab::Folder::unlink :" 
          << " folder " << inlib::sout(aFolder->path()) 
          << " is not a link."
          << std::endl;
      return false;
    }
    fFolders.remove(link);
    delete link;
    return true;
  }

  virtual void clear() {
    while(!fObjects.empty()) {
      Slash::Store::IObject* baseObject = *(fObjects.begin());
      // In case the clear is called from ~Folder.
      baseObject->setFolder(0);
      remove(baseObject);
      delete baseObject;
    }
  
    std::list<Slash::Store::IFolder*>::iterator it;
    // Delete sub folders :
    for(it=fFolders.begin();it!=fFolders.end();it=fFolders.erase(it)) {
      Slash::Store::IFolder* folder = *it;
      BatchLab::LinkFolder* link = INLIB_CAST(*folder,BatchLab::LinkFolder);
      if(link) link->setLinkedFolder(0);
      delete *it;
    }
  }

  virtual Slash::Store::IFolder* mkdir(const std::string& aName){
    if(aName.empty()) return 0;
    if(findFolder(aName)) return 0;
    if(findObject(aName)) return 0;
    Slash::Store::IFolder* folder = new Folder(f_out,this,aName,fTree);
    fFolders.push_back(folder);
    return folder;
  }
private:
  class Folder_Objects_Iterator
  : public virtual Slash::Store::IObjectIterator {
  public: //Slash::Data::Slash::Store::IObjectIterator
    virtual Slash::Store::IObject* object() {
      if(fIterator==fList.end()) return 0;
      return *fIterator;
    }
    virtual void next() { ++fIterator; }
  public:
    Folder_Objects_Iterator(const std::list<Slash::Store::IObject*>& aList)
      :fList(aList) {
      fIterator = fList.begin();
      Lib::Debug::increment("BatchLab::Folder_Objects_Iterator");
    }
    virtual ~Folder_Objects_Iterator() {
      Lib::Debug::decrement("BatchLab::Folder_Objects_Iterator");
    }
  private:
    const std::list<Slash::Store::IObject*>& fList;
    std::list<Slash::Store::IObject*>::const_iterator fIterator;
  };
public:
  virtual Slash::Store::IObjectIterator* objects() const {
    return new Folder_Objects_Iterator(fObjects);
  }
private:
  class Folder_Folders_Iterator
  : public virtual Slash::Store::IFolderIterator {
  public: //Slash::Data::Slash::Store::IObjectIterator
    virtual Slash::Store::IFolder* folder() {
      if(fIterator==fList.end()) return 0;
      return *fIterator;
    }
    virtual void next() { ++fIterator; }
  public:
    Folder_Folders_Iterator(const std::list<Slash::Store::IFolder*>& aList)
    :fList(aList) {
      fIterator = fList.begin();
      Lib::Debug::increment("BatchLab::Folder_Folders_Iterator");
    }
    virtual ~Folder_Folders_Iterator() {
      Lib::Debug::decrement("BatchLab::Folder_Folders_Iterator");
    }
  private:
    const std::list<Slash::Store::IFolder*>& fList;
    std::list<Slash::Store::IFolder*>::const_iterator fIterator;
  };
public:
  virtual Slash::Store::IFolderIterator* folders() const  {
    return new Folder_Folders_Iterator(fFolders);
  }
public:
  Folder(std::ostream& a_out,Slash::Store::IFolder* aParent,
         const std::string& aName,Slash::Store::ITree& aTree)
  :BaseFolder(a_out,aParent,aName,aTree)
  {}
  virtual ~Folder(){Folder::clear();}
private:
  std::list<Slash::Store::IFolder*> fFolders;
  std::list<Slash::Store::IObject*> fObjects;
};
}

#endif
