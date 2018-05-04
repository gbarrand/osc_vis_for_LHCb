// this :
#include <OnX/Xt/XtTools.h>

#include <inlib/srep> //to_xml

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

GC XGetRubberGC(Display* aDisplay,Drawable aDrawable){
  // Have to set correct colors if a_window is not root window.
  XGCValues arggc;
  arggc.function = GXxor;
  arggc.background = XWhitePixelOfScreen(DefaultScreenOfDisplay(aDisplay));
  arggc.foreground = 0xffffffff;
  arggc.plane_mask = 
    XWhitePixelOfScreen(DefaultScreenOfDisplay(aDisplay)) ^ 
    XBlackPixelOfScreen(DefaultScreenOfDisplay(aDisplay));
  arggc.subwindow_mode = IncludeInferiors;
  return XCreateGC(aDisplay,aDrawable,
                   GCFunction|GCForeground|GCBackground|
                   GCPlaneMask|GCSubwindowMode,&arggc);
}

void XDrawRectangle(Display* aDisplay,Drawable aDrawable,GC aGC,const XPoint& aBegin,const XPoint& aEnd){
  XPoint points[5];
  points[0] = aBegin;
  points[1].x = aEnd.x;
  points[1].y = aBegin.y;
  points[2] = aEnd;
  points[3].x = aBegin.x;
  points[3].y = aEnd.y;
  points[4] = aBegin;
  XDrawLines(aDisplay,aDrawable,aGC,points,5,CoordModeOrigin);
}

bool XLoadResourceFile(Display* aDisplay,const std::string& aFile){
  XrmDatabase dbres = XrmGetFileDatabase(aFile.c_str());
  if(!dbres) return false;
  XrmDatabase database = XrmGetDatabase(aDisplay);
  if(database) XrmMergeDatabases(dbres,&database);
  else XrmSetDatabase(aDisplay,dbres);
  return true;
}

void XRingBell(Display* aDisplay,int aNumber,int aLevel) {
  for(int index=0;index<aNumber;index++) XBell(aDisplay,aLevel);
}

bool XPutStringInResourceDatabase(Display* aDisplay,const std::string& aString){
  XrmDatabase dbres = XrmGetStringDatabase(aString.c_str());
  if(dbres==NULL) return false;
 {XrmDatabase database = XrmGetDatabase(aDisplay);
  if(database!=NULL) {
    XrmMergeDatabases(dbres,&database);
  } else {
    XrmSetDatabase(aDisplay,dbres);
  }}
  return true;
}

