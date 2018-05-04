// this :
#include <OnX/Core/Main.h>

#include <Slash/Core/IManager.h>
#include <Slash/Core/ILibrary.h>
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>

#include <inlib/file>
#include <inlib/args>
#include <inlib/system>
#include <inlib/vmanip>
#include <inlib/cast>

#include <Lib/Debug.h>

#include <OnX/Core/Session.h>

/*
std::vector<std::string> OnX::Main::defaultKnownOptions(){
  std::vector<std::string> options;
  options.push_back("-verbose");
  options.push_back("-stdout");
  options.push_back("-defout");
  options.push_back("-uiout");
  options.push_back("-output");
  options.push_back("-ui");
  options.push_back("-toolkit");
  options.push_back("-Xt");
  options.push_back("-xt");
  options.push_back("-Xm");
  options.push_back("-xm");
  options.push_back("-Motif");
  options.push_back("-motif");
  options.push_back("-Win");
  options.push_back("-win");
  options.push_back("-Win32");
  options.push_back("-win32");
  options.push_back("-Gtk");
  options.push_back("-gtk");
  options.push_back("-Qt");
  options.push_back("-qt");
  options.push_back("-NextStep");
  options.push_back("-nextstep");
  options.push_back("-SDL");
  options.push_back("-sdl");
  options.push_back("-Net");
  options.push_back("-net");
  options.push_back("-Python");
  options.push_back("-python");
  options.push_back("-Tcl");
  options.push_back("-tcl");
  options.push_back("-KUIP");
  options.push_back("-kuip");
  options.push_back("-DLD");
  options.push_back("-dld");
  options.push_back("-session");
  options.push_back("-debug_check_class");
  options.push_back("-balance");
  options.push_back("-style");
  options.push_back("-thread");
  //options.push_back("-from");
  options.push_back("-onx_file");
  options.push_back("-ib");
  options.push_back("-enable_terminal");
  // NetUI :
  options.push_back("-server_toolkit");
  // SDL_UI :
  options.push_back("-password");
  options.push_back("-video_width");
  options.push_back("-video_height");
  options.push_back("-not_full_screen");
  return options;  
}
*/

bool belong(const std::vector<std::string>& aArgs,const std::string& aWhat) {
  std::string senv;
  if(inlib::getenv("OSC_ARGS",senv)) {
    std::vector<std::string> args(aArgs);
    inlib::append<std::string>(args,inlib::words(senv," "));
    return inlib::belong(args,aWhat);
  } else {
    return inlib::belong(aArgs,aWhat);
  }
}

#define s_balance "-balance"
#define s_debug_check_class "-debug_check_class"

OnX::Main::Main(const std::vector<std::string>& aArgs)
: OnX::Session(belong(aArgs,s_balance),
               belong(aArgs,s_debug_check_class))
,fStatus(false)
,fIB(false)
{
  Lib::Debug::increment("OnX::Main");
  initialize(aArgs);
}

// deprecated (for Panoramix) :
OnX::Main::Main(bool,bool,const std::vector<std::string>& aArgs)
: OnX::Session(belong(aArgs,s_balance),
               belong(aArgs,s_debug_check_class))
,fStatus(false)
,fIB(false)
{
  Lib::Debug::increment("OnX::Main");
  initialize(aArgs);
  cout() << "OnX::Main::Main(int,int,const std::vector<std::string>&) :"
         << " deprecated cnstructor." 
         << std::endl;  
}

OnX::Main::~Main(){
  printer().disable(); //since the UI is no more here.

  if(verboseLevel()) {
    cout() << "OnX::Main::~Main :"
          << " Exiting..." << std::endl;
  }
  Lib::Debug::decrement("OnX::Main");
}

void* OnX::Main::cast(const std::string& a_class) const {
  // inheritance :
  if(void* p = OnX::Session::cast(a_class)) return p;
  // this :
  else INLIB_IF_CAST(OnX::Main)
  else return 0;
}

//private:
OnX::Main::Main(const Main&){}
OnX::Main& OnX::Main::operator=(const Main&){return *this;}

