#ifndef BatchLab_Rio_MemberReader_h
#define BatchLab_Rio_MemberReader_h

// Inheritance :
#include <Slash/Store/IVisitor.h>

namespace Slash {namespace Core { class ISession;}}

namespace Rio {class IBuffer;}

namespace BatchLab {

namespace Rio {

class MemberReader : public virtual Slash::Store::IVisitor {
public: //Slash::Store::IVisitor
  virtual bool begin(Slash::Store::IStorable&);
  virtual bool end(Slash::Store::IStorable&);
  virtual bool visit(bool&);
  virtual bool visit(char&);
  virtual bool visit(short&);
  virtual bool visit(int&);
  virtual bool visit(unsigned int&);
  virtual bool visit(Slash::int64&);
  virtual bool visit(Slash::uint64&);
  virtual bool visit(float&);
  virtual bool visit(double&);
  virtual bool visit(std::string&);
  virtual bool visit(std::vector<bool>&);
  virtual bool visit(std::vector<char>&);
  virtual bool visit(std::vector<short>&);
  virtual bool visit(std::vector<int>&);
  virtual bool visit(std::vector<Slash::int64>&);
  virtual bool visit(std::vector<float>&);
  virtual bool visit(std::vector<double>&);
  virtual bool visit(std::vector<unsigned char>&);
  virtual bool visit(std::vector<std::string>&);
  virtual bool visit(std::vector< std::vector<double> >&);
  virtual bool visit_double(Slash::Store::IArray&);
  virtual Slash::Core::ISession& session();
public:
  MemberReader(Slash::Core::ISession&,::Rio::IBuffer&);
  virtual ~MemberReader();  
private:
  Slash::Core::ISession& fSession;
  ::Rio::IBuffer& fBuffer;
};

}}

#endif