bool XGetStringResource(Display* aDisplay,const std::string& aName,std::string& aValue){
  XrmDatabase database = XrmGetDatabase(aDisplay);
  if(database==NULL) return false;
  char* type;
  XrmValue value;
  if(XrmGetResource
       (database,aName.c_str(),"String",&type,&value)==False) return false;
  //::printf("%s|%s\n",type,value.addr);
  aValue = std::string(value.addr);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//#include <OnX/Xt/ListTree.h>
extern "C" {
  void ListTreeGetHighlighted(Widget,ListTreeMultiReturnStruct*);
  void ListTreeClearHighlighted(Widget);
}

void ListTreeClear(Widget aWidget){
  ListTreeRefreshOff(aWidget);
  while(1) {
    ListTreeItem* first = ListTreeFirstItem(aWidget);
    if(!first) break;
    ListTreeDelete(aWidget,first);
  }
  ListTreeRefreshOn(aWidget);
}
ListTreeItem* ListTreeFindItem(Widget aWidget,const std::vector<std::string>& aPath){
  unsigned int itemn = aPath.size();
  ListTreeItem* item = ListTreeFirstItem(aWidget);
  for(unsigned int index=0;index<itemn;index++){
    if(!item) return 0;
    bool found = false;
    while(item) {
      if(item->text && (aPath[index]==std::string(item->text))) {
        if(index==itemn-1) return item; //Found.
        found = true;
        item = item->firstchild;
        break;
      }
      item = item->nextsibling;
    }
    if(!found) return 0;
  }
  return 0;
}
std::string ListTreeGetSelection(Widget aWidget,bool& aBranch){
  ListTreeMultiReturnStruct ret;
  ListTreeGetHighlighted(aWidget,&ret);
  if(!ret.count) {
    aBranch = true;
    return "";
  }
  aBranch = ret.items[0]->firstchild?true:false;
  return ListTreeItemGetPath(ret.items[0]);
}
bool ListTreeSetSelection(Widget aWidget,const std::vector<std::string>& aPath){
  ListTreeClearHighlighted(aWidget);
  unsigned int itemn = aPath.size();
  bool found = true;
  ListTreeItem* item = ListTreeFirstItem(aWidget);
  for(unsigned int index=0;index<itemn;index++){
    if(!item) {found = false;break;}
    bool found2 = false;
    while(item) {
      if(aPath[index]==std::string(item->text?item->text:"")) {
        if(index!=itemn-1) {
          ListTreeOpenCloseItem(aWidget,item,True);
        } else {
          ListTreeHighlightItem(aWidget,item);
        }
        found2 = true;
        item = item->firstchild;
        break;
      }
      item = item->nextsibling;
    }
    if(!found2) {found = false;break;}
  }
  if(found) ListTreeRefresh(aWidget);
  else ListTreeClearHighlighted(aWidget);
  return found;
}

std::string ListTreeGetTopItem(Widget aWidget){
  ListTreeItem* item = ListTreeTopItem(aWidget);
  if(!item) return "";
  return ListTreeItemGetPath(item);
}

bool ListTreeSetTopItem(Widget aWidget,const std::vector<std::string>& aPath){
  ListTreeItem* item = ListTreeFindItem(aWidget,aPath);
  if(!item) return false;
  ListTreeSetTopItem(aWidget,item);
  return true;
}

std::string ListTreeItemGetPath(ListTreeItem* aItem){
  std::string path;
  ListTreeItem* item = aItem;
  while(item) {
    char* s = item->text;
    std::string ss = s?s:"";
    std::string opath = path;
    path = "\n";
    path += ss;
    path += opath;
    item = item->parent;
  }
  // Remove the leading \n
  if(path.size()) path = path.substr(1,path.size()-1);
  return path;
}
std::string ListTreeItemGetXML(ListTreeItem* aItem,int depth) {
  //return a XML string representing this tree
  std::string spaceItem = "";
  std::string spaceRoot = "";
  //spaceItem.insert(0,depth*2,' ');
  //spaceRoot.insert(0,(depth-1)*2,' ');
  std::string line;
  do  {
    line += spaceRoot + "<treeItem>";
    line += spaceItem + "<label>";
    line += inlib::to_xml(aItem->text);
    line += "</label>";
    line += spaceItem + "<opened>";
    line += (aItem->open?"true":"false");
    line += "</opened>";
    if (aItem->firstchild) {
      line += ListTreeItemGetXML(aItem->firstchild,depth+1);
    }    
    line += spaceRoot + "</treeItem>";
    aItem = aItem->nextsibling;
  }
  while (aItem);
  return line;
}
//////////////////////////////////////////////////////////////////////////////
//// Motif ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool XmGetString(Widget aWidget,const std::string& aName,std::string& aValue){
  XmString value = 0;
  Arg args[1];
  XtSetArg(args[0],(char*)aName.c_str(),&value);
  XtGetValues(aWidget,args,1);
  if(!value) {aValue = "";return false;}
  return XmConvertString(value,aValue);
}
void XmSetString(Widget aWidget,const std::string& aName,const std::string& aValue){
  XmString cps = XmStringLtoRCreate((char*)aValue.c_str(),
                                    XmSTRING_DEFAULT_CHARSET);
  Arg args[1];
  XtSetArg(args[0],(char*)aName.c_str(),cps);
  XtSetValues(aWidget,args,1);
  XmStringFree(cps);
}

