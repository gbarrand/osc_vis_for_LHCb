
// proxy should be on pure Xt/Motif (and not on OnX itself).

#include <OnX/Xt/XtTools.h>

#include <inlib/sto>

#include <OnX/Xt/OpenGLArea.h>

//#include <Inventor/Xt/SoXt.h> //FIXME

#include "../source/Core/Proxy.h"

bool xt_proxy(
 void* aTag
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  aReturn = 0;
  ::Widget widget = (::Widget)aTag;
  if(aWhat==PX_WRITE) {
    if(aArgs.size()!=3) return false;
    std::string file = aArgs[0];
    std::string format = aArgs[1];
    std::string opts = aArgs[2];
    if(XtIsSubclass(widget,openGLAreaWidgetClass)) {
      if(format=="gl2ps") {
        int stat = OpenGLAreaWrite_gl2ps(widget,file.c_str(),opts.c_str());
        return stat?false:true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else if(aWhat==PX_SIZE) {
    typedef unsigned int uint_t;
    uint_t* sz = new uint_t[2];
    sz[0] = XWidgetGetWidth(widget);
    sz[1] = XWidgetGetHeight(widget);
    aReturn = sz;
    return true;
  } else if(aWhat==PX_POSITION) {
    int* pos = new int[2];
    pos[0] = XWidgetGetX(widget);
    pos[1] = XWidgetGetY(widget);
    aReturn = pos;
    return true;
  } else if(aWhat==PX_MANAGE) {
    if(aArgs.size()!=1) return false;
    bool value;
    if(!inlib::to(aArgs[0],value)) return false;
    if(value) 
      XtManageChild(widget);
    else 
      XtUnmanageChild(widget);
    return true;
  } else if(aWhat==PX_REFRESH) {
    if(XtIsSubclass(widget,openGLAreaWidgetClass)) {
      OpenGLAreaPaint(widget);
    }
    return true;
  } else if(aWhat==PX_SHOW) {
    XmShow(widget);
    return true;
  } else if(aWhat==PX_HIDE) {
    //SoXt::hide(widget);
    XmHide(widget);
    return true;
  }
  return false;
}
