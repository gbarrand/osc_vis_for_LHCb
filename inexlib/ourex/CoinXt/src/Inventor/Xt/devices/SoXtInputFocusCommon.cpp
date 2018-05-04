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
  \class SoXtInputFocus SoXtInputFocus.h Inventor/Xt/devices/SoXtInputFocus.h
  \brief The SoXtInputFocus class is an abstraction for widget focus events as an input device.
  \ingroup devices

  This device class is a "virtual device" which can be used to get
  events when the mouse pointer enters or leaves a specific widget.
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Xt/devices/SoXtInputFocus.h>
#include <Inventor/Xt/devices/SoGuiInputFocusP.h>

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtInputFocus);

// *************************************************************************

/*!
  \enum SoXtInputFocus::Events

  Bit-wise enumeration over events handled by the SoXtInputFocus
  device class.  Used in the SoXtInputFocus constructor.
*/

/*!
  \var SoXtInputFocus::Events SoXtInputFocus::ENTER_WINDOW

  Maskbit for receiving events when focus comes to a widget.
*/

/*!
  \var SoXtInputFocus::Events SoXtInputFocus::LEAVE_WINDOW

  Maskbit for receiving events when focus leaves a widget.
*/

/*!
  \var SoXtInputFocus::Events SoXtInputFocus::ALL_EVENTS

  Combined bitmask for all possible events.
*/

/*!
  \fn SoXtInputFocus::SoXtInputFocus(int mask)

  Constructor. The \a mask specifies which focus-related events to
  handle. Others will just be ignored.
*/

// *************************************************************************

SoGuiInputFocusP::SoGuiInputFocusP(SoXtInputFocus * p)
{
  this->pub = p;
}

SoGuiInputFocusP::~SoGuiInputFocusP()
{
}

// *************************************************************************
