// this :
#include <OnX/Win/PageViewer.h>

#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <OnX/Inventor/GuiViewer.h>

#include <OnX/Win/WinTools.h>

#include <inlib/sprintf>

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

#define KEY_UP 0x8000

namespace WinTk {
class PageViewerCollectCallback : public virtual SbCollectCallback {
public: //SbCollectCallback
  virtual void execute(const SoCanvas& aPage,const SoNodeList& aValue) {
    CallbackData data;
    data.value = "";
    int number = aValue.getLength();
    for(int index=0;index<number;index++) {
      if(index) data.value += "\n";
      data.value += std::string(aValue[index]->getName().getString());      
    }
    fPageViewer.executeCallbacks("collect",data);
  }
public:
  PageViewerCollectCallback(PageViewer& aPageViewer):fPageViewer(aPageViewer){}
private:
  PageViewer& fPageViewer;
};
}
//////////////////////////////////////////////////////////////////////////////
WinTk::PageViewer::PageViewer(
 Component& aParent
,const std::string& aName
,const std::string& aViewerClass
)
:Component("PageViewer",aParent)
,fViewer(0)
,fCounter(314)
,fRubberMove(false)
//////////////////////////////////////////////////////////////////////////////
// Handle a window to handle the pick mode popup menu messages.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBeginMove.x = 0;
  fBeginMove.y = 0;
  fEndMove.x = 0;
  fEndMove.y = 0;
  fOffset.x = 0;
  fOffset.y = 0;

  if(!fParent || !fParent->nativeWindow()) return;
  HWND parent = fParent->nativeWindow();

  static char sPageViewerClassName[] = "WinTk::PageViewer";
  static bool done = false;
  if(!done) {
    WNDCLASS         wc;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WinTk::PageViewer::proc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetWindowInstance(parent),
    wc.hIcon         = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName  = sPageViewerClassName;
    wc.lpszClassName = sPageViewerClassName;
    ::RegisterClass(&wc);
    done = true;
  }
  
  // The WS_BORDER is needed. Else probleme of size at startup.
  RECT rect;
  ::GetClientRect(parent,&rect);
  fWindow = ::CreateWindow(sPageViewerClassName,
                           NULL,
                           WS_CHILD | WS_VISIBLE | WS_BORDER,
                           0,0,
                           rect.right-rect.left,
                           rect.bottom-rect.top,
                           parent,NULL,
                           GetWindowInstance(parent),
                           NULL);
  if(!fWindow) return;
  ::SetWindowLong(fWindow,GWL_USERDATA,LONG(this));

  if(aViewerClass=="Plane") {
    fViewer = new SoWinPlaneViewer(fWindow,aName.c_str(),TRUE);
  } else {
    fViewer = new SoWinExaminerViewer(fWindow,aName.c_str(),TRUE);
  }
  GuiViewer_initialize<SoWinViewer>(*fViewer);

#ifdef __COIN__
  // To be coherent with SoWinComponent::getComponent.
  //fWindow = fViewer->getParentWidget();
#else
  //fWindow = fViewer->getWidget();
#endif
  setName(fViewer->getWidgetName());

  SoPage* soPage = GuiViewer_SoPage<SoWinViewer>(*fViewer);
  if(soPage) {
    soPage->setCollectCallback(new PageViewerCollectCallback(*this));
  }

  // The Inventor HWND viewer has not yet the right size.
  // It will be forced through the WM_TK_CHILDCREATED mechanism.

  // Create a "pick mode" popup menu :
  fPopupMenu = ::CreatePopupMenu();
  fViewer->setEventCallback(eventCB,this);
}
//////////////////////////////////////////////////////////////////////////////
WinTk::PageViewer::~PageViewer(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(fViewer) {
    GuiViewer_finalize<SoWinViewer>(*fViewer);
    if(fViewer->isOfType(SoWinExaminerViewer::getClassTypeId())) {
      delete (SoWinExaminerViewer*)fViewer;
    } else if(fViewer->isOfType(SoWinPlaneViewer::getClassTypeId())) {
      delete (SoWinPlaneViewer*)fViewer;
    } else {
      SoDebugError::postInfo
        ("WinTk::PageViewer::~PageViewer","unknown viewer class");
    }
    fViewer = 0;
  }

  destroy();
  //FIXME : have to destroy fPopupMenu.
}
//////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WinTk::PageViewer::proc( 
 HWND   aWindow
