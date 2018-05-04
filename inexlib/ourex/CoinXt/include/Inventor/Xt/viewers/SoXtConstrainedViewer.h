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

// @configure_input@

#ifndef SOXT_CONSTRAINEDVIEWER_H
#define SOXT_CONSTRAINEDVIEWER_H

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

// ************************************************************************

class SOXT_DLL_API SoXtConstrainedViewer : public SoXtFullViewer {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtConstrainedViewer, SoXtFullViewer);

public:
  void setUpDirection(const SbVec3f & dir);
  const SbVec3f & getUpDirection(void) const;

  virtual void setCamera(SoCamera * newcam);
  virtual void saveHomePosition(void);
  virtual void resetToHomePosition(void);

protected:
  SoXtConstrainedViewer(Widget parent,
                           const char * const name,
                           SbBool embed,
                           SoXtFullViewer::BuildFlag flag, 
                           SoXtViewer::Type type, 
                           SbBool build);

  ~SoXtConstrainedViewer();

  virtual void tiltCamera(float delta);
  virtual void bottomWheelMotion(float value);
  virtual void leftWheelMotion(float value);

  virtual void changeCameraValues(SoCamera * camera);
  void findUpDirection(SbVec2s mouselocation);
  void checkForCameraUpConstrain(void);

private:
  class SoXtConstrainedViewerP * pimpl;
};

// ************************************************************************

#endif // ! SOXT_CONSTRAINEDVIEWER_H
