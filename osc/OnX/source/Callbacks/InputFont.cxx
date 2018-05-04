//
//  All functions here should be InputPlotter callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

#include <Slash/Tools/Inventor.h>

#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoAxis.h>
#include <HEPVis/nodekits/SoAxes2D.h>
#include <HEPVis/nodekits/SoAxes3D.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoInfosRegion.h>
#include <HEPVis/nodekits/SoTextRegion.h>
#include <HEPVis/nodekits/SoPlotter.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_InputFont_ok(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string fontName = "unchanged";
  if(!aUI.parameterValue("OnX_InputFont_input_fontName.value",fontName)) return;

  bool smoothing;
  if(!Slash::parameter(aUI,"OnX_InputFont_input_smoothing.value",smoothing)) return;
  
  bool hinting;
  if(!Slash::parameter(aUI,"OnX_InputFont_input_hinting.value",hinting)) return;
  
  SoPage* soPage = Slash::find_SoPage(aUI,Slash::s_current());
  if(!soPage) return;
  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;

  SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  soPlotter->setFont(fontName.c_str(),smoothing?TRUE:FALSE,hinting?TRUE:FALSE);

  std::vector<std::string> words;
  inlib::words(fontName,"/",false,words);
  if((words.size()==2)&&(words[0]=="TTF")) {
    
    soPage->getTitleRegion()->getStyle(0)->fontName.setValue(words[1].c_str());  
    soPage->getTitleRegion()->getStyle(0)->smoothing.setValue(smoothing?TRUE:FALSE);  
    soPage->getTitleRegion()->getStyle(0)->hinting.setValue(hinting?TRUE:FALSE);  

   {SoTextStyle* soStyle = soPlotterRegion->getInfosRegion()->getTextStyle();
    soStyle->fontName.setValue(words[1].c_str());
    soStyle->smoothing.setValue(smoothing?TRUE:FALSE);
    soStyle->hinting.setValue(hinting?TRUE:FALSE);}

    SoTextStyle* soStyle = soPlotterRegion->getTitleRegion()->getStyle(0);
    if(soStyle) {
      soStyle->fontName.setValue(words[1].c_str());
      soStyle->smoothing.setValue(smoothing?TRUE:FALSE);
      soStyle->hinting.setValue(hinting?TRUE:FALSE);
    }
  }

}

} // extern "C"
