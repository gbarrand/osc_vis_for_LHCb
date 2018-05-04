#ifndef OnXTemplateFive_IUserManager_h
#define OnXTemplateFive_IUserManager_h

// Inheritance :
#include <Slash/Core/IManager.h>

namespace OnXTemplateFive {

class IUserManager : public virtual Slash::Core::IManager {
public:
  virtual ~IUserManager() {}
public:
  virtual void userMethod() = 0;
};

}

#endif
