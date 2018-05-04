
#include <Slash/Core/ISession.h>
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Inventor.h>

#include <inlib/args>
#include <inlib/sprintf>

// HEPVis :
#include <HEPVis/SbString.h>
#include <HEPVis/SbMath.h>
#include <HEPVis/SbStyle.h>
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoAxis.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodes/SoLineStyle.h>
#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodes/SoStyle.h>

//////////////////////////////////////////////////////////////////////////////
/// So to UI /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static void So_UI_LineStyle(
 Slash::UI::IUI& aUI
,SoLineStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbLinePattern v = aStyle.pattern.getValue();
  std::string s;
  inlib::sprintf(s,32,"0x%x",v);
  aUI.setParameter(aField+"linePattern.default",s);}

 {int v = aStyle.width.getValue();
  aUI.setParameter(aField+"lineWidth.default",inlib::to<int>(v));}
}
//////////////////////////////////////////////////////////////////////////////
static void So_UI_TextStyle(
 Slash::UI::IUI& aUI
,SoTextStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
/*
  SbColor color = aStyle.color.getValue();
  SbString sbs;
  SbStyle::colorToString(color,sbs);
  aUI.setParameter(aField+"color.default",sbs.getString());

  float transparency = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",
    Lib::smanip::tostring(transparency));
*/
 {SbString v = aStyle.fontName.getValue();
  aUI.setParameter(aField+"fontName.default",v.getString());}

 {int v = aStyle.fontSize.getValue();
   aUI.setParameter(aField+"fontSize.default",inlib::to<int>(v));}
}
//////////////////////////////////////////////////////////////////////////////
static void So_UI_Style(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbLinePattern v = aStyle.linePattern.getValue();
  std::string s;
  inlib::sprintf(s,32,"0x%x",v);
  aUI.setParameter(aField+"linePattern.default",s);}

 {int v = aStyle.lineWidth.getValue();
  aUI.setParameter(aField+"lineWidth.default",inlib::to<int>(v));}

 {SbString s = aStyle.fontName.getValue();
  aUI.setParameter(aField+"fontName.default",s.getString());}

 {int v = aStyle.fontSize.getValue();
  aUI.setParameter(aField+"fontSize.default",inlib::to<int>(v));}
}
//FIXME : rm the below : do it with So_UI_LineStyle :
//////////////////////////////////////////////////////////////////////////////
static void So_UI_StyleLine(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbLinePattern v = aStyle.linePattern.getValue();
  std::string s;
  inlib::sprintf(s,32,"0x%x",v);
  aUI.setParameter(aField+"linePattern.default",s);}

 {int v = aStyle.lineWidth.getValue();
  aUI.setParameter(aField+"lineWidth.default",inlib::to<int>(v));}
}
//////////////////////////////////////////////////////////////////////////////
static void So_UI_StyleArea(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbString v;
  aStyle.areaStyle.get(v); //enum
  aUI.setParameter(aField+"areaStyle.default",v.getString());}

}
//////////////////////////////////////////////////////////////////////////////
static void So_UI_StyleHatch(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {float v = aStyle.spacing.getValue();
  aUI.setParameter(aField+"spacing.default",inlib::to<float>(v));}

 {float v = aStyle.stripWidth.getValue();
  aUI.setParameter(aField+"stripWidth.default",inlib::to<float>(v));}

 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbLinePattern v = aStyle.linePattern.getValue();
  std::string s;
  inlib::sprintf(s,32,"0x%x",v);
  aUI.setParameter(aField+"linePattern.default",s);}

 {int v = aStyle.lineWidth.getValue();
  aUI.setParameter(aField+"lineWidth.default",inlib::to<int>(v));}

 {float v = aStyle.angle.getValue();
  v = v * 180 / FM_PI;
  aUI.setParameter(aField+"angle.default",inlib::to<float>(v));}
}
//////////////////////////////////////////////////////////////////////////////
static void So_UI_StyleData(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
,bool aPainting
,bool aHatching
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aUI.setParameter(aField+"index.value","0");

 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbLinePattern v = aStyle.linePattern.getValue();
  std::string s;
  inlib::sprintf(s,32,"0x%x",v);
  aUI.setParameter(aField+"linePattern.default",s);}

 {int v = aStyle.lineWidth.getValue();
  aUI.setParameter(aField+"lineWidth.default",inlib::to<int>(v));}

 {SbString v = aStyle.modeling.getValue();
  aUI.setParameter(aField+"modeling.default",v.getString());}

 {int v = aStyle.multiNodeLimit.getValue();
  aUI.setParameter(aField+"multiNodeLimit.default",inlib::to<int>(v));}

 {SbString v;
  aStyle.areaStyle.get(v); //enum
  aUI.setParameter(aField+"areaStyle.default",v.getString());}

 {int v = aStyle.markerSize.getValue();
  aUI.setParameter(aField+"markerSize.default",inlib::to<int>(v));}

 {SbString v;
  aStyle.markerStyle.get(v); //enum
  aUI.setParameter(aField+"markerStyle.default",v.getString());}

 {SbBool v = aStyle.editable.getValue();
  aUI.setParameter(aField+"editable.value",v?"true":"false");}
 
  if(aPainting) {
   {SbString v;
    aStyle.painting.get(v); //enum
    aUI.setParameter(aField+"painting.default",v.getString());}
  }

  if(aHatching) { 
   {SbString v;
    aStyle.hatching.get(v); //enum
    aUI.setParameter(aField+"hatching.default",v.getString());}
  }
 
}
//////////////////////////////////////////////////////////////////////////////
static void So_UI_StyleErrors(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {SbColor v = aStyle.color.getValue();
  SbString s;
  SbStyle::colorToString(v,s);
  aUI.setParameter(aField+"color.default",s.getString());}

 {float v = aStyle.transparency.getValue();
  aUI.setParameter(aField+"transparency.value",inlib::to<float>(v));}

 {SbLinePattern v = aStyle.linePattern.getValue();
  std::string s;
  inlib::sprintf(s,32,"0x%x",v);
  aUI.setParameter(aField+"linePattern.default",s);}

 {int v = aStyle.lineWidth.getValue();
  aUI.setParameter(aField+"lineWidth.default",inlib::to<int>(v));}

 {int v = aStyle.markerSize.getValue();
  aUI.setParameter(aField+"markerSize.default",inlib::to<int>(v));}

 {SbString v;
  aStyle.markerStyle.get(v); //enum
  aUI.setParameter(aField+"markerStyle.default",v.getString());}
}
//////////////////////////////////////////////////////////////////////////////
/// UI to So /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
inline bool ui_parameterValue(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,std::string& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aUI.parameterValue(aWhat,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    aValue = "";
    return false;
  }
  if(aValue=="unchanged") {
    aChanged = false;
    return true;
  }
  aChanged = true;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
inline bool ui_parameterValue(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,SbColor& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    return false;
  }
  if(s=="unchanged") {
    aChanged = false;
    return true;
  }
  if(SbStyle::stringToColor(s.c_str(),aValue)==FALSE) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " not a color."
        << std::endl;
    return false;
  }
  aChanged = true;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
