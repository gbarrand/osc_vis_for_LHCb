#ifndef Slash_Core_IAction_h
#define Slash_Core_IAction_h

#include <string>
#include <vector>

namespace Slash {

namespace Core {

class IAction {
public:
  virtual ~IAction() {}
  virtual void* cast(const std::string&) const = 0;
public:
  virtual std::string name() const = 0;
  virtual std::string execute(const std::vector<std::string>&) = 0;
};

} //Core

} //Slash

#endif



