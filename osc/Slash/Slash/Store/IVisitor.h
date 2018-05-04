#ifndef Slash_Store_IVisitor_h
#define Slash_Store_IVisitor_h

#include <string>
#include <vector>
#include <Slash/Core/typedefs.h>

namespace Slash {namespace Core { class ISession;}}

namespace Slash {namespace Store { class IArray;}}
namespace Slash {namespace Store { class IStorable;}}

namespace Slash {

namespace Store {

class IVisitor {
public:
  virtual ~IVisitor() {}
public:
  virtual bool begin(IStorable&) = 0;
  virtual bool end(IStorable&) = 0;
  virtual bool visit(bool&) = 0;
  virtual bool visit(char&) = 0;
  virtual bool visit(short&) = 0;
  virtual bool visit(int&) = 0;
  virtual bool visit(unsigned int&) = 0;
  virtual bool visit(int64&) = 0;
  virtual bool visit(uint64&) = 0;
  virtual bool visit(float&) = 0;
  virtual bool visit(double&) = 0;
  virtual bool visit(std::string&) = 0;
  virtual bool visit(std::vector<bool>&) = 0;
  virtual bool visit(std::vector<char>&) = 0;
  virtual bool visit(std::vector<short>&) = 0;
  virtual bool visit(std::vector<int>&) = 0;
  virtual bool visit(std::vector<int64>&) = 0;
  virtual bool visit(std::vector<float>&) = 0;
  virtual bool visit(std::vector<double>&) = 0;
  virtual bool visit(std::vector<unsigned char>&) = 0;
  virtual bool visit(std::vector<std::string>&) = 0;
  virtual bool visit(std::vector< std::vector<double> >&) = 0;
  virtual bool visit_double(IArray&) = 0;  

  /*
   *  In readers, recreating objects need oftenly
   * to access the whole context, then we give an
   * access to the ISession here.
   *  Used by BatchLab::Function::Compiled::read.
   *
   * @return The session.
   */
  virtual Core::ISession& session() = 0;
};

} //Store

} //Slash


#endif