inline bool ui_parameterValue(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,int& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = 0;
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    return false;
  }
  if(s=="unchanged") {
    aChanged = false;
    return true;
  }
  if(!inlib::to<int>(s,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " not an int."
        << std::endl;
    return false;
  }
  aChanged = true;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
inline bool ui_line_pattern(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,unsigned int& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = 0;
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    return false;
  }
  if(s=="unchanged") {
    aChanged = false;
    return true;
  }
  if(inlib::to<unsigned int>(s,aValue)) {
    aChanged = true;
    return true;
  }
  if(s=="solid") {
    aValue = SbLinePattern_solid;
    aChanged = true;
    return true;
  }
  if(s=="dashed") {
    aValue = SbLinePattern_dashed;
    aChanged = true;
    return true;
  }
  if(s=="dotted") {
    aValue = SbLinePattern_dotted;
    aChanged = true;
    return true;
  }
  if(s=="dash_dotted") {
    aValue = SbLinePattern_dash_dotted;
    aChanged = true;
    return true;
  }

  std::ostream& out = aUI.session().cout();
  out << "Passed value " << inlib::sout(s)
      << " for parameter " << inlib::sout(aWhat) 
      << " not a line pattern."
      << std::endl;
  return false;
}
//////////////////////////////////////////////////////////////////////////////
inline bool ui_parameterValue(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,double& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = 0;
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    return false;
  }
  if(s=="unchanged") {
    aChanged = false;
    return true;
  }
  if(!inlib::to<double>(s,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " not a double."
        << std::endl;
    return false;
  }
  aChanged = true;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
inline bool ui_parameterValue(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,float& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = 0;
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    return false;
  }
  if(s=="unchanged") {
    aChanged = false;
    return true;
  }
  if(!inlib::to<float>(s,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " not a float."
        << std::endl;
    return false;
  }
  aChanged = true;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
inline bool ui_parameterValue(
 Slash::UI::IUI& aUI
,const std::string& aWhat
,bool& aValue
,bool& aChanged
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = false;
  std::string s;
  if(!aUI.parameterValue(aWhat,s)) {
    std::ostream& out = aUI.session().cout();
    out << "Paramater " << inlib::sout(aWhat) << " not found."
        << std::endl;
    return false;
  }
  if(s=="unchanged") {
    aChanged = false;
    return true;
  }
  if(!inlib::to(s,aValue)) {
    std::ostream& out = aUI.session().cout();
    out << "Passed value " << inlib::sout(s)
        << " for parameter " << inlib::sout(aWhat) 
        << " not a boolean."
        << std::endl;
    return false;
  }
  aChanged = true;
  return true;
}

//////////////////////////////////////////////////////////////////////////////
static bool UI_So_LineStyle(
 Slash::UI::IUI& aUI
,SoLineStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) aStyle.color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) aStyle.transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) aStyle.width.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) aStyle.pattern.setValue((SbLinePattern)v);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_TextStyle(
 Slash::UI::IUI& aUI
,SoTextStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME color.
  //FIXME transparency.

  bool changed;

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"fontName.value",v,changed)) return false;
  if(changed) aStyle.fontName.setValue(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"fontSize.value",v,changed)) return false;
  if(changed) aStyle.fontSize.setValue((SbLinePattern)v);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_Style(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) aStyle.color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) aStyle.transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) aStyle.lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) aStyle.linePattern.setValue((SbLinePattern)v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"fontName.value",v,changed)) return false;
  if(changed) aStyle.fontName.setValue(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"fontSize.value",v,changed)) return false;
  if(changed) aStyle.fontSize.setValue((SbLinePattern)v);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleLine(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) aStyle.color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) aStyle.transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) aStyle.lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) aStyle.linePattern.setValue((SbLinePattern)v);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleArea(
 Slash::UI::IUI& aUI
,SoStyle& aStyle
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) aStyle.color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) aStyle.transparency.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"areaStyle.value",v,changed)) return false;
  if(changed) aStyle.areaStyle.set(v.c_str());}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleBins(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = 0;
  if(!Slash::parameter<int>(aUI,aField+"index.value",index)) return false;

  SoStyle* style = aPlotter.getBinsStyle(index);

  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) style->color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) style->transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) style->lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) style->linePattern.setValue((SbLinePattern)v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"modeling.value",v,changed)) return false;
  if(changed) style->modeling.setValue(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"multiNodeLimit.value",v,changed)) return false;
  if(changed) style->multiNodeLimit.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"areaStyle.value",v,changed)) return false;
  if(changed) style->areaStyle.set(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"markerSize.value",v,changed)) return false;
  if(changed) style->markerSize.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"markerStyle.value",v,changed)) return false;
  if(changed) style->markerStyle.set(v.c_str());}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"painting.value",v,changed)) return false;
  if(changed) style->painting.set(v.c_str());}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"hatching.value",v,changed)) return false;
  if(changed) style->hatching.set(v.c_str());}

 {bool v;
  if(!ui_parameterValue(aUI,aField+"editable.value",v,changed)) return false;
  if(changed) style->editable.setValue(v?TRUE:FALSE);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StylePoints(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = 0;
  if(!Slash::parameter<int>(aUI,aField+"index.value",index)) return false;

  SoStyle* style = aPlotter.getPointsStyle(index);

  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) style->color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) style->transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) style->lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) style->linePattern.setValue((SbLinePattern)v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"modeling.value",v,changed)) return false;
  if(changed) style->modeling.setValue(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"multiNodeLimit.value",v,changed)) return false;
  if(changed) style->multiNodeLimit.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"areaStyle.value",v,changed)) return false;
  if(changed) style->areaStyle.set(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"markerSize.value",v,changed)) return false;
  if(changed) style->markerSize.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"markerStyle.value",v,changed)) return false;
  if(changed) style->markerStyle.set(v.c_str());}

 {bool v;
  if(!ui_parameterValue(aUI,aField+"editable.value",v,changed)) return false;
  if(changed) style->editable.setValue(v?TRUE:FALSE);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleFunction(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = 0;
  if(!Slash::parameter<int>(aUI,aField+"index.value",index)) return false;

  SoStyle* style = aPlotter.getFunctionStyle(index);

  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) style->color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) style->transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) style->lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) style->linePattern.setValue((SbLinePattern)v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"modeling.value",v,changed)) return false;
  if(changed) style->modeling.setValue(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"multiNodeLimit.value",v,changed)) return false;
  if(changed) style->multiNodeLimit.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"areaStyle.value",v,changed)) return false;
  if(changed) style->areaStyle.set(v.c_str());}

 {int v;
  if(!ui_parameterValue(aUI,aField+"markerSize.value",v,changed)) return false;
  if(changed) style->markerSize.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"markerStyle.value",v,changed)) return false;
  if(changed) style->markerStyle.set(v.c_str());}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"painting.value",v,changed)) return false;
  if(changed) style->painting.set(v.c_str());}

 {bool v;
  if(!ui_parameterValue(aUI,aField+"editable.value",v,changed)) return false;
  if(changed) style->editable.setValue(v?TRUE:FALSE);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleRightHatch(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = 0;
  if(!Slash::parameter<int>(aUI,aField+"index.value",index)) return false;

  SoStyle* style = aPlotter.getRightHatchStyle(index);

  bool changed;

 {float v;
  if(!ui_parameterValue(aUI,aField+"spacing.value",v,changed)) return false;
  if(changed) style->spacing.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"stripWidth.value",v,changed)) return false;
  if(changed) style->stripWidth.setValue(v);}

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) style->color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) style->transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) style->lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) style->linePattern.setValue((SbLinePattern)v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"angle.value",v,changed)) return false;
  v = v * FM_PI / 180;
  if(changed) style->angle.setValue(v);}

  return true;
}
//FIXME : use the upper.
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleLeftHatch(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = 0;
  if(!Slash::parameter<int>(aUI,aField+"index.value",index)) return false;

  SoStyle* style = aPlotter.getLeftHatchStyle(index);

  bool changed;

 {float v;
  if(!ui_parameterValue(aUI,aField+"spacing.value",v,changed)) return false;
  if(changed) style->spacing.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"stripWidth.value",v,changed)) return false;
  if(changed) style->stripWidth.setValue(v);}

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) style->color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) style->transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) style->lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) style->linePattern.setValue((SbLinePattern)v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"angle.value",v,changed)) return false;
  v = v * FM_PI / 180;
  if(changed) style->angle.setValue(v);}

  return true;
}
//////////////////////////////////////////////////////////////////////////////
static bool UI_So_StyleErrors(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
,const std::string& aField
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int index = 0;
  if(!Slash::parameter<int>(aUI,aField+"index.value",index)) return false;

  SoStyle* style = aPlotter.getErrorsStyle(index);

  bool changed;

 {SbColor v;
  if(!ui_parameterValue(aUI,aField+"color.value",v,changed)) return false;
  if(changed) style->color.setValue(v);}

 {float v;
  if(!ui_parameterValue(aUI,aField+"transparency.value",v,changed)) return false;
  if(changed) style->transparency.setValue(v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"lineWidth.value",v,changed)) return false;
  if(changed) style->lineWidth.setValue(v);}

 {unsigned int v;
  if(!ui_line_pattern(aUI,aField+"linePattern.value",v,changed)) return false;
  if(changed) style->linePattern.setValue((SbLinePattern)v);}

 {int v;
  if(!ui_parameterValue(aUI,aField+"markerSize.value",v,changed)) return false;
  if(changed) style->markerSize.setValue(v);}

 {std::string v;
  if(!ui_parameterValue(aUI,aField+"markerStyle.value",v,changed)) return false;
  if(changed) style->markerStyle.set(v.c_str());}

  return true;
}

