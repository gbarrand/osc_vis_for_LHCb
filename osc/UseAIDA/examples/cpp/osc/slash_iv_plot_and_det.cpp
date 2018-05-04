#ifndef slash_iv_plot_and_det_cpp
#define slash_iv_plot_and_det_cpp

#include <AIDA/AIDA.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/Inventor.h>

#include <amanip/find.h>
#include <amanip/create.h>

#include <inlib/random>

//#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoDetectorExample.h>

static void slash_iv_plot_and_det(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
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
  soPage->title.setValue("Histo and detector");
  soPage->setTitleVisible(TRUE);
  soPage->createRegions("SoPlotterRegion",1,1,0);

  /////////////////////////////////////////////////////////////////////////////
  // Histo plotting part :
  /////////////////////////////////////////////////////////////////////////////
  { 
    // Find memory tree :
    AIDA::ITree* tree = amanip::find_mem(a_aida);
    if(!tree) return;

    plotter->createRegions(1,1,0);
    
    AIDA::IHistogram1D* h = 
      amanip::create_histogram_1D(a_aida,*tree,"h","Rand gauss",100,-5,5);
    
    inlib::random::gauss r(0,1);
    for(int index=0;index<10000;index++) h->fill(r.shoot(),1);
    
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.setParameter("plotter.gridStyle.visible","FALSE");
    region.setParameter("plotter.wallStyle.visible","FALSE");
    //FIXME region.style().wallStyle().visible(false);
    //FIXME region.style().gridStyle().visible(false);

    region.plot(*h);
    plotter->show();
    
  } 
 
  {
    SoRegion* soRegion = soPage->currentRegion();
    if(!soRegion) return;
    if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
    SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;

    ///////////////////////////////////////////////////////////////////////////
    // Add a detector in the plotter scene.
    ///////////////////////////////////////////////////////////////////////////
    //WARNING : lights accumulate. Then do not put the below
    // scene graph under plotter.etcSeparator which is under
    // the light of the SoPlotterRegion. We put it under
    // the etc separator of the plotter region.
    SoSeparator* etc = soPlotterRegion->getEtcSeparator();
    //SoSeparator* etc = soPlotterRegion->getPlotter()->getEtcSeparator();
    if(!etc) return;

    SoDirectionalLight* soLight = new SoDirectionalLight;
    soLight->color.setValue(1,1,1);
    soLight->direction.setValue(SbVec3f(1,-1,-10));
    etc->addChild(soLight);

    SoLightModel* soLightModel = new SoLightModel;
    soLightModel->model.setValue(SoLightModel::PHONG);
    etc->addChild(soLightModel);

    SoOrthographicCamera* soCamera = new SoOrthographicCamera;
    soCamera->viewportMapping.setValue(SoCamera::ADJUST_CAMERA);
    soCamera->position.setValue(0,0,4);
    soCamera->orientation.setValue(SbRotation(SbVec3f(0,1,0),0));
    soCamera->height.setValue(2);
    soCamera->nearDistance.setValue(1);
    soCamera->farDistance.setValue(100);
    soCamera->focalDistance.setValue(4);
    etc->addChild(soCamera);

    SoTranslation* soTranslation = new SoTranslation;
    soTranslation->translation.setValue(0.8F,0.7F,0);
    etc->addChild(soTranslation);

    SoScale* soScale = new SoScale;
    soScale->scaleFactor.setValue(0.01F,0.01F,0.01F);
    etc->addChild(soScale);

    SoRotation* soRotation = new SoRotation;
    soRotation->rotation.setValue(SbRotation(SbVec3f(1,1,1),0.8F));
    etc->addChild(soRotation);

    etc->addChild(new SoDetectorExample());
  }

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
    std::cout << "aida_exa_slash_iv_plot_and_det :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  Slash::Core::ISession* session = dynamic_cast<Slash::Core::ISession*>(aida);
  if(!session) {
    std::cout << "aida_exa_slash_iv_plot_and_det :" 
              << " AIDA::IAnalysisFactory not a Slash::Core::ISession." 
              << std::endl;
    return 1;
  }

  // The below create_plotter will create a GUI.
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida);        
  if(!plotter) {
    std::cout << "aida_exa_slash_iv_plot_and_det :" 
              << " can't create plotter." 
              << std::endl;
    return 1;
  }
  delete plotter;

  slash_iv_plot_and_det(*session,*aida,std::cout,"@current@",true);
  delete aida;
  return 0;
}

#endif

