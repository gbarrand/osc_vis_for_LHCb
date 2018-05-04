#ifndef Slash_Tools_Manager_h
#define Slash_Tools_Manager_h

#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>

namespace Slash {

template <class aT> 
  inline aT* findManager(Slash::Core::ISession& aSession,const std::string& aName,const std::string& aCast,bool aWarn = false,bool aForw = true) {
  Slash::Core::IManager* manager = aSession.findManager(aName,aForw);
  if(!manager) {
    if(aWarn) {
      std::ostream& out = aSession.cout();
      out << "Slash::findManager :"
          << " \"" << aName << "\" not found." 
          << std::endl;
    }
    return 0;
  }
#if defined(__linux)
  // Python shell (2.3.3) looses the dynamic_cast on the DLL modules !
  // Then we stay with our "safe cast".
  aT* mgr = (aT*)manager->cast(aCast);
#else
  aT* mgr = dynamic_cast<aT*>(manager);
#endif
  if(!mgr) {
    std::ostream& out = aSession.cout();
    out << "Slash::findManager :"
        << " \"" << aName << "\" cast to  \"" << aCast << "\" failed."
        << std::endl;
  }
  return mgr;
}

}


#define Slash_findManager(aSession,aName,aClass) Slash::findManager<aClass>(aSession,aName,#aClass)

#define Slash_findManager_backward(aSession,aName,aClass) Slash::findManager<aClass>(aSession,aName,#aClass,false,false)

#define Slash_findManager_warn(aSession,aName,aClass) Slash::findManager<aClass>(aSession,aName,#aClass,true)

#endif
