#ifndef COIN_SOSFVEC4I32_H
#define COIN_SOSFVEC4I32_H

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

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4i32.h>

class COIN_DLL_API SoSFVec4i32 : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFVec4i32, SbVec4i32, const SbVec4i32 &);

public:
  static void initClass(void);

  void setValue(int32_t x, int32_t y, int32_t z, int32_t w);
  void setValue(const int32_t xyzw[4]);

}; // SoSFVec4i32

#endif // !COIN_SOSFVEC4I32_H
