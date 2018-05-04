#ifndef OnXLab_Styles_h
#define OnXLab_Styles_h

#include <AIDA/IConstants.h> //For AIDA_VERSION

#include <AIDA/IBaseStyle.h>
#include <AIDA/IBrushStyle.h>
#include <AIDA/IFillStyle.h>
#include <AIDA/ILineStyle.h>
#include <AIDA/IMarkerStyle.h>
#include <AIDA/ITextStyle.h>
#include <AIDA/ITitleStyle.h>
#include <AIDA/IInfoStyle.h>
#include <AIDA/IDataStyle.h>
#include <AIDA/IAxisStyle.h>
#include <AIDA/IPlotterStyle.h>
#include <AIDA/IPlotterLayout.h>
#include <AIDA/IInfo.h>
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
#include <AIDA/IBoxStyle.h>
#include <AIDA/IBorderStyle.h>
#include <AIDA/ILegendBoxStyle.h>
#include <AIDA/IStatisticsBoxStyle.h>
#include <AIDA/IGridStyle.h>
#endif

class SoPage;
class SoRegion;

namespace OnXLab {

class ISetParameter {
public:
  virtual ~ISetParameter() {}
  virtual bool setParameter(const std::string&,const std::string&) = 0;
  virtual bool parameterValue(const std::string&,std::string&) = 0;
};

class RegionSetParameter : public virtual ISetParameter {
public: //ISetParameter
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&);
public:
  RegionSetParameter(SoRegion*);
  RegionSetParameter(const RegionSetParameter&);
  RegionSetParameter& operator=(const RegionSetParameter&);
  virtual ~RegionSetParameter();
private:
  SoRegion* fSoRegion;
};

class PageSetParameter : public virtual ISetParameter {
public: //ISetParameter
  virtual bool setParameter(const std::string&,const std::string&);
  virtual bool parameterValue(const std::string&,std::string&);
public:
  PageSetParameter(SoPage*);
  PageSetParameter(const PageSetParameter&);
  PageSetParameter& operator=(const PageSetParameter&);
  virtual ~PageSetParameter();
public:
  void setSoPage(SoPage*);
private:
  SoPage* fSoPage;
};

// Basic styles (not over other styles) :

//AIDA::IFillStyle : AIDA::IBrushStyle : AIDA::IBaseStyle.

class FillStyle : public virtual AIDA::IFillStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IBrushStyle
  virtual std::vector<std::string>  availableColors() const;
  virtual std::string color() const;
  virtual double opacity() const;
  virtual bool setColor(const std::string & newColor);
  virtual bool setOpacity(double newOpacity);
public: //AIDA::IFillStyle
  virtual std::vector<std::string>  availablePatterns() const;
  virtual std::string pattern() const;
  virtual bool setPattern(const std::string & pattern);
public:
  FillStyle(ISetParameter&,const std::string&);
  virtual ~FillStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
};

//AIDA::ILineStyle : AIDA::IBrushStyle : AIDA::IBaseStyle.

class LineStyle : public virtual AIDA::ILineStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IBrushStyle
  virtual std::vector<std::string>  availableColors() const;
  virtual std::string color() const;
  virtual double opacity() const;
  virtual bool setColor(const std::string & newColor);
  virtual bool setOpacity(double newOpacity);
public: //AIDA::ILineStyle
  virtual std::vector<std::string>  availableLineTypes() const;
  virtual std::string lineType() const;
  virtual int thickness() const;
  virtual bool setLineType(const std::string & newLineType);
  virtual bool setThickness(int newThickness);
public:
  LineStyle(ISetParameter&,const std::string&);
  virtual ~LineStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
};

//AIDA::IMarkerStyle : AIDA::IBrushStyle : AIDA::IBaseStyle.

class MarkerStyle : public virtual AIDA::IMarkerStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IBrushStyle
  virtual std::vector<std::string>  availableColors() const;
  virtual std::string color() const;
  virtual double opacity() const;
  virtual bool setColor(const std::string & newColor);
  virtual bool setOpacity(double newOpacity);
public: //AIDA::IMarkerStyle
  virtual std::vector<std::string>  availableShapes() const;
  virtual std::string shape() const;
  virtual bool setShape(const std::string & shape);
  virtual bool setSize(int size); //AIDA-v3r3p0
  virtual int size() const; //AIDA-v3r3p0
