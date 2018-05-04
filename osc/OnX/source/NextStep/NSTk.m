
#include <OnX/NextStep/NSTk.h>

#import <AppKit/NSOpenPanel.h>
#import <AppKit/NSSavePanel.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/NSTableColumn.h>
#import <AppKit/NSTableHeaderCell.h>
#import <AppKit/NSColorPanel.h>
#import <AppKit/NSColor.h>
#import <AppKit/NSPopUpButton.h>

#import <Onx/NextStep/NSAction.h>

@implementation OnX_NSAction
- (id)init {
  if(self = [super init]) {
  }
  return self;
}
- (void)dealloc {  
  [super dealloc];
}
- (void)execute {
}
@end

@implementation OnX_NSButton
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSToggleButton
- (id)initWithFrame:(NSRect)aRect {
  if(self = [super initWithFrame:aRect]) {
    [self setButtonType:NSToggleButton];
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)action:(id)aSender {
  [super action:aSender];
}
- (void)setTitle:(NSString*)aTitle {
  [self setAlternateTitle:aTitle];
  NSString* nss = [[NSString alloc] initWithString:@"Not "];
  NSString* nss_not = [nss stringByAppendingString:aTitle];
  [nss release];
  [super setTitle:nss_not];
}
@end

@implementation OnX_NSMenuItem
- (id)initWithTitle:(NSString*)aString 
             action:(SEL)aSelector 
      keyEquivalent:(NSString*)aCharCode {
  fActivateActions = 0;
  if(self = [super initWithTitle:aString action:aSelector keyEquivalent:aCharCode]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSToggleMenuItem
- (id)initWithTitle:(NSString*)aString 
             action:(SEL)aSelector 
      keyEquivalent:(NSString*)aCharCode {
  if(self = [super initWithTitle:aString action:aSelector keyEquivalent:aCharCode]) {
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)action:(id)aSender {
  if([self state]==NSOffState) 
    [self setState:NSOnState];
  else if([self state]==NSOnState) 
    [self setState:NSOffState];
  [super action:aSender];
}
@end

@implementation OnX_NSOptionMenu
- (id)initWithFrame:(NSRect)aRect {
  fLabel = 0;
  fPopup = 0;
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    //unsigned int autoResizeMask = NSViewWidthSizable | NSViewHeightSizable;

    fLabel = [[NSButton alloc] initWithFrame:aRect];
    [fLabel setBordered:NO];
    [fLabel setTitle:@""];
    //[fLabel setAutoresizingMask:autoResizeMask];
    [self addSubview:fLabel];

    fPopup = [[NSPopUpButton alloc] initWithFrame:aRect];
    //[fPopup setAutoresizingMask:autoResizeMask];
    [fPopup setTarget:self];
    [fPopup setAction:@selector(action:)];
    [self addSubview:fPopup];

    //[self resizeSubviewsWithOldSize:aRect.size];

    fActivateActions = [[NSMutableArray alloc] init];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc]; //FIXME : will delete fLabel, fPopup ?
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
- (void)resizeSubviewsWithOldSize:(NSSize)aOldSize {
  NSRect rect = [self frame];
  // Second view, share the area with the first :
  NSView* firstView = [[self subviews] objectAtIndex:0]; 
  NSView* secondView = [[self subviews] objectAtIndex:1]; 
  //expand SECOND :
  //float label_size = rect.size.width / 2; //BOTH
  float label_size = [[fLabel title] length] * 8; //cooking.
  NSRect vrect;
  // First view :
  vrect.origin.x = 0;
  vrect.origin.y = 0;
  vrect.size.width = label_size;
  vrect.size.height = rect.size.height;
  [firstView setFrame:vrect];
  // Second view :
  vrect.origin.x = label_size;
  vrect.origin.y = 0;
  vrect.size.width = rect.size.width - label_size;
  vrect.size.height = rect.size.height;
  [secondView setFrame:vrect];
}
- (NSString*)label {return [fLabel title];}
- (void)setLabel:(NSString*)aLabel {[fLabel setTitle:aLabel];}
- (NSString*)value {
  NSMenuItem* menuItem = (NSMenuItem*)[fPopup selectedItem];
  //if(menuItem==nil) return nil;
  return [menuItem title];
}
- (void)setValue:(NSString*)aValue {[fPopup selectItemWithTitle:aValue];}
- (void)addItem:(NSString*)aValue {[fPopup addItemWithTitle:aValue];}
- (void)removeItems {[fPopup removeAllItems];}
@end

@implementation OnX_NSEntry
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSCommandLine
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  fCompleteActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    fCompleteActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [fCompleteActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)addCompleteAction:(OnX_NSAction*)aAction {
  [fCompleteActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
  [self setStringValue:@""];
}
//FIXME : complete.
@end

@implementation OnX_NSSecureTextField
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSComboBox
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSToolbarItem
- (id)initWithItemIdentifier:(NSString*)aData {
  fActivateActions = 0;
  if(self = [super initWithItemIdentifier:aData]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSToolbar
- (id)initWithIdentifier:(NSString*)aData {
  fItems = 0;
  if(self = [super initWithIdentifier:aData]) {
    fItems = [[NSMutableArray alloc] init];
    [self setDelegate:self];
  }
  return self;
}
- (void)dealloc {
  [fItems release];
  [super dealloc];
}
- (void)addToolbarItem:(NSToolbarItem*)aItem {
  [fItems addObject:aItem];
}
//NSToolbarDelegate
- (NSArray*)toolbarAllowedItemIdentifiers:(NSToolbar*)aToolBar {
  return [NSArray arrayWithObjects:nil];
}
- (NSArray*)toolbarDefaultItemIdentifiers:(NSToolbar*)aToolBar {
  return [NSArray arrayWithObjects:nil];
}
- (NSArray*)toolbarSelectableItemIdentifiers:(NSToolbar*)aToolBar {
  return [NSArray arrayWithObjects:nil];
}
- (NSToolbarItem*) toolbar:(NSToolbar*)aToolBar
      itemForItemIdentifier:(NSString*)aItemIdentifier
  willBeInsertedIntoToolbar:(BOOL)aFlag
{
  int number = [fItems count];
  // NSToolbarItem factory :
  int index;
  for(index=0;index<number;index++) {
    NSToolbarItem* toolbarItem = [fItems objectAtIndex:index];
    NSString* identifier = [toolbarItem itemIdentifier];
    if ([aItemIdentifier isEqualToString:identifier]) {
      return toolbarItem;
    }
  }  
  return nil;
}
@end

// Expansion and split position policy :
// if expand FIRST
//   Second child height must not change.
//   If set (not -1), fSizeOfFixed is the height of the second child
//   else the current second child height is used.
// if expand SECOND (Default)
//   First child height must not change.
//   If set (not -1), fSizeOfFixed is the height of the first child
//   else the current first child height is used.
// if expand BOTH
//   The container height is equally divided between the two children.

@implementation OnX_NSVBox
- (id)initWithFrame:(NSRect)aRect {
  fExpand = OnX_SECOND;
  fSizeOfFixed = -1;
  if(self = [super initWithFrame:aRect]) {
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)setExpand:(OnX_Expand)aExpand {fExpand = aExpand;}
- (void)setSizeOfFixed:(int)aSize {fSizeOfFixed = aSize;}
- (void)addView:(NSView*)aView {
  [self addSubview:aView];
  NSRect rect = [self frame];
  [self resizeSubviewsWithOldSize:rect.size];
}
- (void)resizeSubviewsWithOldSize:(NSSize)aOldSize {
  NSRect rect = [self frame];
  if([[self subviews] count]==1) {
    NSView* firstView = [[self subviews] objectAtIndex:0]; 
    // One entry, map to whole area :
    [firstView setFrame:rect];
    return;
  }
  if([[self subviews] count]==2) {

  // Second view, share the area with the first :
  NSView* firstView = [[self subviews] objectAtIndex:0]; 
  NSView* secondView = [[self subviews] objectAtIndex:1]; 
  if(fExpand==OnX_SECOND) { //Default
    // First view height must not change.
    // If set, fSizeOfFixed is the size of the first view.
    // First view :
    NSRect vrect;
    vrect.origin.x = 0;
    vrect.size.width = rect.size.width;
    double firstHeight = 
      fSizeOfFixed>=0 ? fSizeOfFixed : [firstView frame].size.height;
    vrect.origin.y = rect.size.height - firstHeight;
    vrect.size.height = firstHeight;
    [firstView setFrame:vrect];
    // Second view :
    vrect.origin.x = 0;
    vrect.size.width = rect.size.width;
    vrect.origin.y = 0;
    vrect.size.height = rect.size.height - firstHeight;
    [secondView setFrame:vrect];
  } else if(fExpand==OnX_FIRST) {
    // Second child height must not change.
    // If set, fSizeOfFixed is the size of the second child.
    // First view :
    NSRect vrect;
    vrect.origin.x = 0;
    vrect.size.width = rect.size.width;
    double secondHeight = 
      fSizeOfFixed>=0 ? fSizeOfFixed : [secondView frame].size.height;
    vrect.origin.y = secondHeight;
    vrect.size.height = rect.size.height - secondHeight;
    [firstView setFrame:vrect];
    // Second view :
    vrect.origin.x = 0;
    vrect.size.width = rect.size.width;
    vrect.origin.y = 0;
    vrect.size.height = secondHeight;
    [secondView setFrame:vrect];
  } else if(fExpand==OnX_BOTH) {
    // First view :
    float width = rect.size.width;
    float height = rect.size.height / 2;
    NSRect vrect;
    vrect.origin.x = 0;
    vrect.origin.y = height; 
    vrect.size.width = width;
    vrect.size.height = height;
    [firstView setFrame:vrect];
    // Second view :
    width = rect.size.width;
    height = rect.size.height / 2;
    vrect.origin.x = 0;
    vrect.origin.y = 0;
    vrect.size.width = width;
    vrect.size.height = height;
    [secondView setFrame:vrect];
  }

  }
//  unsigned int mask = 
//      NSViewWidthSizable | NSViewHeightSizable; // What is changing :
//  mask = NSViewWidthSizable | NSViewMinYMargin; // What is changing :
//    [aView setAutoresizingMask:mask];
}

@end

// Expansion and split position policy :
// if expand FIRST
//   Second child width must not change.
//   If set (not -1), fSizeOfFixed is the width of the second child
//   else the current second child width is used.
// if expand SECOND (Default)
//   First child width must not change.
//   If set (not -1), fSizeOfFixed is the width of the first child
//   else the current first child width is used.
// if expand BOTH
//   The container width is equally divided between the two children.

@implementation OnX_NSHBox
- (id)initWithFrame:(NSRect)aRect {
  fExpand = OnX_SECOND;
  fSizeOfFixed = -1;
  if(self = [super initWithFrame:aRect]) {
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)setExpand:(OnX_Expand)aExpand {fExpand = aExpand;}
- (void)setSizeOfFixed:(int)aSize {fSizeOfFixed = aSize;}
- (void)addView:(NSView*)aView {
  [self addSubview:aView];
  NSRect rect = [self frame];
  [self resizeSubviewsWithOldSize:rect.size];
}
- (void)resizeSubviewsWithOldSize:(NSSize)aOldSize {
  NSRect rect = [self frame];
  if([[self subviews] count]==1) {
    NSView* firstView = [[self subviews] objectAtIndex:0]; 
    // One entry, map to whole area :
    [firstView setFrame:rect];
    return;
  }

  if([[self subviews] count]==2) {

  // Second view, share the area with the first :
  NSView* firstView = [[self subviews] objectAtIndex:0]; 
  NSView* secondView = [[self subviews] objectAtIndex:1]; 
  if(fExpand==OnX_SECOND) {
    NSRect vrect;
    // First view :
    double firstWidth = 
      fSizeOfFixed>=0 ? fSizeOfFixed : [firstView frame].size.width;
    vrect.origin.x = 0;
    vrect.size.width = firstWidth;
    vrect.origin.y = 0;
    vrect.size.height = rect.size.height;
    [firstView setFrame:vrect];
    // Second view :
    vrect.origin.x = firstWidth;
    vrect.size.width = rect.size.width - firstWidth;
    vrect.origin.y = 0;
    vrect.size.height = rect.size.height;
    [secondView setFrame:vrect];
  } else if(fExpand==OnX_FIRST) {
    NSRect vrect;
    // First view :
    double secondWidth = 
      fSizeOfFixed>=0 ? fSizeOfFixed : [secondView frame].size.width;
    vrect.origin.x = 0;
    vrect.size.width = rect.size.width - secondWidth;
    vrect.origin.y = 0;
    vrect.size.height = rect.size.height;
    [firstView setFrame:vrect];
    // Second view :
    vrect.origin.x = rect.size.width - secondWidth;
    vrect.size.width = secondWidth;
    vrect.origin.y = 0;
    vrect.size.height = rect.size.height;
    [secondView setFrame:vrect];
  } else if(fExpand==OnX_BOTH) {
    NSRect vrect;
    // First view :
    float width = rect.size.width / 2;
    float height = rect.size.height;
    vrect.origin.x = 0;
    vrect.origin.y = 0;
    vrect.size.width = width;
    vrect.size.height = height;
    [firstView setFrame:vrect];
    // Second view :
    width = rect.size.width / 2;
    height = rect.size.height;
    vrect.origin.x = width;
    vrect.origin.y = 0;
    vrect.size.width = width;
    vrect.size.height = height;
    [secondView setFrame:vrect];
  }

  }
}
@end

@implementation OnX_NSVContainer
- (id)init {
  if(self = [super init]) {
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)addView:(NSView*)aView {
  [self addSubview:aView];
  NSRect rect = [self frame];
  // Share area between subviews :
  int number = [[self subviews] count];
  float height = rect.size.height / number;
  int index;
  for(index=0;index<number;index++) {
    NSView* view = [[self subviews] objectAtIndex:index];
    NSRect vrect;
    vrect.origin.x = 0;
    vrect.origin.y = (number - 1 - index) * height;
    vrect.size.width = rect.size.width;
    vrect.size.height = height;
    [view setFrame:vrect];
    unsigned int mask = 0;
    if(index==0) {
      mask = NSViewWidthSizable | NSViewHeightSizable | 
             NSViewMinYMargin;
    } else if(index==number-1) {
      mask = NSViewWidthSizable | NSViewHeightSizable | 
             NSViewMaxYMargin;
    } else {
      mask = NSViewWidthSizable | NSViewHeightSizable | 
             NSViewMinYMargin | NSViewMaxYMargin;
    }
    [view setAutoresizingMask:mask];
  }
}
@end

@implementation OnX_NSHContainer
- (id)init {
  if(self = [super init]) {
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)addView:(NSView*)aView {
  [self addSubview:aView];
  NSRect rect = [self frame];
  // Share area between subviews :
  int number = [[self subviews] count];
  float width = rect.size.width / number;
  int index;
  for(index=0;index<number;index++) {
    NSView* view = [[self subviews] objectAtIndex:index];
    NSRect vrect;
    vrect.origin.x = index * width;
    vrect.origin.y = 0;
    vrect.size.width = width;
    vrect.size.height = rect.size.height;
    [view setFrame:vrect];
    unsigned int mask = 0;
    if(index==0) {
      mask = NSViewWidthSizable | NSViewHeightSizable | 
             NSViewMaxXMargin;
    } else if(index==number-1) {
      mask = NSViewWidthSizable | NSViewHeightSizable | 
             NSViewMinXMargin;
    } else {
      mask = NSViewWidthSizable | NSViewHeightSizable | 
             NSViewMaxXMargin | NSViewMinXMargin;
    }
    [view setAutoresizingMask:mask];
  }
}
@end

@implementation OnX_NSVPaned
- (id)initWithFrame:(NSRect)aRect {
  fSplitPosition = -1;
  if(self = [super initWithFrame:aRect]) {
    //[self setIsPaneSplitter:YES];
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)setSplitPosition:(int)aPosition {
  fSplitPosition = aPosition;
}
- (void)addView:(NSView*)aView {
  [self addSubview:aView];
  NSRect rect = [self frame];
  if([[self subviews] count]==1) {
    // First entry, map to whole area :
    [aView setFrame:rect];
    return;
  }
  // Second view, share the area with the first :
  NSView* firstView = [[self subviews] objectAtIndex:0]; 
  NSView* secondView = [[self subviews] objectAtIndex:1]; 
  NSRect vrect;
  vrect.origin.x = 0;
  vrect.size.width = rect.size.width;
  double firstHeight = 
    fSplitPosition>=0 ? fSplitPosition : [firstView frame].size.height;
  vrect.origin.y = rect.size.height - firstHeight;
  vrect.size.height = firstHeight;
  [firstView setFrame:vrect];
  // Second view :
  vrect.origin.x = 0;
  vrect.size.width = rect.size.width;
  vrect.origin.y = 0;
  vrect.size.height = rect.size.height - firstHeight;
  [secondView setFrame:vrect];
}
@end

@implementation OnX_NSHPaned
- (id)initWithFrame:(NSRect)aRect {
  fSplitPosition = -1;
  if(self = [super initWithFrame:aRect]) {
    [self setVertical:YES];
    //[self setIsPaneSplitter:YES];
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
- (void)setSplitPosition:(int)aPosition {
  fSplitPosition = aPosition;
}
- (void)addView:(NSView*)aView {
  [self addSubview:aView];
  NSRect rect = [self frame];
  if([[self subviews] count]==1) {
    // First entry, map to whole area :
    [aView setFrame:rect];
    return;
  }
  // Second view, share the area with the first :
  NSView* firstView = [[self subviews] objectAtIndex:0]; 
  NSView* secondView = [[self subviews] objectAtIndex:1]; 
  NSRect vrect;
  // First view :
  double firstWidth = 
    fSplitPosition>=0 ? fSplitPosition : [firstView frame].size.width;
  vrect.origin.x = 0;
  vrect.size.width = firstWidth;
  vrect.origin.y = 0;
  vrect.size.height = rect.size.height;
  [firstView setFrame:vrect];
  // Second view :
  vrect.origin.x = firstWidth;
  vrect.size.width = rect.size.width - firstWidth;
  vrect.origin.y = 0;
  vrect.size.height = rect.size.height;
  [secondView setFrame:vrect];
}
@end



@implementation OnX_NSOpenGLArea
- (id)initWithFrame:(NSRect)aRect {
  fPaintActions = 0;
  fEventActions = 0;
  fEventType = NSLeftMouseDown;
  NSOpenGLPixelFormat * pixelFormat;
  if ((pixelFormat = [self createPixelFormat:aRect]) != nil) {
    if (self = [super initWithFrame:aRect pixelFormat:pixelFormat]) {
      // flush buffer only during the vertical retrace of the monitor
      const GLint vals[1] = {1};
      [[self openGLContext] setValues:vals forParameter:NSOpenGLCPSwapInterval];
      [[self openGLContext] makeCurrentContext];
      //other init should be put here.
      fPaintActions = [[NSMutableArray alloc] init];
      fEventActions = [[NSMutableArray alloc] init];
    }
    [pixelFormat release];
  } else {
    NSLog(@"OnX_NSOpenGLArea : can't create a pixel format.");
    //FIXME [[NSNotificationCenter defaultCenter]postNotificationName:SCCouldNotCreateValidPixelFormatNotification object:self];
    [self dealloc];
    self = nil;
  }
  return self;
}
- (void)dealloc {
  [fPaintActions release];
  [fEventActions release];
  [super dealloc];
}
- (void)addPaintAction:(OnX_NSAction*)aAction {
  [fPaintActions addObject:aAction];
}
- (void)addEventAction:(OnX_NSAction*)aAction {
  [fEventActions addObject:aAction];
}
- (NSEventType)eventType {
  return fEventType;
}
- (NSPoint)eventLocation {
  return fEventLocation;
}
- (void)drawRect:(NSRect)aRect {
  [self refresh];
}
- (void)refresh {
  [[self openGLContext] makeCurrentContext];
  int number = [fPaintActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fPaintActions objectAtIndex:index] execute];
  }
  [[self openGLContext] flushBuffer];
}
- (NSOpenGLPixelFormat*)createPixelFormat:(NSRect)aFrame {
  int colorbits = 32; // color depth
  int depthbits = 32; // z buffer resolution
  NSOpenGLPixelFormatAttribute att[16];
  int i = 0;
  att[i++] = NSOpenGLPFADoubleBuffer;
  att[i++] = NSOpenGLPFAAccelerated;
  att[i++] = NSOpenGLPFAAccumSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)32;
  att[i++] = NSOpenGLPFAColorSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)colorbits;
  att[i++] = NSOpenGLPFADepthSize;
  att[i++] = (NSOpenGLPixelFormatAttribute)depthbits;
  att[i++] = NSOpenGLPFAScreenMask;
  att[i++] = (NSOpenGLPixelFormatAttribute)
    CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay);
  att[i] = (NSOpenGLPixelFormatAttribute)0;
  NSOpenGLPixelFormat* pixelFormat = 
    [[NSOpenGLPixelFormat alloc] initWithAttributes:att];
  return pixelFormat;
}
- (void) mouseDown:(NSEvent*)aEvent {
  fEventType = NSLeftMouseDown;
  fEventLocation = [aEvent locationInWindow];
  int number = [fEventActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fEventActions objectAtIndex:index] execute];
  }
  //[[self nextResponder] mouseDown:aEvent];
}
- (void) mouseUp:(NSEvent*)aEvent {
  fEventType = NSLeftMouseUp;
  fEventLocation = [aEvent locationInWindow];
  int number = [fEventActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fEventActions objectAtIndex:index] execute];
  }
  //[[self nextResponder] mouseUp:aEvent];
}
- (void) mouseDragged:(NSEvent*)aEvent {
  fEventType = NSMouseMoved;
  fEventLocation = [aEvent locationInWindow];
  int number = [fEventActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fEventActions objectAtIndex:index] execute];
  }
  //[[self nextResponder] mouseDragged:aEvent];
}
@end

@implementation OnX_NSFileChooser
- (id)init {
  fModeOpen = true;
  fOkActions = 0;
  fCancelActions = 0;
  fFilter = 0;
  fDirectory = 0;
  fFileName = 0;
  if(self = [super init]) {
    fOkActions = [[NSMutableArray alloc] init];
    fCancelActions = [[NSMutableArray alloc] init];
    fFilter = [[NSString alloc] init];
    fDirectory = [[NSString alloc] init];
    //fDirectory = [[NSString alloc] initWithString:@"."];
    fFileName = [[NSString alloc] init];
  }
  return self;
}
- (void)dealloc {
  [fFilter release];
  [fDirectory release];
  [fFileName release];
  [fOkActions release];
  [fCancelActions release];
  [super dealloc];
}
- (void)addOkAction:(OnX_NSAction*)aAction {
  [fOkActions addObject:aAction];
}
- (void)addCancelAction:(OnX_NSAction*)aAction {
  [fCancelActions addObject:aAction];
}
- (void)setModeOpen:(bool)aMode {
  fModeOpen = aMode;
}
- (void)setFilter:(NSString*)aFilter {
  [fFilter release];
  fFilter = [[NSString alloc] initWithString:aFilter];
}
- (void)setDirectory:(NSString*)aDirectory {
  [fDirectory release];
  fDirectory = [[NSString alloc] initWithString:aDirectory];
}
- (void)show {
  if(fModeOpen) {
    //FIXME : handle filter.
    //Found on the web :
    //NSArray* fileTypes = [NSArray arrayWithObject:@"xml"];
    //result = [oPanel runModalForDirectory:NSHomeDirectory() file:nil types:fileTypes];
    //if (result == NSOKButton) {

    NSOpenPanel* panel = [NSOpenPanel openPanel]; //We are not owner.
    [panel retain];
    //NSLog(@"debug : directory %s\n",[fDirectory UTF8String]);
    [panel beginForDirectory:([fDirectory length]?fDirectory:nil)
                        file:nil
                       types:nil
            modelessDelegate:self
              didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:)
                 contextInfo:nil];
  } else { //Save
    NSSavePanel* panel = [NSSavePanel savePanel]; //We are not owner.
    int status = [panel runModal];
    if(status==NSFileHandlingPanelOKButton) {
      [fFileName release];
      fFileName = [[NSString alloc] initWithString:[panel filename]];
      int number = [fOkActions count];
      int index;
      for(index=0;index<number;index++) {
        [(OnX_NSAction*)[fOkActions objectAtIndex:index] execute];
      }
    } else if(status==NSFileHandlingPanelCancelButton) {
      [fFileName release];
      fFileName = [[NSString alloc] init];
      int number = [fCancelActions count];
      int index;
      for(index=0;index<number;index++) {
        [(OnX_NSAction*)[fCancelActions objectAtIndex:index] execute];
      }
    }
  }
}
- (NSString*)fileName {return fFileName;}
- (void)openPanelDidEnd:(NSOpenPanel*)aPanel 
             returnCode:(int)aRC 
            contextInfo:(void*)aContext {
  if (aRC == NSOKButton) {
    [fFileName release];
    fFileName = [[NSString alloc] initWithString:[aPanel filename]];
    //printf("debug : file open \"%s\"\n",fFileName->c_str());
    int number = [fOkActions count];
    int index;
    for(index=0;index<number;index++) {
      [(OnX_NSAction*)[fOkActions objectAtIndex:index] execute];
    }
  } else if(aRC == NSCancelButton) {
    [fFileName release];
    fFileName = [[NSString alloc] init];
    int number = [fCancelActions count];
    int index;
    for(index=0;index<number;index++) {
      [(OnX_NSAction*)[fCancelActions objectAtIndex:index] execute];
    }
  }
  [aPanel release];
}
@end

@implementation OnX_NSColorChooser
- (id)init {
  fColorPanel = 0;
  fOkActions = 0;
  fColor = 0;
  fVBox = fBox = fOk = fApply = fCancel = 0;
  f_panel_view = 0;
  if(self = [super init]) {
    fColorPanel = [NSColorPanel sharedColorPanel]; //We are not owner.

    NSRect rect = [fColorPanel frame];

    f_panel_view = [fColorPanel contentView];
    [f_panel_view retain];
    [fColorPanel setContentView:0];

    OnX_NSVBox* vbox = [[OnX_NSVBox alloc] initWithFrame:rect];
    [vbox setSizeOfFixed:35];
    [vbox setExpand:OnX_FIRST];
    [vbox addView:f_panel_view];
    fVBox = vbox;

    unsigned int autoResizeMask = NSViewWidthSizable | NSViewHeightSizable;

    OnX_NSHContainer* box = [[OnX_NSHContainer alloc] initWithFrame:rect];
    [box setAutoresizingMask:autoResizeMask];
    rect = [box frame];
    fBox = box;
    [vbox addView:box];

    // Ok button :
   {OnX_NSButton* button = [[OnX_NSButton alloc] initWithFrame:rect];
    [button setAutoresizingMask:autoResizeMask];
    [button setTarget:self];
    [button setAction:@selector(ok_action:)];
    NSString* nss = [[NSString alloc] initWithString:@"Ok"];
    [button setTitle:nss];
    [nss release];
    [box addView:button];
    fOk = button;}

    // Apply button :
   {OnX_NSButton* button = [[OnX_NSButton alloc] initWithFrame:rect];
    [button setAutoresizingMask:autoResizeMask];
    [button setTarget:self];
    [button setAction:@selector(apply_action:)];
    NSString* nss = [[NSString alloc] initWithString:@"Apply"];
    [button setTitle:nss];
    [nss release];
    [box addView:button];
    fApply = button;}

   // Cancel button :
   {OnX_NSButton* button = [[OnX_NSButton alloc] initWithFrame:rect];
    [button setAutoresizingMask:autoResizeMask];
    [button setTarget:self];
    [button setAction:@selector(cancel_action:)];
    NSString* nss = [[NSString alloc] initWithString:@"Cancel"];
    [button setTitle:nss];
    [nss release];
    [box addView:button];
    fCancel = button;}

    [fColorPanel setContentView:vbox];
    [f_panel_view release];

    fOkActions = [[NSMutableArray alloc] init];
    fColor = [[NSString alloc] init];
  }
  return self;
}
- (void)dealloc {
  [f_panel_view retain];
  [f_panel_view removeFromSuperview];
  [fColorPanel setContentView:f_panel_view];
  [f_panel_view release];

  [fVBox release];
  [fBox release];
  [fOk release];
  [fApply release];
  [fCancel release];

  [fColor release];
  [fOkActions release];
  [super dealloc];
}
- (void)addOkAction:(OnX_NSAction*)aAction {
  [fOkActions addObject:aAction];
}
- (void)show:(id)aSender {
  if(fColorPanel) {
    [fColorPanel makeKeyAndOrderFront:aSender];
  }
}
- (void)hide {
  if(fColorPanel) {
    //[fColorPanel close];
    [fColorPanel orderOut:nil];
  }
}
- (NSString*)choosenColor {return fColor;}
- (void)ok_action:(id)aSender {
  [self apply_action:aSender];
  [self cancel_action:aSender];
}
- (void)apply_action:(id)aSender {
  NSColor* color = [fColorPanel color];
  float r = [color redComponent];
  float g = [color greenComponent];
  float b = [color blueComponent];
  [fColor release];
  fColor = [[NSString alloc] initWithFormat:@"%g %g %g",r,g,b];  
  int number = [fOkActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fOkActions objectAtIndex:index] execute];
  }
}
- (void)cancel_action:(id)aSender {
  [self hide];
}
@end

