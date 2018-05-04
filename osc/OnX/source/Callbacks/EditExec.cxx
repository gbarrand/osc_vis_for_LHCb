//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

//
//  This file does not need HEPVis and Inventor to compile.
//

#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>

#include <Slash/Tools/Managers.h>

#include <inlib/file>
#include <inlib/system>
#include <inlib/sys/file>
#include <inlib/srep>

inline std::string interpreter_from_suffix(
 Slash::Core::ISession& aSession
,const std::string& aSuffix
){
  Slash::UI::IScriptManager* sm = Slash::scriptManager(aSession);
  if(!sm) return "";
  Slash::UI::IInterpreter* interp = sm->findInterpreterFromSuffix(aSuffix);
  if(!interp) return "";
  return interp->name();
}


extern "C" {

/////////////////////////////////////////////////////////////////////////////
void OnX_EditExec_setFile(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aArgs.size()!=1) return;

  std::string file = aArgs[0];

  std::vector<std::string> text;

  std::string name;
  inlib::file_name(file,name);
  if(!inlib::file::read(name,text)) return;
  aUI.setParameter
    ("OnX_EditExec_text.value",inlib::tos<std::string>(text,"\n"));
  // Set interpreter :
  std::string suffix = inlib::suffix(file);
  std::string interp = interpreter_from_suffix(aUI.session(),suffix);
  if(interp.empty()) interp = "DLD";
  aUI.setParameter("OnX_EditExec_interpreter.default",interp);

  // Save the name of the file on some session parameter :
  aUI.session().setParameter("OnX_EditExec.file",file);
}
/////////////////////////////////////////////////////////////////////////////
void OnX_EditExec_ok(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string svalue;
  if(!aUI.parameterValue("OnX_EditExec_text.value",svalue)) return;
  std::string sinterp;
  if(!aUI.parameterValue("OnX_EditExec_interpreter.value",sinterp)) return;

  Slash::UI::IScriptManager* sm = Slash::scriptManager(aUI.session());
  if(!sm) return;
  Slash::UI::IInterpreter* interp = sm->findInterpreter(sinterp);
  if(!interp) return;
  std::string ssuf = interp->suffix();
  std::string scmd = interp->execFileCommand();
  if(!scmd.size()) {
    std::ostream& out = aUI.session().cout();
    out << "OnX_EditExec_ok :"
        << " no file command given for " << inlib::sout(sinterp) << "." 
        << std::endl;
    return;
  }

  // Save text in a temporary file :
  std::string name;
  if(inlib::tmpname(".","OnX_",ssuf.size()?("."+ssuf):"",name)) {
    if(inlib::file::write(name,svalue)) {

      std::string file;
      inlib::file_name(name,file);
      inlib::replace(scmd,"@onx_file@",file);

      sm->record(sinterp,svalue); //history
      Slash::UI::IInterpreter::Options params;
      interp->executeScript(scmd,params);

      ::remove(file.c_str());
    }
  } else {
    std::ostream& out = aUI.session().cout();
    out << "OnX_EditExec_ok : can't open a temporary file." << std::endl;
  }
}

} // extern "C"
