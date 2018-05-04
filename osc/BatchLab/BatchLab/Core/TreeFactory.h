#ifndef BatchLab_TreeFactory_h
#define BatchLab_TreeFactory_h

#include <AIDA/ITreeFactory.h>
#include <AIDA/IConstants.h> //For AIDA_VERSION
#include <AIDA/ITree.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Store/IStoreManager.h>

#include <BatchLab/Core/MemoryTree.h>
#include <BatchLab/Core/AsciiTree.h>

#include <inlib/cast>

#include <Lib/Debug.h>

namespace BatchLab {

class TreeFactory : public virtual AIDA::ITreeFactory {
public: //AIDA/ITreeFactory
  virtual AIDA::ITree* create(){return BatchLab::TreeFactory::createTree("");}

  virtual AIDA::ITree* createTree(const std::string& = ""){ //AIDA-v3r3p0
    AIDA::ITree* memoryTree = 
      Slash_findManager(fSession,"MemoryTree",AIDA::ITree);
    if(memoryTree) return memoryTree;
    MemoryTree* tree = new MemoryTree(fSession,"MemoryTree");
    tree->setVerboseLevel(fSession.verboseLevel());
    fSession.addManager(tree);
    return tree;
  }

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  virtual AIDA::ITree* createNamedTree(const std::string& name,
                                       const std::string& storeName,
                                       const std::string& storeType,
                                       int mode = AIDA::ITreeFactory::AUTO,
                                       const std::string& options = "");
  virtual AIDA::ITree* createTree(const std::string& aStoreName,
                                  const std::string& aStoreType,
                                  int aMode = AIDA::ITreeFactory::AUTO,
                                  const std::string& aOptions = "") {
    return BatchLab::TreeFactory::createNamedTree
             (aStoreName,aStoreName,aStoreType,aMode,aOptions);
  }
  virtual AIDA::ITree* createNamedTree(const std::string& aName,
                                       const std::string& aStoreName,
                                       const std::string& aStoreType,
                                       int aMode = AIDA::ITreeFactory::AUTO,
                                       const std::string& aOptions = "") {
    if(aMode==AIDA::ITreeFactory::AUTO) {
      bool readOnly = false;
      bool createNew = true;
      return 
        create_tree(aName,aStoreName,aStoreType,readOnly,createNew,aOptions);
    } else if(aMode==AIDA::ITreeFactory::CREATE) {
      bool readOnly = false;
      bool createNew = true;
      return 
        create_tree(aName,aStoreName,aStoreType,readOnly,createNew,aOptions);
    } else if(aMode==AIDA::ITreeFactory::RECREATE) {
      bool readOnly = false;
      bool createNew = true;
      return 
        create_tree(aName,aStoreName,aStoreType,readOnly,createNew,aOptions);
    } else if(aMode==AIDA::ITreeFactory::READONLY) {
      bool readOnly = true;
      bool createNew = false;
      return 
        create_tree(aName,aStoreName,aStoreType,readOnly,createNew,aOptions);
    } else if(aMode==AIDA::ITreeFactory::UPDATE) {
      bool readOnly = false;
      bool createNew = false;
      return 
        create_tree(aName,aStoreName,aStoreType,readOnly,createNew,aOptions);
    } else {
      return 0;
    }
  }
#endif

