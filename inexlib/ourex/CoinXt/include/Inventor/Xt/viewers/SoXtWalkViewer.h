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

#ifndef SOXT_WALKVIEWER_H
#define SOXT_WALKVIEWER_H

#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

class SOXT_DLL_API SoXtWalkViewer : public SoXtConstrainedViewer {
  SOXT_OBJECT_HEADER(SoXtWalkViewer, SoXtConstrainedViewer);

public:
  SoXtWalkViewer(Widget parent = NULL,
                    const char * name = NULL,
                    SbBool embed = TRUE,
                    SoXtFullViewer::BuildFlag flag = BUILD_ALL,
                    SoXtViewer::Type type = BROWSER);
  ~SoXtWalkViewer();

  virtual void setViewing(SbBool enable);
  virtual void setCamera(SoCamera * camera);
  virtual void setCursorEnabled(SbBool enable);

  virtual void setCameraType(SoType type);

protected:
  SoXtWalkViewer(Widget parent,
                    const char * name,
                    SbBool embed,
                    SoXtFullViewer::BuildFlag flag,
                    SoXtViewer::Type type,
                    SbBool build);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void processEvent(XAnyEvent * event);

  virtual void setSeekMode(SbBool enable);
  virtual void actualRedraw(void);

  virtual void rightWheelMotion(float value);

  virtual Widget buildLeftTrim(Widget parent);

private:
  class SoXtWalkViewerP * pimpl;
  friend class SoGuiWalkViewerP;
  friend class SoXtWalkViewerP;

/// FIXME!: merge the So*WalkViewer.h parts in properly. 20020109 mortene. ////

#ifdef __COIN_SOXT__
protected:
  virtual void leftWheel2Start(void);
  virtual void leftWheel2Motion(float value);
  virtual void leftWheel2Finish(void);
  float getLeftWheel2Value(void) const;
  void setLeftWheel2String(char * string);

private:
  void constructor(SbBool build);

  Widget heightwheel;
  float heightvalue;

  void wheelarmed(void);
  static void wheelarmedCB(Widget, XtPointer, XtPointer);
  void wheelchanged(float value);
  static void wheelchangedCB(Widget, XtPointer, XtPointer);
  void wheeldisarmed(void);
  static void wheeldisarmedCB(Widget, XtPointer, XtPointer);
#endif // ! __COIN_SOXT__

#ifdef __COIN_SOGTK__
private:
  void constructor(const SbBool build);
#endif // ! __COIN_SOGTK__
};

#endif // ! SOXT_WALKVIEWER_H
