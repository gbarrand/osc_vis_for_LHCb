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

#ifndef SOXT_FULLVIEWER_H
#define SOXT_FULLVIEWER_H

#include <Inventor/Xt/viewers/SoXtViewer.h>

class SoXtPopupMenu;

// *************************************************************************

class SOXT_DLL_API SoXtFullViewer : public SoXtViewer {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtFullViewer, SoXtViewer);

public:
  enum BuildFlag {
    BUILD_NONE       = 0x00,
    BUILD_DECORATION = 0x01,
    BUILD_POPUP      = 0x02,
    BUILD_ALL        = (BUILD_DECORATION | BUILD_POPUP)
  };

  void setDecoration(const SbBool on);
  SbBool isDecoration(void) const;

  void setPopupMenuEnabled(const SbBool on);
  SbBool isPopupMenuEnabled(void) const;

  Widget getAppPushButtonParent(void) const;
  void addAppPushButton(Widget newButton);
  void insertAppPushButton(Widget newButton, int index);
  void removeAppPushButton(Widget oldButton);
  int findAppPushButton(Widget oldButton) const;
  int lengthAppPushButton(void) const;

  Widget getRenderAreaWidget(void) const;

  virtual void setViewing(SbBool on);

  virtual void setComponentCursor(const SoXtCursor & cursor);

protected:
  SoXtFullViewer(Widget parent,
                    const char * name,
                    SbBool embed,
                    BuildFlag flag,
                    Type type,
                    SbBool build);
  ~SoXtFullViewer();

  virtual void sizeChanged(const SbVec2s & size);

  Widget buildWidget(Widget parent);

  virtual void buildDecoration(Widget parent);
  virtual Widget buildLeftTrim(Widget parent);
  virtual Widget buildBottomTrim(Widget parent);
  virtual Widget buildRightTrim(Widget parent);
  Widget buildAppButtons(Widget parent);
  Widget buildViewerButtons(Widget parent);
  virtual void createViewerButtons(Widget parent, SbPList * buttonlist);

  virtual void buildPopupMenu(void);
  virtual void setPopupMenuString(const char * title);
  virtual void openPopupMenu(const SbVec2s position);

  virtual void leftWheelStart(void);
  virtual void leftWheelMotion(float);
  virtual void leftWheelFinish(void);
  float getLeftWheelValue(void) const;
  void setLeftWheelValue(const float value);

  virtual void bottomWheelStart(void);
  virtual void bottomWheelMotion(float);
  virtual void bottomWheelFinish(void);
  float getBottomWheelValue(void) const;
  void setBottomWheelValue(const float value);

  virtual void rightWheelStart(void);
  virtual void rightWheelMotion(float);
  virtual void rightWheelFinish(void);
  float getRightWheelValue(void) const;
  void setRightWheelValue(const float value);

  void setLeftWheelString(const char * const name);
  Widget getLeftWheelLabelWidget(void) const;
  void setBottomWheelString(const char * const name);
  Widget getBottomWheelLabelWidget(void) const;
  void setRightWheelString(const char * const name);
  Widget getRightWheelLabelWidget(void) const;

  virtual SbBool processSoEvent(const SoEvent * const event);

protected:
  Widget leftWheel;
  Widget rightWheel;
  Widget bottomWheel;

  Widget leftDecoration;
  Widget rightDecoration;
  Widget bottomDecoration;

  Widget leftWheelLabel;
  char * leftWheelStr;
  float leftWheelVal;

  Widget rightWheelLabel;
  char * rightWheelStr;
  float rightWheelVal;

  Widget bottomWheelLabel;
  char * bottomWheelStr;
  float bottomWheelVal;

  SoXtPopupMenu * prefmenu;

private:
  // Private class for implementation hiding. The idiom we're using is
  // a variant of what is known as the "Cheshire Cat", and is also
  // described as the "Bridge" pattern in «Design Patterns» by Gamma
  // et al (aka The Gang Of Four).
  class SoXtFullViewerP * pimpl;

  friend class SoGuiFullViewerP;
  friend class SoXtFullViewerP;



// FIXME: get rid of non-templatized code. 20020108 mortene.

#ifdef __COIN_SOXT__ // FIXME: get rid of non-templatized code. 20020108 mortene.
protected:
  Widget buildFunctionsSubmenu(Widget popup);
  Widget buildDrawStyleSubmenu(Widget popup);

  char * popupTitle;
  SbBool popupEnabled;
  SbPList * viewerButtonWidgets;
#endif // __COIN_SOXT__
};

// *************************************************************************

#endif // ! SOXT_FULLVIEWER_H
