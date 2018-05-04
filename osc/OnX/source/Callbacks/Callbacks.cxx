//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

//
//  This file does not need HEPVis and Inventor to compile.
//

#include <inlib/os>

//Slash:
#include <Slash/Core/IWriter.h>
#include <Slash/UI/ICallbackData.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/UI/IConsole.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Data.h>
#include <Slash/Tools/Session.h>
#include <Slash/Tools/UI.h>

#include "inlib"
#include <inlib/sys/dir>
#include <inlib/cloner>
#include <inlib/args>
#include <inlib/sjust>

#include "AccessorAccessor.h"
#include "SceneGraphAccessor.h"
#include "SoNodeAccessor.h"

#include <cstdlib> //exit

#define MAXIMUM(a,b) ((a)>(b)?a:b)

inline std::string format_suffix(const std::string& a_format) {
  if(a_format.substr(0,3)=="gl2") {
    return a_format.substr(3,a_format.size()-3);    
  } else {
    return a_format;
  }
}

typedef std::vector<std::string> args_t;

extern "C" {

void OnX_declare_accessors(Slash::UI::IUI& aUI); 

//////////////////////////////////////////////////////////////////////////////
//DEPRECATED :
// use : 
//    OnX OnX_declare_accessors
//    OnX session_set_output @{session.output}@

void OnX_Initialize(Slash::UI::IUI& aUI) {
  OnX_declare_accessors(aUI);
  aUI.executeScript("DLD","OnX session_set_output @{session.output}@");
}

//////////////////////////////////////////////////////////////////////////////
/// sys //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_sys_os(Slash::UI::IUI& aUI) {
  aUI.echo(inlib::os());
}

void OnX_sys_copy(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  std::string from;
  inlib::file_name(aArgs[0],from);
  std::string to;
  inlib::file_name(aArgs[1],to);
  inlib::file::cp(from,to);
}

void OnX_sys_remove(Slash::UI::IUI& aUI,const args_t& aArgs){
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  std::string name;
  inlib::file_name(aArgs[0],name);
  ::remove(name.c_str());
}

void OnX_sys_rm(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  inlib::file::rm(aArgs[0]);
}

void OnX_sys_pwd(Slash::UI::IUI& aUI) {
  std::ostream& out = aUI.session().cout();
  std::string s;
  if(!inlib::dir::pwd(s)) {
    out << "sys_pwd :" 
        << "Can't get current directory."
        << std::endl;
    return;
  }
  out << s << std::endl;
}

void OnX_sys_rmdir(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  std::string name;
  inlib::file_name(aArgs[0],name);
  if(!inlib::dir::rmdir(name)) {
    std::ostream& out = aUI.session().cout();
    out << "sys_remove_directory :" 
        << "Can't remove directory " << inlib::sout(aArgs[0]) << "."
        << std::endl;
    return;
  }
}

void OnX_sys_env(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  std::ostream& out = aUI.session().cout();
  std::string s;
  if(!inlib::getenv(aArgs[0],s)) {
    out << "sys_env :" 
        << " environment variable " << inlib::sout(aArgs[0]) << " not defined."
        << std::endl;
    return;
  }
  out << s << std::endl;
}

void OnX_sys_putenv(Slash::UI::IUI& aUI,const args_t& aArgs){
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  if(!inlib::putenv(aArgs[0],last)) {
    std::ostream& out = aUI.session().cout();
    out << "sys_putenv :" 
        << " can't set environment variable " << inlib::sout(aArgs[0])
        << " with value " << inlib::sout(last) << "."
        << std::endl;
    return;
  }
}

void OnX_sys_execute(Slash::UI::IUI& aUI,const args_t& aArgs){
  // args[0,n] command
  std::string last;
  if(!inlib::check_min(aArgs,1,last,aUI.session().cout())) return;
  inlib::execute(last);
}

//////////////////////////////////////////////////////////////////////////////
/// lib //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void OnX_lib_write_file(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] : file name
  // aArgs[1,n] : text : list of lines separated with \n
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  const std::string& file = aArgs[0];
  std::string name;
  inlib::file_name(file,name);
  if(!inlib::file::write(name,last)) {
    std::ostream& out = aUI.session().cout();
    out << "lib_write_file :"
        << " can't write in " << inlib::sout(file)
        << std::endl;
    return;
  }
  //std::ostream& out = aUI.session().cout();
  //out << "lib_write_file :"
  //    << " file " << inlib::sout(file) << " written."
  //    << std::endl;
}

void OnX_lib_clone(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  const std::string& stemplate = aArgs[0];
  const std::string& pack = aArgs[1];
  std::string dir;
  inlib::file_name(stemplate,dir);
  std::string path,name,suffix;
  inlib::path_name_suffix(dir,path,name,suffix);
  if(inlib::cloner::clone(dir,name,pack,aUI.session().out())) 
    aUI.echo("Cloning success.");
  else 
    aUI.echo("Cloning failure.");
}

