//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Inventor.h>

#include <inlib/args>

// Inventor :
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>

// HEPVis :
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>

extern "C" {

void OnXTemplateFour_init(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] the widget to initialize.
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;

  // Get the current SoPage :
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  // Make the page title invisible :
  soPage->titleVisible.setValue(FALSE);

  // Create in the SoPage a SoRegion ready for 3D :
  soPage->createRegions("SoDisplayRegion",1,1,0);

  // Change the background of the region :
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  soRegion->color.setValue(0.3F,0.3F,0.3F);
}

void OnXTemplateFour_inventor_cube(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] the widget to visualize into.
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;

  // Get the current SoPage :
  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  // Get the SoDisplayRegion :
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  SoDisplayRegion* soDisplayRegion = static_cast<SoDisplayRegion*>(soRegion);
  if(!soDisplayRegion) return;
  
  //SoSeparator* getDynamicScene() const;

  // Get the display region "static scene" entry separator.
  // In general the stataic scene is for landscape things
  // (for example a detector in HEP) :
  SoSeparator* soSeparator = soDisplayRegion->getStaticScene();

  // Some user scene :
  SoCube* soCube = new SoCube;
  soSeparator->addChild(soCube);

}

} // extern "C"
