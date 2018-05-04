/*
 *  This example permits to dynamic load an AIDA::IPlotterFactory. 
 *  The default DLL is the OnXLabAnalysisFactory, but the name
 *  of another DLL of another implementation can be given through
 *  arguments too.
 */

#include <AIDA/IPlotterFactory.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>

// The Loader class is the one of AIDA-3.3/test/src/cpp.
#include "lib/Loader.h"

#include <string>
#include <iostream>

#include <stdlib.h>

int main(int aArgc,char** aArgv){
  std::string dll = "OnXLabLoadAIDA";
  if(aArgc==2) {
    dll = std::string(aArgv[1]); 
    // Disconnect other arguments. 
    // Else the dll is understood as a .onx file by OnX::Main.
    aArgc = 1;
  }

  // WARNING : the loader object must be deleted last.
  AIDA::Loader* loader = new AIDA::Loader(dll);
  if(!loader->isOpened()) {
    std::cout << "load_plotter :" 
              << " dll " << dll << " not found."
              << std::endl;
    delete loader;
    return EXIT_FAILURE;
  }

  // Find the plotter factory factory :
  typedef AIDA::IPlotterFactory*(*CreatePlotterFactoryFunction)(int,char**);
  CreatePlotterFactoryFunction func = 
   (CreatePlotterFactoryFunction)loader->find(dll+"_createPlotterFactory");
  if(!func) {
    std::cout << "load_plotter :" 
              << " AIDA_createPlotterFactory not found in dll " 
              << dll << " not found."
              << std::endl;
    delete loader;
    return EXIT_FAILURE;
  }

  // For OSC/OnXLab, in the case of a direct dynamic loading,
  // the plotter factory is in fact a manager and must be 
  // deleted last.
  AIDA::IPlotterFactory* plotterManager = (*func)(aArgc,aArgv);
  if(!plotterManager) {
    std::cout << "load_plotter :" 
              << " can't create a plotter factory."
              << std::endl;
    delete loader;
    return EXIT_FAILURE;
  }

  AIDA::IPlotter* plotter = plotterManager->create();
  if(!plotter) {
    std::cout << "load_plotter :" 
              << " can't create a plotter."
              << std::endl;
    delete plotterManager;
    delete loader;
    return EXIT_FAILURE;
  }

  std::cout << "load_plotter :" 
            << " an empty plotter should be displayed."
            << std::endl;

  plotter->show();
  plotter->interact();

  delete plotter;

  delete plotterManager; //Must be deleted after all plotters.
  delete loader; //Deleted last

  return EXIT_SUCCESS;
}

