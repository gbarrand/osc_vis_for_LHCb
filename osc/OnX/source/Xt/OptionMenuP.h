#ifndef OnX_OptionMenuP_h
#define OnX_OptionMenuP_h 

// this :
#include <OnX/Xt/OptionMenu.h>

#include <Xm/FormP.h>

extern "C"{

#ifndef XtInheritGeoMatrixCreate  // For HP-UX MOTIF.
#define XtInheritGeoMatrixCreate ((XmGeoCreateProc)_XtInherit)
#endif

typedef struct {
  void* extension;
} OptionMenuClassPart;

typedef struct _OptionMenuClassRec {
  CoreClassPart core_class;
  CompositeClassPart composite_class;
  ConstraintClassPart constraint_class;
  XmManagerClassPart manager_class;
  XmBulletinBoardClassPart bulletin_board_class;
  XmFormClassPart form_class;
  OptionMenuClassPart option_menu_class;
} OptionMenuClassRec;

extern OptionMenuClassRec optionMenuClassRec;

typedef struct {
  XtCallbackList valueChangedCallback;
  Widget pdm,rowColumn;
} OptionMenuPart;

typedef struct _OptionMenuRec {
  CorePart core;
  CompositePart        composite;
  ConstraintPart constraint;
  XmManagerPart manager;
  XmBulletinBoardPart bulletin_board;
  XmFormPart form;
  OptionMenuPart optionMenu;
} OptionMenuRec;

}

#endif

