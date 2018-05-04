#ifndef Slash_Core_IMessage_h
#define Slash_Core_IMessage_h

#include <string>

namespace Slash {

namespace Core {

class IMessage {
public:
  virtual ~IMessage() {}
  virtual void* cast(const std::string&) const = 0;
public:
  virtual std::string message() const = 0;
};

} //Core

} //Slash

#endif
