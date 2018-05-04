
#include <Slash/Tools/Inventor.h>

#include <inlib/args>

// HEPVis :
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/SbPlottableThings.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_plotter_rebuild(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  soPlotter->raiseAllUpdateFlags();
  soPlotter->updateChildren();
}
//////////////////////////////////////////////////////////////////////////////
void OnX_plotter_set_tick_modeling(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  soPlotter->setAxesModeling(aArgs[1].c_str());
}
//////////////////////////////////////////////////////////////////////////////
void OnX_plotter_set_style(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  //SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  soPlotterRegion->setStyle(aArgs[1].c_str());
}
//////////////////////////////////////////////////////////////////////////////
void OnX_plotter_dump(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  
  std::ostream& out = aUI.session().cout();

#define Cast(aObject,aClass) (aClass*)(aObject).cast(#aClass)

  const SbPList& list = soPlotter->getPlottableObjects();
  int objn = list.getLength();

  out << "Number of plottable object " << objn << " :" << std::endl;

  for(int index=0;index<objn;index++) {
    SbPlottableObject* object = (SbPlottableObject*)list[index];
    out << " plottable object " << object << " :" << std::endl;
    if(!object) continue; 
    if(!object->isValid()) {
      out << "   valid = no" << std::endl;
      continue; 
    }
    if(Cast(*object,SbPlottableBins1D)) {
      out << "   class = SbPlottableBins1D" << std::endl;
    } else if(Cast(*object,SbPlottableBins2D)) {
      out << "   class = SbPlottableBins2D" << std::endl;
    } else if(Cast(*object,SbPlottablePoints2D)) {
      out << "   class = SbPlottablePoints2D" << std::endl;
    } else if(Cast(*object,SbPlottablePoints3D)) {
      out << "   class = SbPlottablePoints3D" << std::endl;
    } else if(Cast(*object,SbPlottableFunction1D)) {
      out << "   class = SbPlottableFunction1D" << std::endl;
    } else if(Cast(*object,SbPlottableFunction2D)) {
      out << "   class = SbPlottableFunction2D" << std::endl;
    } else {
      out << "   class = unknown" << std::endl;
    }
    out << "   name  = " << std::string(object->getName()) << std::endl;
  }


}

} // extern "C"
