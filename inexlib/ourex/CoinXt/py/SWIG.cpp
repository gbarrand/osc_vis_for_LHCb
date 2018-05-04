//
//WARNING : do not put the below in the SWIG.i file
//  in some %{... %}. Else it is going to be after Python.h
//  and then compilation problem may happen on some platform.
//  (For example clash with Coin-2.x code on a slc3_gcc323).
//

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>

#include <CoinPython/Inventor.h>

#if defined(__linux)
// Clash between os_defines.h (coming from <string>) and pyconfig.h
#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif
#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif
#endif

#include <Python.h>

#ifdef WIN32
#include <windows.h>
#undef max
#undef ERROR
#undef DELETE
#endif

//#undef ANY

//If wrapping full Inventor too.
typedef SoGLLazyElement::GLState GLState;

#include "SWIG.ic"
