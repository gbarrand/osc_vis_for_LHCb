#ifndef BatchLab_FunctionCatalog_h
#define BatchLab_FunctionCatalog_h

// Inheritance :
#include <AIDA/IFunctionCatalog.h>

#include <Slash/Tools/Managers.h>
#include <BatchLab/Core/Environment.h>

#include <inlib/path>

namespace BatchLab {

class FunctionCatalog : public virtual AIDA::IFunctionCatalog {
public: //AIDA/IFunctionCatalog
  virtual bool add(std::string,AIDA::IFunction*){return false;}
  virtual bool add(std::string,const std::string&){return false;}

  virtual std::vector<std::string> list(){return std::vector<std::string>();}
  virtual void remove(const std::string&){}
  virtual bool storeAll(const std::string&){return false;}

  virtual bool loadAll(const std::string& aFile){
    Slash::Data::IFunctionManager* functionManager = 
      Slash::functionManager(fSession);
    if(!functionManager) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadAll :"
          << " FunctionManager not found."
          << std::endl;
      return false;
    }
    return loadFile(*functionManager,aFile);
  }
public:
  FunctionCatalog(Slash::Core::ISession& aSession) :fSession(aSession){}
  virtual ~FunctionCatalog(){}
public:
  Slash::Data::IFunction* find(const std::string& aFile,
                               const std::string& aModel){
    Slash::Data::IFunctionManager* functionManager = 
      Slash::functionManager(fSession);
    if(!functionManager) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadAll :"
          << " FunctionManager not found."
          << std::endl;
      return 0;
    }
    // Already loaded ?
    Slash::Data::IFunction* func = functionManager->findFunction(aModel);
    if(!func) {
      std::string s = aModel;
      inlib::touppercase(s);
      func = functionManager->findFunction(s);
    }
    if(!func && (aFile.size())) { //Not found. Load the file :
      if(!loadFile(*functionManager,aFile)) return 0;
      func = functionManager->findFunction(aModel);
      if(!func) {
        std::string s = aModel;
        inlib::touppercase(s);
        func = functionManager->findFunction(s);
      }
    }
    return func;
  }
private:
  bool loadFile(Slash::Data::IFunctionManager& aFunctionManager,
                const std::string& aFile){
   {//std::vector< std::pair<std::string,std::string> > args = 
    //  fSession.arguments();
    //std::string arg0(args.size()?args[0].first:"");
    if(!set_env(fSession.cout())) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << " BatchLab::set_env failed."
          << std::endl;
      return false;
    }}
  
    if(!inlib::isenv("BATCHLABROOT")) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << "Environment variable BATCHLABROOT not defined."
          << std::endl;
      return false;
    }
    if(!inlib::isenv("LIBROOT")) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << "Environment variable LIBROOT not defined."
          << std::endl;
      return false;
    }
    if(!inlib::isenv("SLASHROOT")) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << "Environment variable SLASHROOT not defined."
          << std::endl;
      return false;
    }
    if(!inlib::isenv("LIBDIRLIB")) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << "Environment variable LIBDIRLIB not defined."
          << std::endl;
      return false;
    }
    if(!inlib::isenv("BATCHLABDIRLIB")) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << "Environment variable BATCHLABDIRLIB not defined."
          << std::endl;
      return false;
    }
  
    std::string builder;
  
    if(inlib::is_f77(aFile)) {

#ifdef WIN32
      builder = "$BATCHLABROOT/scripts/DOS/makf77dll.bat";
#else
      builder = "$BATCHLABROOT/scripts/sh/makf77dll";
#endif

    } else if(inlib::is_cpp(aFile)) {
  
#ifdef WIN32
      builder = "$BATCHLABROOT/scripts/DOS/makcdll.bat";
#else
      builder = "$BATCHLABROOT/scripts/sh/makcdll";
#endif
    }

    if(builder.empty()) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << " builder not found for file " << inlib::sout(aFile)
          << std::endl;
      return false;
    }
  
    if(!aFunctionManager.load(aFile,builder)) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::FunctionCatalog::loadFile :"
          << " can't load " << inlib::sout(aFile)
          << std::endl;
      return false;
    }
  
    return true;
  }
private:
  Slash::Core::ISession& fSession;
};

}

#endif