@implementation OnX_NSTreeItem
- (id)initWithParent:(OnX_NSTreeItem*)aParent {  
  fParent = 0;
  fChildren = 0;
  fLabel = 0;
  fExpanded = false;
  if (self = [super init]) {
    fParent = aParent;
    fChildren = [[NSMutableArray alloc] init];
    fLabel = [[NSString alloc] init];
  }
  return self;
}
- (void)dealloc {
  [fChildren release];
  [fLabel release];
  [super dealloc];
}
- (void)addObject:(OnX_NSTreeItem*)aItem {
  [fChildren addObject:aItem];
}
- (int)numberOfChildren {
  return [fChildren count];
}
- (OnX_NSTreeItem*)childAtIndex:(int)aIndex {
  return [fChildren objectAtIndex:aIndex];
}

- (void)setExpanded:(bool)aValue {fExpanded = aValue;}
- (bool)expanded {return fExpanded;}

- (NSString*)path {
  NSString* path = [[NSString alloc] init];
  OnX_NSTreeItem* item = self;
  while(item) {
    NSString* ss = item->fLabel?
                    [[NSString alloc] initWithString:item->fLabel] :
                    [[NSString alloc] init];
    NSString* p0 = [[NSString alloc] initWithString:@"\n"];
    NSString* p1 = [p0 stringByAppendingString:ss];
    [p0 release];
    [ss release];
    NSString* p2 = [p1 stringByAppendingString:path];
    //[p1 release]; //not owner.
    [path release];
    path = [[NSString alloc] initWithString:p2];
    //[p2 release]; //not owner.

    item = item->fParent;
  }
  // Remove the leading \n (there are two because the root as an empty label)
  if([path length]>=2) {
    NSString* p = [path substringFromIndex:2];
    [path release];
    path = [[NSString alloc] initWithString:p];
    //[p release]; //not owner.
  }
  return path; //released by the caller.
}
- (NSString*)label {return fLabel;}
- (void)setLabel:(NSString*)aLabel {
  if(aLabel!=fLabel) {
    [fLabel release];
    fLabel = [[NSString alloc] initWithString:aLabel];
  }
}
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
@interface OnX_NSTree : NSOutlineView {
#else
@interface OnX_NSTree : NSOutlineView <NSOutlineViewDelegate,NSOutlineViewDataSource> {
#endif
  OnX_NSTreeItem* fRootItem;
  NSMutableArray* fSelectActions;
  NSMutableArray* fSelectBranchActions;
  NSMutableArray* fOpenActions;
  NSMutableArray* fCloseActions;
  NSString* fSelectPath;
  NSString* fOpenPath;
  NSString* fClosePath;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)setRootItem:(OnX_NSTreeItem*)aItem;
- (void)addSelectAction:(OnX_NSAction*)aAction;
- (void)addSelectBranchAction:(OnX_NSAction*)aAction;
- (void)addOpenAction:(OnX_NSAction*)aAction;
- (void)addCloseAction:(OnX_NSAction*)aAction;
- (NSString*)selectPath;
- (NSString*)openPath;
- (NSString*)closePath;
- (OnX_NSTreeItem*)rootItem;
//- (void)clear;
//private
- (void)expandCollapse:(OnX_NSTreeItem*)aItem;
@end

@implementation OnX_NSTree
- (id)initWithFrame:(NSRect)aRect {
  fRootItem = 0;
  fSelectActions = 0;
  fSelectBranchActions = 0;
  fOpenActions = 0;
  fCloseActions = 0;
  fSelectPath = 0;
  fOpenPath = 0;
  fClosePath = 0;
  if(self = [super initWithFrame:aRect]) {
    [self setDelegate:self];
    NSTableColumn* column = 
      [[NSTableColumn alloc] initWithIdentifier:@"NAME"];
    [column setMinWidth:10000];
    NSTableHeaderCell* cell = [column headerCell];
    [cell setStringValue:@""]; //default text is "Field".
    [self addTableColumn:column];
    [self setOutlineTableColumn:column];
    [column release];

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(outlineViewSelectionDidChange:) name:NSOutlineViewSelectionDidChangeNotification object:self];

    fRootItem = [[OnX_NSTreeItem alloc] initWithParent:nil];

    fSelectActions = [[NSMutableArray alloc] init];
    fSelectBranchActions = [[NSMutableArray alloc] init];
    fOpenActions = [[NSMutableArray alloc] init];
    fCloseActions = [[NSMutableArray alloc] init];
    fSelectPath = [[NSString alloc] init];
    fOpenPath = [[NSString alloc] init];
    fClosePath = [[NSString alloc] init];
  }
  return self;
}
- (void)dealloc {
  [fSelectActions release];
  [fSelectBranchActions release];
  [fOpenActions release];
  [fCloseActions release];
  [fSelectPath release];
  [fOpenPath release];
  [fClosePath release];
  [fRootItem release];
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [super dealloc];
}
- (void)setRootItem:(OnX_NSTreeItem*)aItem {
  if(aItem!=fRootItem) {
    [fRootItem release];
    [aItem retain];
    fRootItem = aItem;
    // Trigger an update :
    [self setDataSource:nil];
    [self setDataSource:self];
  }
  [self expandCollapse:fRootItem];
}
- (void)expandCollapse:(OnX_NSTreeItem*)aItem {
  if([aItem expanded]) [self expandItem:aItem];
  else [self collapseItem:aItem];
  //children :
  int number = [aItem numberOfChildren];
  int index;
  for(index=0;index<number;index++) {
    OnX_NSTreeItem* item = (OnX_NSTreeItem*)[aItem childAtIndex:index];
    [self expandCollapse:item];
  }
}
- (void)addSelectAction:(OnX_NSAction*)aAction {
  [fSelectActions addObject:aAction];
}
- (void)addSelectBranchAction:(OnX_NSAction*)aAction {
  [fSelectBranchActions addObject:aAction];
}
- (void)addOpenAction:(OnX_NSAction*)aAction {
  [fOpenActions addObject:aAction];
}
- (void)addCloseAction:(OnX_NSAction*)aAction {
  [fCloseActions addObject:aAction];
}
- (NSString*)selectPath {return fSelectPath;}
- (NSString*)openPath {return fOpenPath;}
- (NSString*)closePath {return fClosePath;}
- (OnX_NSTreeItem*)rootItem {return fRootItem;}

