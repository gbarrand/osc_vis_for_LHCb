#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/ILibraryManager.h>
#include <Slash/Core/ILibrary.h>
#include <Slash/Tools/Manager.h>

#include <inlib/cast>
#include <inlib/sprintf>

#include <Slash/UI/IScriptManager.h>
#include <Slash/UI/IInterpreter.h>

extern "C" {
  void OnXTclInitialize(Slash::Core::ISession&);
  void OnXTclFinalize(Slash::Core::ISession&);
}

#include <tcl.h>

static int sessionPointer(ClientData,Tcl_Interp*,int,struct Tcl_Obj* CONST[]); 

static bool tcl_Function(const std::string&,void*);

namespace OnX {

class TclManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnX::TclManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  TclManager(Slash::Core::ISession& aSession)
  :fName("TclManager"),fSession(aSession),fInterp(0) {
    //WIN32 : TclEval("source <file>") crashs without the below.
    std::string binary;
    fSession.parameterValue("session.binary",binary);
    Tcl_FindExecutable((char*)binary.c_str());
    fInterp = Tcl_CreateInterp();
  }
  virtual ~TclManager() { 
    Tcl_DeleteInterp(fInterp);
  }
  Tcl_Interp* tclInterp() const { return fInterp;}
  std::ostream& out() const { return fSession.out();}
private:
  std::string fName;
  Slash::Core::ISession& fSession;
  Tcl_Interp* fInterp;
};

}

//////////////////////////////////////////////////////////////////////////////
void OnXTclInitialize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
// Executed, by Interpreters, at first tcl>... command
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  OnX::TclManager* tclManager =
    Slash_findManager(aSession,"OnX::TclManager",OnX::TclManager);
  if(!tclManager) {
    tclManager = new OnX::TclManager(aSession);
    aSession.addManager(tclManager);
  }

  Slash::UI::IScriptManager* scriptManager = 
    Slash_findManager(aSession,"ScriptManager",Slash::UI::IScriptManager);
  if(scriptManager) {
    Slash::UI::IInterpreter* interp = scriptManager->findInterpreter("Tcl");
    if(interp) interp->setFunction(tcl_Function,tclManager);
  }

  Tcl_Interp* tclInterp = tclManager->tclInterp();

  // The OnX_sessionPointer command returns on a string the 
  // session address. In the OnX/scripts/Tcl/init.tcl script
  // we use the SWIG wrapped OnX_cast_ISession (see OnX.i) to
  // convert the string to a SWIG Slash::Core::ISession pointer.
  Tcl_CreateObjCommand(tclInterp,(char*)"OnX_sessionPointer",
                       sessionPointer,&aSession,0);
}
//////////////////////////////////////////////////////////////////////////////
void OnXTclFinalize(
 Slash::Core::ISession& aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aSession.destroyManager("TclManager");
}
//////////////////////////////////////////////////////////////////////////////
bool tcl_Function(
 const std::string& aString
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aTag) return false;
  OnX::TclManager* tclManager = (OnX::TclManager*)aTag;
  if(Tcl_Eval(tclManager->tclInterp(),(char*)aString.c_str())==TCL_ERROR) {
    char* result = (char*)Tcl_GetStringResult(tclManager->tclInterp());
    tclManager->out() << (result?result:"") << std::endl;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int sessionPointer(
 ClientData aClientData
,Tcl_Interp* aInterp
,int //aArgc
,struct Tcl_Obj* CONST /*aArgv*/[]
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Tcl_Obj* tcl_result = Tcl_GetObjResult(aInterp);

  Slash::Core::ISession* session = (Slash::Core::ISession*)aClientData;
  if(!session) {
    Tcl_SetStringObj(tcl_result,(char*)"NULL",-1);
    return TCL_ERROR;
  }

  // The format (%lx) must be the same than the 
  // OnX.i/OnX_cast_ISession function.
  std::string s;
  inlib::sprintf(s,64,"%lx",session);
  Tcl_SetStringObj(tcl_result,s.c_str(),-1);

  return TCL_OK;
}
