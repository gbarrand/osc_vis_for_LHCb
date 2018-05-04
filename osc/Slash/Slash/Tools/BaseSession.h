#ifndef Slash_Tools_BaseSession_h
#define Slash_Tools_BaseSession_h

// Inheritance :
#include <Slash/Core/ISession.h>

#include <Slash/Core/IManager.h>

//#include <Lib/Debug.h>

#include <inlib/sout>
#include <inlib/cast>
#include <inlib/params>

#include "StreamBuffer.h"

namespace Slash {

class BaseSession : public virtual Slash::Core::ISession {
public: //Slash::Core::ISession
  virtual void* cast(const std::string& a_class) const {
    // inheritance :
    INLIB_IF_CAST(Slash::Core::ISession)
    // this :
    else INLIB_IF_CAST(Slash::BaseSession)
    else return 0;
  }

  virtual void addManager(Slash::Core::IManager* aManager){
    if(findManager(aManager->name())) {
      f_out << "Slash::BaseSession::addManager :"
            << " WARNING : a manager with name "
            << inlib::sout(aManager->name()) << " already exists." 
            << std::endl;
    }
    if(fVerboseLevel) {
      f_out << "Slash::BaseSession::addManager :"
            << " " << inlib::sout(aManager->name()) << "." 
            << std::endl;
    }
    fManagers.push_back(aManager);
  }

  virtual void removeManager(Slash::Core::IManager* aManager){
    if(fVerboseLevel) {
      f_out << "Slash::BaseSession::removeManager :"
            << " " << inlib::sout(aManager->name()) << "." 
            << std::endl;
    }
    std::vector<Slash::Core::IManager*>::iterator it;
    for(it=fManagers.begin();it!=fManagers.end();) {
      if((*it)==aManager) {
        it = fManagers.erase(it);
      } else {
        ++it;
      }
    }
  }

  virtual Slash::Core::IManager* findManager(const std::string& aString,bool aForward = true) const {
    if(aForward) {
      std::vector<Slash::Core::IManager*>::const_iterator it;
      for(it=fManagers.begin();it!=fManagers.end();++it) {
        if(aString==(*it)->name()) return *it;
      }
    } else {
      std::vector<Slash::Core::IManager*>::const_reverse_iterator it;
      for(it=fManagers.rbegin();it!=fManagers.rend();++it) {
        if(aString==(*it)->name()) return *it;
      }
    }
    return 0;
  }

  virtual void destroyManager(const std::string& aName){
    std::vector<Slash::Core::IManager*>::iterator it;
    for(it=fManagers.begin();it!=fManagers.end();) {
      Slash::Core::IManager* manager = *it;
      if(manager->name()==aName) {
        it = fManagers.erase(it);
        if(fVerboseLevel) {
          f_out << "Slash::BaseSession::destroyManager :"
                << " " << inlib::sout(aName) << "." 
                << std::endl;
        }
        delete manager;
      } else {
        ++it;
      }
    }
  }

  virtual unsigned int numberOfManagers() const{return fManagers.size();}

  virtual const Slash::Core::IManager* manager(unsigned int aIndex) const {
    return fManagers[aIndex];
  }

  virtual std::vector<std::string> managerNames() const{
    std::vector<std::string> names;
    std::vector<Slash::Core::IManager*>::const_iterator it;
    for(it=fManagers.begin();it!=fManagers.end();++it) {
      names.push_back((*it)->name());
    }
    return names;
  }

  virtual Slash::Core::IWriter& printer(){return fPrinter;}
  virtual std::ostream& out(){return f_out;}
  virtual std::ostream& cout(){return f_out;}

  virtual void setVerboseLevel(int aVerboseLevel){
    fVerboseLevel = aVerboseLevel;
  }
  virtual int verboseLevel() const{return fVerboseLevel;}

  virtual bool setParameter(const std::string& aKey,
                            const std::string& aValue){
    if(aKey.empty()) return false;
    if(fParameters.is_key(aKey)) { 
      fParameters.set_value(aKey,aValue);
    } else {
      fParameters.add(0,aKey,"",aValue,"","");
    }
    return true;
  }

  virtual bool parameterValue(const std::string& aKey,std::string& aValue){
    if(fParameters.is_key(aKey)) {
      aValue = fParameters.value(aKey);
      return true;
    } else {
      aValue = "";
      return false;
    }
  }

  virtual bool removeParameter(const std::string& aKey){
    fParameters.remove(aKey);
    return true;
  }

  virtual std::vector<std::string> availableParameters() const {
    return fParameters.keys();
  }

  virtual std::vector< std::pair<std::string,std::string> > arguments() const {
    return fArguments;
  }

  // no implementation :
  virtual bool load(const std::string&,const std::string&){return false;}
  virtual void redirectOutput(bool){}
  virtual bool flush(){return false;}

public:
  inline BaseSession(Slash::Core::IWriter& aPrinter)
  :fPrinter(aPrinter)
  ,fStreamBuffer(aPrinter)
  ,f_out(&fStreamBuffer)
  ,fVerboseLevel(0)
  ,fParameters("","")
  {
  }

  virtual ~BaseSession(){
    rm_managers();
  }

protected:
  inline BaseSession(const BaseSession& aFrom)
  :fPrinter(aFrom.fPrinter)
  ,fStreamBuffer(aFrom.fPrinter)
  ,f_out(&fStreamBuffer)
  ,fParameters("","")
  {}
private:
  inline BaseSession& operator=(const BaseSession&){return *this;}
public:
  void setArguments(const std::vector< std::pair<std::string,std::string> >& aArgs){fArguments = aArgs;}

  Slash::StreamBuffer& streamBuffer(){return fStreamBuffer;}

protected:
  inline bool rm_managers(){
    std::vector<Slash::Core::IManager*>::iterator it;
    while(!fManagers.empty()) {
      it = fManagers.end();
      it--;
      if(fVerboseLevel) {
        f_out << "Slash::BaseSession::~Session :"
               << " delete manager " << inlib::sout((*it)->name()) 
               << "." << std::endl;
      }
      Slash::Core::IManager* manager = *it;
      fManagers.erase(it); //WARNING : must be done before the below.
      delete manager; //WARNING : some ~manager may rm themself from the list.
    }
    return true;
  }
private:
  Slash::Core::IWriter& fPrinter;
  Slash::StreamBuffer fStreamBuffer;
protected:
  std::ostream f_out;
  int fVerboseLevel;
private:
  std::vector<Slash::Core::IManager*> fManagers;
  std::vector< std::pair<std::string,std::string> > fArguments;
  inlib::params<std::string,std::string> fParameters;
};

}

#endif

