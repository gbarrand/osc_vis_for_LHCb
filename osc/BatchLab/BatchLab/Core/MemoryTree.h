#ifndef BatchLab_MemoryTree_h
#define BatchLab_MemoryTree_h

// Inheritance :
#include <BatchLab/Core/BaseTree.h>

#include <BatchLab/Core/Folder.h>

#include <AIDA/ITuple.h> //FIXME
#include <BatchLab/Core/TupleFactory.h> //FIXME

namespace BatchLab {

// ITree should come first due to BatchLab::castTree function.

class MemoryTree : public BaseTree {
public: //Slash::Core::IManager
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::MemoryTree)
    else return BaseTree::cast(a_class);
  }
public: //AIDA/ITree
  virtual AIDA::IManagedObject* find(const std::string& aPath){
    Slash::Store::IObject* baseObject = findObject(aPath);
    if(!baseObject) return 0;
    return INLIB_CAST(*baseObject,AIDA::IManagedObject);
  }

  virtual bool cd(const std::string& aPath){
    Slash::Store::IFolder* folder = findFolder(aPath);
    if(!folder) {
      out() << "BatchLab::MemoryTree::cd :" 
          << " can't find folder " << inlib::sout(aPath) << "." 
          << std::endl;
      return false;
    }
    fCurrentFolder = folder;
    return true;
  }

  virtual bool mkdir(const std::string& aPath){
    if(aPath.empty()) return false;
    std::string path,name,suffix;
    inlib::path_name_suffix(aPath,path,name,suffix);
    //printf("debug : mkdir \"%s\" : \"%s\" \"%s\" \"%s\"\n",
      //aPath.c_str(),path.c_str(),name.c_str(),suffix.c_str());
    // name is the last folder to create.
    Slash::Store::IFolder* folder = findFolder(path);
    if(!folder) {
      out() << "BatchLab::MemoryTree::mkdir :" 
          << " can't find folder " << inlib::sout(path) << "." 
          << std::endl;
      return false;
    }
    if(!folder->mkdir(name)) {
      out() << "BatchLab::MemoryTree::mkdir :" 
          << " can't create folder " << inlib::sout(name) 
          << " in folder " << inlib::sout(folder->path()) << "." 
          << std::endl;
      return false;
    }
    if(verboseLevel()) {
      out() << "BatchLab::MemoryTree::mkdir :" 
          << " folder " << inlib::sout(name) 
          << " created in folder " << inlib::sout(folder->path()) << "." 
          << std::endl;
    }
    return true;
  }

  virtual bool mkdirs(const std::string& aPath) {
    return (makeFolders(aPath) ? true : false);
  }

  virtual bool commit(){return true;}
  virtual std::string findPath(const AIDA::IManagedObject& aObject) const {
    const Slash::Store::IObject* baseObject = 
      INLIB_CONST_CAST(aObject,Slash::Store::IObject);
    if(!baseObject) return "";
    FindFolder visitor;
    visitor.fObject = baseObject;
    visitor.fFolder = 0;
    fRoot.visit(visitor);
    Slash::Store::IFolder* folder = visitor.fFolder; //Found folder.
    if(!folder) return "";
    std::string path = folder->linkingPath();
    if(path=="/") return path + aObject.name();
    return path + "/" + aObject.name();
  }


  virtual std::string storeName() const {return "";}

  virtual bool rm(const std::string& aPath){
    if(aPath.empty()) return false;
    std::string path,name,suffix;
    inlib::path_name_suffix(aPath,path,name,suffix);
    // Find folder :
    Slash::Store::IFolder* folder = findFolder(path);
    if(!folder) {
      out() << "BatchLab::MemoryTree::rm :" 
          << " can't find folder " << inlib::sout(path) << "." 
          << std::endl;
      return false;
    }
    Slash::Store::IObject* object = folder->findObject(name);
    if(!object) {
      out() << "BatchLab::MemoryTree::rm :" 
          << " can't find object " << inlib::sout(name)
          << " in folder " << inlib::sout(folder->path()) << "." 
          << std::endl;
      return false;
    }
    if(verboseLevel()) {
      out() << "BatchLab::MemoryTree::rm :" 
          << " " << inlib::sout(object->name())
          << " in folder " << inlib::sout(folder->path()) << "..."
          << std::endl;
    }
    //folder->remove(object);
    delete object; //It remove the object from the folder (See ~BaseObject)
    emitUpdate();
    return true;
  }

  virtual std::string pwd() const {
    return fCurrentFolder->linkingPath();
  }

  virtual bool ls(const std::string& aPath = ".",
                  bool aRecursive = false,
                  std::ostream& = std::cout) const {
    Slash::Store::IFolder* folder = findFolder(aPath);
    if(!folder) {
      out() << "BatchLab::MemoryTree::ls :" 
          << " can't find folder " << inlib::sout(aPath) << "." 
          << std::endl;
      return false;
    }
    //out() << "MemoryTree (" << name() << ") : " << std::endl;
    folder->ls(aRecursive);
    return true;
  }
 
  virtual bool rmdir(const std::string& aPath){
    if(aPath=="/") {
      fRoot.clear();
      fCurrentFolder = &fRoot;
      return true;
    }
    // Find folder :
    Slash::Store::IFolder* folder = findFolder(aPath);
    if(!folder) {
      out() << "BatchLab::MemoryTree::rmdir :" 
          << " can't find folder " << inlib::sout(aPath) << "." 
          << std::endl;
      return false;
    }
    //FIXME : case of linked.
    // Case the found folder is the root (of everything) :
    if(folder==&fRoot) {
      fRoot.clear();
      fCurrentFolder = &fRoot;
      return true;
    }
    // Take care of the current folder :
    if(folder==fCurrentFolder) {
      fCurrentFolder = folder->parent();
    }
    // Then remove the folder :
    folder->clear();
    Slash::Store::IFolder* parent = folder->parent();
    parent->remove(folder);
    delete folder;
    if(verboseLevel()) {
      out() << "BatchLab::MemoryTree::rmdir :" 
          << " folder " << inlib::sout(aPath) << " removed."
          << std::endl;
    }
    return true;
  }

  virtual bool close() {return true;}

  virtual std::vector<std::string> listObjectNames(const std::string& aPath = ".",bool aRecursive = false) const {
    // aPath could be a directory path or an object path
    // (to pass test AIDA/TestTree::testListObjectNames_ListObjectTypes)
    std::vector<std::string> vect;
    std::string path,name,suffix;
    inlib::path_name_suffix(aPath,path,name,suffix);
    Slash::Store::IFolder* folder = findFolder(path);
    if(!folder) {
      out() << "BatchLab::MemoryTree::listObjectNames :" 
          << " path " << inlib::sout(path)
          << " not found in tree " << inlib::sout(storeName()) << "."
          << std::endl;
      return vect;
    }
    if(folder->findObject(name)) {
      vect.push_back(aPath);
      return vect;
    }
    if(name.size()) {
      folder = folder->findFolder(name);
      if(!folder) {
        out() << "BatchLab::MemoryTree::listObjectNames :" 
            << " folder " << inlib::sout(name)
            << " not found in directory " << inlib::sout(path)
            << " of tree " << inlib::sout(storeName()) << "."
            << std::endl;
        return vect;
      }
    }
    ListObjectsVisitor visitor(aRecursive,aPath,vect,true);
    folder->visit(visitor);
    return vect;
  }

  virtual std::vector<std::string> listObjectTypes(const std::string& aPath = ".",bool aRecursive = false) const {
    // aPath could be a directory path or an object path
    // (to pass test AIDA/TestTree::testListObjectNames_ListObjectTypes)
    std::vector<std::string> vect;
    std::string path,name,suffix;
    inlib::path_name_suffix(aPath,path,name,suffix);
    Slash::Store::IFolder* folder = findFolder(path);
    if(!folder) {
      out() << "BatchLab::MemoryTree::listObjectTypes :" 
          << " path " << inlib::sout(path)
          << " not found in tree " << inlib::sout(storeName()) << "."
          << std::endl;
      return vect;
    }
    Slash::Store::IObject* baseObject = folder->findObject(name);
    if(baseObject) {
      AIDA::IManagedObject* aobject = 
        INLIB_CAST(*baseObject,AIDA::IManagedObject);
      if(aobject) {
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
        inlib::replace(sclass,"AIDA::","");
        vect.push_back(sclass);
        return vect;
      }
    }
    if(name.size()) {
      folder = folder->findFolder(name);
      if(!folder) {
        out() << "BatchLab::MemoryTree::listObjectTypes:" 
            << " folder " << inlib::sout(name)
            << " not found in directory " << inlib::sout(path)
            << " of tree " << inlib::sout(storeName()) << "."
            << std::endl;
        return vect;
      }
    }
    ListObjectsVisitor visitor(aRecursive,aPath,vect,false);
    folder->visit(visitor);
    return vect;
  }

  virtual void setOverwrite(bool = true){}

  virtual bool mv(const std::string& aWhat,const std::string& aTo) {
    std::string wpath,wname,wsuffix;
    inlib::path_name_suffix(aWhat,wpath,wname,wsuffix);
    bool wobj = false;
    Slash::Store::IFolder* folder = findFolder(aWhat);
    if(!folder) {
      folder = findFolder(wpath);
      wobj = true;
    }
    if(!folder) {
      out() << "BatchLab::MemoryTree::mv :" 
          << " from path " << inlib::sout(aWhat)
          << " not found in tree " << inlib::sout(name()) << "."
          << std::endl;
      return false;
    }
  
    std::string topath,toname,tosuffix;
    inlib::path_name_suffix(aTo,topath,toname,tosuffix);
    bool toobj = false;
    Slash::Store::IFolder* toFolder = findFolder(aTo);
    if(!toFolder) {
      toFolder = findFolder(topath);
      toobj = true;
    }
    if(!toFolder) {
      out() << "BatchLab::MemoryTree::mv :" 
          << " target path " << inlib::sout(aTo)
          << " not found in tree " << inlib::sout(name()) << "."
          << std::endl;
      return false;
    }

    if(!wobj && !toobj) {
      //Move aWhat directory into aTo directory :
      if(folder==toFolder) { //Can't be moved in itself.
        out() << "BatchLab::MemoryTree::mv :" 
            << " can't move a folder into itself."
            << std::endl;
        return false;
      }
      // Check that toFolder does not have folder as ancestor :
      Slash::Store::IFolder* parent = toFolder->parent();
      while(parent) {
        if(parent==folder) {
          out() << "BatchLab::MemoryTree::mv :" 
              << " Folder " << inlib::sout(aTo)
              << " has folder " << inlib::sout(aWhat)
              << " as ancestor."
              << std::endl;
          return false;
        }
        parent = parent->parent();
      }
      // Seems ok.
      parent = folder->parent();
      if(!parent) return false; //The root directory can't be moved.
      parent->remove(folder);
      toFolder->add(folder);
      folder->setParent(toFolder);
      if(verboseLevel()) {
        out() << "BatchLab::MemoryTree::mv :" 
              << " Folder " << inlib::sout(aWhat)
  	  << " moved into folder " << inlib::sout(aTo) << "."
              << std::endl;
      }
      return true;
  
    } else if(wobj && !toobj) {
      // Move aWhat object into aTo directory :
      Slash::Store::IObject* object = folder->findObject(wname);
      if(!object) {
        out() << "BatchLab::MemoryTree::mv :" 
            << " Object " << inlib::sout(wname)
            << " not found in folder " << inlib::sout(wpath) << "."
            << std::endl;
        return false;
      }  
      folder->remove(object);
      toFolder->add(object);
      if(verboseLevel()) {
        out() << "BatchLab::MemoryTree::mv :" 
            << " Object " << inlib::sout(aWhat)
            << " moved into folder " << inlib::sout(aTo) << "."
            << std::endl;
      }
      return true;
  
    } else if(!wobj && toobj) {
      // Move aWhat directory into aTo directory :
      if(toFolder->findObject(toname)) {
        // Move aWhat directory into aTo object : no sense.
        out() << "BatchLab::MemoryTree::mv :" 
            << " Can't move " << inlib::sout(aWhat)
            << " directory into " << inlib::sout(aTo) << " object."
            << std::endl;
        return false;
      }
      //toname not an existing object or directory.
      if(folder==toFolder) { //Can't be moved in itself.
        out() << "BatchLab::MemoryTree::mv :" 
            << " can't move a folder into itself."
            << std::endl;
        return false;
      }
      // Check that toFolder does not have folder as ancestor :
      Slash::Store::IFolder* parent = toFolder->parent();
      while(parent) {
        if(parent==folder) {
          out() << "BatchLab::MemoryTree::mv :" 
              << " Folder " << inlib::sout(aTo)
              << " has folder " << inlib::sout(aWhat)
              << " as ancestor."
              << std::endl;
          return false;
        }
        parent = parent->parent();
      }
      // Seems ok.
      parent = folder->parent();
      if(!parent) return false; //The root directory can't be moved.
      parent->remove(folder);
      toFolder->add(folder);
      folder->setParent(toFolder);
      folder->setName(toname);
      if(verboseLevel()) {
        out() << "BatchLab::MemoryTree::mv :" 
              << " Folder " << inlib::sout(aWhat)
  	  << " moved in folder " << inlib::sout(aTo) << "."
              << std::endl;
      }
      return true;
  
    } else if(wobj && toobj) {
      // Move aWhat object into aTo object (the renaming it) :
      Slash::Store::IObject* object = folder->findObject(wname);
      if(!object) {
        out() << "BatchLab::MemoryTree::mv :" 
            << " Object " << inlib::sout(wname)
            << " not found in folder " << inlib::sout(wpath) << "."
            << std::endl;
        return false;
      }  
      if(toFolder->findObject(toname)) {
        out() << "BatchLab::MemoryTree::mv :" 
            << " Object " << inlib::sout(aTo)
            << " already exists."
            << std::endl;
        return false;
      }  
      folder->remove(object);
      toFolder->add(object);
      object->setName(toname);
      if(verboseLevel()) {
        out() << "BatchLab::MemoryTree::mv :" 
            << " Object " << inlib::sout(aWhat)
            << " moved to " << inlib::sout(aTo) << "."
            << std::endl;
      }
      return true;
    }
    return false;
  }

  virtual bool cp(const std::string& aFrom,const std::string& aTo,
                  bool = false /*aRecursive*/) {
    if(aTo==aFrom) return true;
    // Is aFrom an object or a folder ?
    Slash::Store::IFolder* fromFolder = findFolder(aFrom);
    if(fromFolder) { //aFrom is a folder.
      Slash::Store::IFolder* toFolder = findFolder(aTo);
      if(!toFolder) {
        out() << "BatchLab::MemoryTree::cp :" 
            << " target path " << inlib::sout(aTo)
            << " is not a folder in tree " << inlib::sout(name()) << "."
            << std::endl;
        return false;
      }
      if(toFolder==fromFolder) return true;
      CopyVisitor visitor(session(),out(),*toFolder,verboseLevel());
      if(verboseLevel()) {
        out() << "BatchLab::MemoryTree::cp :" 
            << " folders found. Start copying..."
            << std::endl;
      }
      fromFolder->visit(visitor);
      return visitor.status();
    } else { //is it an object ?
      Slash::Store::IObject* fromObject = findObject(aFrom);
      if(!fromObject) {
        out() << "BatchLab::MemoryTree::cp :" 
            << " from path " << inlib::sout(aFrom)
            << " not found in tree " << inlib::sout(name()) << "."
            << std::endl;
        return false;
      }
      Slash::Store::IFolder* toFolder = findFolder(aTo);
      if(!toFolder) {
        out() << "BatchLab::MemoryTree::cp :" 
            << " target path " << inlib::sout(aTo)
            << " is not a folder in tree " << inlib::sout(name()) << "."
            << std::endl;
        return false;
      }
      return copy_object(session(),*fromObject,*toFolder);
    }
  }

  virtual bool symlink(const std::string& aPath,const std::string& aLink) {
    return mount(aLink,*this,aPath);
  }

  virtual bool mount(const std::string& aLink,AIDA::ITree& aTree,
                     const std::string& aPath) {
    // aTree is mounted within "this".
    // The aPath path within aTree is mounted at aLink within "this".
    if(aLink.empty()) return false;
    //FIXME : have to install a notification if aTree is deleted.
    
    std::string lpath,lname,suffix;
    inlib::path_name_suffix(aLink,lpath,lname,suffix);
    // lname is the linking point.
    Slash::Store::IFolder* parent = findFolder(lpath);
    if(!parent) {
      out() << "BatchLab::MemoryTree::mount :" 
          << " can't find parent path " << inlib::sout(lpath)
          << " in mounting tree " << inlib::sout(name())
          << std::endl;
      return false;
    }
  
    BatchLab::MemoryTree* batchlabTree = 
      INLIB_CAST(aTree,BatchLab::MemoryTree);
    if(!batchlabTree) {
      out() << "BatchLab::MemoryTree::mount :" 
          << " tree is not a BatchLab::MemoryTree."
          << std::endl;
      return false;
    }
  
    //AIDA/TestTree::testMountAndUnmount : aPath=="" means "/" !
    Slash::Store::IFolder* tfolder = 
      batchlabTree->findFolder(aPath.empty()?"/":aPath);
    if(!tfolder) {
      out() << "BatchLab::MemoryTree::mount :" 
          << " can't find path to mount " << inlib::sout(aPath)
          << " in tree " << inlib::sout(aTree.storeName())
          << std::endl;
      return false;
    }
  
    bool status = parent->link(tfolder,lname);
    if(verboseLevel()) {
      out() << "BatchLab::MemoryTree::mount :" 
          <<  " mount of " << inlib::sout(aPath)
          <<  " on " << inlib::sout(aLink) 
          << (status?" is a success.":" is a failure.") 
          << std::endl;
    }
    return status;
  }

  virtual bool unmount(const std::string& aLink) {
    Slash::Store::IFolder* folder = findFolder(aLink);
    if(!folder) {
      out() << "BatchLab::MemoryTree::unmount :" 
          << " can't find path " << inlib::sout(aLink)
          << " in tree " << inlib::sout(name())
          << std::endl;
      return false;
    }
    folder = folder->linkingFolder();
    if(folder) {
      BatchLab::LinkFolder* link = INLIB_CAST(*folder,BatchLab::LinkFolder);
      if(link) {
        if(link->isSymlink()) { //Symlink
        } else { //Mounting
          Slash::Store::IFolder* parent = folder->parent();
          if(!parent) return false;
          bool status = parent->unlink(folder);
          if(verboseLevel()) {
            out() << "BatchLab::MemoryTree::unmount :" 
                <<  " unmount of " << inlib::sout(aLink)
                << (status?" is a success.":" is a failure.") 
                << std::endl;
          }
          return status;
        }
      }
    }
    out() << "BatchLab::MemoryTree::unmount :" 
        << " folder " << inlib::sout(aLink)
        << " is not a mouting point."
        << std::endl;
    return false;
  }
  
  virtual AIDA::ITree* findTree(const std::string& aPath) {
    Slash::Store::IFolder* folder = findFolder(aPath);
    if(!folder) {
      out() << "BatchLab::MemoryTree::findTree :" 
          << " path " << inlib::sout(aPath)
          << " in tree " << inlib::sout(name())
          << " not found."
          << std::endl;
      return 0;
    }
    AIDA::ITree* aidaTree = INLIB_CAST(folder->tree(),AIDA::ITree);
    if(!aidaTree) {
      out() << "BatchLab::MemoryTree::findTree :" 
          << " folder tree not an AIDA::ITree."
          << std::endl;
    }
    return aidaTree;
  }

  virtual std::string storeType() const { //AIDA-v3r3p0
    return "BatchLab::MemoryTree";
  }
  virtual bool isReadOnly() const {return false;} //AIDA-v3r3p0
