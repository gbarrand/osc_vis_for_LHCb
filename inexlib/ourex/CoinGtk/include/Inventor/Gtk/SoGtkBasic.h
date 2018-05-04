/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// @configure_input@
// 

#ifndef SOGTK_BASIC_H
#define SOGTK_BASIC_H

// *************************************************************************

/* For the Doxygen tool. */
/* FIXME: these Doxygen "tags" should be moved to an internal
   headerfile. 20010810 mortene. */

/*! \defgroup devices Gtk Device Classes */
/*! \defgroup components Gtk Components */
/*! \defgroup viewers Gtk Viewer Components */
/*! \defgroup editors Gtk Editor Components */
/*! \defgroup slidersets Gtk Slider Set Components */
/*! \defgroup misc Miscellaneous Classes */
/*! \defgroup common Common Code */

// *************************************************************************

/* Some useful inline template functions (instead of macros):
 *   SoGtkMin(Val1, Val2)       - returns minimum value
 *   SoGtkMax(Val1, Val2)       - returns maximum value
 *   SoGtkClamp(Val, Min, Max)  - returns clamped value
 *   SoGtkSwap(Val1, Val2)      - swaps the two values (no return value)
 *
 * FIXME: write Doxygen doc for these methods. 20010810 mortene.
 */

template <class Type>
inline Type SoGtkAbs(Type Val) {
  return (Val < 0) ? -Val : Val;
}

template <class Type>
inline Type SoGtkMin(Type a, Type b) {
  return (b < a) ? b : a;
}

template <class Type>
inline Type SoGtkMax(Type a, Type b) {
  return (b > a) ? b : a;
}

template <class Type>
inline Type SoGtkClamp(Type val, Type min, Type max) {
  return SoGtkMax(min, SoGtkMin(max, val));
}

template <class Type>
inline void SoGtkSwap(Type & a, Type & b) {
  Type t = a; a = b; b = t;
}

// *************************************************************************

#define __COIN_SOGTK__

#if ! defined(SOGTK_MAJOR_VERSION)
#define SOGTK_MAJOR_VERSION 0
#endif /* ! SOGTK_MAJOR_VERSION */
#if ! defined(SOGTK_MINOR_VERSION)
#define SOGTK_MINOR_VERSION 5
#endif /* ! SOGTK_MINOR_VERSION */
#if ! defined(SOGTK_MICRO_VERSION)
#define SOGTK_MICRO_VERSION 0
#endif /* ! SOGTK_MICRO_VERSION */
#if ! defined(SOGTK_VERSION)
#define SOGTK_VERSION "0.5.0"
#endif /* ! SOGTK_VERSION */

// *************************************************************************

/* Precaution to avoid an error easily made by the application programmer. */
#ifdef SOGTK_DLL_API
# error Leave the internal SOGTK_DLL_API define alone.
#endif /* SOGTK_DLL_API */

/*
  On MSWindows platforms, one of these defines must always be set when
  building application programs:

   - "SOGTK_DLL", when the application programmer is using the
     library in the form of a dynamic link library (DLL)

   - "SOGTK_NOT_DLL", when the application programmer is using the
     library in the form of a static object library (LIB)

  Note that either SOGTK_DLL or SOGTK_NOT_DLL _must_ be defined by
  the application programmer on MSWindows platforms, or else the
  #error statement will hit. Set up one or the other of these two
  defines in your compiler environment according to how the library
  was built -- as a DLL (use "SOGTK_DLL") or as a LIB (use
  "SOGTK_NOT_DLL").

  (Setting up defines for the compiler is typically done by either
  adding something like "/DSOGTK_DLL" to the compiler's argument
  line (for command-line build processes), or by adding the define to
  the list of preprocessor symbols in your IDE GUI (in the MSVC IDE,
  this is done from the "Project"->"Settings" menu, choose the "C/C++"
  tab, then "Preprocessor" from the dropdown box and add the
  appropriate define)).

  It is extremely important that the application programmer uses the
  correct define, as using "SOGTK_NOT_DLL" when "SOGTK_DLL" is
  correct is likely to cause mysterious crashes.
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# ifdef SOGTK_INTERNAL
#  ifdef SOGTK_MAKE_DLL
#   define SOGTK_DLL_API __declspec(dllexport)
#  endif /* SOGTK_MAKE_DLL */
# else /* !SOGTK_INTERNAL */
#  ifdef SOGTK_DLL
#   define SOGTK_DLL_API __declspec(dllimport)
#  else /* !SOGTK_DLL */
#   ifndef SOGTK_NOT_DLL
#    error Define either SOGTK_DLL or SOGTK_NOT_DLL as appropriate for your linkage! See Inventor/Gtk/SoGtkBasic.h for further instructions.
#   endif /* SOGTK_NOT_DLL */
#  endif /* !SOGTK_DLL */
# endif /* !SOGTK_MAKE_DLL */
#endif /* Microsoft Windows */

