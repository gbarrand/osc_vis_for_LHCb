#ifndef OnX_Password_h
#define OnX_Password_h

#include <X11/Intrinsic.h>

#ifndef XtNpasswordEnteredCallback
#define XtNpasswordEnteredCallback "passwordEnteredCallback"
#endif

#ifndef XtCR_PASSWORD_ENTERED
#define XtCR_PASSWORD_ENTERED 1
#endif

extern "C"{

typedef struct {
  int reason;
  XEvent* event;
  char* value;
} PasswordCallbackStruct;

typedef struct _PasswordClassRec* PasswordWidgetClass;
typedef struct _PasswordRec* PasswordWidget;

extern WidgetClass passwordWidgetClass;
}

#endif
