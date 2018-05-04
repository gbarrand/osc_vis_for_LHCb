//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

#include <Slash/Tools/Inventor.h>

// HEPVis :
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>
#include <HEPVis/nodekits/SoRulerRegion.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_ruler(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::UI::ISoViewer* soViewer = 
    Slash::find_SoViewer(aUI,Slash::s_current());
  if(!soViewer) return;
  soViewer->setBackgroundColor(1,1,1);
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;

  soPage->deleteRegions();
  soPage->titleVisible.setValue(FALSE);

  float w = 0.9F;
  float h = 0.9F;

  // Visualization region :
  SoDisplayRegion* displayRegion = 
    (SoDisplayRegion*)soPage->createRegion("SoDisplayRegion",
                                           1-w,1-h,w,h);
  if(!displayRegion) return;
  soPage->setCurrentRegion(displayRegion);
  displayRegion->getViewportRegion()->backgroundColor.setValue(SbColor(0,0,0));

  // Horizontal ruler region :
  SoRulerRegion* hRuler = 
    (SoRulerRegion*)soPage->createRegion("SoRulerRegion",
                                         1-w,0,w,1-h);
  if(!hRuler) return;
  hRuler->getViewportRegion()->backgroundColor.setValue(SbColor(1,1,1));

  // Connect the display region to the ruler :
  hRuler->setMeasuredRegion(displayRegion);

  // Vertical ruler region :
  SoRulerRegion* vRuler = 
    (SoRulerRegion*)soPage->createRegion("SoRulerRegion",
                                         0,1-h,1-w,h);
  if(!vRuler) return;
  vRuler->getViewportRegion()->backgroundColor.setValue(SbColor(1,1,1));

  // Connect the display region to the ruler :
  vRuler->setMeasuredRegion(displayRegion);

  // Set display region as current :
  soPage->setCurrentRegion(0);

}

} // extern "C"