public: //Slash::Store::ITree.
  virtual bool addObject(Slash::Store::IObject* aObject,
                         const std::string& aPath){
    if(aObject->name().empty()) {
      out() << "BatchLab::MemoryTree::addObject :"
          << " WARNING : add object with empty name." << std::endl;
    }
    // Find folder :
    Slash::Store::IFolder* folder = findFolder(aPath);
    if(!folder) {
      out() << "BatchLab::MemoryTree::addObject :" 
          << " can't find folder " << inlib::sout(aPath) << "." 
          << std::endl;
      return false;
    }
    folder->add(aObject); //will do a fTree.emitUpdate.
    aObject->setFolder(folder);
    if(verboseLevel()) {
      out() << "BatchLab::MemoryTree::addObject :" 
          << " " << inlib::sout(aObject->name())
          << " in folder " << inlib::sout(folder->path()) << "." 
          << std::endl;
    }
    return true;
  }

  virtual bool isObjectValid(const Slash::Store::IObject* aObject) const {
    return fRoot.isValid(aObject);
  }

  virtual Slash::Store::IObject* findObject(const std::string& aPath){
    if(aPath.empty()) return 0;
    std::string path,name,suffix;
    inlib::path_name_suffix(aPath,path,name,suffix);
    Slash::Store::IFolder* folder = findFolder(path);
    if(!folder) return 0;
    return folder->findObject(name);
  }

