//
//  All functions here should be Session callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::Core::ISession&);
//     void callback_with_arguments(Slash::Core::ISession&,const std::vector<std::string>&);
//   }
//

#include <Slash/Core/ISession.h>
#include <Slash/UI/IInterpreter.h>
#include <Slash/Tools/Managers.h>

#include <inlib/file>
#include <inlib/system>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_session_echo(
 Slash::Core::ISession& aSession
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = aSession.cout();
  if(!aArgs.size()) {
    out << std::endl;
  } else {
    out << aArgs[0];
    for(unsigned int index=1;index<aArgs.size();index++) {
      out << " ";
      out << aArgs[index];
    }
    out << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
/// INTERNAL /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnX_session_exec_file(
 Slash::Core::ISession& aSession
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// aArgs[0] file
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aArgs.size()!=1) return;
  Slash::UI::IScriptManager* sm = Slash::scriptManager(aSession);
  if(!sm) return;
  Slash::UI::IInterpreter* interp = sm->findInterpreter("Session");
  if(!interp) return;
  std::vector<std::string> text;
  std::string name;
  inlib::file_name(aArgs[0],name);
  if(!inlib::file::read(name,text)) {
    std::ostream& out = aSession.cout();
    out << "session_exec_file : "
        << " problem reading file \"" << aArgs[0] << "\"."
        << std::endl;
    return;
  }
  Slash::UI::IInterpreter::Options params;
  for(std::vector<std::string>::iterator it=text.begin();it!=text.end();++it) {
    interp->executeScript(*it,params);
  } 
}

}
