#ifndef User_PrintProcessor_h
#define User_PrintProcessor_h

//
// Example of simple user framework print service.
//

// Inheritance :
#include <User/Processor.h>

namespace User {

class PrintProcessor : public Processor {
public:
  PrintProcessor(Framework&);
  PrintProcessor(const PrintProcessor&);
  virtual ~PrintProcessor();
  PrintProcessor& operator=(const PrintProcessor&);
public:
  virtual void processEvent(const Event&);
};

}

#endif
