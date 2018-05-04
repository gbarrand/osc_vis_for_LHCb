#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

int main( int argc, char* argv[] )
{
  //G.Barrand : the file is created by anaphe_b4.
  std::string storeType = "hbook";
  if(argc>=3) {
    std::string arg1 = argv[1];
    std::string arg2 = argv[2];
    if(arg1=="-storeType") storeType = arg2;
  }

  // Creating the analysis factory
  std::auto_ptr<AIDA::IAnalysisFactory> af( AIDA_createAnalysisFactory() );

  // Creating the tree factory
  std::auto_ptr<AIDA::ITreeFactory> tf( af->createTreeFactory() );

  // Creating a memory-mapped tree
  std::auto_ptr<AIDA::ITree> tree( tf->create() );

  // Creating a histogram factory attached to the tree
  std::auto_ptr<AIDA::IHistogramFactory> hf( af->createHistogramFactory( *tree ) );

  bool readOnly = true;
  bool createNew = false;
  AIDA::ITree* treeHBook = 
    tf->create("histo."+storeType, storeType, readOnly, createNew );
  if(!treeHBook) {
    std::cout << "Can't create the tree histo." << storeType << "."
              << std::endl;
    std::cout << "You have to run anaphe_b4 first."
              << std::endl;
    return 1;    
  }

  // Creating a tree mapped to an XML file
  readOnly = false;
  createNew = true;
  AIDA::ITree* treeXML = tf->create("histo.xml", "xml", readOnly, createNew );
  if(!treeXML) {
    std::cout << "Can't create the tree histo.xml."<< std::endl;
    return 1;    
  }

  // Mounting the hbook and xml trees under the master memory tree.
  //GB FIXME : tree->mkdir( "hbook" );
  //GB FIXME : tree->mkdir( "xml" );
  tree->mount( "/hbook", *treeHBook, "/" );
  tree->mount( "/xml", *treeXML, "/" );

  // Fetching the histograms from hbook
  AIDA::IManagedObject* mo = tree->find( "/hbook/1" );
  if(!mo) {
    std::cout << "Object /hbook/1 not found." << std::endl;
    return 1;
  }
  AIDA::IHistogram1D& h1 = dynamic_cast<AIDA::IHistogram1D&>( *mo );

  mo = tree->find( "/hbook/2" );
  if(!mo) {
    std::cout << "Object /hbook/2 not found." << std::endl;
    return 1;
  }
  AIDA::IHistogram1D& h2 = dynamic_cast<AIDA::IHistogram1D&>( *mo );

  std::cout << "Histogram 1 : " << std::endl;
  std::cout << "   Entries : " << h1.entries() << std::endl;
  std::cout << "   Mean : " << h1.mean() << std::endl;
  std::cout << "   Rms : " << h1.rms() << std::endl;

  std::cout << "Histogram 2 : " << std::endl;
  std::cout << "   Entries : " << h2.entries() << std::endl;
  std::cout << "   Mean : " << h2.mean() << std::endl;
  std::cout << "   Rms : " << h2.rms() << std::endl;

  // Adding the two histograms and putting the result in the xml file
  tree->cd( "/xml" );
  AIDA::IHistogram1D& h3 = *( hf->add( "histo_3", h1, h2 ) );
  h3.setTitle( "Sum histogram" );

  std::cout << "Histogram 3 ( 1+2 ) : " << std::endl;
  std::cout << "   Entries : " << h3.entries() << std::endl;
  std::cout << "   Mean : " << h3.mean() << std::endl;
  std::cout << "   Rms : " << h3.rms() << std::endl;

  // Plotting the three histograms
  // Creating the plotter factory
  AIDA::IPlotterFactory* pf = af->createPlotterFactory();
  if(pf) {

    // Creating a plotter
    AIDA::IPlotter* pl = pf->create();
    if(pl) {
      pl->show();
      AIDA::IPlotterRegion& region = pl->currentRegion();
  
      std::cout << "Press <ENTER> to plot the first histogram" << std::endl;
      std::cin.get();
      region.plot( h1, "annotation" );
      pl->refresh();
      pl->interact();
    
      //std::cout << "Press <ENTER> to plot the second histogram" << std::endl;
      //std::cin.get();
      region.clear();
      region.plot( h2, "annotation" );
      pl->refresh();
      pl->interact();

      //std::cout << "Press <ENTER> to plot the sum histogram" << std::endl;
      //std::cin.get();
      region.clear();
      region.plot( h3, "annotation" );
      pl->refresh();
      pl->interact();

      delete pl;
    }

    delete pf;
  }
  //std::cout << "Press <ENTER> to save the results in XML and exit" << std::endl;
  //std::cin.get();
  std::cout << "Save the results in XML and exit" << std::endl;

  // Copying the first histogram from HBOOK to XML
  tree->cp( "/hbook/1", "." );
  tree->mkdir( "dir1" );
  tree->cp( "/hbook/2", "/xml/dir1" );

  std::cout << "Full tree contents: " << std::endl;
  tree->ls( "/", true );

  // Flushing the results into the XML file
  tree->commit();

  // Explicitly closing the tree
  tree->close();

  return 0;
}
