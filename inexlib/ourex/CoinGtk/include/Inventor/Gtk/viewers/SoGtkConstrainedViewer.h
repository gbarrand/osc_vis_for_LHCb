/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// @configure_input@
// 

#ifndef SOGTK_CONSTRAINEDVIEWER_H
#define SOGTK_CONSTRAINEDVIEWER_H

#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>

// ************************************************************************

class SOGTK_DLL_API SoGtkConstrainedViewer : public SoGtkFullViewer {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkConstrainedViewer, SoGtkFullViewer);
  

public:
  void setUpDirection(const SbVec3f & dir);
  const SbVec3f & getUpDirection(void) const;

  virtual void setCamera(SoCamera * newcam);
  virtual void saveHomePosition(void);
  virtual void resetToHomePosition(void);
  virtual void recomputeSceneSize(void);

protected:
  SoGtkConstrainedViewer(
    GtkWidget * parent,
    const char * const name,
    SbBool embed,
    SoGtkFullViewer::BuildFlag flag, 
    SoGtkViewer::Type type, 
    SbBool build);

  ~SoGtkConstrainedViewer();

  SbVec3f upVector;
  float sceneHeight;
  
  virtual void tiltCamera(float delta);
  virtual void bottomWheelMotion(float value);
  virtual void leftWheelMotion(float value);

  virtual void changeCameraValues(SoCamera * camera);
  void findUpDirection(SbVec2s mouselocation);
  void checkForCameraUpConstrain(void);

private:
  class SoGtkConstrainedViewerP * pimpl;

  void rotateCamera(const float delta) const;

}; // class SoGtkConstrainedViewer

// ************************************************************************

#endif // ! SOGTK_CONSTRAINEDVIEWER_H
