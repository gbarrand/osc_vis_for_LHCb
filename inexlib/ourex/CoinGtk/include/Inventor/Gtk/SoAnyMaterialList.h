/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
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

// @configure_input@
// 

#ifndef SOANY_MATERIALLIST_H
#define SOANY_MATERIALLIST_H

#include <Inventor/Gtk/SoGtkMaterialList.h>

class SbPList;
class SoMaterial;

// *************************************************************************

#define SOGTK_BUILTIN_MATERIALS 0x0001

struct SoGtkMaterial {
  const char * name;
  const char * data;
};

struct SoGtkMaterialGroup {
  const char * name;
  short numMaterials;
  SoGtkMaterial ** materials;
  GtkWidget * menuitem;
};

struct SoGtkMaterialDirectory {
  short flags;
  short numGroups;
  SoGtkMaterialGroup ** groups;
  short current;
};

// *************************************************************************

class SOGTK_DLL_API SoAnyMaterialList {
public:
  SoAnyMaterialList(const char * const dir);
  ~SoAnyMaterialList(void);

  void addCallback(
    SoGtkMaterialListCB * const callback, void * const closure);
  void removeCallback(
    SoGtkMaterialListCB * const callback, void * const closure);
  void invokeCallbacks(SoMaterial * material);

  const char * getMaterialDirectoryPath(void) const;
  SoGtkMaterialDirectory * getMaterialDirectory(void);

private:
  void setupBuiltinMaterials(SoGtkMaterialDirectory * const index) const;
  void freeMaterialDirectory(void);

  static char ** getNonemptySubdirs(const char * const path);
  static char ** getRegularFiles(const char * const path);

  static int qsort_comparator(const void * itemA, const void * itemB);

private:
  SbPList * callbacks;
  char * dirpath;
  SoGtkMaterialDirectory * directory;

}; // class SoAnyMaterialList

// *************************************************************************

#endif // ! SOANY_MATERIALLIST_H
