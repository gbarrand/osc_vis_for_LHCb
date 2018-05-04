#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int, char** )
{
  // Creating the analysis factory
  std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );
  if(!af.get()) return EXIT_FAILURE;

  // Creating the tree factory
  std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );

  // Creating a tree in memory
  std::auto_ptr<AIDA::ITree> tree( tf->create() );

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr< AIDA::IHistogramFactory > hf( af->createHistogramFactory( *tree ) );

  // Creating a histogram
  AIDA::IHistogram1D* h1p = hf->createHistogram1D( "histo", "Example histogram.", 50, 0, 50 );
  // Since we do not own the pointer of the histogram (the tree does)
  // it is better to assign it to a reference
  AIDA::IHistogram1D& h1 = *h1p;

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h1.fill( 50 * static_cast<double>( std::rand() ) / RAND_MAX );
  }

  // Creating the plotter factory
  std::auto_ptr< AIDA::IPlotterFactory > pf( af->createPlotterFactory() );
  if(pf.get()) {
    // Creating a plotter
    std::auto_ptr< AIDA::IPlotter > plotter( pf->create() );
    if(plotter.get()) {
      plotter->show();
      // Plotting the histogram
      plotter->currentRegion().plot( h1, "annotation" );
      plotter->refresh();
      // Play with the plotter :
      plotter->interact();
    }
  }

  // Wait for the keyboard return to avoid destroying the plotter window too quickly.
  //std::cout << "Press <ENTER> to exit" << std::endl;
  //std::cin.get();

  return 0;
}
