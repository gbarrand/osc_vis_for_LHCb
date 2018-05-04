#ifndef Slash_UI_IInterpreter_h
#define Slash_UI_IInterpreter_h

#include <string>
#include <vector>

namespace Slash {

namespace UI {

class IInterpreter {
public:
  virtual ~IInterpreter() {};
public:
  typedef std::pair<std::string,std::string> Option;
  typedef std::vector<Option> Options;
  typedef std::pair<std::string,std::string> Alias;
  typedef std::vector<Alias> Aliases;
  typedef bool (*Function)(const std::string&,
                           const Options&,
                           const Aliases&,
                           void*);
  typedef bool (*Completer)(const std::string&,std::string&,void*);
public:
  virtual std::string name() const = 0;
  virtual std::string package() const = 0;
  virtual std::string suffix() const = 0;
  virtual std::string execFileCommand() const = 0;
  virtual void setFunction(Function,void*) = 0;
  virtual Function function(void*&) const = 0;
  virtual bool executeScript(const std::string&,const Options&) = 0;
  virtual bool isOperational() = 0;
  virtual bool load(bool quiet = false) = 0;
  virtual bool addAlias(const std::string& alias,const std::string& value) = 0;
  virtual void setCompleter(Completer,void*) = 0;
  virtual Completer completer(void*&) const = 0;
  virtual bool complete(const std::string&,std::string&) = 0;
};

} //UI

} //Slash

#endif
