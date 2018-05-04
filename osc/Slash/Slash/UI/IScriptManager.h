#ifndef Slash_UI_IScriptManager_h
#define Slash_UI_IScriptManager_h

#include <vector>

#include <Slash/UI/IInterpreter.h> //For Function typedef.

namespace Slash {

namespace UI {

class IScriptManager {
public:
  virtual ~IScriptManager() {};
public:
  virtual IInterpreter* addInterpreter(const std::string&, //name
                              const std::string&, //library
                              const std::string&, //package
                              const std::string&,  //file suffix
                              const std::string&,  //exec file command
                              const std::string&,  //mendatory env variable
                              IInterpreter::Function, //function
                              void*, ///function tag
                              IInterpreter::Completer = 0,
                              void* = 0
                              ) = 0;
  virtual IInterpreter* findInterpreter(const std::string&) const = 0;
  virtual IInterpreter* findInterpreterFromSuffix(const std::string&) const = 0;

  virtual bool executeScript(const std::string&,const std::string&,const Slash::UI::IInterpreter::Options&) = 0;
  virtual void recordExecutedScripts(bool) = 0;
  virtual bool record(const std::string&,const std::string&) = 0;
  virtual std::vector<std::string> names() const = 0;

  virtual bool removeInterpreter(IInterpreter*&) = 0;
};

} //UI

} //Slash

#endif
