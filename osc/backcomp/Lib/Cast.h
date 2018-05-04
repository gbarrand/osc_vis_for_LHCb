#ifndef Lib_Cast_h
#define Lib_Cast_h

// For Panoramix backward compatibility.

#if defined(__linux)

// Python shell (2.3.3) looses the dynamic_cast on the DLL modules !
// Then we stay with our "safe cast".
// Stay in sync with Slash/Store/IStorable.h.
// Stay in sync with Lib/Manager.h.

#define Lib_Cast(aObject,aClass) (aClass*)(aObject).cast(#aClass)
#define Lib_ConstCast(aObject,aClass) (const aClass*)(aObject).cast(#aClass)

#else

#define Lib_Cast(aObject,aClass) dynamic_cast<aClass*>(&aObject)
#define Lib_ConstCast(aObject,aClass) dynamic_cast<const aClass*>(&aObject)

#endif

#define Lib_SafeCast(aObject,aClass) (aClass*)(aObject).cast(#aClass)

// Used in implementations of interfaces cast methods.
#define Lib_SCast(aClass) (void*)static_cast<const aClass*>(this)

#define if_Lib_SCast(a_Class) if(aClass==#a_Class) {return Lib_SCast(a_Class);}

// To break const :
#define Lib_self(aClass) aClass& self = const_cast<aClass&>(*this)

#endif
