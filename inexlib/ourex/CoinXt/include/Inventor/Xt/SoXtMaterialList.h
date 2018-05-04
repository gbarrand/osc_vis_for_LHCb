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

#ifndef SOXT_MATERIALLIST_H
#define SOXT_MATERIALLIST_H

#include <Inventor/Xt/SoXtComponent.h>

class SoMaterial;
class SoAnyMaterialList;

typedef void SoXtMaterialListCB(void * closure, const SoMaterial * material);

// *************************************************************************

class SOXT_DLL_API SoXtMaterialList : public SoXtComponent {
  SOXT_OBJECT_HEADER(SoXtMaterialList, SoXtComponent);

public:
  SoXtMaterialList(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE,
    const char * const dir = NULL);
  ~SoXtMaterialList(void);

  void addCallback(
    SoXtMaterialListCB * const callback,
    void * const closure = NULL);
  void removeCallback(
    SoXtMaterialListCB * const callback,
    void * const closure = NULL);

protected:
  SoXtMaterialList(
    Widget parent,
    const char * const name,
    SbBool embed,
    const char * const dir,
    SbBool build);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  Widget buildWidget(Widget parent);
  Widget buildPulldownMenu(Widget parent);

private:
  void constructor(const char * const dir, const SbBool build);

  // FIXME: remove all this and set it up for use with a
  // SoGuiMaterialListP class. 2001???? larsa.

  void selectionCallback(int i);
  static void selection_cb(Widget, void *, void *);

  void paletteMenuCallback(Widget);
  static void palette_menu_cb(Widget, void *, void *);


#ifdef __COIN_SOGTK__
  void menuactivation(GtkWidget * menuitem);
  static void menuactivationCB(GtkObject * obj, gpointer closure);

  void itemactivation(int item);
  static void itemactivationCB(GtkObject * obj, gint row, gint column,
                               GdkEvent * event, gpointer closure);
#endif // __COIN_SOGTK__

  Widget listwidget;

  SoAnyMaterialList * common;

}; // public SoXtMaterialList

// *************************************************************************

#endif // ! SOXT_MATERIALLIST_H
