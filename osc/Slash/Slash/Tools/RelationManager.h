#ifndef Slash_Tools_RelationManager_h
#define Slash_Tools_RelationManager_h

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Core/IRelationManager.h>

#include <Slash/Core/IRelation.h>
#include <Slash/Core/IRelationVisitor.h>

#include <list>

#include <inlib/cast>

namespace Slash {namespace Core {class ISession;}}

namespace Slash {

class RelationManager
:public virtual Slash::Core::IManager 
,public virtual Slash::Core::IRelationManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Core::IRelationManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public: //IRelationManager
  virtual void add(Slash::Core::IRelation* aRelation){
    //look if a similar relation already here :
    std::list<Slash::Core::IRelation*>::iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if( (aRelation->first()==(*it)->first()) &&
          (aRelation->second()==(*it)->second()) &&
          (aRelation->name()==(*it)->name()) //done last to optimize
        ) {
        delete aRelation;
        return; //done.
      }
    }
    fList.push_back(aRelation);
  }

  virtual void visit(Slash::Core::IRelationVisitor& aVisitor){
    std::list<Slash::Core::IRelation*>::iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(!aVisitor.visit(*(*it))) return;
    }
  }

  virtual bool isValid(const Slash::Core::IRelation& aRelation) const {
    std::list<Slash::Core::IRelation*>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if((*it)==&(aRelation)) return true;
    }
    return false;
  }

  virtual void remove(Slash::Core::IRelationVisitor& aVisitor,bool aSchedule = false){
    std::list<Slash::Core::IRelation*> rels;
    std::list<Slash::Core::IRelation*>::iterator it;
    for(it=fList.begin();it!=fList.end();) {
      Slash::Core::IRelation* relation = *it;
      if(aVisitor.visit(*relation)) {
        it = fList.erase(it);
        rels.push_back(relation);
      } else {
        ++it;
      }
    }
    for(it=rels.begin();it!=rels.end();it++) {
      Slash::Core::IRelation* relation = *it;
      if(aSchedule) relation->schedule();
      delete relation;
    }
  }

  virtual void removeWith(void* aObject){ 
    RelationVisitor visitor(aObject);
    remove(visitor);
  }

public:
  inline RelationManager(const std::string& aName,Slash::Core::ISession& aSession)
  :fSession(aSession)
  ,fName(aName)
  {
    //Lib::Debug::increment("Lib::RelationManager");
  }

  virtual ~RelationManager(){
    std::list<Slash::Core::IRelation*>::iterator it;
    while(!fList.empty()) {
      it = fList.begin();
      Slash::Core::IRelation* entry  = *it;
      fList.erase(it);
      delete entry;    
    }
    //Lib::Debug::decrement("Lib::RelationManager");
  }
protected:
  inline RelationManager(const RelationManager& aFrom)
  :fSession(aFrom.fSession),fName(aFrom.fName){}
private:
  inline RelationManager& operator=(const RelationManager& aFrom) {return *this;}

public:
  class RelationVisitor : public virtual Slash::Core::IRelationVisitor {
  public: //IRelationVisitor
    virtual bool visit(Slash::Core::IRelation& aRelation) {
      if(fObject==aRelation.first()) return true; //remove
      if(fObject==aRelation.second()) return true; //remove
      return false;
    }
  public:
    RelationVisitor(void* aObject):fObject(aObject){}
    virtual ~RelationVisitor(){}
  private:
    void* fObject;
  };
  
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  std::list<Slash::Core::IRelation*> fList;
};

}

#endif




