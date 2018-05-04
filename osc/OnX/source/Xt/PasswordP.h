#ifndef OnX_PasswordP_h
#define OnX_PasswordP_h 

// this :
#include <OnX/Xt/Password.h>

#include <Xm/TextP.h>

#include <string>

extern "C"{

#ifndef XtInheritGeoMatrixCreate  // For HP-UX MOTIF.
#define XtInheritGeoMatrixCreate ((XmGeoCreateProc)_XtInherit)
#endif

typedef struct {
  void* extension;
} PasswordClassPart;

typedef struct _PasswordClassRec {
  CoreClassPart core_class;
  XmPrimitiveClassPart primitive_class;
  XmTextClassPart text_class;
  PasswordClassPart password_class;
} PasswordClassRec;

extern PasswordClassRec passwordClassRec;

typedef struct {
  XtCallbackList passwordEnteredCallback;
  std::string* value; //WARNING : std::string value crashes.
} PasswordPart;

typedef struct _PasswordRec {
  CorePart core;
  XmPrimitivePart primitive;
  XmTextPart text;
  PasswordPart password;
} PasswordRec;

}

#endif

