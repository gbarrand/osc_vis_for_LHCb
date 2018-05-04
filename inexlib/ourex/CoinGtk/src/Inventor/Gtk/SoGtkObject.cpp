/**************************************************************************
 *
 *  This file is part of the Coin GUI toolkit libraries.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin GUI toolkits for applications not compatible
 *  with the LGPL, please contact SIM to acquire a Professional Edition
 *  License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if SOGTK_DEBUG
static const char rcsid[] =
  "";
#endif

#ifdef SOQT_INTERNAL
#include <qmetaobject.h>
#include <moc_SoQtObject.cpp>
#endif // SOQT_INTERNAL

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoGtkObject.h>
#include <Inventor/Gtk/devices/SoGtkDevice.h>
#include <Inventor/Gtk/SoGtkComponent.h>

/*!
  \class SoGtkObject Inventor/Gtk/SoGtkObject.h
  \brief The SoGtkObject class is for run-time type checking of the SoGtk
  objects.
*/

// *************************************************************************

/*!
  \var SoType SoGtkObject::classTypeId
  FIXME: write doc
*/

/*!
  \fn SoType SoGtkObject::getTypeId
  FIXME: write doc
*/

SoType SoGtkObject::classTypeId; // static

/*!
  \fn void SoGtkObject::init(void)

  FIXME: write doc
*/

/*!
  FIXME: write doc
*/

void
SoGtkObject::initClass(// static
  void)
{
  assert(SoGtkObject::classTypeId == SoType::badType());
  SoGtkObject::classTypeId =
    SoType::createType(SoType::badType(), "SoGtkObject");
} // initClass()

/*!
  FIXME: write doc
*/

SbBool
SoGtkObject::isOfType(
  SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
} // isOfType()

/*!
  FIXME: write doc
*/

SoType
SoGtkObject::getClassTypeId(// static
  void) 
{
  return SoGtkObject::classTypeId;
} // getClassTypeId()

// *************************************************************************
