#include <OnX/Core/Term.h>

#include <inlib/strip>
#include <inlib/sprintf>

#define HAS_TERM

#ifdef HAS_TERM
#include <OnX/Core/Getline.h>
#include <cerrno>
#include <cstring>
#endif

OnX::Term::Term(const std::string& aPrompt
)
:fPrompt(aPrompt)
,fCounter(-1)
{
#ifdef HAS_TERM
  OnX_Term_Gl_histinit((char*)"OnX_term.hist");
  set_prompt();
#endif
}

OnX::Term::~Term() {
#ifdef HAS_TERM
  OnX_Term_Gl_erase_line();
  //FIXME : have to reset the cursor to the begin of line and 
  //        erase the prompt. This would avoid to have in 
  //        OpenPAWFinalize an extra 
  //          out << std::endl;
  OnX_Term_Getlinem(2,0);
#endif
}

bool OnX::Term::isValid() const {
#ifdef HAS_TERM
  return true;
#else 
  return false;
#endif
}

void OnX::Term::set_prompt() {
  fCounter++;
  //WARNING : the prompt address is put in the static gl_prmpt !
  //in case fPrompt is a format (for "root[%d]")
  inlib::sprintf(f_prompt,fPrompt.size()+32,fPrompt.c_str(),fCounter);  
  OnX_Term_Getlinem(-1,f_prompt.c_str());
}


std::string OnX::Term::ask(const std::string& aPrompt) {
#ifdef HAS_TERM
  char* p = OnX_Term_Getline(aPrompt.c_str());
  if(!(*p)) {
    //^D
    return "";
  } else {
    std::string cmd(p);
    inlib::strip(cmd,inlib::trailing,'\n');
    return cmd;
  }
#else
  return "";
#endif
}

void OnX::Term::setPrompt(const std::string& aPrompt) {fPrompt = aPrompt;}

std::string OnX::Term::prompt() const {return fPrompt;}

void OnX::Term::initPrompt(const std::string& aPrompt) {
#ifdef HAS_TERM
  OnX_Term_Getlinem(-1,aPrompt.c_str());
#endif
}

bool OnX::Term::getInput(eKind& aWhat,std::string& aInput) {
#ifdef HAS_TERM
  char* p = OnX_Term_Getlinem(1,0);
  if(!p) {
    aInput = "";
    aWhat = no_input;
  } else {
    if(*p) {
      aInput = std::string(p);
      inlib::strip(aInput,inlib::trailing,'\n');
      aWhat = input;
    } else { //^D
      aInput = "";
      aWhat = escape;
    }
  }
  return true;
#else
  aInput = "";
  return false;
#endif
}

bool OnX::Term::is_there_stdin_input(bool& aIs) {
#ifdef HAS_TERM
  int is_stdin;
  if(!OnX_Gl_is_there_stdin_input(&is_stdin)) {
    aIs = false;
    return false;
  }
  aIs = is_stdin?true:false; 
  return true;
#else
  return false;
#endif
}


//////////////////////////////////////////////////////////////////////////////
// related to IUI ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Slash/UI/IUI.h>
#include <Slash/Core/ISession.h>

void OnX::Term::cyclicExecute(Slash::UI::IUI& aUI,const std::string& aInterp) {
#ifdef HAS_TERM
  //printf("debug : term cyclic\n");
  int is_stdin;
  if(!OnX_Gl_is_there_stdin_input(&is_stdin)) {
    std::ostream& out = aUI.session().cout();
    out << "OnX::Term::cyclicExecute :"
        << " problem with select."
        << " errno \"" << std::string(strerror(errno)) << "\"."
        << std::endl;
    return;
  }
  if(!is_stdin) return;
  char* p = OnX_Term_Getlinem(1,0);
  if(!p) return;
  if(*p) {
    std::string cmd(p);
    inlib::strip(cmd,inlib::trailing,'\n');

    OnX_Term_Gl_histadd((char*)cmd.c_str());
    //::printf("|%s|\n",cmd.c_str());

    aUI.executeScript(aInterp,cmd);  //FIXME : ret val.
    set_prompt();
  } else { // ^D
    aUI.executeScript("DLD","OnX ui_exit"); //FIXME : ret val.
    set_prompt();
  }
#endif
}

std::string OnX::Term::ask(Slash::UI::IUI& aUI,const std::string& aPrompt) {
#ifdef HAS_TERM
  //return Term::ask(aPrompt);
  //printf("debug : ask : begin 6+\n");
  initPrompt(aPrompt);
  while(true) {    
    aUI.synchronize();
    bool is_stdin;
    if(!is_there_stdin_input(is_stdin)) {
      std::ostream& out = aUI.session().cout();
      out << "OnX::TermCyclic::ask :"
          << " problem with stdin select."
          << " errno \"" << std::string(strerror(errno)) << "\"."
          << std::endl;
      return "";
    }
    if(is_stdin) {
      eKind kind;
      std::string cmd;
      if(!getInput(kind,cmd)) {
        std::ostream& out = aUI.session().cout();
        out << "OnX::TermCyclic::ask :"
            << " problem to get input."
            << std::endl;
        return "";
      }
      if(kind==input) {
        //initPrompt(fPrompt);
        return cmd;
      } else if(kind==escape) {
        //initPrompt(fPrompt);
        //return cmd;
      } //no_input, continue.
    }
  }
#else
  return "";
#endif
}
