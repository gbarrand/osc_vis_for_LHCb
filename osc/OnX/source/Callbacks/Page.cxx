//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

//
// HEPVis and Inventor related callbacks dealing with an SoPage.
//

#include <Slash/Tools/Inventor.h>

#include <inlib/sjust>
#include <inlib/args>

#include "inlib"

#include <Inventor/errors/SoDebugError.h>

#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>
#include <HEPVis/nodekits/SoRulerRegion.h>

static void page_dressCurrentRegionWithRulers(SoPage& aPage,double aRulerSize = 0.9) {
  SoRegion* soRegion = aPage.currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) {
    SoDebugError::postInfo("page_dressCurrentRegionWithRulers",
      "current region is not a SoDisplayRegion.");
    return;
  }

  float w = float(aRulerSize);
  float h = float(aRulerSize);

  // Visualization region :
  SoViewportRegion* vpr = soRegion->getViewportRegion();
  vpr->setPositionPercent(1-w,1-h);
  vpr->setSizePercent(w,h);

  // Horizontal ruler region :
  SoRulerRegion* hRuler = 
    (SoRulerRegion*)aPage.createRegion("SoRulerRegion",1-w,0,w,1-h);
  if(!hRuler) return;
  hRuler->getViewportRegion()->backgroundColor.setValue(SbColor(1,1,1));

  // Connect the display region to the ruler :
  hRuler->setMeasuredRegion(soRegion);

  // Vertical ruler region :
  SoRulerRegion* vRuler = 
    (SoRulerRegion*)aPage.createRegion("SoRulerRegion",0,1-h,1-w,h);
  if(!vRuler) return;
  vRuler->getViewportRegion()->backgroundColor.setValue(SbColor(1,1,1));

  // Connect the display region to the ruler :
  vRuler->setMeasuredRegion(soRegion);

  // Set display region as current :
  aPage.highlightRegion(soRegion);

}

// Find rulers and remove them.
// Map the first SoDisplayRegion to the page.
static void page_removeRulers(SoPage& aPage) {
  SoRegion* soRegion = aPage.currentRegion();
  if(!soRegion) return;
  // Map first display region to page.
  std::vector<SoRegion*> torm;
 {int number = aPage.getNumberOfRootRegions();
  for(int index=0;index<number;index++) {
    SoRegion* soRegion = aPage.getRootRegion(index);
    if(soRegion->isOfType(SoDisplayRegion::getClassTypeId())) {
      SoViewportRegion* vpr = soRegion->getViewportRegion();
      vpr->setPositionPercent(0,0);
      vpr->setSizePercent(1,1);
    } else if(soRegion->isOfType(SoRulerRegion::getClassTypeId())) {
      torm.push_back(soRegion);
    }
  }}
 {unsigned int number = torm.size();
  for(unsigned int index=0;index<number;index++) {
    aPage.deleteRegion(torm[index]);
  }}
}

typedef std::vector<std::string> args_t;

extern "C" {

//////////////////////////////////////////////////////////////////////////////
/// page /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_page_create_region(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_min_args(aArgs,6,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  std::ostream& out = aUI.session().cout();

  const std::string& sclass = aArgs[1];

  double x;
  if(!inlib::to<double>(out,aArgs[2],x)) return;
  double y;
  if(!inlib::to<double>(out,aArgs[3],y)) return;
  double w;
  if(!inlib::to<double>(out,aArgs[4],w)) return;
  double h;
  if(!inlib::to<double>(out,aArgs[5],h)) return;

  SoRegion* soRegion = 
    soPage->createRegion(sclass.c_str(),(float)x,(float)y,(float)w,(float)h);
  if(!soRegion) {
    std::ostream& out = aUI.session().cout();
    out << "page_create_region :"
        << " SoPage.createRegion failed."
        << std::endl;
    return;
  }

  bool set_current = true;

  if(aArgs.size()>=7) {
    if(!inlib::to(aUI.session().cout(),aArgs[6],set_current)) {
      set_current = true;
    }
  }

  if(set_current) {
    if(soPage->setCurrentRegion(soRegion)==FALSE) {
      std::ostream& out = aUI.session().cout();
      out << "page_create_region :"
          << " can't set current region."
          << std::endl;
      return;
    }
  }
}

