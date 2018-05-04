
#include <Slash/Core/ISession.h>

#include <Slash/UI/IScriptManager.h>
#include <Slash/Tools/Managers.h>

#include <inlib/args>

#include <iostream>

int main(int argc,char** argv){

  inlib::args args(argc,argv);
  std::string interp;
  std::string script;
  if(argc==1) {
    // default action does a "Session OnX_session_echo hello"
    // in order that execution from check_install does something.
    interp = "Session";
    script = "OnX session_echo hello";
  } else if(argc==3) {
    interp = argv[1];
    script = argv[2];
    args.remove(interp);
    args.remove(script);
  } else {
    std::cout << "Usage :" << std::endl;
    std::cout << " OS> <exe> <interp> <script>" << std::endl;
    std::cout << "Examples :" << std::endl;
    std::cout << "   OS> <exe> Session \"OnX session_echo hello\"" << std::endl;
    std::cout << "   OS> <exe> Python \"import hello\"" << std::endl;
    return 0;
  }

  Slash::Core::ISession* session = Slash_create_session(args.tovector());
  if(!session) {
    std::cout << "OnX_obatch :"
              << " session not found." << std::endl;
    return EXIT_FAILURE;
  }

  // No UI needed then no UI created !

  Slash::UI::IScriptManager* sm = Slash::scriptManager(*session);
  if(!sm) {
    std::cout << "OnX_obatch :"
              << " ScriptManager not found." 
              << std::endl;
    delete session;
    return 0;
  }

  Slash::UI::IInterpreter::Options opts;
  sm->executeScript(interp,script,opts);

  delete session;

  return 0;
}


// to check compilation from examples/Callback/Included.cxx
static void obatch(Slash::Core::ISession&,std::ostream&,const std::string&,bool) {}
