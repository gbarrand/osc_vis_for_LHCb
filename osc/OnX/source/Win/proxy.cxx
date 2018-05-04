
// proxy should be on pure WinTk (and not on OnX itself).

#include <inlib/smanip>

#include <OnX/Win/WinTk.h>
#include <OnX/Win/WinTools.h>

#include "../source/Core/Proxy.h"

bool win_proxy(
 void* aTag
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  aReturn = 0;
  WinTk::Component* widget = (WinTk::Component*)aTag;
  if(aWhat==PX_WRITE) {
    if(aArgs.size()!=3) return false;
    std::string file = aArgs[0];
    std::string format = aArgs[1];
    std::string opts = aArgs[2];
    if(widget->type()=="OpenGLArea") {
      if(format=="gl2ps") {
        return ((WinTk::OpenGLArea*)widget)->write_gl2ps(file,opts);
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else if(aWhat==PX_SIZE) {
    typedef unsigned int uint_t;
    uint_t* sz = new uint_t[2];
    aReturn = sz;
    return widget->size(sz[0],sz[1]);
  } else if(aWhat==PX_POSITION) {
    int* pos = new int[2];
    aReturn = pos;
    return widget->position(pos[0],pos[1]);
  } else if(aWhat==PX_MANAGE) {
    if(aArgs.size()!=1) return false;
    bool value;
    if(!inlib::to(aArgs[0],value)) return false;
    //FIXME
    //if(value) 
    //  XtManageChild(widget);
    //else 
    //  XtUnmanageChild(widget);
    return false;
  } else if(aWhat==PX_REFRESH) {
    if(widget->type()=="OpenGLArea") {
      HWND window = widget->nativeWindow();
      if(!window) return false;
      ::PostMessage(window,WM_PAINT,0,0);
      //FIXME : the below does not work with refresh indirectly 
      //  triggered by mouse events.
      //::UpdateWindow(window);
      return true;
    }
  } else if(aWhat==PX_SHOW) {
    if(widget->type()=="FileSelectionDialog") {
      widget->show();
    } else if(widget->type()=="ColorSelectionDialog") {
      widget->show();
    } else if(widget->type()=="ColorChooser") {
      widget->show();
    } else if(widget->type()=="WarningDialog") {
      widget->show();
    } else {
      HWND window = widget->nativeWindow();
      if(window) WinTk::Show(window);
    }
    return true;
  } else if(aWhat==PX_HIDE) {
    HWND window = widget->nativeWindow();
    if(window) WinTk::Hide(window);
    return true;
  }
  return false;
}
