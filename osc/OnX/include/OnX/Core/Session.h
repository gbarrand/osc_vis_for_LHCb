#ifndef OnX_Session_h
#define OnX_Session_h

// Inheritance :
#include <OnX/Core/Printer.h>
#include <Lib/Session.h>

#include <sstream>

namespace OnX {class StreamDestination;}

namespace OnX {

class Session 
:public OnX::Printer //must be before the below
,public Lib::Session {
public: //Slash::Core::ISession
  virtual void* cast(const std::string&) const;
  virtual bool load(const std::string&,const std::string&);
  virtual void redirectOutput(bool);
  virtual bool flush();
public:
  Session(bool balance = false,bool check_class = false);
  virtual ~Session();
private:
  Session(const Session&);
  Session& operator=(const Session&);
public:
  bool setDefaults(const std::string&);
  void coutToOnX();
  void coutToDefault();
  void stdoutToOnX();
  void stdoutToDefault();
  //virtual void executeScript(const std::string&,const std::string&);
private:
  // cout redirection :
  std::streambuf* fOutOld;
  StreamDestination* fOutDestination;
  // cerr redirection :
  std::streambuf* fErrOld;
  StreamDestination* fErrDestination;
  // stdout redirection :
  std::string fOutDefault;
  std::string fOutName;
  void* fOutFile; //Do not expose FILE*
  // stderr redirection :
  std::string fErrDefault;
  std::string fErrName;
  void* fErrFile; //Do not expose FILE*
};

}

#endif


