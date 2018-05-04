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

/*!
  \class SoXtSpaceball SoXtSpaceball.h Inventor/Xt/devices/SoXtSpaceball.h
  \brief The SoXtSpaceball class is the spaceball input device abstraction.
  \ingroup devices

  The SoXtSpaceball class is the glue between native spaceball
  handling and how those events translates to generic events for the
  Inventor API scenegraph.
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Xt/devices/SoXtSpaceball.h>
#include <Inventor/Xt/devices/SoXtSpaceballP.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtSpaceball);

// *************************************************************************

/*!
  \enum SoXtSpaceball::DeviceType

  Enumeration of spaceball input device types.
*/
/*!
  \enum SoXtSpaceball::Mask
  Enumeration of spaceball input device event types.
*/

/*!
  \fn SoXtSpaceball::SoXtSpaceball(int mask)

  Public constructor. The \a mask argument specifies which events we
  are interested in.
*/

/*!
  \fn SoXtSpaceball::~SoXtSpaceball()
  Destructor.
*/

// *************************************************************************

/*!
  This method sets the scale factor for the spaceball rotation vectors.
*/
void
SoXtSpaceball::setRotationScaleFactor(float f)
{
  PRIVATE(this)->rotationscale = f;
}

/*!
  This method returns the scale factor for the spaceball rotation vectors.
*/
float
SoXtSpaceball::getRotationScaleFactor(void) const
{
  return PRIVATE(this)->rotationscale;
}

/*!
  This method sets the scale factor for the spaceball translation vectors.
*/
void
SoXtSpaceball::setTranslationScaleFactor(float f)
{
  PRIVATE(this)->translationscale = f;
}

/*!
  This method returns the scale factor for the spaceball translation vectors.
*/
float
SoXtSpaceball::getTranslationScaleFactor(void) const
{
  return PRIVATE(this)->translationscale;
}

// *************************************************************************

/*!
  \fn SbBool SoXtSpaceball::exists(void)

  Returns \c TRUE iff there could be a device of this type available
  on the system.

  Note that a return value of \c TRUE does \e not signify that there
  is such a device active.
*/

// *************************************************************************

/*!
  \fn void SoXtSpaceball::setFocusToWindow(SbBool flag)

  Set to \c TRUE if you want spaceball events to be received even when
  the component does not have focus.
*/

/*!
  \fn SbBool SoXtSpaceball::isFocusToWindow(void) const

  Returns whether or not spaceball events are caught even when the
  component does not have focus.
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

SoGuiSpaceballP::SoGuiSpaceballP(SoXtSpaceball * p)
{
  this->pub = p;
}

SoGuiSpaceballP::~SoGuiSpaceballP()
{
}

#endif // !DOXYGEN_SKIP_THIS

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

