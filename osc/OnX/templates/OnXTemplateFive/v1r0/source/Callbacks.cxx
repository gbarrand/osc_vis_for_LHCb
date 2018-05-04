//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Manager.h>

// OnXTemplateFive :
#include <OnXTemplateFive/Interfaces/IUserManager.h>

namespace OnXTemplateFive {

class UserManager : public virtual IUserManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& aClass) const { //Safe cast.
    if(aClass=="OnXTemplateFive::UserManager") {
      return (void*)static_cast<const OnXTemplateFive::UserManager*>(this);
    } else if(aClass=="Slash::Core::IManager") {
      return (void*)static_cast<const Slash::Core::IManager*>(this);
    } else if(aClass=="OnXTemplateFive::IUserManager") {
      return (void*)static_cast<const OnXTemplateFive::IUserManager*>(this);
    } else {
      return 0;
    }
  }
public: //IUserManager
  virtual void userMethod() {
    // Our method :
    std::ostream& out = fSession.cout();
    out << "From OnXTemplateFive::UserManager::userMethod() !" << std::endl;
  }
public:
  UserManager(Slash::Core::ISession& aSession)
  :fSession(aSession),fName("OnXTemplateFive::UserManager"){}
  virtual ~UserManager(){}
private:
  Slash::Core::ISession& fSession;
  std::string fName; //Name of our manager.
};

}

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnXTemplateFive_initialize(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
// Should be executed once. 
// Here done on the create callback of the :
//   scripts/OnX/OnXTemplateFive.onx 
// file loaded by onx.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession& session = aUI.session();

  OnXTemplateFive::UserManager* manager = new OnXTemplateFive::UserManager(session);
  session.addManager(manager);
}
//////////////////////////////////////////////////////////////////////////////
void OnXTemplateFive_userMethod(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
// Executed from "File/User manager method" menu item.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Find our manager in the session.
  OnXTemplateFive::IUserManager* userManager = 
    Slash_findManager(aUI.session(),
      "OnXTemplateFive::UserManager",OnXTemplateFive::IUserManager);
  if(!userManager) {
    aUI.echo("OnXTemplateFive::UserManager not found.");
    return;
  }

  // Call our manager method :
  userManager->userMethod();
}

} // extern "C"
