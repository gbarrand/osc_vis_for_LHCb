#ifndef COIN_SOCGMVECTOROUTPUT_H
#define COIN_SOCGMVECTOROUTPUT_H

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

#include <Inventor/annex/HardCopy/SoVectorOutput.h>
#include <Inventor/SbBasic.h>

class SoCGMVectorOutputP;

class COIN_DLL_API SoCGMVectorOutput : public SoVectorOutput {
public:
  SoCGMVectorOutput();
  virtual ~SoCGMVectorOutput();

  virtual SbBool openFile(const char * filename);
 
  void setBinary(SbBool flag = TRUE);
  SbBool isBinary(void) const;
 
  void setVersion(int vers = 1);
  int getVersion(void) const;
 
  void setIndexed(SbBool flag = TRUE);
  SbBool isIndexed(void) const;

private:
  SoCGMVectorOutputP * pimpl;
};

#endif // !COIN_SOCGMVECTOROUTPUT_H
