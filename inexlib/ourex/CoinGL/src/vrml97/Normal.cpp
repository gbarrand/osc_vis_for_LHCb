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
  \class SoVRMLNormal SoVRMLNormal.h Inventor/VRMLnodes/SoVRMLNormal.h
  \brief The SoVRMLNormal class is used to bind normals to geometry.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Normal {
    exposedField MFVec3f vector  []   # (-,)
  }
  \endverbatim

  This node defines a set of 3D surface normal vectors to be used in
  the vector field of some geometry nodes (e.g., SoVRMLIndexedFaceSet
  and SoVRMLElevationGrid). This node contains one multiple-valued
  field that contains the normal vectors. Normals shall be of unit
  length.
*/

/*!
  \var SoMFVec3f SoVRMLNormal::vector
  The normal vectors. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoGLVBOElement.h>
#include "../misc/SoVBO.h"
#include "../misc/PointerMap.h"

SO_NODE_SOURCE(SoVRMLNormal);

class SoVRMLNormalP {
 public:
  SoVRMLNormalP() : vbo(NULL) { }
  ~SoVRMLNormalP() { delete this->vbo; }
  
  SoVBO * vbo;
};

#define PRIVATE(obj) ((SoVRMLNormalP*) PointerMap::get(obj))

// Doc in parent
void
SoVRMLNormal::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLNormal, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLNormal::SoVRMLNormal(void)
{
  PointerMap::add(this, new SoVRMLNormalP);
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLNormal);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(vector);
}

/*!
  Destructor.
*/
SoVRMLNormal::~SoVRMLNormal()
{
  SoVRMLNormalP * pimpl = PRIVATE(this);
  PointerMap::remove(this);
  delete pimpl;  
}

// Doc in parent
void
SoVRMLNormal::doAction(SoAction * action)
{
  SoNormalElement::set(action->getState(), this,
                       this->vector.getNum(), this->vector.getValues(0));
}

// Doc in parent
void
SoVRMLNormal::GLRender(SoGLRenderAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
  SoVRMLNormalP * pimpl = PRIVATE(this);
  SoState * state = action->getState();
  
  SoBase::staticDataLock();
  SbBool setvbo = FALSE;
  const int num = this->vector.getNum();
  if (SoGLVBOElement::shouldCreateVBO(state, num)) {
    setvbo = TRUE;
    SbBool dirty = FALSE;
    if (pimpl->vbo == NULL) {
      pimpl->vbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
      dirty =  TRUE;
    }
    else if (pimpl->vbo->getBufferDataId() != this->getNodeId()) {
      dirty = TRUE;
    }
    if (dirty) {
      pimpl->vbo->setBufferData(this->vector.getValues(0),
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
    SoGLVBOElement::setNormalVBO(state, pimpl->vbo);
  }
}

// Doc in parent
void
SoVRMLNormal::callback(SoCallbackAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::pick(SoPickAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

#undef PRIVATE
#endif // HAVE_VRML97
