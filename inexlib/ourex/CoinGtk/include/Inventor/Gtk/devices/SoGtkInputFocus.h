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

#ifndef SOGTK_INPUTFOCUS_H
#define SOGTK_INPUTFOCUS_H

#include <Inventor/Gtk/devices/SoGtkDevice.h>

// *************************************************************************

// FIXME: I'm not sure if the X11 enum value matching here is
// necessary or not. Should investigate. 20011211 mortene.
#define sogtkEnterWindowMask 0x01 // Matches X11 EnterWindowMask
#define sogtkLeaveWindowMask 0x02 // Matches X11 LeaveWindowMask


#define SO_GTK_ALL_FOCUS_EVENTS SoGtkInputFocus::ALL_EVENTS;

class SOGTK_DLL_API SoGtkInputFocus : public SoGtkDevice {
  SOGTK_OBJECT_HEADER(SoGtkInputFocus, SoGtkDevice);
  

public:
  enum Events {
    ENTER_WINDOW = sogtkEnterWindowMask,
    LEAVE_WINDOW = sogtkLeaveWindowMask,
    ALL_EVENTS   = sogtkEnterWindowMask | sogtkLeaveWindowMask
  };

  SoGtkInputFocus(int mask = ALL_EVENTS);
  virtual ~SoGtkInputFocus(void);

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

}; // class SoGtkInputFocus

// *************************************************************************

#endif // ! SOGTK_INPUTFOCUS_H
