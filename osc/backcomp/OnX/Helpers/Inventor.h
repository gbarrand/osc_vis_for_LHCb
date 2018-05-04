#ifndef OnX_Helpers_Inventor_h
#define OnX_Helpers_Inventor_h

//
//  This file contains static helper methods related to Inventor.
// It is intended to be included in OnX Callbacks file
// to ease callbacks programming.
//

#include <OnX/Helpers/OnX.h> //find_SoViewer

// Slash :
//#include <Slash/UI/IUI.h>
//#include <Slash/UI/IWidget.h>
//#include <Slash/UI/ISoViewer.h>

// Lib :
//#include <Lib/Cast.h>

// HEPVis :
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>

//////////////////////////////////////////////////////////////////////////////
/// Slash::UI::ISoViewer /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline SoNode* find_SoNode(Slash::UI::IUI& aUI,const std::string& aString,const std::string& aInterest) {
  Slash::UI::ISoViewer* soViewer = ::find_SoViewer(aUI,aString);
  if(!soViewer) return 0;
  SoNode* soNode = soViewer->nodeOfInterest(aInterest);
  if(!soNode) {
    Lib::Out out(aUI.session().printer());
    out << "find_SoNode : "
        << " failed for " << Lib::sout(aInterest) << "."
        << Lib::endl;
    return 0;
  }
  return soNode;
}

inline SoPage* find_SoPage(Slash::UI::IUI& aUI,const std::string& aString) {
  Slash::UI::ISoViewer* soViewer = ::find_SoViewer(aUI,aString);
  if(!soViewer) return 0;
  SoNode* soNode = soViewer->sceneGraph();
  if(!soNode) {
    Lib::Out out(aUI.session().printer());
    out << "find_SoPage : "
        << " viewer " << Lib::sout(aString) << " has no scene graph."
        << Lib::endl;
    return 0;
  }
  if(!soNode->isOfType(SoPage::getClassTypeId())) {
    Lib::Out out(aUI.session().printer());
    out << "find_SoPage : "
        << " node of " << Lib::sout(aString) << " is not an SoPage."
        << " It is a " 
        << Lib::sout(std::string(soNode->getTypeId().getName().getString()))
        << "." 
        << Lib::endl;
    return 0;
  }
  return (SoPage*)soNode;    
}

inline SoRegion* find_SoRegion(Slash::UI::IUI& aUI,const std::string& aString,const std::string& aRegion = "",SoPage** aPage = 0) {
  SoPage* soPage = find_SoPage(aUI,aString);
  if(!soPage) return 0;
  SoNode* soNode = soPage->currentRegion();
  if(!soNode) {
    Lib::Out out(aUI.session().printer());
    out << "find_SoRegion : "
        << " viewer " << Lib::sout(aString) 
        << " has no region " << Lib::sout(aRegion) << "."
        << Lib::endl;
    return 0;
  }
  if(aPage) *aPage = soPage;
  return (SoRegion*)soNode;
}

// NOTE : the below is deprecated. Use the find_xxx functions.

inline Slash::UI::ISoViewer* ui_SoViewer(Slash::UI::IUI& aUI){
  Slash::UI::IWidget* widget = aUI.currentWidget();
  if(!widget) return 0;
  return Lib_Cast(*widget,Slash::UI::ISoViewer);
}

inline Slash::UI::ISoViewer* ui_SoViewer(Slash::UI::IWidget& aWidget){
  return Lib_Cast(aWidget,Slash::UI::ISoViewer);
}

inline SoPage* ui_SoPage(Slash::UI::IUI& aUI){
  Slash::UI::ISoViewer* soViewer = ui_SoViewer(aUI);
  if(!soViewer) return 0;
  SoNode* soNode = soViewer->sceneGraph();
  if(!soNode) return 0;
  if(!soNode->isOfType(SoPage::getClassTypeId())) return 0;
  return (SoPage*)soNode;    
}

inline SoRegion* ui_SoRegion(Slash::UI::IUI& aUI){
  SoPage* soPage = ui_SoPage(aUI);
  if(!soPage) return 0;
  return soPage->currentRegion();
}

inline SoPage* ui_SoPage(Slash::UI::IWidget& aWidget){
  Slash::UI::ISoViewer* soViewer = Lib_Cast(aWidget,Slash::UI::ISoViewer);
  if(!soViewer) return 0;
  SoNode* soNode = soViewer->sceneGraph();
  if(!soNode) return 0;
  if(!soNode->isOfType(SoPage::getClassTypeId())) return 0;
  return (SoPage*)soNode;    
}

#endif
