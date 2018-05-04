#ifndef Slash_Tools_RegionSceneHandler_h
#define Slash_Tools_RegionSceneHandler_h

// inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/UI/ISceneHandler.h>

#include <Slash/Tools/Inventor.h>

#include <Inventor/nodes/SoSeparator.h>

namespace Slash {

class RegionSceneHandler
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

  virtual bool setViewer(const std::string& aViewer) {
    Slash::UI::IUI* ui = Slash::find_UI(fSession);
    if(!ui) {fSoRegion = 0;return false;}
    fSoRegion = Slash::find_SoRegion(*ui,aViewer);
    return fSoRegion?true:false;
  }

  virtual bool addScene(const std::string& aPlacement,SoNode* aWhat) {
    if(!fSoRegion) return false;
    SbAddNode sb(aWhat,aPlacement.c_str());
    bool status = fSoRegion->doIt(sb)==TRUE?true:false;
    sb.reset(); //to disconnect dstor unref.
    return status;
  }

  virtual bool clear(const std::string& aViewer,
                     const std::string& aPlacement) {
    Slash::UI::IUI* ui = Slash::find_UI(fSession);
    if(!ui) return false;
    SoRegion* soRegion = Slash::find_SoRegion(*ui,aViewer);
    if(!soRegion) return false;
    soRegion->clear(aPlacement.c_str());
    return true;
  }

  virtual SoNode* find(const std::string& aViewer,
                       const std::string& aPlacement) {
    //used in OnX/SceneGraphAccessor.
    Slash::UI::IUI* ui = Slash::find_UI(fSession);
    if(!ui) return 0;
    SoRegion* soRegion = Slash::find_SoRegion(*ui,aViewer);
    if(!soRegion) return 0;
    if(aPlacement==Slash_topSeparator) return soRegion->getTopSeparator();
    return soRegion;
  }

public:
  inline RegionSceneHandler(Slash::Core::ISession& aSession,
                            const std::string& aName)
  :fSession(aSession)
  ,fName(aName)
  ,fSoRegion(0)
  {}

  virtual ~RegionSceneHandler() {}
protected:
  inline RegionSceneHandler(const RegionSceneHandler& aFrom)
  :fSession(aFrom.fSession),fName(aFrom.fName)
  {}
private:
  inline RegionSceneHandler& operator=(const RegionSceneHandler&){
    return *this;
  }
protected:
  Slash::Core::ISession& fSession;
  std::string fName;
  SoRegion* fSoRegion;
};

}

#endif
