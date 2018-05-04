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

#ifndef SOXT_KEYBOARD_H
#define SOXT_KEYBOARD_H

#include <Inventor/Xt/devices/SoXtDevice.h>

class SoKeyboardEvent;

// *************************************************************************

#define SO_XT_ALL_KEYBOARD_EVENTS SoXtKeyboard::ALL

class SOXT_DLL_API SoXtKeyboard : public SoXtDevice {
  SOXT_OBJECT_HEADER(SoXtKeyboard, SoXtDevice);

public:
  enum Events {
    KEY_PRESS = 0x01,
    KEY_RELEASE = 0x02,
    ALL_EVENTS = KEY_PRESS | KEY_RELEASE
  };

  SoXtKeyboard(int eventmask = ALL_EVENTS);
  virtual ~SoXtKeyboard(void);

  virtual void enable(Widget widget, SoXtEventHandler * handler, void * closure);
  virtual void disable(Widget widget, SoXtEventHandler * handler, void * closure);

  virtual const SoEvent * translateEvent(XAnyEvent * event);

private:
  class SoXtKeyboardP * pimpl;
  friend class SoXtKeyboardP;
  friend class SoGuiKeyboardP;
};

// *************************************************************************

#endif // ! SOXT_KEYBOARD_H
