#ifndef BatchLab_BaseTree_h
#define BatchLab_BaseTree_h

// Inheritance :
#include <AIDA/ITree.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/IHierarchy.h>
#include <Slash/Store/ITree.h>

#include <Slash/Core/IRelationVisitor.h>
#include <Slash/Core/IRelation.h>
#include <Slash/Store/IObject.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Relation.h>

#include <inlib/cast>

#include <Lib/Debug.h>

#include <list>

//namespace Slash {namespace Core {class ISession;}}
//namespace Slash {namespace Core {class IRelationManager;}} 

namespace BatchLab {

class BaseTree 
:public virtual AIDA::ITree
,public virtual Slash::Store::ITree
,public virtual Slash::Core::IManager
,public virtual Slash::Core::IHierarchy {
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(AIDA::ITree)
    else INLIB_IF_CAST(Slash::Store::ITree) 
    else INLIB_IF_CAST(BatchLab::BaseTree) 
    else INLIB_IF_CAST(Slash::Core::IManager)
    else INLIB_IF_CAST(Slash::Core::IHierarchy)
    else return 0;
  }

public: //Slash::Store::ITree
  virtual bool manageObject(Slash::Store::IObject* aObject,
                            const std::string& aPath) {
    return addObject(aObject,aPath);
  }
  virtual void emitUpdate() {
    // To update UI.
    if(!fRelationManager) return;
    Slash::RV_First_Schedule visitor(this);
    fRelationManager->visit(visitor);
  }
public:
  BaseTree(Slash::Core::ISession& aSession,const std::string& aName)
  :fSession(aSession) //For Validers
  ,fName(aName)
  ,fVerboseLevel(0)
  ,fRelationManager(0) {
    Lib::Debug::increment("BatchLab::BaseTree");
    //printf("debug : %lu %lu :%lu\n",this,(AIDA::ITree*)this,&fKey);

    fRelationManager = Slash::relationManager(fSession);

    //NOTE : Relation to UI done in TreeFactory to avoid a dependency to IUI.
    //       Can't emitUpdate here since the construction is not finished.
  }


  virtual ~BaseTree() {
    if(fVerboseLevel) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::BaseTree::~Tree :"
          << " called for " << this << " " << fName 
          << std::endl;
    }
  
    // Notify UI.
    //emitUpdate(); //virtual !
    if(fRelationManager) {
      Slash::RV_First_Schedule visitor(this);
      fRelationManager->visit(visitor);
      // Remove relations concerning this tree :
      fRelationManager->removeWith(static_cast<AIDA::ITree*>(this));
  
      //NOTE : if, when visiting, the relation does a schedule
      //       that indirectly uses this object with some method
      //       of a super class, we are dead. Then we should do a :
      //         RV_Or_Remove visitor(this,this);
      //         fRelationManager->remove(visitor,true);
      //       as in ~BaseObject.
    }
  
    fSession.removeManager(this);  //To support the AIDA delete tree.
    Lib::Debug::decrement("BatchLab::BaseTree");
  }

public:
  virtual void setVerboseLevel(int aVerboseLevel) {
    fVerboseLevel = aVerboseLevel;
  }
  int verboseLevel() const {return fVerboseLevel;}
  std::ostream& out() const {return fSession.cout();}
public:
  virtual Slash::Core::ISession& session() const {return fSession;}
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  int fVerboseLevel;
protected:
  Slash::Core::IRelationManager* fRelationManager;
};

}

#endif
