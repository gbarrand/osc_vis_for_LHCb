#ifndef Slash_Tools_OpaqueManager_h
#define Slash_Tools_OpaqueManager_h

#include <list>

#include <Slash/Core/IManager.h>
#include <Slash/Core/IOpaqueManager.h>

#include <inlib/cast>

namespace Slash {

class OpaqueManager;

class OpaqueProxy {
public:
  inline OpaqueProxy(OpaqueManager& aManager,void* aObject,const std::string& aID,bool aOwner)
  :fManager(aManager)
  ,fObject(aObject)
  ,fID(aID)
  ,fOwner(aOwner) 
  {}
  virtual ~OpaqueProxy(){}
public:
  inline OpaqueProxy(const OpaqueProxy& aFrom)
  :fManager(aFrom.fManager)
  ,fObject(aFrom.fObject)
  ,fID(aFrom.fID)
  ,fOwner(aFrom.fOwner) 
  {}
  inline OpaqueProxy& operator=(const OpaqueProxy& aFrom) {
    fObject = aFrom.fObject;
    fID = aFrom.fID;
    fOwner = aFrom.fOwner;
    return *this;
  }
public:
  inline bool isOwner() const {return fOwner;}
  inline void* object() const {return fObject;}
  inline const std::string& identifier() const {return fID;}
private:
  OpaqueManager& fManager;
  void* fObject;
  std::string fID;
  bool fOwner;
};

inline bool operator==(const OpaqueProxy& aA,const OpaqueProxy& aB) {
  if(aA.object()!=aB.object()) return false;
  if(aA.identifier()!=aB.identifier()) return false;
  if(aA.isOwner()!=aB.isOwner()) return false;
  return true;
}

class OpaqueManager 
:public virtual Slash::Core::IManager
,public virtual Slash::Core::IOpaqueManager {
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Core::IManager)
    else INLIB_IF_CAST(Slash::Core::IOpaqueManager)
    else return 0;
  }
public: //IObjectManager
  virtual void* find(const std::string& aName) const {
    std::list<OpaqueProxy>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(aName==(*it).identifier()) return (*it).object();
    }
    return 0;
  }
  virtual bool add(void* aObject,const std::string& aName,bool aOwner = false) {
    fList.push_back(OpaqueProxy(*this,aObject,aName,aOwner));
    return true;
  }
  virtual bool isOwner(const std::string& aName) const {
    std::list<OpaqueProxy>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(aName==(*it).identifier()) return (*it).isOwner();
    }
    return false;
  }
  virtual bool destroy(const std::string& aName) {
    std::list<OpaqueProxy>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(aName==(*it).identifier()) {
        fList.remove(*it);
        return true;
      }
    }
    return false;
  }
public:
  inline OpaqueManager(const std::string& aName):fName(aName) {}
  virtual ~OpaqueManager() {}
protected:
  inline OpaqueManager(const OpaqueManager& aFrom)
  :Slash::Core::IManager()
  ,Slash::Core::IOpaqueManager()
  {}
private:
  inline OpaqueManager& operator=(const OpaqueManager& aFrom){return *this;}
private:
  std::list<OpaqueProxy> fList;
  std::string fName;
};

}

#endif