//////////////////////////////////////////////////////////////////////////////
/// session //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_session_exit(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // The below exit may call some atexit function (for exa with gtk)
  // that may do some print. We have to switch off OnX output.
  aUI.session().redirectOutput(false);

  int code = 0;
  if(aArgs.size()==1) {
    if(!inlib::to<int>(aArgs[0],code)) code = 0;
  }
  ::exit(code);
}

void OnX_session_set_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what
  // args[1,n] value
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  aUI.session().setParameter(aArgs[0],last);
}

void OnX_session_remove_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  aUI.session().removeParameter(aArgs[0]);
}

void OnX_session_printer(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  const std::string& a_0 = aArgs[0];
  if(a_0=="enable") {
    aUI.session().printer().enable();
  } else if(a_0=="disable") {
    aUI.session().printer().disable();
  } else if(a_0=="flush") {
    aUI.session().printer().flush();
  } else {
    std::ostream& out = aUI.session().cout();
    out << "session_printer :" 
        << " " << inlib::sout(a_0) << " is a bad keyword."
        << std::endl;
    return;
  }
}

void OnX_session_set_output(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] [UI,terminal,defout]

  //  The defaults in OnX::Session constructor are such that
  // session.printer is enabled (on the UI) and
  // std[out,err],std::[cout,cerr] are not redirected 
  // to session.printer (then they still write on terminal).
  //  Then :
  // - session.output=UI redirect all on UI.
  // - session.output=terminal redirect all on the terminal.
  // - session.output=defout redirect only session.printer to UI
  //   which is then the default of OnX::Session constructor.  

  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  const std::string& arg0 = aArgs[0];
  Slash::Core::ISession& session = aUI.session();
  if(arg0=="UI") { // OnX::Session default
    if(!aUI.isComponent("consoleText")) { 
      ::printf("OnX : session_set_output :\n");
      ::printf(" output wanted to be \"UI\" but\n");
      ::printf(" there is no \"consoleText\" widget around.\n");
      ::printf(" Then output are going to be send on termminal.\n");
      session.redirectOutput(false); //it does a session.printer().disable()
    } else {
      session.redirectOutput(true); //it does a session.printer().enable()
    }
  } else if(arg0=="terminal") {
    session.redirectOutput(false); //it does a session.printer().disable()
  } else if(arg0=="defout") {
    session.redirectOutput(false); //it does a session.printer().disable()
    session.printer().enable();
  } else {
    ::printf("session_set_output : unknown keyword \"%s\"\n",arg0.c_str());
    // hoping that the upper ::printf still work !
  }
}

void OnX_session_redirect_output(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,1,out)) return;
  bool value;
  if(!inlib::to(out,aArgs[0],value)) return;
  aUI.session().redirectOutput(value);
}

void OnX_session_toggle_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what

  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::Core::ISession& session = aUI.session();
  bool value;
  if(!Slash::parameter(session,aArgs[0],value)) return;
  if(value) session.setParameter(aArgs[0],"false");
  else session.setParameter(aArgs[0],"true");
}

void OnX_session_toggle_verbose(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();
  session.setVerboseLevel(session.verboseLevel()==0?1:0);
}

void OnX_session_toggle_helpOutput(Slash::UI::IUI& aUI) {
  Slash::Core::ISession& session = aUI.session();
  std::string svalue;
  if(!session.parameterValue("session.helpOnDialog",svalue)) return;
  session.setParameter("session.helpOnDialog",svalue=="yes"?"no":"yes");
}

void OnX_session_load(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] library
  // args[1] package
  if(aArgs.size()!=2) return;
  aUI.session().load(aArgs[0],aArgs[1]);
}

void OnX_session_load_styles(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] style file
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::IStyleManager* styleManager = Slash::styleManager(aUI.session());
  if(!styleManager) return;
  styleManager->loadFile(aArgs[0]);
}

void OnX_session_modeling_none(Slash::UI::IUI& aUI) {
  Slash::modeling_none(aUI.session());
}

void OnX_session_dump_parameters(Slash::UI::IUI& aUI) {
  std::ostream& out = aUI.session().cout();
  std::vector<std::string> keys = aUI.session().availableParameters();
  unsigned int l = 0;
  for(unsigned int index=0;index<keys.size();index++) {
    const std::string& key = keys[index];
    l = MAXIMUM(l,key.size());
  }
  for(unsigned int index=0;index<keys.size();index++) {
    const std::string& key = keys[index];
    std::string value;
    aUI.session().parameterValue(key,value);
    out << inlib::sjust(key,l) << " = " << value << std::endl;
  }
}

void OnX_session_flush(Slash::UI::IUI& aUI) {
  aUI.session().flush();
}

