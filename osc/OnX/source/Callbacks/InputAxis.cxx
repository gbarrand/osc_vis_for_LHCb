
#include <Slash/Tools/Inventor.h>

#include <inlib/args>

// HEPVis :
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoAxis.h>

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_InputAxis_ok(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
//
// validate inputAxis values
//
// Input fields : 
//   bool auto
//   bool log
//   real amin
//   real amax
//   int steps
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotter* soPlotter = ((SoPlotterRegion*)soRegion)->getPlotter();

  /////////////////////////////
  // Get input fields values :
  /////////////////////////////

  bool xauto;
  if(!Slash::parameter(aUI,"OnX_InputAxis_x_automated.set",xauto)) return;
  float xmin;
  if(!Slash::parameter<float>(aUI,"OnX_InputAxis_X_minimumValue.value",xmin)) return;
  float xmax;
  if(!Slash::parameter<float>(aUI,"OnX_InputAxis_X_maximumValue.value",xmax)) return;
  bool xlog;
  if(!Slash::parameter(aUI,"OnX_InputAxis_X_logScale.set",xlog)) return;

  bool yauto;
  if(!Slash::parameter(aUI,"OnX_InputAxis_y_automated.set",yauto)) return;
  float ymin;
  if(!Slash::parameter<float>(aUI,"OnX_InputAxis_Y_minimumValue.value",ymin)) return;
  float ymax;
  if(!Slash::parameter<float>(aUI,"OnX_InputAxis_Y_maximumValue.value",ymax)) return;
  bool ylog;
  if(!Slash::parameter(aUI,"OnX_InputAxis_Y_logScale.set",ylog)) return;

  bool zauto;
  if(!Slash::parameter(aUI,"OnX_InputAxis_z_automated.set",zauto)) return;
  float zmin;
  if(!Slash::parameter<float>(aUI,"OnX_InputAxis_Z_minimumValue.value",zmin)) return;
  float zmax;
  if(!Slash::parameter<float>(aUI,"OnX_InputAxis_Z_maximumValue.value",zmax)) return;
  bool zlog;
  if(!Slash::parameter(aUI,"OnX_InputAxis_Z_logScale.set",zlog)) return;

  // Do not set directly the variaous SoAxis since
  // the plotter has its own logic for them.

  soPlotter->xAxisAutomated.setValue(xauto?TRUE:FALSE);
  soPlotter->xAxisMinimum.setValue(xmin);
  soPlotter->xAxisMaximum.setValue(xmax);
  soPlotter->xAxisLogScale.setValue(xlog?TRUE:FALSE);
  soPlotter->yAxisAutomated.setValue(yauto?TRUE:FALSE);
  soPlotter->yAxisMinimum.setValue(ymin);
  soPlotter->yAxisMaximum.setValue(ymax);
  soPlotter->yAxisLogScale.setValue(ylog?TRUE:FALSE);
  soPlotter->zAxisAutomated.setValue(zauto?TRUE:FALSE);
  soPlotter->zAxisMinimum.setValue(zmin);
  soPlotter->zAxisMaximum.setValue(zmax);
  soPlotter->zAxisLogScale.setValue(zlog?TRUE:FALSE);

  // Enforce a rebuild :
  soPlotter->raiseAllUpdateFlags();
  soPlotter->updateChildren();
}
//////////////////////////////////////////////////////////////////////////////
void OnX_InputAxis_update(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// Get all parameters from plotter and update the display with thoses value
// Update all the InputAxis 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotter* soPlotter = ((SoPlotterRegion*)soRegion)->getPlotter();

#define UPD_AUTO(a_method,a_axis) \
 {SbString s;\
  soPlotter->a_method.get(s);\
  aUI.setParameter("OnX_InputAxis_"+std::string(#a_axis)+"_automated.set",s.getString());}

#define UPD(a_axis,a_what,a_field) \
 {SbString s;\
  soPlotter->get##a_axis##Axis()->a_what.get(s);\
  aUI.setParameter("OnX_InputAxis_"+std::string(#a_axis)+"_"+std::string(#a_what)+"."+std::string(#a_field),s.getString());}

  UPD_AUTO(xAxisAutomated,x)
  UPD(X,minimumValue,value)
  UPD(X,maximumValue,value)
  UPD(X,logScale,set)

  UPD_AUTO(yAxisAutomated,y)
  UPD(Y,minimumValue,value)
  UPD(Y,maximumValue,value)
  UPD(Y,logScale,set)

  UPD_AUTO(zAxisAutomated,z)
  UPD(Z,minimumValue,value)
  UPD(Z,maximumValue,value)
  UPD(Z,logScale,set)
}

} // extern "C"
