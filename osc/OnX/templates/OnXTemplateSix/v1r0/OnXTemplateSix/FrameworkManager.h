#ifndef OnXTemplateSix_FrameworkManager_h
#define OnXTemplateSix_FrameworkManager_h

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Have a OnXTemplateSix::FrameworkManager to acces the user
// framework by using the OnX session.
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Inheritance :
#include <Slash/Core/IManager.h>

// Else :
namespace User {class Framework;}

namespace OnXTemplateSix {

class FrameworkManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const;
  virtual void* cast(const std::string&) const;
public:
  FrameworkManager(User::Framework*,bool);
  virtual ~FrameworkManager();
  User::Framework* framework();
  bool startedFromOnX_exe() const;
private:
  std::string fName;
  User::Framework* fUserFramework;
  bool fStartFromOnX_exe;
};

}

#endif
