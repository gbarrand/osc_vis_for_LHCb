#ifndef BatchLab_XML_ReadContext_h
#define BatchLab_XML_ReadContext_h

#include <string>
#include <ostream>

namespace exlib {namespace xml {class tree;}}

namespace BatchLab {

namespace XML {

class ReadContext {
public:
  inline ReadContext(int aVerboseLevel,
		     std::ostream& a_out,
                     exlib::xml::tree& aItemML,void* aTag)
  :fVerboseLevel(aVerboseLevel)
  ,fOut(a_out)
  ,fItemML(aItemML)
  ,fTag(aTag)
  {}
  virtual ~ReadContext(){}
public:
  inline int verboseLevel() const {return fVerboseLevel;}
  inline std::ostream& out() {return fOut;}
  inline exlib::xml::tree& itemML() const {return fItemML;}
  inline void* tag() const {return fTag;}
private:
  int fVerboseLevel;
  std::ostream& fOut;
  exlib::xml::tree& fItemML;
  void* fTag;
};

} //XML
 
} //BatchLab

#endif
