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

#ifndef SOXT_6DOFEVENTS_H
#define SOXT_6DOFEVENTS_H

/*
  Customized XEvent-like structures
*/

#define soxt6dofDeviceButtonPressedEvent   (LASTEvent + 1)
#define soxt6dofDeviceButtonReleasedEvent  (LASTEvent + 2)
#define soxt6dofDevicePressureEvent        (LASTEvent + 3)

typedef struct {
  int type;
  unsigned long serial;
  Bool send_event;
  Display * display;
  Window window;
  Time time;
  unsigned int state;
  unsigned int button;
  unsigned int buttons;
} SoXt6dofDeviceButtonEvent;
typedef SoXt6dofDeviceButtonEvent SoXt6dofDeviceButtonPressedEvent;
typedef SoXt6dofDeviceButtonEvent SoXt6dofDeviceButtonReleasedEvent;

typedef struct {
  int type;
  unsigned long serial;
  Bool send_event;
  Display * display;
  Window window;
  Time time;
  float trans_x, trans_y, trans_z;
  float rot_x, rot_y, rot_z;
} SoXt6dofDevicePressureEvent;

#endif // !SOXT_6DOFEVENTS_H
