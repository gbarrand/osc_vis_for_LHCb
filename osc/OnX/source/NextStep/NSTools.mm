// this :
#include <OnX/NextStep/NSTools.h>

#define ONX_HAS_INVENTOR
#ifdef ONX_HAS_INVENTOR
#import <OnX/NextStep/NSSoExaminerViewer.h>
#endif

NSRect OnX::getFrame(NSObject* aObject){
  if([aObject isKindOfClass:[NSWindow class]]==YES) {
    NSRect rect = [[(NSWindow*)aObject contentView] frame];
    //printf("debug : getFrame Window rec %g %g %g %g\n",
      //rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);
    return rect;
#ifdef ONX_HAS_INVENTOR
  } else if([aObject isKindOfClass:[NSSoExaminerViewer class]]==YES) {
    NSRect rect = [[(NSSoExaminerViewer*)aObject view] frame];
    return rect;
#endif
  } else if([aObject isKindOfClass:[NSView class]]==YES) {
    NSRect rect = [(NSView*)aObject frame];
    return rect;
  } else {
    NSRect rect = {0,0,0,0};
    return rect;
  }
}

#include <OnX/NextStep/NSTk.h>

inline void append(NSString*& aFrom,const char* aString) {
  NSString* nsa = [[NSString alloc] initWithUTF8String:aString];
  NSString* ns = [aFrom stringByAppendingString:nsa];
  [nsa release];
  [aFrom release];
  aFrom = [[NSString alloc] initWithString:ns];
  //[ns release]; //not owner.
}

inline void append(NSString*& aFrom,NSString* aAppend) {
  NSString* ns = [aFrom stringByAppendingString:aAppend];
  [aFrom release];
  aFrom = [[NSString alloc] initWithString:ns];
  //[ns release]; //not owner.
}

NSString* OnX::getXML(OnX_NSTreeItem* aItem) {
  NSString* line = [[NSString alloc] init];
  int number = [aItem numberOfChildren];
  int index;
  for(index=0;index<number;index++) {
    OnX_NSTreeItem* item = (OnX_NSTreeItem*)[aItem childAtIndex:index];

    append(line,"<treeItem>");
    append(line,"<label>");

    NSString* nss = [item label];
    //FIXME std::string s = nss ? std::string([nss cString]) : "";
    //s = Lib::smanip::toxml(s);
    if(nss) append(line,nss);

    append(line,"</label>");

    append(line,"<opened>");
    if([item expanded]) append(line,"true");
    else append(line,"false");
    append(line,"</opened>");

    if ([item numberOfChildren]!=0) {
      NSString* ns = getXML(item);
      append(line,ns);
      [ns release];
    }    
    append(line,"</treeItem>");
  }
  return line;
}

unsigned int OnX::NSGetDecorationMask(const std::vector< std::pair<bool,std::string> >& aItems){
  // get decorations
  // default values for decorations are in BaseUI.cxx
  bool close_button = true;
  bool iconify_button = true;
  bool min_max_button = true;
  bool resize_handle = true;
  bool border = true;
  bool caption = true;
  bool system_menu = true;
  std::vector< std::pair<bool,std::string> >::const_iterator it;
  for(it=aItems.begin();it!=aItems.end();++it) {
    if ((*it).second== "close_button") close_button = (*it).first; 
    if ((*it).second == "iconify_button") iconify_button = (*it).first; 
    if ((*it).second == "min_max_button") min_max_button = (*it).first; 
    if ((*it).second == "resize_handle") resize_handle = (*it).first; 
    if ((*it).second == "border") border = (*it).first;
    if ((*it).second == "caption") caption = (*it).first; 
    if ((*it).second == "system_menu") system_menu = (*it).first;
  }
  
  unsigned int mask = 0;

  if(close_button)  {
    mask |= NSClosableWindowMask;
  }
  
  if(iconify_button) {
    mask |= NSMiniaturizableWindowMask;
  }

  if(min_max_button) {
    mask |= NSResizableWindowMask;
  }

  if(resize_handle) {
    mask |= NSResizableWindowMask;
  }

  if(!border) {
    mask |= NSBorderlessWindowMask;
  }  

  if(caption) {
    mask |= NSTitledWindowMask;
  }

  if (system_menu) {
  }

  return mask;  
}
