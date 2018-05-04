#ifndef slash_iv_aida_soplotter_cpp
#define slash_iv_aida_soplotter_cpp

#include <AIDA/AIDA.h>

#include <Slash/Core/ISession.h>
#include <Slash/UI/IPlotter.h>
#include <Slash/Tools/UI.h>

#include <amanip/find.h>
#include <amanip/create.h>

#include <inlib/random>

#include <HEPVis/nodes/SoStyle.h>
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoPlotter.h>

static void slash_iv_aida_soplotter(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // In the standalone example (not running from osc-plot), the below
  // create_plotter will create a GUI.

  AIDA::IPlotter* plotter = amanip::create_plotter(a_aida,a_plotter);        
  if(!plotter) return;

  //dynamic_cast to a Slash::UI::IPlotter :
  Slash::UI::IPlotter* sp = dynamic_cast<Slash::UI::IPlotter*>(plotter);
  if(!sp) {
    a_out << "AIDA::IPlotter not a Slash::UI::IPlotter." << std::endl;
    return;
  }

  SoPage* soPage = (SoPage*)sp->nodeOfInterest("page");
  if(!soPage) {
    a_out << "page node not found." << std::endl;
    return;
  }

  soPage->deleteRegions();
  soPage->title.setValue("Histo and detector");
  soPage->setTitleVisible(TRUE);
  soPage->createRegions("SoPlotterRegion",1,1,0);


 {AIDA::ITree* tree = amanip::find_mem(a_aida);
  if(!tree) return;
  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D(a_aida,*tree,"h","Rand gauss",100,-5,5);
  inlib::random::gauss r(0,1);
  for(int index=0;index<10000;index++) h->fill(r.shoot(),1);    
  AIDA::IPlotterRegion& region = plotter->currentRegion();
  region.plot(*h);
  plotter->show();}
    
  // customize by using SoPlotterRegion and SoPlotter :

  SoRegion* soRegion = soPage->currentRegion();
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter(); 

  soPlotter->getGridStyle()->visible.setValue(FALSE);
  soPlotter->getWallStyle()->visible.setValue(FALSE);

  if(a_ui) {
    Slash::UI::IUI* ui = Slash::find_UI(a_session);     
    if(!ui) {
      a_out << "Slash::UI::IUI not found." << std::endl;
      return;
    }
    ui->steer();
  }

  delete plotter;
}

#include <inlib/app>

int main(int argc,char* argv[]) {

  if(argc>0) {
    //Darwin : OSC specific if mouse starting from a .app.
    std::vector<std::string> home_dirs;
    home_dirs.push_back("OpenScientist");
    home_dirs.push_back("AIDA");
    home_dirs.push_back("session");
    inlib::if_mouse_startup(argv[0],"OSC_HOME_DIR",home_dirs,std::cout);
  }

  //Slash::AIDA::IAnalysisFactory* aida = Slash_AIDA_createAnalysisFactory();
  //if(!aida) return EXIT_FAILURE;
  //Slash::Core::ISession* session = Cast(*aida,Slash::Core::ISession);
  //if(!session) {
  //  std::cout << "AIDA::IAnalysisFactory not a Slash::Core::ISession." 
  //            << std::endl;
  //  return EXIT_FAILURE;
  //}

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_slash_iv_aida_soplotter :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  Slash::Core::ISession* session = dynamic_cast<Slash::Core::ISession*>(aida);
  if(!session) {
    std::cout << "aida_exa_slash_iv_aida_soplotter :" 
              << " AIDA::IAnalysisFactory not a Slash::Core::ISession." 
              << std::endl;
    return 1;
  }

  AIDA::IPlotter* plotter = amanip::create_plotter(*aida);        
  if(!plotter) {
    std::cout << "aida_exa_slash_iv_aida_soplotter :" 
              << " can't create plotter." 
              << std::endl;
    return 1;
  }
  delete plotter;

  slash_iv_aida_soplotter(*session,*aida,std::cout,"@current@",true);
  delete aida;
  return 0;
}

#endif

