#ifndef User_Processor_h
#define User_Processor_h

//
// Example of simple "user" framework service.
//

namespace User {class Framework;}
namespace User {class Event;}

namespace User {

class Processor {
public:
  Processor(Framework&);
  Processor(const Processor&);
  virtual ~Processor();
  Processor& operator=(const Processor&);
public:
  virtual void processEvent(const Event&) = 0;
private:
  Framework& fFramework;
};

}

#endif
