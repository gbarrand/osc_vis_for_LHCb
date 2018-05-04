/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
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

// 

#ifndef SOGTK_COMPONENT_H
#define SOGTK_COMPONENT_H

#include <Inventor/SbLinear.h>
#include <Inventor/Gtk/SoGtkObject.h>

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
#include <wtypes.h>
#endif // __COIN_SOWIN__
#ifdef __COIN_SOMAC__
// WORK-IN-PROGRESS
#endif // __COIN_SOMAC__


class SoGtkComponent;
class SoGtkCursor;

typedef void SoGtkComponentCB(void * user, SoGtkComponent * component);
typedef void SoGtkComponentVisibilityCB(void * user, SbBool visible);

// *************************************************************************

class SOGTK_DLL_API SoGtkComponent : public SoGtkObject {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkComponent, SoGtkObject);
  

public:
  virtual ~SoGtkComponent(void);

  virtual void show(void);
  virtual void hide(void);

  virtual void setComponentCursor(const SoGtkCursor & cursor);
  static void setWidgetCursor(GtkWidget * w, const SoGtkCursor & cursor);

  SbBool isFullScreen(void) const;
  SbBool setFullScreen(const SbBool onoff);

  SbBool isVisible(void);
  SbBool isTopLevelShell(void) const;

  GtkWidget * getWidget(void) const;
  GtkWidget * baseWidget(void) const;
  GtkWidget * getShellWidget(void) const;
  GtkWidget * getParentWidget(void) const;

#ifdef __COIN_SOWIN__
  HWND getBaseWidget(void) const;
  HWND setFocusProxy(HWND widget);
  HWND getFocusProxy(void);
#endif // __COIN_SOWIN__
#ifdef __COIN_SOXT__
  Widget getBaseWidget(void) const;
  Display * getDisplay(void);
  void fitSize(const SbVec2s size);
  // FIXME: I guess these should really be part of the common
  // API. 20011012 mortene.
  void addWindowCloseCallback(SoXtComponentCB * callback, void * closure = NULL);
  void removeWindowCloseCallback(SoXtComponentCB * callback, void * closure = NULL);
#endif // __COIN_SOXT__

  void setSize(const SbVec2s size);
  SbVec2s getSize(void) const;

  void setTitle(const char * const title);
  const char * getTitle(void) const;
  void setIconTitle(const char * const title);
  const char * getIconTitle(void) const;

  const char * getWidgetName(void) const;
  const char * getClassName(void) const;

  void setWindowCloseCallback(SoGtkComponentCB * const func,
                              void * const user = NULL);
  static SoGtkComponent * getComponent(GtkWidget * const widget);

  static void initClasses(void);

protected:
  SoGtkComponent(GtkWidget * const parent = NULL,
                   const char * const name = NULL,
                   const SbBool embed = TRUE);

  virtual void afterRealizeHook(void);

  void setClassName(const char * const name);
  void setBaseWidget(GtkWidget * widget);

#ifdef __COIN_SOWIN__
  HWND buildFormWidget(HWND parent);
  virtual void windowCloseAction(void);
  void setResize(SbBool set);
  SbBool getResize(void);
  SbBool realized;
#endif // __COIN_SOWIN__
#ifdef __COIN_SOXT__
  // FIXME: I guess this should perhaps be part of the common API?
  // 20011012 mortene.
  virtual void windowCloseAction(void);
  void invokeVisibilityChangeCallbacks(const SbBool enable) const;
  void invokeWindowCloseCallbacks(void) const;

  // FIXME: get rid of these two? 20011012 mortene.
  static char * getlabel(unsigned int what);
  SbBool firstRealize;

  // FIXME: should this really be protected? Same goes for the other
  // toolkits' eventHandler() functions. They also have nameclashes
  // with the static So*GLWidget::eventHandler() function. 20011024 mortene.
  virtual Boolean sysEventHandler(Widget widget, XEvent * event);
#endif // __COIN_SOXT__

  void registerWidget(GtkWidget * widget);
  void unregisterWidget(GtkWidget * widget);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void sizeChanged(const SbVec2s & size);

  void addVisibilityChangeCallback(SoGtkComponentVisibilityCB * const func,
                                   void * const user = NULL);
  void removeVisibilityChangeCallback(SoGtkComponentVisibilityCB * const func,
                                      void * const user = NULL);

  void openHelpCard(const char * const name);

#ifdef __COIN_SOQT__
  virtual bool eventFilter(QObject * obj, QEvent * e);
#endif // __COIN_SOQT__

#ifdef __COIN_SOGTK__
  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
#endif // __COIN_SOGTK__


#ifdef __COIN_SOQT__
private slots:
  void widgetClosed(void);
#endif // __COIN_SOQT__

private:
  class SoGtkComponentP * pimpl;
  // FIXME: get rid of this? Possibly used by SoGtkComponent and
  // SoWinComponent. 20011012 mortene.
  friend class SoGtkComponentP;

#ifdef __COIN_SOXT__
  // FIXME: get rid of this? 20011012 mortene.
  static void event_handler(Widget, XtPointer, XEvent *, Boolean *);
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__
  static gint eventHandler(GtkWidget * object, GdkEvent * event, gpointer closure);
#endif // __COIN_SOGTK__
};

// *************************************************************************

#endif // ! SOGTK_COMPONENT_H
