#ifndef BatchLab_BaseObject_h
#define BatchLab_BaseObject_h

// Inheritance :
#include <AIDA/IManagedObject.h>
#include <Slash/Store/IObject.h>

#include <Slash/Core/IRelationManager.h>
#include <Slash/Store/IFolder.h>

#include <Slash/Tools/Relation.h>
#include <Slash/Tools/Mutex.h>

#include <inlib/cast>
#include <Lib/Debug.h>

namespace BatchLab {

class BaseObject 
:public virtual AIDA::IManagedObject 
,public virtual Slash::Store::IObject 
{
public: //AIDA/IManagedObject
  virtual std::string name() const {return fName;}
public: //Slash::Store::IObject
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(AIDA::IManagedObject)
    else INLIB_IF_CAST(Slash::Store::IObject) 
    else INLIB_IF_CAST(BatchLab::BaseObject)
    else return 0;
  }

  virtual void setFolder(Slash::Store::IFolder* aFolder) {fFolder = aFolder;}

  virtual void setName(const std::string& aName) {fName = aName;}

public:
  virtual void emitUpdate() {
    // To update plotters.
    if(fRelationManager) {
      Slash::RV_First_Schedule visitor(this);
      fRelationManager->visit(visitor);
    }
  }
protected:
  BaseObject(std::ostream& a_out,const std::string& aName)
  :f_out(a_out),fName(aName),fFolder(0),fRelationManager(0)
  ,fVerboseLevel(0),fMutex(0){
    Lib::Debug::increment("BatchLab::BaseObject");
  }

  BaseObject(const BaseObject& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,f_out(aFrom.f_out),fName(aFrom.fName),fFolder(0)
  ,fRelationManager(0),fMutex(0){
    Lib::Debug::increment("BatchLab::BaseObject");
  }

public:
  virtual ~BaseObject() {
    if(fMutex) delete fMutex;
    if(fFolder) fFolder->remove(this);
    if(fRelationManager) {    
      Slash::RV_Or_Remove visitor(this,this);
      fRelationManager->remove(visitor,true);
    }
    Lib::Debug::decrement("BatchLab::BaseObject");
  }

//protected:
public: //for XML_DataWriter.
  bool lock() const {
    if(!fMutex) return true; //we consider that thread safety not required.
    return fMutex->lock();
  }
  bool unlock() const {
    if(!fMutex) return true; //we consider that thread safety not required.
    return fMutex->unlock();
  }
public:
  std::ostream& out() const { return f_out;}

  void setRelationManager(Slash::Core::IRelationManager* aRelationManager) {
    fRelationManager = aRelationManager;
  }

  void setVerboseLevel(int aVerboseLevel) {fVerboseLevel = aVerboseLevel;}
  int verboseLevel() const {return fVerboseLevel;}

  bool setThreadSafe(bool a_safe){
    if(a_safe) {
      if(fMutex) return true; //done.
      Slash::Mutex* mutex = new Slash::Mutex();
      if(!mutex) return false;
      if(!mutex->initialize()) {
        f_out << "BatchLab::BaseObject::setThreadSafe :"
            << " failed."
            << std::endl;
        delete mutex;
        return false;
      }
      fMutex = mutex;
      return true;
    } else {
      delete fMutex;
      fMutex = 0;
      return true;
    }
  }

  bool hasMutex() const {return fMutex?true:false;}

  //protected:
private:
  std::ostream& f_out;
private:
  std::string fName;
  Slash::Store::IFolder* fFolder;
  Slash::Core::IRelationManager* fRelationManager;
  int fVerboseLevel;
  Slash::Thread::IMutex* fMutex;
};

}

#endif
