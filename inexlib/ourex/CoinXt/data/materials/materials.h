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

/*  $Id: materials.h,v 1.3 2005/11/30 16:05:03 barrand Exp $  */
/*  @configure_input@  */

#ifndef SOXT_MATERIALS_IV_H
#define SOXT_MATERIALS_IV_H

/* ********************************************************************** */

#include <materials/artdeco/artdeco.h>
#include <materials/autumn/autumn.h>
#include <materials/glass/glass.h>
#include <materials/metal/metal.h>
#include <materials/neon/neon.h>
#include <materials/rococo/rococo.h>
#include <materials/santafe/santafe.h>
#include <materials/sheen/sheen.h>
#include <materials/silky/silky.h>
#include <materials/spring/spring.h>
#include <materials/summer/summer.h>
#include <materials/tropical/tropical.h>
#include <materials/winter/winter.h>

enum SoXtMaterialStyle {
  ARTDECO,
  AUTUMN,
  GLASS,
  METAL,
  NEON,
  ROCOCO,
  SANTAFE,
  SHEEN,
  SILKY,
  SPRING,
  SUMMER,
  TROPICAL,
  WINTER,
  NO_MATERIAL
};

/* ********************************************************************** */

#endif /* ! SOXT_MATERIALS_IV_H */
