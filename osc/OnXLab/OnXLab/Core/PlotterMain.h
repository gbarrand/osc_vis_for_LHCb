#ifndef OnXLab_PlotterMain_h
#define OnXLab_PlotterMain_h

// Inheritance :
#include <AIDA/IPlotterFactory.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

#include <OnXLab/Core/Main.h>

namespace OnXLab {

class PlotterMain : public virtual AIDA::IPlotterFactory {
public: //AIDA::IPlotterFactory
  virtual AIDA::IPlotter* create(const std::string& aName = ""
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    ,const std::string& = "" //aOptions
#endif
  ) {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->create(aName);
  }
  virtual AIDA::IMarkerStyle* createMarkerStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createMarkerStyle();
  }
  virtual AIDA::ITextStyle* createTextStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createTextStyle();
  }
  virtual AIDA::ILineStyle* createLineStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createLineStyle();
  }
  virtual AIDA::IFillStyle* createFillStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createFillStyle();
  }
  virtual AIDA::IDataStyle* createDataStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createDataStyle();
  }
  virtual AIDA::IAxisStyle* createAxisStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createAxisStyle();
  }
  virtual AIDA::ITitleStyle* createTitleStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createTitleStyle();
  }
  virtual AIDA::IPlotterStyle* createPlotterStyle() {
    if(!fPlotterFactory) return 0;
    return fPlotterFactory->createPlotterStyle();
  }
public:
  PlotterMain(int aArgc,char** aArgv):fAIDA(0),fPlotterFactory(0) {
    std::vector<std::string> args;
    for(int index=0;index<aArgc;index++) args.push_back(aArgv[index]);
    fAIDA = new OnXLab::Main(args);

    //if(!fAIDA->createUI()) { //Will load from ONXLABPLOTTER env variable.
    //  delete fAIDA;
    //  fAIDA = 0;
    //  return;
    //}

    fPlotterFactory = fAIDA->createPlotterFactory(aArgc,aArgv,"","");
    if(!fPlotterFactory) {
      delete fAIDA;
      fAIDA = 0;
      return;
    }
  }

  virtual ~PlotterMain() {delete fPlotterFactory;delete fAIDA;}
public:
  bool isValid() const {return (fPlotterFactory?true:false);}
private:
  Main* fAIDA;  
  AIDA::IPlotterFactory* fPlotterFactory;
};
}

#endif
