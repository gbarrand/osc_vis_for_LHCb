#ifndef Slash_Tools_DummySceneHandler_h
#define Slash_Tools_DummySceneHandler_h

// inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/ISceneHandler.h>

#include <inlib/cast>

namespace Slash {

class DummySceneHandler
:public virtual Slash::Core::IManager 
,public virtual Slash::UI::ISceneHandler {

public: //IManager
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::UI::ISceneHandler)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //ISceneHandler

  virtual bool setViewer(const std::string& aViewer) {return false;}

  virtual bool addScene(const std::string& aPlacement,SoNode* aWhat) {
    return false;
  }

  virtual bool clear(const std::string& aViewer,
                     const std::string& aPlacement) {return false;}

  virtual SoNode* find(const std::string& aViewer,
                       const std::string& aPlacement) {return 0;}

public:
  inline DummySceneHandler(const std::string& aName):fName(aName){}

  virtual ~DummySceneHandler() {}
protected:
  inline DummySceneHandler(const DummySceneHandler& aFrom)
  :fName(aFrom.fName)
  {}
private:
  inline DummySceneHandler& operator=(const DummySceneHandler&){
    return *this;
  }
protected:
  std::string fName;
};

}

#endif
