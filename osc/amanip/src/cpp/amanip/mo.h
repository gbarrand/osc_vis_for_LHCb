#ifndef amanip_mo_h
#define amanip_mo_h

#include <AIDA/IManagedObject.h>
#include <AIDA/IBaseHistogram.h>
#include <AIDA/IDataPointSet.h>
#include <AIDA/IFunction.h>
#include <AIDA/ITuple.h>

#include "cast.h"

namespace amanip {

template <class aT>
inline std::string name(const aT& aObject) {
  const AIDA::IManagedObject* mo = 
    cast_const<AIDA::IManagedObject,aT>(aObject,"AIDA::IManagedObject");
  if(!mo) return "";
  return mo->name();
}

inline bool title(const AIDA::IManagedObject& a_o,std::string& a_value) {
  if(const AIDA::IBaseHistogram* h = 
       cast_const<AIDA::IBaseHistogram,AIDA::IManagedObject>
         (a_o,"AIDA::IBaseHistogram")) {
    a_value = h->title();
    return true;   
  } else if(const AIDA::IDataPointSet* dps = 
       cast_const<AIDA::IDataPointSet,AIDA::IManagedObject>
         (a_o,"AIDA::IDataPointSet")) {
    a_value = dps->title();
    return true;   
  } else if(const AIDA::IFunction* f = 
       cast_const<AIDA::IFunction,AIDA::IManagedObject>
         (a_o,"AIDA::IFunction")) {
    a_value = f->title();
    return true;   
  } else if(const AIDA::ITuple* tuple = 
       cast_const<AIDA::ITuple,AIDA::IManagedObject>(a_o,"AIDA::ITuple")) {
    a_value = tuple->title();
    return true;   
  }
  a_value = "";
  return false;
}

// used in BatchLab/BaseFolder.cxx.
inline bool type(const AIDA::IManagedObject& a_o,std::string& a_value) {
  //NOTE : with AIDA-3.3 you should use a_o.type()
  if(a_o.cast("AIDA::IHistogram1D")) {
    a_value = "AIDA::IHistogram1D";
    return true;
  } else if(a_o.cast("AIDA::IHistogram2D")) {
    a_value = "AIDA::IHistogram2D";
    return true;
  } else if(a_o.cast("AIDA::IHistogram3D")) {
    a_value = "AIDA::IHistogram3D";
    return true;
  } else if(a_o.cast("AIDA::IProfile1D")) {
    a_value = "AIDA::IProfile1D";
    return true;
  } else if(a_o.cast("AIDA::IProfile2D")) {
    a_value = "AIDA::IProfile2D";
    return true;
  } else if(a_o.cast("AIDA::ICloud1D")) {
    a_value = "AIDA::ICloud1D";
    return true;
  } else if(a_o.cast("AIDA::ICloud2D")) {
    a_value = "AIDA::ICloud2D";
    return true;
  } else if(a_o.cast("AIDA::ICloud3D")) {
    a_value = "AIDA::ICloud3D";
    return true;
  } else if(a_o.cast("AIDA::IDataPointSet")) {
    a_value = "AIDA::IDataPointSet";
    return true;
  } else if(a_o.cast("AIDA::IFunction")) {
    a_value = "AIDA::IFunction";
    return true;
  } else if(a_o.cast("AIDA::ITuple")) {
    a_value = "AIDA::ITuple";
    return true;
  }
  a_value = "unknown";
  return false;
}

}

#endif
