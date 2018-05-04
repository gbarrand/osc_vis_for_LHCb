#ifndef User_Framework_h
#define User_Framework_h

//
// Example of simple "user" data framework.
//

#include <User/Event.h>
#include <User/Processor.h>

namespace User {

class Framework {
public:
  Framework();
  virtual ~Framework();
  void mainLoop(unsigned int);
  const User::Event& nextEvent();
  const User::Event& currentEvent() const;
  void addProcessor(Processor*);
  void dispatchEvent(const User::Event&);
  void wantToExit();
private:
  User::Event fCurrentEvent;
  std::vector<Processor*> fProcessors;
  bool fWantToExit;
};

}

#endif