void OnX_page_create_regions(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // args[1] wanted region class
  // args[2] cols
  // args[3] rows
  // args[4] index of wanted current region
  if(!inlib::check_args(aArgs,5,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  std::ostream& out = aUI.session().cout();

  const std::string& sclass = aArgs[1];

  int cols;
  if(!inlib::to<int>(out,aArgs[2],cols)) return;
  int rows;
  if(!inlib::to<int>(out,aArgs[3],rows)) return;
  int index;
  if(!inlib::to<int>(out,aArgs[4],index)) return;

  soPage->createRegions(sclass.c_str(),cols,rows,index);
}

void OnX_page_add_regions(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // args[1] wanted region class
  // args[2] cols
  // args[3] rows
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,4,out)) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  int cols;
  if(!inlib::to<int>(out,aArgs[2],cols)) return;
  int rows;
  if(!inlib::to<int>(out,aArgs[3],rows)) return;

  soPage->addRegions(aArgs[1].c_str(),cols,rows);
}

void OnX_page_set_title(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // args[1,n] title
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  soPage->title.setValue(last.c_str());
}

void OnX_page_title_visible(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // args[1] [true,false] visible or not
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,2,out)) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  bool visible;
  if(!inlib::to(out,aArgs[1],visible)) return;
  soPage->titleVisible.setValue(visible?TRUE:FALSE);
  soPage->updateChildren();
}

void OnX_page_delete_regions(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  soPage->deleteRegions();
}

void OnX_page_delete_region(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  SoRegion* highlighted = soPage->getHighlightedRegion();
  if(!highlighted) return;
  if(soPage->isRootRegion(highlighted)==FALSE) return;
  soPage->deleteHighlightedRegion();
}

void OnX_page_set_current_region(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,2,out)) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  int index;
  if(!inlib::to<int>(out,aArgs[1],index)) return;
  if(soPage->setCurrentRegion(index)==FALSE) {
    std::ostream& out = aUI.session().cout();
    out << "page_set_current_region :"
        << " can't set region " << index << " to current region."
        << std::endl;
    return;
  }
}

void OnX_page_set_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] viewer
  // args[1] what
  // args[2,n] value
  std::string last;
  if(!inlib::check_min(aArgs,3,last,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  SoTools::setKitPart(soPage,aArgs[1].c_str(),last.c_str());
}

void OnX_page_toggle_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] viewer
  // args[1] what
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  SoTools::toggleKitPart(soPage,aArgs[1].c_str());
}

void OnX_page_set_root_regions_highlightable(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // Highlight mechanism on regions may be disconnected (for exa in OpenPAW).
  // The below restore it.
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;
  bool b;
  if(!inlib::to(aUI.session().cout(),aArgs[1],b)) return;
  soPage->setRootRegionsHighlightable(b?TRUE:FALSE);
}

void OnX_page_clear(Slash::UI::IUI& aUI,const args_t& aArgs){
  SoPage* soPage = 0;
  if(!aArgs.size()) {
    std::ostream& out = aUI.session().cout();
    out << "OnX_page_clear :"
        << " no argument is deprecated."
        << " You have to pass the viewer name."
        << std::endl;
    soPage = Slash::find_SoPage(aUI,Slash::s_current());
  } else {
    soPage = Slash::find_SoPage(aUI,aArgs[0]);
  }
  if(!soPage) return;
  soPage->clearRegions();
}

void OnX_page_refresh(Slash::UI::IUI& aUI,const args_t& aArgs){
  SoPage* soPage = 0;
  if(!aArgs.size()) {
    std::ostream& out = aUI.session().cout();
    out << "OnX_page_refresh :"
        << " no argument is deprecated."
        << " You have to pass the viewer name."
        << std::endl;
    soPage = Slash::find_SoPage(aUI,Slash::s_current());
  } else {
    soPage = Slash::find_SoPage(aUI,aArgs[0]);
  }
  if(!soPage) return;
  soPage->refresh();
}

//////////////////////////////////////////////////////////////////////////////
/// FIXME : revisit the below to pass the widget as first argument ///////////
//////////////////////////////////////////////////////////////////////////////

void OnX_page_next(Slash::UI::IUI& aUI) {
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* highlighted = soPage->getHighlightedRegion();
  if(!highlighted) return;
  SoRegion* nextSoRegion = soPage->getNextRootRegion(highlighted);
  if(!nextSoRegion) return;
  soPage->highlightRegion(nextSoRegion);
}

