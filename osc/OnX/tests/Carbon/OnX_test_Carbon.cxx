
#include <Carbon/Carbon.h>

int main() {

  ProcessSerialNumber psn;
  if(GetCurrentProcess(&psn) == noErr) {
    SetFrontProcess(&psn);
  }

  OSStatus ret;

  MenuBarHandle mac_menubar = GetMenuBar();

  //MenuRef root = AcquireRootMenu();
  //ReleaseMenu(root);

  MenuRef apple_menu;
  ret = CreateNewMenu(0, 0, &apple_menu);
  printf("debug : @+@+@ %d %ld\n",ret,apple_menu);
  if (ret != noErr) return 0;
  InsertMenu(apple_menu,-1);

  //InvalMenuBar();
  //DisposeMenuBar(mac_menubar);

  MenuAttributes attr = 0;
  //attr |= kMenuAttrAutoDisable;
  //attr |= kMenuItemAttrCustomDraw;
  MenuRef menu;
  ret = CreateNewMenu(0, attr, &menu);
  printf("debug : @@@ %d %ld\n",ret,menu);
  if (ret != noErr) return 0;
  //ReleaseMenu(ret);
  //SetMenuID(menu, ++mid);
  SetMenuTitleWithCFString(menu,CFSTR("Hello"));
  InsertMenu(menu,-1);
  //AppendMenuItemText(menu,"item 1");

  //SetRootMenu(root);
  SetMenuBar(mac_menubar);
  //InvalMenuBar();

  Rect null_rect;
  SetRect(&null_rect, 0, 0, 100, 100);
  WindowRef w = 0;
  //WindowClass wclass = kOverlayWindowClass;
  //WindowClass wclass = kPlainWindowClass;
  WindowClass wclass = kDocumentWindowClass;
  //WindowAttributes wattr = kWindowNoAttributes;
  WindowAttributes wattr = kWindowCollapseBoxAttribute | kWindowResizableAttribute;
  //WindowAttributes wattr = kWindowResizableAttribute;

  ret = CreateNewWindow(wclass, wattr, &null_rect, &w);
  if (ret != noErr) return 0;
/*
  Rect r;
  SetRect(&r, 0, 0, 100, 100);
  //ret = SetWindowBounds(w, kWindowContentRgn, &r);
  if(ret != noErr) return 0;
  printf("debug : 002\n");
*/

  ShowHide(w, true);

  printf("debug : event loop...\n");
  while(1) {
  EventRef event;
  do {
    do {
      if(ReceiveNextEvent(0,0,kEventDurationNoWait,TRUE,&event)!=noErr)  break;
      //if(qt_mac_send_event(flags, event))  nevents++;
      ReleaseEvent(event);
    } while(GetNumEventsInQueue(GetMainEventQueue()));
    //QApplication::sendPostedEvents();
  } while(GetNumEventsInQueue(GetMainEventQueue()));

  }
  printf("debug : exit...\n");

  return 0;
}
