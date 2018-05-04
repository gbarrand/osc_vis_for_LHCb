#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int argc, char* argv[] )
{
  // Creating the analysis factory
  std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );

  //G.Barrand : the file is used by anaphe_b5. 
  //            The below can't be xml since anaphe_b5 creates a histo.xml
  std::string storeType = "hbook";
  if(argc>=3) {
    std::string arg1 = argv[1];
    std::string arg2 = argv[2];
    if(arg1=="-storeType") storeType = arg2;
  }

  bool readOnly = false;
  bool createNew = true;
  AIDA::ITree* tree = 
    tf->create("histo."+storeType, storeType, readOnly, createNew );
  if(!tree) {
    std::cout << "Can't create the tree histo." << storeType << "."
              << std::endl;
    return 1;    
  }

  // Creating a histogram factory, whose histograms will be handled by the tree
  std::auto_ptr<AIDA::IHistogramFactory> hf( af->createHistogramFactory( *tree ) );

  // Creating a histogram
  AIDA::IHistogram1D& h1 = *( hf->createHistogram1D( "1", "Example histogram 1.", 50, 0, 50 ) );

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h1.fill( 25 * static_cast<double>( std::rand() ) / RAND_MAX );
  }

  // Creating a histogram
  AIDA::IHistogram1D& h2 = *( hf->createHistogram1D( "2", "Example histogram 2.", 50, 0, 50 ) );

  // Filling the histogram with random data
  std::srand( 0 );
  for ( int i = 0; i < 1000; ++i ) {
    h2.fill( 25 + 25 * static_cast<double>( std::rand() ) / RAND_MAX );
  }


  // Creating the plotter factory
  AIDA::IPlotterFactory* pf = af->createPlotterFactory();
  if(pf) {

    // Creating a plotter
    AIDA::IPlotter* plotter = pf->create();
    if(plotter) {
      plotter->show();

      // Plotting the first histogram
      plotter->currentRegion().plot( h1, "annotation" );
      plotter->refresh();
      plotter->interact();
      //std::cout << "Press <ENTER> to continue" << std::endl;
      //std::cin.get();

      // Plotting the secnd histogram
      plotter->currentRegion().clear();
      plotter->currentRegion().plot( h2, "annotation" );
      plotter->refresh();
      plotter->interact();
      //std::cout << "Press <ENTER> to continue" << std::endl;
      //std::cin.get();

      delete plotter;
    }

    delete pf;
  }

  // Flushing the histograms into the file
  tree->commit();

  // Explicitly closing the tree
  tree->close();

  return 0;
}
