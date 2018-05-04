// Dummy file to help building the onx-config system for Python.

#include <Slash/UI/IUI.h>

//#include <Lib/Printer.h>

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

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_config_python(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Put some code in order to be sure to build correctly.
  //Lib::Printer printer;

  std::string s;
  PyRun_SimpleString((char*)s.c_str());
}

}
