#ifndef OnX_ColorEditorP_h
#define OnX_ColorEditorP_h 

#include <OnX/Xt/ColorEditor.h>

#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <X11/CompositeP.h>
#include <X11/ConstrainP.h>
#include <Xm/XmP.h>
#include <Xm/ManagerP.h>

typedef struct {
  int fDummy;
} ColorEditorClassPart;


typedef struct _ColorEditorClassRec {
  CoreClassPart                core_class;
  CompositeClassPart        composite_class;
  ConstraintClassPart        constraint_class;
  XmManagerClassPart        manager_class;
  ColorEditorClassPart        color_editor_class;
} ColorEditorClassRec;

#ifdef __cplusplus
extern "C"{
#endif
extern ColorEditorClassRec colorEditorClassRec;
#ifdef __cplusplus
}
#endif

typedef struct {
  Widget fForm;
  Widget fColorPatch;
  Widget fRedScale;
  Widget fGreenScale;
  Widget fBlueScale;
  double fRed,fGreen,fBlue;
  XtCallbackList okCallback;
  XtCallbackList applyCallback;
  XtCallbackList cancelCallback;
} ColorEditorPart;


typedef struct _ColorEditorRec {
  CorePart                core;
  CompositePart                composite;
  ConstraintPart        constraint;
  XmManagerPart                manager;
  ColorEditorPart        color_editor;
} ColorEditorRec;

#endif
/* DON'T ADD ANYTHING AFTER THIS #endif */
