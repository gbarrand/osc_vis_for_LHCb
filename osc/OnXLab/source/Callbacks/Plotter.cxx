//
//  Functions here should be InputPlotter callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// AIDA :
#include <AIDA/IAnnotation.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>

// Slash :
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/Inventor.h>

// amanip :
#include <amanip/find.h>
#include <amanip/create.h>
#include <amanip/name.h>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>

#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/misc/SoStyleCache.h>
#include <HEPVis/SbColors.h>

#include <inlib/svalues>
#include <inlib/args>

#define MAXIMUM(a,b) ((a)>(b)?a:b)

extern "C" {

void OnXLab_rectangular_collect(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs)
{
  // args[0] widget name (or @current@)

  //std::ostream& out = aUI.session().cout();
  //out << "OnXExas_plotter_rectangular_collect :" 
  //    << " callback value : " << aUI.callbackValue()
  //    << std::endl;

  std::vector<int> ints;
  if(!inlib::values<int>(aUI.callbackValue().c_str()," ",false,ints)) return;
  if(ints.size()!=4) return;
  int x = ints[0];
  int y = ints[1];
  int w = ints[2];
  int h = ints[3];

  int xmn = x;
  int ymn = y;
  int xmx = x+w;
  int ymx = y+h;

  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;

  SoPage* soPage = Slash::find_SoPage(aUI,aArgs[0]);
  if(!soPage) return;

  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;

  SoViewportRegion* vpr = soRegion->getViewportRegion();
  int vx,vy;
  vpr->getPositionPixels(vx,vy);
  unsigned int vw,vh;
  vpr->getSizePixels(vw,vh);

  int vxmn = vx;
  int vymn = vy;
  int vxmx = vx+vw;
  int vymx = vy+vh;

  if(xmn<vxmn) xmn = vxmn;
  if(ymn<vymn) ymn = vymn;
  if(xmx>vxmx) xmx = vxmx;
  if(ymx>vymx) ymx = vymx;

  // size in pixels in the region viewport coordinates :
  w = xmx-xmn;
  if(w<=0) return;
  h = ymx-ymn;
  if(h<=0) return;
  x = xmn-vx;
  y = ymn-vy;

  //::printf("OnXExas_ploter_rectangular_collect : x %d y %d w %d h%d\n",
  //  x,y,w,h);

  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;

  SoPlotter* soPlotter = soPlotterRegion->getPlotter();

#define PT(a_x,a_y) SbVec3f(float(a_x)/float(vw),float(a_y)/float(vh),0)

  SbVec3f pts[4];
  soPlotter->vp2axis(PT(x,y),pts[0]);
  soPlotter->vp2axis(PT(x+w,y),pts[1]);
  soPlotter->vp2axis(PT(x+w,y+h),pts[2]);
  soPlotter->vp2axis(PT(x,y+h),pts[3]);

  //out << "OnXExas_plotter_rectangular_collect :" 
  //    << " xmn " << pts[0][0]
  //    << " ymn " << pts[0][1]
  //    << " xmx " << pts[2][0]
  //    << " ymx " << pts[2][1]
  //    << " (in axis coords)."
  //    << std::endl;

  if(soPlotter->shape.getValue()==SoPlotter::XY) {

    // Create a two dimensional InsidePolygon IFunction.

    AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
    if(!aida) return;
    AIDA::ITree* tree = amanip::find_mem(*aida);
    if(!tree) return;
    std::string s = amanip::create_name(*tree,"colfun_");
    
    // Visualize the cut :
    AIDA::IFunction* function = 
      amanip::create_function(*aida,*tree,s,s,"InsidePolygon");
    if(!function) return;
    std::vector<double> params;
    for(unsigned int i=0;i<4;i++) {
      params.push_back(pts[i][0]);
      params.push_back(pts[i][1]);
    }
    function->setParameters(params);

    AIDA::IPlotter* plotter = amanip::create_plotter(*aida,"@current@");
    if(!plotter) return;
    AIDA::IPlotterRegion& region = plotter->currentRegion();

    function->annotation().addItem("plotter.modeling","polygon");
    function->annotation().addItem("plotter.color","0.6 0.6 0.6");
    function->annotation().addItem("plotter.transparency","0.5");

    region.plot(*function);

    delete plotter;

  }

}

} // extern "C"
