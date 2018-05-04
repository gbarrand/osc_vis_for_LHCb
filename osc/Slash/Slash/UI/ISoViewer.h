#ifndef Slash_UI_ISoViewer_h
#define Slash_UI_ISoViewer_h

/**
 * @author G.Barrand
 */

#include <string>
#include <vector>

class SoNode; //Ok since a forward cast does not tie to a library.
class SbViewportRegion; //Ok since a forward cast does not tie to a library.
class SoGLRenderAction;

namespace Slash {

namespace UI {

class ISoViewer {
public:
  virtual ~ISoViewer() {}
public: 
  //So<GUI>Component :
  virtual void setTitle(const std::string&) = 0;

  //So<GUI>RenderArea :
  virtual void setBackgroundColor(double red,double green,double blue) = 0;
  virtual void render() = 0;
  virtual SbViewportRegion* viewportRegion() const = 0;
  virtual SoNode* sceneGraph() const = 0;
  virtual SoGLRenderAction* getGLRenderAction() const = 0;
  virtual void setGLRenderAction(SoGLRenderAction*) = 0;

  // From coin3d/SoGLRenderAction.h
  //enum TransparencyType {
  //  SCREEN_DOOR,
  //  ADD, DELAYED_ADD, SORTED_OBJECT_ADD,
  //  BLEND, DELAYED_BLEND, SORTED_OBJECT_BLEND,
  //  // The remaining are Coin extensions to the common Inventor API
  //  SORTED_OBJECT_SORTED_TRIANGLE_ADD,
  //  SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
  //  NONE, SORTED_LAYERS_BLEND
  //};
  virtual bool setTransparencyType(int) = 0;
  virtual bool transparencyType(int&) const = 0;

  //So<GUI>Viewer :
  virtual void setHeadlight(bool) = 0;
  virtual void viewAll() = 0;
  virtual void setViewing(bool) = 0;
  virtual void setAutoClipping(bool) = 0;

  //So<GUI>FullViewer :
  virtual void setDecoration(bool) = 0;

  //So<GUI>ExaminerViewer :
  virtual void setAnimation(bool) = 0;
  virtual void stopAnimating() = 0;
  virtual void setFeedbackVisibility(bool) = 0;

  // else :
  virtual void setSmoothing(bool) = 0;
  virtual bool readScene(const std::string& file,
                         const std::string& format,
                         const std::string& placement) = 0;
  virtual bool clear(const std::string& opts = "") = 0;
  virtual bool removeManips(const std::string& opts = "") = 0;
  virtual bool detectIntersections(bool stopAtFirst,
                                   const std::string& opts = "") = 0;
  virtual bool collect(int,int,unsigned int,unsigned int) = 0;

  virtual SoNode* nodeOfInterest(const std::string& what) const = 0;
};

} //UI

} //Slash

#endif
