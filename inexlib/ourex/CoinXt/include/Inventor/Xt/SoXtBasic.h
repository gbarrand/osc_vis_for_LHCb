/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

// @configure_input@

#ifndef SOXT_BASIC_H
#define SOXT_BASIC_H

// *************************************************************************

/* Some useful inline template functions:
 *   SoXtMin(Val1, Val2)       - returns minimum value
 *   SoXtMax(Val1, Val2)       - returns maximum value
 *   SoXtClamp(Val, Min, Max)  - returns clamped value
 *   SoXtSwap(Val1, Val2)      - swaps the two values (no return value)
 */

template <class Type>
inline Type SoXtAbs(Type Val) {
  return (Val < 0) ? -Val : Val;
}

template <class Type>
inline Type SoXtMin(Type a, Type b) {
  return (b < a) ? b : a;
}

template <class Type>
inline Type SoXtMax(Type a, Type b) {
  return (b > a) ? b : a;
}

template <class Type>
inline Type SoXtClamp(Type val, Type min, Type max) {
  return SoXtMax(min, SoXtMin(max, val));
}

template <class Type>
inline void SoXtSwap(Type & a, Type & b) {
  Type t = a; a = b; b = t;
}

// *************************************************************************

#define __COIN_SOXT__

#if ! defined(SOXT_MAJOR_VERSION)
#define SOXT_MAJOR_VERSION 1
#endif /* ! SOXT_MAJOR_VERSION */
#if ! defined(SOXT_MINOR_VERSION)
#define SOXT_MINOR_VERSION 2
#endif /* ! SOXT_MINOR_VERSION */
#if ! defined(SOXT_MICRO_VERSION)
#define SOXT_MICRO_VERSION 2
#endif /* ! SOXT_MICRO_VERSION */
#if ! defined(SOXT_BETA_VERSION)
/* #undef SOXT_BETA_VERSION */
#endif /* ! SOXT_BETA_VERSION */
#if ! defined(SOXT_VERSION)
#define SOXT_VERSION "1.2.2"
#endif /* ! SOXT_VERSION */

// *************************************************************************

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef SOXT_DLL_API
# error Leave the internal SOXT_DLL_API define alone.
#endif /* SOXT_DLL_API */

/*
  On MSWindows platforms, one of these defines must always be set when
  building application programs:

   - "SOXT_DLL", when the application programmer is using the
     library in the form of a dynamic link library (DLL)

   - "SOXT_NOT_DLL", when the application programmer is using the
     library in the form of a static object library (LIB)

  Note that either SOXT_DLL or SOXT_NOT_DLL _must_ be defined by
  the application programmer on MSWindows platforms, or else the
  #error statement will hit. Set up one or the other of these two
  defines in your compiler environment according to how the library
  was built -- as a DLL (use "SOXT_DLL") or as a LIB (use
  "SOXT_NOT_DLL").

  (Setting up defines for the compiler is typically done by either
  adding something like "/DSOXT_DLL" to the compiler's argument
  line (for command-line build processes), or by adding the define to
  the list of preprocessor symbols in your IDE GUI (in the MSVC IDE,
  this is done from the "Project"->"Settings" menu, choose the "C/C++"
  tab, then "Preprocessor" from the dropdown box and add the
  appropriate define)).

  It is extremely important that the application programmer uses the
  correct define, as using "SOXT_NOT_DLL" when "SOXT_DLL" is
  correct is likely to cause mysterious crashes.
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# ifdef SOXT_INTERNAL
#  ifdef SOXT_MAKE_DLL
#   define SOXT_DLL_API __declspec(dllexport)
#  endif /* SOXT_MAKE_DLL */
# else /* !SOXT_INTERNAL */
#  ifdef SOXT_DLL
#   define SOXT_DLL_API __declspec(dllimport)
#  else /* !SOXT_DLL */
#   ifndef SOXT_NOT_DLL
#    error Define either SOXT_DLL or SOXT_NOT_DLL as appropriate for your linkage! See Inventor/Xt/SoXtBasic.h for further instructions.
#   endif /* SOXT_NOT_DLL */
#  endif /* !SOXT_DLL */
# endif /* !SOXT_MAKE_DLL */
#endif /* Microsoft Windows */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef SOXT_DLL_API
# define SOXT_DLL_API
#endif /* !SOXT_DLL_API */

// *************************************************************************

#endif // ! SOXT_BASIC_H
