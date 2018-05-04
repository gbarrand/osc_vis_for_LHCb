#ifndef OnX_NextStepTk_h
#define OnX_NextStepTk_h 

#import <AppKit/NSButton.h>
#import <AppKit/NSMenuItem.h>
#import <AppKit/NSToolbar.h>
#import <AppKit/NSToolbarItem.h>
#import <AppKit/NSView.h>
#import <AppKit/NSOpenGLView.h>
#import <AppKit/NSOutlineView.h>
#import <AppKit/NSEvent.h>
#import <AppKit/NSTextField.h>
#import <AppKit/NSComboBox.h>
#import <AppKit/NSSecureTextField.h>
#import <AppKit/NSSlider.h>
#import <AppKit/NSScroller.h>
#import <AppKit/NSTabView.h>
#import <AppKit/NSSplitView.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSScrollView.h>

@class NSOpenPanel;
@class NSColorPanel;
@class NSPopUpButton;
@class OnX_NSAction;

@interface OnX_NSButton : NSButton {
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSToggleButton : OnX_NSButton {
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)action:(id)aSender;
@end

@interface OnX_NSMenuItem : NSMenuItem {
  NSMutableArray* fActivateActions;
}
- (id)initWithTitle:(NSString*)aString action:(SEL)aSelector keyEquivalent:(NSString*)aCharCode;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSToggleMenuItem : OnX_NSMenuItem {
}
- (id)initWithTitle:(NSString*)aString action:(SEL)aSelector keyEquivalent:(NSString*)aCharCode;
- (void)dealloc;
- (void)action:(id)aSender;
@end

@interface OnX_NSOptionMenu : NSView {
  NSButton* fLabel;
  NSPopUpButton* fPopup;
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
- (NSString*)label;
- (void)setLabel:(NSString*)aLabel;
- (NSString*)value;
- (void)setValue:(NSString*)aValue;
- (void)addItem:(NSString*)aValue;
- (void)removeItems;
@end

@interface OnX_NSEntry : NSTextField {
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSCommandLine : NSTextField {
  NSMutableArray* fActivateActions;
  NSMutableArray* fCompleteActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)addCompleteAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSSecureTextField : NSSecureTextField {
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSComboBox : NSComboBox {
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSToolbarItem : NSToolbarItem {
  NSMutableArray* fActivateActions;
}
- (id)initWithItemIdentifier:(NSString*)aData;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
@interface OnX_NSToolbar : NSToolbar {
#else
@interface OnX_NSToolbar : NSToolbar <NSToolbarDelegate> {
#endif
  NSMutableArray* fItems;
}
- (id)initWithIdentifier:(NSString*)aData;
- (void)dealloc;
- (void)addToolbarItem:(NSToolbarItem*)aItem;
@end

typedef enum _OnX_Expand {
  OnX_FIRST = 0,
  OnX_SECOND = 1,
  OnX_BOTH = 3
} OnX_Expand;

@interface OnX_NSVBox : NSView {
  OnX_Expand fExpand;  
  int fSizeOfFixed;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)setExpand:(OnX_Expand)aExpand;
- (void)addView:(NSView*)aView;
- (void)setSizeOfFixed:(int)aPosition;
@end

@interface OnX_NSHBox : NSView {
  OnX_Expand fExpand;  
  int fSizeOfFixed;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)setExpand:(OnX_Expand)aExpand;
- (void)addView:(NSView*)aView;
- (void)setSizeOfFixed:(int)aPosition;
@end

@interface OnX_NSVPaned : NSSplitView {
  int fSplitPosition;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addView:(NSView*)aView;
- (void)setSplitPosition:(int)aPosition;
@end

@interface OnX_NSHPaned : NSSplitView {
  int fSplitPosition;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addView:(NSView*)aView;
- (void)setSplitPosition:(int)aPosition;
@end

@interface OnX_NSVContainer : NSView {
}
- (id)init;
- (void)dealloc;
- (void)addView:(NSView*)aView;
@end

@interface OnX_NSHContainer : NSView {
}
- (id)init;
- (void)dealloc;
- (void)addView:(NSView*)aView;
@end

@interface OnX_NSOpenGLArea : NSOpenGLView {
  NSMutableArray* fPaintActions;
  NSMutableArray* fEventActions;
  NSEventType fEventType;
  NSPoint fEventLocation;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addPaintAction:(OnX_NSAction*)aAction;
- (void)addEventAction:(OnX_NSAction*)aAction;
- (NSEventType)eventType;
- (NSPoint)eventLocation;
- (void)drawRect:(NSRect)aRect;
- (void)refresh;
- (NSOpenGLPixelFormat*)createPixelFormat:(NSRect)aFrame;
- (void)mouseDown:(NSEvent*)aEvent;
- (void)mouseUp:(NSEvent*)aEvent;
- (void)mouseDragged:(NSEvent*)aEvent;
@end

@interface OnX_NSFileChooser : NSObject {
  bool fModeOpen;
  NSMutableArray* fOkActions;
  NSMutableArray* fCancelActions;
  NSString* fFilter;
  NSString* fDirectory;
  NSString* fFileName;
}
- (id)init;
- (void)dealloc;
- (void)addOkAction:(OnX_NSAction*)aAction;
- (void)addCancelAction:(OnX_NSAction*)aAction;
- (void)show;
- (void)openPanelDidEnd:(NSOpenPanel*)aPanel
             returnCode:(int)aRC 
            contextInfo:(void*)aContext;
- (NSString*)fileName;
- (void)setModeOpen:(bool)aMode;
- (void)setFilter:(NSString*)aFilter;
- (void)setDirectory:(NSString*)aDirectory;
@end

@interface OnX_NSColorChooser : NSObject {
  NSColorPanel* fColorPanel;
  NSMutableArray* fOkActions;
  NSString* fColor;
  NSObject* fVBox;
  NSObject* fBox;
  NSObject* fOk;
  NSObject* fApply;
  NSObject* fCancel;
  NSView* f_panel_view;
}
- (id)init;
- (void)dealloc;
- (void)addOkAction:(OnX_NSAction*)aAction;
- (void)show:(id)aSender;
- (void)hide;
- (NSString*)choosenColor;
- (void)ok_action:(id)aSender;
- (void)apply_action:(id)aSender;
- (void)cancel_action:(id)aSender;
@end

@interface OnX_NSTreeItem : NSObject {
  OnX_NSTreeItem* fParent;
  NSMutableArray* fChildren;
  NSString* fLabel;
  bool fExpanded;
}
- (id)initWithParent:(OnX_NSTreeItem*)aParent;
- (void)dealloc;
- (void)addObject:(OnX_NSTreeItem*)aItem;
- (int)numberOfChildren;
- (OnX_NSTreeItem*)childAtIndex:(int)aIndex;
- (NSString*)label;
- (void)setLabel:(NSString*)aLabel;
- (NSString*)path;
- (void)setExpanded:(bool)aValue;
- (bool)expanded;
@end

@class OnX_NSTree;

@interface OnX_NSScrolledTree : NSScrollView {
  OnX_NSTree* fTree;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
// proxy to fTree :
- (void)setRootItem:(OnX_NSTreeItem*)aItem;
- (void)addSelectAction:(OnX_NSAction*)aAction;
- (void)addSelectBranchAction:(OnX_NSAction*)aAction;
- (void)addOpenAction:(OnX_NSAction*)aAction;
- (void)addCloseAction:(OnX_NSAction*)aAction;
- (NSString*)selectPath;
- (NSString*)openPath;
- (NSString*)closePath;
- (OnX_NSTreeItem*)rootItem;
- (int)selectedRow;
- (void)selectRowIndexes:(NSIndexSet*)indexes byExtendingSelection:(BOOL)extend;
@end

@class OnX_NSList;

@interface OnX_NSScrolledList : NSScrollView {
  OnX_NSList* fList;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)setRootItem:(OnX_NSTreeItem*)aItem;
- (void)addSelectAction:(OnX_NSAction*)aAction;
- (NSString*)selection;
- (BOOL)setSelection:(NSString*)aSelection;
@end

@interface OnX_NSScrolledText : NSScrollView {
  NSText* fText;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (NSString*)value;
- (void)setValue:(NSString*)value;
- (void)insertText:(NSString*)text;
@end

@interface OnX_NSSlider : NSSlider {
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

@interface OnX_NSScroller : NSScroller {
  NSMutableArray* fActivateActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addActivateAction:(OnX_NSAction*)aAction;
- (void)action:(id)aSender;
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
@interface OnX_NSTabView : NSTabView {
#else
@interface OnX_NSTabView : NSTabView <NSTabViewDelegate> {
#endif
  NSMutableArray* fSelectActions;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)addSelectAction:(OnX_NSAction*)aAction;
@end

@interface OnX_NSWindow : NSWindow {
  NSMutableArray* fDestroyActions;
}
- (id)initWithContentRect:(NSRect)aRect mask:(unsigned int)aMask;
- (void)dealloc;
- (void)addDestroyAction:(OnX_NSAction*)aAction;
@end

@interface OnX_NSDialog : NSWindow {
}
- (id)initWithContentRect:(NSRect)aRect;
- (void)dealloc;
@end

#endif
