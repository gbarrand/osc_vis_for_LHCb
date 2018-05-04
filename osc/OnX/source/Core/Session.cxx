// this :
#include <OnX/Core/Session.h>

#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/IWriter.h>
#include <Slash/Core/ILibrary.h>

#include <Slash/UI/IUI.h>
#include <Slash/UI/ICallbackData.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/RelationManager.h>
#include <Slash/Tools/LibraryManager.h>
#include <Slash/Tools/StyleManager.h>
#include <Slash/Tools/ScriptManager.h>

#include <inlib/sys/file>
#include <inlib/args>
#include <inlib/system>
#include <inlib/file>
#include <inlib/cast>
#include <exlib/xml/parser>

#include <Lib/AccessorManager.h>

#include <OnX/Core/UI_Manager.h>
#include <OnX/Version.h>

#include <cstdio>

// Embedded code :
#include <OnX/Core/Session_onx.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static bool system_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);
static bool sh_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);
static bool DOS_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);
static bool session_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);
static bool dld_Function(const std::string&,const Slash::UI::IInterpreter::Options&,const Slash::UI::IInterpreter::Aliases&,void*);

OnX::Session::Session(bool aBalance,bool aDebugCheckClass)
: OnX::Printer() //WARNING : must be done before the below line.
,Lib::Session(*((Slash::Core::IWriter*)this),aBalance,aDebugCheckClass)
,fOutOld(0)
,fErrOld(0)
,fOutFile(0)
,fErrFile(0)
{
  OnX::Printer::setSession(this);
}

OnX::Session::~Session(){
  stdoutToDefault();
  OnX::Printer::disable();
  coutToDefault();
}

void* OnX::Session::cast(const std::string& a_class) const {
  // inheritance :
  INLIB_IF_CAST(OnX::Printer)
  else if(void* p = Lib::Session::cast(a_class)) return p;
  // this :
  else INLIB_IF_CAST(OnX::Session)
  else return 0;
}

//private:
OnX::Session::Session(const Session& aFrom)
: Lib::Session(aFrom)
,fOutOld(0)
,fErrOld(0)
,fOutFile(0)
,fErrFile(0)
{}
//private:
OnX::Session& OnX::Session::operator=(const Session&){return *this;}