public:
  MarkerStyle(ISetParameter&,const std::string&);
  virtual ~MarkerStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
};

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
//AIDA::IBorderStyle : AIDA:ILineStyle : : AIDA::IBrushStyle : AIDA::IBaseStyle

class BorderStyle : public LineStyle, public virtual AIDA::IBorderStyle {
public: //AIDA::IBorderStyle
  virtual std::vector<std::string> availableBorderTypes() const;
  virtual bool setBorderType(const std::string & newBorderType);
  virtual std::string borderType() const;
public:
  BorderStyle(ISetParameter&,const std::string&);
  virtual ~BorderStyle();
//private:
  //ISetParameter& fSetParameter;
  //std::string fField;
};

class GridStyle : public LineStyle, public virtual AIDA::IGridStyle {
public: //AIDA::IGridStyle
  virtual bool setCellSize(double);
  virtual double cellSize() const;
  virtual bool setUnits(int);
  virtual int units();
public:
  GridStyle(ISetParameter&,const std::string&);
  virtual ~GridStyle();
//private:
  //ISetParameter& fSetParameter;
  //std::string fField;
};

//AIDA::IBoxStyle : AIDA::IBaseStyle.

class BoxStyle : public virtual AIDA::IBoxStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual AIDA::IBaseStyle* child(const std::string & name) const;
  virtual std::vector<AIDA::IBaseStyle*> children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void* cast(const std::string & className) const;
public: //AIDA::IBoxStyle
  virtual AIDA::IFillStyle & backgroundStyle();
  virtual bool setBackgroundStyle(const AIDA::IFillStyle & fillStyle);
  virtual AIDA::IFillStyle & foregroundStyle();
  virtual bool setForegroundStyle(const AIDA::IFillStyle & fillStyle);
  virtual AIDA::IBorderStyle & borderStyle();
  virtual bool setBorderStyle(const AIDA::IBorderStyle & borderStyle);
  virtual bool setX(double x);
  virtual bool setY(double y);
  virtual double x() const;
  virtual double y() const;
  virtual bool setHeight(double height);
  virtual bool setWidth(double width);
  virtual double height() const;
  virtual double width() const;
  virtual bool setPlacement(int originDefinition);
  virtual int placement();
  virtual bool setUnits(int unitsDefinition);
  virtual int units();
public:
  BoxStyle(ISetParameter&,const std::string&);
  virtual ~BoxStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  FillStyle fBackgroundStyle;
  FillStyle fForegroundStyle;
  BorderStyle fBorderStyle;
};
#endif

//AIDA::IBoxStyle : AIDA::IBrushStyle : AIDA::IBaseStyle.

class TextStyle : public virtual AIDA::ITextStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IBrushStyle
  virtual std::vector<std::string>  availableColors() const;
  virtual std::string color() const;
  virtual double opacity() const;
  virtual bool setColor(const std::string & newColor);
  virtual bool setOpacity(double newOpacity);
public: //AIDA::ITextStyle
  virtual std::vector<std::string>  availableFonts() const;
  virtual double fontSize() const;
  virtual bool setFontSize(double size);
  virtual std::string font() const;
  virtual bool setFont(const std::string & font);
  virtual bool isBold() const;
  virtual bool isItalic() const;
  virtual bool isUnderlined() const;
  virtual bool setBold(bool bold = true);
  virtual bool setItalic(bool italic = true);
  virtual bool setUnderlined(bool underlined = true);
public:
  TextStyle(ISetParameter&,const std::string&);
  virtual ~TextStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
};

// Plotter scene part styles (over basic styles) :

//AIDA::IDataStyle : AIDA::IBaseStyle.

class DataStyle : public virtual AIDA::IDataStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IDataStyle
  virtual AIDA::ILineStyle & lineStyle();
  virtual AIDA::IMarkerStyle & markerStyle();
  virtual AIDA::IFillStyle& fillStyle();
  virtual AIDA::ILineStyle& errorBarStyle();
  virtual AIDA::ILineStyle& outlineStyle();
  virtual bool setLineStyle(const AIDA::ILineStyle & lineStyle);
  virtual bool setMarkerStyle(const AIDA::IMarkerStyle & markerStyle);
  virtual bool setFillStyle(const AIDA::IFillStyle & fillStyle);
  virtual bool setErrorBarStyle(const AIDA::ILineStyle&);
  virtual bool setOutlineStyle(const AIDA::ILineStyle&);
  virtual bool setModel(const std::string&);
  virtual std::string model() const;
  virtual void showInStatisticsBox(bool);
  virtual bool isShownInStatisticsBox() const;
  virtual void showInLegendBox(bool);
  virtual bool isShownInLegendBox() const;