public: //Slash::Core::IHierarchy
  virtual std::string hierarchy(const std::string& = "") const {
    std::string sout;
    sout += "<tree>";
    sout += fRoot.hierarchy("all");
    sout += "</tree>";
    return sout;
  }
public: 
  virtual Slash::Store::IFolder* currentFolder() const {return fCurrentFolder;}

  virtual void setCurrentFolder(Slash::Store::IFolder* aFolder) {
    fCurrentFolder = aFolder;
  }

  virtual Slash::Store::IFolder& rootFolder(){return fRoot;}

  virtual std::vector<AIDA::IManagedObject*> allObjects() const {
    std::vector<AIDA::IManagedObject*> vec;
    AllObjectsVisitor visitor(vec);
    fRoot.visit(visitor);
    return vec;
  }
  
  virtual bool isADirectory(const std::string& aPath) const {
    return findFolder(aPath)?true:false;
  }
public:
  MemoryTree(Slash::Core::ISession& aSession,const std::string& aName)
  :BaseTree(aSession,aName)
  ,fRoot(aSession.cout(),0,"",*this)
  ,fCurrentFolder(0){
    Lib::Debug::increment("BatchLab::MemoryTree");
    fCurrentFolder = &fRoot;
    //Note : for UI notification; the below is useless since the 
    //       UI relation is not already set !
    emitUpdate();
  }
  virtual ~MemoryTree() {
    //out() << "BatchLab::MemoryTree::~Tree : called for " << this << " " << fName << std::endl;
    Lib::Debug::decrement("BatchLab::MemoryTree");
  }
