#ifndef amanip_inlib_h
#define amanip_inlib_h

//extracted from inlib/smanip

#include <string>
#include <vector>

namespace ain_lib {

inline std::string sout(const std::string& a_string) {
  return std::string("\"")+a_string+"\"";
}

inline void words(const std::string& a_string,const std::string& a_sep,bool a_take_empty,std::vector<std::string>& a_words){
  //  If a_sep is for exa "|" and for "xxx||xxx" :
  //  - a_take_empty false : {"xxx","xxx"} will be created 
  //    (and NOT {"xxx","","xxx"}).
  //  - a_take_empty true : {"xxx","","xxx"} will be created.
  a_words.clear();
  if(a_string.empty()) return;
  std::string::size_type lim = (a_take_empty?0:1);
  if(a_sep.empty()) {
    a_words.push_back(a_string);
  } else {
    std::string::size_type l = a_string.length();
    std::string::size_type llimiter = a_sep.length();
    std::string::size_type pos = 0;
    while(true) {
      std::string::size_type index = a_string.find(a_sep,pos);
      if(index==std::string::npos){ // Last word.
        if((l-pos)>=lim) a_words.push_back(a_string.substr(pos,l-pos));
        break;
      } else {
        //     abcxxxef
        //     0  3  67
        if((index-pos)>=lim) a_words.push_back(a_string.substr(pos,index-pos));
        pos = index + llimiter;
      }
    }
  }
}

inline bool path_name_suffix(
 const std::string& a_string
,std::string& a_path
,std::string& a_name
,std::string& a_suffix
){
  // If a_string = dir0/dir1/dir2/dir3/name.xxx
  //   a_path = dir0/dir1/dir2/dir3
  //   a_name = name.xxx
  //   a_suffix = xxx
  // If a_string = dir0/name.xxx
  //   a_path = dir0
  //   a_name = name.xxx
  //   a_suffix = xxx
  // If a_string = name.xxx
  //   a_path.clear()
  //   a_name = name.xxx
  //   a_suffix = xxx
  // If a_string = /name.xxx
  //   a_path = "/"
  //   a_name = name.xxx
  //   a_suffix = xxx
  // If a_string = .
  //   a_path = "."
  //   a_name.clear()
  //   a_suffix.clear()
  // If a_string = ..
  //   a_path = ".."
  //   a_name.clear()
  //   a_suffix.clear()
  if(a_string==".") {
    a_path = ".";
    a_name.clear();
    a_suffix.clear();
    return true;
  } else if(a_string=="..") {
    a_path = "..";
    a_name.clear();
    a_suffix.clear();
    return true;
  }

  std::string::size_type pos_slash = a_string.rfind('/');
  std::string::size_type pos_bslash = a_string.rfind('\\');
  std::string::size_type pos = 0;
  if(pos_slash==std::string::npos) {
    if(pos_bslash==std::string::npos) {
      pos = std::string::npos;
    } else {
      pos = pos_bslash;
    }
  } else {
    if(pos_bslash==std::string::npos) {
      pos = pos_slash;
    } else {
      if(pos_slash<=pos_bslash) {
        pos = pos_bslash;
      } else {
        pos = pos_slash;
      }
    }
  }

  if(pos==std::string::npos) {
    a_path.clear();
    pos = 0;
  } else if(pos==0) {
    a_path = "/";
    pos++;
  } else {
    a_path = a_string.substr(0,pos);
    pos++;
  }
  std::string s = a_string.substr(pos,a_string.size()-pos);
  pos = s.rfind('.');
  if(pos==std::string::npos) {
    a_name = s;
    a_suffix.clear();
  } else {
    a_name = s;
    pos++;
    a_suffix = s.substr(pos,s.size()-pos);
  }
  return true;
}

inline std::string to(bool a_value){return a_value?"true":"false";}

inline bool to(const std::string& a_string,bool& a_value){
  if(  (a_string=="1")
     ||(a_string=="true")||(a_string=="TRUE")||(a_string=="True")
     ||(a_string=="yes")||(a_string=="YES")||(a_string=="Yes")
     ||(a_string=="on")||(a_string=="ON")||(a_string=="On")
     ){
    a_value = true;
    return true;
  } else if((a_string=="0")
          ||(a_string=="false")||(a_string=="FALSE")||(a_string=="False")
          ||(a_string=="no")||(a_string=="NO")||(a_string=="No")
          ||(a_string=="off")||(a_string=="OFF")||(a_string=="Off")
          ){
    a_value = false;
    return true;
  } else {
    a_value = false;
    return false;
  }
}

}

#include <sstream>

namespace ain_lib {

template <class T>
inline bool to(const std::string& a_s,T& a_v) {
  if(a_s.empty()) {a_v = T();return false;} //for INLIB_STL istringstream.
  std::istringstream strm(a_s.c_str());
  strm >> a_v;
  if(strm.fail()) {a_v = T();return false;}
  return strm.eof();
}

template <class T>
inline std::string to(const T& a_v) {
  std::ostringstream strm;
  strm << a_v;
  return strm.str();
}

inline std::string d2s(double a_value){
  std::ostringstream strm;
  strm.precision(25);
  strm << a_value;
  return strm.str();
}

enum what { leading, trailing, both };

inline void strip(std::string& a_string,what a_type = both,char a_char = ' '){
  std::string::size_type l = a_string.length();;
  if(l==0) return;

  switch ( a_type ) {
  case leading:{
    std::string::size_type i;
    char* pos = (char*)a_string.c_str();
    for(i=0;i<l;i++,pos++) {
      if(*pos!=a_char) {
        a_string = a_string.substr(i,l-i);
        return;
      }
    }
    }break;
  case trailing:{
    std::string::size_type i;
    char* pos = (char*)a_string.c_str();
    pos += (l-1);
    for(i=l-1;;i--,pos--) {
      if(*pos!=a_char) {
        a_string = a_string.substr(0,i+1);
        return;
      }
    }
    }break;
  case both:
    strip(a_string,leading,a_char);
    strip(a_string,trailing,a_char);
    break;
  //default:break;
  }
}

enum side { left, right, middle };

inline bool justify(std::string& a_string,unsigned int a_size,side a_side = left){
  // a_size is the final string length.
  strip(a_string);
  if(a_size<=a_string.size()) {
    a_string.resize(a_size);
    return false;
  } else {
    if(a_side==left) {
      a_string = a_string + std::string(a_size-a_string.size(),' ');
    } else if(a_side==right) {
      a_string = std::string(a_size-a_string.size(),' ') + a_string;
    } else if(a_side==middle) {
      int l = a_size - a_string.size();
      int h = l/2;
      if(h*2==l) { //even number of spaces :
        a_string = std::string(h,' ') + a_string + std::string(h,' ');
      } else { // odd number of spaces :
        a_string = std::string(h,' ') + a_string + std::string(h+1,' ');
      }
    }
    return false;
  }
}

inline std::string sjust(const std::string& a_string,unsigned int a_size,side a_side = left){
  std::string s(a_string); 
  justify(s,a_size,a_side);
  return s;
}

template <class aT>
inline std::string sjust(aT a_value,unsigned int a_size,side a_side = left) {
  std::string s = to<aT>(a_value);
  justify(s,a_size,a_side);
  return s;
}

}

#endif