bool OnX::Session::setDefaults(const std::string& aFile){
  setParameter("session.output","UI");
  setParameter("AccessorManager.dumpMode","table");
  setParameter("session.helpOnDialog","yes");
  // Used in the OnX/NextDialog dialog :
  //setParameter("ui.exitSecondaryLoop","no");

  // Session XML file :
  inlib::xml::tree* top = 0;
  inlib::xml::default_factory factory;
  exlib::xml::parser sessionML(factory,cout());
  std::vector<std::string> tags;
  tags.push_back("session");
  tags.push_back("manager");
  tags.push_back("interpreter");
  sessionML.set_tags(tags);
  if(aFile.size()) {
    sessionML.load_file(aFile,false);
    top = sessionML.top_item();
  } else {
    std::string s = inlib::tos(Session_onx_linen,Session_onx_lines);
    inlib::replace(s,"@@double_quote@@","\"");
    inlib::replace(s,"@@back_slash@@","\\");
    sessionML.load_string(s);
    top = sessionML.top_item();
  }
  std::string svalue;
  int ivalue;
  if(top) {

   {inlib::xml::looper _for(*top);
    while(inlib::xml::element* _elem = _for.next_element()) {
      std::string name = _elem->name();
      std::string value = _elem->value();
      //printf("debug : \"%s\" \"%s\"\n",name.c_str(),value.c_str());
      if(name=="verbose") {
        if(inlib::to<int>(value,ivalue)) fVerboseLevel = ivalue;
      } else if(name=="output") {
        setParameter("session.output",value);
      } else {
        setParameter(name,value);
      }
    }}

    // Set some manager attribute (verbose) in the session parameters :
    // <manager>.<attribute>:<value>
   {inlib::xml::looper _for(*top);
    while(inlib::xml::tree* _tree = _for.next_tree()) {
      if(_tree->tag_name()=="manager") {
        std::string sname;
        if(_tree->attribute_value("name",sname)) {
          std::string param = sname + ".verbose";
          if(_tree->element_value("verbose",svalue) && 
             inlib::to<int>(svalue,ivalue) ) {
            setParameter(param,inlib::to<int>(ivalue));
          } else {
            setParameter(param,inlib::to<int>(fVerboseLevel));
          }
        }
      }
    }}

  }

  // Should be the first.
  if(!findManager(Slash_LibraryManager)) {
    addManager(new Slash::LibraryManager(*this,Slash_LibraryManager));
  } else {
    //already inited.
  }

  if(!findManager(Slash_AccessorManager)) {
    Lib::AccessorManager* accessorManager = 
      new Lib::AccessorManager(cout());
    addManager(accessorManager);

    inlib::xml::tree* item = 0;
    if(top && (item = top->find_item("AccessorManager")) ) {
      if(item->element_value("verbose",svalue)) {
        if(inlib::to<int>(svalue,ivalue))
          accessorManager->setVerboseLevel(ivalue);
      }
    } else {
      accessorManager->setVerboseLevel(fVerboseLevel);
    }

    accessorManager->addAction(new Lib::CollectAction(*accessorManager));
    accessorManager->addAction(new Lib::FilterAction(*accessorManager));
    accessorManager->addAction(new Lib::DumpAction(*accessorManager));
    accessorManager->addAction(new Lib::NumberAction(*accessorManager));
    accessorManager->addAction(new Lib::GetValuesAction(*accessorManager));
    accessorManager->addAction(new Lib::DestroyAction(*accessorManager));
    accessorManager->addAction(new Lib::SetAction(*accessorManager));
    accessorManager->addAction(new Lib::CalculateAction(*accessorManager));
    accessorManager->addAction(new Lib::InitAction(*accessorManager));
    accessorManager->addAction(new Lib::VisualizeAction(*accessorManager));
  }

  if(!findManager(Slash_RelationManager)) {
    addManager(new Slash::RelationManager(Slash_RelationManager,*this));
  }

  if(!findManager(Slash_ScriptManager)) {
    Slash::ScriptManager* scriptManager = 
      new Slash::ScriptManager(*this,Slash_ScriptManager);
    addManager(scriptManager);

    // Default interpreters :
#ifdef WIN32
    scriptManager->addInterpreter("system","","","bat","call @onx_file@","",
                                  system_Function,0);
#else
    scriptManager->addInterpreter("system","","","","chmod a+x @onx_file@;./@onx_file@","",
                                  system_Function,0);
#endif
    scriptManager->addInterpreter("sh","","","sh","chmod a+x @onx_file@;./@onx_file@","",
                                  sh_Function,0);
    scriptManager->addInterpreter("DOS","","","bat","call @onx_file@","",
                                  DOS_Function,0);

    scriptManager->addInterpreter("Session","","","session","OnX session_exec_file","",session_Function,(Slash::Core::ISession*)this);

    scriptManager->addInterpreter("DLD","","","dld","OnX dld_exec_file @onx_file@","",dld_Function,(Slash::Core::ISession*)this);

    inlib::xml::tree* item = 0;
    if(top && (item = top->find_item("ScriptManager")) ) {
      if(item->element_value("recordExecutedScripts",svalue)) {
        bool bvalue;
        if(inlib::to(svalue,bvalue))
          scriptManager->recordExecutedScripts(bvalue);
      }
      inlib::xml::looper _for(*item);
      while(inlib::xml::tree* _tree = _for.next_tree()) {
        if(_tree->tag_name()!="interpreter") continue;

        std::string sname;
        if(_tree->attribute_value("name",sname)) {
          std::string slibrary;
          if(_tree->element_value("library",slibrary)) {
            std::string spackage;
            if(_tree->element_value("package",spackage)) {
              std::string ssuffix;
              if(_tree->element_value("suffix",ssuffix)) {
                std::string scmd;
                if(_tree->element_value("execFileCommand",scmd)) {
                  std::string senv;
                  _tree->element_value("mendatoryEnvironmentVariable",senv);

                  if(fVerboseLevel) {
                    std::ostream& out = cout();
                    out << "OnX::Session::Session :"
                        << " from file " << inlib::sout(aFile)
                        << " addInterpreter :" 
                        << " name " << inlib::sout(sname)
                        << " library " << inlib::sout(slibrary)
                        << " package " << inlib::sout(spackage)
                        << " suffix " << inlib::sout(ssuffix)
                        << " execFileCommand " << inlib::sout(scmd)
                        << " mendatoryEnvironmentVariable " << inlib::sout(senv)
                        << std::endl;
                  }
                  scriptManager->addInterpreter(sname,slibrary,spackage,
                                                ssuffix,scmd,senv,0,0);
                }
              }
            }
          }
        }

      }
    }
  }

  if(!findManager(Slash_StyleManager)) {
    addManager(new Slash::StyleManager(*this,Slash_StyleManager));
  }

  
  if(!findManager(Slash_UI_Manager)) {
    OnX::UI_Manager* uiManager = new OnX::UI_Manager(*this);
    addManager(uiManager);
    //FIXME : put the below params on the UI_Manager.
    inlib::xml::tree* item = 0;
    if(top && (item = top->find_item("UI_Manager")) ) {
      if(item->element_value("toolkit",svalue) && (svalue.size())) {
        setParameter("UI_Manager.toolkit",svalue);
      }
      if(item->element_value("threaded",svalue) && (svalue.size())) {
        setParameter("UI_Manager.thread",svalue);
      }
      if(item->element_value("style",svalue) && (svalue.size())) {
        setParameter("UI_Manager.style",svalue);
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// overloaded :

bool OnX::Session::load(const std::string& aFile,const std::string& aPackage){
  Slash::Core::ILibraryManager* libraryManager = Slash::libraryManager(*this);
  if(!libraryManager) return false;
  return libraryManager->addLibrary(aPackage,aFile,aPackage) ? true : false;
}

#include <stdio.h>
void OnX::Session::redirectOutput(bool aYesNo){
  if(aYesNo) {
    coutToOnX();    // cout to fPrinter.
    stdoutToOnX();  // stdout to fPrinter.
    OnX::Printer::enable(); // Slash::Core::IWriter to Slash::UI::IUI.
  } else {
    stdoutToDefault(); // stdout to terminal.
    coutToDefault();   // cout to terminal.
    OnX::Printer::disable();   // Slash::Core::IWriter to stdout.
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Should be after any inclusion of string :
#include <iostream>

// To redirect stdout :
#include <stdio.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

void OnX::Session::coutToOnX(){
//#define NO_COUT
#if !defined(NO_COUT) && !defined(__hpux)
  if(!fOutOld) {
    fOutOld = std::cout.rdbuf(&(streamBuffer()));
  }
  if(!fErrOld) {
    fErrOld = std::cerr.rdbuf(&(streamBuffer()));
  }
#endif
}

void OnX::Session::coutToDefault(){
#if !defined(NO_COUT) && !defined(__hpux)
  if(fOutOld) {
    std::cout.rdbuf(fOutOld);
    fOutOld = 0;
  }
  if(fErrOld) {
    std::cerr.rdbuf(fErrOld);
    fErrOld = 0;
  }
#endif
}

void OnX::Session::stdoutToOnX(){
  // Avoid to redirect stdout, stderr.
  // On Darwin and Windows we loose the Python output !
  //return;

  //if ((::dup2(::fileno(stdout), ::fileno(stderr))) >= 0) {
  //} 
  /*FIXME :
    Linux : if starting with the mouse, we loose output 
     (redirected to /dev/null ???)
  */

  if(!fOutFile) {
    if(inlib::tmpname(".","OnX_",".out",fOutName)) {
#ifdef WIN32
      fOutDefault = "CONOUT$";
#else
      char* stdoutName = ttyname(STDOUT_FILENO);
      // Could be NULL if stdout had been redirected on the command line.
      fOutDefault = stdoutName ? std::string(stdoutName) : std::string("");
#endif
      if(fOutDefault.size()) {
        if (::freopen(fOutName.c_str(), "w", stdout)) {
          fOutFile = ::fopen(fOutName.c_str(), "r");
        } else {
          ::remove(fOutName.c_str());
        }
      }
    }
  }
  if(!fErrFile) {
    if(inlib::tmpname(".","OnX_",".err",fErrName)) {
#ifdef WIN32
      fErrDefault = "CONOUT$";
#else
      char* stderrName = ttyname(STDERR_FILENO);
      // Could be NULL if stderr had been redirected on the command line.
      fErrDefault = stderrName ? std::string(stderrName) : std::string("");
#endif
      if(fErrDefault.size()) {
        if (::freopen(fErrName.c_str(), "w", stderr)) {
          fErrFile = ::fopen(fErrName.c_str(), "r");
        } else {
          ::remove(fErrName.c_str());
        }
      }
    }
  }
}

void OnX::Session::stdoutToDefault(){
  if(fOutFile) {
    if (::freopen(fOutDefault.c_str(), "w", stdout)) {}
    ::fclose((FILE*)fOutFile);
    fOutFile = 0;
    ::remove(fOutName.c_str());
  }
  if(fErrFile) {
    if (::freopen(fErrDefault.c_str(), "w", stderr)) {}
    ::fclose((FILE*)fErrFile);
    fErrFile = 0;
    ::remove(fErrName.c_str());
  }
}
#define READBUFSIZE 32768  //16384  //65536;
//#define READBUFSIZE 16

bool OnX::Session::flush(){
  char buf[READBUFSIZE];
  int len;
  if(fOutFile) {
    ::fflush(stdout);
    do {
      len = ::fread(buf,1,READBUFSIZE-1,(FILE*)fOutFile);
      if(len<=0) break;
      buf[len] = '\0';
      OnX::Printer::write(buf);
    } while (len > 0);
  }
  if(fErrFile) {
    ::fflush(stderr);
    do {
      len = ::fread(buf,1,READBUFSIZE-1,(FILE*)fErrFile);
      if(len<=0) break;
      buf[len] = '\0';
      OnX::Printer::write(buf);
    } while (len > 0);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool system_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options&
,const Slash::UI::IInterpreter::Aliases&
,void*
){
  inlib::execute(aString);
  return true;  
}

bool sh_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options&
,const Slash::UI::IInterpreter::Aliases&
,void*
){
#ifdef WIN32
  return false;
#else
  inlib::execute(aString);
  return true;  
#endif
}

bool DOS_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options&
,const Slash::UI::IInterpreter::Aliases&
,void*
){
#ifdef WIN32
  inlib::execute(aString);
  return true;  
#else
  return false;
#endif
}

bool dld_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options& aOptions
,const Slash::UI::IInterpreter::Aliases& aAliases
,void* aTag
){
  Slash::Core::ISession* session = (Slash::Core::ISession*)aTag;
  std::ostream& out = session->cout();

  if(session->verboseLevel()) {
    out << "OnX::Session::dld_Function :"
        << " execute " << inlib::sout(aString) << "..."
        << std::endl;
  }

  Slash::UI::IUI* ui = Slash::find_UI(*session);
  if(!ui) {
    //if(session->verboseLevel()) {
      out << "OnX::Session::dld_Function :"
          << " Slash::UI::IUI not found." 
          << " Can't execute script " << inlib::sout(aString) << "."
          << std::endl;
    //}
    return false;
  }

  std::vector<std::string> words;
  inlib::words(aString," ",false,words);
  if(words.size()<2) {
    out << "OnX::Session::dld_Function :"
        << " bad syntax in " << inlib::sout(aString) 
        << std::endl;
    return false;
  }

  std::string sdll;
  std::string scbk;
  sdll = words[0];
  scbk = words[1];
  //  if(!treat_atat(*ui,words[0],sdll)) return false;
  //  if(!treat_atat(*ui,words[1],scbk)) return false;
  //  inlib::strip(sdll);
  //  inlib::strip(scbk);

  std::string at("yes"); //default
  std::string alias("yes"); //default
  if(aOptions.size()) {
    inlib::args opts(aOptions);
    if(!opts.find("at",at)) at = "yes";
    if(!opts.find("alias",alias)) alias = "yes";
  }

  std::vector<std::string> args;
  if(at=="no") {
    for(unsigned int index=2;index<words.size();index++) {
      args.push_back(words[index]);
    }
  } else if(at=="yes") {
    for(unsigned int index=2;index<words.size();index++) {
      bool b_cat_empty = (words[index]=="@cat@");
      bool b_cat_space = (words[index]=="@cat_space@");
      if(b_cat_empty||b_cat_space) {
        if(args.empty()) {
          if(index==(words.size()-1)) {
          } else {
            std::string s;
            if(!ui->atat(words[index+1],s)) {
              out << "OnX::Session::dld_Function :"
                  << " atat(1) failure for " 
                  << inlib::sout(words[index+1]) << "." 
                  << std::endl;
              return false;
            }
            args.push_back(s);
            index++;
          }     
        } else {
          if(index==(words.size()-1)) {
          } else {
            std::string prev = args.back();
            args.pop_back();
            std::string s;
            if(!ui->atat(words[index+1],s)) {
              out << "OnX::Session::dld_Function :"
                  << " atat(2) failure for " 
                  << inlib::sout(words[index+1]) << "." 
                  << std::endl;
              return false;
            }
            if(b_cat_empty) {
              args.push_back(prev+s);
            } else if(b_cat_space) {
              args.push_back(prev+" "+s);
            }
            index++;
          }     
        }
      } else {
        std::string s;
        if(!ui->atat(words[index],s)) {
          out << "OnX::Session::dld_Function :"
              << " atat(3) failure for " << inlib::sout(words[index]) << "." 
              << std::endl;
          return false;
        }
        args.push_back(s);
      }
    }
  } else {
    out << "OnX::Session::dld_Function :"
        << " at option " << inlib::sout(at) << " unknown." 
        << std::endl;
    return false;
  }

  if(sdll.empty()) {
    out << "OnX::Session::dld_Function :"
        << " dll not specified in " << inlib::sout(aString) << "." 
        << std::endl;
    return false;
  }

  // treat aliases :
  if(alias=="yes") {
    for(Slash::UI::IInterpreter::Aliases::const_iterator it=aAliases.begin();
      it!=aAliases.end();++it) {
    if(scbk==(*it).first) {
      scbk = (*it).second;
      break;
    }
  }}

  if(scbk.empty()) {
    out << "OnX::Session::dld_Function :"
        << " cbk not specified in " << inlib::sout(aString) << "." 
        << std::endl;
    return false;
  }

 {Slash::Core::ILibraryManager* lm = Slash::libraryManager(*session);
  if(lm) {
    Slash::Core::ILibrary* library = lm->addLibrary(sdll,sdll,"");
    if(library) {
      typedef void(*Callback)(Slash::UI::IUI&,const std::vector<std::string>&);
      Callback cbk = (Callback)library->findProcedure(scbk,true);
      if(cbk) {  
        cbk(*ui,args);
      } else {
        // try with a sdll namespace : sdll_scbk.
        Callback cbk = (Callback)library->findProcedure(sdll+"_"+scbk,true);
        if(cbk) {  
          cbk(*ui,args);
        } else {
          out << "OnX::Session::dld_Function :"
              << " can't find callback " << inlib::sout(scbk)
              << " or " << inlib::sout(sdll+"_"+scbk) 
              << " in " << inlib::sout(sdll) << "."
              << " Command was " << inlib::sout(aString) << "." 
              << std::endl;
          return false;
        }
      }
    }
  }}

  return true;  
}

bool session_Function(
 const std::string& aString
,const Slash::UI::IInterpreter::Options&
,const Slash::UI::IInterpreter::Aliases&
,void* aTag
){
  Slash::Core::ISession* session = (Slash::Core::ISession*)aTag;

  std::ostream& out = session->cout();

  if(session->verboseLevel()) {
    out << "OnX::Session::session_Function :"
        << " execute " << inlib::sout(aString) << "..."
        << std::endl;
  }

  std::string sdll;
  std::string scbk;

  std::vector<std::string> words;
  inlib::words(aString," ",false,words);
  if(words.size()<2) {
    out << "OnX::Session::session_Function :"
        << " bad syntax in " << inlib::sout(aString) << "." 
        << std::endl;
    return false;
  }
  sdll = words[0];
  scbk = words[1];
  inlib::strip(sdll);
  inlib::strip(scbk);

  std::vector<std::string> args;
  for(unsigned int index=2;index<words.size();index++) 
    args.push_back(words[index]);

  if(sdll.empty()) {
    out << "OnX::Session::session_Function :"
        << " dll not specified in " << inlib::sout(aString) << "." 
        << std::endl;
    return false;
  }
  if(scbk.empty()) {
    out << "OnX::Session::session_Function :"
        << " cbk not specified in " << inlib::sout(aString) << "." 
        << std::endl;
    return false;
  }

 {Slash::Core::ILibraryManager* lm = Slash::libraryManager(*session);
  if(lm) {
    Slash::Core::ILibrary* library = lm->addLibrary(sdll,sdll,"");
    if(library) {
      typedef void(*Callback)(Slash::Core::ISession&,const std::vector<std::string>&);
      Callback cbk = (Callback)library->findProcedure(scbk,true);
      if(cbk) {
        cbk(*session,args);
      } else {
        // try with a sdll namespace : sdll_scbk.
        Callback cbk = (Callback)library->findProcedure(sdll+"_"+scbk,true);
        if(cbk) {  
          cbk(*session,args);
        } else {
          out << "OnX::Session::session_Function :"
              << " can't find callback " << inlib::sout(scbk)
              << " or " << inlib::sout(sdll+"_"+scbk)
              << " in " << inlib::sout(sdll) << "."
              << " Command was " << inlib::sout(aString) << "." 
              << std::endl;
          return false;
        }
      }
    }
  }}

  return true;
}
