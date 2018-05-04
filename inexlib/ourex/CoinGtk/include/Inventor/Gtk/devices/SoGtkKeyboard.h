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

#ifndef SOGTK_KEYBOARD_H
#define SOGTK_KEYBOARD_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>

class SoKeyboardEvent;

// *************************************************************************

#define SO_GTK_ALL_KEYBOARD_EVENTS SoGtkKeyboard::ALL

class SOGTK_DLL_API SoGtkKeyboard : public SoGtkDevice {
  SOGTK_OBJECT_HEADER(SoGtkKeyboard, SoGtkDevice);
  

public:
  enum Events {
    // FIXME: check if the define matches with X11 is really
    // necessary. If not, remove the comments below, if so, document
    // properly this dependency. 20011229 mortene.

    KEY_PRESS = 0x01, // matches X11's KeyPressMask define
    KEY_RELEASE = 0x02, // matches X11's KeyReleaseMask define
    ALL_EVENTS = KEY_PRESS | KEY_RELEASE
  };

  SoGtkKeyboard(int eventmask = ALL_EVENTS);
  virtual ~SoGtkKeyboard(void);

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
  int eventmask;
  SoKeyboardEvent * kbdevent;
};

// *************************************************************************

#endif // ! SOGTK_KEYBOARD_H