void OnX::Main::initialize(const std::vector<std::string>& aArgs){

  //if(!OnX::set_env(aArgs.size()?aArgs[0]:"",cout())) {
  //  //throw...
  //}

  inlib::args args(aArgs);

 {std::string senv;
  if(inlib::getenv("OSC_ARGS",senv)) {
    std::vector<std::string> ws;
    inlib::words(senv," ",false,ws);
    args.add(ws);
    args.files_at_end();
  }}

  args.remove(s_balance);
  args.remove(s_debug_check_class);

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) setVerboseLevel(1);
    args.remove("-verbose");
  }

  std::string arg0(aArgs.size()?aArgs[0]:"");

 {std::string sessionFile;
  if(args.find("-session",sessionFile)) {
    args.remove("-session");
  } else {
    if(inlib::check_getenv(cout(),"ONX_SESSION","ONXSESSION",sessionFile)) {
      if(!inlib::file::exists(sessionFile)) {
        cout() << "OnX::Main::Main :"
              << " Environment variable ONX_SESSION does not refer"
              << " to an existing file."
              << " ONX_SESSION value was " << inlib::sout(sessionFile) << "."
              << " Load the embedded Session.onx." 
              << std::endl;
        sessionFile = "";
      }
    }
  }
  OnX::Session::setDefaults(sessionFile);}

  Slash::Core::ISession& f_session = *this;

  f_session.setParameter("session.binary",arg0);

 {std::string sstyle;
  if(args.find("-style",sstyle)) {
    args.remove("-style");
  }
  if(sstyle.size()) f_session.setParameter("UI_Manager.style",sstyle);}

  if(sverbose.size()) {
    // Assume that the given value is the name of a manager.
    // For exa :
    //   OS> onx -verbose=UI_Manager
    f_session.setParameter(sverbose+".verbose","1");
  }

 {std::string sout;
  if(args.find("-output",sout)) {
    if((sout!="terminal")&&(sout!="UI")&&((sout!="defout"))) {
      cout() << "OnX::Main::Main :"
            << " unknown output option " << inlib::sout(sout) << "." 
            << std::endl;
    } else {
      f_session.setParameter("session.output",sout);
    }
    args.remove("-output");
  } else {
    if(args.is_arg("-stdout")) {
      f_session.setParameter("session.output","terminal");
      args.remove("-stdout");
    }
    if(args.is_arg("-defout")) {
      f_session.setParameter("session.output","defout");
      args.remove("-defout");
    }
    if(args.is_arg("-uiout")) {
      f_session.setParameter("session.output","UI");
      args.remove("-uiout");
    }
  }}

  std::string interp;
  std::string cmd;
  if(args.find("-Python",cmd)||args.find("-python",cmd)) {
    interp = "Python";
    args.remove("-Python");
  } else if(args.find("-Tcl",cmd)||args.find("-tcl",cmd)) {
    interp = "Tcl";
    args.remove("-Tcl");
  } else if(args.find("-KUIP",cmd)||args.find("-kuip",cmd)) {
    interp = "KUIP";
    args.remove("-KUIP");
  } else if(args.find("-DLD",cmd)||args.find("-dld",cmd)) {
    interp = "DLD";
    args.remove("-DLD");
  }

  if(args.is_arg("-thread")) {
    f_session.setParameter("UI_Manager.thread","yes");
    args.remove("-thread");
  }

  if(args.is_arg("-ib")) {
    fIB = true;
    args.remove("-ib");
  }

  if(args.find("-enable_terminal",fTerm)) {
    if(fTerm.empty()) fTerm = "DLD";
    f_session.setParameter("session.output","terminal");
    args.remove("-enable_terminal");
  }

  // GUI file :
  findFile(cout(),args,fDefaultFile);

  // Get default driver and OnX file here in order
  // to free the args (saved in the session)
  // from the related args :
 {std::string value;
  if(parameterValue("UI_Manager.toolkit",value) && (value.size()) ) {
    fDefaultDriver = value;
  } else { //check args :
    // rm in args only if used.
    fDefaultDriver = findDriver(args,fDefaultFile.size()?true:false);
  }}


  bool show_ui;
  if(args.find("-show_ui",show_ui)) {
    args.remove("-show_ui");
  } else {
    show_ui = true;
  }

  // save only stripped arguments in the session.
  setArguments(args.get_args()); 

  if(fDefaultFile.size()) {
    Slash::UI::IUI* ui = createUI("",fDefaultFile,show_ui);
    if(!ui) {
      f_session.printer().disable();
      fStatus = false;
      return;
    } 
  }

//if(aCreateUI) {
//  Slash::UI::IUI* ui = createUI("","",aShowUI);
//  if(!ui) {
//    f_session.printer().disable();
//    fStatus = false;
//    return;
//  } 
//}

  if(interp.size()) {
    if(verboseLevel()) {
      cout() << "OnX::Main::Main :"
            << " execute " << inlib::sout(interp)
            << " : " << inlib::sout(cmd) << "." << std::endl;
    }
    // Do not use anymore a UI to execute a script
    // so that someone can execute a script without creating a UI.
    //ui->executeScript(interp,cmd);
    Slash::UI::IScriptManager* sm = Slash::scriptManager(*this);
    if(!sm) {
      cout() << "OnX::Main::Main :"
            << " ScriptManager not found." 
            << " can't execute " << inlib::sout(interp)
            << " : " << inlib::sout(cmd) << "."
            << std::endl;
    } else {
      Slash::UI::IInterpreter::Options opts;
      sm->executeScript(interp,cmd,opts);
    }
  }

  fStatus = true;
}