// NSOutlineViewDelegate
- (void)outlineViewItemDidExpand:(NSNotification*)aNotification {
  //NOTE : this callback is also executed for expanded sub branches !
  OnX_NSTreeItem* item = (OnX_NSTreeItem*)[[aNotification userInfo] objectForKey:@"NSObject"];
  //if([item expanded]) return;
  NSString* label = [item label];
  //NSLog(@"debug : expand \"%s\"\n",[label UTF8String]);
  [item setExpanded:true]; //keep in sync item::fExpanded
  [fOpenPath release];
  fOpenPath = [item path];
  int number = [fOpenActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fOpenActions objectAtIndex:index] execute];
  }
}
- (void)outlineViewItemDidCollapse:(NSNotification*)aNotification {
  //NOTE : this callback is also executed for expanded sub branches !
  OnX_NSTreeItem* item = (OnX_NSTreeItem*)[[aNotification userInfo] objectForKey:@"NSObject"];
  //if(![item expanded]) return;
  NSString* label = [item label];
  //NSLog(@"debug : collapse \"%s\"\n",[label UTF8String]);
  [item setExpanded:false]; //keep in sync item::fExpanded
  [fClosePath release];
  fClosePath = [item path];
  int number = [fCloseActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fCloseActions objectAtIndex:index] execute];
  }
}
// NSOutlineViewDataSource
- (void)outlineViewSelectionDidChange:(NSNotification*)aNotification {
  OnX_NSTree* tree = (OnX_NSTree*)[aNotification object];
  int row = [tree selectedRow];
  if(row==(-1)) { //deselect.
    [fSelectPath release];
    fSelectPath = [[NSString alloc] init];
    return;
  }
  OnX_NSTreeItem* item = (OnX_NSTreeItem*)[tree itemAtRow:row]; 
  [fSelectPath release];
  fSelectPath = [item path];
  if([item numberOfChildren]) { //branch
    int number = [fSelectBranchActions count];
    int index;
    for(index=0;index<number;index++) {
      [(OnX_NSAction*)[fSelectBranchActions objectAtIndex:index] execute];
    }
  } else {
    int number = [fSelectActions count];
    int index;
    for(index=0;index<number;index++) {
      [(OnX_NSAction*)[fSelectActions objectAtIndex:index] execute];
    }
  }
}
- (NSInteger)outlineView:(NSOutlineView*)aView numberOfChildrenOfItem:(id)aItem {
  if(aItem==nil) {
    return [fRootItem numberOfChildren];
  } else {
    return [(OnX_NSTreeItem*)aItem numberOfChildren];
  }
}
- (id)outlineView:(NSOutlineView*)aView child:(NSInteger)aIndex ofItem:(id)aItem {
  if(aItem==nil) {
    return [fRootItem childAtIndex:aIndex];
  } else {
    return [(OnX_NSTreeItem*)aItem childAtIndex:aIndex];
  }
}
- (BOOL)outlineView:(NSOutlineView*)aView isItemExpandable:(id)aItem {
  if(aItem==nil) {
    return YES;
  } else {
    int n = [(OnX_NSTreeItem*)aItem numberOfChildren];
    return (n==0?NO:YES);
  }
}
- (id)outlineView:(NSOutlineView*)aView objectValueForTableColumn:(NSTableColumn*)aColumn byItem:(id)aItem {
  //if ([[aColumn identifier] isEqual: @"NAME"]){
  return [(OnX_NSTreeItem*)aItem label];
}
@end

