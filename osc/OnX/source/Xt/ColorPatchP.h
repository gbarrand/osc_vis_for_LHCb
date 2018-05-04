#ifndef OnX_ColorPatchP_h
#define OnX_ColorPatchP_h 

#include <OnX/Xt/ColorPatch.h>

#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>

typedef struct {
  int fDummy;
} ColorPatchClassPart;


typedef struct _ColorPatchClassRec {
  CoreClassPart                core_class;
  ColorPatchClassPart        color_patch_class;
} ColorPatchClassRec;

#ifdef __cplusplus
extern "C"{
#endif
extern ColorPatchClassRec colorPatchClassRec;
#ifdef __cplusplus
}
#endif

#define MAX_COLOR 256

typedef struct {
  GC fGC;
  double fRed,fGreen,fBlue;
  Pixel fPixel;
  int fAllocatedNumber;
  Pixel fAllocatedPixels[MAX_COLOR];
} ColorPatchPart;


typedef struct _ColorPatchRec {
  CorePart                core;
  ColorPatchPart        color_patch;
} ColorPatchRec;

#endif
/* DON'T ADD ANYTHING AFTER THIS #endif */
