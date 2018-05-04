
// wrap the below for backward compatiblity.

namespace Lib {

class smanip {
public:
  //NOTE : used in some Panoramix examples.
  static std::string p2sx(void* a_value){
    char s[512];
    ::sprintf(s,"0x%lx",(unsigned long)a_value);
    return s;
  }

  static std::vector<std::string> lines(const std::string& aString) {
    std::vector<std::string> vs;
    smanip_lines(aString,vs); //FIXME : if(!) throw ?
    return vs;
  }

  //NOTE : used in Panoramix.py.
  static std::string torgbs(const std::string& aString) {
    double r,g,b;
    inlib::to_rgb(aString,r,g,b);
    char s[512];
    ::sprintf(s,"%g %g %g",r,g,b);
    return s;
  }

  static std::vector<std::string> pathNameSuffix(const std::string& aPath) {
    std::vector<std::string> vs;
    std::string p,n,s;
    inlib::path_name_suffix(aPath,p,n,s);
    vs.push_back(p);
    vs.push_back(n);
    vs.push_back(s);
    return vs;
  }
};

class fmanip {
public:
  static std::vector<std::string> read(const std::string& aFile) {
    std::vector<std::string> vs;
    std::string file;
    inlib::file_name(aFile,file);
    inlib::file::read(file,vs);
    return vs;
  }
};

}