public:
  DataStyle(ISetParameter&,const std::string&);
  virtual ~DataStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  LineStyle fLineStyle;
  MarkerStyle fMarkerStyle;
  FillStyle fFillStyle;
  LineStyle fErrorBarStyle;
  LineStyle fOutlineStyle;
};

// AIDA::IInfoStyle : AIDA::IBaseStyle

class InfoStyle : public virtual AIDA::IInfoStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IInfoStyle
  virtual AIDA::ITextStyle & textStyle();
  virtual bool setTextStyle(const AIDA::ITextStyle & textStyle);
public:
  InfoStyle(ISetParameter&,const std::string&);
  virtual ~InfoStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  TextStyle fTextStyle;
};

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
// AIDA::ILegendBoxStyle : AIDA::IBaseStyle

class LegendBoxStyle : public virtual AIDA::ILegendBoxStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::ILegendBoxStyle
  virtual AIDA::ITextStyle & textStyle();
  virtual bool setTextStyle(const AIDA::ITextStyle&);
  virtual bool setBoxStyle(const AIDA::IBoxStyle&);
  virtual AIDA::IBoxStyle& boxStyle();
public:
  LegendBoxStyle(ISetParameter&,const std::string&);
  virtual ~LegendBoxStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  TextStyle fTextStyle;
  BoxStyle fBoxStyle;
};

// AIDA::IStatisticsBoxStyle : AIDA::IBaseStyle

class StatisticsBoxStyle : public virtual AIDA::IStatisticsBoxStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IStatisticsBoxStyle
  virtual bool setVisibileStatistics(const std::string&);
  virtual std::string visibleStatistics() const;
  virtual AIDA::ITextStyle & textStyle();
  virtual bool setTextStyle(const AIDA::ITextStyle&);
  virtual bool setBoxStyle(const AIDA::IBoxStyle&);
  virtual AIDA::IBoxStyle& boxStyle();
public:
  StatisticsBoxStyle(ISetParameter&,const std::string&);
  virtual ~StatisticsBoxStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  TextStyle fTextStyle;
  BoxStyle fBoxStyle;
};
#endif

// AIDA::IAxisStyle : AIDA::IBaseStyle

class AxisStyle : public virtual AIDA::IAxisStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IAxisStyle
  virtual AIDA::ILineStyle& lineStyle();
  virtual AIDA::ITextStyle& tickLabelStyle();
  virtual AIDA::ITextStyle& labelStyle();
  virtual bool setlineStyle(const AIDA::ILineStyle& lineStyle);
  virtual bool setLineStyle(const AIDA::ILineStyle& lineStyle);
  virtual bool setTickLabelStyle(const AIDA::ITextStyle& tickLabelStyle);
  virtual bool setLabelStyle(const AIDA::ITextStyle& labelStyle);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual void setLabel(const std::string& label);
#else
  virtual bool setLabel(const std::string & label);
#endif
  virtual bool setScaling(const std::string & scaling);
  virtual std::string scaling() const;
  virtual std::string label() const;
public: 
  AxisStyle(ISetParameter&,const std::string&);
  virtual ~AxisStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  LineStyle fLineStyle;
  LineStyle fTicksStyle;
  TextStyle fLabelsStyle;
  TextStyle fTitleStyle;
};

//AIDA::ITitleStyle : AIDA::IBaseStyle

class TitleStyle : public virtual AIDA::ITitleStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::ITitleStyle
  virtual AIDA::ITextStyle& textStyle();
  virtual bool setTextStyle(const AIDA::ITextStyle& textStyle);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  virtual bool setBoxStyle(const AIDA::IBoxStyle & boxStyle);
  virtual AIDA::IBoxStyle & boxStyle();
