#ifndef Lib_smanip_h
#define Lib_smanip_h

// for Panoramix backward compatibility.

// Code taken from osc_16.

#include <inlib/words>
#include <inlib/touplow>
#include <inlib/sprintf>
#include <inlib/cstr>
#include <inlib/srep>

#include <cstdlib> //strtol
#include <cstdio>  //sscanf

#include <map>

#include "typedefs.h"

namespace Lib {

class smanip {
public:
  static std::string quote(const std::string& aString) {
    return std::string("\"")+aString+"\"";
  }

  static bool toint(const std::string& aString,int& aValue) {
    //FIXME : notation n[eE][+]m not supported !
    char* s;
    long l = ::strtol(aString.c_str(),&s,10);
    if( (s==aString.c_str()) || (s!=(aString.c_str()+aString.size())) ) {
      aValue = 0;
      return false;
    }
    aValue = (int)l;
    return true;
  }

  static bool touint(const std::string& aString,unsigned int& aValue) {
    // Notation n[eE][+]m not supported !
    if(::sscanf(aString.c_str(),"0x%x",&aValue)==1) return true;
    if(::sscanf(aString.c_str(),"%u",&aValue)==1) return true;
    aValue = 0;
    return false;
  }

  static bool tobool(const std::string& aString,bool& aValue) {
    std::string slower = aString;
    inlib::tolowercase(slower);
    if((slower=="1")||(slower=="true")||(slower=="yes")||(slower=="on")) {
      aValue = true;
      return true;
    } else if((slower=="0")||(slower=="false")||
              (slower=="no")||(slower=="off")){
      aValue = false;
      return true;
    } else {
      aValue = false;
      return false;
    }
  }

  static bool todouble(const std::string& aString,double& aValue) {
    char* s;
    aValue = ::strtod(aString.c_str(),&s);
    if( (s==aString.c_str()) || (s!=(aString.c_str()+aString.size())) ) {
      aValue = 0;
      return false;
    }
    return true;
  }

  static bool tofloat(const std::string& aString,float& aValue) {
    char* s;
     aValue = (float)::strtod(aString.c_str(),&s);
    if( (s==aString.c_str()) || (s!=(aString.c_str()+aString.size())) ) {
      aValue = 0;
      return false;
    }
    return true;
  }

  static bool toints(const std::string& aString,std::vector<int>& aValues) {
    std::vector<std::string> words;
    inlib::words(aString," ",false,words);
    aValues.resize(words.size());
    int valuei = 0;
    for(std::vector<std::string>::iterator it = words.begin();
        it!=words.end();++it) {
      int value;
      if(!toint(*it,value)) {
        aValues.clear();
        return false;
      }
      aValues[valuei] = value;
      valuei++;
    }
    return true;
  }

  static bool todoubles(const std::string& aString,
                        std::vector<double>& aValues,
                        const std::string& aSeparator  = " ") {
    // Support PAW syntax : 3*2.4 is understood as three times 2.4.
    aValues.clear();
    std::vector<std::string> words;
    inlib::words(aString,aSeparator,false,words);
    bool seekMultiplier = (aSeparator!="*"?true:false);
    if(!seekMultiplier) aValues.resize(words.size());
    int valuei = 0;
    std::vector<std::string> subwords;
    std::vector<std::string>::iterator it;
    for(it = words.begin();it!=words.end();++it) {
      if(seekMultiplier) {
        std::string::size_type pos = (*it).find('*');
        if(pos==std::string::npos){
          double value;
          if(!todouble(*it,value)) {
            aValues.clear();
            return false;
          }
          aValues.push_back(value);
        } else {
          //3*2.4
          //01234
          std::string sn = (*it).substr(0,pos);
          std::string sv = (*it).substr(pos+1,aString.size()-(pos+1));
          int multiplier;
          if(!toint(sn,multiplier)) {
            aValues.clear();
            return false;
          }
          double value;
          if(!todouble(sv,value)) {
            aValues.clear();
            return false;
          }
          for(int index=0;index<multiplier;index++) aValues.push_back(value);
        }
      } else {
        double value;
        if(!todouble(*it,value)) {
          aValues.clear();
          return false;
        }
        aValues[valuei] = value;
        valuei++;
      }    
    }
    return true;
  }
  