/* Empty define to avoid errors when _not_ compiling an MSWindows DLL. */
#ifndef SOGTK_DLL_API
# define SOGTK_DLL_API
#endif /* !SOGTK_DLL_API */

// *************************************************************************
// These sanity checks are designed to detect common pitfall errors for
// Microsoft Windows linkage with SoGtk and Coin.

enum SoGtkABIType {
  SOGTK_DLL_ABI, SOGTK_LIB_ABI, SOGTK_UNKNOWN_ABI };

enum SoGtkABIError {
  SOGTK_LINKTIME_MISMATCH, SOGTK_LINKSTYLE_MISMATCH,
  SOGTK_RUNTIME_MISMATCH };

// FIXME: use an "is-this-a-DLL-capable-platform" configure check
// and remove the system "#if defined" tests below. 20011203 mortene.
// FIXME: I disabled this because it wasn't inlined in the client app
// but compiled into the library by MSVC++ and with SOWIN_DLL undefined,
// the ABI test always tried the LIB_ABI path.  20020126 larsa
#if 0 && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define SOGTK_SANITY_CHECK(forward_call)                  \
  if (SoGtk::isDebugLibrary() &&                       \
       SoDB::isInitialized())                          \
    SoDebugError::postWarning("SoGtk::init",            \
      "unable to verify application linkage sanity "    \
      "because Open Inventor has already been "         \
      "initialized before SoGtk::init call");           \
  SOGTK_RUNTIME_SANITY_CHECK();                         \
  SOGTK_LINKSTYLE_SANITY_CHECK();                       \
  forward_call;                                         \
  SOGTK_LINKTIME_SANITY_CHECK()

#define SOGTK_RUNTIME_SANITY_CHECK()                    \
  do {                                                  \
    if (!SoGtk::isCompatible(SOGTK_MAJOR_VERSION,      \
                            SOGTK_MINOR_VERSION))      \
      SoGtk::abort(SOGTK_RUNTIME_MISMATCH);             \
  } while (FALSE)

#ifdef SOGTK_DLL
#define SOGTK_LINKSTYLE_SANITY_CHECK()                  \
  do {                                                  \
    if (SoGtk::getABIType() != SOGTK_DLL_ABI)         \
      SoGtk::abort(SOGTK_LINKSTYLE_MISMATCH);           \
  } while (FALSE)
#else
#define SOGTK_LINKSTYLE_SANITY_CHECK()                  \
  do {                                                  \
    if (SoGtk::getABIType() != SOGTK_LIB_ABI)         \
      SoGtk::abort(SOGTK_LINKSTYLE_MISMATCH);           \
  } while (FALSE)
#endif

#define SOGTK_LINKTIME_SANITY_CHECK()                   \
  do {                                                  \
    if (!SoDB::isInitialized())                       \
      SoGtk::abort(SOGTK_LINKTIME_MISMATCH);            \
  } while (FALSE)
#else /* ! MS Windows */
#define SOGTK_SANITY_CHECK(forward_call)                \
  forward_call
#endif /* ! MS Windows */

// *************************************************************************

#endif // ! SOGTK_BASIC_H
