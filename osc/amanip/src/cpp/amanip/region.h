#ifndef amanip_region_h
#define amanip_region_h

#include <AIDA/IPlotterRegion.h>

#include "inlib.h"

namespace amanip {

inline bool parameter(AIDA::IPlotterRegion& aRegion,const std::string& aWhat,double& aValue){
  aValue = 0;
  std::string s = aRegion.parameterValue(aWhat);
  double value;
  if(!ain_lib::to<double>(s,value)) return false;
  aValue = value;
  return true;
}
inline bool parameter(AIDA::IPlotterRegion& aRegion,const std::string& aWhat,int& aValue){
  aValue = 0;
  std::string s = aRegion.parameterValue(aWhat);
  int value;
  if(!ain_lib::to<int>(s,value)) return false;
  aValue = value;
  return true;
}
inline bool parameter(AIDA::IPlotterRegion& aRegion,const std::string& aWhat,bool& aValue){
  aValue = false;
  std::string s = aRegion.parameterValue(aWhat);
  bool value;
  if(!ain_lib::to(s,value)) return false;
  aValue = value;
  return true;
}

}

#endif
