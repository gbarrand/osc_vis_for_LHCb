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

/*!
  \class SoAntiSquish SoAntiSquish.h Inventor/nodes/SoAntiSquish.h
  \brief The SoAntiSquish class is used to reset to uniform scaling.
  \ingroup nodes

  When traversed, this node replaces the scale vector of the matrix
  with uniform values, based on one of the SoAntiSquish::Sizing
  strategies.

  This node is for instance used by manipulators to retain the shape
  of their dragger geometries when set up within the influence of an
  SoSurroundScale node (which can yield nonuniform scale-vectors in
  the current state transformation matrix).

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    AntiSquish {
        recalcAlways TRUE
        sizing AVERAGE_DIMENSION
    }
  \endcode

  \sa SoSurroundScale for a usage example.
*/

#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \enum SoAntiSquish::Sizing

  Different strategies for "unsquishing" a scale. Values are used by
  the SoAntiSquish::sizing field.
*/


/*!
  \var SoSFEnum SoAntiSquish::sizing

  The current "unsquish" strategy. Default value is
  SoAntiSquish::AVERAGE_DIMENSION.
*/
/*!
  \var SoSFBool SoAntiSquish::recalcAlways

  Whether to automatically have the unsquishing parameters
  recalculated for every traversal. Default value is \c TRUE.

  You can set this to \c FALSE and manually invoke
  SoAntiSquish::recalc() if you need closer control of the geometry
  influenced by this node.
*/


// *************************************************************************

SO_NODE_SOURCE(SoAntiSquish);

/*!
  Constructor.
*/
SoAntiSquish::SoAntiSquish(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAntiSquish);

  SO_NODE_ADD_FIELD(recalcAlways, (TRUE));
  SO_NODE_ADD_FIELD(sizing, (SoAntiSquish::AVERAGE_DIMENSION));

  SO_NODE_DEFINE_ENUM_VALUE(Sizing, X);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, Y);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, Z);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, AVERAGE_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, BIGGEST_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, SMALLEST_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, LONGEST_DIAGONAL);
  SO_NODE_SET_SF_ENUM_TYPE(sizing, Sizing);

  this->matrixvalid = FALSE;
  this->inversevalid = FALSE;
}

/*!
  Destructor.
*/
SoAntiSquish::~SoAntiSquish()
{
}

// Doc from superclass.
void
SoAntiSquish::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAntiSquish, SO_FROM_INVENTOR_1);
}

// Doc from superclass.
void
SoAntiSquish::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbBool matrixwasvalid = this->matrixvalid;
  SoAntiSquish::doAction(action);
  if (this->recalcAlways.getValue() == FALSE) {
    // Usually when recalcAlways is FALSE, SoAntiSquish::recalc() is
    // called by manipulators at the same time as
    // SoSurroundScale::invalidate() is called. This means that the
    // first time we get here is often because SoSurrondScale applied
    // an SoGetBoundingBoxAction to the scene graph it is going to
    // calculate the surround scale for. This is _not_ the scene graph
    // we want to anti-squish so we should recalculate it the next time
    // we get here.
    if (matrixwasvalid == FALSE) {
      this->matrixvalid = FALSE;
    }
  }
}

/*!
  If SoAntiSquish::recalcAlways has been set to \c FALSE, you must
  call this method whenever the transformations before this node in
  the graph has changed.

  \sa SoAntiSquish::recalcAlways
*/
void
SoAntiSquish::recalc(void)
{
  this->matrixvalid = FALSE;
}

/*!
  Accumulates an "unsquishing" matrix on top of the current model
  matrix.
 */
void
SoAntiSquish::doAction(SoAction * action)
{
  SoState * state = action->getState();
  if (!this->matrixvalid || this->recalcAlways.getValue()) {
    this->matrixvalid = TRUE;
    this->inversevalid = FALSE;
    this->unsquishedmatrix =
      this->getUnsquishingMatrix(SoModelMatrixElement::get(state),
                                 FALSE, this->inversematrix);
  }
  SoModelMatrixElement::mult(action->getState(), this, this->unsquishedmatrix);
}

