#ifndef BatchLab_XML_WriteContext_h
#define BatchLab_XML_WriteContext_h

#include <string>
#include <ostream>

namespace BatchLab {

namespace XML {

class WriteContext {
public:
  inline WriteContext(std::ostream& a_out,std::ostream& a_writer,
               const std::string& aPath,const std::string& aName,
               int aShift = 0)
  :f_out(a_out)
  ,f_writer(a_writer)
  ,fPath(aPath)
  ,fName(aName)
  ,fShift(aShift)
  {}
  virtual ~WriteContext(){}
public:
  inline std::ostream& out() const {return f_out;}
  inline std::ostream& writer() const {return f_writer;}
  inline const std::string& path() const {return fPath;}
  inline const std::string& name() const {return fName;}
  inline int shift() const {return fShift;}
private:
  std::ostream& f_out;
  std::ostream& f_writer;
  std::string fPath;
  std::string fName;
  int fShift;
};

} //XML

} //BatchLab

#endif
