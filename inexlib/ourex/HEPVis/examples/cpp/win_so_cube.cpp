
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/errors/SoDebugError.h>

#include <windowsx.h>
#include <commctrl.h>

#include <stdlib.h>

#define Widget HWND

static HWND sButton = 0;

static WNDPROC oldProc = 0;

static LRESULT CALLBACK proc(HWND aWindow,UINT aMessage,WPARAM aWParam,LPARAM aLParam){
  switch(aMessage) { 
  case WM_COMMAND:{
    HWND wnd = (HWND)aLParam;
    if(wnd==sButton) {
      printf("exit...\n");
      ::PostQuitMessage(0);
    }
  }return 0;
  default: 
    break;
  }
  return CallWindowProc(oldProc,aWindow,aMessage,aWParam,aLParam);
}

int main(int,char** aArgv) {
  Widget top = SoWin::init(aArgv[0]);
  if (top==NULL) exit(1);

  // Create a scene :

  SoSeparator* root=new SoSeparator;
  root->ref();

  SoMaterial* material = new SoMaterial;
  material->diffuseColor.setValue (SbColor(1,1,0));
  root->addChild(material);

  SoScale* scale = new SoScale;
  scale->scaleFactor.setValue(0.1F,0.1F,0.1F);
  //scale->scaleFactor.setValue(0.5F,0.5F,0.5F);
  root->addChild(scale);

  SoCube* cube = new SoCube;
  root->addChild(cube);

  SoWinExaminerViewer* viewer = new SoWinExaminerViewer(top);

  HWND buttonsParent = viewer->getAppPushButtonParent();
  if(buttonsParent) {
    // Create a button :
    sButton = ::CreateWindow(WC_BUTTON,
                                 "Esc",
                                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                 0,0,100,100,
                                 buttonsParent,0,
                                 GetWindowInstance(buttonsParent),
                                 NULL);
    if(!sButton) return EXIT_FAILURE;
    viewer->addAppPushButton(sButton);
    // Override buttonsParent proc to catch WM_COMMAND coming 
    // from the button :
    oldProc = (WNDPROC)::GetWindowLong(buttonsParent,GWL_WNDPROC);
    ::SetWindowLong(buttonsParent,GWL_WNDPROC,(LONG)proc);
  } else {
    SoDebugError::postInfo("main","no app buttons parent !");
  }


  viewer->setBackgroundColor(SbColor(0.4F,0.4F,0.4F));
  viewer->setSceneGraph(root);
  viewer->viewAll();
  viewer->setSize(SbVec2s(700,500));
  //viewer->show();
  viewer->setTitle("CoinWin");
  //viewer->setViewing(FALSE);
  //viewer->setDecoration(FALSE);
  //viewer->setAutoClipping(FALSE);

  SoWin::show(top);

  SoWin::mainLoop();

  //Cleanup
  root->unref();

  delete viewer;

  //SoWin::clear();  

  return EXIT_SUCCESS;
}
