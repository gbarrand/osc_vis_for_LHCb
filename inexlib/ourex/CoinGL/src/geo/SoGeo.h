#ifndef COIN_SOGEO_H
#define COIN_SOGEO_H

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

#include <Inventor/SbMatrix.h>

class SbString;
class SbVec3d;

class SoGeo {
public:
  static void init(void);

  static SbMatrix calculateTransform(const SbString * originsystem,
                                     const int numoriginsys,
                                     const SbVec3d & origincoords,

                                     const SbString * localsystem,
                                     const int numlocalsys,
                                     const SbVec3d & localcoords);

};

#endif // COIN_SOGEO_H
