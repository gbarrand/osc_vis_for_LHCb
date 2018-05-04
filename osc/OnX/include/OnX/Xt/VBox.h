#ifndef OnX_VBox_h
#define OnX_VBox_h

#include <X11/Intrinsic.h>

#include <string>

#ifdef __cplusplus
extern "C"{
#endif

bool VBoxAttachChildren(Widget,const std::string&,bool,unsigned int);

#ifdef __cplusplus
}
#endif

#endif
