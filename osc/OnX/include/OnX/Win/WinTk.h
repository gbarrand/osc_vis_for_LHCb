#ifndef OnX_WinTk_h
#define OnX_WinTk_h 

#include <windows.h>
#include <commctrl.h>

#include <string>
#include <vector>

namespace WinTk {

class Shell;
typedef Shell* (*GetFocusedShellProc)(void*);

int steer(GetFocusedShellProc = 0,void* = 0);

class Component;

class CallbackData {
public:
  CallbackData():x(0),y(0),wparam(0),lparam(0){}
public:
  std::string value;
  int x;
  int y;
  WPARAM wparam;
  LPARAM lparam;
};

typedef void (*Callback)(Component&,CallbackData&,void*);

class Component {
  typedef std::vector< std::pair<Callback,void*> > Callbacks;
  typedef std::pair<std::string,Callbacks> NamedCallbacks;
public:
  Component(const std::string&);
  Component(const std::string&,Component&);
  virtual ~Component();
  //FIXME : private : cpsctor et operator=.
public:
  const std::string& type() const;
  Component* parent() const;
  virtual void show();
  void hide();
  Component* findFather(const std::string&);
  void addCallback(const std::string&,Callback,void*);
  void removeCallback(const std::string&,Callback,void*);
  bool executeCallbacks(const std::string&,CallbackData&);
  bool hasCallbacks(const std::string&) const;
  std::string name() const;
  void setName(const std::string&);
  HWND nativeWindow();
  virtual bool size(unsigned int&,unsigned int&);
  virtual bool position(int&,int&);
  bool setBackground(double,double,double);
protected:
  static void wm__destroy(HWND); 
protected:
  void destroy();
  static LRESULT CALLBACK containerProc(HWND,UINT,WPARAM,LPARAM);
  void rubberDrawLine(POINT&,POINT&);
  void rubberDrawRect(POINT&,POINT&);
private:
  std::string fType;
protected:
  HWND fWindow;
  Component* fParent;
  std::vector<NamedCallbacks> fCallbacks;
  std::string fName;
};

class MessageWindow : public Component {
public:
  MessageWindow();
  virtual ~MessageWindow();
  bool sendMessage(void*);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
};

typedef void(*SetFocusCallback)(Shell*,void*);

class Shell : public Component {
public:
  Shell(unsigned int);
  virtual ~Shell();
  virtual void show();
  void setFocusWindow(HWND);
  HWND focusWindow();
  void setTitle(const std::string&);
  void setGeometry(int,int,unsigned int,unsigned int);
  void setSize(unsigned int,unsigned int);
  void setSetFocusCallback(SetFocusCallback,void*);
  HACCEL nativeAcceleratorTable(); 
  bool addAccelerator(const std::string&,int);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  void callSetFocusCallback(); 
private:
  HWND fFocusWindow;
  SetFocusCallback fSetFocusCallback;
  void* fSetFocusTag;
  HACCEL fAcceleratorTable;
};

class Dialog : public Component {
public:
  Dialog(Component&,unsigned int =0);
  virtual ~Dialog();
  virtual void show();
  void setTitle(const std::string&);
  void setSize(unsigned int,unsigned int);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
};

class WarningDialog : public Component {
public:
  WarningDialog(Component&);
  virtual ~WarningDialog();
  virtual void show();
  void setTitle(const std::string&);
  void setMessage(const std::string&);
private:
  std::string fTitle;
  std::string fMessage;
};

class Label : public Component {
public:
  Label(Component&);
  virtual ~Label();
  void setLabel(const std::string&);
  const std::string& label();
private:
  std::string fLabel;
};

class Button : public Component {
public:
  Button(Component&,bool = false);
  virtual ~Button();
  void setLabel(const std::string&);
  void setPixmap(const std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
  HBITMAP fBitmap;
};

class ToggleButton : public Component {
public:
  ToggleButton(Component&);
  virtual ~ToggleButton();
  void setLabel(const std::string&);
  bool isChecked() const;
  void setChecked(bool);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};


class OptionMenu : public Component {
public:
  OptionMenu(Component&);
  virtual ~OptionMenu();
  bool initialize(const std::vector<std::string>&,const std::string&);
  bool items(std::vector<std::string>&);
  bool setDefault(const std::string&);
  std::string value() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  static LRESULT CALLBACK editProc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
  WNDPROC fEditOldProc;
};

class ComboBox : public Component {
public:
  ComboBox(Component&);
  virtual ~ComboBox();
  bool setItems(const std::vector<std::string>&);
  bool items(std::vector<std::string>&);
  bool setValue(const std::string&);
  std::string value() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  static LRESULT CALLBACK editProc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
  WNDPROC fEditOldProc;
};

class Tree : public Component {
public:
  Tree(Component&);
  virtual ~Tree();
  HTREEITEM insertItem(HTREEITEM,const std::string&); 
  bool selection(std::string&) const;
  bool setSelection(const std::vector<std::string>&);
  void clear();
  bool enableCallbacks(bool);
  void selectFirst();
  HTREEITEM firstItem(); 
  std::vector<HTREEITEM> itemChildren(HTREEITEM);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
  bool fCallbackEnable;
};

class Frame : public Component {
public:
  Frame(Component&);
  virtual ~Frame();
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
};

class ToolBarItem : public Component {
public:
  ToolBarItem(Component&,
              const std::string&,const std::string&,const std::string&);
  virtual ~ToolBarItem();
  const std::string& label() const;
  const std::string& toolTip() const;
  const std::string& pixmap() const;
  void setBitmap(HBITMAP);
  HBITMAP bitmap() const;
private:
  std::string fLabel;
  std::string fPixmap;
  std::string fToolTip;
  HBITMAP fBitmap;
};

class ToolBar : public Component {
public:
  ToolBar(Component&);
  virtual ~ToolBar();
  void addItem(ToolBarItem*);
  void createItems();
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
  std::vector<ToolBarItem*> fItems;
  std::vector<HBITMAP> fBitmaps;
};

class ProgressBar : public Component {
public:
  ProgressBar(Component&);
  virtual ~ProgressBar();
  void setValue(int);
  int value() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class TabStack : public Component {
public:
  TabStack(Component&);
  virtual ~TabStack();
  bool addItem(Component&,const std::string&);
  bool removeItem(Component&);
  bool selection(std::string&) const;
  bool setSelection(const std::string&);
  int itemCount() const;
  bool setLabel(Component&,const std::string&);
  bool label(Component&,std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  void setCurrent(int);
private:
  WNDPROC fOldProc;
};

class VBox : public Component {
public:
  enum Expand {FIRST,SECOND,BOTH};
  VBox(Component&);
  virtual ~VBox();
  void setSizeOfFixed(int);
  int sizeOfFixed() const;
  void setExpand(Expand);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  int fSizeOfFixed;
  int fSplitHalfSize;
  Expand fExpand;
};

class HBox : public Component {
public:
  enum Expand {FIRST,SECOND,BOTH};
  HBox(Component&);
  virtual ~HBox();
  void setSizeOfFixed(int);
  int sizeOfFixed() const;
  void setExpand(Expand);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  int fSizeOfFixed;
  int fSplitHalfSize;
  Expand fExpand;
};

class VPaned : public Component {
public:
  VPaned(Component&);
  virtual ~VPaned();
  void setSplitPosition(double);
  double splitPosition() const;
  void setSplitPositionPixel(int);
  int splitPositionPixel() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  void resizeChildren(int,int);
private:
  double fSplitPosition;
  int fSplitHalfSize;
  bool fRubberMove;
  POINT fBeginMove,fEndMove;
};

class HPaned : public Component {
public:
  HPaned(Component&);
  virtual ~HPaned();
  void setSplitPosition(double);
  double splitPosition() const;
  void setSplitPositionPixel(int);
  int splitPositionPixel() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  void resizeChildren(int,int);
private:
  double fSplitPosition;
  int fSplitHalfSize;
  bool fRubberMove;
  POINT fBeginMove,fEndMove;
  int fOldWidth;
};

class VContainer : public Component {
public:
  VContainer(Component&);
  virtual ~VContainer();
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  int fMargin;
};

class HContainer : public Component {
public:
  HContainer(Component&);
  virtual ~HContainer();
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  int fMargin;
};

class VScrollBar : public Component {
public:
  VScrollBar(Component&);
  virtual ~VScrollBar();
  int value() const;
  void setValue(int);
  void setMinimum(int);
  void setMaximum(int);
  int minimum() const;
  int maximum() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class HScrollBar : public Component {
public:
  HScrollBar(Component&);
  virtual ~HScrollBar();
  int value() const;
  void setValue(int);
  void setMinimum(int);
  void setMaximum(int);
  int minimum() const;
  int maximum() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class VScale : public Component {
public:
  VScale(Component&);
  virtual ~VScale();
  int value() const;
  void setValue(int);
  void setMinimum(int);
  void setMaximum(int);
  int minimum() const;
  int maximum() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class HScale : public Component {
public:
  HScale(Component&);
  virtual ~HScale();
  int value() const;
  void setValue(int);
  void setMinimum(int);
  void setMaximum(int);
  int minimum() const;
  int maximum() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class MenuBar : public Component {
public:
  MenuBar(Component&);
  virtual ~MenuBar();
  HMENU hmenu() const;
private:
  HMENU fMenu;
};

class CascadeButton : public Component {
public:
  CascadeButton(Component&,const std::string&);
  virtual ~CascadeButton();
  void setLabel(const std::string&);
  const std::string& label() const;
  HMENU hmenu() const;
private:
  void refresh();
private:
  HMENU fParentMenu;
  HMENU fMenu;
  int fPos;
  std::string fLabel;
};

class Menu : public Component {
public:
  Menu(Component&);
  virtual ~Menu();
  HMENU hmenu() const;
};

class MenuItem : public Component {
public:
  MenuItem(Component&,const std::string&,int,bool = false);
  virtual ~MenuItem();
  bool isToggle() const;
  int identifier();
  void setLabel(const std::string&);
  bool label(std::string&) const;
  bool isChecked() const; 
  void setChecked(bool);
  void toggleChecked();
  bool addAccelerator(const std::string&);
private:
  bool fIsToggle;
  int fID;
  HMENU fMenu;
};

class MenuSeparator : public Component {
public:
  MenuSeparator(Component&);
  virtual ~MenuSeparator();
};

class CommandLine : public Component {
public:
  CommandLine(Component&);
  virtual ~CommandLine();
  const std::string& value() const;
  void setValue(const std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
  std::vector<std::string> fHistory;
  int fHistoryPos;
  std::string fValue;
};

class Entry : public Component {
public:
  Entry(Component&,bool = false);
  virtual ~Entry();
  std::string value();
  void setValue(const std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class ScrolledList : public Component {
public:
  ScrolledList(Component&);
  virtual ~ScrolledList();
  void setItems(const std::vector<std::string>&,const std::string& = "");
  std::vector<std::string> items() const;
  bool selection(std::string&) const;
  bool setSelection(const std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class ScrolledText : public Component {
public:
  ScrolledText(Component&);
  virtual ~ScrolledText();
  std::string value(); 
  void setText(const std::string&);
  void appendString(const std::string&);
  void clear();
private:
  void appendSimpleString(const std::string&, bool cr = true);
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  WNDPROC fOldProc;
};

class FileSelectionDialog : public Component {
public:
  FileSelectionDialog(Component&);
  virtual ~FileSelectionDialog();
  virtual void show();
  void setDirectory(const std::string&);
  void setDirMask(const std::string&);
  void setMode(const std::string&);
private:
  std::string fDirectory;
  std::string fDirMask;
  std::string fMode;
};

class ColorSelectionDialog : public Component {
public:
  ColorSelectionDialog(Component&);
  virtual ~ColorSelectionDialog();
  virtual void show();
};

class PaintArea : public Component {
public:
  typedef void(*PaintCallback)(HWND,void*);
public:
  PaintArea(Component&);
  virtual ~PaintArea();
  void refresh();
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
};

class OpenGLArea : public Component {
public:
  OpenGLArea(Component&);
  virtual ~OpenGLArea();
public:
  bool write_gl2ps(const std::string&,const std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  HGLRC fContext;
  HDC fHDC;
};

}

#endif