void OnX_session_destroy_manager(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] manager
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  aUI.session().destroyManager(aArgs[0]);
}

//////////////////////////////////////////////////////////////////////////////
/// ui ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_ui_do_nothing(Slash::UI::IUI&) {}
void OnX_ui_steer(Slash::UI::IUI& aUI) {aUI.steer();}
void OnX_ui_synchronize(Slash::UI::IUI& aUI) {aUI.synchronize();}

void OnX_ui_exit(Slash::UI::IUI& aUI,const args_t& aArgs) {
  int exitCode = 0;
  if(aArgs.size()==1) {
    int l;
    if(inlib::to<int>(aArgs[0],l)) {
      exitCode = l;
    } else {
      aUI.echo("ui_exit : "+aArgs[0]+" not a number.");
    }
  }
  aUI.exit(exitCode);
}

void OnX_ui_break_modal(Slash::UI::IUI& aUI) {aUI.exitModal();}

void OnX_ui_echo(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::string last;
  if(!inlib::check_min(aArgs,0,last,aUI.session().cout())) return;
  aUI.echo(last);
}

void OnX_ui_echo_file(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] : file name.
  if(!aArgs.size()) return;
  ui_echo_file(aUI,aArgs[0]);
}

void OnX_ui_warn(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::string last;
  if(!inlib::check_min(aArgs,0,last,aUI.session().cout())) return;
  aUI.warn(last);
}

void OnX_ui_ask(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::string last;
  if(!inlib::check_min(aArgs,0,last,aUI.session().cout())) return;
  aUI.echo(aUI.ask(last));
}

void OnX_ui_set_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what
  // args[1,n] value
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  aUI.setParameter(aArgs[0],last);
}

void OnX_ui_toggle_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] what
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  bool value;
  if(!Slash::parameter(aUI,aArgs[0],value)) return;
  if(value) aUI.setParameter(aArgs[0],"false");
  else aUI.setParameter(aArgs[0],"true");
}

/* try to use @tree_items@<list>@
void OnX_ui_set_tree(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] <widget>.items
  // args[1,n] list of items.
  if(!inlib::check_min_args(aArgs,1,aUI.session().cout())) return;
  std::string sout = "<tree>";
  for(unsigned int index=1;index<aArgs.size();index++) {
    sout += "<treeItem><label>"+aArgs[index]+"</label></treeItem>";
  }
  sout += "</tree>";
  aUI.setParameter(aArgs[0],sout);
}
*/

void OnX_ui_parameterValue(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget.parameter
  if(!aArgs.size()) return;
  std::string value;
  if(!aUI.parameterValue(aArgs[0],value)) return;
  aUI.echo(value);
}

void OnX_ui_execute_file(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] interpreter
  // aArgs[1] file
  if(aArgs.size()!=2) return;
  ui_execute_file(aUI,aArgs[0],aArgs[1]);
}

void OnX_ui_load(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] parent
  // args[1] file
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  aUI.load(aArgs[0],aArgs[1]);
}

void OnX_ui_show_dialog(Slash::UI::IUI& aUI,const args_t& aArgs) {
  //used in SWIG/Slash.i
  // args[0] file
  // args[1] dialog

  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  Slash::UI::IWidget* main = aUI.findWidgetByAttribute("class","Window");
  if(!main) {
    std::ostream& out = aUI.session().cout();
    out << "ui_show_dialog :" 
        << " Can't find a Window to attach the dialog."
        << std::endl;
    return;
  }
  if(!aUI.load(*main,aArgs[0])) return;
  Slash::UI::IWidget* w = aUI.findWidget(aArgs[1]);
  if(w) w->show();
}

void OnX_ui_hide(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!aArgs.size()) return;
  Slash::UI::IWidget* w = aUI.findWidget(aArgs[0]);
  if(w) w->hide();
}

void OnX_ui_show(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!aArgs.size()) return;
  Slash::UI::IWidget* w = aUI.findWidget(aArgs[0]);
  if(w) w->show();
}

void OnX_ui_show_shell(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  const std::string& component = aArgs[0];
  std::vector<std::string> found = aUI.findParent(component,"shell","");
  if(found.size()) {
    const std::string& shell = found[0];
    Slash::UI::IWidget* w = aUI.findWidget(shell);
    if(w) w->show();
  }
}

//TO_REVISIT
void OnX_ui_help_text(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs : text to print in the help dialog (or terminal).
  //         It is a list of strings separated by \n.
  std::string last;
  if(!inlib::check_min(aArgs,0,last,aUI.session().cout())) return;
  inlib::replace(last,"\\n","\n");
  ui_help_text(aUI,last);
}

//TO_REVISIT
void OnX_ui_help_file(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] : file name.
  if(!aArgs.size()) return;
  ui_help_file(aUI,aArgs[0]);
}

