/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

/*  $Id: materials.h,v 1.1.1.1 2002/02/08 15:04:12 barrand Exp $  */
/*  @configure_input@  */

#ifndef SOQT_MATERIALS_IV_H
#define SOQT_MATERIALS_IV_H

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

enum SoQtMaterialStyle {
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

#endif /* ! SOQT_MATERIALS_IV_H */
