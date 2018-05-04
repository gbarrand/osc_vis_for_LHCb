// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file exlib.license for terms.

#include "../swig/inlib_i"

#ifdef EXLIB_USE_NATIVE_GLUTESS
#include <GL/gl.h>
#include <GL/glu.h>
typedef GLenum GLUenum;
#else
#include <glutess/ourex_glutess.h>
#endif

#ifdef EXLIB_USE_NATIVE_FREETYPE2
#include <ft2build.h>
#else
#include <ourex_ft2build.h>
#endif
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include "../sg/write_gl2ps"

#include <inlib/sg/infos>

#ifdef WIN32
#include "../win32/plotter"
#define EXLIB_SWIG_XANY win32
#else
#include "../X11/plotter"
#define EXLIB_SWIG_XANY X11
#endif

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

#include "exlib_swig_py.icc"