  static bool topointer(const std::string& aString,void*& aValue) {
#ifdef LIB_INT64_AS_LONG
    unsigned long v = 0L;
    if(::sscanf(aString.c_str(),"0x%lx",&v)!=1) {
      if(::sscanf(aString.c_str(),"%lu",&v)!=1) {
        aValue = 0;
        return false;
      }
    }
#else //32bits platform :
    unsigned int v = 0;
    if(::sscanf(aString.c_str(),"0x%x",&v)!=1) {
      if(::sscanf(aString.c_str(),"%u",&v)!=1) {
        aValue = 0;
        return false;
      }
    }
#endif
    aValue = (void*)v;
    return true;
  }

public:
  class Color {
  public:
    Color(double aRed = 0,double aGreen = 0,double aBlue = 0)
    :fRed(aRed),fGreen(aGreen),fBlue(aBlue)
    {}
  public:
    Color(const Color& aFrom)
    :fRed(aFrom.fRed),fGreen(aFrom.fGreen),fBlue(aFrom.fBlue)
    {}
    Color& operator=(const Color& aFrom) {
      fRed = aFrom.fRed;
      fGreen = aFrom.fGreen;
      fBlue = aFrom.fBlue;
      return *this;
    }
  public:
    double fRed,fGreen,fBlue;
  };

