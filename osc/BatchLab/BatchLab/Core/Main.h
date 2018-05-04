#ifndef BatchLab_Main_h
#define BatchLab_Main_h

// Inheritance :
#include <Slash/Tools/Printer.h>
#include <Lib/Session.h>
#include <BatchLab/Core/AnalysisFactory.h>

#include <inlib/system>
#include <inlib/args>
#include <inlib/vmanip>

namespace BatchLab {

class Main 
:public Slash::Printer //must be before Lib::Session
,public Lib::Session
,public AnalysisFactory {
public:
  virtual void* cast(const std::string& a_class) const {
    // inheritance :
    INLIB_IF_CAST(Slash::Printer)
    else if(void* p = Lib::Session::cast(a_class)) return p;
    else if(void* p = BatchLab::AnalysisFactory::cast(a_class)) return p;
    // this :
    else INLIB_IF_CAST(BatchLab::Main)
    else return 0;
  }
public:
  Main(const std::vector<std::string>& aArgs)
  : Lib::Session(*((Slash::Printer*)this),
               belong(aArgs,"-balance"),
               belong(aArgs,"-debug_check_class")) //acrobatic !
  , AnalysisFactory(*((Lib::Session*)this)) //acrobatic !
  {
    Lib::Debug::increment("BatchLab::Main");
    inlib::args args(aArgs);
   {std::string senv;
    if(inlib::getenv("OSC_ARGS",senv)) {
      args.add(inlib::words(senv," "));
    }}
    std::string sverbose;
    if(args.find("-verbose",sverbose)) {
      if(sverbose.empty()) {
        Lib::Session::setVerboseLevel(1);
      }
    }
    if(fVerboseLevel) {    
      cout() << "BatchLab::Main::Main :"
             << " " << this 
             << std::endl;
    }
    Lib::Session::setArguments(args.get_args());
    // declare ourself in the Session.
    // WARNING : we are a Session and a Manager !
    addManager(this);
  }


  virtual ~Main() {
    if(fVerboseLevel) {
      cout() << "BatchLab::Main::~Main :"
             << " " << this 
             << std::endl;
    }
  
    // take care of ourself being a manager :
    Lib::Session::removeManager(this);
  
    Lib::Debug::decrement("BatchLab::Main");
  }

private:
  Main(const Main& aFrom)
  : Lib::Session(const_cast<Main*>(&aFrom)->printer())
  , AnalysisFactory(*static_cast<Lib::Session*>( const_cast<Main*>(&aFrom)  ))
  {}
  Main& operator=(const Main&){return *this;}

private:
  inline static bool belong(const std::vector<std::string>& aArgs,
                            const std::string& aWhat) {
    std::string senv;
    if(inlib::getenv("OSC_ARGS",senv)) {
      std::vector<std::string> args(aArgs);
      inlib::append<std::string>(args,inlib::words(senv," "));
      return inlib::belong(args,aWhat);
    } else {
      return inlib::belong(aArgs,aWhat);
    }
  }    
};

}


#endif
