#ifndef Slash_Tools_Session_h
#define Slash_Tools_Session_h

#include <Slash/Core/ISession.h>

#include <inlib/sto>

namespace Slash {

inline bool set_parameter(Slash::Core::ISession& aSession,const std::string& aWhat,bool aValue){
  return aSession.setParameter(aWhat,aValue?"true":"false");
}

template <class T>
inline bool set_parameter(Slash::Core::ISession& aSession,const std::string& aWhat,const T& aValue){
  return aSession.setParameter(aWhat,inlib::to<T>(aValue));
}

inline bool parameter(Slash::Core::ISession& aSession,const std::string& aWhat,bool& a_value){
  std::string s;
  if(!aSession.parameterValue(aWhat,s)) {a_value = false;return false;}
  return inlib::to(s,a_value);
}

template <class T>
inline bool parameter(Slash::Core::ISession& aSession,const std::string& aWhat,T& a_value){
  std::string s;
  if(!aSession.parameterValue(aWhat,s)) {a_value = T();return false;}
  return inlib::to<T>(s,a_value);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

inline void set_color(Slash::Core::ISession& aSession,const std::string& aColor){aSession.setParameter("modeling.color",aColor);}

inline void modeling_none(Slash::Core::ISession& aSession){
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

inline std::string style_from_modeling(Slash::Core::ISession& aSession){
  //used in InventorAccessor.
  //the returned string is understood by HEPVis/SbStyle.

  std::string sstyle;
  std::string value;

  aSession.parameterValue("modeling.color",value);
  if((value.size())&&(value!="none")) sstyle += "\ncolor "+value;

  aSession.parameterValue("modeling.highlightColor",value);
  if((value.size())&&(value!="none")) sstyle += "\nhighlightColor "+value;

  aSession.parameterValue("modeling.transparency",value);
  if((value.size())&&(value!="none")) sstyle += "\ntransparency "+value;
  
  aSession.parameterValue("modeling.lineStyle",value); // Deprecated :
  if((value.size())&&(value!="none")) sstyle += "\nlineStyle "+value;

  aSession.parameterValue("modeling.linePattern",value);
  if((value.size())&&(value!="none")) sstyle += "\nlinePattern "+value;

  aSession.parameterValue("modeling.lineWidth",value);
  if((value.size())&&(value!="none")) sstyle += "\nlineWidth "+value;
  
  //Polyhedron :
  aSession.parameterValue("modeling.projection",value);
  if((value.size())&&(value!="none")) sstyle += "\nprojection "+value;

  aSession.parameterValue("modeling.cut",value);
  if((value.size())&&(value!="none")) sstyle += "\ncut "+value;

  aSession.parameterValue("modeling.rotationSteps",value);
  if((value.size())&&(value!="none")) sstyle += "\nrotationSteps "+value;

  aSession.parameterValue("modeling.modeling",value);
  if((value.size())&&(value!="none")) sstyle += "\nmodeling "+value;
  
  //Backward compatibility.
  aSession.parameterValue("modeling.type",value);
  if((value.size())&&(value!="none")) {
    if(value=="text") {
      sstyle += "\nmodeling texts";
    } else {
      sstyle += "\nmodeling "+value;
    }
  }

  return sstyle;

}

}  

#endif