#endif
public:
  TitleStyle(ISetParameter&,const std::string&);
  virtual ~TitleStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  TextStyle fTextStyle;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  BoxStyle fBoxStyle;
#endif
};

// Global plotter style :

// AIDA::IPlotterStyle : AIDA::IBaseStyle

class PlotterStyle : public virtual AIDA::IPlotterStyle {
public: //AIDA::IBaseStyle
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, const std::string & options = "");
  virtual std::string parameterValue(const std::string & parameter) const;
  virtual std::vector<std::string>  availableParameters() const;
  virtual std::vector<std::string>  availableParameterOptions(const std::string & paramName) const;
  //AIDA-v3r3p0
  virtual std::string name() const;
  virtual std::string type() const;
  virtual IBaseStyle * child(const std::string & name) const;
  virtual std::vector<IBaseStyle *>  children() const;
  virtual bool setVisible(bool visible);
  virtual bool isVisible() const;
  virtual void * cast(const std::string & className) const;
public: //AIDA::IPlotterStyle
  virtual AIDA::IDataStyle& dataStyle();
  virtual AIDA::IAxisStyle& xAxisStyle();
  virtual AIDA::IAxisStyle& yAxisStyle();
  virtual AIDA::IAxisStyle& zAxisStyle();
  virtual AIDA::ITitleStyle& titleStyle();
  virtual AIDA::IInfoStyle& infoStyle();
  virtual bool setDataStyle(const AIDA::IDataStyle& dataStyle);
  virtual bool setAxisStyleX(const AIDA::IAxisStyle& xAxisStyle);
  virtual bool setAxisStyleY(const AIDA::IAxisStyle& yAxisStyle);
  virtual bool setAxisStyleZ(const AIDA::IAxisStyle& zAxisStyle);
  virtual bool setTitleStyle(const AIDA::ITitleStyle& textStyle);
  virtual bool setInfoStyle(const AIDA::IInfoStyle& infoStyle);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  virtual AIDA::ILegendBoxStyle& legendBoxStyle();
  virtual AIDA::IStatisticsBoxStyle& statisticsBoxStyle();
  virtual AIDA::IGridStyle& gridStyle();
  virtual AIDA::IBoxStyle& regionBoxStyle();
  virtual AIDA::IBoxStyle& dataBoxStyle();
  virtual bool setLegendBoxStyle(const AIDA::ILegendBoxStyle&);
  virtual bool setStatisticsBoxStyle(const AIDA::IStatisticsBoxStyle&);
  virtual bool setGridStyle(const AIDA::IGridStyle&);
  virtual bool setRegionBoxStyle(const AIDA::IBoxStyle&);
  virtual bool setDataBoxStyle(const AIDA::IBoxStyle&);
#endif
public:
  PlotterStyle(ISetParameter&,const std::string&);
  virtual ~PlotterStyle();
private:
  ISetParameter& fSetParameter;
  std::string fField;
  DataStyle fDataStyle;
  AxisStyle f_xAxisStyle;
  AxisStyle f_yAxisStyle;
  AxisStyle f_zAxisStyle;
  TitleStyle fTitleStyle;
  InfoStyle fInfoStyle;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  LegendBoxStyle fLegendBoxStyle;
  StatisticsBoxStyle fStatisticsBoxStyle;
  GridStyle fGridStyle;
  BoxStyle fRegionBoxStyle;
  BoxStyle fDataBoxStyle;
#endif
};

class PlotterLayout : public virtual AIDA::IPlotterLayout {
public: 
  virtual void reset();
  virtual bool setParameter(const std::string & paramName, double paramValue);
  virtual double parameterValue(const std::string & paramName);
  virtual std::vector<std::string>  availableParameters() const;
public:
  PlotterLayout(ISetParameter&,const std::string&);
  virtual ~PlotterLayout();
private:
  ISetParameter& fSetParameter;
  std::string fField;
};

class Info : public virtual AIDA::IInfo {
public: 
  virtual void clear();
  virtual void addText(const std::string & text);
  virtual void addLegend(const AIDA::IMarkerStyle & style, const std::string & description);
  virtual void addLegend(const AIDA::ILineStyle & style, const std::string & description);
  virtual void addLegend(const AIDA::IFillStyle & style, const std::string & description);
public:
  virtual ~Info();
};

}

#endif