  static const std::map<std::string,Color>& s_colors() {
    static std::map<std::string,Color> s_v;
    if(s_v.empty()) {
      s_v["aquamarine"] = Color(0.496101,0.996109,0.828138);//1
      s_v["mediumaquamarine"] = Color(0.398444,0.800793,0.664073);
      s_v["black"] = Color(0,0,0);
      s_v["blue"] = Color(0,0,1);
      s_v["cadetblue"] = Color(0.371099,0.617197,0.62501);
      s_v["cornflowerblue"] = Color(0.390631,0.58204,0.925795);
      s_v["darkslateblue"] = Color(0.281254,0.238285,0.542977);
      s_v["lightblue"] = Color(0.675792,0.843763,0.898451);
      s_v["lightsteelblue"] = Color(0.68751,0.765637,0.867201);
      s_v["mediumblue"] = Color(0,0,0.800793); //10
    
      s_v["mediumslateblue"] = Color(0.480476,0.406256,0.929702);
      s_v["midnightblue"] = Color(0.0976577,0.0976577,0.437507);
      s_v["navyblue"] = Color(0,0,0.500008);
      s_v["navy"] = Color(0,0,0.500008);
      s_v["skyblue"] = Color(0.527352,0.8047,0.917983);
      s_v["slateblue"] = Color(0.414069,0.351568,0.800793);
      s_v["steelblue"] = Color(0.273442,0.50782,0.703136);
      s_v["coral"] = Color(0.996109,0.496101,0.312505);
      s_v["cyan"] = Color(0,1,1);
      s_v["firebrick"] = Color(0.695323,0.132815,0.132815);//20
    
      s_v["brown"] = Color(0.644541,0.164065,0.164065);
      s_v["gold"] = Color(0.996109,0.839857,0);
      s_v["goldenrod"] = Color(0.851575,0.644541,0.125002);
      s_v["green"] = Color(0,1,0);
      s_v["darkgreen"] = Color(0,0.390631,0);
      s_v["darkolivegreen"] = Color(0.332036,0.417975,0.183597);
      s_v["forestgreen"] = Color(0.132815,0.542977,0.132815);
      s_v["limegreen"] = Color(0.195315,0.800793,0.195315);
      s_v["mediumseagreen"] = Color(0.234379,0.699229,0.441413);
      s_v["mediumspringgreen"] = Color(0,0.976577,0.601572);//30
    
      s_v["palegreen"] = Color(0.593759,0.980484,0.593759);
      s_v["seagreen"] = Color(0.17969,0.542977,0.339849);
      s_v["springgreen"] = Color(0,0.996109,0.496101);
      s_v["yellowgreen"] = Color(0.601572,0.800793,0.195315);
      s_v["darkslategrey"] = Color(0.183597,0.308598,0.308598);
      s_v["dimgrey"] = Color(0.410163,0.410163,0.410163);
      s_v["lightgrey"] = Color(0.824231,0.824231,0.824231);
      s_v["grey"] = Color(0.750011,0.750011,0.750011);
      s_v["khaki"] = Color(0.937514,0.898451,0.546883);
      s_v["magenta"] = Color(1,0,1);//40
    
      s_v["maroon"] = Color(0.68751,0.187503,0.375006);
      s_v["orange"] = Color(0.996109,0.644541,0);
      s_v["orchid"] = Color(0.851575,0.437507,0.83595);
      s_v["darkorchid"] = Color(0.597665,0.195315,0.796887);
      s_v["mediumorchid"] = Color(0.726574,0.332036,0.824231);
      s_v["pink"] = Color(0.996109,0.750011,0.792981);
      s_v["plum"] = Color(0.863294,0.62501,0.863294);
      s_v["red"] = Color(1,0,0);
      s_v["indianred"] = Color(0.800793,0.35938,0.35938);
      s_v["mediumvioletred"] = Color(0.777356,0.0820325,0.519539);//50
          
      s_v["orangered"] = Color(0.996109,0.269535,0);
      s_v["violetred"] = Color(0.812512,0.125002,0.562509);
      s_v["salmon"] = Color(0.976577,0.500008,0.445319);
      s_v["sienna"] = Color(0.62501,0.320317,0.175784);
      s_v["tan"] = Color(0.820325,0.703136,0.546883);
      s_v["thistle"] = Color(0.843763,0.746105,0.843763);
      s_v["turquoise"] = Color(0.250004,0.875013,0.812512);
      s_v["darkturquoise"] = Color(0,0.8047,0.816419);
      s_v["mediumturquoise"] = Color(0.281254,0.816419,0.796887);
      s_v["violet"] = Color(0.929702,0.50782,0.929702);//60
    
      s_v["blueviolet"] = Color(0.539071F,0.167971F,0.882826F);
      s_v["wheat"] = Color(0.957046F,0.867201F,0.699229F);
      s_v["white"] = Color(1,1,1);
      s_v["yellow"] = Color(1,1,0);
      s_v["greenyellow"] = Color(0.675792F,0.996109F,0.18359);  
    }

    return s_v;
  }
public:
  static bool torgb(const std::string& aString,
                    double& aRed,double& aGreen,double& aBlue) {
    aRed = 0.5;
    aGreen = 0.5;
    aBlue = 0.5;
    if(aString.empty()) return false;
    std::vector<double> ds;
    if(todoubles(aString,ds) && (ds.size()>=3)) {
      if( ds[0]<0 || ds[0]>1 ||
          ds[1]<0 || ds[1]>1 ||
          ds[2]<0 || ds[2]>1 ) return false;
      aRed = ds[0];
      aGreen = ds[1];
      aBlue = ds[2];
      return true;
    } else if( (aString.size()==7) && (aString[0]=='#') ) {
      // #RRGGBB format :
      //  1 3 5
      unsigned int rr,gg,bb;
      if(!touint(std::string("0x")+aString.substr(1,2),rr)) return false;
      if(!touint(std::string("0x")+aString.substr(3,2),gg)) return false;
      if(!touint(std::string("0x")+aString.substr(5,2),bb)) return false;
      aRed = double(rr)/255.0;
      aGreen = double(gg)/255.0;
      aBlue = double(bb)/255.0;
      return true;
    } else {
      const std::map<std::string,Color>& mcols = s_colors();
      std::map<std::string,Color>::const_iterator it = mcols.find(aString);
      if(it!=mcols.end()) {
        aRed = (*it).second.fRed;
        aGreen = (*it).second.fGreen;
        aBlue = (*it).second.fBlue;
        return true;
        
      }
    }
    return false;
  }

  static bool torgbs(const std::string& aString,std::string& aValue) {
    double r,g,b;
    bool status = torgb(aString,r,g,b);
    inlib::sprintf(aValue,128,"%g %g %g",r,g,b);
    return status;
  }

