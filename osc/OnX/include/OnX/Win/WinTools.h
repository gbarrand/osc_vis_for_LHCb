#ifndef OnX_Win_WinTools_h
#define OnX_Win_WinTools_h 

#include <windows.h>
#include <commctrl.h>

#include <string>
#include <vector>

namespace WinTk {

std::string TreeGetItemLabel(HWND,HTREEITEM); 
bool TreeIsItemBranch(HWND,HTREEITEM); 
bool TreeIsItemExpanded(HWND,HTREEITEM); 
std::string TreeGetItemPath(HWND,HTREEITEM);
std::string TreeGetItemXML(HWND,HTREEITEM);

void ChangeStyle(HWND,LONG,bool);
bool IsStyle(HWND,LONG);
bool IsShell(HWND);
HWND GetShell(HWND);
bool Show(HWND);
bool Hide(HWND);
void GetSize(HWND,int&,int&);
unsigned int GetNumberOfChildren(HWND);
HWND GetLastChild(HWND);
std::string GetText(HWND);
HWND GetTabStop(HWND,HWND,bool = true);
HWND GetTabStopInSibling(HWND,bool);
HWND GetTabStopInTree(HWND,bool);
bool IsTabStop(HWND);
std::string GetClassName(HWND);
HWND GetChildByClassName(HWND,const std::string&);
void SetGeometry(HWND,int,int,unsigned int,unsigned int);
void PrintChildren(HWND);
HBITMAP ReadXpm(HDC,const std::string&,int&,int&);
HBITMAP ConvertXpmToDIB(HDC,const std::vector<std::string>&,int&,int&);
HBITMAP CreateDIB(HDC,int,int,int,void**);

}

#endif

