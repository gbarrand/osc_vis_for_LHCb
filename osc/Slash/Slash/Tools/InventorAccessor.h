#ifndef Slash_Tools_InventorAccessor_h
#define Slash_Tools_InventorAccessor_h

// Inheritance :
#include "BaseAccessor.h"
#include <Slash/Data/IVisualizer.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IStyle.h>
#include <Slash/UI/ISceneHandler.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Session.h> //style_from_modeling

#include <HEPVis/misc/SoGC.h> //FIXME : over SbStyle only ?

class SoNode;

#include <inlib/args>
#include <inlib/cast>

namespace Slash {

class InventorAccessor 
:public BaseAccessor
,public virtual Slash::Data::IVisualizer {
public: //Slash::Data::IVisualizer.
  virtual void beginVisualize(const Slash::UI::IStyle& aStyle) {
    fBegVisStyle = aStyle.toString();
    begin_visualize();
  }
  virtual void beginVisualize() {
    fBegVisStyle.clear();
    begin_visualize();
  }

  //virtual void visualize(Slash::Data::IAccessor::Data,void*){
  //  Still pure virtual.
  //}
  virtual void endVisualize() {
    fSoGC.clear(); 
    fBegVisStyle.clear();
  }
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::InventorAccessor)
    else INLIB_IF_CAST(Slash::Data::IVisualizer)
    else {
      return BaseAccessor::cast(a_class);
    }
  }
public:
  inline InventorAccessor(Slash::Core::ISession& aSession,
                          Slash::UI::ISceneHandler& aSH,
                          const std::string& aStyleFile = "")
  : BaseAccessor(aSession.out())
  ,fSession(aSession)
  ,fSH(aSH)
  ,fStyleManager(0)
  {
    fStyleManager = Slash::styleManager(fSession);
    if(fStyleManager && (aStyleFile.size()) ) {
      fStyleManager->loadFile(aStyleFile);
    }
  }
  virtual ~InventorAccessor() {}
protected:
  inline InventorAccessor(const InventorAccessor& aFrom)
  : BaseAccessor(aFrom)
  ,fSession(aFrom.fSession)
  ,fSH(aFrom.fSH)
  ,fStyleManager(0)
  {}
private:
  inline InventorAccessor& operator=(const InventorAccessor&){return *this;}
public:
  inline bool loadStyleFile(const std::string& aStyleFile) {
    if(!fStyleManager) return false;
    return fStyleManager->loadFile(aStyleFile);
  }
  inline bool isStyle(const std::string& aStyle) const {
    if(!fStyleManager) return false;
    return fStyleManager->isStyle(aStyle);
  }
protected:
  inline void begin_visualize() {
    const std::vector<std::string>& eargs = fExecuteArgs;
    inlib::args args(eargs);
    args.find("where",fWhere);
    args.find("style",fExecArgStyle);

    fSoGC.setDefaultStyleCache();
    fSH.setViewer(fWhere);
  }

  inline void fillSoGC(const std::string& aStyle = "") {
    fSoGC.reset();

    std::string value;

    fSession.parameterValue("modeling.localSetup",value); //used in G4Lab.
    if((value.size())&&(value!="none")) {
      bool b;
      if(inlib::to(value,b)) {
        fSoGC.setLocalSetup(b?TRUE:FALSE);
      }
    }

    // Style priority :
    // - fBegVisStyle set from IStyle arg in beginVisualize(IStyle)
    // - fExecArgStyle set from fExecuteArgs in beginVisualize()
    //   and coming from an argument "style=<name>" to the data_visualize.
    // - aStyle coming as argument.
    // - a style named name().
    // - style coming from session modeling.<xxx> parameters.

    if(fBegVisStyle.size()) {
      fSoGC.setFromString(SbString(fBegVisStyle.c_str()));
      return;
    }

    if(fExecArgStyle.size()) {
      if(fStyleManager && fStyleManager->findStyle(fExecArgStyle,value)) { 
        fSoGC.setFromString(SbString(value.c_str()));
        return;
      }
    }

    if(aStyle.size()) {
      if(fStyleManager && fStyleManager->findStyle(aStyle,value)) { 
        fSoGC.setFromString(SbString(value.c_str()));
        return;
      }
    }

    if(name().size()) {
      if(fStyleManager && fStyleManager->findStyle(name(),value)) { 
        fSoGC.setFromString(SbString(value.c_str()));
        return;
      }
    }

    value = Slash::style_from_modeling(fSession);
    fSoGC.setFromString(SbString(value.c_str()));

  }
  inline SoGC& soGC() { return fSoGC;}
  inline bool addScene(SoNode* aNode,const std::string& aWhere) {
    return fSH.addScene(aWhere,aNode);
  }
  inline std::string where() const { return fWhere;}
protected:
  Slash::Core::ISession& fSession;
  Slash::UI::ISceneHandler& fSH;
  Slash::UI::IStyleManager* fStyleManager;
protected:
  SoGC fSoGC;
  std::string fWhere;
  std::string fBegVisStyle;
  std::string fExecArgStyle;
private:
  static void check_instantiation(){
    class X : public InventorAccessor {
    public:
      virtual std::string name() const {return "";}
      virtual Core::IValue* findValue(Data,const std::string&,void*) {
        return 0;
      }
      virtual void visualize(Slash::Data::IAccessor::Data,void*) {}
    public:
      X(Slash::Core::ISession& aSession,Slash::UI::ISceneHandler& aSH)
      : InventorAccessor(aSession,aSH){}
    };
    Slash::Core::ISession* s;
    Slash::UI::ISceneHandler* sh;
    X o(*s,*sh);
  }  
};

}



#endif
