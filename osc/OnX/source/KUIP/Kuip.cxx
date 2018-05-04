
#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IScriptManager.h>
#include <Slash/UI/IInterpreter.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/Manager.h>

#include <Slash/Tools/OpaqueManager.h>

#include <kuip/ksys.h>
#include <kuip/kfor.h>
#include <kuip/kuser.h>
#include <kuip/kout.h> // Specific to KUIP LAL.

extern "C" {
  void OnXKuipInitialize(Slash::Core::ISession&);
  void OnXKuipFinalize(Slash::Core::ISession&);
}

extern "C" {
  void kuinit_(int);
  char* disp_prompt_C(char*,void*);
  int disp_flush_C(void*);
  int disp_exit_C(void*);
}

extern "C" {
  void onx_kuip_def_();
}

static bool KUIP_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);
static void putV(void*,const char*,va_list);

void OnXKuipInitialize(Slash::Core::ISession& aSession) {
  // Executed when dynamic load of OnXKUIP is done.
  // NOTE : if working with a UI, someone has to do a 
  //        ui->executeScript("Session","OnXKUIP set_UI");

  if(!aSession.findManager("KuipManager")) {
    aSession.addManager(new Slash::OpaqueManager("KuipManager"));
  }

  Slash::UI::IScriptManager* scriptManager = 
    Slash_findManager(aSession,"ScriptManager",Slash::UI::IScriptManager);
  if(scriptManager) {
    Slash::UI::IInterpreter* interp = scriptManager->findInterpreter("KUIP");
    if(interp) {
      interp->setFunction(KUIP_Function,0);
    }
  }

  int NWORDS=15000;
  Kuinit(NWORDS);
  ku_exec("/KUIP/SET_SHOW/RECALL_STYLE 'NONE'"); //ifdef WIN32
  ku_exec("/KUIP/SET_SHOW/BREAK OFF");
  
  ku_action_data(&aSession); 

  // Special to KUIP LAL :
  k_set_out_func((koutFunc)putV,&(aSession.out())); 

  //  Should create here a KUIP/LOAD command for dynamic loading
  // KUIP modules (libs containing KUIP commands).
  //
  //  Below should be dynamicly loaded by the upper
  // non-existing KUIP/LOAD command.
  onx_kuip_def_(); 
}

void OnXKuipFinalize(Slash::Core::ISession& aSession) {
  k_set_out_func(0,0);
  aSession.destroyManager("KuipManager");
}


extern "C" {

void OnXKUIP_set_UI(Slash::Core::ISession& aSession) {
  Slash::UI::IUI* ui = Slash::find_UI(aSession);
  if(!ui) {
    std::ostream& out = aSession.cout();
    out << "OnXKUIP_set_UI :"
        << "UI not found."
        << std::endl;       
    return;
  }
  ku_prompt(disp_prompt_C,ui); 
  ku_flush(disp_flush_C,ui); 
  ku_exit(disp_exit_C,ui); 
  ku_quit(disp_exit_C,ui); 

}

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int disp_flush_C(void* aUI){
  Slash::UI::IUI* ui = (Slash::UI::IUI*)aUI;
  ui->synchronize();
  return 0;
}
int disp_exit_C(void* aUI){
  Slash::UI::IUI* ui = (Slash::UI::IUI*)aUI;
  ui->exit();
  return 0;
}

#include <inlib/cstr>

char* disp_prompt_C(char* aPrompt,void* aUI){
  // We should never pass here !
  // KUIP free the return string ; need to allocate one.
  Slash::UI::IUI* ui = (Slash::UI::IUI*)aUI;
  std::string value = ui->ask(aPrompt);
  return inlib::str_dup(value.c_str());
}
//////////////////////////////////////////////////////////////////////////////
bool KUIP_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options&
,const Slash::UI::IInterpreter::Aliases&
,void*
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> text = inlib::words(aString,"\\n\\");
  // Do not use ku_exec to avoid to be prompted 
  // in case of syntax error.
  //ku_exec((char*)aString);
  for(unsigned int index=0;index<text.size();index++) {
    ku_exec_no_help((char*)text[index].c_str());
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void putV(
 void* a_out
,const char* aFormat
,va_list aArgs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  char s[1024];
  vsprintf(s,aFormat,aArgs);
  std::ostream* out = (std::ostream*)a_out;
  (*out) << s;
}
