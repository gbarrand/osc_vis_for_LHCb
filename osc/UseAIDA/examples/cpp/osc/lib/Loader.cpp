// this :
#include "Loader.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <stdlib.h>

#include <iostream>

AIDA::Loader::Loader(const std::string& aName,bool aQuiet,bool aVerbose)
:fModule(0)
,fName(aName)
,fVerbose(aVerbose)
{
  if(!open()) {
    if(!aQuiet) {
      std::cerr << "AIDA::Loader::Loader :" 
                << " can't load " << fName << " library." 
                << std::endl;
      std::cerr << "AIDA::Loader::Loader :" 
                << " error  : " << fError 
                << std::endl;
    }
  }
}

AIDA::Loader::~Loader() {
  if (fModule) {
    if(fVerbose) {
      std::cout << "AIDA::Loader::~Loader :" 
                << " close \"" << fName << "\"..." 
                << std::endl;
    }
#ifdef WIN32
    ::FreeLibrary((HMODULE)fModule);
#else
    if(::dlclose(fModule)) {
      std::cerr << "AIDA::Loader::~Loader :" 
                << " error  : " << ::dlerror() 
                << std::endl;
    }
#endif
    if(fVerbose) {
      std::cout << "AIDA::Loader::~Loader :" 
                << " close \"" << fName << "\" done." 
                << std::endl;
    }
  }
  fModule = 0;
}
//////////////////////////////////////////////////////////////////////////////
bool AIDA::Loader::open(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fError = "";
  if(fVerbose) {
    std::cout << "AIDA::Loader::open :" 
              << " open \"" << fName 
              << std::endl;
  }
#ifdef WIN32
  if(fVerbose) {
    std::cout << "AIDA::Loader::open :" 
              << " open \"" << fName << "\"..." 
              << std::endl;
  }
  fModule = ::LoadLibrary(fName.c_str());
#else
  int flags = RTLD_NOW | RTLD_GLOBAL;
  fModule = ::dlopen(fName.c_str(),flags);
  if(!fModule) {
#if defined(__CYGWIN__) && defined(__GNUC__)
    std::string dll = fName+".dll";
#elif defined(__APPLE__)
    std::string dll = fName+".bundle";
#else
    std::string dll = "lib"+fName+".so";
#endif
    fModule = ::dlopen(dll.c_str(),flags);
  }
  if(!fModule) fError = ::dlerror();
#endif
  return fModule ? true : false;
}

bool AIDA::Loader::isOpened() const {return fModule?true:false;}

const std::string& AIDA::Loader::fileName() const {return fName;}

AIDA::Procedure AIDA::Loader::find(const std::string& aName) const {
  if(!fModule) return 0;
  Procedure p = 0;
#ifdef WIN32
  p = (Procedure)::GetProcAddress((HMODULE)fModule,aName.c_str());
  if (!p) {
    std::string s = "_";
    s += aName;
    p = (Procedure)::GetProcAddress((HMODULE)fModule,s.c_str());
    if (!p) {
      std::cerr << "AIDA::Loader::findProcedure :"
                << " can't load " << aName << " procedure." 
                << std::endl;
    }
  }
#else
  const char* cerror;
  p = (Procedure)::dlsym(fModule,(char*)aName.c_str());
  if ((cerror=dlerror())!=NULL) { //error : undefined symbol.
    std::string serror = cerror;
    std::string s = "_";
    s += aName;
    p = (Procedure)::dlsym(fModule,(char*)s.c_str());
    if ((cerror=dlerror())!=NULL) { //error : undefined symbol.
      std::cerr << "AIDA::Loader::findProcedure :" 
                << " problem when searching \"" << aName << "\""
                << " in \"" << fName << "\"."
                << " dlerror  : " << serror 
                << std::endl;
    } 
  }
#endif
  return p;
}

AIDA::IAnalysisFactory* AIDA::Loader::findAIDA(const std::string& aEntry) {
  std::string entry = aEntry;
  if(entry.empty()) entry = fName + "_createAnalysisFactory";

  typedef AIDA::IAnalysisFactory*(*CreateAnalysisFactoryFunction)();
  CreateAnalysisFactoryFunction func = 
   (CreateAnalysisFactoryFunction)find(entry);
  if(!func) {
    std::cout << "AIDA::Loader::findAIDA :" 
              << " " << entry << " not found in dll " << fName << "."
              << std::endl;
    return 0;
  }
  AIDA::IAnalysisFactory* af = (AIDA::IAnalysisFactory*)(*func)();
  if(!af) {
    std::cout << "AIDA::Loader::findAIDA :" 
              << " can't create an AIDA::IAnalysisFactory" 
              << " from dll " << fName << "."
              << std::endl;
    return 0;
  }
  return af;
}
