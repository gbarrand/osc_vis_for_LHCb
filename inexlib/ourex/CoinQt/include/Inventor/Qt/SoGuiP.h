#ifndef SOGUIP_H
#define SOGUIP_H

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

#ifndef SOQT_INTERNAL
#error this is a private header file
#endif /* !SOQT_INTERNAL */

// ************************************************************************

// This class contains private data and methods used within the
// SoQt class.

class SoGuiP
{
public:
  enum ABIError {
    LINKTIME_MISMATCH, LINKSTYLE_MISMATCH, RUNTIME_MISMATCH
  };
  static void abort(SoGuiP::ABIError error);

  static void sensorQueueChanged(void * cbdata);
};

// ************************************************************************

#endif // ! SOGUIP_H
