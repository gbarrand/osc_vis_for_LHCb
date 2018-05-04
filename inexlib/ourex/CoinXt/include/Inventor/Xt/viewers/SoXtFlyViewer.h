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

#ifndef SOXT_FLYVIEWER_H
#define SOXT_FLYVIEWER_H

#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

// ************************************************************************

class SOXT_DLL_API SoXtFlyViewer : public SoXtConstrainedViewer {
  SOXT_OBJECT_HEADER(SoXtFlyViewer, SoXtConstrainedViewer);

public:
  SoXtFlyViewer(Widget parent = NULL,
                   const char * name = NULL, 
                   SbBool embed = TRUE, 
                   SoXtFullViewer::BuildFlag flag = BUILD_ALL,
                   SoXtViewer::Type type = BROWSER);
  ~SoXtFlyViewer();

  virtual void setViewing(SbBool enable);
  virtual void viewAll(void);
  virtual void resetToHomePosition(void);
  virtual void setCamera(SoCamera * camera);
  virtual void setCursorEnabled(SbBool enable);
  virtual void setCameraType(SoType type);

protected:
  SoXtFlyViewer(Widget parent,
                   const char * const name, 
                   SbBool embed, 
                   SoXtFullViewer::BuildFlag flag, 
                   SoXtViewer::Type type, 
                   SbBool build);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual SbBool processSoEvent(const SoEvent * const event);
  virtual void setSeekMode(SbBool enable);
  virtual void actualRedraw(void);

  virtual void rightWheelMotion(float value);

  virtual void afterRealizeHook(void);

private:
  class SoXtFlyViewerP * pimpl;
  friend class SoXtFlyViewerP;
};

// ************************************************************************

#endif // ! SOXT_FLYVIEWER_H
