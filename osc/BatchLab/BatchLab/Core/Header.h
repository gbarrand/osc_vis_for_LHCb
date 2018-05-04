#ifndef BatchLab_Header_h
#define BatchLab_Header_h

// Inheritance :
#include <Slash/Store/IStorable.h>

#include <Slash/Store/IConstVisitor.h>
#include <Slash/Store/IVisitor.h>

#include <BatchLab/Version.h>
#include <AIDA/IConstants.h>

#include <inlib/sys/atime>
#include <inlib/typedefs>

namespace BatchLab {

class Header : public virtual Slash::Store::IStorable {
public: //IVisited
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Store::IStorable)
    else INLIB_IF_CAST(BatchLab::Header)
    else return 0;
  }

  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,Header::s_class(),Header::s_visit)) return false;
  
    int version = 2;
    if(!aVisitor.visit("fVersion",version)) return false;
  
    if(!aVisitor.visit("fPackage",fPackage)) return false;
    if(!aVisitor.visit("fPackageVersion",fPackageVersion)) return false;
    if(!aVisitor.visit("fAIDA_Version",fAIDA_Version)) return false;
    if(!aVisitor.visit("fDate",fDate)) return false;
  
    if(!aVisitor.end(*this)) return false;
  
    return true;
  }

  virtual bool read(Slash::Store::IVisitor& aVisitor) {
    if(!aVisitor.begin(*this)) return false;
    int version;
    if(!aVisitor.visit(version)) return false;  
    if(!aVisitor.visit(fPackage)) return false;
    if(!aVisitor.visit(fPackageVersion)) return false;
    if(!aVisitor.visit(fAIDA_Version)) return false;
    if(version<=1) {
      unsigned int date;
      if(!aVisitor.visit(date)) return false;
    } else {
      if(!aVisitor.visit(fDate)) return false;
    }  
    if(!aVisitor.end(*this)) return false;
    return true;
  }
public:
  Header()
  :fPackage("BatchLab")
  ,fPackageVersion(BATCHLAB_VERSION)
  ,fAIDA_Version(AIDA_VERSION)
  ,fDate(inlib::atime::now().seconds())
  {}
  virtual ~Header(){}
private:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Header");return s_v;
  }
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Header* local = INLIB_CONST_CAST(aObject,BatchLab::Header);
    if(!local) return false;
    return local->Header::visit(aVisitor);
  }
private:
  std::string fPackage;
  std::string fPackageVersion;
  std::string fAIDA_Version;
  inlib::uint64 fDate;
};

}

#endif
