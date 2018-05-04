#ifndef OnXLab_TreeFactory_h
#define OnXLab_TreeFactory_h

// Inheritance :
#include <BatchLab/Core/TreeFactory.h>

#include <Slash/Tools/UI.h>
#include <Lib/Messages.h>

namespace OnXLab {

  class AIDA_ITree_IUI_Relation : public virtual Slash::Core::IRelation {
  public: // IRelation :
    virtual std::string name() const {return fName;}
    virtual void* cast(const std::string& aTo) const {
      if(aTo=="OnXLab::AIDA_ITree_IUI_Relation")  {
        return INLIB_SCAST(OnXLab::AIDA_ITree_IUI_Relation);
      } else if(aTo=="Slash::Core::IRelation") {
        return INLIB_SCAST(Slash::Core::IRelation);
      } else {
        return 0;
      }
    }
    virtual bool schedule() {
      if(fScheduled) return true;
      std::string value;
      if(!fUI.session().parameterValue("OnXLab.tree",value)) {
        std::ostream& out = fUI.session().cout();
        out << "OnXLab::AIDA_ITree_IUI_Relation::schedule :"
            << " session parameter OnXLab.tree not found."
            << std::endl;
        return false;
      }
      if(value.empty()) {
        std::ostream& out = fUI.session().cout();
        out << "OnXLab::AIDA_ITree_IUI_Relation::schedule :"
            << " OnXLab.tree variable is empty."
            << std::endl;
        return false;
      }
      //::printf("debug : OnXLab::TreeUI_Relation::notify : widget \"%s\"\n",
      //    value.c_str());
      if(value!="none") {
        if(!fUI.notify(new Lib::UpdateMessage(value,this))) return false;
      }
      fScheduled = true;
      return true;
    }
    virtual void unSchedule() { 
      fScheduled = false;
    }
    virtual void* first() const { return &fTree;}
    virtual void* second() const { return &fUI;}
  public:
    AIDA_ITree_IUI_Relation(AIDA::ITree& aTree,Slash::UI::IUI& aUI)
    :fName("OnXLab::AIDA_ITree_IUI_Relation")
    ,fScheduled(false)
    ,fTree(aTree),fUI(aUI){
      Lib::Debug::increment("OnXLab::AIDA_ITree_IUI_Relation");
      //printf("debug : OnXLab::AIDA_ITree_IUI_Relation::AIDA_ITree_IUI_Relation : %ld\n",this);
    }
    virtual ~AIDA_ITree_IUI_Relation(){
      //printf("debug : OnXLab::AIDA_ITree_IUI_Relation::~AIDA_ITree_IUI_Relation : %ld\n",this);
      Lib::Debug::decrement("OnXLab::AIDA_ITree_IUI_Relation");
    }
  private:
    std::string fName;
    bool fScheduled;
    AIDA::ITree& fTree;
    Slash::UI::IUI& fUI;
  };

class TreeFactory : public BatchLab::TreeFactory {
public: //AIDA/ITreeFactory
  virtual AIDA::ITree* create(){return create_mem();}

  virtual AIDA::ITree* createTree(const std::string& = ""){ //AIDA-v3r3p0
    return create_mem();
  }

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  virtual AIDA::ITree* createTree(const std::string& aStoreName,
                                  const std::string& aStoreType,
                                  int mode = AIDA::ITreeFactory::AUTO,
                                  const std::string& aOptions = ""){
    AIDA::ITree* tree =  
      BatchLab::TreeFactory::createTree(aStoreName,aStoreType,aMode,aOptions);
    if(!tree) return 0;
    setUIRelation(*tree);
  
    //FIXME : we should be able to pass through AIDA :
    BatchLab::BaseTree* labTree = INLIB_CAST(*tree,BatchLab::BaseTree);
    if(labTree) labTree->emitUpdate();
  
    return tree;
  }
  virtual AIDA::ITree* createNamedTree(const std::string& aName,
                                       const std::string& aStoreName,
                                       const std::string& aStoreType
                                       int mode = AIDA::ITreeFactory::AUTO,
                                       const std::string& aOptions = ""){
    AIDA::ITree* tree =  
      BatchLab::TreeFactory::createNamedTree
        (aName,aStoreName,aStoreType,aMode,aOptions);
    if(!tree) return 0;
    setUIRelation(*tree);
  
    //FIXME : we should be able to pass through AIDA :
    BatchLab::BaseTree* labTree = INLIB_CAST(*tree,BatchLab::BaseTree);
    if(labTree) labTree->emitUpdate();
  
    return tree;
  }
#endif

  virtual AIDA::ITree* create(const std::string& aStoreName,
                              const std::string& aStoreType = "",
                              bool aReadOnly = false,bool aCreateNew = false,
                              const std::string& aOptions = ""){
    AIDA::ITree* tree =  
      BatchLab::TreeFactory::create
        (aStoreName,aStoreType,aReadOnly,aCreateNew,aOptions);
    if(!tree) return 0;
    setUIRelation(*tree);
  
    //FIXME : we should be able to pass through AIDA :
    BatchLab::BaseTree* labTree = INLIB_CAST(*tree,BatchLab::BaseTree);
    if(labTree) labTree->emitUpdate();
  
    return tree;
  }

public:
  TreeFactory(Slash::Core::ISession& aSession)
  :BatchLab::TreeFactory(aSession){
    Lib::Debug::increment("OnXLab::TreeFactory");
  }

  virtual ~TreeFactory(){
    Lib::Debug::decrement("OnXLab::TreeFactory");
  }
private:
  AIDA::ITree* create_mem(){
    AIDA::ITree* tree =  BatchLab::TreeFactory::create();
    if(!tree) return 0;
    setUIRelation(*tree);
    /*
    AIDA::ITree* memoryTree = 
      Slash_findManager(fSession,"MemoryTree",AIDA::ITree);
    if(memoryTree) return memoryTree;
    BatchLab::MemoryTree* tree = 
      new BatchLab::MemoryTree(fSession,"MemoryTree");
    tree->setVerboseLevel(fSession.verboseLevel());
    fSession.addManager(tree);
    //WARNING : the UI relation is done once for the mem tree,
    //          then the UI should be here at first call.
    */
    return tree;    
  }

  void setUIRelation(AIDA::ITree& aTree){
    Slash::Core::IRelationManager* mgr = Slash::relationManager(fSession);
    if(!mgr) {
      //std::ostream& out = fSession.cout();
      //out << "OnXLab::TreeFactory::setUIRelation :"
      //    << " RelationManager not found."
      //    << std::endl;
      return;
    }
    Slash::UI::IUI* ui = Slash::find_UI(fSession);
    if(!ui) {
      std::ostream& out = fSession.cout();
      //out << "OnXLab::TreeFactory::setUIRelation :"
      //    << " UI not found."
      //    << std::endl;
      return;
    }
    mgr->add(new OnXLab::AIDA_ITree_IUI_Relation(aTree,*ui));
    //FIXME : aTree emitUpdate about its creation ?
  }
};

}

#endif
