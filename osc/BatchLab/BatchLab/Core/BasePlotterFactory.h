#ifndef BatchLab_PlotterFactory_h
#define BatchLab_PlotterFactory_h

// Inheritance :
#include <AIDA/IPlotterFactory.h>

#include <Slash/Core/ILibrary.h>
#include <Slash/UI/IPlotterManager.h>
#include <Slash/UI/IPlotter.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/Manager.h>

#include <inlib/cast>
#include <Lib/Debug.h>

#include <AIDA/IPlotter.h>

namespace BatchLab {

class BasePlotterFactory : public virtual AIDA::IPlotterFactory {
public: //AIDA/IPlotterFactory
  //virtual AIDA::IPlotter* create(const std::string&);

  virtual AIDA::IMarkerStyle* createMarkerStyle() {return 0;}
  virtual AIDA::ITextStyle* createTextStyle() {return 0;}
  virtual AIDA::ILineStyle* createLineStyle() {return 0;}
  virtual AIDA::IFillStyle* createFillStyle() {return 0;}
  virtual AIDA::IDataStyle* createDataStyle() {return 0;}
  virtual AIDA::IAxisStyle* createAxisStyle() {return 0;}
  virtual AIDA::ITitleStyle* createTitleStyle() {return 0;}
  virtual AIDA::IPlotterStyle* createPlotterStyle() {return 0;}

public:
  BasePlotterFactory(Slash::Core::ISession& aSession,
                     const std::string& aImplementation,
                     const std::string& aLibrary,int aArgc,char** aArgv)
  :fSession(aSession)
  ,fImplementation(aImplementation)
  ,fLibrary(aLibrary){
    Lib::Debug::increment("BatchLab::BasePlotterFactory");
    for(int index=0;index<aArgc;index++) fArgs.push_back(aArgv[index]);
  }
  virtual ~BasePlotterFactory(){
    Lib::Debug::decrement("BatchLab::BasePlotterFactory");
  }

protected:
  AIDA::IPlotter* loadPlotter(const std::string& aLibrary,
                              const std::string& aPath,
                              const std::string& aPackage,
                              const std::string& aPlotterManager,
                              const std::string& aName){
    Slash::Core::ILibraryManager* libraryManager = 
      Slash::libraryManager(fSession);
    if(!libraryManager) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::BasePlotterFactory::load :"
          << " LibraryManager not found." << std::endl;
      return 0;
    }
    Slash::Core::ILibrary* dll = 
      libraryManager->addLibrary(aLibrary,aPath,aPackage);
    if(!dll) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::BasePlotterFactory::load :"
          << " unable to add a DLL" << std::endl;
      return 0;
    }
    Slash::UI::IPlotterManager* plotterManager = 
      Slash_findManager(fSession,aPlotterManager,Slash::UI::IPlotterManager);
    if(!plotterManager) {
      std::ostream& out = fSession.cout();
      out << "BatchLab::BasePlotterFactory::load :"
          << " unable to find " << aPlotterManager << " plotter manager." 
          << std::endl;
      return 0;
    }
    plotterManager->setArguments(fArgs);
    Slash::UI::IPlotter* plotter = plotterManager->create(aName);
    if(!plotter) return 0;
    return INLIB_CAST(*plotter,AIDA::IPlotter);
  }
protected:
  Slash::Core::ISession& fSession;
  std::string fImplementation;
  std::string fLibrary;  
  std::vector<std::string> fArgs;  
};

}

#endif
