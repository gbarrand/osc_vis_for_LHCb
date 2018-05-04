
#ifndef main
#define main slash_iv_plot_and_det_main_dummy
#endif
#include "slash_iv_plot_and_det.cpp"
#undef main

#include <HEPVis/nodes/SoImage.h>
#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodekits/SoImageRegion.h>
#include <HEPVis/nodekits/SoTextRegion.h>

#include <cstdlib> //::getenv

static void slash_iv_plot_and_images(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {

  slash_iv_plot_and_det(a_session,a_aida,a_out,a_plotter,false);

  Slash::UI::IUI* ui = Slash::find_UI(a_session);     
  if(!ui) {
    a_out << "Slash::UI::IUI not found." << std::endl;
    return;
  }

  std::string OSC_HOME_DIR;
 {const char* env = ::getenv("OSC_HOME_DIR");
  if(env) {
    OSC_HOME_DIR = env;
  } else {
    a_out << " Environment variable OSC_HOME_DIR not defined." << std::endl;
  }}

  std::string DATA = OSC_HOME_DIR+"/Resources/AIDA/examples/data/";

  SoPage* soPage = Slash::find_SoPage(*ui,a_plotter);
  if(!soPage) return;

  // A region with the Dirac equation done with a .gif produced 
  // from a .tex file :
  {
    SoImageRegion* soRegion = 
      (SoImageRegion*)soPage->createRegion("SoImageRegion",
                                           0.01F,0.85F,0.25F,0.25F);
    if(!soRegion) return;
    soPage->setCurrentRegion(soRegion);
    HEPVis_SoImage* soImage = soRegion->getImage();

    std::string file = DATA+"Dirac.gif";

    soImage->fileName.setValue(file.c_str());

    a_out << " The Dirac equation is an insertion" << std::endl;
    a_out << "of a GIF file done with the HEPVis/SoImage node." << std::endl;
    a_out << "The GIF file had been produced from a latex file" << std::endl;
    a_out << "(examples/data/Dirac.tex) and with the dvi2bitmap program." 
          << std::endl;
  }

  // A region with a Feynman diagram done with a .gif produced 
  // from a .tex file :
  {
    SoImageRegion* soRegion = 
      (SoImageRegion*)soPage->createRegion("SoImageRegion",
                                           0.5F,0.03F,0.25F,0.25F);
    if(!soRegion) return;
    soPage->setCurrentRegion(soRegion);
    HEPVis_SoImage* soImage = soRegion->getImage();

    std::string file = DATA+"fmf_w_e_nu.gif";

    soImage->fileName.setValue(file.c_str());

    a_out << " The Feynman equation is an insertion" << std::endl;
    a_out << "of a GIF file done with the HEPVis/SoImage node." << std::endl;
    a_out << "The GIF file had been produced from a latex file" << std::endl;
    a_out << "(examples/data/fmf_w_e_nu.tex) by using the feynMF package" 
          << std::endl;
    a_out << "and with the dvi2bitmap program." << std::endl;
  }

  // A region with a formula :
  {
    SoTextRegion* soRegion = 
      (SoTextRegion*)soPage->createRegion("SoTextRegion",
                                          0.4F,0.6F,0.5F,0.2F);
    if(!soRegion) return;
    soPage->setCurrentRegion(soRegion);

    soRegion->color.setValue(SbColor(0.8F,0.8F,0.8F));
    // PAW encoding :
    soRegion->text.set1Value(0,"W^+! -> e^+! [n]?e!");
    soRegion->text.set1Value(1,"W^-! -> [m]^-! |[n]?[m]!");

    SoTextStyle* soStyle = soRegion->getStyle(0);
    if(soStyle) {
      soStyle->encoding.setValue("PAW");
      soStyle->color.setValue(0,0,1);
      soStyle->fontName.setValue("times");
    }

    soStyle = soRegion->getStyle(1);
    if(soStyle) {
      soStyle->encoding.setValue("PAW");
      soStyle->color.setValue(1,0,0);
      soStyle->fontName.setValue("arialbd");
    }
  }

  // A region with LaJoconde :
  {
    SoImageRegion* soRegion = 
      (SoImageRegion*)soPage->createRegion("SoImageRegion",
                                           0.01F,0.03F,0.25F,0.25F);
    if(!soRegion) return;
    soPage->setCurrentRegion(soRegion);
    HEPVis_SoImage* soImage = soRegion->getImage();

    std::string file = DATA+"LaJoconde.gif";

    soImage->fileName.setValue(file.c_str());

    soRegion->getViewportRegion()->horizontalShadow.setValue(10);
    soRegion->getViewportRegion()->verticalShadow.setValue(-10);

    soRegion->horizontalMargin.setValue(20);
    soRegion->verticalMargin.setValue(20);
  }

  soPage->setCurrentRegion(0);

  if(a_ui) {
    ui->steer();
  }

  //delete plotter;
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
    std::cout << "aida_exa_slash_iv_plot_and_images :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  Slash::Core::ISession* session = dynamic_cast<Slash::Core::ISession*>(aida);
  if(!session) {
    std::cout << "aida_exa_slash_iv_plot_and_images :" 
              << " AIDA::IAnalysisFactory not a Slash::Core::ISession." 
              << std::endl;
    return 1;
  }

  AIDA::IPlotter* plotter = amanip::create_plotter(*aida);        
  if(!plotter) {
    std::cout << "aida_exa_slash_iv_plot_and_images :" 
              << " can't create plotter." 
              << std::endl;
    return 1;
  }
  delete plotter;

  slash_iv_plot_and_images(*session,*aida,std::cout,"@current@",true);
  delete aida;
  return 0;
}
