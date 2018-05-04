#ifndef OnX_Helpers_OnX_h
#define OnX_Helpers_OnX_h

//
//  This file contains static helper methods.
// It is intended to be included in OnX Callbacks file
// to ease callbacks programming.
//  It relies on interfaces and basic libraries (Lib, stdcxx) only.
//

// This file contains no Inventor related helpers.
// See OnX/Helpers/Inventor.h for Inventor related ones.

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Core/IWriter.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ICallbackData.h>
#include <Slash/Tools/Managers.h>

// Lib :
#include <Lib/fmanip.h>
#include <Lib/smanip.h>
#include <Lib/Cast.h>
#include <Lib/Out.h>
#include <Lib/sout.h>
#include <Lib/System.h>
#include <Lib/Env.h>

//////////////////////////////////////////////////////////////////////////////
/// lib //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline std::string lib_torgb(const std::string& aColorName){
  std::string value;
  Lib::smanip::torgbs(aColorName,value);
  return value;
}

//////////////////////////////////////////////////////////////////////////////
/// session //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void session_setColor(Slash::Core::ISession& aSession,const std::string& aColor){aSession.setParameter("modeling.color",aColor);}

inline void session_modeling_none(Slash::Core::ISession& aSession){
  // Must match list of SbStyle::setFromString.
  aSession.setParameter("modeling.modeling","none");
  aSession.setParameter("modeling.color","none");
  aSession.setParameter("modeling.highlightColor","none");
  aSession.setParameter("modeling.lineStyle","none");
  aSession.setParameter("modeling.linePattern","none");
  aSession.setParameter("modeling.markerStyle","none");
  aSession.setParameter("modeling.areaStyle","none");
  aSession.setParameter("modeling.cut","none");
  aSession.setParameter("modeling.painting","none");
  aSession.setParameter("modeling.projection","none");
  aSession.setParameter("modeling.fontName","none");
  aSession.setParameter("modeling.lineWidth","none");
  aSession.setParameter("modeling.markerSize","none");
  aSession.setParameter("modeling.fontSize","none");
  aSession.setParameter("modeling.transparency","none");
  aSession.setParameter("modeling.multiNodeLimit","none");
  aSession.setParameter("modeling.hatching","none");
  aSession.setParameter("modeling.angle","none");
  aSession.setParameter("modeling.offset","none");
  aSession.setParameter("modeling.stripWidth","none");
  aSession.setParameter("modeling.spacing","none");
  aSession.setParameter("modeling.visible","none");
  aSession.setParameter("modeling.barOffset","none");
  aSession.setParameter("modeling.barWidth","none");

  aSession.setParameter("modeling.localSetup","none");
  // Backward compatibility :
  aSession.setParameter("modeling.type","none");
}

inline bool session_setParameter(
 Slash::Core::ISession& aSession
,const std::string& aWhat
,int aValue
){
  std::string s;
  Lib::smanip::printf(s,32,"%d",aValue);
  return aSession.setParameter(aWhat,s);
}

inline bool session_setParameter(
 Slash::Core::ISession& aSession
,const std::string& aWhat
,double aValue
){
  std::string s;
  Lib::smanip::printf(s,32,"%g",aValue);
  return aSession.setParameter(aWhat,s);
}

inline bool session_setParameter(
 Slash::Core::ISession& aSession
,const std::string& aWhat
,bool aValue
){
  return aSession.setParameter(aWhat,std::string(aValue?"true":"false"));
}

inline bool session_parameterValue(
 Slash::Core::ISession& aSession
,const std::string& aWhat
,int& aValue
){
  aValue = 0;
  std::string s;
  if(!aSession.parameterValue(aWhat,s)) return false;
  int value;
  if(!Lib::smanip::toint(s,value)) return false;
  aValue = value;
  return true;
}

inline bool session_parameterValue(
 Slash::Core::ISession& aSession
,const std::string& aWhat
,double& aValue
){
  aValue = 0;
  std::string s;
  if(!aSession.parameterValue(aWhat,s)) return false;
  double value;
  if(!Lib::smanip::todouble(s,value)) return false;
  aValue = value;
  return true;
}

