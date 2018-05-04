#ifndef amanip_name_h
#define amanip_name_h

#include <AIDA/ITree.h>

#include "inlib.h"

namespace amanip {

inline std::string create_name(const AIDA::ITree& aTree,const std::string& aPrefix){  
  std::string prefix(aPrefix);
  if(aPrefix.empty()) prefix = "object_";
  int mx = -1;
  unsigned int pfxl = prefix.size();
  std::vector<std::string> nms = aTree.listObjectNames();
  std::vector<std::string>::const_iterator it;
  for(it=nms.begin();it!=nms.end();++it) {
    if((*it).substr(0,pfxl)==prefix) {
      std::string r = (*it).substr(pfxl,(*it).size()-pfxl);
      int i;
      if(ain_lib::to<int>(r,i)) mx = (mx>i?mx:i);
    }
  }
  std::ostringstream strm;
  strm << (mx+1);
  std::string s = prefix+strm.str();
  return s;
}

}

#endif
