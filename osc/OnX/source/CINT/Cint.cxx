#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>
#include <Slash/UI/IScriptManager.h>
#include <Slash/UI/IInterpreter.h>

#include <Lib/fmanip.h>
#include <Lib/smanip.h>
#include <Lib/Out.h>
#include <Slash/Tools/Manager.h>

extern "C" {
void OnXCintInitialize(Slash::Core::ISession&);
void OnXCintFinalize(Slash::Core::ISession&);
Slash::Core::ISession& OnXCintGetSession();
}

// CINT :
#include <G__ci.h>

extern "C" {
  //extern void G__setothermain(int othermain);
  //extern int G__main(int argc,char **argv);
extern void G__set_p2fsetup(void (*p2f)());
extern void G__free_p2fsetup();
extern G__value G__exec_tempfile(char*);
extern void G__cpp_setupOnXCintWrap();
}

static bool cint_Function(const std::string&,void*);

static Slash::Core::ISession* sSession = 0; // Beurk.
//////////////////////////////////////////////////////////////////////////////
Slash::Core::ISession& OnXCintGetSession(
) 
//////////////////////////////////////////////////////////////////////////////
// So that we can find the session from a CINT script.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *sSession;
}
//////////////////////////////////////////////////////////////////////////////
void OnXCintInitialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
// Executed, by Interpreters, at first cint>... command
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  sSession = &aSession;

  Slash::UI::IScriptManager* scriptManager = 
    Slash_findManager(aSession,"ScriptManager",Slash::UI::IScriptManager);
  if(scriptManager) {
    Slash::UI::IInterpreter* interp = scriptManager->findInterpreter("CINT");
    if(interp) interp->setFunction(cint_Function,&aSession);
  }

  // CINT :

  // Wrapped compiled classes :
  // Should be dynamically loaded.
  G__set_p2fsetup(G__cpp_setupOnXCintWrap);

  G__init_cint((char*)"cint");

}
//////////////////////////////////////////////////////////////////////////////
void OnXCintFinalize(
 Slash::Core::ISession&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // CINT cleanup :
  G__free_p2fsetup();
  G__scratch_all();
  sSession = 0;
}
//////////////////////////////////////////////////////////////////////////////
bool cint_Function(
 const std::string& aString
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aTag) return false;
  Slash::Core::ISession* session = (Slash::Core::ISession*)aTag;
  // Parse some meta commands : 
  //  x <file>
  std::string cmd = aString;
  Lib::smanip::strip(cmd);
  Lib::smanip::replace(cmd,"\\n\\","");
  if( (cmd.length()>=2) && (cmd[0]=='x') & (cmd[1]==' ') ) {
    std::string fname = cmd.substr(2,cmd.length()-2);
    Lib::smanip::strip(fname);
    // To work on the given name to resolve env variables, etc.
    std::string name = Lib::fmanip::name(fname);
    G__exec_tempfile((char*)name.c_str());
  } else {
    //? G__loadfile();
    //? G__calc((char*)aString);
    std::string name;
    FILE* file = 0;
    if(Lib::fmanip::tmpname(name,".","OnX_",".cint") && 
       (file = fopen(name.c_str(),"wb+")) ) {
      fprintf(file,"%s",cmd.c_str());
      fclose(file);
      G__exec_tempfile((char*)name.c_str());
      Lib::fmanip::remove(name);
    } else {
      Lib::Out out(session->printer());
      out << "Can't open a temporary file." << Lib::endl;
    }
  }
  return true;
}
/*
//////////////////////////////////////////////////////////////////////////////
void OnXCintExecFile(
 const char* aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aName) return;
  // To work on the given name to resolve env variables, etc.
  Lib::File file(aName,"READ",sSession->printer());
  Lib::String name = file.getName(); 
  file.close();
  G__exec_tempfile((char)name.c_str());
}
*/