inline bool session_parameterValue(
 Slash::Core::ISession& aSession
,const std::string& aWhat
,bool& aValue
){
  aValue = false;
  std::string s;
  if(!aSession.parameterValue(aWhat,s)) return false;
  bool value;
  if(!Lib::smanip::tobool(s,value)) return false;
  aValue = value;
  return true;
}

inline std::string session_interpreterFromSuffix(
 Slash::Core::ISession& aSession
,const std::string& aSuffix
){
  Slash::UI::IScriptManager* sm = Slash::scriptManager(aSession);
  if(!sm) return "";
  Slash::UI::IInterpreter* interp = sm->findInterpreterFromSuffix(aSuffix);
  if(!interp) return "";
  return interp->name();
}

//////////////////////////////////////////////////////////////////////////////
/// ui ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void ui_echo_file(Slash::UI::IUI& aUI,const std::string& aFile) {
  std::vector<std::string> text;
  if(!Lib::fmanip::read(aFile,text)) return;
  Slash::Core::ISession& session = aUI.session();
  unsigned int textn = text.size();
  for(unsigned int index=0;index<textn;index++) {
    session.printer().write(text[index]);
    session.printer().write("\n");
  }
}

inline void ui_help_text(Slash::UI::IUI& aUI,const std::string& aString) {
  // aString : text to output in the help dialog (or terminal).
  //           It is a list of strings separated by \n.
  Slash::Core::ISession& session = aUI.session();
  std::string svalue;
  if(!session.parameterValue("session.helpOnDialog",svalue)) return;
  if(svalue=="yes") {
    // Output on the Help window (that does not need a parent) :
    aUI.load("",Lib::fmanip::name("$ONXROOT/scripts/OnX/HelpWindow.onx"));
    aUI.setParameter("OnX_HelpWindow_text.value",aString);
    Slash::UI::IWidget* w = aUI.findWidget("OnX_HelpWindow_window");
    if(w) w->show();
  } else {
    std::vector<std::string> text;
    Lib::smanip::words(aString,"\n",text,true);
    // Output on console :
    unsigned int textn = text.size();
    for(unsigned int index=0;index<textn;index++) {
      session.printer().write(text[index]);
      session.printer().write("\n");
    }
  }
}

inline void ui_help_file(Slash::UI::IUI& aUI,const std::string& aFile) {
  // aArgs[0] : file name.
  std::vector<std::string> text;
  if(!Lib::fmanip::read(aFile,text)) {
    Lib::Out out(aUI.session().printer());
    out << "ui_help_file : can't read " << Lib::sout(aFile) << "."
        << Lib::endl;
    return;
  }
  ui_help_text(aUI,Lib::smanip::tostring(text,"\n"));
}

