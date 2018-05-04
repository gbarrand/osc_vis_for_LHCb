
#include <tcl.h>

int main(int argc,char* argv[]) {
  Tcl_Interp* tclInterp = 0;
  //WIN32 : TclEval("source <file>") crashs without the below.
  Tcl_FindExecutable(argv[0]);
  tclInterp = Tcl_CreateInterp();
  //if(Tcl_Eval(tclInterp,"source init.tcl")==TCL_ERROR) {
  //if(Tcl_Eval(tclInterp,"puts hello")==TCL_ERROR) {
  if(Tcl_Eval(tclInterp,argc>1?argv[1]:"puts hello")==TCL_ERROR) {
    char* result = (char*)Tcl_GetStringResult(tclInterp);
    printf("%s\n",result);
  }
  return 0;
}

