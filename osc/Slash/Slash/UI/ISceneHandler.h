#ifndef Slash_UI_ISceneHandler_h
#define Slash_UI_ISceneHandler_h

#include <string>

class SoNode; //forward cast, ok.

namespace Slash {
namespace UI {

class ISceneHandler {
public:
  virtual ~ISceneHandler() {};
public:
  virtual bool setViewer(const std::string& viewer) = 0;
  virtual bool addScene(const std::string& placement,SoNode* what) = 0;

  virtual bool clear(const std::string& viewer,
                     const std::string& placement) = 0;
  virtual SoNode* find(const std::string& viewer,
                       const std::string& placement) = 0;
};

}}

#endif