void OnX_ui_set_file_chooser(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] default directory
  // args[1] dirmask
  // args[2] [open,save]
  // args[3] interp
  // args[4,n] script passed to the ok callback.

  std::string last;
  if(!inlib::check_min(aArgs,5,last,aUI.session().cout())) return;

  aUI.setParameter("fileChooser.dirMask",aArgs[1]); //Must come first.
  aUI.setParameter("fileChooser.directory",aArgs[0]);
  aUI.setParameter("fileChooser.mode",aArgs[2]);

  aUI.setCallback("fileChooser","ok","DLD","OnX ui_hide fileChooser",0);
  aUI.setCallback("fileChooser","ok",aArgs[3],last,1);
  aUI.setCallback("fileChooser","cancel","DLD","OnX ui_hide fileChooser");
}

void OnX_ui_set_callback(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // args[1] callback
  // args[2] interpreter
  // args[3,n] script
  std::string last;
  if(!inlib::check_min(aArgs,4,last,aUI.session().cout())) return;
  if(!aUI.setCallback(aArgs[0],aArgs[1],aArgs[2],last)) {
    std::ostream& out = aUI.session().cout();
    out << "ui_set_callback :" 
        << " Can't set callback " << inlib::sout(aArgs[1])
        << " on widget " << inlib::sout(aArgs[0])
        << " with interp " << inlib::sout(aArgs[2])
        << " and value " << inlib::sout(last)
        << std::endl;
  }
}

/*
void OnX_ui_add_callback(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // args[1] callback
  // args[2] interpreter
  // args[4] : script
  if(!inlib::check_args(aArgs,4,aUI.session().cout())) return;
  if(!aUI.setCallback(aArgs[0],aArgs[1],aArgs[2],aArgs[3],-1)) {
    std::ostream& out = aUI.session().cout();
    out << "ui_set_callback :" 
        << " Can't set callback " << inlib::sout(aArgs[1])
        << " on widget " << inlib::sout(aArgs[0])
        << " with interp " << inlib::sout(aArgs[2])
        << " and value " << inlib::sout(aArgs[3])
        << std::endl;
  }
}
*/

void OnX_ui_execute_callbacks(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  aUI.executeCallbacks(aArgs[0],aArgs[1]);
}

void OnX_ui_execute_script(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  aUI.executeScript(aArgs[0],last);
}

void OnX_ui_save(Slash::UI::IUI& aUI) {aUI.save();}

void OnX_ui_add_prompt(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // Add a prompt in the consolePrompt list.
  if(!aArgs.size()) return;
  Slash::add_prompt(aUI,aArgs[0]);
}

void OnX_ui_lock_screen(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] password
  // args[1] interp when unlocking
  // args[2] script when unlocking
  if(!inlib::check_args(aArgs,3,aUI.session().cout())) return;
  aUI.lockScreen(aArgs[0],aArgs[1],aArgs[2]);
}

void OnX_ui_set_current_widget(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  const std::string& name = aArgs[0];
  Slash::UI::IWidget* widget = aUI.findWidget(name);
  if(!widget) {
    std::ostream& out = aUI.session().cout();
    out << "ui_set_current_widget :" 
        << "Can't find widget " << inlib::sout(name)
        << std::endl;
    return;
  }
  aUI.setCurrentWidget(widget);
}

void OnX_ui_create_component(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  // args[1] class
  // args[2] parent
  if(!inlib::check_args(aArgs,3,aUI.session().cout())) return;
  aUI.createComponent(aArgs[0],aArgs[1],aArgs[2]);
}

void OnX_ui_copy_component(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  // args[1] from
  // args[2] parent
  if(!inlib::check_args(aArgs,3,aUI.session().cout())) return;
  aUI.copyComponent(aArgs[0],aArgs[1],aArgs[2]);
}

void OnX_ui_destroy_component(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  aUI.destroyComponent(aArgs[0]);
}

void OnX_ui_destroy_widget(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
  if(!widget) return;
  aUI.destroyWidget(widget);
}

void OnX_ui_enable_terminal(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] interp
  // args[1,n] prompt
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  aUI.disableTerminal();
  aUI.enableTerminal(last,aArgs[0]);
}

void OnX_ui_disable_terminal(Slash::UI::IUI& aUI) {
  aUI.disableTerminal();
}

//////////////////////////////////////////////////////////////////////////////
/// widget ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_widget_find(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget name (or @current@)
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  // @current@ then not treated.
  Slash::UI::IWidget* widget = aUI.findWidget(aArgs[0]);
  std::ostream& out = aUI.session().cout();
  if(widget) {
    out << "widget "<< inlib::sout(aArgs[0]) << " found." << std::endl;
  } else {
    out << "widget "<< inlib::sout(aArgs[0]) << " not found." << std::endl;
  }
}

