#ifndef Lib_Debug_h
#define Lib_Debug_h

#include <string>
#include <ostream>

namespace Lib {

class Debug {
public:
  Debug();
  virtual ~Debug();
public:
  static void increment();
  static void decrement();
  static void increment(const char*);
  static void decrement(const char*);
  static void increment(const std::string&);
  static void decrement(const std::string&);
  static void balance(std::ostream&);
  static void checkByClass(bool);
  static bool checkByClass();
};

}

#endif




