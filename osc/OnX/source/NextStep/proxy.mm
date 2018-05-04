
// proxy should be on pure NextStep (and not on OnX itself).

#import <Cocoa/Cocoa.h>
#import <OnX/NextStep/NSTk.h>
#import <OnX/NextStep/NSTools.h>

#import "../source/Core/Proxy.h"

#include <inlib/args>

#define ONX_HAS_INVENTOR

bool ns_proxy(
 void* aTag
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  aReturn = 0;
  NSObject* widget = (NSObject*)aTag;
  if(aWhat==PX_WRITE) {
    if(aArgs.size()!=3) return false;
    std::string file = aArgs[0];
    std::string format = aArgs[1];
    std::string opts = aArgs[2];
/*
    if(XtIsSubclass(widget,openGLAreaWidgetClass)) {
      if(format=="gl2ps") {
        int stat = OpenGLAreaWrite_gl2ps(widget,file.c_str(),opts.c_str());
        return stat?false:true;
      } else {
        return false;
      }
#ifdef ONX_HAS_INVENTOR
    if([widget isKindOfClass:[NSPageViewer class]]==YES) {
      //FIXME return ((NSPageViewer*)widget)->write(aWhat,aFile);
      //return true;
    }
#endif
*/
    return false;
  } else if(aWhat==PX_MANAGE) {
    if(aArgs.size()!=1) return false;
    bool value;
    if(!inlib::to(aArgs[0],value)) return false;
    return true;
  } else if(aWhat==PX_REFRESH) {
    if([widget isKindOfClass:[OnX_NSOpenGLArea class]]==YES) {
      [(OnX_NSOpenGLArea*)widget refresh];
    }
    return true;
  } else if(aWhat==PX_SIZE) {
    NSRect rect = OnX::getFrame(widget);
    typedef unsigned int uint_t;
    uint_t* sz = new uint_t[2];
    sz[0] = (unsigned int)rect.size.width;
    sz[1] = (unsigned int)rect.size.height;
    aReturn = sz;
    return true;
  } else if(aWhat==PX_POSITION) {
    NSRect rect = OnX::getFrame(widget);
    int* pos = new int[2];
    pos[0] = (int)rect.origin.x;
    pos[1] = (int)rect.origin.y;
    aReturn = pos;
    return true;
  } else if(aWhat==PX_SHOW) {
    if([widget isKindOfClass:[OnX_NSDialog class]]==YES) {
      NSRect rect = [(NSWindow*)widget frame];
      NSRect srect = [[NSScreen mainScreen] visibleFrame];
      if(rect.size.width>srect.size.width) 
        rect.size.width = srect.size.width;
      if(rect.size.height>srect.size.height) 
        rect.size.height = srect.size.height;
      NSPoint origin;
      origin.x = (srect.size.width - rect.size.width)/2;
      if(origin.x<0) origin.x = 0;
      origin.y = (srect.size.height - rect.size.height)/2;
      if(origin.y<0) origin.y = 0;
      [(NSWindow*)widget setFrameOrigin:origin];
      [(NSWindow*)widget makeKeyAndOrderFront:nil];
    } else if([widget isKindOfClass:[NSWindow class]]==YES) {
      [(NSWindow*)widget makeKeyAndOrderFront:nil];
    } else if([widget isKindOfClass:[OnX_NSFileChooser class]]==YES) {
      [(OnX_NSFileChooser*)widget show];
    } else if([widget isKindOfClass:[OnX_NSColorChooser class]]==YES) {
      [(OnX_NSColorChooser*)widget show:nil];
    } else if([widget isKindOfClass:[NSView class]]==YES) {
      NSWindow* win = [(NSView*)widget window];
      [win makeKeyAndOrderFront:nil];
    }
    return true;
  } else if(aWhat==PX_HIDE) {
    //SoXt::hide(widget);
    if([widget isKindOfClass:[NSWindow class]]==YES) {
      [(NSWindow*)widget orderOut:nil];
    } else if([widget isKindOfClass:[OnX_NSColorChooser class]]==YES) {
      [(OnX_NSColorChooser*)widget hide];
    } else if([widget isKindOfClass:[NSView class]]==YES) {
      NSWindow* win = [(NSView*)widget window];
      [win orderOut:nil];
    }
    return true;
  }
  return false;
}
