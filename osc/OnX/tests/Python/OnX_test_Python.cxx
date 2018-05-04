
#include <string> // KCC : else Python.h does not compile !

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

//////////////////////////////////////////////////////////////////////////////
int main (
 int
,char**
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // O'Reilly p 578 :
  Py_Initialize();
  PyRun_SimpleString((char*)"import string");
  PyRun_SimpleString((char*)"print string.uppercase");
  PyRun_SimpleString((char*)"x = string.uppercase");
  PyRun_SimpleString((char*)"print string.lower(x)");

  PyRun_SimpleString((char*)"import OnX");
  //PyRun_SimpleString((char*)"OnX_SWIG_Hello()");
  PyRun_SimpleString((char*)"import math");
  PyRun_SimpleString((char*)"from math import cos");
  PyRun_SimpleString((char*)"print cos(0)");

  return 0;
}