  static void lines(const std::string& aString,
                    std::vector<std::string>& aLines) {
    // aString is a list separated by "\n" or "\\n".
    // For "xxx\n\nxxx", {"xxx","","xxx"} will be created.
    // WARNING : if aString is a Windows file name, it may 
    //  contains a \n which is not a delimiter ; like ..\data\ntuples.hbook.
    aLines.clear();
    int length = aString.length();
    if(length<=0) return;
    char* cstring = inlib::str_dup(aString.c_str());
    if(!cstring) return;
    int pos = 0;
    length++;
    for(int count=0;count<length;count++) {
      if( (cstring[count]=='\n') || 
          (cstring[count]=='\0') || 
          ( (cstring[count]=='\\') && (cstring[count+1]=='n') ) ) { 
        char shift_one = (cstring[count]=='\n' ? 1 : 0);
        cstring[count] = '\0';
        aLines.push_back(cstring+pos);
        if(shift_one==1) {
          pos = count+1;
        } else {
          pos = count+2;
          count++;
        }
      }
    }
    inlib::str_del(cstring);
  }

public: //backcomp (Panoramix)
  static std::vector<std::string> words(const std::string& aString,
                                        const std::string& aLimiter,
                                        bool aTakeEmpty =false) {
    std::vector<std::string> words;
    inlib::words(aString,aLimiter,aTakeEmpty,words);
    return words;
  }
  static void words(const std::string& aString,
                    const std::string& aLimiter,
                    std::vector<std::string>& aWords,
                    bool aTakeEmpty = false) {
    aWords = words(aString,aLimiter,aTakeEmpty);
  }
  static bool printf(std::string& a_string,int a_length,
                     const char* a_format,...){
    a_string.clear();
    if(a_length<0) return false;
    if(!a_format) return false;
    char* s = new char[a_length+1];
    if(!s) return false;
    s[a_length] = '\0';
    va_list args;
    va_start(args,a_format);
    int n = inlib::vsnpf(s,a_length+1,a_format,args);
    va_end(args);
    if(n>a_length) {delete [] s;return false;}
    if(s[a_length]!='\0') {delete [] s;return false;}
    a_string = s;
    delete [] s;
    return true;
  }

  //Panoramix
  static bool getenv(std::string& aString) {
    std::string::size_type dollar;
    while((dollar=aString.find('$'))!=std::string::npos){
      std::string::size_type slash = aString.find('/',dollar+1);
      std::string::size_type back_slash = aString.find('\\',dollar+1);
      std::string::size_type pos = std::string::npos; 
      if(slash!=std::string::npos) {
        if(back_slash!=std::string::npos) {
          pos = slash<back_slash?slash:back_slash;
        } else {
          pos = slash;
        }
      } else {
        if(back_slash!=std::string::npos) {
          pos = back_slash;
        } else {
          pos = std::string::npos; 
        }
      }
      std::string env;
      if(pos==std::string::npos) {
        env = aString.substr(dollar+1,aString.length()-(dollar+1));
      } else {
        //     abc$xxx/ef
        //     0  3   7 9
        env = aString.substr(dollar+1,pos-(dollar+1));
      }
      char* val = ::getenv(env.c_str());
      if(!val) return false; // Stop looping if env variable not found.
      std::string value = aString.substr(0,dollar);
      value += val;
      if(pos!=std::string::npos)
        value += aString.substr(pos,aString.length()-pos);
      aString = value;
    }
    return true;
  }
  static std::string tostring(const std::vector<std::string>& aVector,
                              const std::string& aSeparator,
                              bool a_sep_at_end = false) {
    unsigned int number = aVector.size();
    if(number<=0) return "";
    std::string result;
    number--;
    for(unsigned int index=0;index<number;index++) {
      result += aVector[index];
      result += aSeparator;
    }
    result += aVector[number];
    if(a_sep_at_end) result += aSeparator;
    return result;
  }

  static bool replace(std::string& aString,
                      const std::string& aOld,const std::string& aNew) {
    // return true : some replacement done.
    // return false : nothing replaced.
    if(aOld.empty()) return false;
    std::string snew;
    std::string::size_type lold = aOld.length();
    bool status = false;
    std::string stmp = aString;
    while(true) {
      std::string::size_type pos = stmp.find(aOld);
      if(pos==std::string::npos){
        snew += stmp;
        break;
      } else {
        snew += stmp.substr(0,pos);
        snew += aNew;
        stmp = stmp.substr(pos+lold,stmp.length()-(pos+lold));
        status = true;
      }
    }
    aString = snew;
    return status;
  }

  static void unique(std::vector<std::string>& aStrings) {
    std::vector<std::string>::iterator it, it2;
    for(it=aStrings.begin();it!=aStrings.end();++it) {
      for(it2=it+1;it2!=aStrings.end();) {
        if(*it2==*it) {
          it2 = aStrings.erase(it2);
        } else { 
          ++it2;
        }
      }
    }
  }

