/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef SOQTINTERNAL_H
#define SOQTINTERNAL_H

#include <QtGui/qevent.h> //G.Barrand QtGui

// ************************************************************************

// This class contains common data and methods that we want to share
// among classes within SoQt, but which should not be publicly visible
// in the library API.

class SoQtInternal {
public:
  enum CustomEventId { SPACEBALL_EVENT = QEvent::User };
};

// ************************************************************************

#endif // ! SOQTINTERNAL_H
