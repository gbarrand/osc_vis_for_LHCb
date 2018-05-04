#ifndef Lib_Env_h
#define Lib_Env_h

#include <Slash/Core/IWriter.h>

#include <Lib/System.h>
#include <Lib/Out.h>
#include <Lib/sout.h>

namespace Lib {

inline bool check_getenv(Slash::Core::IWriter& aWriter,
                           const std::string& aNew,const std::string& aOld,
                           std::string& aEnv){
  if(Lib::System::getenv(aNew,aEnv)) return true;
  if(Lib::System::getenv(aOld,aEnv)) {
    Lib::Out out(aWriter);
    out << "Environment variable " << Lib::sout(aOld) << " is deprecated."
        << " Use " << Lib::sout(aNew) << " instead."
        << Lib::endl;
    return true;
  }
  return false;
}

}

#endif

