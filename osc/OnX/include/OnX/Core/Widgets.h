#ifndef OnX_Widgets_h
#define OnX_Widgets_h 

// inheritance :
#include <OnX/Core/Widget.h>
#include <Slash/UI/ISoViewer.h>

// Slash :
#include <Slash/UI/IUI.h>

#include <inlib/sprintf>
#include <inlib/tos>
#include <inlib/pointer>
#include <inlib/cast>

namespace OnX {

class SoViewer : public OnX::Widget, public virtual Slash::UI::ISoViewer {
public: //Slash::UI::IWidget
  virtual bool write(const std::string& aFile,
                     const std::string& aFormat,
                     const std::string& aOptions) {
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    args.push_back(aFile);
    args.push_back(aFormat);
    args.push_back(aOptions);
    void* ret;
    return fViewerProxy(fViewer,"write",args,ret);
  }
public: //Slash::UI::ISoViewer
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnX::SoViewer)
    else INLIB_IF_CAST(Slash::UI::ISoViewer)
    else return OnX::Widget::cast(a_class);
  }
  virtual SoNode* sceneGraph() const {return fSoNode;}
  virtual void setBackgroundColor(double aRed,double aGreen,double aBlue) {
    if(!fViewerProxy) return;
    std::string s;
    inlib::sprintf(s,96,"%g %g %g",aRed,aGreen,aBlue);
    std::vector<std::string> args;
    args.push_back(s);
    void* ret;
    fViewerProxy(fViewer,"set_background_color",args,ret);
  }
  virtual void setAutoClipping(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_auto_clipping",args,ret);
  }
  virtual void setDecoration(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_decoration",args,ret);
  }
  virtual void setViewing(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_viewing",args,ret);
  }
  virtual void setHeadlight(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_head_light",args,ret);
  }
  virtual void setSmoothing(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_smoothing",args,ret);
  }
  virtual void setTitle(const std::string& aValue){
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(aValue);
    void* ret;
    fViewerProxy(fViewer,"set_title",args,ret);
  }
  virtual void render() {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    void* ret;
    fViewerProxy(fViewer,"render",args,ret);
  }
  virtual void viewAll() {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    void* ret;
    fViewerProxy(fViewer,"view_all",args,ret);
  }
  virtual bool readScene(const std::string& aFile,
                         const std::string& aFormat,
                         const std::string& aPlacement){
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    args.push_back(aFile);
    args.push_back(aFormat);
    args.push_back(aPlacement);
    void* ret;
    return fViewerProxy(fViewer,"read_scene",args,ret);
  }
  virtual bool clear(const std::string& aWhat){
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    args.push_back(aWhat);
    void* ret;
    return fViewerProxy(fViewer,"clear",args,ret);
  }
  virtual bool removeManips(const std::string&){
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    void* ret;
    return fViewerProxy(fViewer,"remove_manips",args,ret);
  }
  virtual bool detectIntersections(bool stopAtFirst,
                                   const std::string& aOptions) {
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    args.push_back(inlib::tos(stopAtFirst));
    args.push_back(aOptions);
    void* ret;
    return fViewerProxy(fViewer,"detect_intersections",args,ret);
  }
  virtual SoNode* nodeOfInterest(const std::string& aWhat) const {
    if(!fViewerProxy) return 0;
    std::vector<std::string> args;
    args.push_back(aWhat);
    void* ret;
    if(!fViewerProxy(fViewer,"interest",args,ret)) return 0;
    return (SoNode*)ret;
  }
  virtual SbViewportRegion* viewportRegion() const {
    if(!fViewerProxy) return 0;
    std::vector<std::string> args;
    void* ret;
    fViewerProxy(fViewer,"viewport_region",args,ret);
    return (SbViewportRegion*)ret;
  }
  virtual SoGLRenderAction* getGLRenderAction() const {
    if(!fViewerProxy) return 0;
    std::vector<std::string> args;
    void* ret;
    if(!fViewerProxy(fViewer,"getGLRenderAction",args,ret)) return 0;
    return (SoGLRenderAction*)ret;
  }
  virtual void setGLRenderAction(SoGLRenderAction* a_action) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::p2s(a_action));
    void* ret;
    fViewerProxy(fViewer,"setGLRenderAction",args,ret);
  }
  virtual bool setTransparencyType(int a_type) {
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    args.push_back(inlib::tos(a_type));
    void* ret;
    return fViewerProxy(fViewer,"setTransparencyType",args,ret);
  }
  virtual bool transparencyType(int& a_type) const {
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    void* ret;
    if(!fViewerProxy(fViewer,"transparencyType",args,ret)) return false;
    int* i = (int*)ret;
    a_type = *i;
    delete i;
    return true;
  }
  virtual bool collect(int a_x,int a_y,unsigned int a_w,unsigned int a_h) {
    if(!fViewerProxy) return false;
    std::vector<std::string> args;
    args.push_back(inlib::tos(a_x));
    args.push_back(inlib::tos(a_y));
    args.push_back(inlib::tos(a_w));
    args.push_back(inlib::tos(a_h));
    void* ret;
    return fViewerProxy(fViewer,"collect",args,ret);
  }

  virtual void setFeedbackVisibility(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_feedback_visibility",args,ret);
  }
  virtual void setAnimation(bool aValue) {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    args.push_back(inlib::tos(aValue));
    void* ret;
    fViewerProxy(fViewer,"set_animation",args,ret);
  }
  virtual void stopAnimating() {
    if(!fViewerProxy) return;
    std::vector<std::string> args;
    void* ret;
    fViewerProxy(fViewer,"stop_animating",args,ret);
  }
