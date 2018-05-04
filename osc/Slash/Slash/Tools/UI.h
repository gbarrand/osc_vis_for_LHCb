#ifndef Slash_Tools_UI_h
#define Slash_Tools_UI_h

#include <Slash/Tools/Managers.h>
#include <Slash/UI/IUI.h>

namespace Slash {

inline std::string s_default() {return "default";}
inline std::string s_current() {return "@current@";}

inline Slash::UI::IUI* find_UI(Slash::Core::ISession& aSession,const std::string& aName = s_default(),bool aWarn = false) {
  Slash::UI::IUI_Manager* mgr = Slash::uiManager(aSession);
  if(!mgr) {
    if(aWarn) {
      aSession.cout() << "Slash::find_UI :"
                      << " UI_Manager not found." 
                      << std::endl;
    }
    return 0;
  }
  return mgr->find(aName);
}

}

#include <inlib/sout>
#include <inlib/sto>
#include <inlib/tos>
#include <inlib/words>
#include <inlib/svalues>
#include <inlib/vmanip>

namespace Slash {

inline Slash::UI::IWidget* find_widget(Slash::UI::IUI& aUI,const std::string& aString) {
  Slash::UI::IWidget* widget = 0;
  if(aString==s_current()) {
    widget = aUI.currentWidget();
    if(!widget) {
      std::ostream& out = aUI.session().cout();
      out << "Slash::find_widget : "
          << " no current widget."
          << std::endl;
      return 0;
    }
  } else {
    widget = aUI.findWidget(aString);
    if(!widget) {
      std::ostream& out = aUI.session().cout();
      out << "Slash::find_widget : "
          << inlib::sout(aString) << " not found."
          << std::endl;
      return 0;
    }
  }
  return widget;
}

inline std::string build_name(Slash::UI::IUI& aUI,const std::string& aTop,const std::string& aPrefix) {
  std::string::size_type l = aPrefix.size();

  std::vector<unsigned int> uis;
  std::vector<std::string> names = aUI.findChildren(aTop,"name","");
  for(std::vector<std::string>::const_iterator it = names.begin();
      it!=names.end();++it) {
    //viewer_10
    //012345678
    if((*it).substr(0,l)==aPrefix) {
      std::string end = (*it).substr(l,(*it).size()-l);
      unsigned int ui;
      if(inlib::to<unsigned int>(end,ui)) {
        uis.push_back(ui);
      }
    }
  }
  unsigned int mx;
  if(!inlib::maximum<unsigned int>(uis,mx)) {
    return aPrefix+"0";
  } else {
    return aPrefix+inlib::to<unsigned int>(mx+1);
  }
}

inline bool set_parameter(Slash::UI::IUI& aUI,const std::string& aWhat,bool aValue){return aUI.setParameter(aWhat,std::string(aValue?"true":"false"));}

template <class T>
inline bool set_parameter(Slash::UI::IUI& aUI,const std::string& aWhat,T aValue){return aUI.setParameter(aWhat,inlib::to<T>(aValue));}

inline bool parameter(Slash::UI::IUI& aUI,const std::string& aWhat,std::string& aValue){
  if(!aUI.parameterValue(aWhat,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    aValue = "";
    return false;
  }
  return true;
}

inline bool parameter(Slash::UI::IUI& aUI,const std::string& aWhat,bool& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    aValue = false;
    return false;
  }
  if(!inlib::to(s,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " not a boolean."
        << std::endl;
    return false;
  }
  return true;
}

template <class T>
inline bool parameter(Slash::UI::IUI& aUI,const std::string& aWhat,T& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    aValue = 0;
    return false;
  }
  if(!inlib::to<T>(s,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " is of bad type."
        << std::endl;
    return false;
  }
  return true;
}

template <class T>
inline bool parameter(Slash::UI::IUI& aUI,const std::string& aWhat,std::vector<T>& aValue){
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    aValue.clear();
    return false;
  }
  if(!inlib::values<T>(s," ",false,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " is of bad type."
        << std::endl;
    return false;
  }
  return true;
}

inline void add_prompt(Slash::UI::IUI& aUI,const std::string& aArg,bool aWarn = true) {
  // Add a prompt in the consolePrompt list.
  if(!aWarn) {
    if(!aUI.findWidget("consolePrompt")) return;
  }
  std::string value;
  if(!aUI.parameterValue("consolePrompt.items",value)) return;
  std::vector<std::string> items;
  inlib::words(value,"\n",false,items);
  items.push_back(aArg);
  inlib::unique<std::string>(items);
  aUI.setParameter("consolePrompt.items",inlib::tos<std::string>(items,"\n"));
}

}

//////////////////////////////////////////////////////////////////////////////
/// ISoViewer ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <Slash/UI/IWidget.h>
#include <Slash/UI/ISoViewer.h>

#include <inlib/cast>

namespace Slash {

inline Slash::UI::ISoViewer* find_SoViewer(Slash::UI::IUI& aUI,const std::string& aString) {
  Slash::UI::IWidget* widget = find_widget(aUI,aString);
  if(!widget) return 0;
  Slash::UI::ISoViewer* soViewer = INLIB_CAST(*widget,Slash::UI::ISoViewer);
  if(!soViewer) {
    std::ostream& out = aUI.session().cout();
    out << "Slash::find_SoViewer : "
        << inlib::sout(aString) << " not a Slash::UI::ISoViewer."
        << std::endl;
    return 0;
  }
  return soViewer;
}

inline Slash::UI::ISoViewer* cast_SoViewer(Slash::UI::IWidget& a_widget){
  return INLIB_CAST(a_widget,Slash::UI::ISoViewer);
}


}

#endif
