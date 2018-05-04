// this :
#include <OnX/Core/UI_Manager.h>

#include <Slash/Core/ISession.h>
#include <Slash/Core/ILibrary.h>
#include <Slash/UI/IUI.h>

#include <Slash/Tools/Managers.h>

#include <inlib/sys/plugin>
#include <inlib/system>

#include <inlib/cast>

OnX::UI_Manager::UI_Manager(Slash::Core::ISession& aSession)
:fSession(aSession)
,fName(Slash_UI_Manager)
,fUI(0)
{}

OnX::UI_Manager::~UI_Manager(){
  if(fUI) {
    fUI->finalize();
    delete fUI;
  }
  fUI = 0;
}

std::string OnX::UI_Manager::name() const {return fName;}

void* OnX::UI_Manager::cast(const std::string& a_class) const {
  INLIB_IF_CAST(Slash::UI::IUI_Manager)
  else INLIB_IF_CAST(Slash::Core::IManager)
  else return 0;
}

bool OnX::UI_Manager::add(Slash::UI::IUI* aUI) {
  if(fUI) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::add :"
        << " a UI already exist." 
        << std::endl;
    return false;
  }
  fUI = aUI; //take ownership.
  return true;
}

Slash::UI::IUI* OnX::UI_Manager::find(const std::string&) const {return fUI;}

#include <inlib/app>

Slash::UI::IUI* OnX::UI_Manager::create(
 const std::string& aDriver
,const std::string& aFile
,const std::vector<std::string>& aArgs
,bool aShow
){
  // For the moment only one Slash::UI::IUI handled.
  if(fUI) return fUI; //Done.

  std::string driver = aDriver;
  if(driver.empty()) {
    inlib::getenv("OSC_GUI_TOOLKIT",driver);
    if(driver.empty()) {
      inlib::getenv("ONX_DRIVER",driver);
    }
  }

  if((driver.empty())||((driver=="default"))) { // Guess something :
#if defined(WIN32)
    driver = "Win";
#else
#ifdef __APPLE__
    bool is_mac_app = false;
   {typedef std::pair<std::string,std::string> arg_t;
    std::vector<arg_t> args = fSession.arguments();
    if(args.size()) {
      is_mac_app = inlib::is_mac_app(args[0].first);
    }}

    if(is_mac_app) {

      if(inlib::plugin::is_valid("OnXNextStep",fSession.cout())) {
        driver = "NextStep";
      } else if(inlib::plugin::is_valid("OnXXt",fSession.cout())) {
        driver = "Xt";
      } else if(inlib::plugin::is_valid("OnXQt",fSession.cout())) {
        driver = "Qt";
      } else if(inlib::plugin::is_valid("OnXGtk",fSession.cout())) {
        driver = "Gtk";
      } else if(inlib::plugin::is_valid("OnXSDL",fSession.cout())) {
        driver = "SDL";
      } else {
        driver = "Xt";
      }

    } else {

      if(inlib::plugin::is_valid("OnXXt",fSession.cout())) {
        driver = "Xt";
      } else if(inlib::plugin::is_valid("OnXQt",fSession.cout())) {
        driver = "Qt";
      } else if(inlib::plugin::is_valid("OnXNextStep",fSession.cout())) {
        driver = "NextStep";
      } else {
        driver = "Xt";
      }

    }

#else //Linux
    if(inlib::plugin::is_valid("OnXGtk",fSession.cout())) {
      driver = "Gtk";
    } else if(inlib::plugin::is_valid("OnXXt",fSession.cout())) {
      driver = "Xt";
    } else if(inlib::plugin::is_valid("OnXQt",fSession.cout())) {
      driver = "Qt";
    } else {
      driver = "Gtk";
    }
#endif
#endif
  }

  std::string library = "OnX"+driver;

  Slash::Core::ILibraryManager* lm = Slash::libraryManager(fSession);
  if(!lm) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " LibraryManager not found." << std::endl;
    return 0;
  }
  Slash::Core::ILibrary* dll = 
    lm->addLibrary("OnX_driver_"+driver,library,"");
  if(!dll) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " Unable to add DLL " << inlib::sout(library) 
        << std::endl;
    return 0;
  }

  if(fSession.verboseLevel()) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " Load " << inlib::sout(driver) << " driver..." 
        << std::endl;
  }

  std::string function = "OnX_create"+driver+"UI";
  Factory uiFactory = (Factory)dll->findProcedure(function);
  if(!uiFactory) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " Function " << inlib::sout(function) << " not found" 
        << " in library " << inlib::sout(library) << "." 
        << std::endl;
    return 0;
  }

  if(fSession.verboseLevel()) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " Load file " << inlib::sout(aFile) << "."
        << std::endl;
  }

  fUI = uiFactory(fSession,aArgs);
  if(!fUI) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " Creation of UI for " << inlib::sout(aFile) << " failed." 
        << std::endl;
    return 0;
  }

  if(!fUI->load("",aFile,true,aShow)) {
    delete fUI;
    fUI = 0; // To redirect output to stdout before the below print.
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " Problem when creating the UI for file " 
        << inlib::sout(aFile) << "." 
        << std::endl;
    return 0;
  }

  if(fSession.verboseLevel()) {
    std::ostream& out = fSession.cout();
    out << "OnX::UI_Manager::create :"
        << " loading " << inlib::sout(aFile) << " ok." 
        << std::endl;
  }

  return fUI;
}