@implementation OnX_NSScrolledTree
- (id)initWithFrame:(NSRect)aRect {
  fTree = 0;
  if(self = [super initWithFrame:aRect]) {
    [self setHasVerticalScroller:YES];
    fTree = [[OnX_NSTree alloc] initWithFrame:aRect];
    //unsigned int autoResizeMask = NSViewWidthSizable | NSViewHeightSizable;
    //[fTree setAutoresizingMask:autoResizeMask];
    [self setDocumentView:fTree];
  }
  return self;
}
- (void)dealloc {
  [fTree release];
  [super dealloc];
}
- (void)setRootItem:(OnX_NSTreeItem*)aItem {
  [fTree setRootItem:aItem];
}
- (void)addSelectAction:(OnX_NSAction*)aAction {
  [fTree addSelectAction:aAction];
}
- (void)addSelectBranchAction:(OnX_NSAction*)aAction {
  [fTree addSelectBranchAction:aAction];
}
- (void)addOpenAction:(OnX_NSAction*)aAction {
  [fTree addOpenAction:aAction];
}
- (void)addCloseAction:(OnX_NSAction*)aAction {
  [fTree addCloseAction:aAction];
}
- (NSString*)selectPath {return [fTree selectPath];}
- (NSString*)openPath {return [fTree openPath];}
- (NSString*)closePath {return [fTree closePath];}
- (OnX_NSTreeItem*)rootItem { return [fTree rootItem];}
- (int)selectedRow {return [fTree selectedRow];}
- (void)selectRowIndexes:(NSIndexSet*)indexes byExtendingSelection:(BOOL)extend {[fTree selectRowIndexes:indexes byExtendingSelection:extend];}
@end

