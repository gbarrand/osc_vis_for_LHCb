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

/*!
  \class SoGtk SoGtk.h Inventor/Gtk/SoGtk.h
  \brief The SoGtk class takes care of Gtk initialization and event dispatching.
  \ingroup misc

  This is the "application-wide" class with solely static methods
  handling initialization and event processing tasks. You must use
  this class in any application built on top of the SoGtk
  library. Typical usage is as follows (complete application code):

  \code
#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>
#include <Inventor/nodes/SoCube.h>

int
main(int argc, char **argv)
{
  // Initialize SoGtk and Open Inventor libraries. This returns a main
  // window to use.
  GtkWidget * mainwin = SoGtk::init(argv[0]);

   // Make a dead simple scene graph, only containing a single cube.
  SoCube * cube = new SoCube;

  // Use one of the convenient viewer classes.
  SoGtkExaminerViewer * eviewer = new SoGtkExaminerViewer(mainwin);
  eviewer->setSceneGraph(cube);
  eviewer->show();

  // Pop up the main window.
  SoGtk::show(mainwin);
  // Loop until exit.
  SoGtk::mainLoop();
  return 0;
}
  \endcode

  \sa SoGtkComponent
*/

#include <stdlib.h>
#include <string.h>
#include <Inventor/Gtk/SoGtk.h>
#include <Inventor/Gtk/SoAny.h>


// Default constructor and the destructor is private (SoGtk class
// consists solely of static functions).
SoGtk::SoGtk(void) { }
SoGtk::~SoGtk() { }


// *************************************************************************

/*!
  \fn GtkWidget * SoGtk::init(const char * appname, const char * classname)

  This function initializes the SoGtk library.

  The returned GtkWidget * is a toplevel shell widget for the
  application, which can be used as a shell for the main component.

  This method is provided for easier porting / compatibility with the
  original SGI Inventor InventorXt library. It just adds dummy \a argc
  and \a argv arguments and calls the SoGtk::init() method below.
*/
GtkWidget *
SoGtk::internal_init(const char * appname, const char * classname)
{
  // If this is SoQt, Qt will keep a reference to the appname string
  // -- so make it guaranteed permanent.
  char * buf = NULL;
  if (appname != NULL) {
    buf = new char[strlen(appname) + 1]; // FIXME: minor memleak, free on app-exit. 20020117 mortene.
    (void)strcpy(buf, appname);
  }

  // Fake argc and argv setup, forward to real init routine.
  char * array[] = { buf, NULL };
  int argc = (buf != NULL) ? 1 : 0;
  return SoGtk::init(argc, array, appname, classname);
}

// *************************************************************************

/*!
  This method will fill in the integers pointed to by the arguments
  with the corresponding part of the version release number of the
  SoGtk library.

  A \c NULL pointer will make that part of the version number be ignored.

  This method is not part of the original InventorXt API from SGI.
*/
void
SoGtk::getVersionInfo(int * major, int * minor, int * micro)
{
  if (major) { *major = SOGTK_MAJOR_VERSION; }
  if (minor) { *minor = SOGTK_MINOR_VERSION; }
  if (micro) { *micro = SOGTK_MICRO_VERSION; }
}

/*!
  This method returns a string containing the version id of the
  library.

  This method is not part of the original InventorXt API from SGI.
*/
const char *
SoGtk::getVersionString(void)
{
  static const char version[] = SOGTK_VERSION;
  return version;
}

// *************************************************************************

/*!
  \typedef void SoGtk::FatalErrorCB(const SbString errmsg, SoGtk::FatalErrors errcode, void * userdata)

  An application function callback for handling fatal errors should be
  of this type.

  The first argument is an error message in English describing the
  details of the error. The second argument is an error code used so
  the application can identify specific conditions. The third argument
  is the userdata pointer passed in to SoGtk::setFatalErrorHandler().
*/

/*!
  \enum SoGtk::FatalErrors
  Numerical identifiers for classifying the different kinds of possible
  fatal errors.
*/