void OnX_widget_write(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget name (or @current@)
  // args[1] file
  // args[2] format
  // args[3] options
  if(!inlib::check_args(aArgs,4,aUI.session().cout())) return;
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
  if(!widget) return;
  if(!widget->write(aArgs[1],aArgs[2],aArgs[3])) {
    std::ostream& out = aUI.session().cout();
    out << "widget_write : "
        << inlib::sout(aArgs[0]) << " failed."
        << std::endl;
    return;
  }
}

void OnX_widget_manage(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget name (or @current@)
  // args[1] yes/no
  std::ostream& out = aUI.session().cout();
  if(!inlib::check_args(aArgs,2,out)) return;
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
  if(!widget) return;
  bool value;
  if(!inlib::to(out,aArgs[1],value)) return;
  if(!widget->manage(value)) {
    std::ostream& out = aUI.session().cout();
    out << "widget_manage : "
        << inlib::sout(aArgs[1]) << " failed."
        << std::endl;
    return;
  }
}

void OnX_widget_set_parameter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget name (or @current@)
  // args[1] parameter
  // args[2] value
  std::string last;
  if(!inlib::check_min(aArgs,3,last,aUI.session().cout())) return;
  Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[0]);
  if(!widget) return;
  if(!widget->setParameter(aArgs[1],last)) {
    std::ostream& out = aUI.session().cout();
    out << "widget_set_parameter : "
        << " for widget " << inlib::sout(aArgs[0]) 
        << " set parameter " << inlib::sout(aArgs[1])
        << " with value " << inlib::sout(last) << " failed."
        << std::endl;
    return;
  }
}

//////////////////////////////////////////////////////////////////////////////
/// data /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// used in oroot_root.cxx :
void OnX_declare_accessors_sess(Slash::Core::ISession& aSession) {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(accessorManager) {
    if(!accessorManager->findAccessor("Lib_Accessor"))
      accessorManager->addAccessor(new OnX::AccessorAccessor(aSession));
    if(!accessorManager->findAccessor("SceneGraph"))
      accessorManager->addAccessor(new OnX::SceneGraphAccessor(aSession));
    if(!accessorManager->findAccessor("SoNode"))
      accessorManager->addAccessor(new OnX::SoNodeAccessor(aSession));
  }
}

void OnX_declare_accessors(Slash::UI::IUI& aUI) {
  OnX_declare_accessors_sess(aUI.session());
}

void OnX_data_exec(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_min_args(aArgs,1,aUI.session().cout())) return;

  Slash::Data::IProcessor* da = Slash::accessorManager(aUI.session());
  if(!da) return;

  std::vector<std::string> args;
  for(unsigned int index=1;index<aArgs.size();index++) {
    args.push_back(aArgs[index]);
  }

  da->execute(aArgs[0],args);
}

void OnX_data_collect(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_or_args(aArgs,1,2,aUI.session().cout())) return;
  if(aArgs.size()==1) {
    Slash::Data::collect(aUI.session(),aArgs[0]);
  } else {
    Slash::Data::collect(aUI.session(),aArgs[0],aArgs[1]);
  }
}

void OnX_data_visualize(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] where to visualize in.
  // optional :
  // args[1] style.
  if(!inlib::check_min_args(aArgs,1,aUI.session().cout())) return;
  std::string style = (aArgs.size()>=2)?aArgs[1]:"";
  Slash::Data::visualize(aUI.session(),aArgs[0],style);
}

void OnX_data_filter(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::Data::filter(aUI.session(),aArgs[0]);
}

void OnX_data_number(Slash::UI::IUI& aUI) {
  Slash::Data::number(aUI.session());
}

void OnX_data_dump(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // optional :
  //  aArgs[0] = format (table,raw) def=table
  //  aArgs[1] = where def=terminal 
  if(aArgs.size()==0) {
    Slash::Data::dump(aUI.session());
  } else if(aArgs.size()==1) {
    Slash::Data::dump(aUI.session(),aArgs[0]);
  } else if(aArgs.size()==2) {
    Slash::UI::IWidget* widget = Slash::find_widget(aUI,aArgs[1]);
    if(!widget) {
      std::ostream& out = aUI.session().cout();
      out << "OnX_data_dump :" 
          << " widget " << inlib::sout(aArgs[1]) << " not found."
          << std::endl;
      return;
    }
    Slash::UI::IConsole* console = INLIB_CAST(*widget,Slash::UI::IConsole);
    if(!console) {
      std::ostream& out = aUI.session().cout();
      out << "OnX_data_dump :" 
          << " widget " << inlib::sout(aArgs[1]) << " not a IConsole."
          << std::endl;
      return;
    }
    console->beginOut();
    Slash::Data::dump(aUI.session(),console->out(),aArgs[0]);
    console->endOut();
  } else {
    std::ostream& out = aUI.session().cout();
    out << "data_dump :" 
        << "zero, one or two argument expected."
        << std::endl;
  }
}

