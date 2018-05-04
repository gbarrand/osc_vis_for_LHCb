#include <iostream>
#include <cstdlib>
#include <memory>

#include "AIDA/AIDA.h"

#include <sstream>

int main( int argc, char* argv[] )
{

 {// Put everything in a block to check the destruction of everything at end.

  //G.Barrand : the file is created by anaphe_b7.
  std::string storeType = "xml";
  if(argc>=3) {
    std::string arg1 = argv[1];
    std::string arg2 = argv[2];
    if(arg1=="-storeType") storeType = arg2;
  }

  // Creating the analysis factory
  std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );
  if(!af.get()) return EXIT_FAILURE;

  // Creating the tree factory
  std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );

  // Creating a tree in memory
  std::auto_ptr< AIDA::ITree > tree( tf->create() );

  // Creating a histogram factory, whose histograms 
  // will be handled by the tree
  std::auto_ptr< AIDA::IHistogramFactory > hf( af->createHistogramFactory( *tree ) );

  // Creating a function factory whose functions will be handled by the tree
  std::auto_ptr< AIDA::IFunctionFactory > ff( af->createFunctionFactory( *tree ) );

  // Creating a tuple
  std::auto_ptr< AIDA::ITupleFactory > tpf( af->createTupleFactory( *tree ) );

  // Creating a fit factory
  std::auto_ptr< AIDA::IFitFactory > fitf( af->createFitFactory() );
 
  bool readOnly = true;
  bool createNew = false;
  AIDA::ITree* treeHBook = 
    tf->create("tuple."+storeType,storeType,readOnly,createNew,"lrec=2048" );
  if(!treeHBook) {
    std::cout << "Can't create the tree tuple." 
              << storeType << "."
              << std::endl;
    std::cout << "You have to run anaphe_b7 first."
              << std::endl;
    return 1;    
  }

  // Creating a tree mapped to an XML file
  readOnly = false;
  createNew = true;
  AIDA::ITree* treeXML = tf->create("tuple_analysis.xml", "xml", readOnly, createNew );
  if(!treeXML) {
    std::cout << "Can't create the tree tuple_analysis.xml."<< std::endl;
    return 1;    
  }

  // Mounting the hbook and xml trees under the master memory tree.
  tree->mkdir( "hbook" );
  tree->mkdir( "xml" );
  tree->mount( "/hbook", *treeHBook, "/" );
  tree->mount( "/xml", *treeXML, "/" );

  // Fetching the tuple
  AIDA::IManagedObject* mo = tree->find( "/hbook/100" );
  if(!mo) {
    std::cout << "Object /hbook/100 not found." << std::endl;
    return 1;
  }
  AIDA::ITuple& tp = dynamic_cast<AIDA::ITuple&>( *mo );

  // tuple description
  std::cout << "Tuple title : " << tp.title() << std::endl;
  std::cout << "Tuple variables : " << std::endl;
  for ( int i = 0; i < tp.columns(); ++i ) {
    std::cout << tp.columnName(i) << " \t " << tp.columnType(i) << std::endl;
  }

  // Creating an evaluator object
  std::auto_ptr<AIDA::IEvaluator> eval( tpf->createEvaluator( "beamEnergy / nTracks" ) );
  std::auto_ptr<AIDA::IFilter> cut( tpf->createFilter( "goodTrigger && nTracks > 0" ) );

  // Creating a cloud to project the tuple
  AIDA::ICloud1D& cl = *( hf->createCloud1D( "cloud_1", "Beam energy per track" ) );
  tp.project( cl, *eval, *cut );

  // Copying the cloud into a binned histogram in XML
  tree->cd( "/xml" );
  AIDA::IHistogram1D& h1 = *( hf->createCopy( "histo_1", cl.histogram() ) );
  h1.setTitle( "Beam energy per track" );

  // Creating the function which is going to be fitted with the histogram data
  AIDA::IFunction& gaussFun = *( ff->createFunctionByName("Gaussian Fit","G") );

  // set function initial parameters
  gaussFun.setParameter("mean" , h1.mean() );
  gaussFun.setParameter("sigma", h1.rms() );
  gaussFun.setParameter("amp"  , 10 );

  // Creating the fitter
  std::auto_ptr<AIDA::IFitter> fitter( fitf->createFitter() );

  AIDA::IAnnotation& annotation = h1.annotation();

  // Performing the fit
  AIDA::IFitResult* fitResult = fitter->fit( h1, gaussFun );
  if(!fitResult) {
    std::cout << "Fit failed." << std::endl;
  } else {
    const std::vector<std::string>& parNames = 
      fitResult->fittedParameterNames();
    const std::vector<double>& par = fitResult->fittedParameters();
    const std::vector<double>& epar = fitResult->errors();

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
      std::ostringstream osOptions;
      osOptions << "[" << h1.axis().lowerEdge() << "," << h1.axis().upperEdge() << "] overlay";
      plotter->currentRegion().plot( gaussFun, osOptions.str() );
      plotter->refresh();
      plotter->interact();
    }
  }

  // Listing the tree contents
  std::cout << "Full tree contents :" << std::endl;
  tree->ls( "/", 1 );

  // Flushing the results
  tree->commit();
  tree->close();

  // Wait for the keyboard return to avoid destroying the plotter window too quickly.
  //std::cout << "Press <ENTER> to exit" << std::endl;
  //std::cin.get();

 } // End of global block.

  std::cout << "exit..." << std::endl;

  return 0;
}
