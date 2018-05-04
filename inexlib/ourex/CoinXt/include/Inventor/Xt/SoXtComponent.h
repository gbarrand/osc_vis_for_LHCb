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

#ifndef SOXT_COMPONENT_H
#define SOXT_COMPONENT_H

#include <Inventor/SbLinear.h>
#include <Inventor/Xt/SoXtObject.h>

#ifdef __COIN_SOQT__
class QWidget;
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
#include <X11/Intrinsic.h>
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
#include <gtk/gtk.h>
#endif // __COIN_SOGTK__
#ifdef __COIN_SOWIN__
#include <windows.h>
#endif // __COIN_SOWIN__


class SoXtComponent;
class SoXtCursor;

typedef void SoXtComponentCB(void * user, SoXtComponent * component);
typedef void SoXtComponentVisibilityCB(void * user, SbBool visible);

// *************************************************************************

class SOXT_DLL_API SoXtComponent : public SoXtObject {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtComponent, SoXtObject);

public:
  virtual ~SoXtComponent();

  virtual void show(void);
  virtual void hide(void);

  virtual void setComponentCursor(const SoXtCursor & cursor);
  static void setWidgetCursor(Widget w, const SoXtCursor & cursor);

  SbBool isFullScreen(void) const;
  SbBool setFullScreen(const SbBool onoff);

  SbBool isVisible(void);
  SbBool isTopLevelShell(void) const;

  Widget getWidget(void) const;
  Widget getBaseWidget(void) const;
  Widget getShellWidget(void) const;
  Widget getParentWidget(void) const;

  void setSize(const SbVec2s size);
  SbVec2s getSize(void) const;

  void setTitle(const char * const title);
  const char * getTitle(void) const;
  void setIconTitle(const char * const title);
  const char * getIconTitle(void) const;

  const char * getWidgetName(void) const;
  const char * getClassName(void) const;

  void setWindowCloseCallback(SoXtComponentCB * const func,
                              void * const user = NULL);
  static SoXtComponent * getComponent(Widget widget);

  static void initClasses(void);

protected:
  SoXtComponent(Widget const parent = NULL,
                   const char * const name = NULL,
                   const SbBool embed = TRUE);

  virtual void afterRealizeHook(void);

  // About the wrapping below: this variable was added after SoQt 1.0,
  // and before SoXt 1.1. To be able to release SoQt 1.1 from this
  // same branch, sizeof(SoQtComponent) needs to be the same as for
  // SoQt 1.0, which means we can't add this variable for SoQt.
#ifndef __COIN_SOQT__
  SbBool firstRealize;
#endif // __COIN_SOQT__

  void setClassName(const char * const name);
  void setBaseWidget(Widget widget);

  void registerWidget(Widget widget);
  void unregisterWidget(Widget widget);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void sizeChanged(const SbVec2s & size);

  void addVisibilityChangeCallback(SoXtComponentVisibilityCB * const func,
                                   void * const user = NULL);
  void removeVisibilityChangeCallback(SoXtComponentVisibilityCB * const func,
                                      void * const user = NULL);

private:
  class SoXtComponentP * pimpl;
  friend class SoGuiComponentP;
  friend class SoXtComponentP;

  // FIXME!: audit and remove as much as possible of the remaining
  // toolkit specific parts below. 20020117 mortene.

#ifdef __COIN_SOXT__
public:
  Display * getDisplay(void);
  void fitSize(const SbVec2s size);
  // FIXME: I guess these should really be part of the common
  // API. 20011012 mortene.
  void addWindowCloseCallback(SoXtComponentCB * callback, void * closure = NULL);
  void removeWindowCloseCallback(SoXtComponentCB * callback, void * closure = NULL);

protected:
  // FIXME: I guess this should perhaps be part of the common API?
  // 20011012 mortene.
  void invokeVisibilityChangeCallbacks(const SbBool enable) const;
  void invokeWindowCloseCallbacks(void) const;
  virtual void windowCloseAction(void);

private:
  // FIXME: get rid of this? 20011012 mortene.
  static void event_handler(Widget, XtPointer, XEvent *, Boolean *);
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__
protected:
  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
private:
  static gint eventHandler(GtkWidget * object, GdkEvent * event, gpointer closure);
#endif // __COIN_SOGTK__
};

// *************************************************************************

#endif // ! SOXT_COMPONENT_H