#if MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5
@interface OnX_NSList : NSTableView {
#else
@interface OnX_NSList : NSTableView <NSTableViewDataSource> {
#endif
  OnX_NSTreeItem* fRootItem;
  NSMutableArray* fSelectActions;
  NSString* fSelection;
}
- (id)initWithFrame:(NSRect)aRect;
- (void)dealloc;
- (void)setRootItem:(OnX_NSTreeItem*)aItem;
- (void)addSelectAction:(OnX_NSAction*)aAction;
- (NSString*)selection;
- (BOOL)setSelection:(NSString*)aSelection;
@end

@implementation OnX_NSList
- (id)initWithFrame:(NSRect)aRect {
  fRootItem = 0;
  fSelectActions = 0;
  fSelection = 0;
  if(self = [super initWithFrame:aRect]) {
    NSTableColumn* column = 
      [[NSTableColumn alloc] initWithIdentifier:@"NAME"];
    [self addTableColumn:column];
    [column release];

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(tableViewSelectionDidChange:) name:NSTableViewSelectionDidChangeNotification object:self];

    fRootItem = [[OnX_NSTreeItem alloc] initWithParent:nil];

    fSelectActions = [[NSMutableArray alloc] init];
    fSelection = [[NSString alloc] init];
  }
  return self;
}
- (void)dealloc {
  [fSelectActions release];
  [fSelection release];
  [fRootItem release];
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [super dealloc];
}
- (void)setRootItem:(OnX_NSTreeItem*)aItem {
  if(aItem!=fRootItem) {
    [fRootItem release];
    [aItem retain];
    fRootItem = aItem;
    // Trigger an update :
    [self setDataSource:nil];
    [self setDataSource:self];
  }
}
- (void)addSelectAction:(OnX_NSAction*)aAction {
  [fSelectActions addObject:aAction];
}
- (NSString*)selection {return fSelection;}
- (BOOL)setSelection:(NSString*)aSelection {
  int number = [fRootItem numberOfChildren];
  int index;
  for(index=0;index<number;index++) {
    OnX_NSTreeItem* item = (OnX_NSTreeItem*)[fRootItem childAtIndex:index];
    NSString* label = [item label];
    if ([label isEqualToString:aSelection]) {
      NSIndexSet* indexes = [[NSIndexSet alloc] initWithIndex:index];
      [self selectRowIndexes:indexes byExtendingSelection:NO];
      [indexes release];
      return YES;
    }
  }
  return NO;
}
// NSTableViewDataSource :
- (NSInteger)numberOfRowsInTableView:(NSTableView*)aTableView {
  return [fRootItem numberOfChildren];
}
- (id)tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)aTableColumn row:(NSInteger)aRow {
  return [[fRootItem childAtIndex:aRow] label];
}
// NSTableViewNotification :
- (void)tableViewSelectionDidChange:(NSNotification*)aNotification {
  [fSelection release];
  OnX_NSList* tree = (OnX_NSList*)[aNotification object];
  int row = [tree selectedRow];
  OnX_NSTreeItem* treeItem = [fRootItem childAtIndex:row];
  fSelection = [treeItem path];
  int number = [fSelectActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fSelectActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSScrolledList
- (id)initWithFrame:(NSRect)aRect {
  fList = 0;
  if(self = [super initWithFrame:aRect]) {
    [self setHasVerticalScroller:YES];
    fList = [[OnX_NSList alloc] initWithFrame:aRect];
    //unsigned int autoResizeMask = NSViewWidthSizable | NSViewHeightSizable;
    //[fList setAutoresizingMask:autoResizeMask];
    [self setDocumentView:fList];
  }
  return self;
}
- (void)dealloc {
  [fList release];
  [super dealloc];
}
- (void)setRootItem:(OnX_NSTreeItem*)aItem {
  [fList setRootItem:aItem];
}
- (void)addSelectAction:(OnX_NSAction*)aAction {
  [fList addSelectAction:aAction];
}
- (NSString*)selection {
  return [fList selection];
}
- (BOOL)setSelection:(NSString*)aSelection {
  return [fList setSelection:aSelection];
}
@end

@implementation OnX_NSScrolledText
- (id)initWithFrame:(NSRect)aRect {
  fText = 0;
  if(self = [super initWithFrame:aRect]) {
    [self setHasVerticalScroller:YES];
    fText = [[NSText alloc] initWithFrame:aRect];
    //unsigned int autoResizeMask = NSViewWidthSizable | NSViewHeightSizable;
    //[fText setAutoresizingMask:autoResizeMask];
    [self setDocumentView:fText];
  }
  return self;
}
- (void)dealloc {
  [fText release];
  [super dealloc];
}
- (NSString*)value {
  return [fText string];
}
- (void)setValue:(NSString*)aValue {
  [fText setString:aValue];
}
- (void)insertText:(NSString*)aValue {
  [fText insertText:aValue];
}
@end

@implementation OnX_NSSlider
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSScroller
- (id)initWithFrame:(NSRect)aRect {
  fActivateActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fActivateActions = [[NSMutableArray alloc] init];
    [self setTarget:self];
    [self setAction:@selector(action:)];
  }
  return self;
}
- (void)dealloc {
  [fActivateActions release];
  [super dealloc];
}
- (void)addActivateAction:(OnX_NSAction*)aAction {
  [fActivateActions addObject:aAction];
}
- (void)action:(id)aSender {
  int number = [fActivateActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fActivateActions objectAtIndex:index] execute];
  }
}
@end

