#ifndef OnX_Xt_XtTools_h
#define OnX_Xt_XtTools_h 

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

#include <OnX/Xt/ListTree.h>

#include <vector>
#include <string>
#include <ostream>

//bool XThreaded();

GC XGetRubberGC(Display*,Drawable);
void XDrawRectangle(Display*,Drawable,GC,const XPoint&,const XPoint&);
bool XLoadResourceFile(Display*,const std::string&);
void XRingBell(Display*,int,int = 99);
bool XPutStringInResourceDatabase(Display*,const std::string&);
bool XGetStringResource(Display*,const std::string&,std::string&);

void ListTreeClear(Widget);
ListTreeItem* ListTreeFindItem(Widget,const std::vector<std::string>&);
std::string ListTreeGetSelection(Widget,bool&);
bool ListTreeSetSelection(Widget,const std::vector<std::string>&);
std::string ListTreeGetTopItem(Widget);
bool ListTreeSetTopItem(Widget,const std::vector<std::string>&);
std::string ListTreeItemGetPath(ListTreeItem*);
std::string ListTreeItemGetXML(ListTreeItem*,int);

std::string XClassGetName(WidgetClass);
Boolean XClassIsSubclass(WidgetClass,WidgetClass);

void XWidgetSetString(Widget,const std::string&,const std::string&,bool = false);
std::string XWidgetGetString(Widget,const std::string&);
void XWidgetPrintChildren(Widget,std::ostream&);
Pixel XWidgetConvertStringToPixel(Widget,const std::string&);
Pixel XWidgetGetPixelResource(Widget,const std::string&);
int XWidgetGetChildrenNumber(Widget); 
Widget XWidgetGetChild(Widget,unsigned int);
void XWidgetGetChildren(Widget,std::vector<Widget>&);
void XWidgetGetPopupChildren(Widget,std::vector<Widget>&);

Widget XWidgetGetShell(Widget);
Widget XWidgetGetFirstChild(Widget);
Widget XWidgetFindChild(Widget,const std::string&);
std::string XWidgetGetClassName(Widget);
Position XWidgetGetX(Widget);
Position XWidgetGetY(Widget);
Dimension XWidgetGetWidth(Widget);
Dimension XWidgetGetHeight(Widget);
void XWidgetCompellResize(Widget);
void XWidgetDumpPopupChildren(Widget);
void XWidgetDumpChildren(Widget);
bool XWidgetSendMessage(Widget,long,long);

bool XmGetString(Widget,const std::string&,std::string&);
void XmSetString(Widget,const std::string&,const std::string&);
bool XmGetItems(Widget,const std::string&,const std::string&,std::vector<std::string>&);
bool XmSetLabelPixmap(Widget,const std::string&);
XmString* XmStringTableCreateFromText(const std::vector<std::string>&,int&);
void XmStringTableDelete(XmString*);
bool XmShow(Widget);
bool XmHide(Widget);

void XmScaleGetMinMax(Widget,int&,int&);
void XmScrollBarGetMinMax(Widget,int&,int&);

void XmTextAppendString(Widget,const std::string&);
void XmTextSetNumberOfRowsAndColumns(Widget,int,int);

void XmComboBoxSetItems(Widget,const std::vector<std::string>&);
void XmComboBoxSetValue(Widget,const std::string&);
bool XmComboBoxGetValue(Widget,std::string&);
bool XmComboBoxGetItems(Widget,std::vector<std::string>&);
bool XmPanedSetSplitPosition(Widget,int);
bool XmPanedIsVertical(Widget);
int XmPanedGetNumberOfUserChildren(Widget);
void XmShellSetDecorations(Widget,const std::vector< std::pair<bool,std::string> >&);

bool XmConvertString(XmString,std::string&);

#endif

