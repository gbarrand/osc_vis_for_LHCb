/* include/Inventor/C/basic.h.  Generated by configure.  */
/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_BASIC_H
#define COIN_BASIC_H

/*
  NOTE: basic.h is automatically generated from basic.h.in, so don't
  edit basic.h directly.
*/

/* *********************************************************************** */

/* Documented for Doxygen in SoDB.cpp. */
typedef int SbBool;

#ifndef FALSE
#define FALSE 0
#endif /* !FALSE */
#ifndef TRUE
#define TRUE  1
#endif /* !TRUE */

/* *********************************************************************** */

/* Ye good olde min/max macros. No library would be complete without them. */

#define cc_min(x, y) (((x) < (y)) ? (x) : (y))
#define cc_max(x, y) (((x) > (y)) ? (x) : (y))

/* *********************************************************************** */

/* Include this to 1) be compatible with Open Inventor's SbBasic.h, 2)
 * provide a way for application programmers to get hold of the type
 * definitions without explicitly including inttypes.h.
 *
 * The latter point is important because the inttypes.h file in SGI
 * and TGS Open Inventor is placed _below_ the Inventor/ directory in
 * the header files hierarchy. This is a stupid thing to do because it
 * could easily conflict with the inttypes.h file installed as part of
 * the C library on many systems (like GNU/Linux's glibc, for
 * instance).
 */
#include <Inventor/system/inttypes.h>

/* Internal note for Coin developers: in other sourcefiles in Coin, we
 * depend on math.h being included through SbBasic.h. It is done this
 * way to always make sure we have the M_* symbols available under
 * MSWin (see below). So don't remove the following line unless you
 * know very well what you are doing.
 */
#include <math.h>

/* Define misc values if they are not available from math.h. UNIX
 * systems typically have these defines, and MSWindows systems don't.
 */
#ifndef M_E
#define M_E 2.7182818284590452354
#endif /* !M_E */
#ifndef M_LOG2E
#define M_LOG2E 1.4426950408889634074
#endif /* !M_LOG2E */
#ifndef M_LOG10E
#define M_LOG10E 0.43429448190325182765
#endif /* !M_LOG10E */
#ifndef M_LN2
#define M_LN2 0.69314718055994530942
#endif /* !M_LN2 */
#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif /* !M_LN10 */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* !M_PI */
#ifndef M_TWOPI
#define M_TWOPI (M_PI * 2.0)
#endif /* !M_TWOPI */
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif /* !M_PI_2 */
#ifndef M_PI_4
#define M_PI_4 0.78539816339744830962
#endif /* !M_PI_4 */
#ifndef M_3PI_4
#define M_3PI_4 2.3561944901923448370E0
#endif /* !M_3PI_4 */
#ifndef M_SQRTPI
#define M_SQRTPI 1.77245385090551602792981
#endif /* !M_SQRTPI */
#ifndef M_1_PI
#define M_1_PI 0.31830988618379067154
#endif /* !M_1_PI */
#ifndef M_2_PI
#define M_2_PI 0.63661977236758134308
#endif /* !M_2_PI */
#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257390
#endif /* !M_2_SQRTPI */
#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif /* !M_SQRT2 */
#ifndef M_SQRT1_2
#define M_SQRT1_2 0.70710678118654752440
#endif /* !M_SQRT1_2 */
#ifndef M_LN2LO
#define M_LN2LO 1.9082149292705877000E-10
#endif /* !M_LN2LO */
#ifndef M_LN2HI
#define M_LN2HI 6.9314718036912381649E-1
#endif /* !M_LN2HI */
#ifndef M_SQRT3
#define M_SQRT3 1.73205080756887719000
#endif /* !M_SQRT3 */
#ifndef M_IVLN10
#define M_IVLN10 0.43429448190325182765 /* 1 / log(10) */
#endif /* !M_IVLN10 */
#ifndef M_LOG2_E
#define M_LOG2_E 0.693147180559945309417
#endif /* !M_LOG2_E */
#ifndef M_INVLN2
#define M_INVLN2 1.4426950408889633870E0 /* 1 / log(2) */
#endif /* !M_INVLN2 */

/* *********************************************************************** */

/* A unique identifier to recognize whether or not we're running under
 * Systems in Motion's Coin library.
 */
#define __COIN__

/* The following #undef'ed defines are automatically defined and
 * synchronized with the settings in configure.in when ``configure''
 * is executed.
 *
 * The #ifndef wrapper is necessary because during development, these
 * are also defined in the config.h file generated by ``configure''.
 */
#ifndef COIN_VERSION

/* These are available for adding or omitting features based on Coin
 * version numbers in client application sources. */
#define COIN_MAJOR_VERSION 2
#define COIN_MINOR_VERSION 5
#define COIN_MICRO_VERSION 0
/* #undef COIN_BETA_VERSION */
#define COIN_VERSION "2.5.0"

/* This define is set by the configure script if singleprecision math
   functions are available from the C library API headers. */
/* #undef HAVE_SINGLEPRECISION_MATH */

/* Most compilers should have "hash quoting", as it is part of the
   ANSI standard. */
#define HAVE_HASH_QUOTING 1
/* #undef HAVE_APOSTROPHES_QUOTING */

