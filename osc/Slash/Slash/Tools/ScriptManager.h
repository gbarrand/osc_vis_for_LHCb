#ifndef Slash_Tools_ScriptManager_h
#define Slash_Tools_ScriptManager_h

// Inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/IScriptManager.h>

#include <Slash/Core/ISession.h>

#include <inlib/cast>
#include <inlib/sys/file>
#include <inlib/touplow>

#include <list>
#include <cstdio>

#include "Interpreter.h"

namespace Slash {

class ScriptManager
:public virtual Slash::Core::IManager 
,public virtual Slash::UI::IScriptManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::UI::IScriptManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //Slash::UI::IScriptManager
  virtual Slash::UI::IInterpreter* addInterpreter(const std::string& aName,const std::string& aLibrary,const std::string& aPackage,const std::string& aSuffix,const std::string& aFileCmd,const std::string& aMendatoryEnvironmentVariable,Slash::UI::IInterpreter::Function aFunction,void* aTag,Slash::UI::IInterpreter::Completer aCompleter = 0,void* aCompleterTag = 0){
    if(aName.empty()) return 0;
    Slash::UI::IInterpreter* interp = 
      new Interpreter(fSession,aName,
                      aLibrary,aPackage,aSuffix,aFileCmd,
                      aMendatoryEnvironmentVariable,
                      aFunction,aTag,aCompleter,aCompleterTag);
    fList.push_back(interp);
    return interp;
  }

  virtual Slash::UI::IInterpreter* findInterpreter(const std::string& aName) const {
    std::list<Slash::UI::IInterpreter*>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if((*it)->name()==aName) return (*it);
    }
    // Exact matching failed ; tolerate case insensitive :
    std::string sa = aName;
    inlib::tolowercase(sa);
    for(it=fList.begin();it!=fList.end();++it) {
      std::string si = (*it)->name();
      inlib::tolowercase(si);
      if(si==sa) return (*it);
    }
    return 0;
  }

  virtual Slash::UI::IInterpreter* findInterpreterFromSuffix(const std::string& aSuffix) const {
    std::list<Slash::UI::IInterpreter*>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if((*it)->suffix()==aSuffix) return (*it);
    }
    return 0;
  }

  virtual bool executeScript(const std::string& aInterp,const std::string& aScript,const Slash::UI::IInterpreter::Options& aParams) {
    if(aScript.empty()) return true;

    //{for(unsigned int index=0;index<aParams.size();index++) {
    //  ::printf("debug : ScriptManager : %d : %s %s\n",index,
    //      aParams[index].first.c_str(),aParams[index].second.c_str());
    //}}
  
    record(aInterp,aScript);
  
    Slash::UI::IInterpreter* interpreter = findInterpreter(aInterp);
    if(!interpreter) {
      std::ostream& out = fSession.cout();
      out << "Slash::ScriptManager::executeScript :"
          << " Unknown interpreter : " << aInterp << std::endl;
      return false;
    }
  
    bool status = interpreter->executeScript(aScript,aParams);
    fSession.flush(); //FIXME : do it here ? Needed only in interactive mode...
    return status;
  }
  
  virtual void recordExecutedScripts(bool aValue) {
    if(aValue) {
      if(fLogFile.size()) return; //done
      if(!inlib::tmpname(".","OnX_",".log",fLogFile)) fLogFile = "OnX.log";
    } else {
      closeLog();
    }
  }

  virtual bool record(const std::string& aInterp,const std::string& aString){
    if(fLogFile.empty()) return true;
    FILE* file = ::fopen(fLogFile.c_str(),"ab");
    if(!file) return false;
    ::fprintf(file,"%s>%s\n",aInterp.c_str(),aString.c_str());
    ::fclose (file);
    return true;
  }

  virtual std::vector<std::string> names() const {
    std::vector<std::string> vs;
    std::list<Slash::UI::IInterpreter*>::const_iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      vs.push_back((*it)->name());
    }
    return vs;
  }

 
  virtual bool removeInterpreter(Slash::UI::IInterpreter*& a_interp) {
    fList.remove(a_interp);
    delete a_interp;
    a_interp = 0;
    return true;
  }

public:
  inline ScriptManager(Slash::Core::ISession& aSession,const std::string& aName)
  :fSession(aSession)
  ,fName(aName)
  {
    //Lib::Debug::increment("OnX::ScriptManager");
  }

  virtual ~ScriptManager(){
    std::list<Slash::UI::IInterpreter*>::iterator it;
    for(it=fList.begin();it!=fList.end();it = fList.erase(it)) delete (*it);
    closeLog();
    //Lib::Debug::decrement("OnX::ScriptManager");
  }

protected:
  inline ScriptManager(const ScriptManager& aFrom)
  :fSession(aFrom.fSession),fName(aFrom.fName){}
private:
  inline ScriptManager& operator=(const ScriptManager& aFrom) {return *this;}
private:
  inline void closeLog(){fLogFile.clear();}
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  std::list<Slash::UI::IInterpreter*> fList;
  std::string fLogFile;
};

}

#endif