void OnX_page_previous(Slash::UI::IUI& aUI) {
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* highlighted = soPage->getHighlightedRegion();
  if(!highlighted) return;
  SoRegion* prevSoRegion = soPage->getPreviousRootRegion(highlighted);
  if(!prevSoRegion) return;
  soPage->highlightRegion(prevSoRegion);
}

void OnX_page_currentRegionToPage(Slash::UI::IUI& aUI) {
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* highlighted = soPage->getHighlightedRegion();
  if(!highlighted) return;
  if(soPage->isRootRegion(highlighted)==FALSE) return;
  soPage->mapHighlightedRegionToPage();
}

void OnX_page_currentRegionOnTop(Slash::UI::IUI& aUI) {
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* highlighted = soPage->getHighlightedRegion();
  if(!highlighted) return;
  if(soPage->isRootRegion(highlighted)==FALSE) return;
  soPage->setRegionOnTop(highlighted);
}

void OnX_page_setParameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  if(!aArgs.size()) return;
  if(aArgs.size()==1) {
    SoTools::setKitPart(soPage,aArgs[0].c_str(),"");
  } else {
    std::string value = aArgs[1];
    for(unsigned int index=2;index<aArgs.size();index++) 
      value += " " + aArgs[index];
    SoTools::setKitPart(soPage,aArgs[0].c_str(),value.c_str());
  }
}

void OnX_page_toggleParameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what
  if(!aArgs.size()) return;
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoTools::toggleKitPart(soPage,aArgs[0].c_str());
}

void OnX_page_setRootRegionsHighlightable(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // Highlight mechanism on regions may be disconnected (for exa in OpenPAW).
  // The below restore it.
  if(!aArgs.size()) return;
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  bool b;
  if(!inlib::to(aUI.session().cout(),aArgs[0],b)) return;
  soPage->setRootRegionsHighlightable(b?TRUE:FALSE);
}

void OnX_page_dressCurrentRegionWithRulers(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] ruler size (default is 0.9)
  double rulerSize = 0.9;
  if(aArgs.size()) {
    if(!inlib::to<double>(aUI.session().cout(),aArgs[0],rulerSize)) return;
  }
  Slash::UI::ISoViewer* soViewer = 
    Slash::find_SoViewer(aUI,Slash::s_current());
  if(!soViewer) return;
  soViewer->setBackgroundColor(1,1,1);
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  page_dressCurrentRegionWithRulers(*soPage,rulerSize);
}

void OnX_page_removeRulers(Slash::UI::IUI& aUI) {
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  page_removeRulers(*soPage);
}

} // extern "C"


#include <Slash/Tools/Inventor.h>

#include <HEPVis/actions/SoCollectAction.h>

static void dump_nodes(SoCanvas& aCanvas,int aX,int aY,int aW,int aH,const SbViewportRegion& aVP,std::ostream& aOut){
  SoRegion* node = aCanvas.getHighlightedRegion();
  if(!node) return;
  SoCollectAction action(aVP);
  action.setSearchArea(aX,aY,aW,aH);
  action.apply(node);
  SoPathList& pathList = action.paths();
  int pathn = pathList.getLength();
  for(int index=0;index<pathn;index++) {
    SoFullPath* path = (SoFullPath*)pathList[index];
    SoNode* n = path->getTail();
    if(n) {        
      aOut << n->getTypeId().getName().getString() << std::endl;
    }
  }
}

extern "C" {

void OnX_page_rectangular_collect(Slash::UI::IUI& aUI,const args_t& aArgs)
{
  // args[0] widget name (or @current@)

  std::ostream& out = aUI.session().cout();
  out << "OnXExas_rectangular_collect :" 
      << " callback value : " << aUI.callbackValue()
      << std::endl;

  std::vector<std::string> words;
  inlib::words(aUI.callbackValue()," ",false,words);
  if(words.size()!=4) return;
  int x;
  if(!inlib::to<int>(words[0],x)) return;
  int y;
  if(!inlib::to<int>(words[1],y)) return;
  unsigned int w;
  if(!inlib::to<unsigned int>(words[2],w)) return;
  unsigned int h;
  if(!inlib::to<unsigned int>(words[3],h)) return;

  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ISoViewer* soViewer = Slash::find_SoViewer(aUI,aArgs[0]);
  if(!soViewer) return;

  SbViewportRegion* vpr = soViewer->viewportRegion();
  if(!vpr) return;

  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  dump_nodes(*soPage,x,y,w,h,*vpr,out);
}

}
