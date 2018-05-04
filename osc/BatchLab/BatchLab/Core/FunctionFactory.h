#ifndef BatchLab_FunctionFactory_h
#define BatchLab_FunctionFactory_h

// Inheritance :
#include <BatchLab/Core/BaseFactory.h>
#include <AIDA/IFunctionFactory.h>

#include <BatchLab/Core/FFactory.h>
#include <BatchLab/Core/Function.h>
#include <BatchLab/Core/FunctionCatalog.h>

namespace BatchLab {

class FunctionFactory 
:public BaseFactory
,public virtual AIDA::IFunctionFactory 
{
public: //AIDA/IFunctionFactory
  virtual AIDA::IFunction* createFunctionByName(const std::string& aPath,
                                                const std::string& aWhat){
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    std::string title = name; //FIXME
  
    //if(aWhat.substr(0,12)=="thread_safe:") 
  
    BatchLab::BaseFunction* object = 
      BatchLab::FFactory::create(fSession,name,title,aWhat);
    if(!object) return 0;
    //if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }

  virtual AIDA::IFunction* createFunctionFromScript(const std::string& aPath,
			                int aDimension, //Wanted dimension. 0 could be passed if guessed from script.
                                        const std::string& aScript,
                                        const std::string& /*aParameters*/,
                                        const std::string& /*aDescription*/,
                                        const std::string& /*aGradExpr*/ = ""){
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    std::string title = name;//FIXME
  
    std::string what = "scripted:"+aScript;
    BatchLab::BaseFunction* object = 
      BatchLab::FFactory::create(fSession,name,title,what);
    if(!object) return 0;
    //if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
  virtual AIDA::IFunction* cloneFunction(const std::string&,AIDA::IFunction&) {
    return 0;
  }

  virtual AIDA::IFunctionCatalog* catalog() {
    return new BatchLab::FunctionCatalog(fSession);
  }
public:
  FunctionFactory(Slash::Core::ISession& aSession,Slash::Store::ITree& aTree)
  :BaseFactory(aSession.cout(),aTree)
  ,fSession(aSession){
    Lib::Debug::increment("BatchLab::FunctionFactory");
  }
  virtual ~FunctionFactory(){
    Lib::Debug::decrement("BatchLab::FunctionFactory");
  }
private:
  Slash::Core::ISession& fSession;
};

}

#endif