/*!
  \var SoGtk::FatalErrors SoGtk::NO_OPENGL_CANVAS

  Could not construct \e any valid OpenGL canvas. Something is very
  wrong on the client system.
*/
/*!
  \var SoGtk::FatalErrors SoGtk::INTERNAL_ASSERT

  An internal error condition that should never happen was
  detected. The most likely cause of this is programmering errors
  within the SoGtk library itself.
*/
/*!
  \var SoGtk::FatalErrors SoGtk::UNSPECIFIED_ERROR

  Signifies that we were not able to specify in any greater detail the
  error condition that came up.
*/

/*!
  Set up a callback to invoke in the case of unexpected fatal error
  conditions within the SoGtk library.

  Almost any error condition within the library is handled in a robust
  way through return values indicating errors for the offending calls,
  but there are a few cases that are impossible to handle without
  seriously crippling the functionality.

  (One example is if we fail to find \e any way of making a valid
  OpenGL canvas. This is an indication that something is seriously
  wrong on the end-user's system, and the SoGtk library will not work
  properly.)

  In the case of a fatal error, it is expected that the given
  application callback function communicates the problem to the
  end-user and then either exits the application or at least refrains
  from using any part of the SoGtk library.

  If no callback is explicitly set up by the application, the SoGtk
  library will display an error message to the end-user and then exit
  the application.

  When setting a callback, this method returns a pointer to the
  previous callback function, or \c NULL if none.

  (This is an extension versus the original SGI InventorXt library
  API.)


  On a related note, be aware that the end-user will still be notified
  about non-fatal errors and warning messages through a dialog box. If
  you want to handle these yourself, or if you don't want your
  end-user to see any of these non-critical messages, you will also
  need to install an error handler callback function on the Coin
  library itself:

  \code
    SoDebugError::setHandlerCallback(myErrorHandlerCB, myCBUserData);
  \endcode

  (Please also see the documentation of
  SoDebugError::setHandlerCallback().)

 */
SoGtk::FatalErrorCB *
SoGtk::setFatalErrorHandler(SoGtk::FatalErrorCB * cb, void * userdata)
{
  return SoAny::si()->setFatalErrorHandler(cb, userdata);
}

// *************************************************************************

/*!
  Returns \c TRUE if this binary version of the SoGtk library was
  compiled with debug settings on.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SbBool
SoGtk::isDebugLibrary(void)
{
  return SOGTK_DEBUG != 0 ? TRUE : FALSE;
}

/*!
  Returns \c TRUE if this version of the SoGtk library has an
  Application Binary Interface compatible with the given version.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SbBool
SoGtk::isCompatible(unsigned int major, unsigned int minor)
{
  if (major != SOGTK_MAJOR_VERSION) { return FALSE; }
  if (minor > SOGTK_MINOR_VERSION) { return FALSE; }
  return TRUE;
}

/*!
  Returns an indication on how the library was compiled: as a dynamic
  library, or as a static library.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SoGtkABIType
SoGtk::getABIType(void)
{
  // FIXME: replace this ugly shit with a configure-check to test if
  // we're on a DLL-capable platform. 20020118 mortene.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef SOGTK_MAKE_DLL
  return SOGTK_DLL_ABI;
#else
  return SOGTK_LIB_ABI;
#endif
#endif
  return SOGTK_UNKNOWN_ABI;
}

/*!
  Abort the application due to some kind of mismatch in the ABI
  settings / configuration. This should hopefully help application
  programmers avoid shooting themselves in the foot by controlling
  certain run-time parameters in the client application versus what is
  expected by the SoGtk library.

  If you're an application programmer, it is very likely that you only
  need to consider this an internal library method.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
void
SoGtk::abort(SoGtkABIError error)
{
  switch (error) {
  case SOGTK_LINKTIME_MISMATCH:
    SoGtk::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected linktime mismatch error.");
    break;

  case SOGTK_LINKSTYLE_MISMATCH:
    SoGtk::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected linkstyle mismatch error (DLL vs. LIB).");
    break;

  case SOGTK_RUNTIME_MISMATCH:
    SoGtk::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected runtime mismatch error (versioning and ABI compatibility).");
    break;

  default:
    // FIXME: shouldn't this rather be an assert? 20020118 mortene.
    SoGtk::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Unknown error in SoGtk :(");
    break;
  }

  // FIXME: call fatal error handler in SoAny. 20020118 mortene.
  exit(-1);
}

// *************************************************************************
