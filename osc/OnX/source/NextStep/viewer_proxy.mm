//FIXME : arrange to use OnX/Inventor/GuiViewer.h

#include <OnX/Inventor/GuiViewer.h>

// for GuiViewer_write template :
class SoGuiViewer {
public:
  SoGuiViewer(NSSoExaminerViewer* aNS):fNS(aNS),fBack(0,0,0){}
public:
  SoNode* getSceneGraph() {return [fNS sceneGraph];} 
  void render() {[fNS render];}
  SoGLRenderAction* getGLRenderAction(void) {return [fNS renderAction];}
  SoSceneManager* getSceneManager(void) {return [fNS sceneManager];}
  const SbViewportRegion & getViewportRegion(void) const {
    SoGLRenderAction* ra = [fNS renderAction];
    return ra->getViewportRegion();    
  }
  const SbColor& getBackgroundColor() {
    NSColor* color = [fNS backgroundColor];
    fBack.setValue([color redComponent],
                   [color greenComponent],
                   [color blueComponent]);
    return fBack;
  }
private:
  NSSoExaminerViewer* fNS;
  SbColor fBack;
};

inline bool viewer_proxy(
 void* aNativeViewer
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  NSSoExaminerViewer* viewer = (NSSoExaminerViewer*)aNativeViewer;
  aReturn = 0;

  if(aWhat=="write") {
    if(aArgs.size()!=3) {
      SoDebugError::postInfo("viewer_proxy",
                             "three arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    std::string file = aArgs[0];
    std::string format = aArgs[1];
    std::string opts = aArgs[2];

    //NSLog(@"debug : write \"%s\" \"%s\" \"%s\"\n",
    //      file.c_str(),format.c_str(),opts.c_str());

    SoGuiViewer gw(viewer);
    return GuiViewer_write<SoGuiViewer>(gw,format,file,opts);

  } else if(aWhat=="render") {
    [viewer render];
    return true;

  } else if(aWhat=="view_all") {
    [viewer viewAll];
    return true;

  } else if(aWhat=="set_auto_clipping") {
/*

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }
    if(value) {
      if(viewer->isAutoClipping()==FALSE) viewer->setAutoClipping(TRUE);
    } else {
      if(viewer->isAutoClipping()==TRUE) viewer->setAutoClipping(FALSE);
    }
*/
    return false;

  } else if(aWhat=="set_decoration") {
/*FIXME
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(viewer->isDecoration()==FALSE) viewer->setDecoration(TRUE);
    } else {
      if(viewer->isDecoration()==TRUE) viewer->setDecoration(FALSE);
    }
*/
    return false;

  } else if(aWhat=="set_viewing") {
/*FIXME
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(viewer->isViewing()==FALSE) viewer->setViewing(TRUE);
    } else {
      if(viewer->isViewing()==TRUE) viewer->setViewing(FALSE);
    }
*/
    return false;

  } else if(aWhat=="set_head_light") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if([viewer isHeadlight]==FALSE) [viewer setHeadlight:TRUE];
    } else {
      if([viewer isHeadlight]==TRUE) [viewer setHeadlight:FALSE];
    }
    return true;

  } else if(aWhat=="set_smoothing") {
/*
    SoGLRenderAction* action = viewer->getGLRenderAction();  
    if(!action) return false;

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(action->isSmoothing()==FALSE) action->setSmoothing(TRUE);
    } else {
      if(action->isSmoothing()==TRUE) action->setSmoothing(FALSE);
    }
*/
    return false;

  } else if(aWhat=="set_background_color") {

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    double r,g,b;
    if(!inlib::to_rgb(aArgs[0],r,g,b)) return false;
    [viewer setBackgroundColor:(float)r:(float)g:(float)b];
    return true;

  } else if(aWhat=="set_title") {
/*
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    viewer->setTitle(aArgs[0].c_str());
*/
    return false;

  } else if(aWhat=="viewport_region") {
/*
    const SbViewportRegion& vpr = viewer->getViewportRegion();
    aReturn = (void*)(&vpr);
*/
    return false;

  } else if(aWhat=="getGLRenderAction") {
/*
    SoGLRenderAction* action = viewer->getGLRenderAction();
    aReturn = (void*)action;
*/
    return false;

  } else if(aWhat=="setGLRenderAction") {
/*
    SoGLRenderAction* action =
    viewer->setGLRenderAction();
*/
    return false;

  } else if(aWhat=="setTransparencyType") {
/*
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("setTransparencyType",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }

    int type;
    if(!inlib::to<int>(aArgs[0],type)) {
      SoDebugError::postInfo("setTransparencyType",
                             "option \"%s\" not an int.",
                             aArgs[0].c_str());
      return false;
    }

    viewer->setTransparencyType((SoGLRenderAction::TransparencyType)type);
*/
    return false;

  } else if(aWhat=="transparencyType") {
/*
    SoGLRenderAction::TransparencyType type = viewer->getTransparencyType();
    int* i = new int; //should be deleted by the caller.
    *i = (int)type;
    aReturn = (void*)i;
*/
    return false;

  } else if(aWhat=="collect") {
/*
    if(aArgs.size()!=4) {
      SoDebugError::postInfo("viewer_proxy",
                             "four arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }

    int x;
    if(!inlib::to<int>(aArgs[0],x)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an int.",
                             aArgs[0].c_str());
      return false;
    }
    int y;
    if(!inlib::to<int>(aArgs[1],y)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an int.",
                             aArgs[1].c_str());
      return false;
    }
    unsigned int w;
    if(!inlib::to<unsigned int>(aArgs[2],w)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an unsigned int.",
                             aArgs[2].c_str());
      return false;
    }
    unsigned int h;
    if(!inlib::to<unsigned int>(aArgs[3],h)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an unsigned int.",
                             aArgs[3].c_str());
      return false;
    }

    const SbViewportRegion& vpr = viewer->getViewportRegion();

    SoNode* soNode = viewer->getSceneGraph();
    if(!soNode) return false;
    if(soNode->isOfType(SoPage::getClassTypeId())) {
      SoPage* soPage = (SoPage*)soNode;    
      soPage->collect(x,y,w,h,vpr);
    }
*/
    return false;

//examiner viewer :
  } else if(aWhat=="set_feedback_visibility") {
    return false;

  } else if(aWhat=="set_animation") {
    return false;

  } else if(aWhat=="stop_animating") {
    return false;
  }

  SoNode* node = [viewer sceneGraph];
  if(!node) {
    SoDebugError::postInfo("viewer_proxy","No scene graph.");
    return false;
  }

  return node_proxy(*node,aWhat,aArgs,aReturn);
}