void OnX_data_set(Slash::UI::IUI& aUI,const args_t& aArgs) {
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  Slash::Data::set(aUI.session(),aArgs[0],last);
}

void OnX_data_init(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::Data::init(aUI.session(),aArgs[0]);
}

void OnX_data_destroy(Slash::UI::IUI& aUI) {
  Slash::Data::destroy(aUI.session());
}

void OnX_data_clear(Slash::UI::IUI& aUI){
  Slash::Data::IProcessor* mgr = Slash::accessorManager(aUI.session());
  if(!mgr) return;
  mgr->eraseHandlers();
}

//////////////////////////////////////////////////////////////////////////////
/// cyclic ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_cyclic_create(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  // args[1] time (milisec)
  // args[2] start
  // args[3] interp
  // args[4,n] script
  std::ostream& out = aUI.session().cout();
  std::string last;
  if(!inlib::check_min(aArgs,5,last,out)) return;

  int delay = 0;
  if(!inlib::to<int>(out,aArgs[1],delay)) return;
  bool start;
  if(!inlib::to(out,aArgs[2],start)) return;

  Slash::UI::ICyclic* cyclic = aUI.findCyclic(aArgs[0]);
  if(cyclic) {
    std::ostream& out = aUI.session().cout();
    out << "cyclic_create :" 
        << " cyclic " << inlib::sout(aArgs[0]) << " already exists."
        << std::endl;
    return;
  }
  cyclic = aUI.createCyclic(aArgs[0],delay,aArgs[3],last);
  if(!cyclic) {
    std::ostream& out = aUI.session().cout();
    out << "cyclic_create :" 
        << " can't create " << inlib::sout(aArgs[0]) << "."
        << std::endl;
    return;
  }
  if(start) cyclic->start();
}

void OnX_cyclic_destroy(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ICyclic* cyclic = aUI.findCyclic(aArgs[0]);
  if(!cyclic) return;
  aUI.removeCyclic(cyclic);
}

void OnX_cyclic_stop(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ICyclic* cyclic = aUI.findCyclic(aArgs[0]);
  if(!cyclic) return;
  cyclic->stop();
}

void OnX_cyclic_start(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ICyclic* cyclic = aUI.findCyclic(aArgs[0]);
  if(!cyclic) return;
  cyclic->start();
}

void OnX_cyclic_toggle(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  Slash::UI::ICyclic* cyclic = aUI.findCyclic(aArgs[0]);
  if(!cyclic) return;
  if(cyclic->started()) {
    cyclic->stop();
  } else {
    cyclic->start();
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_tree_set(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] <widget>.items
  // args[1] list of items separated by \n
  // Example : tree_set mainTree.items aa\nbb
  std::string last;
  if(!inlib::check_min(aArgs,2,last,aUI.session().cout())) return;
  tree_set(aUI,aArgs[0],last);
}

void OnX_tree_set_with_file(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  tree_set_with_file(aUI,aArgs[0],aArgs[1]);
}

//////////////////////////////////////////////////////////////////////////////
/// currents /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void OnX_ui_add_in_currents(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget to declare in the "currents" list.
  // args[1] pdm to maintain the list of potential current widgets.
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  const std::string& name = aArgs[0];
  const std::string& pdm = aArgs[1];
  if(!aUI.isComponent(pdm)) return;
  std::string s = pdm+"_item_" + name;
  aUI.createComponent(s,"MenuItem",pdm);
  aUI.setParameter(s+".label",name);
  aUI.setCallback(s,"activate","DLD","OnX ui_set_current_widget "+name,0);
  aUI.setCallback(s,"activate","DLD","OnX ui_show_shell "+name,1);
}

void OnX_ui_change_current(Slash::UI::IUI&,const std::vector<std::string>&);

void OnX_ui_remove_from_currents(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget to remove from the "currents" list.
  // args[1] pdm to maintain the list of potential current widgets.
  if(!inlib::check_args(aArgs,2,aUI.session().cout())) return;
  const std::string& name = aArgs[0];
  const std::string& pdm = aArgs[1];
  if(aUI.isComponent(pdm)) aUI.destroyComponent(pdm+"_item_" + name);
  OnX_ui_change_current(aUI,aArgs);
}

void OnX_ui_change_current(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] widget
  // optional :
  // args[1] pdm to maintain the list of potential current widgets.
  if(!inlib::check_or_args(aArgs,1,2,aUI.session().cout())) return;
  Slash::UI::IWidget* w = aUI.currentWidget();
  if(!w) return;
  const std::string& name = aArgs[0];
  if(w->name()!=name) return;
  aUI.setCurrentWidget(0);
  //The widget was the current widget.
  //We have to set the current widget to another one.
  //We take the last entry in the passed viewers pdm.
  if(!aArgs.size()) return;
  const std::string& pdm = aArgs[1];
  std::vector<std::string> items = aUI.findChildren(pdm,"name","");
  if(!items.size()) return;
  std::string s = items[items.size()-1];
  inlib::replace(s,pdm+"_item_","");
  w = aUI.findWidget(s);
  aUI.setCurrentWidget(w);
}