Slash::UI::IUI* OnX::Main::createUI(const std::string& aDriver,const std::string& aFile,bool aShow){
  Slash::UI::IUI_Manager* uiManager = Slash::uiManager(*this);
  if(!uiManager) {
    cout() << "OnX::Main::createUI :"
          << " UI_Manager not found." << std::endl;
    return 0;
  }

  // For the moment only one Slash::UI::IUI handled.
 {Slash::UI::IUI* ui = uiManager->find("default");
  if(ui) return ui;} //Done

  // GUI driver :
  std::string driver;
  if(aDriver.empty()) {
    driver = fDefaultDriver;
  } else {
    driver = aDriver;
  }

  // GUI file :
  std::string file;
  if(aFile.empty()) {
    file = fDefaultFile;
  } else {
    file = aFile;
  }

  if(file.empty()) {
    cout() << "OnX::Main::createUI :"
          << " no OnX GUI XML file given." << std::endl;
    return 0;
  }

  //NOTE : we pass args without the Main specific ones.
  //       (Because they are going to be passed to the GUI toolkits).

  inlib::args args(arguments());
  Slash::UI::IUI* ui = uiManager->create(driver,file,args.tovector(),aShow);
  if(!ui) {
    cout() << "OnX::Main::createUI :"
          << " can't create UI for driver " << inlib::sout(driver) 
          << " and file " << inlib::sout(file) 
          << std::endl;
    return 0;
  }

  if(fIB) {
    std::string OSC_HOME_DIR;
    if(!inlib::getenv("OSC_HOME_DIR",OSC_HOME_DIR)) {
      cout() << "OnX::Main :"
            << " environment variable OSC_HOME_DIR not defined."
            << std::endl;
    } else {
      std::string file = 
        OSC_HOME_DIR + "/Resources/OnX/scripts/OnX/IB/IB.onx";
      ui->executeScript("DLD","OnX ui_load @empty@ "+file);
      ui->executeScript("DLD","OnX ui_show OnX_IB_main");
    }
  }

  if(fTerm.size()) ui->enableTerminal(fTerm+"> ",fTerm);

  return ui;
}

Slash::UI::IUI* OnX::Main::ui() const{
  INLIB_SELF(OnX::Main);
  Slash::UI::IUI_Manager* mgr = Slash::uiManager(self);
  if(!mgr) return 0;
  return mgr->find("default");
}

bool OnX::Main::loadInterpreter(const std::string& aInterp,bool aQuiet) const{
  INLIB_SELF(OnX::Main);
  Slash::UI::IScriptManager* sm = Slash::scriptManager(self);
  if(!sm) return false;
  Slash::UI::IInterpreter* interp = sm->findInterpreter(aInterp);
  if(!interp) return false;
  return interp->load(aQuiet);
}

std::string OnX::Main::findDriver(inlib::args& aArgs,bool a_rm_from_args){
  std::string driver = inlib::gui_toolkit(aArgs,a_rm_from_args);
  if(driver.empty()) {
    inlib::getenv("OSC_GUI_TOOLKIT",driver);
    if(driver.empty()) {
      inlib::getenv("ONX_DRIVER",driver);
    }
  }
  return driver;
}

bool OnX::Main::findFile(std::ostream& a_out,inlib::args& aArgs,std::string& aFile){
  std::string file;

  if(aArgs.find("-onx_file",file)) {
    aArgs.remove("-onx_file");
  } else if(aArgs.file(file)) {
    aArgs.remove(file);
  } else {
    if(inlib::getenv("OSC_GUI_FILE",file)) {
    } else {
      if(inlib::getenv("ONX_FILE",file)) {
        a_out << "Environment variable ONX_FILE is deprecated."
              << " Use OSC_GUI_FILE instead."
              << std::endl;
      } else {
        if(inlib::getenv("ONXFILE",file)) {
          a_out << "Environment variable ONXFILE is deprecated."
                << " Use OSC_GUI_FILE instead."
                << std::endl;
        }
      }
    }
  }
  aFile = file;
  return true;
}

//deprecated : 

bool OnX::Main::isValid() const{return fStatus;}

int OnX::Main::steer(){
  Slash::UI::IUI* iui = ui();
  if(!iui) return 0;
  return iui->steer();
}
void OnX::Main::show(){
  Slash::UI::IUI* iui = ui();
  if(!iui) return;
  iui->show();
  iui->steer();
}

//keep it for Panoramix :
//Slash::Core::ISession* OnX::Main::session() const{return *this;}

extern "C" {
  Slash::Core::ISession* Slash_create_session(const std::vector<std::string>& a_args){return new OnX::Main(a_args);}
}
