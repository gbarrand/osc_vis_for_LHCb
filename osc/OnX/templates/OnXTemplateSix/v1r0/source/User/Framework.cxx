// this :
#include <User/Framework.h>

#include <User/Event.h>

User::Framework::Framework():fWantToExit(false){}

User::Framework::~Framework() {
  unsigned int number = fProcessors.size();
  for(unsigned int index=0;index<number;index++) delete fProcessors[index];
  fProcessors.clear();
}

const User::Event& User::Framework::nextEvent() {
  fCurrentEvent = Event();
  return fCurrentEvent;
}
const User::Event& User::Framework::currentEvent() const {
  return fCurrentEvent;
}

void User::Framework::mainLoop(unsigned int aNumber) {
  fWantToExit = false;
  for(unsigned int index=0;index<aNumber;index++) {   
    dispatchEvent(nextEvent());
    if(fWantToExit) break;
  }
}

void User::Framework::dispatchEvent(const Event& aEvent) {
  unsigned int number = fProcessors.size();
  for(unsigned int index=0;index<number;index++) {
    Processor& svc = *(fProcessors[index]);
    svc.processEvent(aEvent);
  }
}

void User::Framework::addProcessor(Processor* aProcessor) {
  fProcessors.push_back(aProcessor); 
}
void User::Framework::wantToExit() {
  fWantToExit = true;
}