//////////////////////////////////////////////////////////////////////////////
static void OnX_InputPlotter_updateComponentPanel(
 Slash::UI::IUI& aUI
,SoPlotterRegion& aPlotterRegion
,SoPlotter& aPlotter
) 
//////////////////////////////////////////////////////////////////////////////
// Get all parameters from plotter and update the display with thoses value
// Only update the component tabstack 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {SbBool v = aPlotter.getGridStyle()->visible.getValue();
  aUI.setParameter("OnX_InputPlotter_gridStyle.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotter.getWallStyle()->visible.getValue();
  aUI.setParameter("OnX_InputPlotter_wallStyle.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotter.getInnerFrameStyle()->visible.getValue();
  aUI.setParameter("OnX_InputPlotter_innerFrameStyle.set",v?"TRUE":"FALSE");}
 
 {SbBool v = aPlotterRegion.infosRegionVisible.getValue();
  aUI.setParameter("OnX_InputPlotter_infosVisible.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotterRegion.legendRegionVisible.getValue();
  aUI.setParameter("OnX_InputPlotter_legendVisible.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotter.getTitleStyle()->visible.getValue();
  aUI.setParameter("OnX_InputPlotter_titleStyle.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotter.titleUp.getValue();
  aUI.setParameter("OnX_InputPlotter_titleUp.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotter.superposeBins.getValue();
  aUI.setParameter("OnX_InputPlotter_superposeBins.set",v?"TRUE":"FALSE");}

 {SbBool v = aPlotter.shapeAutomated.getValue();
  aUI.setParameter("OnX_InputPlotter_shape_automated.set",v?"TRUE":"FALSE");}

 {SoPlotter::Shape v = (SoPlotter::Shape)aPlotter.shape.getValue();
  if(v==SoPlotter::XY) {
    aUI.setParameter("OnX_InputPlotter_shape.default","XY");
  } else if(v==SoPlotter::XYZ) {
    aUI.setParameter("OnX_InputPlotter_shape.default","XYZ");
  } else {
    std::ostream& out = aUI.session().cout();
    out << "shape " << int(v) << " not known."
        << std::endl;
  }}
}
//////////////////////////////////////////////////////////////////////////////
static void OnX_InputPlotter_updateAllTabStack(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
) 
//////////////////////////////////////////////////////////////////////////////
// Get all parameters from plotter and update the display with thoses value
// Only update the style and plotter tabstack from InputPlotter 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Axes :
  //FIXME : TicksStyle, TitleStyle, MagStyle.
  So_UI_LineStyle(aUI,*(aPlotter.getXAxis()->getLineStyle()),
                  "OnX_InputPlotter_xAxis_");
  So_UI_LineStyle(aUI,*(aPlotter.getYAxis()->getLineStyle()),
                  "OnX_InputPlotter_yAxis_");
  So_UI_LineStyle(aUI,*(aPlotter.getZAxis()->getLineStyle()),
                 "OnX_InputPlotter_zAxis_");

  So_UI_TextStyle(aUI,*(aPlotter.getXAxis()->getLabelsStyle()),
                  "OnX_InputPlotter_xAxis_");
  So_UI_TextStyle(aUI,*(aPlotter.getYAxis()->getLabelsStyle()),
                  "OnX_InputPlotter_yAxis_");
  So_UI_TextStyle(aUI,*(aPlotter.getZAxis()->getLabelsStyle()),
                  "OnX_InputPlotter_zAxis_");

  // Infos :
  aUI.setParameter("OnX_InputPlotter_infos_content.default",
                   aPlotter.getInfosStyle()->options.getValue().getString());
  So_UI_Style(aUI,*aPlotter.getInfosStyle(),"OnX_InputPlotter_infos_");

  // Title :
  So_UI_TextStyle(aUI,*aPlotter.getTitleStyle(),"OnX_InputPlotter_title_");

  // Grid :
  //FIXME : So_UI_LineStyle
  So_UI_StyleLine(aUI,*aPlotter.getGridStyle(),"OnX_InputPlotter_grid_");

  // InnerFrame :
  //FIXME : So_UI_LineStyle
  So_UI_StyleLine(aUI,*aPlotter.getInnerFrameStyle(),
                  "OnX_InputPlotter_innerFrame_");

  // Wall :
  //FIXME : updateAreaStyle
  So_UI_StyleArea(aUI,*aPlotter.getWallStyle(),
                  "OnX_InputPlotter_wall_");

  // Bins :
  So_UI_StyleData(aUI,*aPlotter.getBinsStyle(0),
                  "OnX_InputPlotter_bins_",true,true);

  // Points :
  So_UI_StyleData(aUI,*aPlotter.getPointsStyle(0),
                  "OnX_InputPlotter_points_",false,false);

  // Function :
  So_UI_StyleData(aUI,*aPlotter.getFunctionStyle(0),
                  "OnX_InputPlotter_function_",true,false);

  // Errors :
  So_UI_StyleErrors(aUI,*aPlotter.getErrorsStyle(0),
                    "OnX_InputPlotter_errors_");
    
}

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_InputPlotter_updateHatchEditor(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// Get all parameters from plotter and update the display with thoses value
// Only update the hatch editor 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();

  So_UI_StyleHatch(aUI,*soPlotter->getRightHatchStyle(0),
                   "OnX_InputPlotter_rightHatch_");

  So_UI_StyleHatch(aUI,*soPlotter->getLeftHatchStyle(0),
                   "OnX_InputPlotter_leftHatch_");
}

//////////////////////////////////////////////////////////////////////////////
void OnX_InputPlotter_update(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// Get all parameters from plotter and update the display with thoses value
// Update all the InputPlotter (no update for InputPlotterHatch) 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();

  OnX_InputPlotter_updateComponentPanel(aUI,*soPlotterRegion,*soPlotter);
  OnX_InputPlotter_updateAllTabStack(aUI,*soPlotter);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

} //extern "C"

//////////////////////////////////////////////////////////////////////////////
static bool OnX_InputPlotter_setAllTabStack(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
) 
//////////////////////////////////////////////////////////////////////////////
//
// Get all parameters from display and update the plotter fields with thoses
// value
// Get all parameters from the InputPlotter (style and plotter tabStack)
// (no parameters from InputPlotterHatch) 
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Axes :
  UI_So_LineStyle(aUI,*aPlotter.getXAxis()->getLineStyle(),
                  "OnX_InputPlotter_xAxis_");
  UI_So_LineStyle(aUI,*aPlotter.getYAxis()->getLineStyle(),
                  "OnX_InputPlotter_yAxis_");
  UI_So_LineStyle(aUI,*aPlotter.getZAxis()->getLineStyle(),
                 "OnX_InputPlotter_zAxis_");

  UI_So_TextStyle(aUI,*aPlotter.getXAxis()->getLabelsStyle(),
                  "OnX_InputPlotter_xAxis_");
  UI_So_TextStyle(aUI,*aPlotter.getYAxis()->getLabelsStyle(),
                  "OnX_InputPlotter_yAxis_");
  UI_So_TextStyle(aUI,*aPlotter.getZAxis()->getLabelsStyle(),
                  "OnX_InputPlotter_zAxis_");


  bool changed;

  // Infos :
 {std::string v;
  if(!ui_parameterValue(aUI,"OnX_InputPlotter_infos_content.value",v,changed)) return false;
  if(changed) aPlotter.getInfosStyle()->options.setValue(v.c_str());}

  UI_So_Style(aUI,*aPlotter.getInfosStyle(),"OnX_InputPlotter_infos_");

  // Title :
  UI_So_TextStyle(aUI,*aPlotter.getTitleStyle(),"OnX_InputPlotter_title_");

  // Grid :
  //FIXME : So_UI_LineStyle
  UI_So_StyleLine(aUI,*aPlotter.getGridStyle(),"OnX_InputPlotter_grid_");

  // InnerFrame :
  //FIXME : So_UI_LineStyle
  UI_So_StyleLine(aUI,*aPlotter.getInnerFrameStyle(),
                  "OnX_InputPlotter_innerFrame_");

  // Wall :
  //FIXME : updateAreaStyle
  UI_So_StyleArea(aUI,*aPlotter.getWallStyle(),
                  "OnX_InputPlotter_wall_");

  // Bins :
  UI_So_StyleBins(aUI,aPlotter,"OnX_InputPlotter_bins_");

  // Points :
  UI_So_StylePoints(aUI,aPlotter,"OnX_InputPlotter_points_");

  // Function :
  UI_So_StyleFunction(aUI,aPlotter,"OnX_InputPlotter_function_");

  // Errors :
  UI_So_StyleErrors(aUI,aPlotter,"OnX_InputPlotter_errors_");

  return false;
}
//////////////////////////////////////////////////////////////////////////////
static void OnX_InputPlotter_setHatchTabStack(
 Slash::UI::IUI& aUI
,SoPlotter& aPlotter
) 
//////////////////////////////////////////////////////////////////////////////
//
// Get all parameters from display and update the plotter fields with thoses
// value
// Get all parameters from the InputPlotterHatch 
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  UI_So_StyleRightHatch(aUI,aPlotter,"OnX_InputPlotter_rightHatch_");
  UI_So_StyleLeftHatch(aUI,aPlotter,"OnX_InputPlotter_leftHatch_");
}
//////////////////////////////////////////////////////////////////////////////
static bool OnX_InputPlotter_setComponentPanel(
 Slash::UI::IUI& aUI
,SoPlotterRegion& aPlotterRegion
,SoPlotter& aPlotter
) 
//////////////////////////////////////////////////////////////////////////////
//
// Get all parameters from display and update the plotter fields with thoses
// value
// Get all parameters from the InputPlotter component tabStack 
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_wallStyle.set",v)) return false;
  aPlotter.getWallStyle()->visible.setValue(v?TRUE:FALSE);}
  
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_gridStyle.set",v)) return false;
  aPlotter.getGridStyle()->visible.setValue(v?TRUE:FALSE);}
  
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_innerFrameStyle.set",v)) return false;
  aPlotter.getInnerFrameStyle()->visible.setValue(v?TRUE:FALSE);}

 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_infosVisible.set",v)) return false;
  aPlotterRegion.infosRegionVisible.setValue(v?TRUE:FALSE);}

 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_legendVisible.set",v)) return false;
  aPlotterRegion.legendRegionVisible.setValue(v?TRUE:FALSE);}
  
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_titleStyle.set",v)) return false;
  aPlotter.getTitleStyle()->visible.setValue(v?TRUE:FALSE);}
  
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_titleUp.set",v)) return false;
  aPlotter.titleUp.setValue(v?TRUE:FALSE);}
  
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_superposeBins.set",v)) return false;
  aPlotter.superposeBins.setValue(v?TRUE:FALSE);}
  
 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_shape_automated.set",v)) return false;
  aPlotter.shapeAutomated.setValue(v?TRUE:FALSE);}
  
 {std::string v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_shape.value",v)) return false;
  if(v=="XY") {
    aPlotter.shape.setValue(SoPlotter::XY);
  } else if(v=="XYZ") {
    aPlotter.shape.setValue(SoPlotter::XYZ);
  } else {
    std::ostream& out = aUI.session().cout();
    out << "unknown shape " << inlib::sout(v) << "."
        << std::endl;
  }}

 {bool v;
  if(!Slash::parameter(aUI,"OnX_InputPlotter_errors.set",v)) return false;
  aPlotter.getErrorsStyle(0)->visible.setValue(v?TRUE:FALSE);}

  return true;
}

extern "C" {

//////////////////////////////////////////////////////////////////////////////
void OnX_InputPlotter_ok(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// validate plotter values
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  if(!soRegion->isOfType(SoPlotterRegion::getClassTypeId())) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();

  OnX_InputPlotter_setComponentPanel(aUI,*soPlotterRegion,*soPlotter);
  OnX_InputPlotter_setAllTabStack(aUI,*soPlotter);
}
//////////////////////////////////////////////////////////////////////////////
void OnX_InputPlotter_HatchEditor_ok(
 Slash::UI::IUI& aUI
,const std::vector<std::string>& aArgs
) 
//////////////////////////////////////////////////////////////////////////////
// validate hatchEditor values
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!inlib::check_args(aArgs,1,aUI.session().cout())) return;
  SoRegion* soRegion = Slash::find_SoRegion(aUI,aArgs[0]);
  if(!soRegion) return;
  SoPlotterRegion* soPlotterRegion = (SoPlotterRegion*)soRegion;
  SoPlotter* soPlotter = soPlotterRegion->getPlotter();
  OnX_InputPlotter_setHatchTabStack(aUI,*soPlotter);
}

} // extern "C"
