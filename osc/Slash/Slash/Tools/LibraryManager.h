#ifndef Slash_Tools_LibraryManager_h
#define Slash_Tools_LibraryManager_h

// Inheritance :
#include <Slash/Core/ILibrary.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/ILibraryManager.h>

#include <list>

#include <Slash/Core/ISession.h>

#include <inlib/cast>
#include <inlib/sys/plugin>

namespace Slash {

class Library : public virtual Slash::Core::ILibrary {
public: // Slash::Core::ILibrary :
  virtual std::string name() const {return fName;}

  virtual bool isInitialized() const {return f_plugin ? true : false;}

  virtual Slash::Core::ILibrary::Procedure findProcedure(const std::string& aName,bool aQuiet = false) const {
    if(!f_plugin) return 0;
    return f_plugin->find(aName,aQuiet);
  }


public:
  inline Library(Slash::Core::ISession& aSession,const std::string& aName,const std::string& aLibrary,const std::string& aPackage  ,bool aQuiet = false)
  :fSession(aSession)
  ,fName(aName)
  ,fLibrary(aLibrary)
  ,fPackage(aPackage)
  ,f_plugin(0)
  {
    //Lib::Debug::increment("Lib::Library");
    if(fSession.verboseLevel()) {
      std::ostream& out = fSession.cout();
      out << "Slash::Library::Library :" 
          << " name " << inlib::sout(aName)
          << " library " << inlib::sout(aLibrary)
          << " package " << inlib::sout(aPackage)
          << std::endl;
    }
    if(fLibrary.empty()) return;
    f_plugin = new inlib::plugin
      (fLibrary,fSession.cout(),fSession.verboseLevel()?true:false,aQuiet);
    if(!f_plugin) return;
    if(!f_plugin->is_opened()) {
      delete f_plugin;
      f_plugin = 0;
      return;
    }
    if(fSession.verboseLevel()) {
      std::ostream& out = fSession.cout();
      out << "Slash::Library::Library :" 
          << " load DLL for " 
          << inlib::sout(fLibrary) << " done." << std::endl;
    }
    execute("Initialize");
  }
protected:
  inline Library(const Library& aFrom)
  : Slash::Core::ILibrary()
  ,fSession(aFrom.fSession)
  {}
private:
  inline Library& operator=(const Library& aFrom) {return *this;}

public:
  virtual ~Library(){
    //Lib::Debug::decrement("Lib::Library");
    if(!f_plugin) return;
    execute("Finalize");
    if(fSession.verboseLevel()) {
      std::ostream& out = fSession.cout();
      out << "Slash::Library::~Library :"
          << " \"" << fName << "\" closing DLL..." << std::endl;
    }
    delete f_plugin;
  }

  inline const std::string& package() const {return fPackage;}

  inline bool execute(const std::string& aName) const {
    if(!f_plugin) return false;
    if(fPackage.empty()) return false;
    typedef void(*F)(Slash::Core::ISession&);
    std::string s = fPackage;
    s += aName;
    F func = (F)f_plugin->find(s);
    if(!func) {
      std::ostream& out = fSession.cout();
      out << "Slash::Library::execute :"
          << " Function " << s << " not found" << std::endl;
      return false;
    }
    if(fSession.verboseLevel()) {
      std::ostream& out = fSession.cout();
      out << "Slash::Library::execute :" 
          << " execute \"" << s << "\"..." << std::endl;
    }
    func(fSession);
    if(fSession.verboseLevel()) {
      std::ostream& out = fSession.cout();
      out << "Library::Library::execute :"
          << " execute \"" << s << "\" done." << std::endl;
    }
    return true;
  }
  
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  std::string fLibrary;
  std::string fPackage;
  inlib::plugin* f_plugin;
};

class LibraryManager
:public virtual Slash::Core::IManager 
,public virtual Slash::Core::ILibraryManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::LibraryManager)
    else INLIB_IF_CAST(Slash::Core::ILibraryManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //Slash::Core::ILibraryManager
  virtual Slash::Core::ILibrary* addLibrary(const std::string& aName,const std::string& aLibrary,const std::string& aPackage,bool aQuiet = false){
    if(aName.empty()) return 0;
    Slash::Core::ILibrary* entry = findLibrary(aName);
    if(entry) return entry;
    entry = new Library(fSession,aName,aLibrary,aPackage,aQuiet);
    if(!entry->isInitialized()) {
      delete entry;
      return 0;
    }
    fList.push_back(entry);
    return entry;
  }

  virtual Slash::Core::ILibrary* findLibrary(const std::string& aString) {
    std::list<Slash::Core::ILibrary*>::iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(aString==(*it)->name()) return (*it);
    }
    return 0;
  }

public:
  inline LibraryManager(Slash::Core::ISession& aSession,const std::string& aName)
  :fSession(aSession)
  ,fName(aName)
  {
    //Lib::Debug::increment("Lib::LibraryManager");
  }

  virtual ~LibraryManager() {
    // Delete in reverse order :
    std::list<Slash::Core::ILibrary*>::iterator it;
    while(!fList.empty()) {
      it = fList.end();
      it--;
      Slash::Core::ILibrary* entry  = *it;
      fList.erase(it);
      delete entry;
    }
    //Lib::Debug::decrement("Lib::LibraryManager");
  }

protected:
  inline LibraryManager(const LibraryManager& aFrom)
  :fSession(aFrom.fSession),fName(aFrom.fName){}
private:
  inline LibraryManager& operator=(const LibraryManager& aFrom) {return *this;}

private:
  Slash::Core::ISession& fSession;
  std::string fName;
  std::list<Slash::Core::ILibrary*> fList;
};

}

#endif
