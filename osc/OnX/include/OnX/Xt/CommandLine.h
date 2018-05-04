#ifndef OnX_CommandLine_h
#define OnX_CommandLine_h

#include <X11/Intrinsic.h>

#ifndef XtNcompleteCallback
#define XtNcompleteCallback "completeCallback"
#endif

#ifndef XtCR_COMPLETE
#define XtCR_COMPLETE 100
#endif

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
  int reason;
  XEvent* event;
  char* value;
} CommandLineCallbackStruct;

typedef struct _CommandLineClassRec* CommandLineWidgetClass;
typedef struct _CommandLineRec* CommandLineWidget;

extern WidgetClass commandLineWidgetClass;

void CommandLineSetString(Widget,char*);
char* CommandLineGetString(Widget);

#ifdef __cplusplus
}
#endif

#endif
