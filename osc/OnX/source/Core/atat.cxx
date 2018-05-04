//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//@@ syntax :
// keyword without arguments :
//  @this@
//  @this.value@
//  @current@
//  @current_name@
//  @cat@
//  @cat_space@
//  @empty@
//  @about@
//  @os@
//  @compiler@
//  @processor@
//  @sep@
//  @sep@file@
//  @sep@path@
//  @sep@cmd@
// keyword with arguments :
//  @session@<parameter>@
//    @session@manager_names@
//    @session@managerNames@
//    @session@arguments@
//    @session@command_files@
//  @directory@current@
//  @directory@<directory>@<mask>@
//  @files@<directory>@<mask>@
//  @file@<file>@
//  @tmpname@
//  @path@<string>@
//  @children@<widget>@<what>@<parameter>@
//  @child@<widget>@<what>@<parameter>@<index>@
//  @name@<top>@<prefix>@
//  @name_from_current@<top>@
//  @shell@<widget>@
//  @parent@<widget>@
//  @interpreter@<interpreter>@<what>@
//  @complete@<interpreter>@<string>@
//  @manager@<manager>@<what>@
//  @da@<action>@<param>@
//  @nth@<number>@<list>@ <list> \n separated items.
//  @next@<item>@<list>@<option>@ <list> \n separated items. 
//    <option> being [strip_key,strip_value,none]
//  @filter@<mask>@<list>@ <list> \n separated items.
//  @cvt@<format>@<string>@
//  @suffix@<string.suffix>@
//  @char@<decimal code>@
//  @cyclic@<cyclic>@<what>@
//  @calc@<formula>@<list>@ <list> \n separated values of variables.
//  @replace@<string>@<old>@<new>@
//  @ask@<prompt>@
//  @ui_is_a@<what>@
//  @env@<environment variable>@
//  @net@<file>@
// else :
//  [<widget parameter>]
//  {<session parameter>}

#include <inlib/os> //first include
#include <inlib/smatch>
#include <inlib/srep>
#include <inlib/sep>
#include <inlib/scolor>

namespace inlib {
inline const std::string& cmd_sep() {
#ifdef WIN32
  static const std::string s_v("&&");
#else
  static const std::string s_v(";");
#endif
  return s_v;
}
}

static bool is_wp(const std::string& a_s,char a_l,char a_r,std::string& a_w,std::string& a_p,bool& a_y) {
  if(!a_s.size()) {
    a_y = false;
    return true;
  }
  if(a_s[0]!=a_l) {
    a_y = false;
    return true;
  }
  //{abcdfgh}.ijk
  //01234567890123
  std::string r("x.");
  r[0] = a_r;
  std::string::size_type pos = a_s.rfind(r);
  if(pos==std::string::npos) {
    a_y = false;
    return false;
  }
  a_w = a_s.substr(1,pos-1);
  a_p = a_s.substr(pos+2,a_s.size()-(pos+2));
  if(!a_w.size()||!a_p.size()) {
    a_y = false;
    return false;
  }
  a_y = true;
  return true;
}

inline bool smanip_bracketed(const std::string& aString,char a_l,char a_r,std::string& aValue){
  if(aString.size()<2) return false;
  if(aString[0]!=a_l) return false;
  if(aString[aString.size()-1]!=a_r) return false;
  aValue = aString.substr(1,aString.size()-2); 
  return true;
}

inline std::vector<std::string> smanip_text(const std::string& aString){
  return inlib::words(aString,"\n",true);
}
inline std::vector<std::string> smanip_match(const std::vector<std::string>& aText,const std::string& aPattern 
){
  std::vector<std::string> text;
  unsigned int number = aText.size();
  for(unsigned int index=0;index<number;index++) {
    if(inlib::match(aText[index],aPattern)) text.push_back(aText[index]);
  }
  return text;
}
inline std::vector<std::string> smanip_prepend(const std::string& aWhat,const std::vector<std::string>& aText){
  std::vector<std::string> text;
  unsigned int number = aText.size();
  for(unsigned int index=0;index<number;index++) {
    text.push_back(aWhat+aText[index]);
  }
  return text;
}

#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/UI/ICallbackData.h>

