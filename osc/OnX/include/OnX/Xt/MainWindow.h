#ifndef OnX_MainWindow_h
#define OnX_MainWindow_h

#include <X11/Intrinsic.h>

#ifdef __cplusplus
extern "C"{
#endif

Widget CreateMainWindow(Widget,String,ArgList,Cardinal);
Widget CreateMainWindowShell(Display*,String,ArgList,Cardinal);

Boolean IsMainWindow(Widget);

Widget GetMainWindowMenuBar(Widget);
Widget GetMainWindowToolBar(Widget);

void ActivateMainWindowMenuBar(Widget);
void ActivateMainWindowToolBar(Widget);

#ifdef __cplusplus
}
#endif

#endif
