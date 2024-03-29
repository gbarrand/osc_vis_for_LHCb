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

// Class documentation in common/SoGuiComponentCommon.cpp.in.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// FIXME: get rid of this define. We should fix up the compile issues
// wrt Qt 4 properly. 20050629 mortene.
//G.Barrand_Qt4 #define QT3_SUPPORT

#include <QtCore/qglobal.h> //G.Barrand

#if QT_VERSION < 0x050000 // pre Qt 5
#include <QtGui/qwidget.h> //G.Barrand QtGui
#include <QtGui/qmainwindow.h> //G.Barrand QtGui
#include <QtGui/qapplication.h> //G.Barrand QtGui
#else
#include <QtWidgets/qwidget.h> //G.Barrand QtGui
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qapplication.h>
#endif

#include <QtGui/qbitmap.h> //G.Barrand QtGui
#include <QtCore/qmetaobject.h> //G.Barrand QtCore
#include <QtGui/qcursor.h> //G.Barrand QtGui
#include <QtGui/qevent.h> //G.Barrand QtGui

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Qt/SoQtComponentP.h>
#include <Inventor/Qt/moc_SoQtComponentP.icc>

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtComponent.h>
#include <Inventor/Qt/SoQtGLWidget.h>
#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <Inventor/Qt/viewers/SoQtConstrainedViewer.h>
#include <Inventor/Qt/viewers/SoQtFlyViewer.h>
#include <Inventor/Qt/SoAny.h>

#include <soqtdefs.h>

// debug
#define SOQTCOMP_RESIZE_DEBUG 0

static const char nullstring[] = "(null)";

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS // Skip internal class SoQtComponentP.

// The private data and code for the SoQtComponent.

SbDict * SoQtComponentP::cursordict = NULL;

SoQtComponentP::SoQtComponentP(SoQtComponent * o)
  : SoGuiComponentP(o)
{
}

SoQtComponentP::~SoQtComponentP()
{
}

void
SoQtComponentP::fatalerrorHandler(void * userdata)
{
  SoQtComponentP * that = (SoQtComponentP *)userdata;
  that->cleanupQtReferences();
}

void
SoQtComponentP::cleanupQtReferences(void)
{
  // Kill the forwarding of messages to the eventFilter() method, as
  // that can lead to all kinds of problems if a fatal-error triggers
  // during construction of the component (like for instance in the
  // case where no valid OpenGL canvas can be set up for the
  // SoQtGLWidget).
  this->parent->removeEventFilter(this);
}

// Converts from the common generic cursor format to a QCursor
// instance.
QCursor *
SoQtComponentP::getNativeCursor(const SoQtCursor::CustomCursor * cc)
{
  if (SoQtComponentP::cursordict == NULL) { // first call, initialize
    SoQtComponentP::cursordict = new SbDict; // FIXME: mem leak. 20011121 mortene.
  }

  void * qc;
  SbBool b = SoQtComponentP::cursordict->find((unsigned long)cc, qc);
  if (b) { return (QCursor *)qc; }

#define MAXBITMAPWIDTH 32
#define MAXBITMAPHEIGHT 32
#define MAXBITMAPBYTES (((MAXBITMAPWIDTH + 7) / 8) * MAXBITMAPHEIGHT)

  uchar cursorbitmap[MAXBITMAPBYTES];
  uchar cursormask[MAXBITMAPBYTES];
  (void)memset(cursorbitmap, 0x00, MAXBITMAPBYTES);
  (void)memset(cursormask, 0x00, MAXBITMAPBYTES);

  if ( !(cc->dim[0] <= MAXBITMAPWIDTH) )
    printf("cursor bitmap width too large: %d\n", cc->dim[0]);
  if ( !(cc->dim[1] <= MAXBITMAPHEIGHT) )
    printf("cursor bitmap height too large: %d\n", cc->dim[1]);
  assert(cc->dim[0] <= MAXBITMAPWIDTH && "internal bitmap too large");
  assert(cc->dim[1] <= MAXBITMAPHEIGHT && "internal bitmap too large");

  const int BYTEWIDTH = (cc->dim[0] + 7) / 8;
  for (int h=0; h < cc->dim[1]; h++) {
    for (int w=0; w < BYTEWIDTH; w++) {
      const int cursorpos = h * ((MAXBITMAPWIDTH + 7) / 8) + w;
      const int nativepos = h * BYTEWIDTH + w;
      cursorbitmap[cursorpos] = cc->bitmap[nativepos];
      cursormask[cursorpos] = cc->mask[nativepos];
    }
  }

  // Always 32x32 because that's what is recommended in the Qt
  // documentation for QCursor.  At least WinNT 4 will give us
  // "interesting" bugs for other cursor sizes.
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  QBitmap bitmap(32, 32, cursorbitmap, TRUE);
  QBitmap mask(32, 32, cursormask, TRUE);
#else
  QBitmap bitmap = QBitmap::fromData(QSize(32,32),cursorbitmap,QImage::Format_MonoLSB);
  QBitmap mask = QBitmap::fromData(QSize(32,32),cursormask,QImage::Format_MonoLSB);
#endif

  // Sanity checks.
  assert(bitmap.size().width() > 0 && bitmap.size().height() > 0);
  assert(bitmap.size() == mask.size());
  assert(bitmap.depth() == 1);
  assert(mask.depth() == 1);

  // FIXME: currently a memory leak here. 20011121 mortene.
  QCursor * c = new QCursor(bitmap, mask, cc->hotspot[0], cc->hotspot[1]);
  SoQtComponentP::cursordict->enter((unsigned long)cc, c);
  return c;
}