void OnX_ui_add_in_tab_stack(Slash::UI::IUI& aUI,const args_t& aArgs) {
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  const std::string& name = aArgs[0];
  std::vector<std::string> found = aUI.findParent(name,"name","");
  if(!found.size()||!found[0].size()) return;
  const std::string& parent = found[0];
  std::string sclass;
  if(!aUI.parameterValue(parent+".class",sclass)) return;
  if(sclass!="TabStack") return;
  aUI.setParameter(name+".tabLabel",name);  
}

void OnX_ui_tab_stack_write(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] tab stack name
  // args[1] file prefix
  // args[2] format
  // args[3] options
  if(!inlib::check_args(aArgs,4,aUI.session().cout())) return;

  const std::string& tab_stack = aArgs[0];  
  //FIXME : check we have a TabStack.

  std::string selection;
  if(!aUI.parameterValue(tab_stack+".selection",selection)) {
    std::ostream& out = aUI.session().cout();
    out << "OnX_ui_tab_stack_write : "
        << " can't get selection of " << inlib::sout(tab_stack)
        << std::endl;
  }

  std::vector<std::string> items = aUI.findChildren(tab_stack,"name","");

  std::vector<std::string>::iterator it;
  for(it=items.begin();it!=items.end();++it) {
    Slash::UI::IWidget* widget = aUI.findWidget(*it);
    if(!widget) continue; //ItemML being not a widget.
    
    //to have the widget visible.
    if(!aUI.setParameter(tab_stack+".selection",*it)) {
      std::ostream& out = aUI.session().cout();
      out << "OnX_ui_tab_stack_write : "
          << " can't select " << inlib::sout(*it)
          << std::endl;
      continue;
    }
    aUI.synchronize();

    const std::string& format = aArgs[2];
    std::string file;
    if(aArgs[1].size()) {
      file = aArgs[1]+(*it)+"."+format_suffix(format);
    } else {
      file = (*it)+"."+format_suffix(format);
    }
    if(!widget->write(file,format,aArgs[3])) {
      std::ostream& out = aUI.session().cout();
      out << "OnX_ui_tab_stack_write : "
          << " writing " << inlib::sout(*it)
          << " in " << inlib::sout(file) << " failed."
          << std::endl;
    } else if(aUI.session().verboseLevel()){      
      std::ostream& out = aUI.session().cout();
      out << "OnX_ui_tab_stack_write : "
          << " writing " << inlib::sout(*it)
          << " in " << inlib::sout(file) << " success."
          << std::endl;
    }
  } 

  aUI.setParameter(tab_stack+".selection",selection);  
}

void OnX_ui_find_write(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] top
  // args[1] what
  // args[2] parameter
  // args[3] file prefix
  // args[4] format
  // args[5] options
  if(!inlib::check_args(aArgs,6,aUI.session().cout())) return;
  std::vector<std::string> items = 
    aUI.findChildren(aArgs[0],aArgs[1],aArgs[2]);

  std::vector<std::string>::iterator it;
  for(it=items.begin();it!=items.end();++it) {
    Slash::UI::IWidget* widget = aUI.findWidget(*it);
    if(!widget) continue; //ItemML being not a widget.
    const std::string& format = aArgs[4];
    std::string file;
    if(aArgs[3].size()) {
      file = aArgs[3]+(*it)+"."+format_suffix(format);
    } else {
      file = (*it)+"."+format_suffix(format);
    }
    if(!widget->write(file,format,aArgs[5])) {
      std::ostream& out = aUI.session().cout();
      out << "OnX_ui_find_write : "
          << " writing " << inlib::sout(*it)
          << " in " << inlib::sout(file) << " failed."
          << std::endl;
    } else if(aUI.session().verboseLevel()){      
      std::ostream& out = aUI.session().cout();
      out << "OnX_ui_find_write : "
          << " writing " << inlib::sout(*it)
          << " in " << inlib::sout(file) << " success."
          << std::endl;
    }
  } 
}

//////////////////////////////////////////////////////////////////////////////
/// etc //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// to test @cat@
void OnX_dump_args(Slash::UI::IUI& aUI,const args_t& aArgs){
  out_args(aUI,aArgs);
}

void OnX_dld_exec_file(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] file
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  std::vector<std::string> text;
  std::string name;
  inlib::file_name(aArgs[0],name);
  if(!inlib::file::read(name,text)) {
    std::ostream& out = aUI.session().cout();
    out << "dld_exec_file : "
        << " problem reading file " << inlib::sout(aArgs[0]) << "."
        << std::endl;
    return;
  }
  for(std::vector<std::string>::iterator it=text.begin();it!=text.end();++it) {
    inlib::strip(*it,inlib::trailing);
    if((*it).empty()) continue;
    if(*it=="return") break;
    aUI.executeScript("DLD",*it);
  } 
}

