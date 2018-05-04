#ifndef Lib_Session_h
#define Lib_Session_h

// Inheritance :
#include <Slash/Tools/BaseSession.h>

#include <Lib/Debug.h>

namespace Lib {

class Session : public Slash::BaseSession {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::Session)
    else return Slash::BaseSession::cast(a_class);
  }

public:
  inline Session(Slash::Core::IWriter& aPrinter,
          bool aBalance = false,
          bool aDebugCheckClass = false)
  : Slash::BaseSession(aPrinter)
  ,fBalance(aBalance)
  {
    Lib::Debug::checkByClass(aDebugCheckClass);
    Lib::Debug::increment("Lib::Session");
  }

  virtual ~Session(){
    rm_managers();
    Lib::Debug::decrement("Lib::Session");
    if(fBalance) Lib::Debug::balance(f_out);
  }

protected:
  inline Session(const Session& aFrom):BaseSession(aFrom){}
private:
  inline Session& operator=(const Session&){return *this;}
private:
  bool fBalance;
};

}

#endif
