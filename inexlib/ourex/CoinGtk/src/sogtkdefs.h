/**************************************************************************
 *
 *  This file is part of the Coin SoQt GUI binding library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoQt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

/*  */

#ifndef SOGTK_INTERNALDEFS_H
#define SOGTK_INTERNALDEFS_H

/* ********************************************************************** */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* ********************************************************************** */

#ifdef __FILE__
#define SOGTK_STUB_FILE __FILE__
#else
/* FIXME: avoid this case by redefining the compilation rules to define the
   current filename in some variable.  [20010919 larsa] */
#define SOGTK_STUB_FILE ((char *)0L)
#endif

#ifdef __LINE__
#define SOGTK_STUB_LINE __LINE__
#else
#define SOGTK_STUB_LINE 0
#endif

#if HAVE_VAR___PRETTY_FUNCTION__
#define SOGTK_STUB_FUNC __PRETTY_FUNCTION__
#else
#if HAVE_VAR___FUNCTION__
#define SOGTK_STUB_FUNC __FUNCTION__
#else
#if HAVE_VAR___func__
#define SOGTK_STUB_FUNC __func__
#else
#define SOGTK_STUB_FUNC ((char *)0L)
#endif
#endif
#endif

#if SOGTK_DEBUG

#include <Inventor/errors/SoDebugError.h>

/*
  This stupid thing is here to silence some compilers that complain on
  constant if-expressions.
*/

inline int sogtk_not_null(const void * arg) { return (arg != NULL) ? 1 : 0; }


/* Collects common code from SOGTK_STUB(), SOGTK_STUB()_ONCE and
   SOGTK_OBSOLETED(). */

#define SOGTK_FUNCTIONIDENTIFIER(SOGTKSTR) \
    SbString SOGTKSTR(sogtk_not_null(SOGTK_STUB_FILE) ? SOGTK_STUB_FILE : ""); \
    if (SOGTKSTR.getLength() > 0) { SOGTKSTR += ':'; } \
    SOGTKSTR += SbString((int)SOGTK_STUB_LINE); SOGTKSTR += ':'; \
    SOGTKSTR += sogtk_not_null(SOGTK_STUB_FUNC) ? SOGTK_STUB_FUNC : ""

/*
  SOGTK_STUB(): a macro that prints out stub information.
  Used where there is functionality missing.
*/

#define SOGTK_STUB() \
  do { \
    SOGTK_FUNCTIONIDENTIFIER(s); \
    SoDebugError::postWarning(s.getString(), \
                              "STUB: functionality not yet completed"); \
  } while (0)

/*
  SOGTK_STUB_ONCE(): a macro that prints out stub information, but only on
  the first pass.  Used where there is functionality missing that may be
  called hundreds of times.
*/

#define SOGTK_STUB_ONCE() \
  do { \
    static int first = 1; \
    if (first) { \
      SOGTK_FUNCTIONIDENTIFIER(s); \
      SoDebugError::postWarning(s.getString(), \
                                "STUB: functionality not yet completed " \
                                "(first and last warning)"); \
      first = 0; \
    } \
  } while (0)

/*
  SOGTK_OBSOLETED(): a macro that prints out information about an obsoleted
  function still being in use.
*/

#define SOGTK_OBSOLETED() \
  do { \
    SOGTK_FUNCTIONIDENTIFIER(s); \
    SoDebugError::post(s.getString(), \
                       "OBSOLETED: functionality no longer supported"); \
  } while (0)

#else /* !SOGTK_DEBUG */

/* snub the stubs */
#define SOGTK_STUB()      do { } while (0)
#define SOGTK_STUB_ONCE() do { } while (0)
#define SOGTK_OBSOLETED() do { } while (0)

#endif /* !SOGTK_DEBUG */

/*
  GETTEXT support
*/

#if defined(ENABLE_NLS) && defined(HAVE_GETTEXT)
#include <libintl.h>
#define _(String) dgettext (PACKAGE, String)
#define N_(String) (String)
#else
#define _(String) (String)
/*GB #define N_(String) (String)*/
#define N_(String) String
#define textdomain(Domain)
#define bindtextdomain(Package, Directory)
#endif /* HAVE_GETTEXT && ENABLE_NLS */

/* ********************************************************************** */

#endif /* ! SOGTK_INTERNALDEFS_H */
