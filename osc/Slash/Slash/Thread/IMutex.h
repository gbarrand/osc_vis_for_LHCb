#ifndef Slash_Thread_IMutex_h
#define Slash_Thread_IMutex_h

#include <string>

namespace Slash {
namespace Thread {

class IMutex {
public:
  virtual ~IMutex(){}
public:
  virtual bool lock() = 0;
  virtual bool unlock() = 0;
  //virtual bool trylock() = 0; //WIN32 : pb to implement.
};

}}

#endif