bool XmGetItems(Widget aWidget,const std::string& aResCount,const std::string& aResItems,std::vector<std::string>& aValue) {
  aValue.clear();
  int number;
  XmStringTable table = NULL;
  Arg args[2];
  XtSetArg(args[0],(char*)aResCount.c_str(),&number);
  XtSetArg(args[1],(char*)aResItems.c_str(),&table);
  XtGetValues(aWidget,args,2);
  if(table==NULL) return false;
  for(int index=0;index<number;index++) { 
    std::string s;
    if(!XmConvertString(table[index],s)) {
      aValue.clear();
      return false;
    }        
    aValue.push_back(s);
  }
  return true;
}
bool XmSetLabelPixmap(Widget aWidget,const std::string& aFileName){
  Screen* screen = XDefaultScreenOfDisplay(XtDisplay(aWidget));
  Pixel fore = XBlackPixelOfScreen(screen);
  Pixel back = XWhitePixelOfScreen(screen);
  Pixmap pixmap = XmGetPixmap(screen,(char*)aFileName.c_str(),fore,back);
  if(pixmap==XmUNSPECIFIED_PIXMAP) return false;
  Arg args[2];
  XtSetArg(args[0],XmNlabelType,XmPIXMAP);
  XtSetArg(args[1],XmNlabelPixmap,pixmap);
  XtSetValues(aWidget,args,2);
  return true;
}
XmString* XmStringTableCreateFromText(const std::vector<std::string>& aLines,int& aNumber){
  aNumber = 0;
  XmString* table = new XmString[aLines.size()+1];
  for(unsigned int index=0;index<aLines.size();index++) { 
    XmString xmstring = 
      XmStringLtoRCreate((char*)aLines[index].c_str(),
                         XmSTRING_DEFAULT_CHARSET);
    if(xmstring!=NULL) {
      table[aNumber] = xmstring;
      aNumber++;
    }
  }
  table[aNumber] = NULL;
  return table;
}
void XmStringTableDelete(XmString* aTable) {
  int count = 0;
  while(aTable[count]!=NULL) {
    XmStringFree(aTable[count]);
    count++;
  }
  delete [] aTable;
}
bool XmConvertString(XmString aXmString,std::string& aValue){
  int aNumber = 0; 
  aValue = "";
  XmStringContext context;
  XmStringInitContext(&context,aXmString);
  int icount = 0;
  bool status = false;
  ::Boolean Done = False;
  while(Done==False) {  
    char* text = 0;
    XmStringCharSet charset = 0;
    XmStringDirection direct;
    ::Boolean sep;
    if(XmStringGetNextSegment(context,&text,&charset,&direct,&sep)==True) {
      XtFree(charset);
      if(sep==True) Done = True;
      if(icount==aNumber) { 
        aValue = std::string(text?text:"");
        XtFree(text);
        status = true;
        break;
      }
      icount++;
      XtFree(text);
    } else {
      Done = True;
    }
  }
  XmStringFreeContext(context);
  return status;
}

#include <Xm/DialogS.h>
bool XmShow(Widget aWidget) {
  Widget shell = XWidgetGetShell(aWidget);
  if(!shell) return false;
  if(XmIsDialogShell(shell)) {
    Widget child = XWidgetGetFirstChild(shell);
    if(!child) return false;
    XtManageChild(child);
  } else {
    XtRealizeWidget(shell);
    XtMapWidget(shell);
    // Raise window :
    if(XtIsWidget(shell) && XtIsRealized(shell) ) {
      Display* display = XtDisplay(shell);
      Atom atom = XInternAtom(display,"WM_DELETE_WINDOW",False);
      XSetWMProtocols(display,XtWindow(shell),&atom,1);
      XRaiseWindow(display,XtWindow(shell));
    }
  }
  return true;
}