// SLOT for when the user clicks/selects window decoration close.
void
SoQtComponentP::widgetClosed(void)
{
  if (this->closeCB) { this->closeCB(this->closeCBdata, PUBLIC(this)); }
}

// Helps us detect changes in size (base widget and parent widget)
// and visibility status.
bool
SoQtComponentP::eventFilter(QObject * obj, QEvent * e)
{
#if 0 // debug
  static const char eventnaming[][50] = {
    "None", // 0
    "Timer",
    "MouseButtonPress",
    "MouseButtonRelease",
    "MouseButtonDblClick",
    "MouseMove",
    "KeyPress",
    "KeyRelease",
    "FocusIn",
    "FocusOut",
    "Enter",
    "Leave",
    "Paint",
    "Move",
    "Resize",
    "Create",
    "Destroy",
    "Show",
    "Hide",
    "Close",
    "Quit", // 20
    "*error*", "*error*", "*error*", "*error*", "*error*",
    "*error*", "*error*", "*error*", "*error*",
    "Accel", // 30
    "Wheel",
    "AccelAvailable", // 32
    "*error*", "*error*", "*error*", "*error*",
    "*error*", "*error*", "*error*",
    "Clipboard", // 40
    "*error*", "*error*", "*error*", "*error*", "*error*",
    "*error*", "*error*", "*error*", "*error*",
    "SockAct", // 50
    "AccelOverride", "*error*", "*error*", "*error*", "*error*",
    "*error*", "*error*", "*error*", "*error*",
    "DragEnter", // 60
    "DragMove",
    "DragLeave",
    "Drop",
    "DragResponse", // 64
    "*error*", "*error*", "*error*", "*error*", "*error*",
    "ChildInserted", // 70
    "ChildRemoved",
    "LayoutHint", // 72
    "*error*", "*error*", "*error*", "*error*", "*error*",
    "*error*", "*error*",
    "ActivateControl", // 80
    "DeactivateControl"
  };

  SoDebugError::postInfo("SoQtComponent::eventFilter", "%p: %s",
                         obj, eventnaming[e->type()]);
#endif // debug

  // The parent widget could receive resize events while we have still
  // not completed the set up of our own widget, so we need to check
  // for a NULL pointer.
  //
  // (One way to reproduce such a case: set up two SoQtExaminerViewer
  // instances in an MDI workspace, first one, which is then
  // maximized, then the second one instantiated will cause a crash.)
  if (this->widget == NULL) { return FALSE; }

  if (e->type() == QEvent::Resize) {
    QResizeEvent * r = (QResizeEvent *)e;

    if (obj == (QObject *)this->parent) {
#if SOQTCOMP_RESIZE_DEBUG  // debug
      SoDebugError::postInfo("SoQtComponent::eventFilter",
                             "resize on parent (%p) to %p: (%d, %d)",
                             this->parent, this->widget,
                             r->size().width(), r->size().height());
#endif // debug
      this->widget->resize(r->size());
      this->storesize.setValue(r->size().width(), r->size().height());
      PUBLIC(this)->sizeChanged(this->storesize);
    }
    else if (obj == (QObject *)this->widget) {
      this->storesize.setValue(r->size().width(), r->size().height());
      PUBLIC(this)->sizeChanged(this->storesize);
    }
  }
  // Detect visibility changes.
  else if (obj == this->widget &&
           (e->type() == QEvent::Show || e->type() == QEvent::Hide)) {
    if (this->visibilitychangeCBs) {
      for (int i=0; i < this->visibilitychangeCBs->getLength()/2; i++) {
        SoQtComponentVisibilityCB * cb =
          (SoQtComponentVisibilityCB *)(*(this->visibilitychangeCBs))[i*2+0];
        void * userdata = (*(this->visibilitychangeCBs))[i*2+1];
        cb(userdata, e->type() == QEvent::Show ? TRUE : FALSE);
      }
    }
  }

  // It would seem more sensible that we should trigger on
  // QEvent::Create than on QEvent::Show for the afterRealizeHook()
  // method, but the QEvent::Create type is not yet used in Qt (as of
  // version 2.2.2 at least) -- it has just been reserved for future
  // releases.
  if (e->type() == QEvent::Show && !this->realized) {
    this->realized = TRUE;
    PUBLIC(this)->afterRealizeHook();
  }

  return FALSE;
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

SOQT_OBJECT_ABSTRACT_SOURCE(SoQtComponent);

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::initClasses(void)
{
  SoQtComponent::initClass();
  SoQtGLWidget::initClass();
  SoQtRenderArea::initClass();
  SoQtViewer::initClass();
  SoQtFullViewer::initClass();
  SoQtExaminerViewer::initClass();
  SoQtPlaneViewer::initClass();
  SoQtConstrainedViewer::initClass();
#if 0  // TMP DISABLED: walkviewer not properly implemented yet. 20020624 mortene.
  SoQtWalkViewer::initClass();
#endif // TMP DISABLED
  SoQtFlyViewer::initClass();
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
SoQtComponent::SoQtComponent(QWidget * const parent,
                             const char * const name,
                             const SbBool embed)
{
  PRIVATE(this) = new SoQtComponentP(this);

  PRIVATE(this)->realized = FALSE;
  PRIVATE(this)->shelled = FALSE;
  PRIVATE(this)->widget = NULL;
  PRIVATE(this)->parent = parent;
  PRIVATE(this)->closeCB = NULL;
  PRIVATE(this)->closeCBdata = NULL;
  PRIVATE(this)->visibilitychangeCBs = NULL;
  PRIVATE(this)->fullscreen = FALSE;

  if (name)
    PRIVATE(this)->widgetname = name;

  PRIVATE(this)->classname = "SoQtComponent";

  PRIVATE(this)->storesize.setValue(-1, -1);
#ifdef Q_WS_MAC 
  PRIVATE(this)->windowsize.setValue(-1, -1);
#endif

  SoAny::si()->addInternalFatalErrorHandler(SoQtComponentP::fatalerrorHandler,
                                            PRIVATE(this));

  if ((parent == NULL) || ! embed) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    PRIVATE(this)->parent = (QWidget*)new QMainWindow(NULL, name);
#else
    PRIVATE(this)->parent = (QWidget*)new QMainWindow(NULL);
#if QT_VERSION < 0x050000 // pre Qt 5
    PRIVATE(this)->parent->setObjectName(QString::fromAscii(name));
#else
    PRIVATE(this)->parent->setObjectName(QString::fromLatin1(name));
#endif
#endif
    PRIVATE(this)->embedded = FALSE;
    PRIVATE(this)->shelled = TRUE;
  }
  else {
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->embedded = TRUE;
  }

  PRIVATE(this)->parent->installEventFilter(PRIVATE(this));
}

// documented in common/SoGuiComponentCommon.cpp.in.
SoQtComponent::~SoQtComponent()
{
  if (PRIVATE(this)->widget) {
    this->unregisterWidget(PRIVATE(this)->widget);
  }

  delete PRIVATE(this)->visibilitychangeCBs;

  // Had to disable this code, as it caused seg-faults.  FIXME: does
  // that mean there's a leak now? 20020524 mortene.
#if 0
  // If we've got a toplevel widget on our hands it won't
  // automatically be deallocated (there's no real parent widget).
  if (PRIVATE(this)->widget && !PRIVATE(this)->widget->parentWidget()) delete PRIVATE(this)->widget;
#endif

  delete PRIVATE(this);
}

// FIXME: the visibility callback handling could be done in common
// code. 20020624 mortene.

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::addVisibilityChangeCallback(SoQtComponentVisibilityCB * const func,
                                           void * const user)
{
  if (! PRIVATE(this)->visibilitychangeCBs)
    PRIVATE(this)->visibilitychangeCBs = new SbPList;

  PRIVATE(this)->visibilitychangeCBs->append((void *) func);
  PRIVATE(this)->visibilitychangeCBs->append(user);
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::removeVisibilityChangeCallback(SoQtComponentVisibilityCB * const func,
                                              void * const data)
{
#if SOQT_DEBUG
  if (! PRIVATE(this)->visibilitychangeCBs) {
    SoDebugError::postWarning("SoQtComponent::removeVisibilityChangeCallback",
                              "empty callback list");
    return;
  }
#endif // SOQT_DEBUG

  int idx = PRIVATE(this)->visibilitychangeCBs->find((void *) func);
  if (idx != -1) {
    PRIVATE(this)->visibilitychangeCBs->remove(idx);
    PRIVATE(this)->visibilitychangeCBs->remove(idx);
  }

#if SOQT_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoQtComponent::removeVisibilityChangeCallback",
                              "tried to remove non-existant callback");
    return;
  }
#endif // SOQT_DEBUG
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setClassName(const char * const name)
{
  PRIVATE(this)->classname = name;
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setBaseWidget(QWidget * widget)
{
//  SoDebugError::postInfo("SoQtComponent::setBaseWidget", "[invoked]");
  assert(widget);

//  if (PRIVATE(this)->parent)
//    PRIVATE(this)->parent->removeEventFilter(PRIVATE(this));
  if (PRIVATE(this)->widget)
    PRIVATE(this)->widget->removeEventFilter(PRIVATE(this));

  if (PRIVATE(this)->widget) { this->unregisterWidget(PRIVATE(this)->widget); }
  PRIVATE(this)->widget = widget;
  this->registerWidget(PRIVATE(this)->widget);

//  PRIVATE(this)->parent = widget->parentWidget();

#if 0 // debug
  SoDebugError::postInfo("SoQtComponent::setBaseWidget",
                         "widget: %p, parent: %p", w, PRIVATE(this)->parent);
#endif // debug


  if (!PRIVATE(this)->parent || PRIVATE(this)->parent->isTopLevel()) {
    if (PRIVATE(this)->captiontext.isNull()) PRIVATE(this)->captiontext = this->getDefaultTitle();
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    this->setTitle((const char *)PRIVATE(this)->captiontext.local8Bit());
    if (PRIVATE(this)->icontext.isNull()) PRIVATE(this)->icontext = this->getDefaultIconTitle();
    SoQt::getShellWidget(this->getWidget())->setIconText(PRIVATE(this)->icontext);
#else
    this->setTitle((const char *)PRIVATE(this)->captiontext.toLocal8Bit());
    if (PRIVATE(this)->icontext.isNull()) PRIVATE(this)->icontext = this->getDefaultIconTitle();
    SoQt::getShellWidget(this->getWidget())->setWindowIconText(PRIVATE(this)->icontext);
#endif
  }

  if (PRIVATE(this)->widgetname.isNull())
    PRIVATE(this)->widgetname = this->getDefaultWidgetName();

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  PRIVATE(this)->widget->setName(PRIVATE(this)->widgetname);
#else
  PRIVATE(this)->widget->setObjectName(PRIVATE(this)->widgetname);
#endif

  // Need this to auto-detect resize events.
//  if (PRIVATE(this)->parent)
//    PRIVATE(this)->parent->installEventFilter(PRIVATE(this));

  PRIVATE(this)->widget->installEventFilter(PRIVATE(this));

  QObject::connect(PRIVATE(this)->widget, SIGNAL(destroyed()),
                   PRIVATE(this), SLOT(widgetClosed()));

//  if (storesize[0] != -1)
//    PRIVATE(this)->widget->resize(QSize(storesize[0], storesize[1]));
}

// *************************************************************************

/*!
  This function \e must be called by subclasses after the component's
  widget has been otherwise initialized.
*/

/*
void
SoQtComponent::subclassInitialized(
  void)
{
#if SOQT_DEBUG
  if(!PRIVATE(this)->widget) {
    SoDebugError::postWarning("SoQtComponent::subclassInitialized",
                              "Called while no QWidget has been set.");
    return;
  }
#endif // SOQT_DEBUG
}
*/

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::show(void)
{
  if(SOQT_DEBUG && !PRIVATE(this)->widget) { // debug
    SoDebugError::postWarning("SoQtComponent::show",
                              "Called while no QWidget has been set.");
    return;
  }

  if (SOQTCOMP_RESIZE_DEBUG) {  // debug
    SoDebugError::postInfo("SoQtComponent::show-1",
                           "resizing %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           PRIVATE(this)->storesize[0],
                           PRIVATE(this)->storesize[1]);
  }

  if (PRIVATE(this)->shelled) {
    PRIVATE(this)->parent->resize(PRIVATE(this)->storesize[0],
                                  PRIVATE(this)->storesize[1]);
  }
  else {
    PRIVATE(this)->widget->resize(PRIVATE(this)->storesize[0],
                                  PRIVATE(this)->storesize[1]);
  }

  if (SOQTCOMP_RESIZE_DEBUG) {  // debug
    SoDebugError::postInfo("SoQtComponent::show-2",
                           "resized %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           PRIVATE(this)->widget->size().width(),
                           PRIVATE(this)->widget->size().height());
  }

  PRIVATE(this)->widget->topLevelWidget()->show();

  if (SOQTCOMP_RESIZE_DEBUG) {  // debug
    SoDebugError::postInfo("SoQtComponent::show-3",
                           "showed %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           PRIVATE(this)->widget->size().width(),
                           PRIVATE(this)->widget->size().height());
  }

  PRIVATE(this)->widget->raise();

  // FIXME: This is a very, very ugly workaround to force Qt 3.3.x on
  // Mac OS X to repaint our component on startup. Otherwise, all you
  // get is a blank white window. The problem seems to be related to a
  // paint event "getting lost" somewhere (it's registered in
  // SoQtComponent but never makes it to SoQtGLWidget.) This should
  // really be fixed properly but I guess it's better to have things
  // working in the meantime...
  // Note that this workaround is a weird thing in itself: calling
  // raise() twice should not make a difference, since raise() puts
  // the widget to the top of the stack so that "the widget will be
  // visually in front of any overlapping sibling widgets". 
  // kyrah 20040211.
#if (defined Q_WS_MAC && QT_VERSION >= 0x030300)
  PRIVATE(this)->widget->raise();
#endif // Q_WS_MAC

  if (SOQTCOMP_RESIZE_DEBUG) {  // debug
    SoDebugError::postInfo("SoQtComponent::show-4",
                           "raised %p: (%d, %d)",
                           PRIVATE(this)->widget,
                           PRIVATE(this)->widget->size().width(),
                           PRIVATE(this)->widget->size().height());
  }

  this->sizeChanged(PRIVATE(this)->storesize);
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::hide(void)
{
#if SOQT_DEBUG
  if(!PRIVATE(this)->widget) {
    SoDebugError::postWarning("SoQtComponent::hide",
                              "Called while no QWidget has been set.");
    return;
  }
#endif // SOQT_DEBUG

  if (PRIVATE(this)->widget) { PRIVATE(this)->widget->topLevelWidget()->hide(); }
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoQtComponent::isVisible(void)
{
  if (! PRIVATE(this)->widget) { return FALSE; }
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return PRIVATE(this)->widget->isVisible() && PRIVATE(this)->widget->isVisibleToTLW();
#else
  return PRIVATE(this)->widget->isVisible() && PRIVATE(this)->widget->isVisible();
#endif
}

// documented in common/SoGuiComponentCommon.cpp.in.
QWidget *
SoQtComponent::getWidget(void) const
{
  return PRIVATE(this)->widget;
}

// documented in common/SoGuiComponentCommon.cpp.in.
QWidget *
SoQtComponent::getBaseWidget(void) const
{
  return PRIVATE(this)->widget;
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoQtComponent::isTopLevelShell(void) const
{
#if SOQT_DEBUG && 0
  if (! PRIVATE(this)->widget) {
    SoDebugError::postWarning("SoQtComponent::isTopLevelShell",
                              "Called while no QWidget has been set.");
    return FALSE;
  }
#endif // SOQT_DEBUG
  return PRIVATE(this)->embedded ? FALSE : TRUE;
}

// documented in common/SoGuiComponentCommon.cpp.in.
QWidget *
SoQtComponent::getParentWidget(void) const
{
  return PRIVATE(this)->parent;
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setTitle(const char * const title)
{
  PRIVATE(this)->captiontext = title;

  if (this->getWidget()) {
    QWidget * toplevel = this->getWidget();
    while (!toplevel->isTopLevel() ) {
      toplevel = toplevel->parentWidget();
    }
    if (toplevel) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
      toplevel->setCaption(title);
#else
      toplevel->setWindowTitle(title);
#endif
    }
  }
}

// documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoQtComponent::getTitle(void) const
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return
    PRIVATE(this)->captiontext.isNull() ? nullstring : (const char *) PRIVATE(this)->captiontext;
#else
//G.Barrand : the below line induces a temporary std::string :
//return PRIVATE(this)->captiontext.isNull() ? nullstring : (const char *) PRIVATE(this)->captiontext.toStdString().c_str();
  if(PRIVATE(this)->captiontext.isNull()) return nullstring;
  static std::string _s = PRIVATE(this)->captiontext.toStdString();
  return _s.c_str();
#endif
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setIconTitle(const char * const title)
{
  PRIVATE(this)->icontext = title;

  QWidget * w = this->getWidget();
  if (w && this->isTopLevelShell()) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    SoQt::getShellWidget(w)->setIconText(title);
#else
    SoQt::getShellWidget(w)->setWindowIconText(title);
#endif
  }
}

// documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoQtComponent::getIconTitle(void) const
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return PRIVATE(this)->icontext.isNull() ? nullstring : (const char *)PRIVATE(this)->icontext;
#else
//G.Barrand : the below line induces a temporary std::string :
//return PRIVATE(this)->icontext.isNull() ? nullstring : (const char *)PRIVATE(this)->icontext.toStdString().c_str();
  if(PRIVATE(this)->icontext.isNull()) return nullstring;
  static std::string _s = PRIVATE(this)->icontext.toStdString();
  return _s.c_str();
#endif
}

// documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoQtComponent::getWidgetName(void) const
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return
    PRIVATE(this)->widgetname.isNull() ? nullstring : (const char *)PRIVATE(this)->widgetname;
#else
//G.Barrand : the below line induces a temporary std::string :
//return PRIVATE(this)->widgetname.isNull() ? nullstring : (const char *)PRIVATE(this)->widgetname.toStdString().c_str();
  if(PRIVATE(this)->widgetname.isNull()) return nullstring;
  static std::string _s = PRIVATE(this)->widgetname.toStdString();
  return _s.c_str();
#endif
}

// documented in common/SoGuiComponentCommon.cpp.in.
const char *
SoQtComponent::getClassName(void) const
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return (const char *)PRIVATE(this)->classname;
#else
//G.Barrand : the below line induces a temporary std::string :
//return (const char *)PRIVATE(this)->classname.toStdString().c_str();
  static std::string _s = PRIVATE(this)->classname.toStdString();
  return _s.c_str();
#endif
}

// *************************************************************************

// FIXME: the resize handling in SoQtComponent and derived classes is
// really horrible and has provided us with heaps of "interesting"
// bugs. Should clean it up properly.
//
// One important part of the clean-up is to make a decent set of test
// cases to check with (and to later use for testing for
// regressions). The tests should at least cover these usage contexts
// for SoQt component classes:
//
// 
//   * an SoQt-component viewer embedded in other Qt-widgets
//   * a top-level (ie "free window") viewer
//
//   * an embedded SoQtRenderArea
//   * a top-level SoQtRenderArea
//
//   * viewers with and without sidebar- or bottom-decorations
//
//   * changing size for all the above cases by user interaction with
//     window manager decorations
//
//   * changing size programmatically with SoQtComponent::setSize(),
//     before and after component realization
//
// And all the above should be tested with both Qt v2 and Qt v3, on
// both a UNIX/X11 system and an MSWindows system (Qt is supposed to
// behave exactly the same on both platforms, but that's not always
// the case).
//
// 20021024 mortene.

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setSize(const SbVec2s size)
{
#if SOQT_DEBUG
  if((size[0] <= 0) || (size[1] <= 0)) {
    SoDebugError::postWarning("SoQtComponent::setSize",
                              "Invalid size setting: <%d, %d>.",
                              size[0], size[1]);
    return;
  }
#endif // SOQT_DEBUG

#if SOQTCOMP_RESIZE_DEBUG  // debug
  SoDebugError::postInfo("SoQtComponent::setSize",
                         "resize %p: (%d, %d)",
                         PRIVATE(this)->widget,
                         size[0], size[1]);
#endif // debug
  const SbBool yetbuilt = (this->getWidget() != NULL);
  if (yetbuilt) {
    QWidget * shell = this->getShellWidget();
    if (shell) { shell->resize(size[0], size[1]); }
  }
  PRIVATE(this)->storesize = size;
  this->sizeChanged(size);
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbVec2s
SoQtComponent::getSize(void) const
{
  return PRIVATE(this)->storesize;
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::sizeChanged(const SbVec2s & size)
{
  // The default implementation does nothing.
}

// *************************************************************************

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setWindowCloseCallback(SoQtComponentCB * const func,
                                      void * const data)
{
  PRIVATE(this)->closeCB = func;
  PRIVATE(this)->closeCBdata = data;
}

// *************************************************************************

// Documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::afterRealizeHook(void)
{
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoQtComponent::setFullScreen(const SbBool onoff)
{
  if (onoff == PRIVATE(this)->fullscreen) { return TRUE; }

  // FIXME: hmm.. this looks suspicious. Shouldn't we just return
  // FALSE if the (base)widget is not a shellwidget? 20010817 mortene.
  QWidget * w = this->getShellWidget();
  if (w == NULL) w = this->getParentWidget();
  if (w == NULL) w = this->getWidget();
  if (!w) { return FALSE; }

  // FIXME: note that the compile-time binding technique against
  // QWidget::showFullScreen() doesn't work very well with the idea
  // that we'll compile a distribution version of SoQt against the
  // lowest common denominator of Qt versions we support (ie v2.0.0),
  // as that means SoQtComponent::setFullScreen() will never work as
  // expected in the pre-compiled distro version we make.  20010608 mortene.

  // prefer setWindowState() to showFullScreen() since
  // setWindowState() will preserve other window flags/states.
#if HAVE_QWIDGET_SETWINDOWSTATE
  if (onoff) {
#ifdef Q_WS_MAC 
    // Qt/Mac does not remember the window size when going fullscreen,
    // so when going back to windowed mode, the window will be 1x1 pixels
    // small -> we have to store the window size ourselves...
    // FIXME: We should really figure out why this happens, and fix the
    // problem, but I guess having a workaround is better than just having
    // the window "disappear"... kyrah 20041118
    PRIVATE(this)->windowsize[0] = w->size().width();
    PRIVATE(this)->windowsize[1] = w->size().height();
#endif
    w->setWindowState(w->windowState() | Qt::WindowFullScreen);
#ifdef Q_WS_MAC 
    // Explicit show needed for Mac OS X, otherwise the window "vanishes"
    w->show();
#endif
  } else {
    w->setWindowState(w->windowState() & ~Qt::WindowFullScreen);
#ifdef Q_WS_MAC 
    w->resize(QSize(PRIVATE(this)->windowsize[0], PRIVATE(this)->windowsize[1]));
    w->show();
#endif
  }
#elif HAVE_QWIDGET_SHOWFULLSCREEN
  if (onoff) w->showFullScreen();
  else w->showNormal();
#else // HAVE_QWIDGET_SHOWFULLSCREEN
  SoDebugError::postWarning("SoQtComponent::setFullScreen",
                            "SoQt was compiled against version %s of Qt, "
                            "which doesn't have the "
                            "QWidget::showFullScreen() method",
                            QT_VERSION_STR);
  return FALSE;
#endif // !HAVE_QWIDGET_SHOWFULLSCREEN && !HAVE_QWIDGET_SETWINDOWSTATE 
  PRIVATE(this)->fullscreen = onoff;
  return TRUE;
}

// documented in common/SoGuiComponentCommon.cpp.in.
SbBool
SoQtComponent::isFullScreen(void) const
{
  return PRIVATE(this)->fullscreen;
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setComponentCursor(const SoQtCursor & cursor)
{
  SoQtComponent::setWidgetCursor(this->getWidget(), cursor);
}

// documented in common/SoGuiComponentCommon.cpp.in.
void
SoQtComponent::setWidgetCursor(QWidget * w, const SoQtCursor & cursor)
{
  // FIXME: as this function is called all the time when the cursor is
  // grabbed by the window under X11, we should really compare with
  // the previous cursor before doing anything, to avoid spending
  // unnecessary clockcycles during animation. 20011203 mortene.

  if (cursor.getShape() == SoQtCursor::CUSTOM_BITMAP) {

    // Custom cursors do not work correctly in Qt/Mac versions 3.1.0
    // and 3.1.1 - the displayed graphics look totally wrong.
    //
    // The bug has been confirmed fixed in Qt 3.1.2, but instead they
    // introduced another bug: when you click on the widget, the
    // cursor disappears. The Trolls have acknowledged that this is a
    // bug, and that it will be fixed in 3.1.3.
#if defined Q_WS_MAC && ((QT_VERSION == 0x030100) || (QT_VERSION == 0x030101) || (QT_VERSION == 0x030102))
    w->setCursor(QCursor(Qt::arrowCursor));
    // spit out a warning that this is a Qt/Mac bug, not an SoQt problem
    static SbBool warningdisplayed = FALSE;
    if (!warningdisplayed) {
      const char * env = SoAny::si()->getenv("SOQT_NO_QTMAC_BUG_WARNINGS");
      if (!env || !atoi(env))
        SoDebugError::postWarning("SoQtComponent::setWidgetCursor",
                                  "\nThis version of Qt/Mac contains a bug "
                                  "that makes it impossible to use custom\n"
                                  "cursors. Warnings about Qt/Mac bugs "
                                  "can be turned off permanently by setting\n"
                                  "the environment variable "
                                  "SOQT_NO_QTMAC_BUG_WARNINGS=1.\n");
      warningdisplayed = TRUE;
    }
#else 
    const SoQtCursor::CustomCursor * cc = &cursor.getCustomCursor();
    w->setCursor(*SoQtComponentP::getNativeCursor(cc));
#endif
  }
  else {
    switch (cursor.getShape()) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
#else
#define arrowCursor ArrowCursor
#define waitCursor WaitCursor
#define crossCursor CrossCursor
#define upArrowCursor UpArrowCursor
#endif
    case SoQtCursor::DEFAULT:
      w->setCursor(QCursor(Qt::arrowCursor));
      break;

    case SoQtCursor::BUSY:
      w->setCursor(QCursor(Qt::waitCursor));
      break;

    case SoQtCursor::CROSSHAIR:
      w->setCursor(QCursor(Qt::crossCursor));
      break;

    case SoQtCursor::UPARROW:
      w->setCursor(QCursor(Qt::upArrowCursor));
      break;

    default:
      assert(FALSE && "unsupported cursor shape type");
      break;
    }
  }

  // QWidget::setCursor() doesn't have an immediate effect with Qt/Win
  // if the cursor is already positioned over the widget -- and won't
  // change until the cursor position is moved. This is at least true
  // for Qt version 2.1.1.
  //
  // The code below is a simple work-around for this problem -- we
  // just offset the cursor position by a single pixel. There might be
  // a better way to get around the problem, but this seems easy
  // enough.
  //                                                        mortene

  // FIXME: had to disable this as it didn't work under X11 (we get
  // continuous calls to this function, which means the mousecursor is
  // quickly pushed off the screen..). 20011203 mortene.
#if 0
  QPoint p = w->cursor().pos();
  p.setX(p.x() + 1);
  w->cursor().setPos(p);
#endif // disabled
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

