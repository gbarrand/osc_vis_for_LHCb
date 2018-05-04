// this :
#include <OnXLab/Inventor/PlotterManager.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IWidget.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>

// Lib:
#include <inlib/cast>

#include <OnXLab/Inventor/Plotter.h>

#include <HEPVis/nodekits/SoPage.h>

//#include <OnXLab/Inventor/SimplePlotter_onx.h>
#include <OnXLab/Inventor/PlotterWindow_onx.h>

#include <inlib/tos>
#include <inlib/srep>
#include <inlib/args>
#include <inlib/file>
#include <inlib/system>

OnXLab::PlotterManager::PlotterManager(Slash::Core::ISession& aSession)
:fName("OnXLab_PlotterManager")
,fSession(aSession)
{}

OnXLab::PlotterManager::~PlotterManager(){}

std::string OnXLab::PlotterManager::name() const{return fName;}

void* OnXLab::PlotterManager::cast(const std::string& a_class) const{
       INLIB_IF_CAST(OnXLab::PlotterManager)
  else INLIB_IF_CAST(Slash::UI::IPlotterManager)
  else INLIB_IF_CAST(Slash::Core::IManager)
  else return 0;
}

bool OnXLab::PlotterManager::setArguments(const std::vector<std::string>& aArgs){fArgs = aArgs;return true;}

Slash::UI::IPlotter* OnXLab::PlotterManager::create(const std::string& aName){
  Slash::UI::IUI* ui = loadUI();
  if(!ui) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::PlotterManager::create :"
        << " no UI found." 
        << std::endl;
    return 0;
  }
  Slash::UI::IWidget* w = 0;

  if(aName.size()) {
    if(aName==Slash::s_current()) {
      w = ui->currentWidget();
      if(!w) {
        std::ostream& out = fSession.cout();
        out << "OnXLab::PlotterManager::create :"
          << " no current widget."
          << std::endl;
        return 0;
      }
    } else {
      w = ui->findWidget(aName);
      if(!w) {
        std::ostream& out = fSession.cout();
        out << "OnXLab::PlotterManager::create :"
            << " widget " << inlib::sout(aName) << " not found." 
            << std::endl;
        return 0;
      }
    }
  } else { //no name given.

    std::string s = 
      inlib::tos(PlotterWindow_onx_linen,PlotterWindow_onx_lines);
    inlib::replace(s,"@@double_quote@@","\"");
    inlib::replace(s,"@@back_slash@@","\\");
    ui->load("",s,false);

    //in principle the PlotterWindow.onx does a set current widget.
    w = ui->currentWidget();
    if(!w) {
      std::ostream& out = fSession.cout();
      out << "OnXLab::PlotterManager::create :"
          << " no current widget after loading of PlotterWindow.onx."
          << std::endl;
      return 0;
    }

    //printf("debug : plotter name is %s\n",w->name().c_str());

  }
  if(!w) return 0;

  OnXLab::Plotter* plotter = new OnXLab::Plotter
    (fSession.cout(),*w,*ui,fSession.verboseLevel());
  if(!plotter->isValid()) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::PlotterManager::create :"
        << " OnX::Plotter not valid."
        << std::endl;
    delete plotter;
    return 0;
  }
  return plotter;
}

Slash::UI::IUI* OnXLab::PlotterManager::loadUI(){
  Slash::UI::IUI* ui = Slash::find_UI(fSession,Slash::s_default(),true);
  if(ui) return ui; //done
  Slash::UI::IUI_Manager* mgr = Slash::uiManager(fSession);
  if(!mgr) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::PlotterManager::loadUI :"
        << " can't find the UI_Manager."
        << std::endl;
    return 0;
  }

  std::string driver;

 {// the below should be done by OnXLab::Main (created in AIDA.cxx) !
  inlib::args args(fArgs);
  if(args.is_arg("-thread")) {
    fSession.setParameter("UI_Manager.thread","yes");
  }
  if(args.is_arg("-verbose")) {
    fSession.setVerboseLevel(1);
  }
  driver = inlib::gui_toolkit(args,true);
  if(driver.empty()) {
    inlib::getenv("OSC_GUI_TOOLKIT",driver);
  }}

  bool load_simple_plotter = false;

  std::string file;
  if(inlib::check_getenv
       (fSession.cout(),"ONXLAB_PLOTTER","ONXLABPLOTTER",file)) {
    std::string _file;
    inlib::file_name(file,_file);
    if(!inlib::file::exists(_file)) {
      if(fSession.verboseLevel()) {
        std::ostream& out = fSession.cout();
        out << "OnXLab::PlotterManager::loadUI :"
            << " Environment variable ONXLAB_PLOTTER does not refer"
            << " to an existing file."
            << " Load the embedded SimplePlotter.onx..." 
            << std::endl;
      }
      load_simple_plotter = true;
    }
  } else {
    load_simple_plotter = true;
  }

  if(load_simple_plotter) {
    ui = mgr->create(driver,"@empty@",fArgs,false);
    if(!ui) {
      std::ostream& out = fSession.cout();
      out << "OnXLab::PlotterManager::loadUI :"
          << " can't create @empty@ UI for driver " << inlib::sout(driver) 
          << "."
          << std::endl;
      return 0;
    }

    //std::string s = 
    //  inlib::to_string(SimplePlotter_onx_linen,SimplePlotter_onx_lines);
    //inlib::replace(s,"@@double_quote@@","\"");
    //inlib::replace(s,"@@back_slash@@","\\");
    //ui->load("",s,false);

    ui->executeScript("DLD","OnX OnX_declare_accessors");
    ui->executeScript("DLD","OnX session_set_output terminal");
    ui->executeScript("DLD","OnX session_setParameter mainTree.append no");
    ui->executeScript("DLD","OnXLab OnXLab_initialize_AIDA");
    ui->executeScript("DLD","OnXLab OnXLab_declare_accessors");
    // Have to set OnXLab.tree to something :
    ui->executeScript("DLD","OnX session_set_parameter OnXLab.tree plotter_0");

  } else {
    ui = mgr->create(driver,file,fArgs,false);
    if(!ui) {
      std::ostream& out = fSession.cout();
      out << "OnXLab::PlotterManager::loadUI :"
          << " can't create UI for driver " << inlib::sout(driver) 
          << " and file " << inlib::sout(file) 
          << std::endl;
      return 0;
    }
  }

  return ui;
}