  static bool pathNameSuffix(const std::string& aString,
                 std::string& aPath,std::string& aName,std::string& aSuffix) {
    // If aString = dir0/dir1/dir2/dir3/name.xxx
    //   aPath = dir0/dir1/dir2/dir3
    //   aName = name.xxx
    //   aSuffix = xxx
    // If aString = dir0/name.xxx
    //   aPath = dir0
    //   aName = name.xxx
    //   aSuffix = xxx
    // If aString = name.xxx
    //   aPath = ""
    //   aName = name.xxx
    //   aSuffix = xxx
    // If aString = /name.xxx
    //   aPath = "/"
    //   aName = name.xxx
    //   aSuffix = xxx
    // If aString = .
    //   aPath = "."
    //   aName = ""
    //   aSuffix = ""
    // If aString = ..
    //   aPath = ".."
    //   aName = ""
    //   aSuffix = ""

    if(aString==".") {
      aPath = ".";
      aName = "";
      aSuffix = "";
      return true;
    } else if(aString=="..") {
      aPath = "..";
      aName = "";
      aSuffix = "";
      return true;
    }
   
    std::string::size_type pos_slash = aString.rfind('/');
    std::string::size_type pos_bslash = aString.rfind('\\');
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
      aPath = "";
      pos = 0;
    } else if(pos==0) {
      aPath = "/";
      pos++;
    } else {
      aPath = aString.substr(0,pos);
      pos++;
    }
    std::string s = aString.substr(pos,aString.size()-pos);
    pos = s.rfind('.');
    if(pos==std::string::npos) {
      aName = s;
      aSuffix = "";
    } else {
      aName = s;
      pos++;
      aSuffix = s.substr(pos,s.size()-pos);
    }
    return true;
  }

public: //For ItemML thing
  static std::string toxml(const std::string& aString) {
    std::string s = aString;
    replace(s,"<","&lt;");
    replace(s,">","&gt;");
    replace(s,"&","&amp;");
    replace(s,"\"","&quot;");
    replace(s,"'","&apos;");
    return s;
  }

  enum what { leading, trailing, both };
  static void strip(std::string& aString,what aType = both,char aChar = ' ') {
    std::string::size_type l = aString.length();;
    if(l==0) return;

    switch ( aType ) {
    case leading:{
      std::string::size_type i;
      char* pos = (char*)aString.c_str();
      for(i=0;i<l;i++,pos++) {
        if(*pos!=aChar) {
          aString = aString.substr(i,l-i);
          return;
        }
      }
      }break;
    case trailing:{
      std::string::size_type i;
      char* pos = (char*)aString.c_str();
      pos += (l-1);
      for(i=l-1;;i--,pos--) {
        if(*pos!=aChar) {
          aString = aString.substr(0,i+1);
          return;
        }
      }
      }break;
    case both:
      Lib::smanip::strip(aString,leading,aChar);
      Lib::smanip::strip(aString,trailing,aChar);
      break;
    //default:break;
    }
  }

  static std::string dirname(const std::string& aPath,
                             unsigned int aNumber = 1) {
    std::string path = aPath;
    for(unsigned int index=0;index<aNumber;index++) {
      std::string p,n,s;
      Lib::smanip::pathNameSuffix(path,p,n,s);
      path = p;
    }
    return path;
  }

  enum side { left, right, middle };
  static bool justify(std::string& aString,unsigned int aSize,
                      side aSide = left) {
    // aSize is the final string length.
    strip(aString);
    if(aSize<=aString.size()) {
      aString.resize(aSize);
      return false;
    } else {
      if(aSide==left) {
        aString = aString + std::string(aSize-aString.size(),' ');
      } else if(aSide==right) {
        aString = std::string(aSize-aString.size(),' ') + aString;
      } else if(aSide==middle) {
        int l = aSize - aString.size();
        int h = l/2;
        if(h*2==l) { //even number of spaces :
          aString = std::string(h,' ') + aString + std::string(h,' ');
        } else { // odd number of spaces :
          aString = std::string(h,' ') + aString + std::string(h+1,' ');
        }
      }
      return false;
    }
  }

};
  
}

#endif
