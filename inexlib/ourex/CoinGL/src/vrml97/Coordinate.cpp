/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLCoordinate SoVRMLCoordinate.h Inventor/VRMLnodes/SoVRMLCoordinate.h
  \brief The SoVRMLCoordinate class is used to define 3D coordinates for shapes.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  Coordinate { 
    exposedField MFVec3f point  []      # (-inf, inf) 
  }
  \endverbatim
 
  This node defines a set of 3D coordinates to be used in the coord
  field of vertex-based geometry nodes including SoVRMLIndexedFaceSet,
  SoVRMLIndexedLineSet, and SoVRMLPointSet.

*/

/*!
  \var SoMFVec3f SoVRMLCoordinate::point
  The coordinates. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoGLVBOElement.h>
#include <Inventor/actions/SoAction.h>
#include "../misc/SoVBO.h"
#include "../misc/PointerMap.h"

SO_NODE_SOURCE(SoVRMLCoordinate);

class SoVRMLCoordinateP {
 public:
  SoVRMLCoordinateP() : vbo(NULL) { }
  ~SoVRMLCoordinateP() { delete this->vbo; }
  SoVBO * vbo;
};

#define PRIVATE(obj) ((SoVRMLCoordinateP*) PointerMap::get(obj))

// Doc in parent
void
SoVRMLCoordinate::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLCoordinate, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLCoordinate::SoVRMLCoordinate(void)
{
  PointerMap::add(this, new SoVRMLCoordinateP);
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLCoordinate);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(point);
}

/*!
  Destructor.
*/
SoVRMLCoordinate::~SoVRMLCoordinate()
{
  SoVRMLCoordinateP * pimpl = PRIVATE(this);
  PointerMap::remove(this);
  delete pimpl;  
}

// Doc in parent
void
SoVRMLCoordinate::doAction(SoAction * action)
{
  SoCoordinateElement::set3(action->getState(), this,
                            point.getNum(), point.getValues(0));
}

// Doc in parent
void
SoVRMLCoordinate::GLRender(SoGLRenderAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
  SoVRMLCoordinateP * pimpl = PRIVATE(this);
  SoState * state = action->getState();
  const int num = this->point.getNum();
  SbBool setvbo = FALSE;
  SoBase::staticDataLock();
  if (SoGLVBOElement::shouldCreateVBO(state, num)) {
    SbBool dirty = FALSE;
    setvbo = TRUE;
    if (pimpl->vbo == NULL) {
      pimpl->vbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
      dirty =  TRUE;
    }
    else if (pimpl->vbo->getBufferDataId() != this->getNodeId()) {
      dirty = TRUE;
    }
    if (dirty) {
      pimpl->vbo->setBufferData(this->point.getValues(0),
                                num*sizeof(SbVec3f),
                                        this->getNodeId());
    }
  }
  else if (pimpl->vbo && pimpl->vbo->getBufferDataId()) {
    // clear buffers to deallocate VBO memory
    pimpl->vbo->setBufferData(NULL, 0, 0);
  }
  SoBase::staticDataUnlock();
  if (setvbo) {
    SoGLVBOElement::setVertexVBO(state, pimpl->vbo);
  }
}

// Doc in parent
void
SoVRMLCoordinate::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLCoordinate::callback(SoCallbackAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLCoordinate::pick(SoPickAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}

#undef PRIVATE
#endif // HAVE_VRML97
