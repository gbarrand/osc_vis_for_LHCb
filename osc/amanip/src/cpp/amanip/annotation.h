#ifndef amanip_annotation_h
#define amanip_annotation_h

#include <AIDA/IAnnotation.h>

#include "inlib.h"

namespace amanip {

inline std::string to(const AIDA::IAnnotation& a_annot){
  std::string s;
  int number = a_annot.size();
  for(int index=0;index<number;index++) {
    if(index) s += "\n";
    s += a_annot.key(index);
    s += "=";
    s += a_annot.value(index);
  }
  return s;

}

inline bool from(const std::string& a_string,AIDA::IAnnotation& a_annot){
  std::vector<std::string> items;
  ain_lib::words(a_string,"\n",false,items);
  std::vector<std::string>::const_iterator it;
  bool ok = true;
  for(it=items.begin();it!=items.end();++it) {
    std::string::size_type pos = (*it).find('=');
    if(pos!=std::string::npos) {
      // key=value
      // 012345678
      std::string key = (*it).substr(0,pos);
      std::string val = (*it).substr(pos+1,(*it).size()-(pos+1));
      a_annot.addItem(key,val);
    } else {
      ok = false;
    }
  }
  return ok;
}

}

#endif