// Doc from superclass.
void
SoAntiSquish::callback(SoCallbackAction * action)
{
  SoAntiSquish::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoAntiSquish::GLRender(SoGLRenderAction * action)
{
  SoAntiSquish::doAction((SoAction *) action);
}

// Doc from superclass.
void
SoAntiSquish::getMatrix(SoGetMatrixAction * action)
{
  if (!this->matrixvalid || !this->inversevalid ||
      this->recalcAlways.getValue()) {
    this->matrixvalid = TRUE;
    this->inversevalid = TRUE;
    this->unsquishedmatrix = this->getUnsquishingMatrix(action->getMatrix(),
                                                        TRUE,
                                                        this->inversematrix);
  }

  SbMatrix & m = action->getMatrix();
  SbMatrix & i = action->getInverse();  
  m.multLeft(this->unsquishedmatrix);
  i.multRight(this->inversematrix);
}

// Doc from superclass.
void
SoAntiSquish::pick(SoPickAction * action)
{
  SoAntiSquish::doAction((SoAction *) action);
}

/*!
  Calculate and return the matrix needed to "unsquish" the \a
  squishedmatrix.

  If \a calcinverse is \c TRUE, store the inverse of the
  unsquishmatrix in \a getinverse.
*/
SbMatrix
SoAntiSquish::getUnsquishingMatrix(const SbMatrix & squishedmatrix,
                                   const SbBool calcinverse,
                                   SbMatrix & getinverse)
{
  SbRotation r, so;
  SbVec3f t, scale;

  float val = 1.0f;

  squishedmatrix.getTransform(t, r, scale, so);
  switch (this->sizing.getValue()) {
  case X:
    val = scale[0];
    break;
  case Y:
    val = scale[1];
    break;
  case Z:
    val = scale[2];
    break;
  case AVERAGE_DIMENSION:
    val = (scale[0] + scale[1] + scale[2]) / 3.0f;
    break;
  case BIGGEST_DIMENSION:
    val = scale[0];
    if (scale[1] > val) val = scale[1];
    if (scale[2] > val) val = scale[2];
    break;
  case SMALLEST_DIMENSION:
    val = scale[0];
    if (scale[1] < val) val = scale[1];
    if (scale[2] < val) val = scale[2];
    break;
  case LONGEST_DIAGONAL:
    {
      SbVec3f unitcube[8];
      for (int i = 0; i < 8; i++) {
        unitcube[i][0] = i & 1 ? 1.0f : -1.0f;
        unitcube[i][1] = i & 2 ? 1.0f : -1.0f;
        unitcube[i][2] = i & 4 ? 1.0f : -1.0f;
        squishedmatrix.multVecMatrix(unitcube[i], unitcube[i]);
      }

      val = (unitcube[1] - unitcube[6]).sqrLength();
      float tmp = (unitcube[5] - unitcube[2]).sqrLength();
      if (tmp > val) val = tmp;
      tmp = (unitcube[3] - unitcube[4]).sqrLength();
      if (tmp > val) val = tmp;
      tmp = (unitcube[0] - unitcube[7]).sqrLength();
      if (tmp > val) val = tmp;

      val = (float) sqrt(val);
      val *= 0.5f;
      break;
    }
  default:
    assert(0 && "unknown sizing parameter");
    val = (scale[0] + scale[1] + scale[2]) / 3.0f; // use avarage
    break;
  }
  scale[0] = scale[1] = scale[2] = val;
  SbMatrix matrix;
  matrix.setTransform(t, r, scale, so);
  matrix.multRight(squishedmatrix.inverse());
  if (calcinverse) getinverse = matrix.inverse();
  return matrix;
}