protected:
  Slash::Store::IFolder* findFolder(const std::string& aPath,
                                    bool aVerbose = false) const {
    Slash::Store::IFolder* start = 0;
    std::string path;  
    if(aPath.size() && (aPath[0]=='/')) {
      start = (Slash::Store::IFolder*)&fRoot;
      path = aPath.substr(1,aPath.size()-1);
    } else {
      start = fCurrentFolder;
      path = aPath;
    }
    std::vector<std::string> words;
    inlib::words(path,"/",false,words);
    Slash::Store::IFolder* folder = start;
    for(unsigned int index=0;index<words.size();index++) {
      if(words[index]==".") {
      } else if(words[index]=="..") {
        if(Slash::Store::IFolder* p = folder->linkingFolder()) {
          if(&(p->tree())!=&(folder->tree())) { //Mounting.
            folder = p->parent();
          } else { //Symlink.
            folder = folder->parent();
          }
        } else {
          folder = folder->parent();
        }
        if(!folder) {
          if(aVerbose) {
            out() << "BatchLab::MemoryTree::findFolder :" 
                << " input path " << inlib::sout(aPath) << " :"
                << " case .. ; folder without parent."
                << std::endl;
          }
          return 0;
        }
      } else {
        folder = folder->findFolder(words[index]);
        if(!folder) {
          if(aVerbose) {
            out() << "BatchLab::MemoryTree::findFolder :" 
                << " input path " << inlib::sout(aPath) << " :"
                << " can't findFolder " << inlib::sout(words[index])
                << std::endl;
          }
          return 0;
        }
        const BatchLab::LinkFolder* link = 
          INLIB_CONST_CAST(*folder,BatchLab::LinkFolder);
        if(link) {
          folder = link->linkedFolder();
          if(!folder) {
            if(aVerbose) {
              out() << "BatchLab::MemoryTree::findFolder :" 
                  << " input path " << inlib::sout(aPath) << " :"
                  << " can't find linkedFolder for "
                  << inlib::sout(words[index])
                  << std::endl;
            }
            return 0;
          }
        }
      }
    }
    return folder;
  }

  Slash::Store::IFolder* makeFolders(const std::string& aPath) {
    // Leaf in the path is returned.
    Slash::Store::IFolder* start = 0;
    std::string path;  
    if(aPath.size() && (aPath[0]=='/')) {
      start = (Slash::Store::IFolder*)&fRoot;
      path = aPath.substr(1,aPath.size()-1);
    } else {
      start = fCurrentFolder;
      path = aPath;
    }
    std::vector<std::string> words;
    inlib::words(path,"/",false,words);
    Slash::Store::IFolder* folder = start;
    for(unsigned int index=0;index<words.size();index++) {
      if(words[index]==".") {
      } else if(words[index]=="..") {
        folder = folder->parent();
        if(!folder) {
          out() << "BatchLab::MemoryTree::makeFolders :" 
              << " input path " << inlib::sout(aPath) << " :"
              << " case .. ; folder without parent."
              << std::endl;
          return 0;
        }
      } else {
        Slash::Store::IFolder* f = folder->findFolder(words[index]);
        if(f) {
          folder = f;
        } else {
          folder = folder->mkdir(words[index]);
          if(!folder) {
            out() << "BatchLab::MemoryTree::makeFolders :" 
                << " input path " << inlib::sout(aPath) << " :"
                << " can't mkdir " << inlib::sout(words[index])
                << std::endl;
            return 0;
          }
        }
      }
    }
    return folder;
  }