@implementation OnX_NSTabView
- (id)initWithFrame:(NSRect)aRect {
  fSelectActions = 0;
  if(self = [super initWithFrame:aRect]) {
    fSelectActions = [[NSMutableArray alloc] init];
    [self setDelegate:self];
  }
  return self;
}
- (void)dealloc {
  [fSelectActions release];
  [super dealloc];
}
- (void)addSelectAction:(OnX_NSAction*)aAction {
  [fSelectActions addObject:aAction];
}
// NSTabViewDelegate :
- (void)tabView:(NSTabView*)aTabView didSelectTabViewItem:(NSTabViewItem*)aTabViewItem {
  int number = [fSelectActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fSelectActions objectAtIndex:index] execute];
  }
}
@end

NSString* NSOnXMessageNotification = @"NSOnXMessage";

@interface OnX_NSMessage : NSObject {
  id fId;
}
- (id)initWithIdentifier:(id)aId;
- (void)dealloc;
- (id)identifier;
@end
@implementation OnX_NSMessage
- (id)initWithIdentifier:(id)aId {
  fId = 0;
  if(self = [super init]) {
    fId = aId;
  }
  return self;
}
- (void)dealloc {  
  [super dealloc];
}
- (id)identifier {
  return fId;
}
@end

@implementation OnX_NSWindow
- (id)initWithContentRect:(NSRect)aRect mask:(unsigned int)aMask{
  fDestroyActions = 0;
  if(self = [super initWithContentRect:aRect 
                             styleMask:aMask
                               backing:NSBackingStoreBuffered 
		                 defer:NO]) { //we are the owner.
    //[self setShowsResizeIndicator:YES];
    [self setReleasedWhenClosed:YES];
    fDestroyActions = [[NSMutableArray alloc] init];
  }
  return self;
}
- (void)dealloc {
  //NSLog(@"OnX_NSWindow : dealloc\n");
 {int number = [fDestroyActions count];
  int index;
  for(index=0;index<number;index++) {
    [(OnX_NSAction*)[fDestroyActions objectAtIndex:index] execute];
  }}
  [fDestroyActions release];
  [super dealloc];
}
- (void)addDestroyAction:(OnX_NSAction*)aAction {
  [fDestroyActions addObject:aAction];
}
@end

@implementation OnX_NSDialog
- (id)initWithContentRect:(NSRect)aRect {
  unsigned int mask = NSResizableWindowMask | NSTitledWindowMask;
  if(self = [super initWithContentRect:aRect 
                             styleMask:mask
                               backing:NSBackingStoreBuffered 
                                 defer:NO]) {
    //[self setShowsResizeIndicator:YES];
    //[self setReleasedWhenClosed:NO];
  }
  return self;
}
- (void)dealloc {
  [super dealloc];
}
@end

