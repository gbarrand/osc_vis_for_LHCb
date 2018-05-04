// this :
#include <OnX/Win/WinTools.h>

#include <inlib/smanip>

#include <windowsx.h>

inline int smanip_axtoi(const std::string& a_string) {
  // convert from ASCII hex to int
  // Exa : "FF" -> 256.
  int x = 0;
  int n = a_string.size();
  // convert n nibbles
  for (int i = 0; i < n; i++) {
    char c = a_string[i];
    // numbers 0 - 9
    if ((c > 0x2F) && (c < 0x3A))
      x += ((c - 0x30) << ((n - i - 1) * 4));

    // capital letters A - F
    if ((c > 0x40) && (c < 0x47))
      x += ((c - 0x41 + 0x0A) << ((n - i - 1) * 4));

    // lower case letters a - f
    if ((c > 0x60) && (c < 0x67))
      x += ((c - 0x61 + 0x0A) << ((n - i - 1) * 4));
  }
  return x;
}

//////////////////////////////////////////////////////////////////////////////
std::string WinTk::TreeGetItemLabel(
 HWND aWindow
,HTREEITEM aItem
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  TCHAR text[256];
  TV_ITEM item;
  item.hItem = aItem;
  item.mask = TVIF_TEXT;
  item.pszText = text;
  item.cchTextMax = 256;
  if(!TreeView_GetItem(aWindow,&item)) return "";
  return std::string(text);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::TreeIsItemBranch(
 HWND aWindow
,HTREEITEM aItem
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  TV_ITEM item;
  item.hItem = aItem;
  item.mask = TVIF_CHILDREN;
  if(!TreeView_GetItem(aWindow,&item)) return 0;
  return item.cChildren?true:false;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::TreeIsItemExpanded(
 HWND aWindow
,HTREEITEM aItem
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  TV_ITEM item;
  item.hItem = aItem;
  item.mask = TVIS_EXPANDED;
  if(!TreeView_GetItem(aWindow,&item)) return 0;
  return ((item.state & TVIS_EXPANDED)== TVIS_EXPANDED) ? true : false;
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::TreeGetItemPath(
 HWND aWindow
,HTREEITEM aItem
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  HTREEITEM item = aItem;
  std::string path;
  while(item && (item!=TVI_ROOT)) {
    std::string ss = TreeGetItemLabel(aWindow,item);
    std::string opath = path;
    path = "\n";
    path += ss;
    path += opath;
    item = TreeView_GetParent(aWindow,item);
  }
  // Remove the leading \n
  if(path.size()) path = path.substr(1,path.size()-1);
  return path;
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::TreeGetItemXML(
 HWND aWindow
,HTREEITEM aItem
)
//////////////////////////////////////////////////////////////////////////////
//return a XML string representing this tree
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string spaceItem = "";
  std::string spaceRoot = "";
  std::string line;
  do  {
    line += spaceRoot + "<treeItem>";
    line += spaceItem + "<label>";
    std::string s = WinTk::TreeGetItemLabel(aWindow,aItem);
    line.append(s);
    line += "</label>";
    line += spaceItem + "<opened>";
    if (TreeIsItemExpanded(aWindow,aItem)) line.append("true");
    else line.append("false");
    line += "</opened>";
    if (TreeView_GetChild(aWindow,aItem)) {
      line += TreeGetItemXML(aWindow,TreeView_GetChild(aWindow,aItem));
    }    
    line += spaceRoot + "</treeItem>";
    aItem = TreeView_GetNextSibling(aWindow,aItem);
  }
  while (aItem);
  return line;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool WinTk::IsStyle(
 HWND aWindow
,LONG aStyle
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  LONG style = ::GetWindowLong(aWindow,GWL_STYLE);
  return ( ((style & aStyle ) == aStyle) ? true : false);
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::ChangeStyle(
 HWND aWindow
,LONG aStyle
,bool aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  LONG style = ::GetWindowLong(aWindow,GWL_STYLE);
  if(aValue) { //Enable style.
    style = style | aStyle;
  } else { //Disbale style.
    style = style & ~aStyle;
  }
  ::SetWindowLong(aWindow,GWL_STYLE,style);
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::IsShell(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(IsStyle(aWindow,WS_OVERLAPPEDWINDOW)) return true;
  if(IsStyle(aWindow,WS_POPUP)) return true;
  return false;
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::GetShell(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  HWND window = aWindow;
  if(IsShell(window)) return window;
  while(true) {
    HWND parent = GetParent(window);
    if(!parent) return window;
    if(IsShell(parent)) return parent;
    window = parent;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Show(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  HWND shell = WinTk::GetShell(aWindow);
  if(!shell) return false;
  ::SetForegroundWindow(shell);
  ::ShowWindow(shell,SW_SHOWDEFAULT);
  ::UpdateWindow(shell);
  ::DrawMenuBar(shell);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool WinTk::Hide(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  HWND shell = WinTk::GetShell(aWindow);
  if(!shell) return false;
  ::ShowWindow(shell,SW_HIDE);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::GetSize(
 HWND aWindow
,int& aWidth
,int& aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  RECT rect;
  ::GetWindowRect(aWindow,&rect);
  aWidth = rect.right-rect.left;
  aHeight = rect.bottom-rect.top;
}
//////////////////////////////////////////////////////////////////////////////
unsigned int WinTk::GetNumberOfChildren(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = 0;
  HWND child = GetFirstChild(aWindow);
  while(child) {
    number++;
    child = GetNextSibling(child);
  }
  return number;
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::GetLastChild(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  HWND child = GetFirstChild(aWindow);
  while(child) {
    HWND next = GetNextSibling(child);
    if(!next) return child;
    child = next;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::GetText(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int l = ::GetWindowTextLength(aWindow);
  std::string s;
  s.resize(l);
  ::GetWindowText(aWindow,(char*)s.c_str(),l+1);
  return s;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::GetTabStop(
 HWND aDialog
,HWND aWindow
,bool aForward
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aWindow) return NULL;
  HWND window = aWindow;
  while(window && (window!=aDialog) ) {
    HWND tabStop = GetTabStopInSibling(window,aForward);
    if(tabStop) return tabStop;
    window = GetParent(window);
  }
  return GetTabStopInTree(aDialog,aForward);
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::GetTabStopInSibling(
 HWND aWindow
,bool aForward
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aWindow) return NULL;
  HWND child = aForward ? GetNextSibling(aWindow) : GetPrevSibling(aWindow);
  while(child) {
    HWND tabStop = GetTabStopInTree(child,aForward);
    if(tabStop) return tabStop;
    child = aForward ? GetNextSibling(child) : GetPrevSibling(child);
  }
  return NULL;
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::GetTabStopInTree(
 HWND aWindow
,bool aForward
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(IsTabStop(aWindow)) return aWindow;
  HWND child = aForward ? GetFirstChild(aWindow) : GetLastChild(aWindow);
  while(child) {
    HWND tabStop = GetTabStopInTree(child,aForward);
    if(tabStop) return tabStop;
    child = aForward ? GetNextSibling(child) : GetPrevSibling(child);
  }
  return NULL;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool WinTk::IsTabStop( 
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  LONG style = ::GetWindowLong(aWindow,GWL_STYLE);
  return ((style & WS_TABSTOP ) == WS_TABSTOP ? true : false);
}
//////////////////////////////////////////////////////////////////////////////
std::string WinTk::GetClassName(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!aWindow) return "";
  char className[256];
  ::GetClassName(aWindow,className,128);
  return className;
}
//////////////////////////////////////////////////////////////////////////////
HWND WinTk::GetChildByClassName(
 HWND aWindow
,const std::string& aClassName
)
//////////////////////////////////////////////////////////////////////////////
// A CBS_SIMPLE has two children of class ComboLBox and Edit.
// At creation, a CBS_DROPDOWN has one child of class Edit.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aWindow) return NULL;
  HWND child = GetFirstChild(aWindow);
  while(child) {
    if(GetClassName(child)==aClassName) return child;
    child = GetNextSibling(child);
  }
  return NULL;
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::SetGeometry(
 HWND aWindow
,int aX
,int aY
,unsigned int aWidth
,unsigned int aHeight
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  if(!aWindow) return;
  std::string className = GetClassName(aWindow);
  if(className=="ComboBox") {
    // For a Combo the height is the visible part + the height of the list !
    // (A combo may have no edit control).
    ::MoveWindow(aWindow,aX,aY,aWidth,200,TRUE);
  } else {
    ::MoveWindow(aWindow,aX,aY,aWidth,aHeight,TRUE);
  }
}
//////////////////////////////////////////////////////////////////////////////
void WinTk::PrintChildren(
 HWND aWindow
)
//////////////////////////////////////////////////////////////////////////////
// A CBS_SIMPLE has two children of class ComboLBox and Edit.
// At creation, a CBS_DROPDOWN has one child of class Edit.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aWindow) return;
  HWND child = GetFirstChild(aWindow);
  while(child) {
    //printf("debug : window : %ld, child : %ld class \"%s\"\n",
    //   aWindow,child,ClassName(child).c_str());
    child = GetNextSibling(child);
  }
}

#include <inlib/file>
#include <inlib/system>

//////////////////////////////////////////////////////////////////////////////
HBITMAP WinTk::ReadXpm(
 HDC aDC
,const std::string& aFileName
,int& aWidth
,int& aHeight
)
//////////////////////////////////////////////////////////////////////////////
{
  aWidth = 0;
  aHeight = 0;
  std::vector<std::string> text;
  std::string name;
  inlib::file_name(aFileName,name);
  if(!inlib::file::read(name,text)) return 0;

  std::vector<std::string> xpm;

  {for(unsigned int index=0;index<text.size();index++) {
    if(text[index][0]!='"') continue;
    std::string::size_type l = text[index].size();
    std::string line = text[index].substr(1,l-1);
    std::string::size_type pos = line.find("\"");
    if(pos==std::string::npos) return 0;
    xpm.push_back(line.substr(0,pos));
  }}

  if(!xpm.size()) return 0;

  HBITMAP bitmap = ConvertXpmToDIB(aDC,xpm,aWidth,aHeight);

  return bitmap;
}
#include <cstring>
#include <cstdlib> //atoi
//////////////////////////////////////////////////////////////////////////////
HBITMAP WinTk::ConvertXpmToDIB(
 HDC aDC
,const std::vector<std::string>& aXPM
,int& aWidth
,int& aHeight
)
//////////////////////////////////////////////////////////////////////////////
// Convert from xpm to DIB (demands hex colors).
// From CoinWin/widgets/SoWinBitmapButton.cpp.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aWidth = 0;
  aHeight = 0;
  if(!aXPM.size()) return 0;

  std::vector<int> vals;
  inlib::values<int>(aXPM[0]," ",false,vals);
  if(vals.size()!=4) return 0;
  int width = vals[0];
  int height = vals[1];
  int numcol = vals[2];
  int numchars = vals[3];
  if(width<=0) return 0;
  if(height<=0) return 0;
  if(numcol<=0) return 0;
  if(numchars<=0) return 0;

  // Check consistency :
  if((height+numcol)!=aXPM.size()-1) return 0;

  int lcolor = 0;
  int i;
  for (i = 0; i < numcol; i++) {
    std::string::size_type pos = aXPM[i+1].rfind("c #");
    if(pos==std::string::npos) continue; // May be "c None"
    lcolor = aXPM[i+1].size()-(pos+3);
    break;
  }
  //printf("debug : xpm : numcol %d (%d)\n",numcol,lcolor);

  if( (lcolor!=6) && (lcolor!=12) ) return 0;

  // create color lookup table
  char* charlookuptable = new char[numcol * numchars];
  //long* colorlookuptable = new long[numcol];
  int* rlookuptable = new int[numcol];
  int* glookuptable = new int[numcol];
  int* blookuptable = new int[numcol];

  bool done = true;
  // get colors
  for (i = 0; i < numcol; i++) {

    const std::string& line = aXPM[i+1];

    // Check consistency :
    if((int)line.size()<numchars) {
      done = false;
      break;
    }

    for (int j = 0; j < numchars; j ++) {
      charlookuptable[(i * numchars) + j] = line[j];
    }

    // Find color by value :
    std::string::size_type pos = line.find("c #",numchars);
    if(pos!=std::string::npos) {
      int lc = line.size()-(pos+3);
      if(lc!=lcolor) {
        done = false;
        //printf("debug : xpm : error in \"%s\" %d %d\n",
        //line.c_str(),lc,lcolor);
        break;
      }
      if(lcolor==6) {
        rlookuptable[i] = smanip_axtoi(line.substr(pos+3+0,2));
        glookuptable[i] = smanip_axtoi(line.substr(pos+3+2,2));
        blookuptable[i] = smanip_axtoi(line.substr(pos+3+4,2));
      } else { //lcolor 12
        rlookuptable[i] = smanip_axtoi(line.substr(pos+3+0,2));
        glookuptable[i] = smanip_axtoi(line.substr(pos+3+4,2));
        blookuptable[i] = smanip_axtoi(line.substr(pos+3+8,2));
      }

      //printf("debug : xpm : color %d : (%x,%x,%x)\n",
      //     i,rlookuptable[i],glookuptable[i],blookuptable[i]);

    } else { // Could be "c None"
      rlookuptable[i] = -1;
      glookuptable[i] = -1;
      blookuptable[i] = -1;
    }
  }

  //printf("debug : xpm : read color %d\n",done);

  //WARNING : pixelsize 3 does not work with button in a toolbar.
  // unsigned char pixelsize = 3;
  unsigned char pixelsize = 4;

  if(!done) pixelsize = 0; // Will induce a clean exit.

  // create bitmap
  void* dest;
  HBITMAP hbmp = CreateDIB(aDC,width,height,pixelsize * 8,&dest);

  if(hbmp) {
    int noneColor = ::GetSysColor(COLOR_3DFACE);
    done = true;
    // put pixels
    for (i = 0; i < height; i++) {

      const std::string& line = aXPM[i + 1 + numcol];

      // Check consistency :
      if((int)line.size()!=(numchars*width)) {
        done = false;
        break;
      }

      int y = i * width * pixelsize;

      for (int j = 0; j < width; j++) {

        int x = j * pixelsize;

        // for every color
        for (int k = 0; k < numcol; k++) {

          bool found = true;
          for (int l = 0; l < numchars; l++) {
            if (charlookuptable[(k * numchars) + l] 
                  != line[(j * numchars) + l]) {
              found = false;
              break;
            }
          }

          if(found) {

            unsigned char r,g,b;
            if (rlookuptable[k] == -1) {
              r = (noneColor & 0x00FF0000)>>16;
              g = (noneColor & 0x0000FF00)>>8;
              b = noneColor & 0x000000FF;
            } else {
              r = rlookuptable[k];
              g = glookuptable[k];
              b = blookuptable[k];
            }
            
            if(pixelsize==4) {
              ((unsigned char*)dest)[y + x + 0] = b;
              ((unsigned char*)dest)[y + x + 1] = g;
              ((unsigned char*)dest)[y + x + 2] = r;
              ((unsigned char*)dest)[y + x + 3] = 0;
            } else {
              ((unsigned char*)dest)[y + x + 0] = b;
              ((unsigned char*)dest)[y + x + 1] = g;
              ((unsigned char*)dest)[y + x + 2] = r;
            }
            
            // next pixel
            break;
            
          }
          
        }
        
      }
      
    }
    if(!done) {
      //printf("debug : xpm : can'tread pixels.\n");
      ::DeleteObject(hbmp);
      hbmp = 0;
    }    
  }

  // cleanup
  delete [] charlookuptable;
  delete [] rlookuptable;
  delete [] glookuptable;
  delete [] blookuptable;

  if(hbmp) {
    aWidth = width;
    aHeight = height;
  }

  return hbmp;
}
//////////////////////////////////////////////////////////////////////////////
HBITMAP WinTk::CreateDIB(
 HDC aDC
,int aWidth
,int aHeight
,int aBPP          // 16||24||32 bpp
,void** aBits 
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if((aBPP!=24) && (aBPP!=32)) return 0;

  BITMAPINFO format;
  BITMAPINFOHEADER* header = (BITMAPINFOHEADER*)&format;
  header->biSize = sizeof(BITMAPINFOHEADER);
  header->biWidth = aWidth;
  header->biHeight = -aHeight;
  header->biPlanes = 1;
  header->biBitCount = aBPP;
  header->biCompression = BI_RGB;
  header->biSizeImage = 0;
  header->biXPelsPerMeter = 0;
  header->biYPelsPerMeter = 0;
  header->biClrUsed = 0;
  header->biClrImportant = 0;

  UINT flag = DIB_RGB_COLORS;
  HBITMAP bitmap = ::CreateDIBSection(aDC,&format,flag,(void**)aBits,NULL,0);

  if(!(*aBits)) return 0;

  return bitmap;
}
