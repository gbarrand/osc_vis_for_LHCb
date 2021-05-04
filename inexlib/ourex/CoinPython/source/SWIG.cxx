//
//WARNING : do not put the below in the SWIG.i file
//  in some %{... %}. Else it is going to be after Python.h
//  and then compilation problem may happen on some platform.
//  (For example clash with Coin-2.x code on a slc3_gcc323).
//

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

#ifdef _WIN32
#include <windows.h>
#undef max
#undef ERROR
#undef DELETE
#endif

//#undef ANY

typedef SoGLLazyElement::GLState GLState;

#if PY_VERSION_HEX >= 0x03000000
inline int PyString_AsStringAndSize(PyObject* obj,char** s,int* len) {
  Py_ssize_t llen;
  int status = PyBytes_AsStringAndSize(obj,s,&llen);
  *len = (int)llen;
  return status;
}
inline PyObject* PyString_FromStringAndSize(const char* a_buffer,Py_ssize_t a_size) {
#if PY_VERSION_HEX >= 0x03010000
  return PyUnicode_DecodeUTF8(a_buffer, static_cast< int >(a_size), "surrogateescape");
#else
  return PyUnicode_FromStringAndSize(a_buffer, static_cast< int >(a_size));
#endif
}
//FIXME : what is PyFile_Check and PyFile_AsFile for Python3 ?
#define PyFile_Check(op) false
#define PyFile_AsFile(op) 0
#else
inline int PyString_AsStringAndSize(PyObject* obj,char** s,int* len) {
  Py_ssize_t llen;
  int status = PyString_AsStringAndSize(obj,s,&llen);
  *len = (int)llen;
  return status;
}
#endif

#include "SWIG.ic"
