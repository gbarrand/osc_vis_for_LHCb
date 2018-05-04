#ifndef OnX_HBox_h
#define OnX_HBox_h

#include <X11/Intrinsic.h>

#include <string>

#ifdef __cplusplus
extern "C"{
#endif

bool HBoxAttachChildren(Widget,const std::string&,bool,unsigned int);

#ifdef __cplusplus
}
#endif

#endif
