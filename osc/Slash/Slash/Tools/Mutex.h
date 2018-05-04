#ifndef Slash_Tools_Mutex_h
#define Slash_Tools_Mutex_h

// A IMutex implementation.

// inheritance :
#include <Slash/Thread/IMutex.h>

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace Slash {

class Mutex : public virtual Slash::Thread::IMutex {
public: //IMutex
#ifdef WIN32
  virtual bool lock(){
    if(!f_imp) return false;
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)f_imp;
    ::EnterCriticalSection(cs);
    return true;
  }

  virtual bool unlock(){
    if(!f_imp) return false;
    CRITICAL_SECTION* cs = (CRITICAL_SECTION*)f_imp;
    ::LeaveCriticalSection(cs);
    return true;
  }

  //virtual bool trylock(){
  //  if(!f_imp) return false;
  //  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)f_imp;
  //  if(::TryEnterCriticalSection(cs)) return true;
  //  return false;
  //}
#else

  virtual bool lock(){
    if(!f_imp) return false;
    pthread_mutex_t* mutex = (pthread_mutex_t*)f_imp;
    int status = ::pthread_mutex_lock(mutex);
    return status?false:true;
  }

  virtual bool unlock(){
    if(!f_imp) return false;
    pthread_mutex_t* mutex = (pthread_mutex_t*)f_imp;
    int status = ::pthread_mutex_unlock(mutex);
    return status?false:true;
  }

  //virtual bool trylock(){
  //  if(!f_imp) return false;
  //  pthread_mutex_t* mutex = (pthread_mutex_t*)f_imp;
  //  int status = ::pthread_mutex_trylock(mutex);
  //  return status?false:true;
  //}

#endif

public:
  Mutex():f_imp(0){}
#ifdef WIN32
  virtual ~Mutex(){
    if(f_imp) {
      CRITICAL_SECTION* cs = (CRITICAL_SECTION*)f_imp;
      ::DeleteCriticalSection(cs);
      delete cs;
    }
  }
#else
  virtual ~Mutex(){
    if(f_imp) {
      pthread_mutex_t* mutex = (pthread_mutex_t*)f_imp;
      ::pthread_mutex_destroy(mutex);
      delete mutex;
    }
  }
#endif

public:
#ifdef WIN32
  bool initialize(){
    if(f_imp) return true; //done.
    CRITICAL_SECTION* cs = new CRITICAL_SECTION;
    ::InitializeCriticalSection(cs);
    f_imp = cs;
    return true;
  }
#else
  bool initialize(){
    if(f_imp) return true; //done.
    pthread_mutex_t* mutex = new pthread_mutex_t;
    int status = ::pthread_mutex_init(mutex,0);
    if(status) {
      delete mutex;
      return false;
    }
    f_imp = mutex;
    return true;
  }
#endif

private:
  Mutex(const Mutex&){}
  Mutex& operator=(const Mutex&){return *this;}
private:
  void* f_imp;
};

}

#endif