static bool treat_bracketed(Slash::UI::IUI& aUI,const std::string& aString,std::string& aValue) {
  std::string s;
  bool br_1 = smanip_bracketed(aString,'{','}',s);
  bool br_2 = br_1 ? false : smanip_bracketed(aString,'[',']',s);
  if(br_1 || br_2) {
    std::string v;
    if(!treat_bracketed(aUI,s,v)) return false;
    if(br_1) { //look for a session parameter.
      if(!aUI.session().parameterValue(v,aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@@ : session parameter " 
            << inlib::sout(v) << " not found."
            << std::endl;
        return false;
      }
      return true;
    } else { //br_2 :look for a widget parameter.

      if((v.size()>=5)&& (v.substr(0,5)=="this.")) {
        //this.label
        //0123456789
        v = aUI.callbackData().component()+v.substr(4,v.size()-4);
      }

      if(!aUI.parameterValue(v,aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@@ : widget value " 
            << inlib::sout(v) << " not found."
            << std::endl;
        return false;
      }
      return true;
    }
  } else {
    std::string w;
    std::string r;
    bool wr_1;
    if(!is_wp(aString,'{','}',w,r,wr_1)) return false;
    bool wr_2;
    if(!wr_1) {
      if(!is_wp(aString,'[',']',w,r,wr_2)) return false;
    }
    if(wr_1 || wr_2) {
      std::string v;
      if(!treat_bracketed(aUI,w,v)) return false;
      if(wr_1) { //look for a session parameter.
        std::string p;
        if(!aUI.session().parameterValue(v,p)) {
          std::ostream& out = aUI.session().cout();
          out << "@@ : session parameter " 
              << inlib::sout(v) << " not found."
              << std::endl;
          return false;
        }
        aValue = p+"."+r;
        return true;
      } else { //wr_2
        if((v.size()>=5)&& (v.substr(0,5)=="this.")) {
          //this.label
          //0123456789
          v = aUI.callbackData().component()+v.substr(4,v.size()-4);
        }
        std::string vv;
        if(!aUI.parameterValue(v,vv)) {
          std::ostream& out = aUI.session().cout();
          out << "@@ : widget value " 
              << inlib::sout(v) << " not found."
              << std::endl;
          return false;
        }
        aValue = vv+"."+r;
        return true;
      }
    } else { //not bracketed and not wr_ed : simple string.
      if(aString=="this") {
        aValue = aUI.callbackData().component();
      } else if(aString=="this.value") {
        aValue = aUI.callbackValue();
      } else {
        aValue = aString;   //Can contain dots.
      }
      return true;
    }
  }
}

static bool treat_arg(Slash::UI::IUI& aUI,const std::string& a_s,std::string& a_v) {
  if(!treat_bracketed(aUI,a_s,a_v)) {
    std::ostream& out = aUI.session().cout();
    out << "@@ : syntax error in " 
        << inlib::sout(a_s) << "."
        << std::endl;
    return false;
  }
  //::printf("debug : treat_arg : \"%s\" \"%s\"\n",a_s.c_str(),a_v.c_str());
  return true;
}

#include <Slash/UI/IWidget.h>
#include <Slash/UI/ICyclic.h>
#include <Slash/Core/IParameterManager.h>
#include <Slash/Core/IHierarchy.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>

#include <inlib/sys/file>
#include <inlib/sys/dir>
#include <inlib/sprintf>
#include <inlib/args>
#include <inlib/file>
#include <inlib/system>
#include <inlib/vmanip>

#include <inlib/cast>

#include <Lib/calc.h> //FIXME

#include <OnX/Version.h>

bool treat_atat(Slash::UI::IUI& aUI,const std::string& aArg,std::string& aValue) {
  if(aArg=="ui_callbackValue") {
    std::ostream& out = aUI.session().cout();
    out << "ui_callbackValue keyword deprecated. Use @this.value@ instead."
        << std::endl;
    aValue = aUI.callbackValue();
    return true;
  }
  std::string s;
  if(!smanip_bracketed(aArg,'@','@',s)){
    aValue = aArg;
    return true;
  }

  //  @this@
  if(s=="this") {
    aValue = aUI.callbackData().component();

  //  @this.value@
  } else if(s=="this.value") {
    aValue = aUI.callbackValue();

  //  @empty@
  } else if(s=="empty") {
    aValue = "";

  //  @current@
  } else if(s=="current") {
    //OPTIMIZATION : returned "as it". 
    //  It must be treated in the callback itself.
    aValue = aArg;

  //  @current_name@
  } else if(s=="current_name") {
    Slash::UI::IWidget* widget = aUI.currentWidget();
    if(!widget) {
      std::ostream& out = aUI.session().cout();
      out << "@current_name@ : "
          << " no current widget."
          << std::endl;
      return false;
    }
    aValue = widget->name();

  //  @about@
  } else if(s=="about") {
    aValue = std::string("OnX version ")+ONX_VERSION;
    aValue += "\n\n";
    aValue += "Author : Guy Barrand";
    aValue += "\n\n";
    aValue += "URL : http://OpenScientist.lal.in2p3.fr";

  //  @os@
  } else if(s=="os") {
    aValue = inlib::os();

  //  @processor@
  } else if(s=="processor") {
    aValue = inlib::processor();

  //  @compiler@
  } else if(s=="compiler") {
    aValue = inlib::compiler();

  //  @sep@
  } else if(s=="sep") {
    aValue = inlib::sep();

  } else {
    std::vector<std::string> words;
    inlib::words(s,"@",true,words);
    if(words.size()==1) {

      std::string arg;
      if(!treat_arg(aUI,words[0],arg)) return false;
      aValue = arg;

    //  @sep@file@
    } else if((words.size()==2)&&(words[0]=="sep")&&(words[1]=="file")) {
      aValue = inlib::sep();
    //  @sep@cmd@
    } else if((words.size()==2)&&(words[0]=="sep")&&(words[1]=="cmd")) {
      aValue = inlib::cmd_sep();
    //  @sep@path@
    } else if((words.size()==2)&&(words[0]=="sep")&&(words[1]=="path")) {
      aValue = inlib::psep();

    //  @session@<parameter>@
    //  @session@<parameter>@<option>@
    } else if(((words.size()==2)||(words.size()==3))&&(words[0]=="session")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(words.size()==3) {
        if(!treat_arg(aUI,words[2],arg2)) return false;
      }

      if((arg1=="manager_names")||(arg1=="managerNames")) {
        aValue = inlib::tos<std::string>(aUI.session().managerNames(),"\n");
      } else if(arg1=="arguments") {
	inlib::args args(aUI.session().arguments());
        aValue = inlib::tos<std::string>(args.tovector(),"\n");
      } else if(arg1=="command_files") {
	inlib::args args(aUI.session().arguments());
        aValue = inlib::tos<std::string>(args.files(),"\n");
      } else if(arg1=="hierarchy") {
        // Build the XML script to be put on the tree widget :
        std::string sout = "<tree>";
       {unsigned int number = aUI.session().numberOfManagers();
        for(unsigned int index=0;index<number;index++) {
          const Slash::Core::IManager* mgr = aUI.session().manager(index);
          const Slash::Core::IHierarchy* h = 
            INLIB_CONST_CAST(*mgr,Slash::Core::IHierarchy);
          if(h) {
            sout += 
              "<treeItem><label>"+inlib::to_xml(mgr->name())+"</label>";              std::string ss = h->hierarchy(arg2);
            inlib::replace(ss,"<tree>","");
            inlib::replace(ss,"</tree>","");
            sout += ss;
            sout += "</treeItem>";
          }
        }}
        sout += "</tree>";
        aValue = sout;

      } else {
        if(!aUI.session().parameterValue(arg1,aValue)) {
          std::ostream& out = aUI.session().cout();
          out << "@session@ : session parameter " 
              << inlib::sout(arg1) << " not found."
              << std::endl;
          return false;
        }
      }

    //  @directory@current@
    } else if((words.size()==2)&&
              (words[0]=="directory")&&(words[1]=="current")) {
      if(!inlib::dir::pwd(aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@directory@current@ : failed." << std::endl;
        return false;
      }

    //  @directory@<directory>@<mask>@
    } else if((words.size()==3)&&(words[0]=="directory")) {
      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string path;
      inlib::file_name(arg1,path);
      std::vector<std::string> files;
      if(!inlib::dir::entries(path,files,false)) {
        std::ostream& out = aUI.session().cout();
        out << "@directory@ : inlib::dir::entries failed." 
            << std::endl;
        return false;
      } else {
        files = smanip_match(files,arg2);
        files = smanip_prepend(arg1+"/",files);
        aValue = inlib::tos<std::string>(files,"\n");
      }

    //  @files@<directory>@<mask>@
    } else if((words.size()==3)&&(words[0]=="files")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string path;
      inlib::file_name(arg1,path);
      std::vector<std::string> files;
      if(!inlib::dir::entries(path,files,false)) {
        std::ostream& out = aUI.session().cout();
        out << "@files@ : inlib::dir::entries failed." 
            << std::endl;
        return false;
      } else {
        files = smanip_match(files,arg2);
        aValue = inlib::tos<std::string>(files,"\n");
      }

    //  @file@<file>@
    } else if((words.size()==2)&&(words[0]=="file")) {
      std::string arg;
      if(!treat_arg(aUI,words[1],arg)) return false;
      std::vector<std::string> text;
      std::string p;
      inlib::file_name(arg,p);
      if(!inlib::file::read(p,text)) {
        std::ostream& out = aUI.session().cout();
        out << "@file@ : can't read file " << inlib::sout(arg) << "."
            << std::endl;
        return false;
      } else {
        //FIXME : check with edit exec .py. 
        //aValue = inlib::tos<std::string>(text,"\n",true);
        aValue = inlib::tos<std::string>(text,"\n");
      }

    //  @tmpname@
    } else if((words.size()==1)&&(words[0]=="tmpname")) {
      if(!inlib::tmpname(".","","",aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@tmpname@ :"
            << " can't get tmp file name."
            << std::endl;
        return false;
      }

    //  @tmpname@<dir>@<prefix>@<sufix>@
    } else if((words.size()==4)&&(words[0]=="tmpname")) {
      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string arg3;
      if(!treat_arg(aUI,words[3],arg3)) return false;
      if(!inlib::tmpname(arg1,arg2,arg3,aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@tmpname@ :"
            << " can't get tmp file name."
            << std::endl;
        return false;
      }

    //  @path@<string>@
    } else if((words.size()==2)&&(words[0]=="path")) {
      std::string arg;
      if(!treat_arg(aUI,words[1],arg)) return false;
      inlib::file_name(arg,aValue);

    //  @suffix@<string.suffix>@
    } else if((words.size()==2)&&(words[0]=="suffix")) {
      std::string arg;
      if(!treat_arg(aUI,words[1],arg)) return false;
      aValue = inlib::suffix(arg);

    //  @char@<decimal code>@
    } else if((words.size()==2)&&(words[0]=="char")) {
      std::string arg;
      if(!treat_arg(aUI,words[1],arg)) return false;
      unsigned int code;
      if(!inlib::to<unsigned int>(arg,code)) {
        std::ostream& out = aUI.session().cout();
        out << "@char@ : " 
            << inlib::sout(arg) << " not an unsigned int."
            << std::endl;
        return false;
      }
      inlib::sprintf(aValue,2,"%c",code);

    //  @shell@<widget>@
    } else if((words.size()==2)&&(words[0]=="shell")) {
      std::string arg;
      if(!treat_arg(aUI,words[1],arg)) return false;
      std::vector<std::string> found = aUI.findParent(arg,"shell","");
      if(!found.size()||!found[0].size()) {
        std::ostream& out = aUI.session().cout();
        out << "@shell@ : shell not found for " << inlib::sout(arg) 
            << std::endl;
        return false;
      }
      aValue = found[0];

    //  @parent@<widget>@
    } else if((words.size()==2)&&(words[0]=="parent")) {
      std::string arg;
      if(!treat_arg(aUI,words[1],arg)) return false;
      std::vector<std::string> found = aUI.findParent(arg,"name","");
      if(!found.size()||!found[0].size()) {
        std::ostream& out = aUI.session().cout();
        out << "@parent@ : parent not found for " << inlib::sout(arg) 
            << std::endl;
        return false;
      }
      aValue = found[0];

    //  @children@<widget>@<what>@<parameter>@
    } else if((words.size()==4)&&(words[0]=="children")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string arg3;
      if(!treat_arg(aUI,words[3],arg3)) return false;

      aValue = inlib::tos<std::string>(aUI.findChildren(arg1,arg2,arg3),"\n");

    //  @child@<widget>@<what>@<parameter>@<index>
    } else if((words.size()==5)&&(words[0]=="child")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string arg3;
      if(!treat_arg(aUI,words[3],arg3)) return false;
      std::string arg4;
      if(!treat_arg(aUI,words[4],arg4)) return false;

      std::vector<std::string> items = aUI.findChildren(arg1,arg2,arg3);
  
      if(items.empty()) {
        std::ostream& out = aUI.session().cout();
        out << "@child@ : empty list." 
            << std::endl;
        return false; //FIXME : is it an error ?
      } else {
        if(arg4=="first") {
          aValue = items[0];
        } else if(arg4=="last") {
          aValue = items[items.size()-1];
        } else {
          unsigned int index;
          if(!inlib::to<unsigned int>(arg4,index)) {
            std::ostream& out = aUI.session().cout();
            out << "@child@ : " 
                << inlib::sout(arg4) << " not an unsigned int."
                << std::endl;
            return false;
          }
          if(index>=items.size()) {
            std::ostream& out = aUI.session().cout();
            out << "@child@ : index " 
                << inlib::sout(arg4) 
                << " out of range (" << int(items.size()) << ")."
                << std::endl;
            return false; //FIXME : is it an error ?
          }
          aValue = items[index];
        }
      }

    //  @name@<top>@<prefix>@
    } else if((words.size()==3)&&(words[0]=="name")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      aValue = Slash::build_name(aUI,arg1,arg2);

    //INTERNAL : used in ViewerWindows.onx for Copy
    //  @name_from_current@<top>@
    } else if((words.size()==2)&&(words[0]=="name_from_current")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;

      Slash::UI::IWidget* widget = aUI.currentWidget();
      if(!widget) {
        std::ostream& out = aUI.session().cout();
        out << "@name@ : "
            << " no current widget."
            << std::endl;
        return false;
      }
      std::string pref = widget->name()+"_";
      aValue = Slash::build_name(aUI,arg1,pref);

    //  @nth@<index>@<list>@
    } else if((words.size()==3)&&(words[0]=="nth")) {
      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::vector<std::string> items = smanip_text(arg2);
      if(arg1=="last") {
        if(!items.size()) {
          std::ostream& out = aUI.session().cout();
          out << "@nth@ : can't get last. The list is empty." 
              << std::endl;
          return false;
        }
        aValue = items[items.size()-1];
      } else if(arg1=="first") {
        if(!items.size()) {
          std::ostream& out = aUI.session().cout();
          out << "@nth@ : can't get first. The list is empty." 
              << std::endl;
          return false;
        }
        aValue = items[0];
      } else {
        unsigned int index;
        if(!inlib::to<unsigned int>(arg1,index)) {
          std::ostream& out = aUI.session().cout();
          out << "@nth@ : " 
              << inlib::sout(arg1) << " not an unsigned int."
              << std::endl;
          return false;
        }
        if(index>=items.size()) {
          std::ostream& out = aUI.session().cout();
          out << "@nth@ : index " 
              << inlib::sout(arg1)
              << " out of range (" << int(items.size()) << ")."
              << std::endl;
          return false;
        }
        aValue = items[index];
      }

    //  @next@<item>@<list>@<opt>@
    } else if((words.size()==4)&&(words[0]=="next")) {
      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string arg3;
      if(!treat_arg(aUI,words[3],arg3)) return false;


     {std::vector<std::string> lines = smanip_text(arg2);
      for(std::vector<std::string>::const_iterator it = lines.begin();
          it!=lines.end();++it) {
        std::string item = (*it); 
        if(arg3=="strip_key") inlib::strip(item);
        if(arg1==item) {
          ++it;
          if(it==lines.end()) {
            std::ostream& out = aUI.session().cout();
            out << "@next@ :" 
                << " item " << inlib::sout(arg1) << " is the last in the list."
                << std::endl;
            return false;
          }
          aValue = *it;
          if(arg3=="strip_value") inlib::strip(aValue);
          break;
        }
      }}

    //  @filter@<mask>@<list>@
    } else if((words.size()==3)&&(words[0]=="filter")) {
      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      std::vector<std::string> values;

      std::vector<std::string> lines = smanip_text(arg2);
      for(std::vector<std::string>::const_iterator it = lines.begin();
          it!=lines.end();++it) {
        if(inlib::match(*it,arg1)) values.push_back(*it);
      }
      aValue = inlib::tos<std::string>(values,"\n");

    //  @interpreter@<interpreter>@<what>@
    } else if((words.size()==3)&&(words[0]=="interpreter")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      Slash::UI::IScriptManager* mgr = Slash::scriptManager(aUI.session());
      if(!mgr) {
        std::ostream& out = aUI.session().cout();
        out << "@interpreter@ : script manager not found."
            << std::endl;
        return false;
      }
      Slash::UI::IInterpreter* interp = mgr->findInterpreter(arg1);
      if(!interp) {
        std::ostream& out = aUI.session().cout();
        out << "@interpreter@ : interpreter " 
            << inlib::sout(arg1) << " not found."
            << std::endl;
        return false;
      }
      if(arg2=="suffix") {
        aValue = interp->suffix();
      } else {
        std::ostream& out = aUI.session().cout();
        out << "@interpreter@ : keyword " << inlib::sout(arg2) 
            << " not valid for script manager."
            << std::endl;
        return false;
      }

    //  @complete@<interpreter>@<string>@
    } else if((words.size()==3)&&(words[0]=="complete")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      Slash::UI::IScriptManager* mgr = Slash::scriptManager(aUI.session());
      if(!mgr) {
        std::ostream& out = aUI.session().cout();
        out << "@complete@ : script manager not found."
            << std::endl;
        return false;
      }
      Slash::UI::IInterpreter* interp = mgr->findInterpreter(arg1);
      if(!interp) {
        std::ostream& out = aUI.session().cout();
        out << "@complete@ : interpreter " 
            << inlib::sout(arg1) << " not found."
            << std::endl;
        return false;
      }
      if(!interp->complete(arg2,aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@complete@ : can't complete " << inlib::sout(arg2)
            << " with interpreter " << inlib::sout(arg1) << "."
            << std::endl;
        return false;
      }

    //  @cvt@<format>@<string>@
    } else if((words.size()==3)&&(words[0]=="cvt")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      if(arg1=="rgb") {
        if(!inlib::to_rgbs(arg2,aValue)) {
          std::ostream& out = aUI.session().cout();
          out << "@cvt@ : can't convert " << inlib::sout(arg2) << " to rgb."
              << std::endl;
          return false;
        }
      } else {
        std::ostream& out = aUI.session().cout();
        out << "@cvt@ : format " << inlib::sout(arg1) << " is unknown."
            << std::endl;
        return false;
      }

    //  @manager@<manager>@<what>@
    } else if((words.size()==3)&&(words[0]=="manager")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      if(arg1=="AccessorManager") {
        Slash::Data::IProcessor* mgr = Slash::accessorManager(aUI.session());
        if(!mgr) {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : manager " << inlib::sout(arg1) << " not found."
              << std::endl;
          return false;
        }
        if(arg2=="accessors") {
          aValue = inlib::tos<std::string>(mgr->accessorNames(),"\n");
        } else if(arg2=="actions") {
          aValue = inlib::tos<std::string>(mgr->actionNames(),"\n");
        } else {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : keyword " << inlib::sout(arg2) 
              << " not valid for manager " << inlib::sout(arg1) << "."
              << std::endl;
          return false;
        }    
      } else if(arg1=="StyleManager") {
        Slash::UI::IStyleManager* mgr = Slash::styleManager(aUI.session());
        if(!mgr) {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : manager " << inlib::sout(arg1) << " not found."
              << std::endl;
          return false;
        }
        if(arg2=="styles") {
          aValue = inlib::tos<std::string>(mgr->styles(),"\n");
        } else {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : keyword " << inlib::sout(arg2) 
              << " not valid for manager " << inlib::sout(arg1) << "."
              << std::endl;
          return false;
        }    
      } else if(arg1=="ScriptManager") {
        Slash::UI::IScriptManager* mgr = Slash::scriptManager(aUI.session());
        if(!mgr) {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : manager " << inlib::sout(arg1) << " not found."
              << std::endl;
          return false;
        }
        if(arg2=="interpreters") {
          aValue = inlib::tos<std::string>(mgr->names(),"\n");
        } else {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : keyword " << inlib::sout(arg2) 
              << " not valid for manager " << inlib::sout(arg1) << "."
              << std::endl;
          return false;
        }    
      } else {
        // try to seek a IParameterManager :
        Slash::Core::IParameterManager* mgr = 
          Slash_findManager(aUI.session(),arg1,Slash::Core::IParameterManager);
        if(!mgr) {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : " << inlib::sout(arg1) << " not found."
              << std::endl;
          return false;
        }
        if(!mgr->parameterValue(arg2,aValue)) {
          std::ostream& out = aUI.session().cout();
          out << "@manager@ : keyword " << inlib::sout(arg2) 
              << " not valid for manager " << inlib::sout(arg1) << "."
              << std::endl;
          return false;
        }
      }

    //  @da@<action>@<param>@
    } else if((words.size()==3)&&(words[0]=="da")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      Slash::Data::IProcessor* da = Slash::accessorManager(aUI.session());
      if(!da) {
        std::ostream& out = aUI.session().cout();
        out << "@da@ : AccessorManager " << " not found."
            << std::endl;
        return false;
      }
      std::vector<std::string> args = smanip_text(arg2);
      aValue = da->execute(arg1,args);

    //  @cyclic@<name>@<what>@
    } else if((words.size()==3)&&(words[0]=="cyclic")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      Slash::UI::ICyclic* cyclic = aUI.findCyclic(arg1);
      if(!cyclic) {
        std::ostream& out = aUI.session().cout();
        out << "@cyclic@ : cyclic " << inlib::sout(arg1) << " not found."
            << std::endl;
        return false;
      }
      if(arg2=="iterations") {
        aValue = inlib::to<int>(cyclic->iterations());
      } else {
        std::ostream& out = aUI.session().cout();
        out << "@cyclic@ : keyword " << inlib::sout(arg2) 
            << " not valid for cyclic " << inlib::sout(arg1) << "."
            << std::endl;
        return false;
      }    

    //  @calc@<formula>@<list values>@
    } else if((words.size()==3)&&(words[0]=="calc")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;

      std::vector<std::string> vals = smanip_text(arg2);
      if(!Lib::calc(aUI.session().cout(),arg1,vals,aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@calc@ : failed for formula " 
            << " and variables " << inlib::sout(arg2) << "."
            << std::endl;
        return false;
      }

    //  @replace@<string>@<old>@<new>@
    } else if((words.size()==4)&&(words[0]=="replace")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      std::string arg2;
      if(!treat_arg(aUI,words[2],arg2)) return false;
      std::string arg3;
      if(!treat_arg(aUI,words[3],arg3)) return false;

      inlib::replace(arg1,arg2,arg3);
      aValue = arg1;

    //  @ask@<prompt>@
    } else if((words.size()==2)&&(words[0]=="ask")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;

      aValue = aUI.ask(arg1);

    //  @ui_is_a@<what>@
    } else if((words.size()==2)&&(words[0]=="ui_is_a")) {
      // See examples/OnX/sdl.onx.

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      aValue = aUI.cast(arg1)?"true":"false";

    //  @env@<environment variable>@
    } else if((words.size()==2)&&(words[0]=="env")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;
      if(!inlib::getenv(arg1,aValue)) {
        std::ostream& out = aUI.session().cout();
        out << "@env@ : environment variable " 
            << inlib::sout(arg1) << " not defined."
            << std::endl;
        return false;
      }

    //  @net@<file>@
      /*
    } else if((words.size()==2)&&(words[0]=="net")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;

      aValue = arg1;

      if(!Slash::get_file(aUI.session(),arg1)) {
        std::ostream& out = aUI.session().cout();
        out << "@net@ : failed to get file " 
            << inlib::sout(arg1) << "."
            << std::endl;
        return false;
      }
      */

    //  @tree_items@<list>@
    } else if((words.size()==2)&&(words[0]=="tree_items")) {

      std::string arg1;
      if(!treat_arg(aUI,words[1],arg1)) return false;

      aValue = "<tree>";
     {std::vector<std::string> lines = smanip_text(arg1);
      for(std::vector<std::string>::const_iterator it = lines.begin();
          it!=lines.end();++it) {
        aValue += "<treeItem><label>"+(*it)+"</label></treeItem>";
      }}
      aValue += "</tree>";

    } else {
      std::ostream& out = aUI.session().cout();
      out << "@@ : bad syntax in " << inlib::sout(s) << "."
          << std::endl;
      return false;
    }
  }
  return true;
}
