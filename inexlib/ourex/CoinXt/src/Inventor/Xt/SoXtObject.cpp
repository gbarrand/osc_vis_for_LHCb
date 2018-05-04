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

// Pulls in __COIN_SOXT__ define used below.
#include <Inventor/Xt/SoXtBasic.h>

// #ifdef __COIN_SOQT__
// #include <qmetaobject.h>
// #include <moc_SoQtObject.icc>
// #endif // __COIN_SOQT__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtObject.h>
#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/SoXtComponent.h>

/*!
  \class SoXtObject Inventor/Xt/SoXtObject.h
  \brief The SoXtObject class is the common superclass for all SoXt component classes.
  \ingroup misc

  The purpose of making this class the superclass of all SoXt
  device, component and viewer classes is to be able to do run-time
  type checking of the SoXt objects.


  You can place the macro SOXT_OBJECT_HEADER(classname,parentname)
  within a class definition header for SoXt extension components to
  automatically make the necessary definitions for setting up a
  run-time type system for your extension classes:

  \code
  #ifndef MYSPECIALVIEWER_H
  #define MYSPECIALVIEWER_H

  class MySpecialViewer : public SoXtExaminerViewer {
    SOXT_OBJECT_HEADER(MySpecialViewer, SoXtExaminerViewer);
    // [rest of class definition follows]
  };

  #endif // !MYSPECIALVIEWER_H
  \endcode

  Then put the SOXT_OBJECT_SOURCE(classname) macro within the
  actual implementation sourcecode file to include the necessary
  code for the run-time type system:

  \code
  #include <MySpecialViewer.h>

  SOQT_OBJECT_SOURCE(MySpecialViewer);

  // [rest of class implementation]
  \endcode


  See also the documentation of the SoType class in Coin or Inventor.
 */

// *************************************************************************

/*!
  \fn SoType SoXtObject::getTypeId

  Returns the type identification of an object derived from a class
  inheriting SoXtObject.  This is used for run-time type checking
  and "downward" casting.

  Usage example:

  \code
  void foo(SoXtViewer * comp)
  {
    if (comp->getTypeId() == SoXtExaminerViewer::getClassTypeId()) {
      // safe downward cast, knows the type
      SoXtExaminerViewer * exviewer = (SoXtExaminerViewer *)comp;
      /// [then something] ///
    }
    else if (comp->getTypeId().isOfType(SoXtFlyViewer::getClassTypeId())) {
      // safe downward cast, knows the type
      SoXtFlyViewer * flyviewer = (SoXtFlyViewer *)comp;
      // then something else
    }
  }
  \endcode
*/
// FIXME: add doc above to explain how external developers can use the
// type system for their own extension classes. 20020502 mortene.

// This is a private variable.
SoType SoXtObject::classTypeId;

/*!
  Sets up initialization for data common to all instances of this
  class, submitting necessary information to the internal SoXt type
  system.
*/
void
SoXtObject::initClass(void)
{
  assert(SoXtObject::classTypeId == SoType::badType());
  SoXtObject::classTypeId =
    SoType::createType(SoType::badType(), "SoXtObject");
}

/*!
  Returns \c TRUE if the type of this object is either of the same
  type or inherited from \a type.
*/
SbBool
SoXtObject::isOfType(SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  This static method returns the SoType object associated with
  objects of this class.
*/
SoType
SoXtObject::getClassTypeId(void)
{
  return SoXtObject::classTypeId;
}

/*!
  Initialize the type system of SoXtObject, all SoXt device
  classes and all SoXt components (including viewers).
*/
void
SoXtObject::init(void)
{
  SoXtObject::initClass();
  SoXtDevice::initClasses();
  SoXtComponent::initClasses();
}
