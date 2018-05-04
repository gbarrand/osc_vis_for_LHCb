// this :
#include <OnXTemplateSix/FrameworkManager.h>

// User :
#include <User/Framework.h>

//////////////////////////////////////////////////////////////////////////////
OnXTemplateSix::FrameworkManager::FrameworkManager(
 User::Framework* aFramework
,bool aStartFromOnX_exe
)
:fName("OnXTemplateSix_FrameworkManager")
,fUserFramework(aFramework)
,fStartFromOnX_exe(aStartFromOnX_exe)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnXTemplateSix::FrameworkManager::~FrameworkManager(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fStartFromOnX_exe) delete fUserFramework;
}
//////////////////////////////////////////////////////////////////////////////
std::string OnXTemplateSix::FrameworkManager::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
void* OnXTemplateSix::FrameworkManager::cast(
 const std::string& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aClass=="OnXTemplateSix::FrameworkManager") {
    return (void*)static_cast<const OnXTemplateSix::FrameworkManager*>(this);
  } else if(aClass=="Slash::Core::IManager") {
    return (void*)static_cast<const Slash::Core::IManager*>(this);
  } else {
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
User::Framework* OnXTemplateSix::FrameworkManager::framework(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fUserFramework;
}
//////////////////////////////////////////////////////////////////////////////
bool OnXTemplateSix::FrameworkManager::startedFromOnX_exe(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStartFromOnX_exe;
}