inline void ui_execute_file(
 Slash::UI::IUI& aUI
,const std::string& aAction
,const std::string& aFile
){
  //::printf("debug : ui_execute_file : %s %s\n",
  //    aAction.c_str(),aFile.c_str());
  if(aAction=="Styles") {
    Slash::UI::IStyleManager* sm = Slash::styleManager(aUI.session());
    if(!sm) return;
    sm->loadFile(aFile);
  } else if(aAction=="view_ps") {
    std::string cmd;
    if(Lib::check_getenv(aUI.session().printer(),
                         "ONX_PS_VIEWER","ONXPSVIEW",cmd)) {
      Lib::System::execute(cmd+" "+Lib::smanip::quote(aFile));
    } else {
      //if(Lib::System::isTypeOf("darwin")) {
      //  Lib::System::execute("open "+Lib::smanip::quote(aFile));
      //} else if(Lib::System::isKindOf("Windows")) {
      //  Lib::System::execute("gsview32.exe "+Lib::smanip::quote(aFile));
      //} else {
        Lib::System::execute("ghostview "+Lib::smanip::quote(aFile));
      //}
    }
  } else if(aAction=="view_gif") {
    std::string cmd;
    if(Lib::check_getenv(aUI.session().printer(),
                       "ONX_GIF_VIEWER","ONXGIFVIEW",cmd)) {
      Lib::System::execute(cmd+" "+Lib::smanip::quote(aFile));
    } else {
      //if(Lib::System::isTypeOf("darwin")) {
      //  Lib::System::execute("open "+Lib::smanip::quote(aFile));
      //} else if(Lib::System::isKindOf("Windows")) {
      //  Lib::System::execute("mspaint "+Lib::smanip::quote(aFile));
      //} else {
        Lib::System::execute("xv "+Lib::smanip::quote(aFile));
      //}
    }
  } else if(aAction=="view_jpeg") {
    std::string cmd;
    if(Lib::check_getenv(aUI.session().printer(),
                       "ONX_JPEG_VIEWER","ONXJPEGVIEW",cmd)) {
      Lib::System::execute(cmd+" "+Lib::smanip::quote(aFile));
    } else {
      //if(Lib::System::isTypeOf("darwin")) {
      //  Lib::System::execute("open "+Lib::smanip::quote(aFile));
      //} else if(Lib::System::isKindOf("Windows")) {
      //  Lib::System::execute("mspaint "+Lib::smanip::quote(aFile));
      //} else {
        Lib::System::execute("xv "+Lib::smanip::quote(aFile));
      //}
    }

  } else if(aAction=="view_pdf") {
    std::string cmd;
    if(Lib::check_getenv(aUI.session().printer(),
                       "ONX_PDF_VIEWER","ONXPDFVIEW",cmd)) {
      Lib::System::execute(cmd+" "+Lib::smanip::quote(aFile));
    } else {
      //if(Lib::System::isTypeOf("darwin")) {
      //  Lib::System::execute("open "+Lib::smanip::quote(aFile));
      //} else if(Lib::System::isKindOf("Windows")) {
      //  Lib::System::execute("AcroRd32.exe "+Lib::smanip::quote(aFile));
      //} else {
        Lib::System::execute("xpdf "+Lib::smanip::quote(aFile));
      //}
    }
  } else if(aAction=="view_iv") {
    std::string cmd;
    if(Lib::check_getenv(aUI.session().printer(),
                       "ONX_IV_VIEWER","ONXIVVIEW",cmd)) {
      Lib::System::execute(cmd+" "+Lib::smanip::quote(aFile));
    } else {
      //if(Lib::System::isKindOf("Windows")) {
      //  Lib::System::execute("ivview.exe "+Lib::smanip::quote(aFile));
      //} else {
        Lib::System::execute("ivview "+Lib::smanip::quote(aFile));
      //}
    }

  } else {
    Slash::UI::IScriptManager* sm = Slash::scriptManager(aUI.session());
    if(!sm) return;
    Slash::UI::IInterpreter* interp = sm->findInterpreter(aAction);
    if(!interp) return;
    std::string scmd = interp->execFileCommand();
    Lib::smanip::replace(scmd,"@onx_file@",Lib::fmanip::name(aFile));
    sm->record(aAction,scmd);
    Slash::UI::IInterpreter::Options opts;
    interp->executeScript(scmd,opts);
  }
}

inline std::string ui_interpreter_suffix(Slash::UI::IUI& aUI,const std::string& aInterp){
  Slash::UI::IScriptManager* sm = Slash::scriptManager(aUI.session());
  if(!sm) return "";
  Slash::UI::IInterpreter* interp = sm->findInterpreter(aInterp);
  if(!interp) return "";
  return interp->suffix();
}

inline bool ui_setParameter(Slash::UI::IUI& aUI,const std::string& aWhat,int aValue){
  std::string s;
  Lib::smanip::printf(s,32,"%d",aValue);
  return aUI.setParameter(aWhat,s);
}

inline bool ui_setParameter(Slash::UI::IUI& aUI,const std::string& aWhat,double aValue){
  std::string s;
  Lib::smanip::printf(s,32,"%g",aValue);
  return aUI.setParameter(aWhat,s);
}