public: //inlib::xml::tree
  virtual bool invalidate() {
    OnX::Widget::invalidate();
    fSoNode = 0;
    fViewer = 0;
    fViewerProxy = 0;
    return true;
  }
public:
  SoViewer(Slash::UI::IUI& aUI,
           inlib::xml::factory& aFactory,
           inlib::xml::tree* aParent,
           Slash::UI::IWidgetClass& aWidgetClass)
  :OnX::Widget(aUI,aFactory,aParent,aWidgetClass)
  ,fSoNode(0)
  ,fViewer(0)
  ,fViewerProxy(0)
  {}
  virtual ~SoViewer(){}
public:
  void setSceneGraph(SoNode* aNode) { fSoNode = aNode;}
  void setViewerProxy(void* aViewer,Proxy aProxy) { 
    fViewer = aViewer;
    fViewerProxy = aProxy;
  }
private:
  SoNode* fSoNode;
  void* fViewer;
  Proxy fViewerProxy;
};

  /*
class ScrolledTree : public inlib::xml::tree, public virtual IScrolledTree {
public: //Slash::UI::IWidget
  virtual Slash::UI::IWidgetClass& widgetClass() const { return fWidgetClass;}
  virtual std::string name() const { 
    std::string value;
    attribute_value("","name",value);
    return value;
  }
  virtual void* nativeWidget() const {return get_data1();}
public: //Slash::UI::IScrolledTree
  virtual std::vector<std::string> children(const std::string&) {}
public: //IScrolledTree
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnX::ScrolledTree)
    else INLIB_IF_CAST(IScrolledTree)
    else INLIB_IF_CAST(Slash::UI::IWidget)
    else return inlib::xml::tree::cast(a_class);
  }
public:
  ScrolledTree(inlib::xml::factory& aFactory,
               inlib::xml::tree* aParent,
               Slash::UI::IWidgetClass& aWidgetClass)
  :inlib::xml::tree("widget",aFactory,aParent)
  ,fWidgetClass(aWidgetClass) {
  virtual ~ScrolledTree(){
  }
private:
  Slash::UI::IWidgetClass& fWidgetClass;
};

class OpenGLArea : public inlib::xml::tree, public virtual IOpenGLArea {
public: //Slash::UI::IWidget
  virtual Slash::UI::IWidgetClass& widgetClass() const { return fWidgetClass;}
  virtual std::string name() const { 
    std::string value;
    attribute_value("","name",value);
    return value;
  }
  virtual void* nativeWidget() const {return get_data1();}
  virtual void refresh() {}
public: //IOpenGLArea
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnX::OpenGLArea)
    else INLIB_IF_CAST(IOpenGLArea)
    else INLIB_IF_CAST(Slash::UI::IWidget)
    else return inlib::xml::tree::cast(a_class);
  }
public:
  OpenGLArea(inlib::xml::factory& aFactory,
             inlib::xml::tree* aParent,
             Slash::UI::IWidgetClass& aWidgetClass)
  :inlib::xml::tree("widget",aFactory,aParent)
  ,fWidgetClass(aWidgetClass) {
  virtual ~OpenGLArea(){
  }
private:
  Slash::UI::IWidgetClass& fWidgetClass;
};
  */

}

#endif
