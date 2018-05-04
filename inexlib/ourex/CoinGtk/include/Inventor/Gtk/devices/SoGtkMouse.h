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

#ifndef SOGTK_MOUSE_H
#define SOGTK_MOUSE_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>
// FIXME: this include only needed because of SoWin-specific private
// functions -- get rid of it after fixing up the header file to be a
// 100% "pure" template. 20011210 mortene.
#include <Inventor/events/SoButtonEvent.h>

class SoMouseButtonEvent;
class SoLocation2Event;

// *************************************************************************

class SOGTK_DLL_API SoGtkMouse : public SoGtkDevice {
  SOGTK_OBJECT_HEADER(SoGtkMouse, SoGtkDevice);
  

public:
  enum Events {
    // FIXME: check if the define matches with X11 is really
    // necessary. If not, remove the comments below, if so, document
    // properly this dependency. 20011229 mortene.

    BUTTON_PRESS    = 0x01, // matches X11's ButtonPressMask define
    BUTTON_RELEASE  = 0x02, // matches X11's ButtonReleaseMask define
    POINTER_MOTION  = 0x04, // matches X11's PointerMotionMask define
    BUTTON_MOTION   = 0x08, // matches X11's ButtonMotionMask define

    ALL_EVENTS = BUTTON_PRESS | BUTTON_RELEASE | POINTER_MOTION | BUTTON_MOTION
  };

  SoGtkMouse(int eventmask = ALL_EVENTS);
  virtual ~SoGtkMouse(void);

  virtual void enable(GtkWidget * widget, SoGtkEventHandler * handler,
                      void * closure
#ifdef __COIN_SOXT__ // FIXME: get rid of this abominable #ifdef. 20011229 mortene.
                      , Window window = NULL
#endif // __COIN_SOXT__
                      );
  virtual void disable(GtkWidget * widget, SoGtkEventHandler * handler,
                       void * closure);

  virtual const SoEvent * translateEvent(GdkEvent * event);

private:
  SoMouseButtonEvent * buttonevent;
  SoLocation2Event * locationevent;
  int eventmask;

#ifdef __COIN_SOWIN__ // FIXME: make a "pure" template. 20011210 mortene.
  SoLocation2Event * makeLocationEvent(MSG * msg);
  SoMouseButtonEvent * makeButtonEvent(MSG * msg, SoButtonEvent::State state);
#endif // __COIN_SOWIN__
#ifdef __COIN_SOXT__ // FIXME: make a "pure" template. 20011210 mortene.
  SoLocation2Event * makeLocationEvent(XMotionEvent * event);
  SoMouseButtonEvent * makeButtonEvent(XButtonEvent * event, SoButtonEvent::State state);
#endif // __COIN_SOXT__
}; // class SoGtkMouse

// *************************************************************************

#endif // ! SOGTK_MOUSE_H
