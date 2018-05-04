#ifndef Lib_Manager_h
#define Lib_Manager_h

#include <Slash/Tools/Manager.h>

#define Lib_findManager(aSession,aName,aClass) Slash::findManager<aClass>(aSession,aName,#aClass)

#endif
