
// AIDA :
#include <AIDA/IPlotter.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ISoViewer.h>
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/Inventor.h>

// amanip :
#include <amanip/create.h>

static void deprecated0(Slash::UI::IUI& aUI,const std::string& aOld) {
  std::ostream& out = aUI.session().cout();
  out << "DLD callback " << inlib::sout(aOld) << " is deprecated."
      << std::endl;
}

static void deprecated(Slash::UI::IUI& aUI,const std::string& aOld,const std::string& aNew) {
  std::ostream& out = aUI.session().cout();
  out << "DLD callback " << inlib::sout(aOld) << " is deprecated."
      << " Use " << inlib::sout(aNew) << " instead."
      << std::endl;
}

extern "C" {

void OnXLab_initialize_AIDA(Slash::UI::IUI&);
void OnXLab_declare_accessors(Slash::UI::IUI&); 
void OnXLab_command_line_args(Slash::UI::IUI&);

void OnXLab_Initialize(Slash::UI::IUI& aUI) {
  Slash::UI::ISoViewer* soViewer = 
    Slash::find_SoViewer(aUI,Slash::s_current());
  if(soViewer) {
    soViewer->setDecoration(false);
    soViewer->setViewing(false);
  }
  OnXLab_initialize_AIDA(aUI);
  OnXLab_declare_accessors(aUI);
  aUI.session().setParameter("OnXLab_tree.append","no");
  OnXLab_command_line_args(aUI);
}


//////////////////////////////////////////////////////////////////////////////
void OnXLab_mainTree_update(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated(aUI,
             "OnXLab_mainTree_update",
             "OnX ui_set_parameter @{OnXLab.tree}@ @cat@ .items @session@hierarchy@");
  aUI.executeScript("DLD","OnX ui_set_parameter @{OnXLab.tree}@ @cat@ .items @session@hierarchy@");
}

//////////////////////////////////////////////////////////////////////////////
void OnXLab_plotter_initialize(
 Slash::UI::IUI& aUI
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deprecated0(aUI,"OnXLab_plotter_initialize");

  Slash::Core::ISession& session = aUI.session();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(session);
  if(!aida) return;

  // Get current plotter :
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,"@current@");
  if(!plotter) return;

  plotter->createRegions(1,1,0);
  plotter->setTitle("");

  delete plotter;

  Slash::UI::ISoViewer* soViewer =
    Slash::find_SoViewer(aUI,Slash::s_current());
  if(soViewer) {
    soViewer->setDecoration(false);
    soViewer->setViewing(false);
  }
}

} // extern "C"