  virtual AIDA::ITree* create(const std::string& aStoreName,
                              const std::string& aStoreType = "",
                              bool aReadOnly = false,
                              bool aCreateNew = false,
                              const std::string& aOptions = "") {
    return create_tree(aStoreName,
                       aStoreName,aStoreType,aReadOnly,aCreateNew,aOptions);
  }
public:
  TreeFactory(Slash::Core::ISession& aSession)
  :fSession(aSession),f_out(aSession.cout()){
    Lib::Debug::increment("BatchLab::TreeFactory");
  }
  virtual ~TreeFactory(){
    Lib::Debug::decrement("BatchLab::TreeFactory");
  }
protected:
  AIDA::ITree* create_tree(const std::string& aName,
                           const std::string& aStoreName,
                           const std::string& aStoreType = "",
                           bool aReadOnly = false,
                           bool aCreateNew = false,
                           const std::string& aOptions = "") {
    std::string storeType = aStoreType;
    inlib::tolowercase(storeType);
  
    if(storeType=="memory") {
      //FIXME : should protect against the creation of two "MemoryTree".
      // Memory tree :
      MemoryTree* tree = new MemoryTree(fSession,aName);
      tree->setVerboseLevel(fSession.verboseLevel());
      fSession.addManager(tree);
      return tree;
    } else if(storeType=="ascii") {
      AsciiTree* tree = 
        new AsciiTree(fSession,aName,aStoreName,aReadOnly,aCreateNew,aOptions);
      tree->setVerboseLevel(fSession.verboseLevel());
      fSession.addManager(tree);
      return tree;
    } else if( (storeType=="hippodraw") ||
               (storeType=="tnt") ) {
      AsciiTree* tree = 
        new AsciiTree(fSession,aName,aStoreName,aReadOnly,aCreateNew,aOptions);
      tree->setVerboseLevel(fSession.verboseLevel());
      tree->setHippoStyle(true);
      fSession.addManager(tree);
      return tree;
    } else if(storeType=="mysql") {
      return loadTree("MySQL","BatchLabMySQL","BatchLabMySQL",
                      "MySQLManager",aStoreName,aReadOnly,aCreateNew,aOptions);
//  } else if(storeType=="rio") {
//    return loadTree("Rio","BatchLabRio","BatchLabRio",
//                    "Rio_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
    } else if(storeType=="root") {
      return loadTree("Rio","BatchLabRio","BatchLabRio",
                      "Rio_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
    } else if( (storeType.empty())     ||
               (storeType=="xml")  || //Supported by SLAC.
               (storeType=="aida") ){
      return loadTree("XML","BatchLabXML","BatchLabXML",
                      "XML_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
    } else if(storeType=="zip") {
      return loadTree("Zip","BatchLabZip","BatchLabZip",
                      "Zip_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
    } else if(storeType=="fs") {
      return loadTree("FS","BatchLabFS","BatchLabFS",
                      "FS_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
    } else if(storeType=="hdf5") {
      return loadTree("HDF5","BatchLabHDF5","BatchLabHDF5",
                      "HDF5_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
    } else if((storeType=="hbook")||(storeType=="zebra")) {
      AIDA::ITree* tree = 
        loadTree("Zebra","BatchLabZebra","BatchLabZebra",
                 "Zebra_Manager",aStoreName,aReadOnly,aCreateNew,aOptions);
      if(tree) {
        return tree;
      } else if(aCreateNew) {
        f_out << "BatchLab::TreeFactory::create_tree :"
            << " can't create " << inlib::sout(aStoreName) << "." 
            << std::endl;
        f_out << " This is probably because the BatchLab Zebra"
            << " driver had not been reconstructed on this installation." 
            << std::endl;
        std::string path,name,suffix;
        inlib::path_name_suffix(aStoreName,path,name,suffix);
        std::string xmlName = path+inlib::nosuffix(name)+".aida";
        f_out << " We try to proceed by creating"
              << " the AIDA compressed xml file " 
              << inlib::sout(xmlName) << "..." << std::endl;
        return loadTree("XML","BatchLabXML","BatchLabXML",
                 "XML_Manager",xmlName,aReadOnly,aCreateNew,"compress=yes");
      } else {
        f_out << "BatchLab::TreeFactory::create_tree :"
            << " failed to create zebra tree "
            << inlib::sout(aStoreName) << "." 
            << std::endl;
        return 0;
      }
    }
  
    f_out << "BatchLab::TreeFactory::create_tree :"
        << " unknown type \"" << aStoreType << "\"." << std::endl;
    return 0;
  }
private:
  AIDA::ITree* loadTree(const std::string& aLibrary,
                        const std::string& aPath,
                        const std::string& aPackage,
                        const std::string& aStoreManager,
                        const std::string& aName,
                        bool aReadOnly,bool aCreateNew,
                        const std::string& aOptions){
    Slash::Core::ILibraryManager* libraryManager = 
      Slash::libraryManager(fSession);
    if(!libraryManager) {
      f_out << "BatchLab::TreeFactory::loadTree :"
          << " LibraryManager not found." << std::endl;
      return 0;
    }
    Slash::Core::ILibrary* dll = 
      libraryManager->addLibrary(aLibrary,aPath,aPackage);
    if(!dll) {
      f_out << "BatchLab::TreeFactory::loadTree :"
          << " unable to add a DLL" << std::endl;
      return 0;
    }
    Slash::Store::IStoreManager* storeManager = 
      Slash_findManager(fSession,aStoreManager,Slash::Store::IStoreManager);
    if(!storeManager) {
      f_out << "BatchLab::TreeFactory::loadTree :"
          << " unable to find " << aStoreManager << " store manager." 
          << std::endl;
      return 0;
    }
    Slash::Store::ITree* baseTree = 
      storeManager->createTree(aName,aReadOnly,aCreateNew,aOptions);
    if(!baseTree) {
      f_out << "BatchLab::TreeFactory::loadTree :"
          << " can't create tree " << inlib::sout(aName) << "."
          << std::endl;
      return 0;
    }
    AIDA::ITree* aidaTree = INLIB_CAST(*baseTree,AIDA::ITree);
    if(!aidaTree) {
      f_out << "BatchLab::TreeFactory::loadTree :"
          << " tree " << inlib::sout(aName) << " is not an AIDA::ITree."
          << std::endl;
      return 0;
    }
    return aidaTree;
  }
protected:
  Slash::Core::ISession& fSession;
  std::ostream& f_out;
};

}

#endif
