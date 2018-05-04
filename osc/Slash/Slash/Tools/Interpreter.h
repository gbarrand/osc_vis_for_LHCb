#ifndef Slash_Tools_Interpreter_h
#define Slash_Tools_Interpreter_h

// Inheritance :
#include <Slash/UI/IInterpreter.h>

#include <Slash/Core/ISession.h>

#include "Managers.h"

#include <inlib/system>

namespace Slash {

class Interpreter : public virtual Slash::UI::IInterpreter {
public: //Slash::UI::IInterpreter
  virtual std::string name() const {return fName;}
  virtual std::string package() const {return fPackage;}
  virtual std::string suffix() const {return fSuffix;}
  virtual std::string execFileCommand() const {return fExecFileCmd;}

  virtual void setFunction(Slash::UI::IInterpreter::Function aFunction,void* aTag){
    if(!aFunction) return;
    fFunction = aFunction;
    fTag = aTag;
  }

  virtual Slash::UI::IInterpreter::Function function(void*& aTag) const {
    aTag = fTag;
    return fFunction;
  }

  virtual bool load(bool aQuiet = false) {
    if(fLoaded) return true;
    if(fLibrary.size()) {
      Slash::Core::ILibraryManager* lm = Slash::libraryManager(fSession);
      if(!lm) return false;
      if(!lm->addLibrary(fName,fLibrary,fPackage,aQuiet)) return false;
    }
    fLoaded = true;
    return true;
  }


  virtual bool addAlias(const std::string& aAlias,const std::string& aValue){
    //FIXME : should check that it is already here :
    fAliases.push_back(std::pair<std::string,std::string>(aAlias,aValue));
    return true;
  }

  virtual void setCompleter(Completer aFunc,void* aTag) {
    fCompleter = aFunc;
    fCompleterTag = aTag;
  }

  virtual Slash::UI::IInterpreter::Completer completer(void*& aTag) const {
    aTag = fCompleterTag;
    return fCompleter;
  }

  virtual bool complete(const std::string& aString,std::string& aValue) {
    if(!fCompleter) {
      std::ostream& out = fSession.cout();
      out << "Slash::Interpreter::complete :"
          << " interpreter " << inlib::sout(fName) 
          << " has no completion." 
          << std::endl;
      return false;
    }
    return fCompleter(aString,aValue,fCompleterTag);
  }

  virtual bool isOperational(){
    if(fOperationalChecked) return fOperationalStatus;  
    fOperationalChecked = true;
    fOperationalStatus = false;
    if(fMendatoryEnvironmentVariable.size()) {
      bool status = inlib::isenv(fMendatoryEnvironmentVariable);
      if(fSession.verboseLevel()) {
        std::ostream& out = fSession.cout();
        out << "Slash::Interpreter::isOperational :"
            << " mendatoryEnvironmentVariable " 
            << inlib::sout(fMendatoryEnvironmentVariable) 
            << " defined : " << status
            << std::endl;
      }
      if(!status) return false;
    }
    if(!load(true)) return false;
    if(!fFunction) return false;
    fOperationalStatus = true;
    return true;
  }

  virtual bool executeScript(const std::string& aScript,const Slash::UI::IInterpreter::Options& aParams){
    if(aScript.empty()) return true;
    if(!load()) {
      std::ostream& out = fSession.cout();
      out << "Slash::Interpreter::executeScript :"
          << " interpreter " << inlib::sout(fName) 
          << " loading failed." << std::endl;
      out << aScript << std::endl;
      return false;
    } else {
      if(fFunction) {
        if(fSession.verboseLevel()) {
          std::ostream& out = fSession.cout();
          out << "Slash::Interpreter::executeScript :"
              << " with interpreter " << inlib::sout(fName) 
              << " : " << std::endl;
          out << aScript << std::endl;
        }
        return fFunction(aScript,aParams,fAliases,fTag);
      } else {
        std::ostream& out = fSession.cout();
        out << "Slash::Interpreter::executeScript :"
            << " Interpreter (" << fName << ") " 
            << "native function not found for script :" << std::endl;
        out << aScript << std::endl;
        return false;
      }
    }
  
    //FIXME fSession.flush(); do it here ?
  
    return true;
  }

public:

  inline Interpreter(Slash::Core::ISession& aSession,const std::string& aName,const std::string& aLibrary,const std::string& aPackage,const std::string& aSuffix,const std::string& aExecFileCmd,const std::string& aMendatoryEnvironmentVariable,Slash::UI::IInterpreter::Function aFunction,void* aTag,Slash::UI::IInterpreter::Completer aCompleter = 0,void* aCompleterTag = 0)
  :fName(aName)
  ,fLibrary(aLibrary)
  ,fPackage(aPackage)
  ,fSuffix(aSuffix)
  ,fExecFileCmd(aExecFileCmd)
  ,fMendatoryEnvironmentVariable(aMendatoryEnvironmentVariable)
  ,fLoaded(false)
  ,fFunction(aFunction)
  ,fTag(aTag)
  ,fCompleter(aCompleter)
  ,fCompleterTag(aCompleterTag)
  ,fSession(aSession)
  ,fOperationalChecked(false)
  ,fOperationalStatus(false)
  {}

  virtual ~Interpreter(){}

protected:
  inline Interpreter(const Interpreter& aFrom):fSession(aFrom.fSession){}
private:
  inline Interpreter& operator=(const Interpreter& aFrom) {return *this;}
private:
  std::string fName;
  std::string fLibrary;
  std::string fPackage;
  std::string fSuffix;
  std::string fExecFileCmd;
  std::string fMendatoryEnvironmentVariable;
  bool fLoaded;
  Slash::UI::IInterpreter::Function fFunction;
  void* fTag;
  Slash::UI::IInterpreter::Completer fCompleter;
  void* fCompleterTag;
  Slash::Core::ISession& fSession;
  bool fOperationalChecked;
  bool fOperationalStatus;
  std::vector< std::pair<std::string,std::string> > fAliases;
};

}

#endif