private:
  inline static bool copy_object(Slash::Core::ISession& aSession,
                                 const Slash::Store::IObject& aObject,
                                 Slash::Store::IFolder& aFolder) {
    // target tree.
    Slash::Store::ITree& baseTree = aFolder.tree();
    if(const AIDA::ITuple* tuple = INLIB_CONST_CAST(aObject,AIDA::ITuple)) {
      BatchLab::TupleFactory tupleFactory(aSession,baseTree);
      if(!tupleFactory.createCopy(aObject.name(),*tuple)) {
        std::ostream& out = aSession.cout();
        out << "BatchLab::MemoryTree::copy_object :"
            << " can't create a tuple copy of " << inlib::sout(aObject.name()) 
            << std::endl;
        return false;
      }
    } else {
      Slash::Store::IObject* copy = aObject.copy();
      if(!copy) {
        std::ostream& out = aSession.cout();
        out << "BatchLab::MemoryTree::copy_object :"
            << " unable to copy " << inlib::sout(aObject.name()) 
            << std::endl;
        return false;
      } 
      if(!baseTree.manageObject(copy,aFolder.path())) {
        std::ostream& out = aSession.cout();
        out << "BatchLab::MemoryTree::copy_object :"
            << " unable to manage object " << inlib::sout(aObject.name()) 
            << std::endl;
        return false;
      }
    }
    if(aSession.verboseLevel()) {
      std::ostream& out = aSession.cout();
      out << "BatchLab::MemoryTree::copy_object :"
          << " copied " << inlib::sout(aObject.name())
          << " in folder " << inlib::sout(aFolder.name())
          << " of tree " << inlib::sout(baseTree.storeName())
          << std::endl;
    }
    return true;
  }

