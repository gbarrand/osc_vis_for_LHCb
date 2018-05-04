// this :
#include <OnX/Win/WinTk.h>

#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

#include <OnX/Win/WinTools.h>

#include <inlib/smanip>
#include <inlib/sprintf>

static int DROUND(double);

#define NotFound (-1)

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

// WM_PARENTNOTIFY is sent within the CreateWindow of the child.
// When received by the parent, the parent Post (not Send) to itself
// a WM_TK_CHILDCREATED in order to be notify of the child creation
// out of any CreateWindow.
#define WM_TK_CHILDCREATED ((WM_USER)+888)
#define WM_TK_CHILDDELETED ((WM_USER)+889)
#define TK_KEY 127 // To be sure that it is a message of our own.

#define KEY_UP 0x8000

//////////////////////////////////////////////////////////////////////////////
static bool string_to_VK(
 const std::string& aString
,WORD& a_VK
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //VK_xxx are defined in WinUser.h
  if(aString.size()==1) { a_VK = aString[0];return true;} //WM_W is 'W'

#define S_VK(aWhat) \
  if(!::strcmp(aString.c_str(),#aWhat)) { a_VK = VK_##aWhat;return true;}

  S_VK(BACK)
  S_VK(TAB)
  S_VK(CLEAR)
  S_VK(RETURN)
  S_VK(ESCAPE)
  S_VK(SPACE)

  S_VK(UP)
  S_VK(DOWN)
  S_VK(LEFT)
  S_VK(RIGHT)

  S_VK(F1)
  S_VK(F2)
  S_VK(F3)
  S_VK(F4)
  S_VK(F5)
  S_VK(F6)
  S_VK(F7)
  S_VK(F8)
  S_VK(F9)
  S_VK(F10)
  S_VK(F11)
  S_VK(F12)
  S_VK(F13)
  S_VK(F14)
  S_VK(F15)
  S_VK(F16)
  S_VK(F17)
  S_VK(F18)
  S_VK(F19)
  S_VK(F20)
  S_VK(F21)
  S_VK(F22)
  S_VK(F23)
  S_VK(F24)

  //FIXME : handle other keys ? NUMPAD[0-9]

  return false;
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::steer(
 GetFocusedShellProc aProc
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  while (true) {
    MSG event;
    BOOL status = ::GetMessage(&event, NULL, 0, 0);
    if(status == -1) { // This may happen (dixit Microsoft doc).
      return 0;
    } else if(status == 0) { //WM_QUIT
      return (int)event.wParam;
    } else {

      if(!aProc) {
        ::TranslateMessage(&event);
        ::DispatchMessage (&event);
      } else {
        Shell* shell = aProc(aTag);
        if(!shell) {
          ::TranslateMessage(&event);
          ::DispatchMessage (&event);
        } else {
          HWND hwnd = shell->nativeWindow();
          HACCEL haccel = shell->nativeAcceleratorTable();
          if(!hwnd || !haccel) {
            ::TranslateMessage(&event);
            ::DispatchMessage (&event);
          } else {
            if(TranslateAccelerator(hwnd,haccel,&event)) {
              //Done.
            } else {
              ::TranslateMessage(&event);
              ::DispatchMessage(&event);
            }
          }
        }
      }
    }
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Component::Component(
 const std::string& aType                              
)
:fType(aType)
,fWindow(0)
,fParent(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Component::Component(
 const std::string& aType                              
,Component& aParent
)
:fType(aType)
,fWindow(0)
,fParent(&aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Component::~Component(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::destroy(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(hasCallbacks("delete")) {
    CallbackData data;
    executeCallbacks("delete",data);
  }
  if(fWindow) {
    ::SetWindowLong(fWindow,GWL_USERDATA,LONG(NULL));
    ::DestroyWindow(fWindow);
    fWindow = 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::wm__destroy(
 HWND aWindow
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  Component* This = (Component*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(This) { //How to be sure that we have a Component* ???
    if(This->fWindow!=aWindow) {
      ::printf("WinTk::Component::wm_destroy : HWND mismatch !\n");
    }
    if(This->hasCallbacks("WM_DESTROY")) {
      CallbackData data;
      This->executeCallbacks("WM_DESTROY",data);
    }
    This->fWindow = 0;
  }
  ::SetWindowLong(aWindow,GWL_USERDATA,LONG(NULL));
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::Component::type(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fType;
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::Component::nativeWindow(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fWindow;
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Component* WinTk::Component::parent(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fParent;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //if(!fWindow) return;
  //::ShowWindow(fWindow,SW_SHOWDEFAULT);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::hide(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::ShowWindow(fWindow,SW_HIDE);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Component::size(
 unsigned int& aWidth
,unsigned int& aHeight
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  aWidth = 0;
  aHeight = 0;
  if(!fWindow) return false;
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  aWidth = rect.right-rect.left;
  aHeight = rect.bottom-rect.top;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Component::position(
 int& aX
,int& aY
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  aX = 0;
  aY = 0;
  if(!fWindow) return false;
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  aX = rect.left;
  aY = rect.top;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Component* WinTk::Component::findFather(
 const std::string& aType
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(aType==type()) return this;
  Component* parent = fParent;
  while(parent) {
    if(aType==parent->type()) return parent;
    parent = parent->fParent;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::addCallback(
 const std::string& aName
,Callback aFunction
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  NamedCallbacks* cbks = 0;
  std::vector<NamedCallbacks>::iterator it;
  for(it=fCallbacks.begin();it!=fCallbacks.end();++it) {
    if(aName==(*it).first) {
      cbks = &(*it);
      break;
    }
  }
  if(!cbks) {
    fCallbacks.push_back(std::pair<std::string,Callbacks>(aName,Callbacks()));
    cbks = &(fCallbacks.back());
  }
  cbks->second.push_back(std::pair<Callback,void*>(aFunction,aTag));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::removeCallback(
 const std::string& aName
,Callback aFunction
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  NamedCallbacks* cbks = 0;
  std::vector<NamedCallbacks>::iterator it;
  for(it=fCallbacks.begin();it!=fCallbacks.end();++it) {
    if(aName==(*it).first) {
      cbks = &(*it);
      break;
    }
  }
  if(cbks) {
    Callbacks::iterator it;
    for(it=cbks->second.begin();it!=cbks->second.end();++it) {
      if( (aFunction==(*it).first) && (aTag==(*it).second) ) {
        cbks->second.erase(it);
        return;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
bool WinTk::Component::executeCallbacks(
 const std::string& aName
,CallbackData& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<NamedCallbacks>::const_iterator it;
  for(it=fCallbacks.begin();it!=fCallbacks.end();++it) {
    if(aName==(*it).first) {
      const Callbacks& cbks = (*it).second;
      Callbacks::const_iterator it2;
      for(it2=cbks.begin();it2!=cbks.end();++it2) {
        Callback cbk = (*it2).first;
        if(cbk) {
          HWND back_fWindow = fWindow;
          cbk(*this,aData,(*it2).second);
          // The callback may have destroyed the Component and
          // the native HWND. The below checks that.
          // In a proc, it is better to not use the HWND
          // (or the Component) after calling executeCallbacks.
          if(back_fWindow && !::GetWindowLong(back_fWindow,GWL_USERDATA)) {
            //::printf("WinTk::Component::executeCallbacks : WARNING : A callback destroyed the native HWND window !\n");
            return false;
          }
        }
      }      
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Component::hasCallbacks(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<NamedCallbacks>::const_iterator it;
  for(it=fCallbacks.begin();it!=fCallbacks.end();++it) {
    if(aName==(*it).first) return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::Component::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::setName(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fName = aName;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::rubberDrawLine(
 POINT& aBegin
,POINT& aEnd
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  HPEN pen = ::CreatePen(PS_SOLID,5,RGB(0,0,0));
  HDC hdc = ::GetWindowDC(fWindow);
  HPEN oldPen = SelectPen(hdc,pen);
  int oldROP = ::SetROP2(hdc,R2_NOT);
  POINT pt;
  ::MoveToEx(hdc,aBegin.x,aBegin.y,&pt);
  ::LineTo(hdc,aEnd.x,aEnd.y);
  ::SetROP2(hdc,oldROP);
  SelectPen(hdc,oldPen);
  ::ReleaseDC(fWindow,hdc);
  ::DeletePen(pen);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Component::rubberDrawRect(
 POINT& aBegin
,POINT& aEnd
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  HPEN pen = ::CreatePen(PS_SOLID,0,RGB(0,0,0));
  HDC hdc = ::GetWindowDC(fWindow);
  HPEN oldPen = SelectPen(hdc,pen);
  int oldROP = ::SetROP2(hdc,R2_NOT);
  POINT pt;
  ::MoveToEx(hdc,aBegin.x,aBegin.y,&pt);
  ::LineTo(hdc,aBegin.x,aEnd.y);
  ::LineTo(hdc,aEnd.x,aEnd.y);
  ::LineTo(hdc,aEnd.x,aBegin.y);
  ::LineTo(hdc,aBegin.x,aBegin.y);
  ::SetROP2(hdc,oldROP);
  SelectPen(hdc,oldPen);
  ::ReleaseDC(fWindow,hdc);
  ::DeletePen(pen);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Component::setBackground(double aR,double aG,double aB){ 
  if(!fWindow) return false;

  int r = int(aR*255.0);
  if((r<0)||(r>255)) return false;
  int g = int(aG*255.0);
  if((g<0)||(g>255)) return false;
  int b = int(aB*255.0);
  if((b<0)||(b>255)) return false;

/*
  //COLORREF cr = RGB(r,g,b);
  HBRUSH brush = ::CreateSolidBrush(RGB(r,g,b));
  ::printf("debug : WinTk::Component::setBackground : brush  %lu\n",brush);

  HBRUSH prev = (HBRUSH)::GetClassLongPtr(fWindow,GCLP_HBRBACKGROUND);
  ::printf("debug : WinTk::Component::setBackground : prev brush  %lu\n",prev);

 {WNDCLASSEX wc;
  ::GetClassInfoEx(::GetModuleHandle(NULL),WC_STATIC,&wc);
  ::printf("debug : prev brush xxx %lu\n",wc.hbrBackground);}

  ULONG_PTR stat = 
    ::SetClassLongPtr(fWindow,GCLP_HBRBACKGROUND,(LONG_PTR)brush);
  if(prev && !stat) {
    ::printf("debug : WinTk::Component::setBackground : SetClassLongPtr failed.\n");
    return false;
  }

 {ULONG_PTR stat = 
  ::SetClassLongPtr(fWindow,GCLP_HBRBACKGROUND,(LONG_PTR)brush);
  ::printf("debug : WinTk::Component::setBackground : SetClassLongPtr failed.yyy %lu.\n",stat);}

 {WNDCLASSEX wc;
  ::GetClassInfoEx(::GetModuleHandle(NULL),WC_STATIC,&wc);
  ::printf("debug : prev brush yyy %lu\n",wc.hbrBackground);}

  if(!::InvalidateRect(fWindow,NULL,TRUE)) {
    ::printf("debug : WinTk::Component::setBackground : InvalidateRect failed.\n");
    return false;
  }

  //::DeleteObject(brush);
  return true;
*/
  return false;
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Component::containerProc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//  Some child send notification message to their parent (instead of sending
// the message to themselves !). This procedure is used by containers 
// to forward these messages to the child.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_PARENTNOTIFY:{
    if(aWParam==WM_CREATE) {
      ::PostMessage(aWindow,WM_TK_CHILDCREATED,TK_KEY,0);
      return 0;
    } else if(aWParam==WM_DESTROY) {
      ::PostMessage(aWindow,WM_TK_CHILDDELETED,TK_KEY,0);
      return 0;
    }
  }break;
  case WM_TK_CHILDCREATED:{
    if(aWParam==TK_KEY) {
      RECT rect;
      ::GetClientRect(aWindow,&rect);
      LPARAM size = MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top);
      ::SendMessage(aWindow,WM_SIZE,(WPARAM)0,size);
      return 0;
    }
  }break;
  case WM_TK_CHILDDELETED:{
    if(aWParam==TK_KEY) {
    }
  }break;
  case WM_NOTIFY:{ //Coming from a child (combobox, scrollbar, toolbar).
    NMHDR* nmhdr = (NMHDR*)aLParam;
    HWND from_win = nmhdr->hwndFrom;
    if(from_win!=aWindow) {
      ::SendMessage(from_win,aMessage,aWParam,aLParam);
    }
  }return 0;
  case WM_COMMAND:
  case WM_VSCROLL: 
  case WM_HSCROLL:{ 
    ::SendMessage((HWND)aLParam,aMessage,aWParam,(LPARAM)0);
  }return 0;
  }
  return TRUE; //Not handled.
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::MessageWindow::MessageWindow(
)
:Component("MessageWindow")
/////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  static char sMessageWindowClassName[] = "WinTk::MessageWindow";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::MessageWindow::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = ::GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sMessageWindowClassName;
    wc.lpszClassName = sMessageWindowClassName;
    ::RegisterClass(&wc);
    done = true;
  }

  fWindow = ::CreateWindow(sMessageWindowClassName,"messageWindow",
                           WS_OVERLAPPEDWINDOW,
                           0,0,100,100,
                           NULL,NULL,
                           ::GetModuleHandle(NULL),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::MessageWindow::~MessageWindow(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::MessageWindow::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch (aMessage) { 
  case WM_USER:{
    MessageWindow* This = 
      (MessageWindow*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      data.wparam = aWParam;
      data.lparam = aLParam;
      This->executeCallbacks("send",data);
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::MessageWindow::sendMessage(
 void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  ::PostMessage(fWindow,WM_USER,(WPARAM)0,(LPARAM)aTag);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Shell::Shell(
 unsigned int aMask
)
:Component("Shell")
,fFocusWindow(0)
,fSetFocusCallback(0)
,fSetFocusTag(0)
,fAcceleratorTable(0)
/////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  static char sWindowClassName[] = "WinTk::Shell";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::Shell::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = ::GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sWindowClassName;
    wc.lpszClassName = sWindowClassName;
    ::RegisterClass(&wc);
    done = true;
  }

  fWindow = ::CreateWindow(sWindowClassName,
                           NULL,
                           aMask, //WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT,CW_USEDEFAULT, 
                           400,400,
                           NULL,NULL,
                           ::GetModuleHandle(NULL),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Shell::~Shell(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(fAcceleratorTable) {
    ::DestroyAcceleratorTable(fAcceleratorTable);
    fAcceleratorTable = 0;
  }
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Shell::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch (aMessage) { 
  // Same logic as containerProc :
  case WM_NOTIFY:{ //Coming from a child (combobox, scrollbar, toolbar).
    NMHDR* nmhdr = (NMHDR*)aLParam;
    HWND from_win = nmhdr->hwndFrom;
    if(from_win!=aWindow) {
      ::SendMessage(from_win,aMessage,aWParam,aLParam);
    }
  }return 0;
  // Else :
  case WM_SIZE:{ // Assume one child window ! FIXME : have a message if not.
    int width = LOWORD(aLParam);
    int height = HIWORD(aLParam);
    HWND hwnd = GetFirstChild(aWindow);
    if(hwnd) {
      //FIXME : have to treat the case of TOOLBAR not being the first.
      if(GetClassName(hwnd)==std::string(TOOLBARCLASSNAME)) {
        HWND next = GetNextSibling(hwnd);
        // Share area between toolbar and second child :
        RECT rect;
        ::GetWindowRect(hwnd,&rect);
        int htb = rect.bottom-rect.top;
        SetGeometry(next,0,htb,width,height-htb);
      } else {
        SetGeometry(hwnd,0,0,width,height);
      }
    }
  }return 0;
  case WM_SETFOCUS:{
    Shell* This = (Shell*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      This->callSetFocusCallback();
      HWND win = This->focusWindow();
      if(win) ::SetFocus(win);
    }
  }return 0;
  case WM_INITMENUPOPUP:{ //0x0117
    //aWParam is the HMENU (from CreatePopupMenu)
    //aLParam is the MenuItemCount
    Shell* This = (Shell*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      data.wparam = aWParam;
      This->executeCallbacks("cascading",data);
    }
    } return 0;
  case WM_COMMAND:{ //Coming from a menu item, a toolbar !
    if(aLParam) {
      // From a toolbar ; send it back to it :   
      ::SendMessage((HWND)aLParam,aMessage,aWParam,0);
    } else {
      //if(HIWORD(aWParam)==1) { //From an accelerator.
      //}
      // From a menu item :
      Shell* This = (Shell*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        data.wparam = LOWORD(aWParam);
        This->executeCallbacks("activate",data);
      }
    }
  }return 0;
  case WM_CLOSE:{
    Shell* This = (Shell*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      if(This->hasCallbacks("close")) {
        CallbackData data;
        This->executeCallbacks("close",data);
      }
    }
  }break; //NOTE : can't be return 0.
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SetForegroundWindow(fWindow);
  ::ShowWindow(fWindow,SW_SHOWDEFAULT);
  ::UpdateWindow(fWindow);
  ::DrawMenuBar(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::setTitle(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SetWindowText(fWindow,aString.c_str());
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::setGeometry(
 int aX
,int aY
,unsigned int aWidth
,unsigned int aHeight
)
//////////////////////////////////////////////////////////////////////////////
// Given width, height is the desired client area.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int hborder = ::GetSystemMetrics(SM_CYCAPTION);
  if(::GetWindowLong(fWindow,GWL_ID)) // Has a menubar.
    hborder += ::GetSystemMetrics(SM_CYMENU);
  ::MoveWindow(fWindow,aX,aY,aWidth,hborder + aHeight,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::setSize(
 unsigned int aWidth
,unsigned int aHeight
)
//////////////////////////////////////////////////////////////////////////////
// Given width, height is the desired client area.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int hborder = ::GetSystemMetrics(SM_CYCAPTION);
  RECT rect;
  ::GetWindowRect(fWindow,&rect);
  ::MoveWindow(fWindow,rect.left,rect.top,aWidth,hborder + aHeight,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::setFocusWindow(
 HWND aWindow
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fFocusWindow = aWindow;
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::Shell::focusWindow(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fFocusWindow;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::setSetFocusCallback(
 SetFocusCallback aCallback
,void* aTag
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fSetFocusCallback = aCallback;
  fSetFocusTag = aTag;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Shell::callSetFocusCallback(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fSetFocusCallback) return;
  fSetFocusCallback(this,fSetFocusTag);
}
//////////////////////////////////////////////////////////////////////////////
HACCEL WinTk::Shell::nativeAcceleratorTable(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fAcceleratorTable;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Shell::addAccelerator(
 const std::string& aString
,int aID
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //printf("debug : accel : %s %d\n",aString.c_str(),aID);
  // aString should be of the form : "a" or "Ctrl+a".
  ACCEL acc;
  //acc.fVirt : FALT, FCONTROL, FNOINVERT, FSHIFT, FVIRTKEY
 {acc.cmd = aID;
  std::vector<std::string> words;
  std::string s(aString);
  inlib::touppercase(s);
  inlib::words(s,"+",false,words);
  if(words.size()==1) {
    acc.fVirt = FVIRTKEY;
    if(!string_to_VK(words[0],acc.key)) return false;
  } else if((words.size()==2)&&(words[1].size())) {
    if(words[0]=="CTRL") {
      acc.fVirt = FCONTROL;
    } else if(words[0]=="ALT") {
      acc.fVirt = FALT;
    } else if(words[0]=="SHIFT") {
      acc.fVirt = FSHIFT;
    } else {
      return false;
    }
    acc.fVirt |= FVIRTKEY;
    if(!string_to_VK(words[1],acc.key)) return false;
  } else {
    return false;
  }}

  if(!fAcceleratorTable) {
    fAcceleratorTable = ::CreateAcceleratorTable(&acc,1);
  } else {
    int n = ::CopyAcceleratorTable(fAcceleratorTable,NULL,0);
    ACCEL* tbl = new ACCEL[n+1];
    ::CopyAcceleratorTable(fAcceleratorTable,tbl,n);
    ACCEL& tacc = tbl[n];
    tacc.fVirt = acc.fVirt;
    tacc.key = acc.key;
    tacc.cmd = acc.cmd;
    ::DestroyAcceleratorTable(fAcceleratorTable);
    fAcceleratorTable = ::CreateAcceleratorTable(tbl,n+1);
    delete [] tbl;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Dialog::Dialog(
 Component& aParent
,unsigned int aMask
)
:Component("Dialog",aParent)
/////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sDialogClassName[] = "WinTk::Dialog";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::Dialog::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = ::GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sDialogClassName;
    wc.lpszClassName = sDialogClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  //  int oldMask = WS_POPUP | WS_CAPTION | WS_THICKFRAME;
  aMask = WS_POPUP |aMask ; 
  fWindow = ::CreateWindow(sDialogClassName,
                           NULL,
                           aMask,  //WS_POPUP | WS_CAPTION | WS_THICKFRAME,
                           CW_USEDEFAULT,CW_USEDEFAULT, 
                           400,400,
                           parent,
                           NULL,
                           ::GetModuleHandle(NULL),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
#ifdef DEBUG
  printf("debug : Dialog : %lu\n",fWindow);
#endif
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Dialog::~Dialog(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Dialog::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch (aMessage) { 
  case WM_SIZE:{ // Assume one child window !
    int width = LOWORD(aLParam);
    int height = HIWORD(aLParam);
    HWND hwnd = GetFirstChild(aWindow);
    if(hwnd) SetGeometry(hwnd,0,0,width,height);
  }return 0;
  case WM_SETFOCUS:{
    Dialog* This = (Dialog*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      HWND dialogWindow = This->nativeWindow();
      HWND next = GetTabStop(dialogWindow,aWindow);
      if(next) ::SetFocus(next);
    }
  }return 0;
  case WM_SHOWWINDOW:{
    switch(aWParam){
    case SW_SHOWNORMAL:{
      // Set at center of parent :
      RECT rectParent;
      ::GetWindowRect(GetParent(aWindow),&rectParent);
      RECT rect;
      ::GetWindowRect(aWindow,&rect);
      int xCenter = (rectParent.right + rectParent.left)/2; 
      int yCenter = (rectParent.top + rectParent.bottom)/2; 
      int x = xCenter - (rect.right - rect.left)/2;
      int y = yCenter - (rect.bottom - rect.top)/2;
      if(x<0) x = 0;
      if(y<0) y = 0;
      ::MoveWindow(aWindow,x,y,
                   rect.right-rect.left,
                   rect.bottom-rect.top,
                   TRUE);

      // Set focus :
      Dialog* This = (Dialog*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        HWND dialogWindow = This->nativeWindow();
        HWND next = GetTabStop(dialogWindow,aWindow);
        if(next) ::SetFocus(next);
      }
      }return 0;
    }
  }break;
  case WM_CLOSE:{
    Dialog* This = (Dialog*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      if(This->hasCallbacks("close")) {
        CallbackData data;
        This->executeCallbacks("close",data);
      }
    }
  }break; //NOTE : can't be return 0.
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Dialog::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SetForegroundWindow(fWindow);
  ::ShowWindow(fWindow,SW_SHOWDEFAULT);
  ::UpdateWindow(fWindow);
  ::DrawMenuBar(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Dialog::setTitle(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SetWindowText(fWindow,aString.c_str());
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Dialog::setSize(
 unsigned int aWidth
,unsigned int aHeight
)
//////////////////////////////////////////////////////////////////////////////
// Given width, height is the desired client area.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int hborder = ::GetSystemMetrics(SM_CYCAPTION);
  RECT rect;
  ::GetWindowRect(fWindow,&rect);
  ::MoveWindow(fWindow,rect.left,rect.top,aWidth,hborder + aHeight,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::WarningDialog::WarningDialog(
 Component& aParent
)
:Component("WarningDialog",aParent)
/////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
WinTk::WarningDialog::~WarningDialog(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::WarningDialog::setTitle(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fTitle = aString;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::WarningDialog::setMessage(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fMessage = aString;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::WarningDialog::show(
) 
//////////////////////////////////////////////////////////////////////////////
// It is modal.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent || !fParent->nativeWindow()) return;
  ::MessageBox(fParent->nativeWindow(),fMessage.c_str(),fTitle.c_str(),MB_OK);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Label::Label(
 Component& aParent
)
:Component("Label",aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(WC_STATIC,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           //| WS_BORDER,
                           //| SS_WHITERECT,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Label::~Label(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Label::setLabel(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SetWindowText(fWindow,aString.c_str());
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::Label::label(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fLabel = "";
  if(!fWindow) return fLabel;
  fLabel = WinTk::GetText(fWindow);
  return fLabel;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Button::Button(
 Component& aParent
,bool aBitmap
)
:Component("Button",aParent)
,fOldProc(0)
,fBitmap(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();
  
  fWindow = ::CreateWindow(WC_BUTTON,
                           NULL,
                           WS_CHILD | WS_VISIBLE 
                           //| WS_BORDER
                           | WS_TABSTOP
                           | BS_PUSHBUTTON
                           | (aBitmap?BS_BITMAP:BS_TEXT),
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
#ifdef DEBUG
  HDC hdc = ::GetDC(fWindow);
  TEXTMETRIC tm;
  ::GetTextMetrics(hdc,&tm);
  short charWidth = (short)tm.tmAveCharWidth;
  short charHeight = (short)(tm.tmHeight + tm.tmExternalLeading);
  ::ReleaseDC(fWindow,hdc);
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  printf("debug : Button : width %d height %d char width %d height %d\n",
         rect.right-rect.left,rect.bottom-rect.top,charWidth,charHeight);
#endif
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Button::~Button(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
  if(fBitmap) ::DeleteObject(fBitmap);
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Button::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_COMMAND:{
    Button* This = (Button*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      This->executeCallbacks("activate",data);
    }
  }return 0;
  case WM_KEYDOWN:
    switch(aWParam){
    case VK_RETURN:{
      Button* This = (Button*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("activate",data);
      }
    }return 0;
    case VK_TAB:{  // Avoid beeping.
      Button* This = (Button*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }
    }return 0;
    }
    break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  Button* This = (Button*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Button::setLabel(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //FIXME : should handle a min width, height.
  if(!fWindow) return;
  ::SetWindowText(fWindow,aString.c_str());
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Button::setPixmap(
 const std::string& aFile
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  HDC hdc = ::GetDC(fWindow);
  if(!hdc) return;
  int w,h;
  fBitmap = ReadXpm(hdc,aFile,w,h);
  ::ReleaseDC(fWindow,hdc);
  if(!fBitmap) return;
  ::SendMessage(fWindow,BM_SETIMAGE,(WPARAM)IMAGE_BITMAP,(LPARAM)fBitmap);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ToggleButton::ToggleButton(
 Component& aParent
)
:Component("ToggleButton",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();
  
  fWindow = ::CreateWindow(WC_BUTTON,
                           NULL,
                           WS_CHILD | WS_VISIBLE 
                           | WS_TABSTOP
                           | BS_AUTOCHECKBOX,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
#ifdef DEBUG
  HDC hdc = ::GetDC(fWindow);
  TEXTMETRIC tm;
  ::GetTextMetrics(hdc,&tm);
  short charWidth = (short)tm.tmAveCharWidth;
  short charHeight = (short)(tm.tmHeight + tm.tmExternalLeading);
  ::ReleaseDC(fWindow,hdc);
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  printf("debug : Button : width %d height %d char width %d height %d\n",
         rect.right-rect.left,rect.bottom-rect.top,charWidth,charHeight);
#endif
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ToggleButton::~ToggleButton(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::ToggleButton::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_COMMAND:{ //Coming from the window owning the button :
    ToggleButton* This = (ToggleButton*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      This->executeCallbacks("activate",data);
    }
  }return 0;
  case WM_KEYDOWN:
    switch(aWParam){
      /*
    case VK_RETURN:{
      ToggleButton* This = 
        (ToggleButton*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("activate",data);
      }
    }return 0;
      */
    case VK_TAB:{  // Avoid beeping.
      ToggleButton* This = 
        (ToggleButton*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }
    }return 0;
    }
    break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  ToggleButton* This = (ToggleButton*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ToggleButton::setLabel(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //FIXME : should handle a min width, height.
  if(!fWindow) return;
  ::SetWindowText(fWindow,aString.c_str());
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::ToggleButton::isChecked(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return false;
  return (Button_GetCheck(fWindow)==BST_CHECKED ? true : false);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ToggleButton::setChecked(
 bool aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  Button_SetCheck(fWindow,aValue ? BST_CHECKED : BST_UNCHECKED);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::OptionMenu::OptionMenu(Component& aParent)
:Component("OptionMenu",aParent)
,fOldProc(0)
,fEditOldProc(0)
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(WC_COMBOBOX,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER 
                           | WS_TABSTOP | WS_VSCROLL
                           | CBS_DROPDOWNLIST,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
  // A CBS_SIMPLE has two children of class ComboLBox and Edit.
  // A CBS_DROPDOWN has one child of class Edit.
  // A CBS_DROPDOWNLIST has no edit.
  HWND edit = GetChildByClassName(fWindow,"Edit");
  if(edit) {
    fEditOldProc = (WNDPROC)::GetWindowLong(edit,GWL_WNDPROC);
    ::SetWindowLong(edit,GWL_WNDPROC,(LONG)editProc);
  }
}

WinTk::OptionMenu::~OptionMenu(){destroy();}

LRESULT CALLBACK WinTk::OptionMenu::proc(HWND aWindow,UINT aMessage,WPARAM aWParam,LPARAM aLParam){
  switch(aMessage) { 
  case WM_COMMAND:{ //Coming from the parent :
    int code = HIWORD(aWParam);
    if(code==CBN_SELENDOK) {
      OptionMenu* This = (OptionMenu*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("valueChanged",data);
      }
      return 0;
    }
  }break;
  case WM_CHAR:
    switch(aWParam){
    case VK_RETURN:{
      OptionMenu* This = (OptionMenu*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("valueChanged",data);
      }
    }return 0;
    case VK_TAB:{  // Avoid beeping.
      OptionMenu* This = (OptionMenu*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }
    }return 0;
    }
  break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  OptionMenu* This = (OptionMenu*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}

LRESULT CALLBACK WinTk::OptionMenu::editProc(HWND aWindow,UINT aMessage,WPARAM aWParam,LPARAM aLParam){
  switch(aMessage) { 
  case WM_CHAR:
    switch(aWParam){
    case VK_RETURN:{
      OptionMenu* This = 
        (OptionMenu*)::GetWindowLong(GetParent(aWindow),GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("valueChanged",data);
      }
    }
    return 0;
    case VK_TAB:{  // Avoid beeping.
      OptionMenu* This = 
        (OptionMenu*)::GetWindowLong(GetParent(aWindow),GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }
    }return 0;
    }
    break;
  }
  OptionMenu* This = (OptionMenu*)::GetWindowLong(GetParent(aWindow),GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fEditOldProc,aWindow,aMessage,aWParam,aLParam);
}

bool WinTk::OptionMenu::initialize(const std::vector<std::string>& aItems,const std::string& aDefault){
  if(!fWindow) return false;
  ComboBox_ResetContent(fWindow);
  int history = NotFound;
  std::vector<std::string>::const_iterator it;
  unsigned int index = 0;
  for(it=aItems.begin();it!=aItems.end();++it,index++) {
    ComboBox_AddString(fWindow,(*it).c_str());
    if(history==NotFound) history = index;//first item.
    if(aDefault.size()&&((*it)==aDefault)) history = index;
  }
  if(history!=NotFound) ComboBox_SetCurSel(fWindow,history);
  return true;
}

bool WinTk::OptionMenu::items(std::vector<std::string>& aItems){
  aItems.clear();
  if(!fWindow) return false;
  int number = ComboBox_GetCount(fWindow);
  for(int index=0;index<number;index++) {
    int len = ComboBox_GetLBTextLen(fWindow,index);
    char* buffer = new char[len+1];
    ComboBox_GetLBText(fWindow,index,buffer);
    aItems.push_back(buffer);
    delete [] buffer;
  }
  return true;
}

bool WinTk::OptionMenu::setDefault(const std::string& aDefault){
  if(!fWindow) return false;
  int index = ComboBox_FindString(fWindow,0,aDefault.c_str());
  if(index==CB_ERR) return false;
  ComboBox_SetCurSel(fWindow,index);
  return true;
}

std::string WinTk::OptionMenu::value() const{
  if(!fWindow) return "";
  return WinTk::GetText(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ComboBox::ComboBox(Component& aParent)
:Component("ComboBox",aParent)
,fOldProc(0)
,fEditOldProc(0)
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(WC_COMBOBOX,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER 
                           | WS_TABSTOP | WS_VSCROLL
                           | CBS_DROPDOWN,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
  // A CBS_SIMPLE has two children of class ComboLBox and Edit.
  // A CBS_DROPDOWN has one child of class Edit.
  // A CBS_DROPDOWNLIST has no edit.
  HWND edit = GetChildByClassName(fWindow,"Edit");
  if(edit) {
    fEditOldProc = (WNDPROC)::GetWindowLong(edit,GWL_WNDPROC);
    ::SetWindowLong(edit,GWL_WNDPROC,(LONG)editProc);
  }
}

WinTk::ComboBox::~ComboBox(){destroy();}

LRESULT CALLBACK WinTk::ComboBox::proc(HWND aWindow,UINT aMessage,WPARAM aWParam,LPARAM aLParam){
  switch(aMessage) { 
  case WM_COMMAND:{ //Coming from the parent :
    int code = HIWORD(aWParam);
    if(code==CBN_SELENDOK) {
      ComboBox* This = (ComboBox*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("valueChanged",data);
      }
      return 0;
    }
  }break;
  case WM_CHAR:
    switch(aWParam){
    case VK_RETURN:{
      ComboBox* This = (ComboBox*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("valueChanged",data);
      }
    }return 0;
    case VK_TAB:{  // Avoid beeping.
      ComboBox* This = (ComboBox*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }
    }return 0;
    }
  break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  ComboBox* This = (ComboBox*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}

LRESULT CALLBACK WinTk::ComboBox::editProc(HWND aWindow,UINT aMessage,WPARAM aWParam,LPARAM aLParam){
  switch(aMessage) { 
  case WM_CHAR:
    switch(aWParam){
    case VK_RETURN:{
      ComboBox* This = 
        (ComboBox*)::GetWindowLong(GetParent(aWindow),GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("valueChanged",data);
      }
    }
    return 0;
    case VK_TAB:{  // Avoid beeping.
      ComboBox* This = 
        (ComboBox*)::GetWindowLong(GetParent(aWindow),GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }
    }return 0;
    }
    break;
  }
  ComboBox* This = (ComboBox*)::GetWindowLong(GetParent(aWindow),GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fEditOldProc,aWindow,aMessage,aWParam,aLParam);
}

bool WinTk::ComboBox::setItems(const std::vector<std::string>& aItems){
  if(!fWindow) return false;
  ComboBox_ResetContent(fWindow);
  std::vector<std::string>::const_iterator it;
  for(it=aItems.begin();it!=aItems.end();++it) {
    ComboBox_AddString(fWindow,(*it).c_str());
  }
  return true;
}
bool WinTk::ComboBox::items(std::vector<std::string>& aItems){
  aItems.clear();
  if(!fWindow) return false;
  int number = ComboBox_GetCount(fWindow);
  for(int index=0;index<number;index++) {
    int len = ComboBox_GetLBTextLen(fWindow,index);
    char* buffer = new char[len+1];
    ComboBox_GetLBText(fWindow,index,buffer);
    aItems.push_back(buffer);
    delete [] buffer;
  }
  return true;
}

bool WinTk::ComboBox::setValue(const std::string& aValue){
  if(!fWindow) return false;
  int index = ComboBox_FindString(fWindow,0,aValue.c_str());
  if(index==CB_ERR) {
    index = ComboBox_AddString(fWindow,aValue.c_str());
    if(index==CB_ERR) return false;
  }
  ComboBox_SetCurSel(fWindow,index);
  return true;
}

std::string WinTk::ComboBox::value() const{
  if(!fWindow) return "";
  return WinTk::GetText(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Tree::Tree(
 Component& aParent
)
:Component("Tree",aParent)
,fCallbackEnable(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  // Ensure that the common control DLL is loaded : 
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_TREEVIEW_CLASSES;
  ::InitCommonControlsEx(&icex);

  fWindow = ::CreateWindow(WC_TREEVIEW,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER | 
                           TVS_HASBUTTONS | TVS_HASLINES |
                           TVS_LINESATROOT | TVS_SHOWSELALWAYS,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Tree::~Tree(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Tree::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_NOTIFY:{
    NMHDR* nmhdr = (NMHDR*)aLParam;
    if(nmhdr->code==TVN_SELCHANGED) {
      //printf("debug : Tree::proc : TVN_SELCHANGED\n");
      HTREEITEM item = ((NMTREEVIEW*)aLParam)->itemNew.hItem;
      int action = ((NMTREEVIEW*)aLParam)->action;
      //TreeView_SetInsertMark(aWindow,item,TRUE);
      Tree* This = (Tree*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This && This->fCallbackEnable && (action!=TVC_UNKNOWN)) {
        CallbackData data;
        data.value = TreeGetItemPath(aWindow,item);
        if(TreeIsItemBranch(aWindow,item)) {
          This->executeCallbacks("selectBranch",data);
        } else {
          This->executeCallbacks("select",data);
        }
      }
      return 0;
    } else if(nmhdr->code==TVN_ITEMEXPANDED) {
      //printf("debug : Tree::proc : TVN_ITEMEXPANDED\n");
      HTREEITEM item = ((NMTREEVIEW*)aLParam)->itemNew.hItem;
      Tree* This = (Tree*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This && This->fCallbackEnable) {
        CallbackData data;
        data.value = TreeGetItemPath(aWindow,item);
        if(TreeIsItemExpanded(aWindow,item)) {
          This->executeCallbacks("open",data);
        } else {
          This->executeCallbacks("close",data);
        }
      }
      return 0;
    }
  }break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  Tree* This = (Tree*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Tree::clear(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  TreeView_DeleteAllItems(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Tree::enableCallbacks(
 bool aValue
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  bool old = fCallbackEnable;
  fCallbackEnable = aValue;
  return old;
}
//////////////////////////////////////////////////////////////////////////////
HTREEITEM WinTk::Tree::insertItem(
 HTREEITEM aItem
,const std::string& aLabel
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  TV_INSERTSTRUCT str;
  str.hParent = (HTREEITEM)(aItem?aItem:TVI_ROOT);
  str.item.mask = TVIF_TEXT;
  str.hInsertAfter = TVI_SORT;
  str.item.pszText = (LPSTR)aLabel.c_str();
  return TreeView_InsertItem(fWindow,&str);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Tree::selection(
 std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  HTREEITEM treeItem = TreeView_GetSelection(fWindow);
  if(!treeItem) return false;
  HTREEITEM item = treeItem;
  std::string path;
  while(item && (item!=TVI_ROOT)) {
    std::string ss = TreeGetItemLabel(fWindow,item);
    std::string opath = path;
    path = "\n";
    path += ss;
    path += opath;
    item = TreeView_GetParent(fWindow,item);
  }
  // Remove the leading \n
  if(path.size()) path = path.substr(1,path.size()-1);
  aValue = path;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Tree::setSelection(
 const std::vector<std::string>& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  fCallbackEnable = false;
  TreeView_SelectItem(fWindow,0);
  unsigned int itemn = aPath.size();
  bool found = true;
  HTREEITEM item = TreeView_GetRoot(fWindow);
  for(unsigned int index=0;index<itemn;index++){
    if(!item) {found = false;break;}
    bool found2 = false;
    while(item) {
      std::string ss = TreeGetItemLabel(fWindow,item);
      if(aPath[index]==ss) {
        if(index!=itemn-1) TreeView_Expand(fWindow,item,TVE_EXPAND);
        else {
          TreeView_SelectItem(fWindow,item);
        }
        found2 = true;
        item = TreeView_GetChild(fWindow,item);
        break;
      }
      item = TreeView_GetNextSibling(fWindow,item);
    }
    if(!found2) {found = false;break;}
  }
  if(!found) TreeView_SelectItem(fWindow,0);
  fCallbackEnable = true;
  return found;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Tree::selectFirst(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  fCallbackEnable = false;
  //TreeView_SelectItem(fWindow,0);
  HTREEITEM item = TreeView_GetRoot(fWindow);
  /*
  while(item) {
    item = TreeView_GetNextSibling(fWindow,item);
  }
  */
  if(item) TreeView_SelectItem(fWindow,item);
  fCallbackEnable = true;
}
//////////////////////////////////////////////////////////////////////////////
HTREEITEM WinTk::Tree::firstItem(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return TreeView_GetRoot(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
std::vector<HTREEITEM> WinTk::Tree::itemChildren(
 HTREEITEM aItem
)
//////////////////////////////////////////////////////////////////////////////
// Not tested yet.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<HTREEITEM> children;
  if(!fWindow) return children;
  HTREEITEM item = TreeView_GetChild(fWindow,aItem);
  while(true) {
    if(!item) return children;    
    children.push_back(item);
    item = TreeView_GetNextSibling(fWindow,item);
  }
  return children;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Frame::Frame(
 Component& aParent
)
:Component("Frame",aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sFrameClassName[] = "WinTk::Frame";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::Frame::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sFrameClassName;
    wc.lpszClassName = sFrameClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sFrameClassName,
                           NULL,
                           // The WS_BORDER is needed.
                           WS_CHILD | WS_VISIBLE | WS_BORDER,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Frame::~Frame(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Frame::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ToolBar::ToolBar(
 Component& aParent
)
:Component("ToolBar",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  // Parent should be a Shell window.
  HWND parent = fParent->nativeWindow();

  // Ensure that the common control DLL is loaded : 
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_BAR_CLASSES;
  ::InitCommonControlsEx(&icex);

  fWindow = ::CreateWindowEx(0,
                             TOOLBARCLASSNAME,
                             NULL,
                             WS_CHILD | WS_VISIBLE
                             //| CCS_ADJUSTABLE 
                             //| TBSTYLE_FLAT
                             //| | TBSTYLE_EX_MIXEDBUTTONS
			     | TBSTYLE_TOOLTIPS | TBSTYLE_LIST ,
                             0,0,100,100,
                             parent,0,
                             GetWindowInstance(parent),
                             NULL);
  if(!fWindow) return;
  // For backward compatibility (see MSDN).
  ::SendMessage(fWindow,TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0);

  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  // Associate some standard images to the ToolBar :
  //::SendMessage(fWindow,TB_LOADIMAGES,
  //IDB_STD_LARGE_COLOR,(LPARAM)HINST_COMMCTRL);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ToolBar::~ToolBar(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
  for(unsigned index=0;index<fBitmaps.size();index++) {
    ::DeleteObject(fBitmaps[index]);
  }
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::ToolBar::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_COMMAND:{ //Coming from the window owning the toolbar :
    ToolBar* This = (ToolBar*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      int index = aWParam;
      CallbackData data;
      This->fItems[index]->executeCallbacks("activate",data);
    }
  }return 0;
  case WM_NOTIFY:{
    NMHDR* nmhdr = (NMHDR*)aLParam;
    if(nmhdr->code==TTN_NEEDTEXT) {
      ToolBar* This = (ToolBar*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        TOOLTIPTEXT* text = (TOOLTIPTEXT*)aLParam;
        int index = text->hdr.idFrom;
        text->lpszText = (LPTSTR)(This->fItems[index]->toolTip().c_str());
        text->hinst = NULL;
      }
      return 0;
    }
  }break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  ToolBar* This = (ToolBar*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ToolBar::addItem(
 ToolBarItem* aItem
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fItems.push_back(aItem);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ToolBar::createItems(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  if(!fItems.size()) return;

  // First pass to create HBITMAPs :
  int mxbw = 0;
  int mxbh = 0;
  bool hasBitmap = false;
  HDC hdc = ::GetDC(fWindow);
  if(hdc) {
    for(unsigned int index=0;index<fItems.size();index++) {
      ToolBarItem* tbItem = fItems[index];
      //bitmap = (HBITMAP)::LoadImage(GetWindowInstance(fParent->window()),
      //(LPSTR)aPixmap.c_str(),
      //IMAGE_ICON,24,24,LR_LOADFROMFILE);
      int bw,bh;
      HBITMAP bitmap = ReadXpm(hdc,tbItem->pixmap(),bw,bh);
      if(bitmap) {
        hasBitmap = true;
        mxbw = MAXIMUM(mxbw,bw);
        mxbh = MAXIMUM(mxbh,bh);
        fBitmaps.push_back(bitmap); //FIXME : need that ?
        tbItem->setBitmap(bitmap);
      } else {
        tbItem->setBitmap(0);
      }
    }
    ::ReleaseDC(fWindow,hdc);
  }

  //hasBitmap = false;
  if(hasBitmap) {
    LPARAM s = MAKELPARAM(mxbw,mxbh);
    ::SendMessage(fWindow,TB_SETBUTTONSIZE,(WPARAM)0,s);
    // The below should be done before any TB_ADDBITMAP.
    ::SendMessage(fWindow,TB_SETBITMAPSIZE,(WPARAM)0,s);
  }

  // Create all the TBBUTTONs :
  TBBUTTON* tbButtons = new TBBUTTON[fItems.size()]; 
  for(unsigned int index=0;index<fItems.size();index++) {
    ToolBarItem* tbItem = fItems[index];
    TBBUTTON& tbButton = tbButtons[index];

    int iString = -1;
    if(tbItem->label().size()) {
      iString = ::SendMessage(fWindow,TB_ADDSTRING,
                              0,(LPARAM)tbItem->label().c_str());
    }
    int iBitmap = I_IMAGENONE;
    HBITMAP bitmap = tbItem->bitmap();
    if(bitmap) {
      TBADDBITMAP tbAddBitmap;
      tbAddBitmap.hInst = NULL;
      tbAddBitmap.nID = (UINT_PTR)bitmap;
      iBitmap = ::SendMessage(fWindow,TB_ADDBITMAP,
                              (WPARAM)1,(LPARAM)&tbAddBitmap);
    }

    tbButton.idCommand = index;
    tbButton.fsState = TBSTATE_ENABLED;
    tbButton.dwData = 0;
    tbButton.iBitmap = iBitmap;
    tbButton.iString = iString;
    tbButton.fsStyle = TBSTYLE_BUTTON;
    //if(iString!=(-1)) {
      tbButton.fsStyle = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
      //} else {
      //tbButton.fsStyle = TBSTYLE_BUTTON;
      //}
  }


  ::SendMessage(fWindow,TB_ADDBUTTONS,(WPARAM)fItems.size(),(LPARAM)tbButtons);

  ::SendMessage(fWindow,TB_AUTOSIZE,0,0);

  delete [] tbButtons;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ToolBarItem::ToolBarItem(
 Component& aParent
,const std::string& aLabel
,const std::string& aPixmap
,const std::string& aToolTip
)
:Component("ToolBarItem",aParent)
,fLabel(aLabel)
,fPixmap(aPixmap)
,fToolTip(aToolTip)
,fBitmap(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent) return;
  if(fParent->type()!="ToolBar") return;
  WinTk::ToolBar* toolBar = (WinTk::ToolBar*)fParent;
  toolBar->addItem(this);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ToolBarItem::~ToolBarItem(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::ToolBarItem::label(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fLabel;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::ToolBarItem::pixmap(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fPixmap;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::ToolBarItem::toolTip(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fToolTip;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ToolBarItem::setBitmap(
 HBITMAP aBitmap
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fBitmap = aBitmap;
}
//////////////////////////////////////////////////////////////////////////////
HBITMAP  WinTk::ToolBarItem::bitmap(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fBitmap;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::TabStack::TabStack(
 Component& aParent
)
:Component("TabStack",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  // Ensure that the common control DLL is loaded : 
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_TAB_CLASSES;
  ::InitCommonControlsEx(&icex);

  fWindow = ::CreateWindowEx(0,
                             WC_TABCONTROL,
                             NULL,
                             WS_CHILD | WS_VISIBLE,
                             0,0,100,100,
                             parent,0,
                             GetWindowInstance(parent),
                             NULL);
  if(!fWindow) return;

  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::TabStack::~TabStack(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::TabStack::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // A TabStack is a container. We need similar
  // treatement as in WinTk::Component::containerProc.
  switch(aMessage) { 
  //similar to containerProc :
  case WM_PARENTNOTIFY:{
    if(aWParam==WM_CREATE) {
      ::PostMessage(aWindow,WM_TK_CHILDCREATED,TK_KEY,0);
      return 0;
    } else if(aWParam==WM_DESTROY) {
      ::PostMessage(aWindow,WM_TK_CHILDDELETED,TK_KEY,0);
      return 0;
    }
  }break;
  case WM_TK_CHILDCREATED:{
    if(aWParam==TK_KEY) {
      HWND child = (HWND)aLParam;
      RECT rect;
      ::GetClientRect(aWindow,&rect);
      TabCtrl_AdjustRect(aWindow,FALSE,&rect);
      LPARAM size = MAKELPARAM(rect.right-rect.left,rect.bottom-rect.top);
      ::SendMessage(aWindow,WM_SIZE,(WPARAM)0,size);
      return 0;
    }
  }break;
  case WM_TK_CHILDDELETED:{
    if(aWParam==TK_KEY) {
      //HWND child = (HWND)aLParam;
      int number = TabCtrl_GetItemCount(aWindow);
      int index = TabCtrl_GetCurSel(aWindow);
      if((index!=NotFound)) {
        TabStack* This = (TabStack*)::GetWindowLong(aWindow,GWL_USERDATA);
        if(This) {
          This->setCurrent(index);
          CallbackData data;
          This->executeCallbacks("select",data);
        }
      }
      return 0;
    }
  }break;
  case WM_COMMAND:
  case WM_VSCROLL: 
  case WM_HSCROLL:{ 
    ::SendMessage((HWND)aLParam,aMessage,aWParam,(LPARAM)0);
  }return 0;
  // Else :
  case WM_SIZE:{
    RECT rect;
    ::GetClientRect(aWindow,&rect);
    TabCtrl_AdjustRect(aWindow,FALSE,&rect);
    // Move children :
    HWND child = GetFirstChild(aWindow);
    while(child) {
      SetGeometry(child,
                  rect.left,rect.top,
                  rect.right-rect.left,rect.bottom-rect.top);
      child = GetNextSibling(child);
    }
  }break; //FIXME : return 0 does not work with TestTabStack.onx !
  case WM_NOTIFY:{
    NMHDR* nmhdr = (NMHDR*)aLParam;
    HWND from_win = nmhdr->hwndFrom;
    if(nmhdr->code==TCN_SELCHANGE) {
      int index = TabCtrl_GetCurSel(aWindow);
      if(index!=NotFound) {
        TabStack* This = (TabStack*)::GetWindowLong(aWindow,GWL_USERDATA);
        if(This) {
          This->setCurrent(index);
          CallbackData data;
          This->executeCallbacks("select",data);
        }
      }
      return 0;
    } else if(nmhdr->code==TCN_SELCHANGING) {
      return 0;
    } else if(nmhdr->code==NM_CLICK) {
      return 0;
    } else if(from_win!=aWindow) {
      // Forward to child :
      ::SendMessage(from_win,aMessage,aWParam,aLParam);
      return 0;
    }
  }break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  TabStack* This = (TabStack*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::TabStack::addItem(
 Component& aChild
,const std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  if(!aChild.nativeWindow()) return false;

  //NOTE : the aChild.nativeWindow() had been put already
  //       in the parent HWND at creation of the child.
  //NOTE : it seems that we can have more child window
  //       than the ones appearing/declared in the tab.

  TCITEM item;
  item.mask = TCIF_TEXT | TCIF_PARAM;
  item.pszText = (LPTSTR)aLabel.c_str();
  item.lParam = (LPARAM)aChild.nativeWindow();
  int index = TabCtrl_GetItemCount(fWindow);
  int status = TabCtrl_InsertItem(fWindow,index,&item);
  // Set first item by default.
  TabCtrl_SetCurSel(fWindow,0);
  setCurrent(0);
  return (status==NotFound?false:true);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::TabStack::removeItem(
 Component& aChild
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  if(!aChild.nativeWindow()) return false;
  int sel = TabCtrl_GetCurSel(fWindow);
  int number = TabCtrl_GetItemCount(fWindow);
  for(int index=0;index<number;index++) {
    TCITEM item;
    item.mask = TCIF_PARAM;
    if(!TabCtrl_GetItem(fWindow,index,&item)) return false;
    HWND child = (HWND)item.lParam;
    if(child==aChild.nativeWindow()) {
      TabCtrl_DeleteItem(fWindow,index);
      if(sel==index) {
        // Set first item by default.
        TabCtrl_SetCurSel(fWindow,0);
        setCurrent(0);
      }

      return true;
    }
  }
  return false;
}

bool WinTk::TabStack::selection(std::string& aValue) const {
  if(!fWindow) return false;
  int index = TabCtrl_GetCurSel(fWindow);
  if(index==NotFound) return false;
  char buffer[256];
  TCITEM item;
  item.mask = TCIF_TEXT;
  item.cchTextMax = 256;
  item.pszText = buffer;
  if(!TabCtrl_GetItem(fWindow,index,&item)) return false;
  aValue = std::string(buffer);
  return true;
}

bool WinTk::TabStack::setSelection(const std::string& a_label) {
  if(!fWindow) return false;
  int number = TabCtrl_GetItemCount(fWindow);
  for(int index=0;index<number;index++) {
    char buffer[256];
    TCITEM item;
    item.mask = TCIF_TEXT;
    item.cchTextMax = 256;
    item.pszText = buffer;
    if(!TabCtrl_GetItem(fWindow,index,&item)) return false;
    if(std::string(buffer)==a_label) {
      TabCtrl_SetCurSel(fWindow,index);
      setCurrent(index);      
      return true;
    }
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
int WinTk::TabStack::itemCount(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return 0;
  return TabCtrl_GetItemCount(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::TabStack::setCurrent(
 int aIndex
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  TCITEM item;
  item.mask = TCIF_PARAM;
  if(!TabCtrl_GetItem(fWindow,aIndex,&item)) return;
  HWND selChild = (HWND)item.lParam;
  HWND child = GetFirstChild(fWindow);
  while(child) {
    if(child==selChild) {
      ::ShowWindow(child,SW_SHOWDEFAULT);
    } else {
      ::ShowWindow(child,SW_HIDE);
    }
    child = GetNextSibling(child);
  }
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::TabStack::setLabel(
 Component& aChild
,const std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  if(!aChild.nativeWindow()) return false;
  int number = TabCtrl_GetItemCount(fWindow);
  for(int index=0;index<number;index++) {
    TCITEM item;
    item.mask = TCIF_PARAM;
    if(!TabCtrl_GetItem(fWindow,index,&item)) return false;
    HWND child = (HWND)item.lParam;
    if(child==aChild.nativeWindow()) {
      TCITEM item;
      item.mask = TCIF_TEXT;
      item.pszText = (LPTSTR)aLabel.c_str();
      if(!TabCtrl_SetItem(fWindow,index,&item)) return false;
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::TabStack::label(
 Component& aChild
,std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  if(!aChild.nativeWindow()) return false;
  int number = TabCtrl_GetItemCount(fWindow);
  for(int index=0;index<number;index++) {
    TCITEM item;
    item.mask = TCIF_PARAM;
    if(!TabCtrl_GetItem(fWindow,index,&item)) return false;
    HWND child = (HWND)item.lParam;
    if(child==aChild.nativeWindow()) {
      char buffer[256];
      TCITEM item;
      item.mask = TCIF_TEXT;
      item.cchTextMax = 256;
      item.pszText = buffer;
      if(!TabCtrl_GetItem(fWindow,index,&item)) return false;
      aLabel = std::string(buffer);
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ProgressBar::ProgressBar(
 Component& aParent
)
:Component("ProgressBar",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  // Ensure that the common control DLL is loaded : 
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_PROGRESS_CLASS;
  ::InitCommonControlsEx(&icex);

  fWindow = ::CreateWindowEx(0,
                             PROGRESS_CLASS,
                             NULL,
                             WS_CHILD | WS_VISIBLE
                             | PBS_SMOOTH,
                             0,0,100,100,
                             parent,0,
                             GetWindowInstance(parent),
                             NULL);
  if(!fWindow) return;

  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ProgressBar::~ProgressBar(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::ProgressBar::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  ProgressBar* This = (ProgressBar*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ProgressBar::setValue(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,PBM_SETPOS,(WPARAM)aValue,0);
}

int WinTk::ProgressBar::value() const { 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,PBM_GETPOS,0,0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::VBox::VBox(
 Component& aParent
)
:Component("VBox",aParent)
,fSizeOfFixed(-1)
,fSplitHalfSize(1)
,fExpand(SECOND)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sVBoxClassName[] = "WinTk::VBox";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::VBox::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sVBoxClassName;
    wc.lpszClassName = sVBoxClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sVBoxClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::VBox::~VBox(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::VBox::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!containerProc(aWindow,aMessage,aWParam,aLParam)) return 0;
  switch(aMessage) { 
  case WM_SIZE:{
    VBox* This = (VBox*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      int width = LOWORD(aLParam);
      int height = HIWORD(aLParam);
      HWND first = GetFirstChild(aWindow);
      HWND second = first ? GetNextSibling(first) : 0;
      if(first && second) {
        if(This->fExpand==FIRST) {
          // Second child height must not change.
          // If set, fSizeOfFixed is the size of the second child.
          int hsecond = 0;
          if(This->fSizeOfFixed>=0) {
            hsecond = This->fSizeOfFixed;
          } else {
            RECT rect;
            ::GetWindowRect(second,&rect);
            hsecond = rect.bottom - rect.top;
          }
          int hfirst = height - hsecond - 2 * This->fSplitHalfSize;
          if( (hfirst<=0) || (hsecond<=0) ) { //ANOMALY
            //Resize anyway in order to keep children visibles.
            hsecond = height/2;
            hfirst = height - hsecond - 2 * This->fSplitHalfSize;
          }
          SetGeometry(first,0,0,width,hfirst);
          SetGeometry(second,0,height - hsecond,width,hsecond);
        } else if(This->fExpand==SECOND) { //Default
          // First child height must not change.
          // If set, fSizeOfFixed is the size of the first child.
          int hfirst = 0;
          if(This->fSizeOfFixed>=0) {
            hfirst = This->fSizeOfFixed;
          } else {
            RECT rect;
            ::GetWindowRect(first,&rect);
            hfirst = rect.bottom - rect.top;
          }
          int hsecond = height - hfirst - 2 * This->fSplitHalfSize;
          if( (hfirst<=0) || (hsecond<=0) ) { //ANOMALY
            //Resize anyway in order to keep children visibles.
            hfirst = height/2;
            hsecond = height - hfirst - 2 * This->fSplitHalfSize;
          }
          SetGeometry(first,0,0,width,hfirst);
          SetGeometry(second,0,hfirst,width,hsecond);
        } else { //BOTH
          SetGeometry(first,0,0,
                      width,height/2 - This->fSplitHalfSize);
          SetGeometry(second,0,height/2 + This->fSplitHalfSize,
                      width,height/2 - This->fSplitHalfSize);
        }
      } else if(first) {
        SetGeometry(first,0,0,width,height);
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VBox::setSizeOfFixed(
 int aSize
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fSizeOfFixed = aSize;
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VBox::sizeOfFixed(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fSizeOfFixed;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VBox::setExpand(
 Expand aValue
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fExpand = aValue;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::HBox::HBox(
 Component& aParent
)
:Component("HBox",aParent)
,fSizeOfFixed(-1)
,fSplitHalfSize(1)
,fExpand(SECOND)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sHBoxClassName[] = "WinTk::HBox";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::HBox::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sHBoxClassName;
    wc.lpszClassName = sHBoxClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sHBoxClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
#ifdef DEBUG
  printf("debug : HBox : %lu\n",fWindow);
#endif
}
//////////////////////////////////////////////////////////////////////////////
WinTk::HBox::~HBox(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::HBox::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!containerProc(aWindow,aMessage,aWParam,aLParam)) return 0;
  switch(aMessage) { 
  case WM_SIZE:{
    HBox* This = (HBox*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      int width = LOWORD(aLParam);
      int height = HIWORD(aLParam);
      HWND first = GetFirstChild(aWindow);
      HWND second = first ? GetNextSibling(first) : 0;
      if(first && second) {
        if(This->fExpand==FIRST) {
          // Second child width must not change.
          // If set, fSizeOfFixed is the size of the second child.
          int wsecond = 0;
          if(This->fSizeOfFixed>=0) {
            wsecond = This->fSizeOfFixed;
          } else {
            RECT rect;
            ::GetWindowRect(second,&rect);
            wsecond = rect.right - rect.left;
          }
          int wfirst = width - wsecond - 2 * This->fSplitHalfSize;
          if( (wfirst<=0) || (wsecond<=0) ) { //ANOMALY
            //Resize anyway in order to keep children visibles.
            wsecond = width/2;
            wfirst = width - wsecond - 2 * This->fSplitHalfSize;
          }
          SetGeometry(first,0,0,wfirst,height);
          SetGeometry(second,width - wsecond,0,wsecond,height);
        } else if(This->fExpand==SECOND) { //Default
          // First child width must not change.
          // If set, fSizeOfFixed is the size of the first child.
          int wfirst = 0;
          if(This->fSizeOfFixed>=0) {
            wfirst = This->fSizeOfFixed;
          } else {
            RECT rect;
            ::GetWindowRect(first,&rect);
            wfirst = rect.right - rect.left;
          }
          int wsecond = width - wfirst - 2 * This->fSplitHalfSize;
          if( (wfirst<=0) || (wsecond<=0) ) { //ANOMALY
            //Resize anyway in order to keep children visibles.
            wfirst = width/2;
            wsecond = width - wfirst - 2 * This->fSplitHalfSize;
          }
          SetGeometry(first,0,0,wfirst,height);
          SetGeometry(second,wfirst,0,wsecond,height);
        } else { //BOTH
          SetGeometry(first,0,0,
                      width/2 - This->fSplitHalfSize,height);
          SetGeometry(second,width/2 + This->fSplitHalfSize,0,
                      width/2 - This->fSplitHalfSize,height);
        }
      } else if(first) {
        SetGeometry(first,0,0,width,height);
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HBox::setSizeOfFixed(
 int aSize
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fSizeOfFixed = aSize;
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HBox::sizeOfFixed(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fSizeOfFixed;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HBox::setExpand(
 Expand aValue
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fExpand = aValue;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::VPaned::VPaned(
 Component& aParent
)
:Component("VPaned",aParent)
,fSplitPosition(-1)
,fSplitHalfSize(2)
,fRubberMove(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBeginMove.x = 0;
  fBeginMove.y = 0;
  fEndMove.x = 0;
  fEndMove.y = 0;

  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sVPanedClassName[] = "WinTk::VPaned";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::VPaned::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    //wc.hCursor       = LoadCursor(NULL,MAKEINTRESOURCE(IDC_VSPLITBAR));
    wc.hCursor       = LoadCursor(NULL,IDC_SIZENS);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sVPanedClassName;
    wc.lpszClassName = sVPanedClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sVPanedClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::VPaned::~VPaned(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::VPaned::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!containerProc(aWindow,aMessage,aWParam,aLParam)) return 0;
  switch(aMessage) { 
  case WM_SIZE:{
    VPaned* This = (VPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      This->resizeChildren(LOWORD(aLParam),HIWORD(aLParam));
    }
  }return 0;
  case WM_LBUTTONDOWN:{
    ::SetCapture(aWindow);
    RECT rect;
    ::GetClientRect(aWindow,&rect);
    POINT lt;
    lt.x = rect.left;
    lt.y = rect.top;
    POINT rb;
    rb.x = rect.right;
    rb.y = rect.bottom;
    ::ClientToScreen(aWindow,&lt);
    ::ClientToScreen(aWindow,&rb);
    rect.left = lt.x;
    rect.right = rb.x;
    rect.top = lt.y;
    rect.bottom = rb.y;
    ::ClipCursor(&rect);
    VPaned* This = (VPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      if(This->fRubberMove) {
        This->fBeginMove.x = rect.left;
        This->fEndMove.x = rect.right;
        This->fEndMove.y = This->fBeginMove.y = HIWORD(aLParam);
        This->rubberDrawLine(This->fEndMove,This->fBeginMove);
      }
    }
  }return 0;
  case WM_LBUTTONUP:{
    if(::GetCapture()==aWindow) {
      ::ReleaseCapture();
      ::ClipCursor(NULL);
      VPaned* This = (VPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        if(This->fRubberMove) {
          This->rubberDrawLine(This->fEndMove,This->fBeginMove);
        }
        int y = HIWORD(aLParam);
        This->setSplitPositionPixel(y);
        RECT rect;
        ::GetClientRect(aWindow,&rect);
        This->resizeChildren(rect.right-rect.left,rect.bottom-rect.top);
      }
      return 0;
    }
  } break;
  case WM_MOUSEMOVE:{
    unsigned int state = aWParam;
    if((state & MK_LBUTTON)==MK_LBUTTON) {
      VPaned* This = (VPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        if(This->fRubberMove) {
          // Erase previous line :
          This->rubberDrawLine(This->fEndMove,This->fBeginMove);
          // New line :
          RECT rect;
          ::GetClientRect(aWindow,&rect);
          This->fBeginMove.x = rect.left;
          This->fEndMove.x = rect.right;
          This->fEndMove.y = This->fBeginMove.y = HIWORD(aLParam);
          This->rubberDrawLine(This->fEndMove,This->fBeginMove);
        } else {
          int y = HIWORD(aLParam);
          This->setSplitPositionPixel(y);
          RECT rect;
          ::GetClientRect(aWindow,&rect);
          This->resizeChildren(rect.right-rect.left,rect.bottom-rect.top);
        }
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VPaned::setSplitPosition(
 double aSplitPosition
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fSplitPosition = aSplitPosition;
}
//////////////////////////////////////////////////////////////////////////////
double WinTk::VPaned::splitPosition(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fSplitPosition;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VPaned::setSplitPositionPixel(
 int aSplitPosition
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  int height = rect.bottom-rect.top;
  if(height<=0) return;
  fSplitPosition = ((double)aSplitPosition)/((double)height);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VPaned::splitPositionPixel(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  int height = rect.bottom-rect.top;
  return (fSplitPosition>=0 ? DROUND(fSplitPosition * height) : height/2);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VPaned::resizeChildren(
 int aWidth
,int aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  HWND first = GetFirstChild(fWindow);
  HWND second = first ? GetNextSibling(first) : 0;
  if(first && second) {
    int hfirst = splitPositionPixel();
    SetGeometry(first,
                0,0,
                aWidth,hfirst - fSplitHalfSize);
    SetGeometry(second,
                0,hfirst + fSplitHalfSize,
                aWidth,aHeight - hfirst - fSplitHalfSize);
  } else if(first) {
    SetGeometry(first,0,0,aWidth,aHeight);
  }        
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::HPaned::HPaned(
 Component& aParent
)
:Component("HPaned",aParent)
,fSplitPosition(-1)
,fSplitHalfSize(2)
,fRubberMove(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBeginMove.x = 0;
  fBeginMove.y = 0;
  fEndMove.x = 0;
  fEndMove.y = 0;

  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sHPanedClassName[] = "WinTk::HPaned";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::HPaned::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    //wc.hCursor       = LoadCursor(NULL,MAKEINTRESOURCE(IDC_HSPLITBAR));
    wc.hCursor       = LoadCursor(NULL,IDC_SIZEWE);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sHPanedClassName;
    wc.lpszClassName = sHPanedClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sHPanedClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::HPaned::~HPaned(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::HPaned::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!containerProc(aWindow,aMessage,aWParam,aLParam)) return 0;
  switch(aMessage) { 
  case WM_SIZE:{
   HPaned* This = (HPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      This->resizeChildren(LOWORD(aLParam),HIWORD(aLParam));
    }
  }return 0;
  case WM_LBUTTONDOWN:{
    ::SetCapture(aWindow);
    RECT rect;
    ::GetClientRect(aWindow,&rect);
    POINT lt;
    lt.x = rect.left;
    lt.y = rect.top;
    POINT rb;
    rb.x = rect.right;
    rb.y = rect.bottom;
    ::ClientToScreen(aWindow,&lt);
    ::ClientToScreen(aWindow,&rb);
    rect.left = lt.x;
    rect.right = rb.x;
    rect.top = lt.y;
    rect.bottom = rb.y;
    ::ClipCursor(&rect);
    HPaned* This = (HPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      if(This->fRubberMove) {
        This->fEndMove.x = This->fBeginMove.x = LOWORD(aLParam);
        This->fBeginMove.y = rect.top;
        This->fEndMove.y = rect.bottom;
        This->rubberDrawLine(This->fEndMove,This->fBeginMove);
      }
    }
  }return 0;
  case WM_LBUTTONUP:{
    if(::GetCapture()==aWindow) {
      ::ReleaseCapture();
      ::ClipCursor(NULL);
      HPaned* This = (HPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        if(This->fRubberMove) {
          This->rubberDrawLine(This->fEndMove,This->fBeginMove);
        }
        int x = LOWORD(aLParam);
        This->setSplitPositionPixel(x);
        RECT rect;
        ::GetClientRect(aWindow,&rect);
        This->resizeChildren(rect.right-rect.left,rect.bottom-rect.top);
      }
      return 0;
    }
  } break;
  case WM_MOUSEMOVE:{
    unsigned int state = aWParam;
    if((state & MK_LBUTTON)==MK_LBUTTON) {
      HPaned* This = (HPaned*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        if(This->fRubberMove) {
          // Erase previous line :
          This->rubberDrawLine(This->fEndMove,This->fBeginMove);
          // New line :
          RECT rect;
          ::GetClientRect(aWindow,&rect);
          This->fEndMove.x = This->fBeginMove.x = LOWORD(aLParam);
          This->fBeginMove.y = rect.top;
          This->fEndMove.y = rect.bottom;
          This->rubberDrawLine(This->fEndMove,This->fBeginMove);
        } else {
          int x = LOWORD(aLParam);
          This->setSplitPositionPixel(x);
          RECT rect;
          ::GetClientRect(aWindow,&rect);
          This->resizeChildren(rect.right-rect.left,rect.bottom-rect.top);
        }
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HPaned::setSplitPosition(
 double aSplitPosition
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fSplitPosition = aSplitPosition;
}
//////////////////////////////////////////////////////////////////////////////
double WinTk::HPaned::splitPosition(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fSplitPosition;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HPaned::setSplitPositionPixel(
 int aSplitPosition
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  int width = rect.right-rect.left;
  if(width<=0) return;
  fSplitPosition = ((double)aSplitPosition)/((double)width);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HPaned::splitPositionPixel(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  RECT rect;
  ::GetClientRect(fWindow,&rect);
  int width = rect.right-rect.left;
  return (fSplitPosition>=0 ? DROUND(fSplitPosition * width) : width/2);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HPaned::resizeChildren(
 int aWidth
,int aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  HWND first = GetFirstChild(fWindow);
  HWND second = first ? GetNextSibling(first) : 0;
  if(first && second) {
    int wfirst = splitPositionPixel();
    SetGeometry(first,
                0,0,
                wfirst - fSplitHalfSize,aHeight);
    SetGeometry(second,
                wfirst + fSplitHalfSize,0,
                aWidth - wfirst - fSplitHalfSize,aHeight);
  } else if(first) {
    SetGeometry(first,0,0,aWidth,aHeight);
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::VContainer::VContainer(
 Component& aParent
)
:Component("VContainer",aParent)
,fMargin(5)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sVContainerClassName[] = "WinTk::VContainer";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::VContainer::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sVContainerClassName;
    wc.lpszClassName = sVContainerClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sVContainerClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE ,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
#ifdef DEBUG
  printf("debug : VContainer : %lu\n",fWindow);
#endif
}
//////////////////////////////////////////////////////////////////////////////
WinTk::VContainer::~VContainer(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::VContainer::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!containerProc(aWindow,aMessage,aWParam,aLParam)) return 0;
  switch(aMessage) { 
  case WM_SIZE:{
    VContainer* This = (VContainer*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      int width = LOWORD(aLParam);
      int height = HIWORD(aLParam);
      unsigned int childn = GetNumberOfChildren(aWindow);
      if(childn) {
        int widthChild = width - 2 * This->fMargin;
        int heightChild = 
          (height - This->fMargin - childn * This->fMargin)/childn;
        int index = 0;
        HWND child = GetFirstChild(aWindow);
        while(child) {
          SetGeometry(child,
                      This->fMargin,
                      This->fMargin + (This->fMargin + heightChild) * index,
                      widthChild,heightChild);
          index++;
          child = GetNextSibling(child);
        }
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::HContainer::HContainer(
 Component& aParent
)
:Component("HContainer",aParent)
,fMargin(5)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sHContainerClassName[] = "WinTk::HContainer";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::HContainer::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sHContainerClassName;
    wc.lpszClassName = sHContainerClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sHContainerClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::HContainer::~HContainer(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::HContainer::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!containerProc(aWindow,aMessage,aWParam,aLParam)) return 0;
  switch(aMessage) { 
  case WM_SIZE:{
    HContainer* This = (HContainer*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      int width = LOWORD(aLParam);
      int height = HIWORD(aLParam);
      unsigned int childn = GetNumberOfChildren(aWindow);
      if(childn) {
        int widthChild = 
          (width - This->fMargin - childn * This->fMargin)/childn;
        int heightChild = height - 2 * This->fMargin;
        int index = 0;
        HWND child = GetFirstChild(aWindow);
        while(child) {
          SetGeometry(child,
                      This->fMargin + (This->fMargin + widthChild) * index,
                      This->fMargin,
                      widthChild,heightChild);
          index++;
          child = GetNextSibling(child);
        }
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::MenuBar::MenuBar(
 Component& aParent
)
:Component("MenuBar",aParent)
,fMenu(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  // Parent should be a Shell window.
  HWND parent = fParent->nativeWindow();

  fMenu  = (HMENU)::GetWindowLong(parent,GWL_ID);
  if(fMenu) return;
  fMenu = CreateMenu();
  if(!fMenu) return;
  ::SetWindowLong(parent,GWL_ID,(LONG)fMenu);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::MenuBar::~MenuBar(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
HMENU WinTk::MenuBar::hmenu(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fMenu;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::CascadeButton::CascadeButton(
 Component& aParent
,const std::string& aLabel
)
:Component("CascadeButton",aParent)
,fLabel(aLabel)
,fParentMenu(0)
,fMenu(0)
,fPos(0)
//////////////////////////////////////////////////////////////////////////////
// Handle the HMENU here (and not in WinTk::Menu to be able to change label.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent) return;
  if(fParent->type()=="MenuBar") {
    fParentMenu = ((WinTk::MenuBar*)fParent)->hmenu();
  } else if(fParent->type()=="Menu") {
    fParentMenu = ((WinTk::Menu*)fParent)->hmenu();
  }
  if(!fParentMenu) return;
  fMenu = CreatePopupMenu();
  if(!fMenu) return;
  fPos = GetMenuItemCount(fParentMenu);
  ::AppendMenu(fParentMenu,MF_POPUP,(UINT)fMenu,aLabel.c_str());
}
//////////////////////////////////////////////////////////////////////////////
WinTk::CascadeButton::~CascadeButton(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //FIXME : DestroyMenu(fMenu);
}
//////////////////////////////////////////////////////////////////////////////
HMENU WinTk::CascadeButton::hmenu(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fMenu;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::CascadeButton::setLabel(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fMenu) return;
  if(!fParentMenu) return;
  fLabel = aString;
  if(!::ModifyMenu(fParentMenu,fPos,
                   MF_BYPOSITION|MF_STRING,fPos,fLabel.c_str())) return;
  refresh();
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::CascadeButton::label(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fLabel;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::CascadeButton::refresh(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  Component* parent = fParent;
  while(parent) {
    if(parent->type()=="Shell") {
      if(parent->nativeWindow()) ::DrawMenuBar(parent->nativeWindow());
      return;
    }
    parent = parent->parent();
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Menu::Menu(
 Component& aParent
)
:Component("Menu",aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent) return;
  if(fParent->type()!="CascadeButton") return;
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Menu::~Menu(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
HMENU WinTk::Menu::hmenu(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent) return 0;
  WinTk::CascadeButton* casc = (WinTk::CascadeButton*)fParent;
  return casc->hmenu();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::MenuItem::MenuItem(
 Component& aParent
,const std::string& aLabel
,int aID
,bool aIsToggle
)
:Component("MenuItem",aParent)
,fID(aID)
,fIsToggle(aIsToggle)
,fMenu(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent) return;
  if(fParent->type()!="Menu") return;
  WinTk::Menu* menu = (WinTk::Menu*)fParent;
  fMenu = menu->hmenu();
  if(!fMenu) return;
  ::AppendMenu(fMenu,MF_STRING,fID,aLabel.c_str());
}
//////////////////////////////////////////////////////////////////////////////
WinTk::MenuItem::~MenuItem(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(fParent && fMenu) ::DeleteMenu(fMenu,fID,MF_BYCOMMAND);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::MenuItem::isToggle(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fIsToggle;
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::MenuItem::identifier(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fID;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::MenuItem::setLabel(
 const std::string& aLabel
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent) return;
  if(!fMenu) return;
  ::ModifyMenu(fMenu,fID,MF_STRING,fID,aLabel.c_str());
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::MenuItem::label(
 std::string& aLabel
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent || !fMenu) {
    aLabel = "";
    return false;
  }
  int l = ::GetMenuString(fMenu,fID,NULL,0,MF_BYCOMMAND);
  if(l<=0) {
    aLabel = "";
    return true;
  }
  aLabel.resize(l);
  int s = ::GetMenuString(fMenu,fID,(LPSTR)aLabel.c_str(),l+1,MF_BYCOMMAND);
  return (s==0?false:true);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::MenuItem::addAccelerator(
 const std::string& aString
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  Component* w = findFather("Shell");
  if(!w) return false;
  return ((Shell*)w)->addAccelerator(aString,fID);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::MenuItem::isChecked(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent) return false;
  if(!fMenu) return false;
  MENUITEMINFO itemInfo;
  itemInfo.cbSize = sizeof(MENUITEMINFO);
  itemInfo.fMask = MIIM_STATE;
  ::GetMenuItemInfo(fMenu,(UINT)fID,FALSE,&itemInfo);
  return (itemInfo.fState==MFS_CHECKED ? true : false);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::MenuItem::setChecked(
 bool aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent) return;
  if(!fMenu) return;
  MENUITEMINFO itemInfo;
  itemInfo.cbSize = sizeof(MENUITEMINFO);
  itemInfo.fMask = MIIM_STATE;
  ::GetMenuItemInfo(fMenu,(UINT)fID,FALSE,&itemInfo);
  itemInfo.fState = aValue ? MFS_CHECKED : MFS_UNCHECKED;
  ::SetMenuItemInfo(fMenu,(UINT)fID,FALSE,&itemInfo);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::MenuItem::toggleChecked(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent) return;
  if(!fMenu) return;
  MENUITEMINFO itemInfo;
  itemInfo.cbSize = sizeof(MENUITEMINFO);
  itemInfo.fMask = MIIM_STATE;
  ::GetMenuItemInfo(fMenu,(UINT)fID,FALSE,&itemInfo);
  if(itemInfo.fState==MFS_CHECKED) {
    itemInfo.fState = MFS_UNCHECKED;
  } else {
    itemInfo.fState = MFS_CHECKED;
  }
  ::SetMenuItemInfo(fMenu,(UINT)fID,FALSE,&itemInfo);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::MenuSeparator::MenuSeparator(
 Component& aParent
)
:Component("MenuSeparator",aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent) return;
  if(fParent->type()!="Menu") return;
  WinTk::Menu* menu = (WinTk::Menu*)fParent;
  if(!menu->hmenu()) return;
  ::AppendMenu(menu->hmenu(),MF_SEPARATOR,0,0);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::MenuSeparator::~MenuSeparator(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::CommandLine::CommandLine(
 Component& aParent
)
:Component("CommandLine",aParent)
,fOldProc(0)
,fHistoryPos(-1)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  HDC hdc = ::GetDC(parent);
  TEXTMETRIC tm;
  ::GetTextMetrics(hdc,&tm);
  short charWidth = (short)tm.tmAveCharWidth;
  short charHeight = (short)(tm.tmHeight + tm.tmExternalLeading);
  ::ReleaseDC(parent,hdc);

  int cols = 80;

  fWindow = ::CreateWindow(WC_EDIT,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER
                           | WS_TABSTOP
                           | ES_AUTOHSCROLL ,
                           0,0,
                           cols * charWidth,charHeight,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));

  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  if(!findFather("Dialog")) {
    Component* w = findFather("Shell");
    if(w) ((Shell*)w)->setFocusWindow(fWindow);
  }

}
//////////////////////////////////////////////////////////////////////////////
WinTk::CommandLine::~CommandLine(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
  fHistory.clear();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::CommandLine::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  case WM_CHAR:
    switch(aWParam){
    case VK_RETURN:
    case VK_TAB:
      // Avoid beeping.
      return 0;
    }break;
  case WM_KEYDOWN:
    switch(aWParam){
    case VK_RETURN:{
      CommandLine* This = (CommandLine*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        This->fValue = WinTk::GetText(aWindow);

        Edit_SetText(aWindow,"");
        Edit_SetSel(aWindow,0,0);

        This->fHistory.push_back(This->fValue);
        This->fHistoryPos = -1;

        CallbackData data;
        This->executeCallbacks("activate",data);

      }
    }break;
    case VK_UP:{
      CommandLine* This = (CommandLine*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        unsigned int n = This->fHistory.size(); 
        int pos = This->fHistoryPos == -1 ? n-1 : This->fHistoryPos-1;
        if((pos>=0)&&(pos<(int)n)) {
          const std::string& command = This->fHistory[pos];
          const char* d = command.c_str();
          int l = ::strlen(d);
          Edit_SetText(aWindow,d);
          Edit_SetSel(aWindow,l,l);
          This->fHistoryPos = pos;
        }
      }
    }return 0; //Do not jump into oldEditProc.
    case VK_DOWN:{
      CommandLine* This = (CommandLine*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        unsigned int n = This->fHistory.size(); 
        int pos = This->fHistoryPos == -1 ? n : This->fHistoryPos + 1;
        if((pos>=0)&&(pos<(int)n)) {
          const std::string& command = This->fHistory[pos];
          const char* d = command.c_str();
          int l = ::strlen(d);
          Edit_SetText(aWindow,d);
          Edit_SetSel(aWindow,l,l);
          This->fHistoryPos = pos;
        } else if(pos>=(int)n) {
          Edit_SetText(aWindow,"");
          Edit_SetSel(aWindow,0,0);
          This->fHistoryPos = -1;
        }
      }
    }return 0; //Do not jump into oldEditProc.
    case VK_ESCAPE:{ //Completion.
      CommandLine* This = (CommandLine*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        This->fValue = WinTk::GetText(aWindow);
        CallbackData data;
        This->executeCallbacks("complete",data);
      }
    }break;
    }
  }
  CommandLine* This = (CommandLine*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
const std::string& WinTk::CommandLine::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fValue;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::CommandLine::setValue(
 const std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int l = aString.size();
  Edit_SetText(fWindow,aString.c_str());
  Edit_SetSel(fWindow,l,l);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::Entry::Entry(
 Component& aParent
,bool aPassword
)
:Component("Entry",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  int style = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;
  if(aPassword) style |= ES_PASSWORD;
  fWindow = ::CreateWindow(WC_EDIT,
                           NULL,
                           style,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
  if(!findFather("Dialog")) {
    Component* w = findFather("Shell");
    if(w) ((Shell*)w)->setFocusWindow(fWindow);
  }
#ifdef DEBUG
  printf("debug : Entry %lu\n",fWindow);
#endif
}
//////////////////////////////////////////////////////////////////////////////
WinTk::Entry::~Entry(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::Entry::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  case WM_CHAR:
    switch(aWParam){
    case VK_RETURN:{
      Entry* This = (Entry*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("activate",data);
      }
    }break;
    case VK_TAB:{  // Avoid beeping.
      Entry* This = (Entry*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        Dialog* dialog = (Dialog*)This->findFather("Dialog");
        if(dialog) {
          HWND dialogWindow = dialog->nativeWindow();
          HWND next = GetTabStop(dialogWindow,aWindow,
                                 (::GetKeyState(VK_SHIFT)&KEY_UP)?false:true);
          if(next) ::SetFocus(next);
        }
      }return 0;
    }break;
    }
  }
  Entry* This = (Entry*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::Entry::value(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return "";
  return WinTk::GetText(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::Entry::setValue(
 const std::string& aValue
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SetWindowText(fWindow,aValue.c_str());
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ScrolledList::ScrolledList(
 Component& aParent
)
:Component("ScrolledList",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(WC_LISTBOX,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER
                           | WS_VSCROLL | WS_HSCROLL
                           | WS_TABSTOP
                           | LBS_NOTIFY,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ScrolledList::~ScrolledList(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::ScrolledList::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_COMMAND:{ //Coming from the parent :
    int code = HIWORD(aWParam);
    if(code==LBN_SELCHANGE) {
      ScrolledList* This = 
        (ScrolledList*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        This->executeCallbacks("select",data);
      }
      return 0;
    }
  }break;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  ScrolledList* This = (ScrolledList*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ScrolledList::setItems(
 const std::vector<std::string>& aItems
,const std::string& aSelection
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  ListBox_ResetContent(fWindow);
  int found = NotFound;
  for(unsigned int index=0;index<aItems.size();index++) {
    ListBox_AddString(fWindow,aItems[index].c_str());
    if(aSelection==aItems[index]) found = index;
  }
  if(found!=NotFound) ListBox_SetCurSel(fWindow,found);
}
//////////////////////////////////////////////////////////////////////////////
std::vector<std::string> WinTk::ScrolledList::items(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> items;
  if(!fWindow) return items;
  int number = ListBox_GetCount(fWindow);
  for(int index=0;index<number;index++) {
    int len = ListBox_GetTextLen(fWindow,index);
    char* buffer = new char[len+1];
    ListBox_GetText(fWindow,index,buffer);
    items.push_back(buffer);
    delete [] buffer;
  }
  return items;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::ScrolledList::selection(
 std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  int index = ListBox_GetCurSel(fWindow);
  if(index==LB_ERR) return false;
  int len = ListBox_GetTextLen(fWindow,index);
  char* buffer = new char[len+1];
  ListBox_GetText(fWindow,index,buffer);
  aValue = buffer;
  delete [] buffer;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::ScrolledList::setSelection(
 const std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return false;
  //FIXME : the below will select the first begin-matching string !
  return (ListBox_SelectString(fWindow,-1,aString.c_str())==LB_ERR ? 
          false:true);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ScrolledText::ScrolledText(
 Component& aParent
)
:Component("ScrolledText",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(WC_EDIT,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER 
                           | WS_VSCROLL | WS_HSCROLL
                           | WS_TABSTOP
                           | ES_MULTILINE ,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  // Max number of characters is 100000
  ::SendMessage(fWindow, EM_LIMITTEXT, 100000, 0);

  // So that "\r\r\n" in a text does a carriage return, line feed :
  Edit_FmtLines(fWindow,TRUE);

  // Use a fixed font :
  SetWindowFont(fWindow,GetStockFont(SYSTEM_FIXED_FONT),FALSE);

}
//////////////////////////////////////////////////////////////////////////////
WinTk::ScrolledText::~ScrolledText(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::ScrolledText::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  ScrolledText* This = (ScrolledText*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::ScrolledText::value(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return "";
  return WinTk::GetText(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ScrolledText::setText(
 const std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();
  appendString(aString);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ScrolledText::clear(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  ::SetWindowText(fWindow,"");
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ScrolledText::appendString(
 const std::string& aString
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fWindow) return;
  if(aString.empty()) return;
  if(aString.find('\n')!=std::string::npos) {
    std::vector<std::string> text = inlib::words(aString,"\n",true);
    unsigned int linen = text.size(); // minimum = two lines.
    if(!linen) return; // Should never happen.
    // Append first text line to existing one :
    appendSimpleString(text[0]);
    // Add other lines but not the last one :
    unsigned int last = linen-1;
    for(unsigned int index=1;index<last;index++) {
      appendSimpleString(text[index]);
    }
    // If last one not empty, add it :
    if(text[last].length()) {
      appendSimpleString(text[last]);
    }
  } else {
    appendSimpleString(aString, false);
  }
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ScrolledText::appendSimpleString (
 const std::string& aString
,bool aCR
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // tests the length of the buffer. If there is no place for the
  // new text, we remove the first half of the buffer
  UINT limit = ::SendMessage(fWindow, EM_GETLIMITTEXT, 0, 0);
  // GetLimitText() gets the capacity of the edit control.
  // GetWindowTextLength() gets the length of the current contents.
  if (limit < ::GetWindowTextLength(fWindow) + aString.length() + 3) {
    // Now make sure we discard a whole number of lines, i.e.,
    // break on a line boundary, to make the display look nice
    // should user scroll way back up to the top.
    int linePos = ::SendMessage(fWindow,
                                EM_LINEINDEX,
                                ::SendMessage (fWindow,
                                               EM_LINEFROMCHAR,
                                               limit/2,
                                               0),
                                0);
    Edit_SetSel(fWindow,0,linePos);
    ::SendMessage(fWindow, WM_CLEAR, 0, 0);
    Edit_SetSel(fWindow,::GetWindowTextLength(fWindow),-1);
  }
  Edit_SetSel(fWindow,-1,-1);   //Position at end of buffer.
  // Append the new text at the end of the buffer
  // and add a cariage return if needed
  Edit_ReplaceSel(fWindow,aString.c_str());
  if (aCR) {
    Edit_SetSel(fWindow,-1,-1); 
    Edit_ReplaceSel(fWindow,"\r\r\n");
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::HScrollBar::HScrollBar(
 Component& aParent
)
:Component("HScrollBar",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(WC_SCROLLBAR,
                           NULL,
                           WS_CHILD | WS_VISIBLE 
                           | SBS_HORZ,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  ::ScrollBar_SetRange(fWindow,0,1000,FALSE);
  //::ScrollBar_Enable(fWindow,ESB_DISABLE_BOTH); //FIXME
}
//////////////////////////////////////////////////////////////////////////////
WinTk::HScrollBar::~HScrollBar(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::HScrollBar::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  case WM_HSCROLL:{ // Message coming from the parent (see container::proc) :
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_TRACKPOS;
    if(::GetScrollInfo(aWindow,SB_CTL,&scrollInfo)) {
      ::ScrollBar_SetPos(aWindow,scrollInfo.nTrackPos,FALSE);
    }
    HScrollBar* This = (HScrollBar*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      This->executeCallbacks("drag",data);
    }
  }return 0;
  }
  HScrollBar* This = (HScrollBar*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HScrollBar::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::ScrollBar_GetPos(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HScrollBar::setValue(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::ScrollBar_SetPos(fWindow,aValue,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HScrollBar::setMinimum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  ::ScrollBar_SetRange(fWindow,aValue,mx,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HScrollBar::setMaximum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  ::ScrollBar_SetRange(fWindow,mn,aValue,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HScrollBar::minimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  return mn;
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HScrollBar::maximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  return mx;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::VScrollBar::VScrollBar(
 Component& aParent
)
:Component("VScrollBar",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(WC_SCROLLBAR,
                           NULL,
                           WS_CHILD | WS_VISIBLE 
                           | SBS_VERT,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  ::ScrollBar_SetRange(fWindow,0,1000,FALSE);
  //::ScrollBar_Enable(fWindow,ESB_DISABLE_BOTH); //FIXME
}
//////////////////////////////////////////////////////////////////////////////
WinTk::VScrollBar::~VScrollBar(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::VScrollBar::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  case WM_VSCROLL:{ // Message coming from the parent (see container::proc) :
    SCROLLINFO scrollInfo;
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_TRACKPOS;
    if(::GetScrollInfo(aWindow,SB_CTL,&scrollInfo)) {
      ::ScrollBar_SetPos(aWindow,scrollInfo.nTrackPos,FALSE);
    }
    VScrollBar* This = (VScrollBar*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      This->executeCallbacks("drag",data);
    }
  }return 0;
  }
  VScrollBar* This = (VScrollBar*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VScrollBar::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::ScrollBar_GetPos(fWindow);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VScrollBar::setValue(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::ScrollBar_SetPos(fWindow,aValue,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VScrollBar::setMinimum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  ::ScrollBar_SetRange(fWindow,aValue,mx,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VScrollBar::setMaximum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  ::ScrollBar_SetRange(fWindow,mn,aValue,TRUE);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VScrollBar::minimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  return mn;
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VScrollBar::maximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  int mn,mx;
  ::ScrollBar_GetRange(fWindow,&mn,&mx);
  return mx;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::HScale::HScale(
 Component& aParent
)
:Component("HScale",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(TRACKBAR_CLASS,
                           NULL,
                           WS_CHILD | WS_VISIBLE 
                           | TBS_HORZ | TBS_AUTOTICKS,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  ::SendMessage(fWindow,TBM_SETRANGE,(WPARAM)0,MAKELONG(0,1000));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::HScale::~HScale(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::HScale::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  case WM_HSCROLL:{ // Message coming from the parent (see container::proc) :
    HScale* This = (HScale*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      This->executeCallbacks("drag",data);
    }
  }return 0;
  }
  HScale* This = (HScale*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HScale::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,TBM_GETPOS,0,0);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HScale::setValue(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,TBM_SETPOS,(WPARAM)TRUE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HScale::setMinimum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,TBM_SETRANGEMIN,(WPARAM)TRUE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::HScale::setMaximum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,TBM_SETRANGEMAX,(WPARAM)TRUE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HScale::minimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,TBM_GETRANGEMIN,0,0);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::HScale::maximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,TBM_GETRANGEMAX,0,0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::VScale::VScale(
 Component& aParent
)
:Component("VScale",aParent)
,fOldProc(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  fWindow = ::CreateWindow(TRACKBAR_CLASS,
                           NULL,
                           WS_CHILD | WS_VISIBLE 
                           | TBS_VERT | TBS_AUTOTICKS,
                           0,0,100,100,
                           parent,0,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
  fOldProc = (WNDPROC)::GetWindowLong(fWindow,GWL_WNDPROC);
  ::SetWindowLong(fWindow,GWL_WNDPROC,(LONG)proc);

  ::SendMessage(fWindow,TBM_SETRANGE,(WPARAM)0,MAKELONG(0,1000));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::VScale::~VScale(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::VScale::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  case WM_VSCROLL:{ // Message coming from the parent (see container::proc) :
    VScale* This = (VScale*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      This->executeCallbacks("drag",data);
    }
  }return 0;
  }
  VScale* This = (VScale*)::GetWindowLong(aWindow,GWL_USERDATA);
  if(!This) return 0;
  return CallWindowProc(This->fOldProc,aWindow,aMessage,aWParam,aLParam);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VScale::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,TBM_GETPOS,0,0);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VScale::setValue(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,TBM_SETPOS,(WPARAM)TRUE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VScale::setMinimum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,TBM_SETRANGEMIN,(WPARAM)TRUE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::VScale::setMaximum(
 int aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::SendMessage(fWindow,TBM_SETRANGEMAX,(WPARAM)TRUE,aValue);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VScale::minimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,TBM_GETRANGEMIN,0,0);
}
//////////////////////////////////////////////////////////////////////////////
int WinTk::VScale::maximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return 0;
  return ::SendMessage(fWindow,TBM_GETRANGEMAX,0,0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::FileSelectionDialog::FileSelectionDialog(
 Component& aParent
)
:Component("FileSelectionDialog",aParent)
,fDirMask("*.*")
,fMode("open")
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
WinTk::FileSelectionDialog::~FileSelectionDialog(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::FileSelectionDialog::setDirectory(
 const std::string& aDirectory
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fDirectory = aDirectory;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::FileSelectionDialog::setDirMask(
 const std::string& aDirMask
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  // Windows syntax : *.iv;*.wrl.
  fDirMask = aDirMask;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::FileSelectionDialog::setMode(
 const std::string& aMode
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fMode = aMode;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::FileSelectionDialog::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  OPENFILENAME ofn;
  TCHAR szFile[MAX_PATH+1];
  szFile[0] = '\0';

  char* buffer = new char[MAX_PATH+1];
  if(fDirectory.empty()) 
    ::GetCurrentDirectory(MAX_PATH+1, buffer);
  else 
    ::strcpy(buffer,fDirectory.c_str());
  //::SetCurrentDirectory(buffer);

  char* filter = new char[MAX_PATH];
  char* p = filter;
  ::strcpy(p,"Data Files");
  p += ::strlen("Data Files");
  *p++ = ' ';
  *p++ = '(';
  ::strcpy(p,fDirMask.c_str());
  p += fDirMask.size();
  *p++ = ')';
  *p++ = '\0';
  ::strcpy(p,fDirMask.c_str());
  p += fDirMask.size();
  *p++ = '\0';
    
  *p++ = '\0';
  *p = '\0';
    
  ofn.lStructSize       = sizeof(OPENFILENAME);
  ofn.hwndOwner         = parent;
  //ofn.lpstrFilter       = "All Files (*.*)\0\0\0";
  ofn.lpstrFilter       = filter;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter    = 0;
  ofn.nFilterIndex      = 0;
  ofn.lpstrFile         = szFile;
  ofn.nMaxFile          = sizeof(szFile);
  ofn.lpstrFileTitle    = NULL;
  ofn.nMaxFileTitle     = 0;
  ofn.lpstrInitialDir   = buffer;
  ofn.lpstrTitle        = "Data file";
  ofn.nFileOffset       = 0;
  ofn.nFileExtension    = 0;
  ofn.lpstrDefExt       = NULL;
  ofn.lpfnHook                 = NULL; 
  ofn.lCustData         = NULL;
  ofn.lpTemplateName    = NULL;
  ofn.Flags             = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
  int windowsMajorVersion = LOBYTE(LOWORD(GetVersion()));
  if (windowsMajorVersion >= 4) {
    // Use the "explorer" style file selection box on platforms that
    // support it (Win95 and NT4.0, both have a major version number
    // of 4)
    ofn.Flags |= OFN_EXPLORER;
  }
  if(fMode=="open")
    ofn.Flags |= OFN_FILEMUSTEXIST;

  int winCode = GetOpenFileName(&ofn);
  //printf("debug : askFile : %s\n",szFile);
  //fSession.out().println("debug : askFile : %s",szFile);
  
  delete [] buffer;
  delete [] filter;

  std::string value;
  value = winCode ? szFile : "";
  CallbackData data;
  data.value = value;
  if(winCode) {
    executeCallbacks("ok",data);
  } else {
    executeCallbacks("cancel",data);
  }

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::ColorSelectionDialog::ColorSelectionDialog(
 Component& aParent
)
:Component("ColorSelectionDialog",aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
WinTk::ColorSelectionDialog::~ColorSelectionDialog(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ColorSelectionDialog::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static COLORREF custColors[16] = {
    RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
    RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
    RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),
    RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),RGB(255,255,255)
  };

  CHOOSECOLOR cc;
  cc.lStructSize       = sizeof(CHOOSECOLOR);
  cc.hwndOwner         = parent;
  cc.hInstance         = 0;
  cc.rgbResult         = RGB(255,255,255);
  cc.lpCustColors      = custColors;
  cc.lCustData         = NULL;
  cc.lpfnHook          = NULL;
  cc.lpTemplateName    = NULL;
  //cc.Flags             = 0;
  cc.Flags             = CC_RGBINIT;
  int windowsMajorVersion = LOBYTE(LOWORD(GetVersion()));
  if (windowsMajorVersion >= 4) {
    cc.Flags |= CC_ANYCOLOR;
    //    cc.Flags |= CC_SOLIDCOLOR;
  }

  int winCode = ::ChooseColor(&cc);

  COLORREF color = cc.rgbResult;
  std::string value;
  if(winCode) {
    std::string s;
    inlib::sprintf(s,64,"%g %g %g",
                  ((double)GetRValue(color))/255,
                  ((double)GetGValue(color))/255,
                  ((double)GetBValue(color))/255);
    value = s;
  }
  CallbackData data;
  data.value = value;
  if(winCode) {
    executeCallbacks("ok",data);
  } else {
    executeCallbacks("cancel",data);
  }

}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
WinTk::PaintArea::PaintArea(
 Component& aParent
)
:Component("PaintArea",aParent)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sPaintAreaClassName[] = "WinTk::PaintArea";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::PaintArea::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sPaintAreaClassName;
    wc.lpszClassName = sPaintAreaClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  // The WS_BORDER is needed. Else probleme of size at startup.
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sPaintAreaClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));
}
//////////////////////////////////////////////////////////////////////////////
WinTk::PaintArea::~PaintArea(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::PaintArea::refresh(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fWindow) return;
  ::InvalidateRect(fWindow,NULL,FALSE);
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::PaintArea::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch (aMessage) { 
  case WM_PAINT:{
    PaintArea* This = (PaintArea*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      data.wparam = aWParam;
      data.lparam = aLParam;
      This->executeCallbacks("paint",data);
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static bool SetWindowPixelFormat(HDC);
//////////////////////////////////////////////////////////////////////////////
WinTk::OpenGLArea::OpenGLArea(
 Component& aParent
)
:Component("OpenGLArea",aParent)
,fContext(0)
,fHDC(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sOpenGLAreaClassName[] = "WinTk::OpenGLArea";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::OpenGLArea::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sOpenGLAreaClassName;
    wc.lpszClassName = sOpenGLAreaClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  // The WS_BORDER is needed. Else probleme of size at startup.
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sOpenGLAreaClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));

  // initialize OpenGL rendering :
  fHDC = ::GetDC(fWindow);
  if( fHDC && SetWindowPixelFormat(fHDC) ) {
    fContext = ::wglCreateContext(fHDC);
  }

}
//////////////////////////////////////////////////////////////////////////////
WinTk::OpenGLArea::~OpenGLArea(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(wglGetCurrentContext()!=NULL) wglMakeCurrent(NULL,NULL);
  if(fContext)        {
    wglDeleteContext(fContext);
    fContext = 0;
  }
  destroy();
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::OpenGLArea::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch (aMessage) { 
  case WM_PAINT:{
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(aWindow,&ps);
    OpenGLArea* This = (OpenGLArea*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      HDC hdc = This->fHDC;
      HGLRC context = This->fContext;
      if(hdc && context) {
        ::wglMakeCurrent(hdc,context);
        // User OpenGL paint code :

        CallbackData data;
        data.wparam = aWParam;
        data.lparam = aLParam;
        This->executeCallbacks("paint",data);

        ::SwapBuffers(hdc);
      }
    }
    EndPaint(aWindow,&ps);
  }return 0;
  case WM_LBUTTONDOWN:{
    OpenGLArea* This = (OpenGLArea*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      data.wparam = aWParam;
      data.lparam = aLParam;
      data.value = "ButtonPress";
      data.x = LOWORD(aLParam);
      data.y = HIWORD(aLParam);
      This->executeCallbacks("event",data);
    }
  }return 0;
  case WM_LBUTTONUP:{
    OpenGLArea* This = (OpenGLArea*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      CallbackData data;
      data.wparam = aWParam;
      data.lparam = aLParam;
      data.value = "ButtonRelease";
      data.x = LOWORD(aLParam);
      data.y = HIWORD(aLParam);
      This->executeCallbacks("event",data);
    }
  } return 0;
  case WM_MOUSEMOVE:{
    unsigned int state = aWParam;
    if((state & MK_LBUTTON)==MK_LBUTTON) {
      OpenGLArea* This = (OpenGLArea*)::GetWindowLong(aWindow,GWL_USERDATA);
      if(This) {
        CallbackData data;
        data.wparam = aWParam;
        data.lparam = aLParam;
        data.value = "MotionNotify";
        data.x = LOWORD(aLParam);
        data.y = HIWORD(aLParam);
        This->executeCallbacks("event",data);
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
bool SetWindowPixelFormat (
 HDC aHdc
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  PIXELFORMATDESCRIPTOR pfd;
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = 
    PFD_DRAW_TO_WINDOW | 
    PFD_SUPPORT_OPENGL | 
    PFD_DOUBLEBUFFER | 
    PFD_STEREO_DONTCARE;  
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cRedBits = 8;
  pfd.cRedShift = 16;
  pfd.cGreenBits = 8;
  pfd.cGreenShift = 8;
  pfd.cBlueBits        = 8;
  pfd.cBlueShift = 0;
  pfd.cAlphaBits = 0;
  pfd.cAlphaShift = 0;
  pfd.cAccumBits = 64;        
  pfd.cAccumRedBits = 16;
  pfd.cAccumGreenBits = 16;
  pfd.cAccumBlueBits = 16;
  pfd.cAccumAlphaBits = 0;
  pfd.cDepthBits = 32;
  pfd.cStencilBits = 8;
  pfd.cAuxBuffers = 0;
  pfd.iLayerType = PFD_MAIN_PLANE;
  pfd.bReserved        = 0;
  pfd.dwLayerMask = 0;
  pfd.dwVisibleMask = 0;
  pfd.dwDamageMask = 0;
  
  int pixelIndex = ::ChoosePixelFormat(aHdc,&pfd);
  if (pixelIndex==0) {
    // Let's choose a default index.
    pixelIndex = 1;        
    if (::DescribePixelFormat(aHdc, 
                              pixelIndex, 
                              sizeof(PIXELFORMATDESCRIPTOR), 
                              &pfd)==0) {
      return false;
    }
  }

  if (::SetPixelFormat(aHdc,pixelIndex,&pfd)==FALSE) return false;

  return true;
}
//////////////////////////////////////////////////////////////////////////////
int DROUND(
 double aDouble
)
//////////////////////////////////////////////////////////////////////////////
// From CoinGL/src/base/SbViewportRegion.cpp.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aDouble == (double) (int(aDouble))) return int(aDouble);
  else return (aDouble>0) ? int(aDouble+0.5) : -int(0.5-aDouble);
}

#include "../source/Core/OnX_gl2ps.h"

//////////////////////////////////////////////////////////////////////////////
bool WinTk::OpenGLArea::write_gl2ps(
 const std::string& aFileName
,const std::string&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  FILE* file = ::fopen(aFileName.c_str(),"w");
  if(!file) return false;

  int options = GL2PS_OCCLUSION_CULL 
    | GL2PS_BEST_ROOT 
    | GL2PS_SILENT
    | GL2PS_DRAW_BACKGROUND;
  int sort = GL2PS_BSP_SORT;
  //sort = GL2PS_SIMPLE_SORT;
    
  unsigned int width,height;
  size(width,height);

  GLint vp[4];
  vp[0] = 0;
  vp[1] = 0;
  vp[2] = width;
  vp[3] = height;

  int bufsize = 0;
  gl2psBeginPage("title","OnX_gl2ps", 
                 vp,GL2PS_EPS,sort,options, 
                 GL_RGBA,0, NULL,0,0,0,bufsize, 
                 file,aFileName.c_str());
    
  CallbackData data;
  //data.wparam = aWParam;
  //data.lparam = aLParam;
  executeCallbacks("paint",data);

  gl2psEndPage();
  ::fclose(file);

  return true;
}
