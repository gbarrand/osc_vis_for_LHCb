#ifndef Slash_UI_ICyclic_h
#define Slash_UI_ICyclic_h

#include <string>

namespace Slash {

namespace UI {

class ICyclicCallback {
public:
  virtual ~ICyclicCallback() {};
public:
  virtual void execute() = 0;
};

class ICyclic {
public:
  virtual ~ICyclic() {};
public:
  virtual std::string name() const = 0;
  virtual void setDelay(int) = 0;
  virtual void setCallback(ICyclicCallback*) = 0;
  virtual void setScript(const std::string&,const std::string&) = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void execute() = 0;
  virtual int delay() const = 0;
  virtual int iterations() const = 0;
  virtual bool started() const = 0;

  virtual std::string interpreter() const = 0;
  virtual std::string script() const = 0;
};

} //UI

} //Slash

#endif
