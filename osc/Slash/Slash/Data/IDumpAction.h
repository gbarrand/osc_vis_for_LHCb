#ifndef Slash_Data_IDumpAction_h
#define Slash_Data_IDumpAction_h

#include <string>
#include <vector>
#include <ostream>

namespace Slash {
namespace Data {

class IDumpAction {
public:
  virtual ~IDumpAction() {}
public:
  virtual bool execute(const std::vector<std::string>&,std::ostream&) = 0;
};

}}

#endif
