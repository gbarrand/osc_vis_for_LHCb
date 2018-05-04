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

#if SOGTK_DEBUG
static const char rcsid[] =
  "";
#endif // SOGTK_DEBUG

/*!
  \class SoGtkPopupMenu Inventor/Gtk/widgets/SoGtkPopupMenu.h
  \brief The SoGtkPopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.

  The point of this class is to enable client code to be exactly the
  same for all GUI libraries, even without being template-based.
*/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/widgets/SoGtkPopupMenu.h>
#include <Inventor/Gtk/widgets/GtkNativePopupMenu.h>

// *************************************************************************

// Documentation for abstract methods.

/*!
  \fn void SoGtkPopupMenu::popUp(GtkWidget * inside, int x, int y)

  Invokes this popup menu instance.

  The coordinates for the popup will be used relative to the origo of
  the widget argument.
*/

// *************************************************************************

// The private data for the SoGtkPopupMenu.

class SoGtkPopupMenuP {
public:
  SbIntList * menuitems;
  SbIntList * radiogroups;
  SbPList * callbacks;
  SbPList * userdata;
};


#define PRIVATE(o) (o->pimpl)

// *************************************************************************

/*!
  Instantiation method.

  SoGtkPopupMenu is implemented with this "factory" method instead of
  a public constructor to provide a completely generic interface on
  top of the toolkit's native popup menu implementation.
*/
SoGtkPopupMenu *
SoGtkPopupMenu::createInstance(void)
{
  return new GtkNativePopupMenu();
}


/*!
  The class constructor is protected, as SoGtkPopupMenu uses a factory
  instantiation method.
*/
SoGtkPopupMenu::SoGtkPopupMenu(void)
{
  PRIVATE(this) = new SoGtkPopupMenuP;

  PRIVATE(this)->menuitems = new SbIntList;
  PRIVATE(this)->radiogroups = new SbIntList;
  PRIVATE(this)->callbacks = NULL;
  PRIVATE(this)->userdata = NULL;
} // SoGtkPopupMenu()

/*!
  Destructor.
*/
SoGtkPopupMenu::~SoGtkPopupMenu()
{
  delete PRIVATE(this)->menuitems;
  delete PRIVATE(this)->radiogroups;
  delete PRIVATE(this)->callbacks;
  delete PRIVATE(this)->userdata;
} // ~SoGtkPopupMenu()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::setMenuItemMarked(
  int itemid,
  SbBool marked)
{
  int groupid = this->getRadioGroup(itemid);
  if (groupid == -1) {
    this->_setMenuItemMarked(itemid, marked);
    return;
  }
  const int numGroupedItems = PRIVATE(this)->menuitems->getLength();
  int groupsize = 0;
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if (((*PRIVATE(this)->radiogroups)[i] == groupid) &&
         ((*PRIVATE(this)->menuitems)[i] != -1))
      groupsize++;
  }
  if (this->getMenuItemMarked(itemid) == marked) {
#if SOGTK_DEBUG && 0
    SoDebugError::postWarning("SoGtkPopupMenu::SetMenuItemEnabled",
      "Marked state already '%s'.", marked ? "on" : "off");
#endif // SOGTK_DEBUG
  } else if (groupsize == 0) {
    assert(0 && "how weird...");
  } else if (groupsize == 1) {
    this->_setMenuItemMarked(itemid, marked);
  } else if ((groupsize == 2) ||
              ((groupsize > 2) && marked)) {
    for (i = 0; i < numGroupedItems; i++) {
      if (((*PRIVATE(this)->radiogroups)[i] == groupid) &&
           ((*PRIVATE(this)->menuitems)[i] != -1)) {
        this->_setMenuItemMarked((*PRIVATE(this)->menuitems)[i],
          ((*PRIVATE(this)->menuitems)[i] == itemid) ? marked :
          ((groupsize == 2) ? (! marked) : FALSE));
      }
    }
  } else {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkPopupMenu::SetMenuItemEnabled",
      "Couldn't handle request.  "
      "Consider that radio grouping is handled automatically.");
#endif // SOGTK_DEBUG
  }
} // setMenuItemMarked()

// *************************************************************************

/*!
*/

int
SoGtkPopupMenu::newRadioGroup(
  int groupid)
{
  const int numGroupedItems = PRIVATE(this)->radiogroups->getLength();
  int id = groupid;
  if (id != -1) {
    SbBool hit = FALSE;
    int i;
    for (i = 0; i < numGroupedItems && hit == FALSE; i++) {
      if ((*PRIVATE(this)->radiogroups)[i] == id)
        hit = TRUE;
    }
    if (hit == TRUE) {
#if SOGTK_DEBUG
      SoDebugError::postInfo("SoGtkPopupMenu::NewRadioGroup",
        "requested group id already taken");
#endif // SOGTK_DEBUG
      return -1; 
    }
  } else {
    id = 0;
    SbBool found;
    do {
      id++;
      found = FALSE;
      int i;
      for (i = 0; i < numGroupedItems && found == FALSE; i++) {
        if ((*PRIVATE(this)->radiogroups)[i] == id)
          found = TRUE;
      }
    } while (found == TRUE);
  }
  // id is OK here
  PRIVATE(this)->menuitems->append(-1); // fake menu item
  PRIVATE(this)->radiogroups->append(id); // register id in array
  return id;
} // newRadioGroup()

