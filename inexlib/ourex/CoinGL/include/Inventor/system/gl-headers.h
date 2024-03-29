#ifndef COIN_GLHEADERS_H
#define COIN_GLHEADERS_H

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

/*
 * This header file is supposed to take care of all operating system
 * dependent anomalies connected to including the gl.h header file.
 */

/* This define is at least needed before inclusion of the header files
   that are part of NVidia's Linux drivers v41.91. Without it, none of
   the extension and OpenGL 1.1+ function prototypes will be set up. */
#define GL_GLEXT_PROTOTYPES 1

/*G.Barrand*/
#if defined(WIN32)
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
/* #include <GL/glext.h> - not found on system */
#undef GL_EXT_paletted_texture 
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif


#endif /* ! COIN_GLHEADERS_H */
