#ifndef Slash_Tools_Inventor_h
#define Slash_Tools_Inventor_h

#include "UI.h"

// HEPVis :
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>

namespace Slash {

inline SoPage* find_SoPage(Slash::UI::IUI& aUI,const std::string& aString) {
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aString);
  if(!soViewer) return 0;
  SoNode* soNode = soViewer->sceneGraph();
  if(!soNode) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoPage : "
        << " viewer " << inlib::sout(aString) << " has no scene graph."
        << std::endl;
    return 0;
  }
  if(!soNode->isOfType(SoPage::getClassTypeId())) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoPage : "
        << " node of " << inlib::sout(aString) << " is not an SoPage."
        << " It is a " 
        << inlib::sout(std::string(soNode->getTypeId().getName().getString()))
        << "." 
        << std::endl;
    return 0;
  }
  return (SoPage*)soNode;    
}

inline SoRegion* find_SoRegion(Slash::UI::IUI& aUI,const std::string& aString,SoPage** aPage = 0) {
  std::string widget = aString;
  std::string region;
  if(aString!=s_current()) {
    std::string::size_type pos = aString.rfind('@');  
    if(pos!=std::string::npos) { 
      //aString = "<string>@<int>" (but not @current@)
      //aString = @current@@<int> works too.
      widget = aString.substr(0,pos);
      region = aString.substr(pos+1,aString.size()-(pos+1));
    }
  }
  SoPage* soPage = find_SoPage(aUI,widget);
  if(!soPage) return 0;
  SoNode* soNode;
  if(region.empty()) {
    soNode = soPage->currentRegion();
  } else {
    int index;
    if(!inlib::to<int>(region,index)) {
      std::ostream& out = aUI.session().cout();
      out << "Slash::find_SoRegion : "
          << " " << inlib::sout(region) 
          << " is not an int."
          << std::endl;
      return 0;
    }
    soNode = soPage->getRootRegion(index);
  }
  if(!soNode) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoRegion : "
        << " region " << inlib::sout(aString) 
        << " not found."
        << std::endl;
    return 0;
  }
  if(aPage) *aPage = soPage;
  return (SoRegion*)soNode;
}

inline SoNode* find_SoNode(Slash::UI::IUI& aUI,const std::string& aString,const std::string& aInterest) {
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aString);
  if(!soViewer) return 0;
  SoNode* soNode = soViewer->nodeOfInterest(aInterest);
  if(!soNode) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoNode : "
        << " failed for " << inlib::sout(aInterest) << "."
        << std::endl;
    return 0;
  }
  return soNode;
}

inline SoPlotter* find_SoPlotter(Slash::UI::IUI& aUI,const std::string& aString) {
  SoRegion* soRegion = find_SoRegion(aUI,aString);
  if(!soRegion) return 0;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoPlotter : "
        << " region is not a SoPlotterRegion."
        << std::endl;
    return 0;
  }
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  if(!soPlotter) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoRegion : "
        << " SoPlotter not found."
        << std::endl;
    return 0;
  }
  return soPlotter;
}

}

#endif
