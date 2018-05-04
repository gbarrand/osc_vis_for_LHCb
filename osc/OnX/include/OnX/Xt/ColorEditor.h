#ifndef OnX_ColorEditor_h
#define OnX_ColorEditor_h

#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct _ColorEditorClassRec* ColorEditorWidgetClass;
typedef struct _ColorEditorRec*      ColorEditorWidget;

extern WidgetClass colorEditorWidgetClass;

void ColorEditorGetColor(Widget,double*,double*,double*);

#ifdef __cplusplus
}
#endif

#endif
