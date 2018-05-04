
#include <AIDA/AIDA.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/Managers.h>

#include <amanip/find.h>
#include <amanip/create.h>

static void slash_shape_hits_collect(Slash::Core::ISession& a_session,AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {

 {Slash::UI::IScriptManager* sm = Slash::scriptManager(a_session);
  if(!sm) {
    a_out << "slash_shape_hits_collect :" 
          << " ScriptManager not found."
          << std::endl;
    return;
  } else {
    Slash::UI::IInterpreter::Options opts;
    sm->executeScript("Session","OnXExas shape_initialize_sess",opts);
    sm->executeScript("Session","OnXLab declare_accessors_sess",opts);
  }}

  AIDA::ITree* tree = amanip::find_mem(a_aida);
  if(!tree) return;

  std::string hname = "h_OnX_Shape_hits";
  if(tree->find(hname)) tree->rm(hname);

  AIDA::IHistogram1D* h = 
    amanip::create_histogram_1D
      (a_aida,*tree,hname,"OnX Shape hits.",10,0,10);
  if(!h) return;

  Slash::Data::collect(a_session,"OnX_Shape","type==\"Cube\"");
  Slash::Data::number(a_session);

  // Fill histogram with the data_collect :
  Slash::Data::fill_histogram_1D(a_session,hname,"hits");

  // Get current plotter :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->createRegions(1,1,0);
    plotter->setTitle("Histo & data_collect");
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.plot(*h);

    if(a_ui) {
      plotter->show();
      plotter->interact();
    }

    delete plotter;
  }

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
    std::cout << "aida_exa_slash_shape_hits_collect :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 

  Slash::Core::ISession* session = dynamic_cast<Slash::Core::ISession*>(aida);
  if(!session) {
    std::cout << "aida_exa_slash_shape_hits_collect :" 
              << " AIDA::IAnalysisFactory not a Slash::Core::ISession." 
              << std::endl;
    return 1;
  }

  slash_shape_hits_collect(*session,*aida,std::cout,"",true);
  delete aida;
  return 0;
}