inline bool ui_setParameter(Slash::UI::IUI& aUI,const std::string& aWhat,bool aValue){return aUI.setParameter(aWhat,std::string(aValue?"true":"false"));}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,std::string& aValue){
  if(!aUI.parameterValue(aWhat,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue = "";
    return false;
  }
  return true;
}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,unsigned int& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue = 0;
    return false;
  }
  if(!Lib::smanip::touint(s,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Passed value " << Lib::sout(s)
        << " for parameter " << Lib::sout(aWhat) 
        << " not an unsigned int."
        << Lib::endl;
    return false;
  }
  return true;
}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,int& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue = 0;
    return false;
  }
  if(!Lib::smanip::toint(s,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Passed value " << Lib::sout(s)
        << " for parameter " << Lib::sout(aWhat) 
        << " not an int."
        << Lib::endl;
    return false;
  }
  return true;
}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,double& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue = 0;
    return false;
  }
  if(!Lib::smanip::todouble(s,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Passed value " << Lib::sout(s)
        << " for parameter " << Lib::sout(aWhat) 
        << " not a double."
        << Lib::endl;
    return false;
  }
  return true;
}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,float& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue = 0;
    return false;
  }
  if(!Lib::smanip::tofloat(s,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Passed value " << Lib::sout(s)
        << " for parameter " << Lib::sout(aWhat) 
        << " not a float."
        << Lib::endl;
    return false;
  }
  return true;
}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,bool& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue = false;
    return false;
  }
  if(!Lib::smanip::tobool(s,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Passed value " << Lib::sout(s)
        << " for parameter " << Lib::sout(aWhat) 
        << " not a boolean."
        << Lib::endl;
    return false;
  }
  return true;
}

inline bool ui_parameterValue(Slash::UI::IUI& aUI,const std::string& aWhat,std::vector<double>& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    Lib::Out out(aUI.session().printer());
    out << "Paramater " << Lib::sout(aWhat) << " not found."
        << Lib::endl;
    aValue.clear();
    return false;
  }
  if(!Lib::smanip::todoubles(s,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Passed value " << Lib::sout(s)
        << " for parameter " << Lib::sout(aWhat) 
        << " not a double."
        << Lib::endl;
    return false;
  }
  return true;
}

inline void ui_add_prompt(Slash::UI::IUI& aUI,const std::string& aArg,bool aWarn = true) {
  // Add a prompt in the consolePrompt list.
  if(!aWarn) {
    if(!aUI.findWidget("consolePrompt")) return;
  }
  std::string value;
  if(!aUI.parameterValue("consolePrompt.items",value)) return;
  std::vector<std::string> items;
  Lib::smanip::lines(value,items);
  items.push_back(aArg);
  Lib::smanip::unique(items);
  aUI.setParameter("consolePrompt.items",Lib::smanip::tostring(items,"\n"));
}

inline void out_args(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  Lib::Out out(aUI.session().printer());
  out << "number of arguments : " << (int)aArgs.size() << Lib::endl;
  for(unsigned int index=0;index<aArgs.size();index++) {
    out << Lib::sout(aArgs[index]) << Lib::endl;
  }
}
inline bool ui_check_args(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs,unsigned int a_number){
  if(aArgs.size()==a_number) return true;
  Lib::Out out(aUI.session().printer());
  out << "bad argument number." 
      << " Given " << (unsigned int)aArgs.size() 
      << " whilst " << a_number << " expected."
      << Lib::endl;
  return false;
}
inline bool ui_check_min_args(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs,unsigned int a_number){
  if(aArgs.size()>=a_number) return true;
  Lib::Out out(aUI.session().printer());
  out << "bad argument number." 
      << " Given " << (unsigned int)aArgs.size() 
      << " whilst at least " << a_number << " expected."
      << Lib::endl;
  return false;
}
inline bool ui_check_min(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs,unsigned int a_number,std::string& a_last){
  if(aArgs.size()>=a_number) {
    if(a_number==0) {
      if(aArgs.empty()) {
        a_last = "";
      } else {
        a_last = aArgs[0];
        for(unsigned int index=1;index<aArgs.size();index++) {
          a_last += " " + aArgs[index];
        }
      }
    } else {
      a_last = aArgs[a_number-1];
      for(unsigned int index=a_number;index<aArgs.size();index++) {
        a_last += " " + aArgs[index];
      }
    }
    return true;
  }
  Lib::Out out(aUI.session().printer());
  out << "bad argument number." 
      << " Given " << (unsigned int)aArgs.size() 
      << " whilst at least " << a_number << " expected."
      << Lib::endl;
  return false;
}

