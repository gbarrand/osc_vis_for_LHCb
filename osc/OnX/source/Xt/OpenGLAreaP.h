#ifndef OpenGLAreaP_h
#define OpenGLAreaP_h 

#include <OnX/Xt/OpenGLArea.h>

#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <X11/CompositeP.h>

typedef struct
{
  void* extension;
} OpenGLAreaClassPart;

typedef struct _OpenGLAreaClassRec
{
  CoreClassPart core_class;
  CompositeClassPart composite_class;
  OpenGLAreaClassPart openGLArea_class;
} OpenGLAreaClassRec;

#ifdef __cplusplus
extern "C"{
#endif
extern OpenGLAreaClassRec openGLAreaClassRec;
#ifdef __cplusplus
}
#endif

typedef struct
{
  Boolean installColormap;
  Boolean doubleBufferOn;
  Visual* visual;
  Cardinal initialDepth;
  Colormap initialColormap;
  XtCallbackList paintCallback;
  XtCallbackList eventCallback;
} OpenGLAreaPart;

typedef struct _OpenGLAreaRec
{
  CorePart core;
  CompositePart composite;
  OpenGLAreaPart openGLArea;
} OpenGLAreaRec;

#endif
