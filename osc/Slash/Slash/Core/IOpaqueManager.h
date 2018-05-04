#ifndef Slash_Core_IOpaqueManager_h
#define Slash_Core_IOpaqueManager_h

#include <string>

namespace Slash {

namespace Core {

class IOpaqueManager {
public:
  virtual ~IOpaqueManager() {};
public:
  virtual void* find(const std::string&) const = 0;
  virtual bool add(void* aObject,const std::string& aID,bool aOwner = false) = 0;
  virtual bool isOwner(const std::string&) const = 0;
  virtual bool destroy(const std::string&) = 0;
};

} //Core

} //Slash

#endif
