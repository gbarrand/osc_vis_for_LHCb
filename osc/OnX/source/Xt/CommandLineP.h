#ifndef OnX_CommandLineP_h
#define OnX_CommandLineP_h 

/* this : */
#include <OnX/Xt/CommandLine.h>

#include <Xm/FormP.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef XtInheritGeoMatrixCreate  /* For HP-UX MOTIF. */
#define XtInheritGeoMatrixCreate ((XmGeoCreateProc)_XtInherit)
#endif

typedef struct {
  void* extension;
} CommandLineClassPart;

typedef struct _CommandLineClassRec {
  CoreClassPart core_class;
  CompositeClassPart composite_class;
  ConstraintClassPart constraint_class;
  XmManagerClassPart manager_class;
  XmBulletinBoardClassPart bulletin_board_class;
  XmFormClassPart form_class;
  CommandLineClassPart command_line_class;
} CommandLineClassRec;

extern CommandLineClassRec commandLineClassRec;

typedef struct {
  Widget text;
  XtCallbackList commandEnteredCallback;
  XtCallbackList completeCallback;
  int historySize;
  char** historyList;
  int historyPos;
} CommandLinePart;

typedef struct _CommandLineRec {
  CorePart core;
  CompositePart composite;
  ConstraintPart constraint;
  XmManagerPart manager;
  XmBulletinBoardPart bulletin_board;
  XmFormPart form;
  CommandLinePart command_line;
} CommandLineRec;

#ifdef __cplusplus
}
#endif

#endif

