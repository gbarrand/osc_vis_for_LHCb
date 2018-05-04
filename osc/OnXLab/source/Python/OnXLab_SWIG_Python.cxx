
#include <OnXLab/SWIG/SWIG.h>

#if defined(__linux)
// Clash between os_defines.h (coming from <string>) and pyconfig.h
#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif
#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif
#endif

#include <stddef.h> //ptrdiff_t
#include <Python.h>

#include "OnXLab_SWIG_Python.ic"
