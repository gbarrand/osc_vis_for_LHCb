#ifndef OnXLab_Main_h
#define OnXLab_Main_h

// Inheritance :
#include <OnX/Core/Main.h>
#include <OnXLab/Core/AnalysisFactory.h>

namespace OnXLab {

// The Main is a Slash::Core::ISession and 
// an OnXLab::AnalysisFactory.

class Main : public OnX::Main, public OnXLab::AnalysisFactory {
public:
  virtual void* cast(const std::string& a_class) const {
    // inheritance :
    if(void* p = OnX::Main::cast(a_class)) return p;
    else if(void* p = OnXLab::AnalysisFactory::cast(a_class)) return p;
    // this :
    else INLIB_IF_CAST(OnXLab::Main)
    else return 0;
  }
public:
  Main(const std::vector<std::string>& aArgs = std::vector<std::string>())
  : OnX::Main(aArgs)
  , OnXLab::AnalysisFactory(*((Lib::Session*)this)) { //acrobatic !
    Lib::Debug::increment("OnXLab::Main");
    if(verboseLevel()) {    
      cout() << "OnXLab::Main::Main :"
             << " " << this << std::endl;
    }

    // declare ourself in the Session.
    // WARNING : we are a Session and a Manager !
    addManager(this);
  }


  virtual ~Main() {
    if(verboseLevel()) {    
      cout() << "OnXLab::Main::~Main :"
             << " " << this << std::endl;
    }
    // take care of ourself being a manager :
    Lib::Session::removeManager(this);
    Lib::Debug::decrement("OnXLab::Main");
  }

};

}

#endif
