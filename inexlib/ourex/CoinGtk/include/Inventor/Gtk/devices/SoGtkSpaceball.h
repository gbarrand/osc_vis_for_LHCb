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

#ifndef SOGTK_SPACEBALL_H
#define SOGTK_SPACEBALL_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>
#include <Inventor/SbLinear.h>

// FIXME: these includes only needed because of SoWin-specific private
// functions -- get rid of it after fixing up the header file to be a
// 100% "pure" template. 20011210 mortene.
#include <Inventor/events/SoMotion3Event.h>
#include <Inventor/events/SoSpaceballButtonEvent.h>

// *************************************************************************

class SOGTK_DLL_API SoGtkSpaceball : public SoGtkDevice {
  SOGTK_OBJECT_HEADER(SoGtkSpaceball, SoGtkDevice);
  

public:
  enum Mask {
    MOTION_EVENT  = 0x01,  MOTION = MOTION_EVENT,
    PRESS_EVENT   = 0x02,  PRESS = PRESS_EVENT,
    RELEASE_EVENT = 0x04,  RELEASE = RELEASE_EVENT,
    ALL_EVENTS = MOTION_EVENT | PRESS_EVENT | RELEASE_EVENT,
    ALL = ALL_EVENTS
  };

  enum DeviceType {
    SPACE_BALL             = 0x01,
    MAGELLAN_SPACE_MOUSE   = 0x02
  };

  SoGtkSpaceball(int events = ALL_EVENTS);
#ifdef __COIN_SOXT__ // FIXME: get rid of this abominable #ifdef. 20011229 mortene.
  SoXtSpaceball(Display * display, int events = SoXtSpaceball::ALL_EVENTS);
#endif // __COIN_SOXT__
  virtual ~SoGtkSpaceball();

  virtual void enable(GtkWidget * widget, SoGtkEventHandler * handler,
                      void * closure
#ifdef __COIN_SOXT__ // FIXME: get rid of this abominable #ifdef. 20011229 mortene.
                      , Window window = NULL
#endif // __COIN_SOXT__
                      );
  virtual void disable(GtkWidget * widget, SoGtkEventHandler * handler,
                       void * closure);

  virtual const SoEvent * translateEvent(GdkEvent * event);

  void setRotationScaleFactor(float f);
  float getRotationScaleFactor(void) const;
  void setTranslationScaleFactor(float f);
  float getTranslationScaleFactor(void) const;

#ifdef __COIN_SOXT__ // FIXME: make a "pure" template. 20011210 mortene.
  static SbBool exists(Display * display = NULL);
#else
  static SbBool exists(void);
#endif

  void setFocusToWindow(SbBool flag);
  SbBool isFocusToWindow(void) const;

private:
  class SoGtkSpaceballP * pimpl;

#ifdef __COIN_SOWIN__ // FIXME: make a "pure" template. 20011210 mortene.
  UINT eventMask;
  SoMotion3Event * motionEvent;
  SoSpaceballButtonEvent * buttonEvent;
  HWND parentWidget;
  float rotScale, transScale;
  SoMotion3Event * translateMotionEvent(MSG * motionEvent);
  SoSpaceballButtonEvent * translateButtonEvent(MSG * buttonEvent, SoButtonEvent::State whichState);
  SoMotion3Event * translateMotionEventMagellan(MSG * motionEvent);
#endif // __COIN_SOWIN__
#ifdef __COIN_SOXT__ // FIXME: make a "pure" template. 20011210 mortene.
  int events;
  float rotationFactor;
  float translationFactor;
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__ // FIXME: make a "pure" template. 20011210 mortene.
  int events;
  float rotationscale;
  float translationscale;
  SbBool focustowindow;
#endif // __COIN_SOGTK__
};

// *************************************************************************

#endif // ! SOGTK_SPACEBALL_H
