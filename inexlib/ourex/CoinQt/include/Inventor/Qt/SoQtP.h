#ifndef SOQTP_H
#define SOQTP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef SOQT_INTERNAL
#error this is a private header file
#endif /* !SOQT_INTERNAL */

#include <QtCore/qglobal.h> //G.Barrand

#if QT_VERSION < 0x050000 // pre Qt 5
#include <QtGui/qapplication.h> //G.Barrand QtGui
#include <QtGui/qwidget.h> //G.Barrand QtGui
#else
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qwidget.h>
#endif

#include <QtCore/qtimer.h> //G.Barrand QtCore

#include <Inventor/Qt/SoGuiP.h>

// ************************************************************************

typedef int SoQtP_XErrorHandler(void *, void *);

// This class contains private data and methods used within the SoQt
// class.

class SoQtP : public QObject, public SoGuiP
{
  Q_OBJECT

public:
  static SoQtP * soqt_instance(void);

  static int DEBUG_X11SYNC;
  static const char * SOQT_XSYNC;
  static int X11_ERRORHANDLER;
  static const char * SOQT_X11_ERRORHANDLER;
  static SoQtP_XErrorHandler * previous_handler;
  static int X11Errorhandler(void * d, void * ee);

  static int DEBUG_LISTMODULES;

  static int BRIL_X11_SILENCER;

  static QWidget * mainwidget;
  static bool didcreatemainwidget;
  static QApplication * appobject;
  static SbBool madeappobject;
  static QTimer * timerqueuetimer;
  static QTimer * idletimer;
  static QTimer * delaytimeouttimer;

  static SoQtP * slotobj;

public slots:
  void slot_timedOutSensor(void);
  void slot_idleSensor(void);
  void slot_delaytimeoutSensor(void);
};

// ************************************************************************

#endif // ! SOQTP_H