void OnX_dld_exec_text(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // aArgs[0] lines (string separated with \n)
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  std::vector<std::string> text = inlib::words(aArgs[0],"\n");
  for(std::vector<std::string>::iterator it=text.begin();it!=text.end();++it) {
    inlib::strip(*it,inlib::trailing);
    if((*it).empty()) continue;
    if(*it=="return") break;
    aUI.executeScript("DLD",*it);
  } 
}

} // extern "C"

#include <Lib/calc.h>

extern "C" {

/*FIXME : TimeManager ?
void OnX_time_begin(Slash::UI::IUI& aUI,const args_t& aArgs) {
  inlib::atime begin = inlib::atime::now();
}

void OnX_time_end(Slash::UI::IUI& aUI,const args_t& aArgs) {
  inlib::atime end = inlib::atime::elapsed(begin);
  std::ostream& out = aUI.session().cout();
  out << "time_end :" 
      << elapsed : " << end.seconds() << " s. "
      << std::endl;
}
*/

void OnX_lib_calc(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] formula. For exa : 2*x
  // args[1,n-1] : variable values
  if(!inlib::check_min_args(aArgs,1,aUI.session().cout())) return;
  std::vector<std::string> vals;
  for(unsigned int index=1;index<aArgs.size();index++) {
    vals.push_back(aArgs[index]);
  }
  std::string v;
  if(!Lib::calc(aUI.session().out(),aArgs[0],vals,v)) return;
  std::ostream& out = aUI.session().cout();
  out << v << std::endl;
}

}

#include <stdio.h>
#include <iostream>

extern "C" {

void OnX_std_printf(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // To test the stdout redirection.
  std::string last;
  if(!inlib::check_min(aArgs,0,last,aUI.session().cout())) return;
  ::printf("%s",last.c_str());
  ::printf("\n");
}

void OnX_std_cout(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // To test the std::cout redirection.
  std::string last;
  if(!inlib::check_min(aArgs,0,last,aUI.session().cout())) return;
  std::cout << last;
  std::cout << std::endl;
}

}

#include <Lib/Processor.h>

extern "C" {

void OnX_ui_execute_script_if(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] condition
  // args[1] interpreter
  // args[2,n] script
  std::string last;
  if(!inlib::check_min(aArgs,3,last,aUI.session().cout())) return;

  //::printf("debug : \"%s\"\n",aArgs[0].c_str());

  bool result;

 {Lib::Processor proc(aArgs[0],aUI.session().out());
  std::vector<std::string> vars;
  proc.findVariables(vars);
  //Lib::smanip::write(vars,printer);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  std::string serror;
  proc.cut(vs,result,serror);}

  //::printf("debug : if %d\n",result);

  if(result) {
    aUI.executeScript(aArgs[1],last);
  }
}

void OnX_ui_execute_script_loop(Slash::UI::IUI& aUI,const args_t& aArgs) {
  // args[0] name of the item variable
  // args[1] \n list
  // args[2] interpreter
  // args[3,n] script
  std::string last;
  if(!inlib::check_min(aArgs,4,last,aUI.session().cout())) return;

  std::vector<std::string> text = smanip_text(aArgs[1]);
  for(std::vector<std::string>::iterator it=text.begin();it!=text.end();++it) {
    aUI.session().setParameter(aArgs[0],*it);
    aUI.executeScript(aArgs[2],last);
  } 
}

}

extern "C" {

#define ALIAS(a_alias,a_new) \
void a_alias(Slash::UI::IUI& aUI,const args_t& aArgs) {\
 a_new(aUI,aArgs);\
}


ALIAS(OnX_session_redirectOutput,OnX_session_redirect_output)
ALIAS(OnX_session_setParameter,OnX_session_set_parameter)
ALIAS(OnX_set_variable,OnX_session_set_parameter)
ALIAS(OnX_ui_setCurrentWidget,OnX_ui_set_current_widget)
ALIAS(OnX_ui_executeCallbacks,OnX_ui_execute_callbacks)
ALIAS(OnX_ui_executeScript,OnX_ui_execute_script)
ALIAS(OnX_ui_setParameter,OnX_ui_set_parameter)
ALIAS(OnX_ui_setCallback,OnX_ui_set_callback)
ALIAS(OnX_ui_toggleParameter,OnX_ui_toggle_parameter)
ALIAS(OnX_ui_destroyComponent,OnX_ui_destroy_component)
ALIAS(OnX_ui_set_fileChooser,OnX_ui_set_file_chooser)
ALIAS(OnX_widget_set_current,OnX_ui_set_current_widget)

}
