#ifdef WIN32
// Clash with <list> ; include tcl.h first :
#include <tcl.h>
#endif

#include <OnX/SWIG/SWIG.h>

// GET_SESSION no more used for Tcl.
// In OnXTcl/Tcl.cxx we define the OnX_sessionPointer
// Tcl command that returns on a string the 
// session address. In OnX/scripts/Tcl/init.tcl, we
// reconvert it to a SWIG wrapped pointer 
// by using the wrapped OnX.i OnX_cast_Slash::Core::ISession method.

//Slash::Core::ISession* GET_SESSION() { return 0;} //No more used for OnX/Tcl.

#include <OnX_SWIG_Tcl.ic>