inline bool ui_check_or_args(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs,unsigned int a_1,unsigned int a_2){
  if((aArgs.size()==a_1)||(aArgs.size()==a_2)) return true;
  Lib::Out out(aUI.session().printer());
  out << "bad argument number." 
      << " Given " << (unsigned int)aArgs.size() 
      << " whilst " << a_1 << " or " << a_2 << " expected."
      << Lib::endl;
  return false;
}
inline bool ui_check(Slash::UI::IUI& aUI,const std::string& aString,int& aValue){
  if(!Lib::smanip::toint(aString,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Argument " << Lib::sout(aString) << " not an int."
        << Lib::endl;
    return false;
  }
  return true;
}
inline bool ui_check(Slash::UI::IUI& aUI,const std::string& aString,bool& aValue){
  if(!Lib::smanip::tobool(aString,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Argument " << Lib::sout(aString) << " not a boolean."
        << Lib::endl;
    return false;
  }
  return true;
}
inline bool ui_check(Slash::UI::IUI& aUI,const std::string& aString,double& aValue){
  if(!Lib::smanip::todouble(aString,aValue)) {
    Lib::Out out(aUI.session().printer());
    out << "Argument " << Lib::sout(aString) << " not a double."
        << Lib::endl;
    return false;
  }
  return true;
}
inline bool ui_check_rgb(Slash::UI::IUI& aUI,const std::string& aString,double& a_r,double& a_g,double& a_b){
  if(!Lib::smanip::torgb(aString,a_r,a_g,a_b)) {
    Lib::Out out(aUI.session().printer());
    out << "Argument " << Lib::sout(aString) << " not a color."
        << Lib::endl;
    return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
/// data /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void data_collect(Slash::Core::ISession& aSession,const std::string& aWhat,const std::string& aCuts = "") {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return;
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  ss.push_back(aCuts);
  accessorManager->execute("collect",ss);
}

inline void data_visualize(Slash::Core::ISession& aSession,const std::string& aWhere) {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return;
  std::vector<std::string> ss;
  ss.push_back("where="+aWhere);
  accessorManager->execute("visualize",ss);
}

inline int data_number(Slash::Core::ISession& aSession) {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return 0;
  std::vector<std::string> ss;
  std::string s = accessorManager->execute("number",ss);
  int number;
  if(!Lib::smanip::toint(s,number)) return 0;
  return number;  
}

inline std::string data_values(Slash::Core::ISession& aSession,const std::string& aWhat) {
  // returned string is a \n separated list.
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return std::string();
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  return accessorManager->execute("values",ss);
}

inline std::vector<std::string> data_values_vector(Slash::Core::ISession& aSession,const std::string& aWhat) {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return std::vector<std::string>();
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  return Lib::smanip::words(accessorManager->execute("values",ss),"\n");
}

inline void data_dump(Slash::Core::ISession& aSession,const std::string& aFormat = "table") {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return;
  std::vector<std::string> ss;
  ss.push_back(aFormat);
  accessorManager->execute("dump",ss);
}

inline void data_filter(Slash::Core::ISession& aSession,const std::string& aWhat,const std::string& aFilter = "") {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return;
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  ss.push_back(aFilter);
  accessorManager->execute("filter",ss);
}

inline void data_set(Slash::Core::ISession& aSession,const std::string& aWhat,const std::string& aValue) {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return;
  std::vector<std::string> ss;
  ss.push_back(aWhat);
  ss.push_back(aValue);
  accessorManager->execute("set",ss);
}

inline void data_destroy(Slash::Core::ISession& aSession) {
  Slash::Data::IProcessor* accessorManager = Slash::accessorManager(aSession);
  if(!accessorManager) return;
  std::vector<std::string> ss;
  accessorManager->execute("destroy",ss);
}

//////////////////////////////////////////////////////////////////////////////
/// tree /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void tree_set_with_strings(Slash::UI::IUI& aUI,const std::string& aParameter,const std::vector<std::string>& aList) {
  std::string sout = "<tree>";
  for(unsigned int index=0;index<aList.size();index++) {
    sout += "<treeItem><label>"+aList[index]+"</label></treeItem>";
  }
  sout += "</tree>";
  aUI.setParameter(aParameter,sout);
}

inline void tree_set(Slash::UI::IUI& aUI,const std::string& aParameter,const std::string& aString) {
  // Example : tree_set(aUI,"mainTree.items","aa\nbb")
  std::vector<std::string> items;
  Lib::smanip::lines(aString,items);
  tree_set_with_strings(aUI,aParameter,items);
}

inline void tree_set_with_file(Slash::UI::IUI& aUI,const std::string& aParameter,const std::string& aFile) {
  std::vector<std::string> lines;
  if(!Lib::fmanip::read(aFile,lines)) {
    aUI.echo("Can't open "+Lib::smanip::quote(aFile)+" file.");
  } else {
    aUI.setParameter(aParameter,Lib::smanip::tostring(lines,""));
  }
}

//////////////////////////////////////////////////////////////////////////////
/// widget ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

inline void set_current_widget(Slash::UI::IUI& aUI,const std::string& aString){
  Slash::UI::IWidget* widget = aUI.findWidget(aString);
  if(!widget) {
    Lib::Out out(aUI.session().printer());
    out << "set_current_widget :" 
        << "Can't find widget " << Lib::sout(aString)
        << Lib::endl;
    return;
  }
  aUI.setCurrentWidget(widget);
}

inline Slash::UI::IUI* find_UI(Slash::Core::ISession& aSession,const std::string& aName = "default") {
  Slash::UI::IUI_Manager* mgr = Slash::uiManager(aSession);
  if(!mgr) return 0;
  return mgr->find(aName);
}

inline Slash::UI::IWidget* find_widget(Slash::UI::IUI& aUI,const std::string& aString) {
  Slash::UI::IWidget* widget = 0;
  if(aString=="@current@") {
    widget = aUI.currentWidget();
    if(!widget) {
      Lib::Out out(aUI.session().printer());
      out << "find_widget : "
          << " no current widget."
          << Lib::endl;
      return 0;
    }
  } else {
    widget = aUI.findWidget(aString);
    if(!widget) {
      Lib::Out out(aUI.session().printer());
      out << "find_widget : "
          << Lib::sout(aString) << " not found."
          << Lib::endl;
      return 0;
    }
  }
  return widget;
}

//////////////////////////////////////////////////////////////////////////////
/// ISoViewer ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <Slash/UI/ISoViewer.h>

//inline Slash::UI::ISoViewer* cast_SoViewer(Slash::UI::IWidget& aWidget){
//  return Lib_Cast(aWidget,Slash::UI::ISoViewer);
//}
inline Slash::UI::ISoViewer* find_SoViewer(Slash::UI::IUI& aUI,const std::string& aString) {
  Slash::UI::IWidget* widget = ::find_widget(aUI,aString);
  if(!widget) return 0;
  Slash::UI::ISoViewer* soViewer = Lib_Cast(*widget,Slash::UI::ISoViewer);
  if(!soViewer) {
    Lib::Out out(aUI.session().printer());
    out << "find_SoViewer : "
        << Lib::sout(aString) << " not a Slash::UI::ISoViewer."
        << Lib::endl;
    return 0;
  }
  return soViewer;
}

#endif
