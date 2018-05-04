
#include <Slash/Tools/UI.h>

#include <inlib/web>
#include <inlib/file>
#include <inlib/touplow>

extern "C" {
  void onx_kuip(void*);
}

#include <kuip/ksys.h>
#include <kuip/kfor.h>
#include <kuip/kuser.h>

void onx_kuip(void* aSession) {
  Slash::Core::ISession* session = (Slash::Core::ISession*)aSession;

  Slash::UI::IUI* ui = Slash::find_UI(*session);
  if(!ui) return;

  std::string cmd_path = ku_path() ? ku_path() : "";
  //std::string cmd_name = ku_name();

  ///////////////////////////////////////////////////////
  /// SLASH /////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/SLASH/WGET") {
    std::string FILE = ku_gets();
    inlib::web::cache cache(session->cout(),false);
    std::string name = inlib::base_name(FILE);
    ::remove(name.c_str());
    std::string local;
    cache.fetch(FILE,local);    
    inlib::file::mv(local,name);
    return;
  }

  if(cmd_path=="/SLASH/LOAD") {
    std::string LIB = ku_gets();
    std::string PACK = ku_gets();
    session->load(LIB,PACK);
    return;
  }

  ///////////////////////////////////////////////////////
  /// UI ////////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/SLASH/UI/SYNCHRONIZE") {
    ui->synchronize();
    return;
  }

  if(cmd_path=="/SLASH/UI/SET_PARAMETER") {
    std::string WHAT = ku_gets();
    std::string VALUE = ku_gets();
    ui->setParameter(WHAT,VALUE);
    return;
  }

  if(cmd_path=="/SLASH/UI/LOAD") {
    std::string FILE = ku_gets();
    std::string name;
    inlib::file_name(FILE,name);
    ui->load("main",name);
    return;
  }

  if(cmd_path=="/SLASH/UI/SHOW") {

    std::string WIDGET = ku_gets();

    Slash::UI::IWidget* w = ui->findWidget(WIDGET);
    if(w) w->show();

    return;
  }

  if(cmd_path=="/SLASH/UI/DIALOG") {

    std::string NAME = ku_gets();

    inlib::touppercase(NAME);

    if(NAME=="PRINT") {
      ui->executeScript("DLD","OnX ui_show_dialog $ONXROOT/scripts/OnX/InputPrint.onx OnX_InputPrint_dialog");

    } else {
      std::ostream& out = session->cout();
      out << "Unknown dialog box " << inlib::sout(NAME)
          << "."
          << std::endl;
    }
 
    return;
  }

  ///////////////////////////////////////////////////////
  /// VIEWER ////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/SLASH/VIEWER/VIEW_ALL") {
    std::string NAME = ku_gets();
    std::string cmd = "OnX viewer_view_all";
    cmd += " "+NAME;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/VIEWER/RENDER") {
    std::string NAME = ku_gets();
    std::string cmd = "OnX viewer_render";
    cmd += " "+NAME;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/VIEWER/SET_DECORATION") {
    std::string NAME = ku_gets();
    std::string VALUE = ku_gets();
    std::string cmd = "OnX viewer_set_decoration";
    cmd += " "+NAME;
    cmd += " "+VALUE;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/VIEWER/SET_VIEWING") {
    std::string NAME = ku_gets();
    std::string VALUE = ku_gets();
    std::string cmd = "OnX viewer_set_viewing";
    cmd += " "+NAME;
    cmd += " "+VALUE;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/VIEWER/SET_FEEDBACK_VISIBILITY") {
    std::string NAME = ku_gets();
    std::string VALUE = ku_gets();
    std::string cmd = "OnX viewer_set_feedback_visibility";
    cmd += " "+NAME;
    cmd += " "+VALUE;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/VIEWER/READ_SCENE") {
    std::string NAME = ku_gets();
    std::string FILE = ku_gets();
    std::string FORMAT = ku_gets();
    std::string PLACEMENT = ku_gets();
    std::string cmd = "OnX viewer_read_scene";
    cmd += " "+NAME;
    cmd += " "+FILE;
    cmd += " "+FORMAT;
    cmd += " "+PLACEMENT;
    ui->executeScript("DLD",cmd);
    return;
  }

  ///////////////////////////////////////////////////////
  /// PAGE //////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/SLASH/PAGE/REFRESH") {
    std::string NAME = ku_gets();
    std::string cmd = "OnX page_refresh";
    cmd += " "+NAME;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/PAGE/CLEAR") {
    std::string NAME = ku_gets();
    std::string cmd = "OnX page_clear";
    cmd += " "+NAME;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/PAGE/SET_CURRENT_REGION") {
    std::string NAME = ku_gets();
    int N = ku_geti();
    std::string cmd = "OnX page_set_current_region";
    cmd += " "+NAME;
    cmd += " "+inlib::to<int>(N);
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/PAGE/SET_PARAMETER") {
    std::string NAME = ku_gets();
    std::string WHAT = ku_gets();
    std::string VALUE = ku_gets();
    std::string cmd = "OnX page_set_parameter";
    cmd += " "+NAME;
    cmd += " "+WHAT;
    cmd += " "+VALUE;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/PAGE/SET_REGIONS_HIGHLIGHTABLE") {
    std::string NAME = ku_gets();
    std::string VALUE = ku_gets();
    std::string cmd = "OnX page_set_root_regions_highlightable";
    cmd += " "+NAME;
    cmd += " "+VALUE;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/PAGE/CREATE_REGION") {
    std::string NAME = ku_gets();
    std::string CLASS = ku_gets();
    std::string X = ku_gets();
    std::string Y = ku_gets();
    std::string W = ku_gets();
    std::string H = ku_gets();
    std::string SET_CURRENT = ku_gets();
    std::string cmd = "OnX page_create_region";
    cmd += " "+NAME;
    cmd += " "+CLASS;
    cmd += " "+X;
    cmd += " "+Y;
    cmd += " "+W;
    cmd += " "+H;
    cmd += " "+SET_CURRENT;
    ui->executeScript("DLD",cmd);
    return;
  }

  ///////////////////////////////////////////////////////
  /// REGION ////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/SLASH/REGION/CLEAR") {
    std::string NAME = ku_gets();
    std::string cmd = "OnX region_clear";
    cmd += " "+NAME;
    ui->executeScript("DLD",cmd);
    return;
  }

  if(cmd_path=="/SLASH/REGION/SET_PARAMETER") {
    std::string NAME = ku_gets();
    std::string WHAT = ku_gets();
    std::string VALUE = ku_gets();
    std::string cmd = "OnX region_set_parameter";
    cmd += " "+NAME;
    cmd += " "+WHAT;
    cmd += " "+VALUE;
    ui->executeScript("DLD",cmd);
    return;
  }

  std::ostream& out = session->cout();
  out << "onx_kuip :" 
      << " " << cmd_path << " : unknow command." << std::endl;
}
