// this :
#include <OnX/Win/SoViewers.h>

#include <windows.h>
#include <windowsx.h>

#define WIN_SOVIEWER_SOURCE(aClass,aWinClass)\
WinTk::aClass::aClass(Component& aParent,const std::string& aName)\
:Component(#aClass,aParent)\
,fViewer(0)\
{\
  if(!fParent) return;\
  HWND parent = fParent->nativeWindow();\
  if(!parent) return;\
\
  static char className[] = #aClass;\
  static bool done = false;\
  if(!done) {\
    WNDCLASS         wc;\
    wc.style         = CS_HREDRAW | CS_VREDRAW;\
    wc.lpfnWndProc   = (WNDPROC)WinTk::aClass::proc;\
    wc.cbClsExtra    = 0;\
    wc.cbWndExtra    = 0;\
    wc.hInstance     = GetWindowInstance(parent),\
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);\
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);\
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);\
    wc.lpszMenuName  = className;\
    wc.lpszClassName = className;\
    ::RegisterClass(&wc);\
    done = true;\
  }\
\
  /* The WS_BORDER is needed. Else probleme of size at startup. */\
  RECT rect;\
  ::GetClientRect(parent,&rect);\
  fWindow = ::CreateWindow(className,\
                           NULL,\
                           WS_CHILD | WS_VISIBLE | WS_BORDER,\
                           0,0,\
                           rect.right-rect.left,\
                           rect.bottom-rect.top,\
                           parent,NULL,\
                           GetWindowInstance(parent),\
                           NULL);\
  if(!fWindow) return;\
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));\
\
  fViewer = new ::aWinClass(fWindow,aName.c_str(),TRUE);\
}\
WinTk::aClass::~aClass() {\
  destroy();\
}\
void WinTk::aClass::show() {\
  /*if(!fViewer) return;\
    fViewer->show();*/\
}\
LRESULT CALLBACK WinTk::aClass::proc(\
 HWND aWindow,UINT aMessage,WPARAM aWParam,LPARAM aLParam\
){\
  switch(aMessage) {\
  case WM_DESTROY:wm__destroy(aWindow);return 0;\
  }\
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));\
}\
void WinTk::aClass::setSceneGraph(SoNode* aNode){\
  if(!fViewer) return;\
  fViewer->setSceneGraph(aNode);\
}\
aWinClass* WinTk::aClass::soViewer() const { return fViewer;}


WIN_SOVIEWER_SOURCE(SoExaminerViewer,SoWinExaminerViewer)
WIN_SOVIEWER_SOURCE(SoPlaneViewer,SoWinPlaneViewer)
WIN_SOVIEWER_SOURCE(SoFlyViewer,SoWinFlyViewer)
WIN_SOVIEWER_SOURCE(SoWalkViewer,SoWinWalkViewer)
