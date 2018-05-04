#ifndef OnX_Win_SoViewers_h
#define OnX_Win_SoViewers_h 

// Inheritance :
#include <OnX/Win/WinTk.h>

#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <Inventor/Win/viewers/SoWinFlyViewer.h>
#include <Inventor/Win/viewers/SoWinWalkViewer.h>

namespace WinTk {

class SoExaminerViewer : public Component {
public:
  SoExaminerViewer(Component&,const std::string&);
  virtual ~SoExaminerViewer();
  virtual void show();
  void setSceneGraph(SoNode*);
  SoWinExaminerViewer* soViewer() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  SoWinExaminerViewer* fViewer;
};

class SoPlaneViewer : public Component {
public:
  SoPlaneViewer(Component&,const std::string&);
  virtual ~SoPlaneViewer();
  virtual void show();
  void setSceneGraph(SoNode*);
  SoWinPlaneViewer* soViewer() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  SoWinPlaneViewer* fViewer;
};

class SoFlyViewer : public Component {
public:
  SoFlyViewer(Component&,const std::string&);
  virtual ~SoFlyViewer();
  virtual void show();
  void setSceneGraph(SoNode*);
  SoWinFlyViewer* soViewer() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  SoWinFlyViewer* fViewer;
};

class SoWalkViewer : public Component {
public:
  SoWalkViewer(Component&,const std::string&);
  virtual ~SoWalkViewer();
  virtual void show();
  void setSceneGraph(SoNode*);
  SoWinWalkViewer* soViewer() const;
private:
  static LRESULT CALLBACK proc(HWND,UINT,WPARAM,LPARAM);
private:
  SoWinWalkViewer* fViewer;
};

}

#endif