private:
  class FindFolder : public virtual Slash::Store::IFolderVisitor {
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(&aObject!=fObject) return true;
      fFolder = (Slash::Store::IFolder*)&aFolder; 
      return false; //Stop visit.
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder&){ return true;}
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ return true;}
    virtual bool isRecursive() const { return true;}
  public:
    const Slash::Store::IObject* fObject;
    Slash::Store::IFolder* fFolder;
  };

  class AllObjectsVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    AllObjectsVisitor(std::vector<AIDA::IManagedObject*>& aVector)
      :fVector(aVector) {}
  public:
    virtual bool visit(const Slash::Store::IFolder&,
                       const Slash::Store::IObject& aObject){
      const AIDA::IManagedObject* aobject = 
        INLIB_CONST_CAST(aObject,AIDA::IManagedObject);
      if(!aobject) return true;
      fVector.push_back(const_cast<AIDA::IManagedObject*>(aobject));
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder&){ return true;}
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ return true;}
    virtual bool isRecursive() const { return true;}
  private:
    std::vector<AIDA::IManagedObject*>& fVector;
  };

  class CopyVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    CopyVisitor(Slash::Core::ISession& aSession,
                std::ostream& a_out,
                Slash::Store::IFolder& aFolder,int aVerboseLevel)
    :fSession(aSession),f_out(a_out),fFolder(aFolder),fStatus(true)
    ,fVerboseLevel(aVerboseLevel){}
    bool status() const { return fStatus;}
  public:
    virtual bool visit(const Slash::Store::IFolder& /*aFolder*/,
                       const Slash::Store::IObject& aObject){
      if(!copy_object(fSession,aObject,fFolder)) {
        fStatus = false;
        return false;
      }
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder&,
                            const Slash::Store::IFolder& /*aSubFolder*/){ 
      return false; //Do not process subfolders.
    }
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ 
      return false;
    }
    virtual bool isRecursive() const { return true;}
  private:
    Slash::Core::ISession& fSession;
    std::ostream& f_out;
    Slash::Store::IFolder& fFolder; //Target folder.
    bool fStatus;
    int fVerboseLevel;
  };

  class ListObjectsVisitor : public virtual Slash::Store::IFolderVisitor {
  public:
    ListObjectsVisitor
     (bool aRecursive
     ,const std::string& aPath
     ,std::vector<std::string>& aVector
     ,bool aNames)
     :fRecursive(aRecursive)
     ,fPath(aPath)
     ,fVector(aVector)
     ,fNames(aNames)
     ,fDone(false) {}
  public:
    virtual bool visit(const Slash::Store::IFolder& aFolder,
                       const Slash::Store::IObject& aObject){
      if(fNames) {
        if(fPath==".") {
          fVector.push_back(aObject.name());
        } else {
          fVector.push_back(aFolder.linkingPath()+"/"+aObject.name());
        }
      } else {
        const AIDA::IManagedObject* aobject = 
          INLIB_CONST_CAST(aObject,AIDA::IManagedObject);
        if(!aobject) return true; //FIXME : warning.

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
        inlib::replace(sclass,"AIDA::","");
        fVector.push_back(sclass);
      }
      return true;
    }
    virtual bool beginVisit(const Slash::Store::IFolder& aFolder,
                            const Slash::Store::IFolder& aSubFolder){ 
      if(fNames) {
        if(fPath==".") {
          fVector.push_back(aSubFolder.name()+"/");
        } else {
          fVector.push_back(aFolder.linkingPath()+"/"+aSubFolder.name()+"/");
        }
      } else {
        fVector.push_back("dir");
      }
      return true;
    }
    virtual bool endVisit(const Slash::Store::IFolder&,
                          const Slash::Store::IFolder&){ 
      return true;
    }
    virtual bool isRecursive() const { return fRecursive;}
  private:
    bool fRecursive;
    std::string fPath;
    std::vector<std::string>& fVector;
    bool fNames;
    bool fDone;
  };
private:
  Folder fRoot;
  Slash::Store::IFolder* fCurrentFolder;
};

}

#endif
