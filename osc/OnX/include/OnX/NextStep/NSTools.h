#ifndef OnX_NSTools_h
#define OnX_NSTools_h 

#include <Cocoa/Cocoa.h>

#include <string>
#include <vector>

@class OnX_NSTreeItem;
@class OnX_NSTree;

namespace OnX {

  NSRect getFrame(NSObject*);
  NSString* getXML(OnX_NSTreeItem*);
  unsigned int NSGetDecorationMask(const std::vector< std::pair<bool,std::string> >&);

}

#endif

