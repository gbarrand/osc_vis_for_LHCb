#ifndef OnX_ColorPatch_h
#define OnX_ColorPatch_h

#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _ColorPatchClassRec* ColorPatchWidgetClass;
typedef struct _ColorPatchRec*      ColorPatchWidget;

extern WidgetClass colorPatchWidgetClass;

void ColorPatchSetColor(Widget,double,double,double);

#ifdef __cplusplus
}
#endif

#endif
