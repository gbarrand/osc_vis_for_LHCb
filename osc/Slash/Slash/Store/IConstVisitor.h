#ifndef Slash_Store_IConstVisitor_h
#define Slash_Store_IConstVisitor_h

#include <string>
#include <vector>
#include <Slash/Core/typedefs.h>

namespace Slash {namespace Store { class IArray;}}
namespace Slash {namespace Store { class IStorable;}}

namespace Slash {

namespace Store {

class IConstVisitor {
public:
  virtual ~IConstVisitor() {}
public:
  typedef bool(*Local)(const Slash::Store::IStorable&,
                       Slash::Store::IConstVisitor&);
public:
  virtual bool begin(const IStorable&,const std::string&,Local) = 0;
  virtual bool end(const IStorable&) = 0;

  virtual bool visit(const std::string&,bool) = 0;

  virtual bool visit(const std::string&,char) = 0;
  //virtual bool visit(const std::string&,unsigned char) = 0;

  virtual bool visit(const std::string&,short) = 0;
  //virtual bool visit(const std::string&,unsigned short) = 0;

  virtual bool visit(const std::string&,int) = 0;
  virtual bool visit(const std::string&,unsigned int) = 0;

  virtual bool visit(const std::string&,int64) = 0;
  virtual bool visit(const std::string&,uint64) = 0;

  virtual bool visit(const std::string&,float) = 0;
  virtual bool visit(const std::string&,double) = 0;

  virtual bool visit(const std::string&,const std::string&) = 0;
  virtual bool visit(const std::string&,const char*) = 0;
  virtual bool visit(const std::string&,const std::vector<bool>&) = 0;
  virtual bool visit(const std::string&,const std::vector<char>&) = 0;
  virtual bool visit(const std::string&,const std::vector<short>&) = 0;
  virtual bool visit(const std::string&,const std::vector<int>&) = 0;
  virtual bool visit(const std::string&,const std::vector<int64>&) = 0;
  virtual bool visit(const std::string&,const std::vector<float>&) = 0;
  virtual bool visit(const std::string&,const std::vector<double>&) = 0;
  virtual bool visit(const std::string&,const std::vector<unsigned char>&) = 0;
  virtual bool visit(const std::string&,const std::vector<std::string>&) = 0;
  virtual bool visit(const std::string&,const std::vector< std::vector<double> >&) = 0;
  virtual bool visit_double(const std::string&,const IArray&) = 0;

  virtual bool visit(const std::string&,const IStorable&) = 0;
};

} //Store

} //Slash


#endif
