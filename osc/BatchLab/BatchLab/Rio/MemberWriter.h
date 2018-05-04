#ifndef BatchLab_Rio_MemberWriter_h
#define BatchLab_Rio_MemberWriter_h

// Inheritance :
#include <Slash/Store/IConstVisitor.h>

namespace Rio {class IBuffer;}

namespace BatchLab {

namespace Rio {

class MemberWriter : public virtual Slash::Store::IConstVisitor {
public: //Slash::Store::IConstVisitor
  virtual bool begin(const Slash::Store::IStorable&,
                     const std::string&,
                     Slash::Store::IConstVisitor::Local);
  virtual bool end(const Slash::Store::IStorable&);

  virtual bool visit(const std::string&,bool);
  virtual bool visit(const std::string&,char);
  virtual bool visit(const std::string&,short);
  virtual bool visit(const std::string&,int);
  virtual bool visit(const std::string&,unsigned int);
  virtual bool visit(const std::string&,Slash::int64);
  virtual bool visit(const std::string&,Slash::uint64);
  virtual bool visit(const std::string&,float);
  virtual bool visit(const std::string&,double);
  virtual bool visit(const std::string&,const std::string&);
  virtual bool visit(const std::string&,const char*);
  virtual bool visit(const std::string&,const std::vector<bool>&);
  virtual bool visit(const std::string&,const std::vector<char>&);
  virtual bool visit(const std::string&,const std::vector<short>&);
  virtual bool visit(const std::string&,const std::vector<int>&);
  virtual bool visit(const std::string&,const std::vector<Slash::int64>&);
  virtual bool visit(const std::string&,const std::vector<float>&);
  virtual bool visit(const std::string&,const std::vector<double>&);
  virtual bool visit(const std::string&,const std::vector<unsigned char>&);
  virtual bool visit(const std::string&,const std::vector<std::string>&);
  virtual bool visit(const std::string&,const std::vector< std::vector<double> >&);
  virtual bool visit_double(const std::string&,const Slash::Store::IArray&);
  virtual bool visit(const std::string&,const Slash::Store::IStorable&);
public:
  MemberWriter(::Rio::IBuffer&);
  virtual ~MemberWriter();  
private:
  ::Rio::IBuffer& fBuffer;
};

}}

#endif