bool XmHide(Widget aWidget) {
  Widget shell = XWidgetGetShell(aWidget);
  if(!shell) return false;
  if(XmIsDialogShell(shell)) {
    Widget child = XWidgetGetFirstChild(shell);
    if(!child) return false;
    XtUnmanageChild(child);
  } else {
    XtUnmapWidget(shell);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//// Widget specific /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Xm/Scale.h>
void XmScaleGetMinMax(Widget aWidget,int& aMin,int& aMax){
  int mn,mx;
  Arg args[2];
  XtSetArg(args[0],XmNminimum,&mn);
  XtSetArg(args[1],XmNmaximum,&mx);
  XtGetValues(aWidget,args,2);
  aMin = mn;
  aMax = mx;
}

#include <Xm/ScrollBar.h>
void XmScrollBarGetMinMax(Widget aWidget,int& aMin,int& aMax){
  int mn,mx,sz;
  Arg args[3];
  XtSetArg(args[0],XmNminimum,&mn);
  XtSetArg(args[1],XmNmaximum,&mx);
  XtSetArg(args[2],XmNsliderSize,&sz);
  XtGetValues(aWidget,args,3);
  aMin = mn;
  aMax = mx-sz;
}

#include <Xm/Text.h>
void XmTextAppendString(Widget aWidget,const std::string& aString){
  XmTextPosition  lastpos = XmTextGetLastPosition(aWidget);
  XmTextReplace(aWidget,lastpos,lastpos,(char*)aString.c_str());
  XmTextSetInsertionPosition(aWidget,XmTextGetLastPosition(aWidget));
}

void XmTextSetNumberOfRowsAndColumns(Widget aWidget,int aRows,int aCols){
  Arg args[2];
  XtSetArg(args[0],XmNrows,aRows);
  XtSetArg(args[1],XmNcolumns,aCols);
  XtSetValues(aWidget,args,2);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <Xm/ComboBox.h>

void XmComboBoxSetItems(Widget aWidget,const std::vector<std::string>& aItems){
  // Clear :
  int number;
  Arg args[1];
  XtSetArg(args[0],XmNitemCount,&number);
  XtGetValues(aWidget,args,1);
  for(int i=0;i<number;i++) XmComboBoxDeletePos(aWidget,0);
  // Set items :
  std::vector<std::string>::const_iterator it;
  unsigned int index = 0;
  for(it=aItems.begin();it!=aItems.end();++it,index++) {
    XmString cps = XmStringLtoRCreate((char*)(*it).c_str(),
                                      XmSTRING_DEFAULT_CHARSET);
    XmComboBoxAddItem(aWidget,cps,index,FALSE);
    XmStringFree(cps);
  }
}
void XmComboBoxSetValue(Widget aWidget,const std::string& aDefault){
  XmSetString(aWidget,XmNselectedItem,aDefault);
}
bool XmComboBoxGetValue(Widget aWidget,std::string& aValue) {
  return XmGetString(aWidget,XmNselectedItem,aValue);
}
bool XmComboBoxGetItems(Widget aWidget,std::vector<std::string>& aValue) {
  return XmGetItems(aWidget,XmNitemCount,XmNitems,aValue);  
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool XmPanedIsVertical(Widget aWidget){
  unsigned char value = XmVERTICAL;
  Arg args[1];
  XtSetArg(args[0],XmNorientation,&value);
  XtGetValues(aWidget,args,1);
  return (value==XmVERTICAL?true:false);
}

bool XmPanedSetSplitPosition(Widget aWidget,int aPosition){
  // aPosition is in middle of "spacing".
  // It assumes that we have only two "user" children.
  // 0:first user child
  // 1:second user child
  // 2:first XmSash (not display)
  // 3:first XmSeparatorGadget (displayed)
  // 4:second XmSash (displayed)
  // 5:second XmSeparatorGadget (displayed)

  // FIXME : LG : it does not work at creation time.

  // FIXME : GB : in case of two user children, when applied, 
  //         the sash "does not follow" in case of a 
  //         shortening of the shell.

  int childNumber = XWidgetGetChildrenNumber(aWidget);
  if(childNumber==3) {
    Arg args[1];
    Widget first = XWidgetGetChild(aWidget,0);
    XtSetArg(args[0],XmNallowResize,True);
    XtSetValues(first,args,1);

  } else if(childNumber==6) {

    Arg args[2];
  
    Widget first = XWidgetGetChild(aWidget,0);
    XtSetArg(args[0],XmNallowResize,True);
    XtSetValues(first,args,1);
  
    Widget second = XWidgetGetChild(aWidget,1);
    XtSetArg(args[0],XmNallowResize,True);
    XtSetValues(second,args,1);
  
    Dimension margin = 0;
    Dimension spacing = 0;
    if (XmPanedIsVertical(aWidget)) {
      XtSetArg(args[0],XmNmarginHeight,&margin);
      XtSetArg(args[1],XmNspacing,&spacing);
      XtGetValues(aWidget,args,2);
  
      int size = aPosition - margin - spacing/2;
      if(size<0) return false;
      
      XtSetArg(args[0],XmNskipAdjust,False);
      XtSetValues(first,args,1);
      XtSetArg(args[0],XmNpaneMaximum,(Dimension)size);
      XtSetArg(args[1],XmNpaneMinimum,(Dimension)size);
      XtSetValues(first,args,2);
      XtSetArg(args[0],XmNskipAdjust,True);
      XtSetValues(first,args,1);
      XtSetArg(args[0],XmNpaneMaximum,1000);
      XtSetArg(args[1],XmNpaneMinimum,1);
      XtSetValues(first,args,2);
    } else {
      XtSetArg(args[0],XmNmarginWidth,&margin);
      XtSetArg(args[1],XmNspacing,&spacing);
      XtGetValues(aWidget,args,2);
  
      int size = aPosition - margin - spacing/2;
      if(size<0) return false;
  
      XtSetArg(args[0],XmNskipAdjust,False);
      XtSetValues(first,args,1);
      XtSetArg(args[0],XmNpaneMaximum,(Dimension)size);
      XtSetArg(args[1],XmNpaneMinimum,(Dimension)size);
      XtSetValues(first,args,2);
      XtSetArg(args[0],XmNskipAdjust,True);
      XtSetValues(first,args,1);
      XtSetArg(args[0],XmNpaneMaximum,1000);
      XtSetArg(args[1],XmNpaneMinimum,1);
      XtSetValues(first,args,2);
    }
  } else {
    return false;
  }
  return true;
}

int XmPanedGetNumberOfUserChildren(Widget aWidget){
  // XmPanedWindow put "user" children at begin of the composite children list.
  // For each "user" child, it adds at the end a group of 
  // (XmSash,XmSeparatorGadget). Then the number of "user" children
  // is : "total number of chilren" / 3.
  return XWidgetGetChildrenNumber(aWidget)/3; 
  //XmPanedWindowPart* pwp = &(((XmPanedWindowWidget)aWidget)->paned_window);
  //return pwp->num_managed_children; //FIXME ???
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Xm/MwmUtil.h>
void XmShellSetDecorations(Widget aWidget,const std::vector< std::pair<bool,std::string> >& aItems){
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
  
  unsigned long Mwm_decor = 0; //FIXME : ~MWM_DECOR_ALL ?
  unsigned long Mwm_funct = 0; //FIXME : ~MWM_FUNC_ALL ?
  
  if (close_button)  Mwm_funct |= MWM_FUNC_CLOSE;
  
  if (iconify_button) {
    Mwm_funct |= MWM_FUNC_MINIMIZE;
    Mwm_decor |= MWM_DECOR_MINIMIZE;
      }
  if (min_max_button) {
    Mwm_funct |= MWM_FUNC_MAXIMIZE;
    Mwm_decor |= MWM_DECOR_MAXIMIZE;
  }
  if (resize_handle) {
    Mwm_decor |= MWM_DECOR_RESIZEH;
    Mwm_funct |= MWM_FUNC_RESIZE;
  }
  if (border) Mwm_decor |= MWM_DECOR_BORDER;
  
  if (caption) {
    Mwm_funct |= MWM_FUNC_MOVE;
    Mwm_decor |= MWM_DECOR_TITLE;
  }
  if (system_menu) Mwm_decor |= MWM_DECOR_MENU;
  
  Arg args[2];
  XtSetArg(args[0],XmNmwmDecorations,Mwm_decor);
  XtSetArg(args[1],XmNmwmFunctions,Mwm_funct);
  XtSetValues(aWidget,args,2);
}
//////////////////////////////////////////////////////////////////////////////
//// Xt //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Widget XWidgetGetShell(Widget aWidget){
  Widget widget = aWidget;
  while(1) {
    if(!widget) return 0;
    if(XtIsShell(widget)) return widget;
    widget = XtParent(widget);
  }
}
Pixel XWidgetConvertStringToPixel(Widget aWidget,const std::string& aString){
  if(aString.empty()) return 0L;
  if( (aString=="None") || (aString=="none") ) 
    return XWidgetGetPixelResource(aWidget,std::string(XtNbackground));
  XrmValue from,to;
  from.size = aString.size()+1;
  from.addr = (XPointer)aString.c_str();
  XtConvert(aWidget,XtRString,&from,XtRPixel,&to);
  if(to.addr==NULL) return 0L;
  return (*((Pixel*)to.addr));
}
Pixel XWidgetGetPixelResource(Widget aWidget,const std::string& aName){
  Pixel pixel = 0L;
  Arg args[1];
  XtSetArg(args[0],(char*)aName.c_str(),&pixel);
  XtGetValues(aWidget,args,1);
  return pixel;
}

void XWidgetSetString(Widget aWidget,const std::string& aName,const std::string& aValue,bool aFreePrevious){
  if(aFreePrevious) {
    char* s = NULL;
    Arg args[1];
    XtSetArg(args[0],(char*)aName.c_str(),&s);
    XtGetValues(aWidget,args,1);
    if(s!=NULL) XtFree(s);
  }
  Arg args[1];
  XtSetArg(args[0],(char*)aName.c_str(),XtNewString(aValue.c_str()));
  XtSetValues(aWidget,args,1);
}
std::string XWidgetGetString(Widget aWidget,const std::string& aName){
  // XtNgeometry, XtNtitle.
  char* s = NULL;
  Arg args[1];
  XtSetArg(args[0],(char*)aName.c_str(),&s);
  XtGetValues(aWidget,args,1);
  if(s==NULL) return "";
  return std::string(s);
}
void XWidgetPrintChildren(Widget aWidget,std::ostream& a_out){
  int number = XWidgetGetChildrenNumber(aWidget); 
  a_out << "XWidgetPrintChildren :"
        << " " <<  (unsigned long)aWidget
        << " number " << number
        << std::endl;
  for(int index=0;index<number;index++) {
    Widget child = XWidgetGetChild(aWidget,index);
    a_out << " index = " << index
          << ", child = " << (unsigned long)child
          << ", class = " << std::string(XWidgetGetClassName(child))
          << std::endl;
  }
}

bool XWidgetSendMessage(Widget aWidget,long a_1,long a_2) {
  XClientMessageEvent event;
  if(!XtIsRealized(aWidget)) return 0;
  event.type           = ClientMessage;
  event.display        = XtDisplay(aWidget);
  event.window         = XtWindow(aWidget);
  event.message_type   = XA_INTEGER;
  event.format         = 8;
  event.data.l[0]      = a_1;
  event.data.l[1]      = a_2;
  event.data.l[2]      = 0L;
  event.data.l[3]      = 0L;
  event.data.l[4]      = 0L;
  if(XSendEvent(event.display,event.window,False,0L,(XEvent*)&event)==0) 
    return false;
  XFlush(event.display);
  return true;
}

#include <X11/IntrinsicP.h>
Position XWidgetGetX(Widget aWidget){return aWidget->core.x;}
Position XWidgetGetY(Widget aWidget){return aWidget->core.y;}
Dimension XWidgetGetWidth(Widget aWidget){return aWidget->core.width;}
Dimension XWidgetGetHeight(Widget aWidget){return aWidget->core.height;}
std::string XWidgetGetClassName(Widget aWidget) {
  WidgetClass wc = XtClass(aWidget);
  return std::string((char*)wc->core_class.class_name);
}
void XWidgetCompellResize(Widget aWidget){
  if(aWidget->core.widget_class->core_class.resize==NULL) return;
  (aWidget->core.widget_class->core_class.resize)(aWidget);
}
std::string XClassGetName(WidgetClass aClass){
  return std::string((char*)aClass->core_class.class_name);
}
Boolean XClassIsSubclass(WidgetClass aClass,WidgetClass aParent){
  for (WidgetClass wc = aClass; wc != NULL; wc = wc->core_class.superclass) 
    if (wc == aParent) return True;
  return False;
}

#include <X11/CompositeP.h>
int XWidgetGetChildrenNumber(Widget aWidget){
  if(!XtIsComposite(aWidget)) return 0;
  CompositePart* cwp = &(((CompositeWidget)aWidget)->composite);
  return cwp->num_children;
}
Widget XWidgetGetChild(Widget aWidget,unsigned int aIndex){
  if(!XtIsComposite(aWidget)) return 0;
  CompositePart* cwp = &(((CompositeWidget)aWidget)->composite);
  if(aIndex>=cwp->num_children) return 0;
  return cwp->children[aIndex];
}
Widget XWidgetGetFirstChild(Widget aWidget){
  if(!XtIsComposite(aWidget)) return 0;
  CompositePart* cwp = &(((CompositeWidget)aWidget)->composite);
  return (cwp->num_children?cwp->children[0]:0);
}
void XWidgetGetChildren(Widget aWidget,std::vector<Widget>& aList){
  aList.clear();
  if(!XtIsComposite(aWidget)) return;
  CompositePart* cwp = &(((CompositeWidget)aWidget)->composite);
  for (unsigned int count=0;count<cwp->num_children;count++) 
    aList.push_back(cwp->children[count]);
}
void XWidgetGetPopupChildren(Widget aWidget,std::vector<Widget>& aList){
  aList.clear();
  if(!XtIsWidget(aWidget)) return;
  for(int count=0;count<aWidget->core.num_popups;count++) {
    aList.push_back(aWidget->core.popup_list[count]);
  }
}
Widget XWidgetFindChild(Widget aWidget,const std::string& aName){
  if(!XtIsComposite(aWidget)) return 0;
  CompositePart* cwp = &(((CompositeWidget)aWidget)->composite);
  for (unsigned int count=0;count<cwp->num_children;count++) {
    if(aName==std::string(XtName(cwp->children[count]))) 
      return cwp->children[count];
  }
  return 0;
}
void XWidgetDumpPopupChildren(Widget aWidget,std::ostream& a_out){
  a_out << "XWidgetDumpPopupChildren :"
        << " " << std::string(XtName(aWidget))
        << std::endl;
  if(!XtIsWidget(aWidget)) return;
  a_out << " " << aWidget->core.num_popups << " popup children." << std::endl;
  for (unsigned int count=0;count<aWidget->core.num_popups;count++) {
    Widget w = aWidget->core.popup_list[count];
    a_out << "   " << count
          << " child class " << std::string(XWidgetGetClassName(w))
          << " name " << std::string(XtName(w))
          << std::endl;
  }
}
void XWidgetDumpChildren(Widget aWidget,std::ostream& a_out){
  a_out << "XWidgetDumpChildren :"
        << " " << std::string(XtName(aWidget))
        << std::endl;
  if(!XtIsComposite(aWidget)) return;
  CompositePart* cwp = &(((CompositeWidget)aWidget)->composite);
  a_out << " " << cwp->num_children << " children." << std::endl;
  for (unsigned int count=0;count<cwp->num_children;count++) {
    Widget w = cwp->children[count];
    a_out << "   " << count
          << " child class " << std::string(XWidgetGetClassName(w))
          << " name " << std::string(XtName(w))
          << std::endl;
  }
}

//pb with SnowLeopard.
//#define XTHREADS
//#include <X11/Xlibint.h>
//bool XThreaded(){return _Xglobal_lock ? true : false;}

