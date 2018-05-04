#ifndef OnX_Win_PageViewer_h
#define OnX_Win_PageViewer_h 

// Inheritance :
#include <OnX/Win/WinTk.h>

#include <Inventor/SbBasic.h>
class SoWinFullViewer;
class SoNode;

namespace WinTk {

class PageViewer : public Component {
public:
  PageViewer(Component&,const std::string&,const std::string&);
  virtual ~PageViewer();
  virtual void show();
  SoNode* sceneGraph() const;
  SoWinFullViewer* soViewer() const;
  bool setVerboseLevel(int);
  void addPopupEntry(const std::string&);
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
  static SbBool eventCB(void*,MSG*);
  void getRectangle(int,int,int,int,int&,int&,unsigned int&,unsigned int&);
private:
  SoWinFullViewer* fViewer;
  HMENU fPopupMenu;
  std::vector< std::pair<int,std::string> > fPopupEntries;
  int fCounter;
  bool fRubberMove;
  POINT fBeginMove,fEndMove,fOffset;
  int fPopX,fPopY;
};

}

#endif

