#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

#include "RandGauss.h"

#include <sstream>

int main( int, char** )
{

 {// Put everything in a block to check the destruction of everything at end.

  // Creating the analysis factory
  std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );
  if(!af.get()) return EXIT_FAILURE;

  // Creating the tree factory
  std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );

  // Creating a tree in memory
  std::auto_ptr<AIDA::ITree> tree( tf->create() );

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr< AIDA::IHistogramFactory > hf( af->createHistogramFactory( *tree ) );

  // Creating a function factory whose functions will be handled by the tree
  std::auto_ptr< AIDA::IFunctionFactory > ff( af->createFunctionFactory( *tree ) );

  // Creating a fit factory
  std::auto_ptr< AIDA::IFitFactory > fitf( af->createFitFactory() );
 
  // Creating a histogram
  AIDA::IHistogram1D& h1 = *( hf->createHistogram1D("MyHisto", "Gaussian Distribution", 100, 0, 100) );

  // Filling the histogram with random data following a gaussian distribution
  //DRand48Engine randomEngine;
  //RandGauss rgauss( randomEngine, 45, 10 );
  RandGauss rgauss(45,10);
  for ( int i = 0; i < 10000; ++i ) {
    h1.fill( rgauss.fire() );
  }

  std::cout << "Histogram :" << std::endl;
  std::cout << "  Title : " << h1.title() << std::endl;
  std::cout << "  Entries : " << h1.entries() << std::endl;
  std::cout << "  Mean : " << h1.mean() << std::endl;
  std::cout << "  Rms : " << h1.rms() << std::endl;

  // Creating the function which is going to be fitted with the histogram data
  AIDA::IFunction& gaussFun = *( ff->createFunctionByName("Gaussian","G") );

  // set function initial parameters
  gaussFun.setParameter("mean" , 50.);
  gaussFun.setParameter("sigma", 10.);
  gaussFun.setParameter("amplitude"  , 10.);

  // Creating the fitter
  std::auto_ptr<AIDA::IFitter> fitter( fitf->createFitter() );

  AIDA::IAnnotation& annotation = h1.annotation();

  // Performing the fit
  AIDA::IFitResult* fitResult = fitter->fit( h1, gaussFun );
  if(!fitResult) {
    std::cout << "Fit failed." << std::endl;
  } else {
    // Printing the fit results
    std::cout << "Fit result : chi2 / ndf : " 
              << fitResult->quality() << " / " << fitResult->ndf() 
              << std::endl;
    const std::vector<std::string>& parNames = 
      fitResult->fittedParameterNames();
    const std::vector<double>& par = fitResult->fittedParameters();
    const std::vector<double>& epar = fitResult->errors();

    for ( unsigned int i = 0; i < par.size(); ++i ) {
      std::cout << parNames[i] << " = " << par[i] << " +/- " << epar[i] << std::endl;
    }

    // Add in the annotation of the histogram the fit result
    for ( unsigned int i = 0; i < par.size(); ++i ) {
      std::ostringstream os;
      os << par[i] << " +/- " << epar[i];
      annotation.addItem( parNames[i], os.str() );
    }

    delete fitResult;

  }

  // Remove some existing annotation items
  annotation.removeItem( "Overflow" );
  annotation.removeItem( "Underflow" );
  annotation.removeItem( "Extra Entries" );

  // Creating the plotter factory
  std::auto_ptr< AIDA::IPlotterFactory > pf( af->createPlotterFactory() );
  if(pf.get()) {
    // Plotting the function with its annotation and then the histogram
    // Creating a plotter
    std::auto_ptr< AIDA::IPlotter > plotter( pf->create() );
    if(plotter.get()) {
      plotter->show();
      plotter->currentRegion().plot( h1, "annotation" );
      plotter->currentRegion().plot(gaussFun,"[0,100] overlay");
      plotter->refresh();
      plotter->interact();
    }
  }

  // Listing the tree contents
  std::cout << "Tree contents :" << std::endl;
  tree->ls();

  // Wait for the keyboard return to avoid destroying the plotter window too quickly.
  //std::cout << "Press <ENTER> to exit" << std::endl;
  //std::cin.get();

 } // End of global block.

  return 0;
}
