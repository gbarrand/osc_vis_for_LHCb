#ifndef OnXLab_Plotter_h
#define OnXLab_Plotter_h

// Inheritance :
#include <AIDA/IPlotterRegion.h>
#include <AIDA/IPlotter.h>

#include <Slash/UI/IPlotter.h>

#include <AIDA/IPlotterStyle.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

#include <OnXLab/Inventor/Styles.h>
#include <list>

namespace Slash {namespace Core {class IRelationManager;}}

namespace Slash {namespace UI {class IWidget;}}
namespace Slash {namespace UI {class IUI;}}
class SoPage;
class SoRegion;

namespace OnXLab {

class PlotterRegion 
:public virtual AIDA::IPlotterRegion
{
public: //AIDA/IPlotter
  virtual bool plot(const AIDA::IBaseHistogram&,const std::string& = "");
  virtual bool plot(const AIDA::IBaseHistogram&,
                    const AIDA::IPlotterStyle&,const std::string& = "");
  virtual bool plot(const AIDA::IFunction&,const std::string& = "");
  virtual bool plot(const AIDA::IFunction&,
                    const AIDA::IPlotterStyle&,const std::string& = "");
  virtual bool plot(const AIDA::IDataPointSet&,const std::string& = "");
  virtual bool plot(const AIDA::IDataPointSet&,
                    const AIDA::IPlotterStyle&,const std::string& = "");
  virtual bool remove(const AIDA::IBaseHistogram&);
  virtual bool remove(const AIDA::IFunction&);
  virtual bool remove(const AIDA::IDataPointSet&);
  virtual void clear();
  virtual bool setParameter(const std::string&,const std::string&);
  virtual std::string parameterValue(const std::string&) const;
  virtual std::vector<std::string> availableParameterOptions(const std::string&) const;  
  virtual std::vector<std::string> availableParameters() const;

  virtual AIDA::IPlotterStyle & style();
  virtual bool setStyle(const AIDA::IPlotterStyle&);
  virtual bool applyStyle(const AIDA::IPlotterStyle&);
  virtual void setTitle(const std::string&);
  virtual bool setXLimits(double,double);
  virtual bool setYLimits(double,double);
  virtual bool setZLimits(double,double);
  virtual AIDA::IPlotterLayout& layout();
  virtual bool setLayout(const AIDA::IPlotterLayout&);
  virtual AIDA::IInfo& info();
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  virtual bool plot(const AIDA::IPlottable&,const std::string& = "");
  virtual bool plot(const AIDA::IPlottable&,
                    const AIDA::IPlotterStyle&,
                    const std::string& = "");
  virtual bool remove(const AIDA::IPlottable&);
  virtual bool refresh();
  virtual std::string title() const;
  virtual double xLimitMin() const;
  virtual double xLimitMax() const;
  virtual double yLimitMin() const;
  virtual double yLimitMax() const;
  virtual double zLimitMin() const;
  virtual double zLimitMax() const;
  virtual void* cast(const std::string&) const;
#endif
public:
  PlotterRegion(std::ostream& a_out,Slash::Core::IRelationManager&,int,SoRegion*,Slash::UI::IUI&,Slash::UI::IWidget&);
  virtual ~PlotterRegion();
private:
  PlotterRegion(const PlotterRegion&);
  PlotterRegion& operator=(const PlotterRegion&);
public:
  SoRegion* soRegion() const;
private:
  std::ostream& f_out;
  Slash::Core::IRelationManager& fRelationManager;
  int fVerboseLevel;
  SoRegion* fSoRegion;
  Slash::UI::IUI& fUI; //for lock/unlock
  Slash::UI::IWidget& fWidget;
  RegionSetParameter fSetParameter;
  PlotterStyle fPlotterStyle;
  Info fInfo;
  PlotterLayout fLayout;
};

class Plotter 
:public virtual Slash::UI::IPlotter 
,public virtual AIDA::IPlotter
{
public: //Slash/UI/IPlotter
  virtual void* cast(const std::string&) const;
  virtual SoNode* nodeOfInterest(const std::string& what) const;
public: //AIDA/IPlotter
  virtual AIDA::IPlotterRegion* createRegion(double,double,double,double);
  virtual bool createRegions(int,int,int);
  virtual AIDA::IPlotterRegion& currentRegion() const;
  virtual int currentRegionNumber() const;
  virtual int numberOfRegions() const;
  virtual bool setCurrentRegionNumber(int);
  virtual AIDA::IPlotterRegion& next();
  virtual AIDA::IPlotterRegion* region(int) const;
  virtual void destroyRegions();
  virtual void clearRegions();
  virtual bool setParameter(const std::string&,const std::string&);
  virtual std::string parameterValue(const std::string&) const;
  virtual std::vector<std::string> availableParameterOptions(const std::string&) const;  
  virtual std::vector<std::string> availableParameters() const;
  virtual bool show();
  virtual bool hide();
  virtual bool refresh();
  virtual bool interact();
  virtual bool writeToFile(const std::string&,const std::string&);
  virtual void setTitle(const std::string&);
  virtual AIDA::ITitleStyle& titleStyle();
  virtual void setTitleStyle(const AIDA::ITitleStyle&);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  virtual std::string title() const;
  virtual AIDA::IPlotterStyle& style();
  virtual void setStyle(const AIDA::IPlotterStyle&);
#endif
public:
  Plotter(std::ostream& a_out,Slash::UI::IWidget&,Slash::UI::IUI&,int);
  virtual ~Plotter();
private:
  Plotter(const Plotter&);
  Plotter& operator=(const Plotter&);
public:
  bool isValid() const;
  //SoPage& soPage() const;
  //Slash::UI::IUI& ui() const;
  SoRegion* currentSoRegion() const;
private:
  AIDA::IPlotterRegion* plotterRegion(SoRegion*) const;
private:
  std::ostream& f_out;
  Slash::UI::IWidget& fWidget;
  Slash::UI::IUI& fUI;
  int fVerboseLevel;
  SoPage* fSoPage;
  std::list<PlotterRegion*> fRegions;
  PageSetParameter fSetParameter;
  TitleStyle fTitleStyle;
  Slash::Core::IRelationManager* fRelationManager;
  PlotterStyle fPlotterStyle;
};

}

#endif
