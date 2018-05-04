#ifndef Rio_Debug_h
#define Rio_Debug_h

#include <string>
#include <ostream>

namespace Rio {

class Debug {
public:
  Debug();
  virtual ~Debug();
  static void check(std::ostream&);
  static void increment();
  static void decrement();
  static void increment(const char*);
  static void decrement(const char*);
  static void increment(const std::string&);
  static void decrement(const std::string&);
  static void checkByClass(bool);
private:
  static int fCount;
};

}

#endif
