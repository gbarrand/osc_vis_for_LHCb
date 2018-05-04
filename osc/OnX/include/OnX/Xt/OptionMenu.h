#ifndef OnX_OptionMenu_h
#define OnX_OptionMenu_h

#ifdef __alpha
#include <stdio.h>
#endif
#include <X11/Intrinsic.h>

#include <vector>
#include <string>

extern "C"{

typedef struct {
  int reason;
  XEvent* event;
  char* value;
} OptionMenuCallbackStruct;

typedef struct _OptionMenuClassRec* OptionMenuWidgetClass;
typedef struct _OptionMenuRec* OptionMenuWidget;

extern WidgetClass optionMenuWidgetClass;
}

void OptionMenuInitialize(Widget,
                          const std::vector<std::string>&,
                          const std::string&);
void OptionMenuSetDefault(Widget,const std::string&);
bool OptionMenuGetOption(Widget,std::string&);
bool OptionMenuSetOption(Widget,const std::string&);
bool OptionMenuGetItems(Widget,std::vector<std::string>&);
Widget OptionMenuGetLabel(Widget);

#endif
