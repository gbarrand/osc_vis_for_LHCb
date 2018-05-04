
// Have a manager to delete histograms when quiting.
#include <Slash/Core/IManager.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/Inventor.h>

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoAxis.h>

#include <HEPVis/inlib_plot>

#include <inlib/random>

#ifdef OnXExas_include
//FIXME : arrange so that the below be local.

#include <HEPVis/nodes/SoViewportRegion.h>

extern "C" {

void OnXExas_plotter_rectangular_collect(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // args[0] widget name (or @current@)

  std::ostream& out = aUI.session().cout();
  out << "OnXExas_plotter_rectangular_collect :" 
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

  SbVec3f p[4];
  soPlotter->vp2axis(PT(x,y),p[0]);
  soPlotter->vp2axis(PT(x+w,y),p[1]);
  soPlotter->vp2axis(PT(x+w,y+h),p[2]);
  soPlotter->vp2axis(PT(x,y+h),p[3]);

  out << "OnXExas_plotter_rectangular_collect :" 
      << " xmn " << p[0][0]
      << " ymn " << p[0][1]
      << " xmx " << p[2][0]
      << " ymx " << p[2][1]
      << " (in axis coords)."
      << std::endl;

  //if(soPlotter->shape.getValue()==SoPlotter::XYZ) {}

}

} // extern "C"

#endif //OnXExas_include

static void plotter_example(Slash::Core::ISession& a_session,std::ostream& a_out,const std::string& a_viewer,bool a_ui) {

  Slash::UI::IUI* ui = Slash::find_UI(a_session);     
  if(!ui) {
    a_out << "Slash::UI::IUI not found." << std::endl;
    return;
  }

  SoPage* soPage = Slash::find_SoPage(*ui,a_viewer);
  if(!soPage) return;

  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;

  inlib::histo::h1d h("Gaussian",100,-5,5);
  inlib::random::gauss r(0,1);
  for(unsigned int index=0;index<10000;index++) h.fill(r.shoot(),1);

  SoPlotter* plotter = soPlotterRegion->getPlotter();
  plotter->setName("plotter_1");
  plotter->title.setValue("Title");
  plotter->setPlottableObject(new HEPVis::h1d2plot("h",h)); //h is copied.
  //plotter->set("binsStyle[0]","modeling points color 0 0 1 markerStyle PLUS markerSize 9");
  plotter->set("binsStyle[0]","modeling top_lines color 0 0 1");
  plotter->getXAxis()->title.setValue("x axis title");
  plotter->getYAxis()->title.setValue("y axis title");
  plotter->getZAxis()->title.setValue("z axis title");

  if(a_ui) ui->steer();

}

#include <iostream>
#include <inlib/osc_env> //For TTFPATH

int main(int,char*[]) {

  if(!inlib::osc::set_env(std::cout)) {
    std::cout << "create_ui :"
              << " unable to set OSC environment." << std::endl;
    return 1;
  }

  int argc = 1;
  static char* argv[] = {(char*)"-onx_file=plotter.onx"};  

  Slash::Core::ISession* session = Slash_create_session(inlib::to(argc,argv));
  if(!session) {
    std::cout << "plotter_example :"
              << " session not found." << std::endl;
    return 1;
  }

  plotter_example(*session,std::cout,"@current@",true);
  delete session;

  return 0;
}