/* IN_PATH define in HPUX's sys/unistd.h conflicts with SoAction::IN_PATH */
/* #undef COIN_UNDEF_IN_PATH_HACK */

/* This define is wrapped around new features in development that will
   be introduced with the next minor release. Don't switch this define
   without rebuilding the library - just switching it for an installed
   library will just bring the header files out of sync with the library,
   causing a bunch of problems. */
/* NOTE: this define is from now on disabled, but could get reintroduced */
/* # undef COIN_NEXT_MINOR */

#endif /* !COIN_VERSION */ /* Ends configure-generated defines. */

/* *********************************************************************** */

/* The float-version of the math functions below did not come about
   until C99, so we don't want to use them internally, for portability
   reasons. */
#ifdef COIN_INTERNAL
#undef \
  cosf
#define cosf(x) NO_SINGLEPREC /* whatever that'll give us a compile error... */
#undef \
  sinf
#define sinf(x) NO_SINGLEPREC
#undef \
  tanf
#define tanf(x) NO_SINGLEPREC
#undef \
  powf
#define powf(x) NO_SINGLEPREC
#undef \
  sqrtf
#define sqrtf(x) NO_SINGLEPREC
#undef \
  acosf
#define acosf(x) NO_SINGLEPREC
#undef \
  asinf
#define asinf(x) NO_SINGLEPREC
#undef \
  atanf
#define atanf(x) NO_SINGLEPREC
#undef \
  atan2f
#define atan2f(x) NO_SINGLEPREC
#endif /* COIN_INTERNAL */

/* *********************************************************************** */

/* SO__QUOTE(str)        - use the preprocessor to quote a string.
 * SO__CONCAT(str1,str2) - use the preprocessor to concatenate two strings.
 */

#ifdef HAVE_HASH_QUOTING
#define SO__QUOTE(str)           #str
#define SO__CONCAT(str1, str2)   str1##str2
#elif defined(HAVE_APOSTROPHES_QUOTING)
#define SO__QUOTE(str)           "str"
#define SO__CONCAT(str1, str2)   str1/**/str2
#else
#error No valid way to do macro quoting!
#endif

/* *********************************************************************** */

/* Precaution to avoid an some errors easily made by the application
   programmer. */
#ifdef COIN_DLL_API
# error Leave the internal COIN_DLL_API define alone.
#endif /* COIN_DLL_API */
#ifdef COIN_INTERNAL
# ifdef COIN_NOT_DLL
#  error The COIN_NOT_DLL define is not supposed to be used when building the library, only when building Win32 applications.
# endif /* COIN_INTERNAL && COIN_NOT_DLL */
# ifdef COIN_DLL
#  error The COIN_DLL define is not supposed to be used when building the library, only when building Win32 applications.
# endif /* COIN_INTERNAL && COIN_DLL */
#endif /* COIN_INTERNAL */

/*
  On MSWindows platforms, one of these defines must always be set when
  building application programs:

   - "COIN_DLL", when the application programmer is using the library
     in the form of a dynamic link library (DLL)

   - "COIN_NOT_DLL", when the application programmer is using the
     library in the form of a static object library (LIB)

  Note that either COIN_DLL or COIN_NOT_DLL _must_ be defined by the
  application programmer on MSWindows platforms, or else the #error
  statement will hit. Set up one or the other of these two defines in
  your compiler environment according to how the library was built --
  as a DLL (use "COIN_DLL") or as a LIB (use "COIN_NOT_DLL").

  (Setting up defines for the compiler is typically done by either
  adding something like "/DCOIN_DLL" to the compiler's argument line
  (for command-line build processes), or by adding the define to the
  list of preprocessor symbols in your IDE GUI (in the MSVC IDE, this
  is done from the "Project"->"Settings" menu, choose the "C/C++" tab,
  then "Preprocessor" from the dropdown box and add the appropriate
  define)).

  It is extremely important that the application programmer uses the
  correct define, as using "COIN_NOT_DLL" when "COIN_DLL" is correct
  will cause mysterious crashes.
 */
/* FIXME: use a feature check to see if this is a platform which can
   recognize the __declspec keyword instead of the crap #if below.
   20011201 mortene. */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# ifdef COIN_INTERNAL
#  ifdef COIN_MAKE_DLL
#   define COIN_DLL_API __declspec(dllexport)
#  endif /* COIN_MAKE_DLL */
# else /* !COIN_INTERNAL */
#  ifdef COIN_DLL
#   ifdef COIN_NOT_DLL
#     error Define _either_ COIN_DLL _or_ COIN_NOT_DLL as appropriate for your linkage -- not both at the same time! See Inventor/C/basic.h for further instructions.
#   endif /* COIN_NOT_DLL */
#   define COIN_DLL_API __declspec(dllimport)
#  else /* !COIN_DLL */
#   ifndef COIN_NOT_DLL
#    error Define either COIN_DLL or COIN_NOT_DLL as appropriate for your linkage! See Inventor/C/basic.h for further instructions.
#   endif /* !COIN_NOT_DLL */
#  endif /* !COIN_DLL */
# endif /* !COIN_INTERNAL */
#endif /* Microsoft Windows */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef COIN_DLL_API
# define COIN_DLL_API
#endif /* !COIN_DLL_API */

#endif /* !COIN_BASIC_H */