,UINT   aMessage
,WPARAM aWParam
,LPARAM aLParam
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  switch(aMessage) { 
  case WM_COMMAND:{  // Message coming from the popup menu :
    PageViewer* This = (PageViewer*)::GetWindowLong(aWindow,GWL_USERDATA);
    if(This) {
      std::vector< std::pair<int,std::string> >::iterator it;
      for(it=This->fPopupEntries.begin();it!=This->fPopupEntries.end();++it) {
        if(aWParam==(*it).first) {
          CallbackData data;
          data.value = (*it).second;
          data.x = This->fPopX;
          data.y = This->fPopY;
          This->executeCallbacks("popup",data);
          break;
        }
      }
    }
  }return 0;
  case WM_DESTROY:wm__destroy(aWindow);return 0;
  }
  return (DefWindowProc(aWindow,aMessage,aWParam,aLParam));
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::PageViewer::addPopupEntry(
 const std::string& aLabel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  ::AppendMenu(fPopupMenu,MF_STRING,fCounter,aLabel.c_str());
  fPopupEntries.push_back(std::pair<int,std::string>(fCounter,aLabel));
  fCounter++;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::PageViewer::show(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  //if(!fViewer) return;
  //fViewer->show();
}
//////////////////////////////////////////////////////////////////////////////
SoNode* WinTk::PageViewer::sceneGraph(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fViewer) return 0;
  return fViewer->getSceneGraph();
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::PageViewer::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!fViewer) return false;
  SoPage* soPage = GuiViewer_SoPage<SoWinViewer>(*fViewer);
  if(!soPage) return false;
  soPage->verbose.setValue(aVerboseLevel?TRUE:FALSE);
  return true;
}
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>