/*!
*/

int
SoGtkPopupMenu::getRadioGroup(
  int itemid)
{
  assert(itemid != -1);
  const int numGroupedItems = PRIVATE(this)->radiogroups->getLength();
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if ((*PRIVATE(this)->menuitems)[i] == itemid)
      return (*PRIVATE(this)->radiogroups)[i];
  }
  return -1;
} // getRadioGroup()

/*!
*/

int
SoGtkPopupMenu::getRadioGroupSize(
  int groupid)
{
  int count = 0;
  const int numItems = PRIVATE(this)->radiogroups->getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    if ((*PRIVATE(this)->radiogroups)[i] == groupid &&
         (*PRIVATE(this)->menuitems)[i] != -1)
      count++;
  }
  return count;
} // getRadioGroupSize()

/*!
*/

void
SoGtkPopupMenu::addRadioGroupItem(
  int groupid,
  int itemid)
{
  PRIVATE(this)->menuitems->append(itemid);
  PRIVATE(this)->radiogroups->append(groupid);
} // addRadioGroupItem()

/*!
*/

void
SoGtkPopupMenu::removeRadioGroupItem(
  int itemid)
{
  assert(itemid != -1);
  const int numGroupedItems = PRIVATE(this)->menuitems->getLength();
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if ((*PRIVATE(this)->menuitems)[i] == itemid) {
      PRIVATE(this)->menuitems->remove(i);
      PRIVATE(this)->radiogroups->remove(i);
      return;
    }
  }
} // removeRadioGroupItem()

void
SoGtkPopupMenu::setRadioGroupMarkedItem(
  int itemid)
{
  const int numItems = PRIVATE(this)->menuitems->getLength();
  int i;
  int groupid = -1;
  for (i = 0; i < numItems && groupid == -1; i++) {
    if ((*PRIVATE(this)->menuitems)[i] == itemid)
      groupid = (*PRIVATE(this)->radiogroups)[i];
  }
  if (groupid == -1)
    return;
  for (i = 0; i < numItems; i++) {
    if ((*PRIVATE(this)->radiogroups)[i] == groupid) {
      int item = (*PRIVATE(this)->menuitems)[i];
      if (item != -1 && item != itemid)
        this->setMenuItemMarked(item, FALSE);
    }
  }
} // setRadioGroupItemEnabled()

int
SoGtkPopupMenu::getRadioGroupMarkedItem(
  int groupid)
{
  const int numItems = PRIVATE(this)->menuitems->getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    if ((*PRIVATE(this)->radiogroups)[i] == groupid) {
      int item = (*PRIVATE(this)->menuitems)[i];
      if (item != -1 && this->getMenuItemMarked(item))
        return item;
    }
  }
  return -1;
} // setRadioGroupItemEnabled()

// *************************************************************************

/*!
*/

void
SoGtkPopupMenu::addMenuSelectionCallback(
  SoGtkMenuSelectionCallback * callback,
  void * user)
{
  if (PRIVATE(this)->callbacks == NULL) {
    PRIVATE(this)->callbacks = new SbPList;
    PRIVATE(this)->userdata = new SbPList;
  }
  PRIVATE(this)->callbacks->append((void *) callback);
  PRIVATE(this)->userdata->append(user);
} // addMenuSelectionCallback()

/*!
*/

void
SoGtkPopupMenu::removeMenuSelectionCallback(
  SoGtkMenuSelectionCallback * callback,
  void * user)
{
  if (PRIVATE(this)->callbacks == NULL) {
#if SOGTK_DEBUG
    SoDebugError::postInfo("SoGtkPopupMenu::RemoveMenuSelectionCallback",
      "no callbacks at all");
#endif // SOGTK_DEBUG
    return;
  }
  int numCallbacks = PRIVATE(this)->callbacks->getLength();
  int i;
  for (i = 0; i < numCallbacks; i++) {
    if ((*PRIVATE(this)->callbacks)[i] == (void *) callback &&
         (*PRIVATE(this)->userdata)[i] == user) {
      PRIVATE(this)->callbacks->remove(i);
      PRIVATE(this)->userdata->remove(i);
      i--;
      numCallbacks--;
    }
  }
} // removeMenuSelectionCallback()

/*!
*/

void
SoGtkPopupMenu::invokeMenuSelection(
  int itemid)
{
  if (PRIVATE(this)->callbacks == NULL)
    return;
  const int numCallbacks = PRIVATE(this)->callbacks->getLength();
  int i;
  for (i = 0; i < numCallbacks; i++) {
    SoGtkMenuSelectionCallback * cb = (SoGtkMenuSelectionCallback *)
      (*PRIVATE(this)->callbacks)[i];
    (*cb)(itemid, (*PRIVATE(this)->userdata)[i]);
  }
} // invokeMenuSelection()

// *************************************************************************

// silence some compilers
#if SOGTK_DEBUG
static const char * getSoGtkPopupMenuRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

