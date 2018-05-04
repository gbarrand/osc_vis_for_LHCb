
#include <AIDA/AIDA.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/Inventor.h>

#include <amanip/find.h>
#include <amanip/create.h>

#include <inlib/random>

#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoScale.h>

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>
#include <HEPVis/nodekits/SoDetectorExample.h>

static void slash_iv_regions(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {

  AIDA::IPlotter* plotter = amanip::create_plotter(a_aida,a_plotter);        
  if(!plotter) return;

  Slash::UI::IUI* ui = Slash::find_UI(a_session);     
  if(!ui) {
    a_out << "Slash::UI::IUI not found." << std::endl;
    return;
  }

  SoPage* soPage = Slash::find_SoPage(*ui,a_plotter);
  if(!soPage) return;
  soPage->deleteRegions();

  //soPage->setTitleVisible(TRUE);
  soPage->setTitleVisible(FALSE);

  SbBool titleVisible = soPage->titleVisible.getValue();
  float titleHeight = soPage->titleHeight.getValue();

  // Detector :
  {    
    float height = titleVisible?1-titleHeight:1;

    SoDisplayRegion* soRegion = 
      (SoDisplayRegion*)soPage->createRegion("SoDisplayRegion",0,0,1,height);  
    if(!soRegion) return;
    soPage->setCurrentRegion(soRegion);

    soRegion->color.setValue(SbColor(0.8F,0.8F,0.8F));
    
    SoSeparator* sep = soRegion->getStaticScene();

    SoScale* soScale = new SoScale;
    soScale->scaleFactor.setValue(0.1F,0.1F,0.1F);
    sep->addChild(soScale);

    sep->addChild(new SoDetectorExample());
  }    
   
  // Histo viewport :
  {
    // Find memory tree :
    AIDA::ITree* memoryTree = amanip::find_mem(a_aida);
    if(!memoryTree) return;

    AIDA::IHistogram1D* h = 
      amanip::create_histogram_1D
        (a_aida,*memoryTree,"gauss","Gauss",100,-5,5);

    inlib::random::gauss r(0,1);
    for(int index=0;index<10000;index++) h->fill(r.shoot(),1);

    AIDA::IPlotterRegion* region = plotter->createRegion(0.01,0.01,0.3,0.3);
    if(!region) return;
    region->plot(*h);

    plotter->show();

    //if(a_ui) {
    //  plotter->interact();
    //}

  }

  soPage->setCurrentRegion(0);

  if(a_ui) {
    ui->steer();
  }

  delete plotter;
}

#undef name_h1d
#undef name_h2d
#undef name_f

//#include <Slash/AIDA/IAnalysisFactory.h>
//#define Cast(aObject,aClass) (aClass*)(aObject).cast(#aClass)

int main(int argc,char* argv[]) {

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
    std::cout << "aida_exa_slash_iv_regions :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  Slash::Core::ISession* session = dynamic_cast<Slash::Core::ISession*>(aida);
  if(!session) {
    std::cout << "aida_exa_slash_iv_regions :" 
              << " AIDA::IAnalysisFactory not a Slash::Core::ISession." 
              << std::endl;
    return 1;
  }

  AIDA::IPlotter* plotter = amanip::create_plotter(*aida);        
  if(!plotter) {
    std::cout << "aida_exa_slash_iv_regions :" 
              << " can't create plotter." 
              << std::endl;
    return 1;
  }
  delete plotter;

  slash_iv_regions(*session,*aida,std::cout,"@current@",true);
  delete aida;
  return 0;
}
