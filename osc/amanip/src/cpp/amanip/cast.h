#ifndef amanip_cast_h
#define amanip_cast_h

#include <AIDA/IManagedObject.h>
#include <AIDA/IBaseHistogram.h>

//#if defined(__linux)
// Python shell (2.3.3) looses the dynamic_cast on the DLL modules !
// Then we stay with our "safe cast".

//#if defined(__linux)
//#define amanip_cast(a_o,a_class) (a_class*)a_o.cast(#a_class)
//#else
//#define amanip_cast(a_o,a_class) dynamic_cast<a_class*>(&a_o)
//#endif

//#if defined(__linux)
//#define amanip_const_cast(a_o,a_class) (const a_class*)a_o.cast(#a_class)
//#else
//#define amanip_const_cast(a_o,a_class) dynamic_cast<const a_class*>(&a_o)
//#endif

namespace amanip {

template <class aT,class aF>
inline aT* cast(aF& a_o,const std::string& aCast) {
#if defined(__linux)
  // Python shell (2.3.3) looses the dynamic_cast on the DLL modules !
  // Then we stay with our "safe cast".
  return (aT*)a_o.cast(aCast);
#else
  return dynamic_cast<aT*>(&a_o);
#endif
}

template <class aT,class aF>
inline const aT* cast_const(const aF& a_o,const std::string& aCast) {
#if defined(__linux)
  // Python shell (2.3.3) looses the dynamic_cast on the DLL modules !
  // Then we stay with our "safe cast".
  return (const aT*)a_o.cast(aCast);
#else
  return dynamic_cast<const aT*>(&a_o);
#endif
}

/*
template <class aT> 
inline aT* cast(AIDA::IManagedObject& a_o,const std::string& aCast) {
  return cast<aT,AIDA::IManagedObject>(a_o,aCast);
}

template <class aT> 
inline aT* cast(AIDA::IBaseHistogram& a_o,const std::string& aCast) {
  return cast<aT,AIDA::IBaseHistogram>(a_o,aCast);
}

template <class aT> 
inline AIDA::IManagedObject* cast_mo(aT& a_o) {
  return cast<AIDA::IManagedObject,aT>(a_o,"AIDA::IManagedObject");
}
*/

}

#endif
