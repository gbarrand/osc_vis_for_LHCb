#ifndef BatchLab_LinkFolder_h
#define BatchLab_LinkFolder_h

// Inheritance :
#include <BatchLab/Core/BaseFolder.h>

namespace BatchLab {

class LinkFolder : public BaseFolder {
public: //Slash::Store::IFolder
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Store::IFolder)
    else INLIB_IF_CAST(BatchLab::BaseFolder)
    else INLIB_IF_CAST(BatchLab::LinkFolder)
    else return 0;
  }

  virtual void clear(){fLinkedFolder->clear();}
  virtual void add(Slash::Store::IObject* aObject) {
    fLinkedFolder->add(aObject);
  }
  virtual void remove(Slash::Store::IObject* aObject) {
    fLinkedFolder->remove(aObject);
  }
  virtual void add(Slash::Store::IFolder* aFolder) {
    fLinkedFolder->add(aFolder);
  }
  virtual void remove(Slash::Store::IFolder* aFolder) {
    fLinkedFolder->remove(aFolder);
  }
  virtual bool link(Slash::Store::IFolder* aFolder,const std::string& aName){
    return fLinkedFolder->link(aFolder,aName);
  }
  virtual bool unlink(Slash::Store::IFolder* aFolder) {
    return fLinkedFolder->unlink(aFolder);
  }
  virtual Slash::Store::IFolder* mkdir(const std::string& aName) {
    return fLinkedFolder->mkdir(aName);
  }
  virtual Slash::Store::IObjectIterator* objects() const {
    return fLinkedFolder->objects();
  }
  virtual Slash::Store::IFolderIterator* folders() const  {
    return fLinkedFolder->folders();
  }
public:
  LinkFolder(std::ostream& a_out,
             Slash::Store::IFolder* aParent,
             const std::string& aName,
             Slash::Store::ITree& aTree,Slash::Store::IFolder* aLinkedFolder)
  :BaseFolder(a_out,aParent,aName,aTree)
  ,fLinkedFolder(aLinkedFolder) {
    fLinkedFolder->setLinkingFolder(this);
  }
  virtual ~LinkFolder(){
    if(fLinkedFolder) fLinkedFolder->setLinkingFolder(0);
  }
public:
  Slash::Store::IFolder* linkedFolder() const {return fLinkedFolder;}
  void setLinkedFolder(Slash::Store::IFolder* aLinkedFolder){
    fLinkedFolder = aLinkedFolder;
  }
  virtual bool isSymlink() {
    // If not a symlink, it is a mouting point.
    return (&(tree())==&(fLinkedFolder->tree()));
  }
private:
  Slash::Store::IFolder* fLinkedFolder;
};

}

#endif