//////////////////////////////////////////////////////////////////////////////
SbBool WinTk::PageViewer::eventCB(
 void* aTag
,MSG* aEvent
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  PageViewer* This = (PageViewer*)aTag;

  if(aEvent->message==WM_RBUTTONDOWN) {
    if(This->fViewer->isViewing()==TRUE) return FALSE;

    HWND window = This->fViewer->getNormalWidget();
    if(window!=aEvent->hwnd) {
      printf("debug : WinTk::PageViewer::eventCB : HWND mismatch\n");
      return FALSE;
    }

    if(window && This->fPopupMenu && This->nativeWindow()) {

      RECT rect;
      ::GetClientRect(window,&rect);
      POINT point;
      point.x = aEvent->pt.x;
      point.y = aEvent->pt.y;
      ::ClientToScreen(window,&point);
      ::TrackPopupMenu(This->fPopupMenu,TPM_RIGHTBUTTON,
                       point.x,point.y,0,This->nativeWindow(),0);
      
      This->fPopX = aEvent->pt.x; 
      This->fPopY = (rect.bottom-rect.top)-aEvent->pt.y; //GL coordinates.
    }

    return FALSE; //So that SoPage::mouseEventCB is treated.

  } else if(aEvent->message==WM_LBUTTONDOWN) {

    if((::GetKeyState(VK_SHIFT)&KEY_UP)   && 
       (::GetKeyState(VK_CONTROL)&KEY_UP) ) {

      HWND window = This->fViewer->getNormalWidget();
      if(window!=aEvent->hwnd) {
        printf("debug : WinTk::PageViewer::eventCB : HWND mismatch\n");
        return FALSE;
      }
  
      This->fRubberMove = true;
  
      RECT rect;
      ::GetWindowRect(window,&rect);
      RECT rect2;
      ::GetWindowRect(This->fWindow,&rect2);
      This->fOffset.x = rect.left - rect2.left;
      This->fOffset.y = rect.top - rect2.top;
  
      ::GetClientRect(window,&rect);
      This->fEndMove.x = This->fBeginMove.x = aEvent->pt.x + This->fOffset.x;
      This->fEndMove.y = This->fBeginMove.y = aEvent->pt.y + This->fOffset.y;
      This->rubberDrawRect(This->fEndMove,This->fBeginMove);
  
      return TRUE;
    } else {
      // Simple pick :
      if(This->hasCallbacks("collect")) {
      } else {
        SoPage* soPage = GuiViewer_SoPage<SoWinViewer>(*(This->fViewer));
        if(soPage) soPage->setHandlePickedPoint(FALSE);
      }
      return FALSE; //And then let the viewer treats also the event.
    }

  } else if(aEvent->message==WM_LBUTTONUP) {

    if(This->fRubberMove) {
      HWND window = This->fViewer->getNormalWidget();
      if(window!=aEvent->hwnd) {
        printf("debug : WinTk::PageViewer::eventCB : HWND mismatch\n");
        return FALSE;
      }

      This->fRubberMove = false;
      This->rubberDrawRect(This->fEndMove,This->fBeginMove);

      RECT rect;
      ::GetClientRect(window,&rect);

      int x,y;
      unsigned int w,h;
      This->getRectangle(This->fBeginMove.x - This->fOffset.x,
                         This->fBeginMove.y - This->fOffset.y,
                         This->fEndMove.x - This->fOffset.x,
                         This->fEndMove.y - This->fOffset.y,
                         x,y,w,h);

      // We pass in GL coordinate (origin is bottom-left, y toup) :
      int xgl = x;
      int ygl = (rect.bottom-rect.top) - (y+h);

      if(This->fViewer->isViewing()==TRUE) {
        GuiViewer_map<SoWinViewer>(*(This->fViewer),
                          SbVec2s((short)xgl,(short)ygl),
                          SbVec2s((short)w,(short)h),
                          SbVec2s((short)(rect.right-rect.left),
                                  (short)(rect.bottom-rect.top)));
      } else {


        CallbackData data;
        inlib::sprintf(data.value,132,"%d %d %d %d",xgl,ygl,w,h);
        This->executeCallbacks("rectangularCollect",data);
      }

      return TRUE;
    }

  } else if(aEvent->message==WM_MOUSEMOVE) {

    HWND window = This->fViewer->getNormalWidget();
    if(window!=aEvent->hwnd) {
      printf("debug : WinTk::PageViewer::eventCB : HWND mismatch\n");
      return FALSE;
    }

    RECT rect;
    ::GetClientRect(window,&rect);

    //WARNING : we can pass here whilst the event is out
    // of the drawing area window !

    if( (aEvent->pt.x<0) || (aEvent->pt.x>=rect.right) ||
        (aEvent->pt.y<0) || (aEvent->pt.y>=rect.bottom) ) return FALSE;

    if((::GetKeyState(VK_SHIFT)&KEY_UP)   && 
       (::GetKeyState(VK_CONTROL)&KEY_UP) ){
      if(This->fRubberMove){
        // Erase previous rect :
        This->rubberDrawRect(This->fEndMove,This->fBeginMove);
        // New rect :
        This->fEndMove.x = aEvent->pt.x + This->fOffset.x;
        This->fEndMove.y = aEvent->pt.y + This->fOffset.y;
        This->rubberDrawRect(This->fEndMove,This->fBeginMove);
      }
      return TRUE;
    } else {
      SbVec2s pos(aEvent->pt.x,(rect.bottom-rect.top)-aEvent->pt.y); //GL
      //Picking mode : not discarded. We can be in resize/move region.
      if(This->fViewer->isViewing()==FALSE) return FALSE; //Picking mode.
      // Discard mouse move event not in current region in viewing mode.
      if(!GuiViewer_isInCurrentRegion<SoWinViewer>
            (*(This->fViewer),pos)) return TRUE;
    }

  } else if(aEvent->message==WM_KEYDOWN) {
    if(aEvent->wParam==VK_BACK) {
      if(This->fViewer->isViewing()!=TRUE) return FALSE;
      GuiViewer_popCamera<SoWinViewer>(*(This->fViewer));
      return TRUE;
    }
  }

  return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::PageViewer::getRectangle(
 int aBeginX
,int aBeginY
,int aEndX
,int aEndY
,int& aX
,int& aY
,unsigned int& aWidth
,unsigned int& aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aX = MINIMUM(aBeginX,aEndX);
  aY = MINIMUM(aBeginY,aEndY);
  aWidth = MAXIMUM(aBeginX,aEndX) - aX;
  aHeight = MAXIMUM(aBeginY,aEndY) - aY;
}
SoWinFullViewer* WinTk::PageViewer::soViewer() const {return fViewer;}

