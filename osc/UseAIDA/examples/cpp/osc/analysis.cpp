
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>
#include <AIDA/IPlotterFactory.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>

#include <inlib/random>
#include <inlib/args>

#include <iostream>

int main(int argc,char* argv[]){
  int verboseLevel = 0;

  inlib::args args(argc,argv);
  bool help = args.is_arg("-h");
  if(help) {
    std::cout << "analysis :" << std::endl;
    std::cout << "Options : " << std::endl;
    std::cout << "  OS> analysis -h" << std::endl;
    std::cout << "   to have help." << std::endl;
    std::cout << "  OS> analysis -verbose" << std::endl;
    std::cout << "   to switch to verbose mode." << std::endl;
    std::cout << "  OS> analysis -store=[memory,xml,root,hbook]" << std::endl;
    std::cout << "   to store :"
              << "     in memory"
              << "     in an xml file"
              << "     in a file at the ROOT format with OnXLab objects."
              << std::endl;
    std::cout << "  OS> analysis -store=root" << std::endl;
    std::cout << "   to store in a file at the ROOT format with ROOT objects."
              << std::endl;
    std::cout << "  OS> analysis -store=root -streaming=native" 
              << std::endl;
    std::cout << "   to store in a file at the ROOT format but by using BatcLab streaming."
              << std::endl;
    std::cout << "  OS> analysis -plotting" << std::endl;
    std::cout << "   to run with the plotting." << std::endl;
    std::cout << "  OS> analysis -xyModeling=boxes" << std::endl;
    std::cout << "   to plot 2D with variable size boxes instead of colors." 
              << std::endl;
    return 0;
  }

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  AIDA::IAnalysisFactory* analysisFactory = AIDA_createAnalysisFactory();
  if(!analysisFactory) {
    std::cout << "analysis :"
              << " Can't get an AnalysisFactory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Have an AnalysisFactory." << std::endl;
  }

  AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
  if(!treeFactory) {
    std::cout << "analysis :"
              << " Can't get a TreeFactory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Have a TreeFactory." << std::endl;
  }

  int count;

  // Create some random engines :
  inlib::random::gauss randomGauss;
  inlib::random::flat randomFlat;
  inlib::random::bw randomBreitWigner;

  bool first_tree = true;
  //bool first_tree = false;
  bool second_tree = true;

  // Decide storage type and options from arguments :
  std::string store;
  if(!args.find("-store",store)) store = "memory";
  bool compress  = args.is_arg("-compress");
  std::string sstreaming;
  args.find("-streaming",sstreaming);
  if(verboseLevel) {
    std::cout << "analysis :"
              << " Choosen store type is \"" << store << "\"." << std::endl;
  }

  //////////////////////////////////////////////////////////
  /// First tree :
  //////////////////////////////////////////////////////////
  if(first_tree) 
 {AIDA::ITree* tree = 0;
  if(store=="memory") {
    tree = treeFactory->create();
  } else {
    if(store=="root") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      opts += ";streaming="+sstreaming;
      tree = treeFactory->create("analysis.root","root",false,true,opts);
    } else if(store=="xml") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      tree = treeFactory->create("analysis.aida","xml",false,true,opts);
    } else if(store=="zip") {
      std::string opts;
      tree = treeFactory->create("analysis.zip","zip",false,true,opts);
    } else if(store=="fs") {
      std::string opts;
      opts += "streaming=ascii";
      //opts += ";header=false";
      //opts += ";tab=true";
      tree = treeFactory->create("analysis.fs","fs",false,true,opts);
    } else if(store=="hdf5") {
      std::string opts;
      tree = treeFactory->create("analysis.hdf5","hdf5",false,true,opts);
    } else if(store=="ascii") {
      std::string opts;
      tree = treeFactory->create("analysis.ascii","ascii",false,true,opts);
    } else if((store=="hbook")||((store=="zebra"))) {
      tree = treeFactory->create("analysis.hbook","hbook",false,true);
    } else {
      std::cout << "analysis :"
                << " store type " << store << " unkown." << std::endl;
      return 1;
    }
  }
  if(!tree) {
    std::cout << "analysis :"
              << " Can't create the analysis tree." << std::endl;
    return 1;
  }
  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"" << tree->storeName() << "\" created." << std::endl;
  }

  if(!tree->mkdir("histograms")) {
    std::cout << "analysis :"
              << " \"histograms\" directory not created." << std::endl;
    return 1;
  }
  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"histograms\" directory created." << std::endl;
  }

  if(!tree->cd("histograms")) {
    std::cout << "analysis :"
              << " can't cd to \"histograms\"." << std::endl;
    return 1;
  }
  if(verboseLevel) {
    std::cout << "analysis :"
              << " cd to \"histograms\" done." << std::endl;
  }

  // Get an histogram factory :
  AIDA::IHistogramFactory* histoFactory = 
    analysisFactory->createHistogramFactory(*tree);
  if(!histoFactory) {
    std::cout << "analysis :"
              << " Can't get a HistogramFactory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Have a HistogramFactory." << std::endl;
  }

  ////////////////////////////
  // Create some histos :
  ////////////////////////////
  AIDA::IHistogram1D* gauss = 
    histoFactory->createHistogram1D("gauss","Random gauss",100,-5,5);
  if(!gauss) {
    std::cout << "analysis :"
              << " Can't create an histogram." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"gauss\" histogram created." << std::endl;
  }

  for(count=0;count<5000;count++) gauss->fill(randomGauss.shoot());

  std::cout << "OnXLab_analysis::main :"
            << "gauss :"
            << " entries " << gauss->entries() 
            << " mean " << gauss->mean() << " rms " << gauss->rms() 
            << std::endl;

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"gauss\" histogram filled." << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram1D* flat = 
    histoFactory->createHistogram1D("10","Random flat",100,0,1);
  if(!flat) {
    std::cout << "analysis :"
              << " Can't create the flat histogram." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"flat\" histogram created." << std::endl;
  }

  for(count=0;count<5000;count++) flat->fill(randomFlat.shoot());

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"flat\" histogram filled." << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram2D* gauss_BW = 
    histoFactory->createHistogram2D("gauss_BW",
                                    "BW/Gauss",
                                    100,-5,5,
                                    100,-5,5);
  if(!gauss_BW) {
    std::cout << "analysis :"
              << " Can't create the gauss_BW histogram." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"gauss_BW\" histogram created." << std::endl;
  }

  for(count=0;count<1000;count++) {
    gauss_BW->fill(randomGauss.shoot(),randomBreitWigner.shoot());
  }

  std::cout << "OnXLab_analysis::main :"
            << "gauss_BW :"
            << " entries " << gauss_BW->entries() 
            << " meanX " << gauss_BW->meanX() << " rmsX " << gauss_BW->rmsX() 
            << " meanY " << gauss_BW->meanY() << " rmsY " << gauss_BW->rmsY() 
            << std::endl;

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"gauss_BW\" histogram filled." << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram1D* BW = 
    histoFactory->createHistogram1D("BW","Random BW",100,-5,5);
  if(!BW) {
    std::cout << "analysis :"
              << " Can't create the BW histogram." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"BW\" histogram created." << std::endl;
  }

  for(count=0;count<5000;count++) {
    BW->fill(randomBreitWigner.shoot());
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"BW\" histogram filled." << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IProfile1D* profile = 
    histoFactory->createProfile1D("profile",
                                  "Profile",
                                  100,-5,5,-2,2);
  if(!profile) {
    std::cout << "analysis :"
              << " Can't create the Profile histogram." << std::endl;
  } else {
    if(verboseLevel) {
      std::cout << "analysis :"
                << " \"profile\" histogram created." << std::endl;
    }
    for(count=0;count<100000;count++) {
      profile->fill(randomGauss.shoot(),randomBreitWigner.shoot());
    }
    std::cout << "OnXLab_analysis::main :"
              << "profile :"
              << " entries " << gauss_BW->entries() 
              << " meanX " << gauss_BW->meanX() 
              << " rmsX " << gauss_BW->rmsX() 
              << std::endl;
    if(verboseLevel) {
      std::cout << "analysis :"
                << " \"profile\" histogram filled." << std::endl;
    }
  }

  ////////////////////////////
  ////////////////////////////
  std::vector<double> edges;
  edges.push_back(-4);
  edges.push_back(-3);
  edges.push_back(-2.5);
  edges.push_back(-2);
  edges.push_back(-1.75);
  edges.push_back(-1.5);
  edges.push_back(-1.25);
  edges.push_back(-1);
  edges.push_back(-0.875);
  edges.push_back(-0.75);
  edges.push_back(-0.625);
  edges.push_back(-0.5);
  edges.push_back(-0.375);
  edges.push_back(-0.25);
  edges.push_back(-0.125);
  edges.push_back(0);
  edges.push_back(0.125);
  edges.push_back(0.25);
  edges.push_back(0.375);
  edges.push_back(0.5);
  edges.push_back(0.625);
  edges.push_back(0.75);
  edges.push_back(0.875);
  edges.push_back(1);
  edges.push_back(1.25);
  edges.push_back(1.5);
  edges.push_back(1.75);
  edges.push_back(2);
  edges.push_back(2.5);
  edges.push_back(3);
  edges.push_back(4);
  AIDA::IHistogram1D* gauss_vabins = 
    histoFactory->createHistogram1D("gauss_vabins",
                                    "Random gauss variable bins",
                                    edges);
  if(!gauss_vabins) {
    std::cout << "analysis :"
              << " Can't create va bins histogram." << std::endl;
  } else {
    if(verboseLevel) {
      std::cout << "analysis :"
                << " \"gauss_vabins\" histogram created." << std::endl;
    }

    inlib::random::gauss randomGauss2(0,2);
    for(count=0;count<5000;count++) gauss_vabins->fill(randomGauss2.shoot());

    if(verboseLevel) {
      std::cout << "analysis :"
                << " \"gauss_vabins\" histogram filled." << std::endl;
    }
  }

  delete histoFactory; // An histo factory is not "managed", we have to 
                       // delete it ourself.

  ////////////////////////////
  // Plotting :
  ////////////////////////////
  if(args.is_arg("-plotting")) {
    int argc = 1;
    static char* argv[] = {(char*)"-stdout"};

    AIDA::IPlotterFactory* plotterFactory = 
      analysisFactory->createPlotterFactory(argc,argv);
    if(!plotterFactory) {
      std::cout << "analysis :"
                << " Can't get a PlotterFactory." << std::endl;
      return 1;
    } else {
      if(verboseLevel) {
        std::cout << "analysis :"
                  << " Have a PlotterFactory." << std::endl;
      }
    
      AIDA::IPlotter* plotter = plotterFactory->create();
      if(!plotter) {
        std::cout << "analysis :"
                  << " Can't get a plotter." << std::endl;
      } else {
        if(verboseLevel) {
           std::cout << "analysis :"
                    << " Have a Plotter." << std::endl;
        }
    
        plotter->show();
        plotter->clearRegions();
        AIDA::IPlotterRegion& region = plotter->currentRegion();
        region.plot(*BW);
        std::cout << "analysis :"
                  << " Click the escape (or exit, quit) button to continue..." 
                  << std::endl;
        // The user have to click in some "escape GUI" button
        // to return from "interact".
        plotter->interact();
        plotter->hide();
       //{std::cout << "Type c and return to continue" << std::endl;
       // char c;
       // std::cin >> c;}   

        // Demonstrate how to update the plotter :
        region.clear();
        region.plot(*gauss_BW);
    
        region.setParameter("plotter.wallStyle.visible","FALSE");
        region.setParameter("plotter.binsStyle[0].multiNodeLimit","1000000");

       {std::string s;
        args.find("-xyModeling",s);
        if(s=="boxes") {
          region.setParameter("plotter.binsStyle[0].modeling","boxes");
        } else {
          region.setParameter("plotter.binsStyle[0].modeling","solid");
          region.setParameter("plotter.binsStyle[0].painting","BY_VALUE");
          // [0 , 10[ = white ; [10, 50[ = yellow ; [50,oo[ red :
          region.setParameter
            ("plotter.colorMapping","yellow 10 orange 40 red ");
        }}

        for(int update = 0;update<3;update++) {
          // New data :
          for(int count = 0;count<5000;count++) {
            gauss_BW->fill(randomGauss.shoot(),randomBreitWigner.shoot());
          }
          // Update the plotter :
          plotter->refresh();
          std::cout << "analysis :"
                    << " Click the escape (or exit, quit) button to continue ("
                    << update
                    << ")..."
                    << std::endl;
          plotter->show();
          plotter->interact();
          plotter->hide();
         //{std::cout << "Type c and return to continue" << std::endl;
         // char c;
         // std::cin >> c;}
        }
        if(verboseLevel) {
          std::cout << "analysis :"
                    << " End plotting." << std::endl;
        }
    
        delete plotter;
      }
      delete plotterFactory;
    }
  }

  if(!tree->cd("..")) {
    std::cout << "analysis :"
              << " can't cd to \"..\"." << std::endl;
    return 1;
  }

  if(!tree->mkdir("tuples")) {
    std::cout << "analysis :"
              << " can't make directory \"tuples\"." << std::endl;
    return 1;
  }

  if(!tree->cd("tuples")) {
    std::cout << "analysis :"
              << " can't cd to \"tuples\"." << std::endl;
    return 1;
  }

  // Get a tuple factory :
  AIDA::ITupleFactory* tupleFactory = 
    analysisFactory->createTupleFactory(*tree);
  if(!tupleFactory) {
    std::cout << "analysis :"
              << " Can't get a TupleFactory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Have a TupleFactory." << std::endl;
  }

  // Create some tuples :
  AIDA::ITuple* tuple1 = tupleFactory->create("tuple1","tuple1",
                                              "double v1, double v2");
  if(!tuple1) {
    std::cout << "analysis :"
              << " Can't create the \"tuple1\" tuple." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"tuple1\" created." << std::endl;
  }

  // Fill so that an histo will give a step function :
  // The tuple will have 2 columns (filled with same data)
  // and :
  //     0 + 1 + 2 + .... + (n-1) = (n-1) * n/2
  // rows. Then below : n = 10, then 45 rows.
  for(count=0;count<10;count++) {
    for(int i=0;i<count;i++) {
      tuple1->fill(0,(double)count);
      tuple1->fill(1,(double)count);
      tuple1->addRow();
    }
  }

  AIDA::ITuple* tuple2 = 
    tupleFactory->create("tuple2","tuple2",
                         "double rand_gauss, double rand_BW, double step");
  if(!tuple2) {
    std::cout << "analysis :"
              << " Can't create the \"tuple2\" tuple." << std::endl;
    return 1;
  }


  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"tuple2\" created." << std::endl;
  }

  for(count=0;count<5000;count++) {
    tuple2->fill(0,randomGauss.shoot());
    tuple2->fill(1,randomBreitWigner.shoot());
    tuple2->fill(2,(double)count);
    tuple2->addRow();
  }

  if(!tree->cd("..")) {
    std::cout << "analysis :"
              << " can't cd to \"..\"." << std::endl;
    return 1;
  }

  ////////////////////////////
  // Create some clouds :
  ////////////////////////////
  if(!tree->mkdir("clouds")) {
    std::cout << "analysis :"
              << " can't make directory \"clouds\"." << std::endl;
    return 1;
  }

  if(!tree->cd("clouds")) {
    std::cout << "analysis :"
              << " can't cd to \"clouds\"." << std::endl;
    return 1;
  }

  // Get an histogram factory :
  AIDA::IHistogramFactory* cloudFactory = 
    analysisFactory->createHistogramFactory(*tree);
  if(!histoFactory) {
    std::cout << "analysis :"
              << " Can't get a HistogramFactory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Have a HistogramFactory." << std::endl;
  }


  AIDA::ICloud1D* cl1D = 
    cloudFactory->createCloud1D( "cl1D","1-Dimensional Cloud");
  if(!cl1D) {
    std::cout << "analysis :"
              << " Can't create cloud cl1D." << std::endl;
    return 1;
  }
  AIDA::ICloud2D* cl2D = 
    cloudFactory->createCloud2D( "cl2D","2-Dimensional Cloud");
  if(!cl2D) {
    std::cout << "analysis :"
              << " Can't create cloud cl2D." << std::endl;
    return 1;
  }
  AIDA::ICloud3D* cl3D = 
    cloudFactory->createCloud3D( "cl3D","3-Dimensional Cloud");
  if(!cl3D) {
    std::cout << "analysis :"
              << " Can't create cloud cl3D." << std::endl;
    return 1;
  }
      
 {inlib::random::gauss r(1,0.5);
  for ( unsigned int i = 0; i < 20000; i++ ) {
    double xval = randomGauss.shoot();
    double yval = r.shoot();
    double zval = randomBreitWigner.shoot();
    double w    = randomFlat.shoot();         
    cl1D->fill( xval, w );
    cl2D->fill( xval, yval, w );
    cl3D->fill( xval, yval, zval, w );
  }}
      
  delete cloudFactory;

  if(!tree->cd("..")) {
    std::cout << "analysis :"
              << " can't cd to \"..\"." << std::endl;
    return 1;
  }

  tree->ls();

  if(!tree->commit()) {
    std::cout << "analysis :"
              << " can't commit." << std::endl;
    return 1;
  }

  std::cout << "analysis :"
            << " Produce \"" << tree->storeName() << "\"..." << std::endl;
  tree->ls();

  std::cout << "analysis :"
            << " pwd \"" << tree->pwd() << "\"." << std::endl;

  delete tupleFactory;
  if(store!="memory") {
    delete tree;
  } else {
    tree->rmdir("/histograms");
    tree->rmdir("/tuples");
    tree->rmdir("/clouds");
  }
  } //end first_tree

  //////////////////////////////////////////////////////////
  /// Second tree :
  //////////////////////////////////////////////////////////
  if(second_tree)
 {AIDA::ITree* tree = 0;
  if(store=="memory") {
    tree = treeFactory->create();
  } else {
    if(store=="root") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      opts += ";streaming="+sstreaming;
      tree = treeFactory->create("analysis1.root","root",false,true,opts);
    } else if(store=="xml") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      tree = treeFactory->create("analysis1.aida","xml",false,true,opts);
    } else if(store=="zip") {
      std::string opts;
      tree = treeFactory->create("analysis1.zip","zip",false,true,opts);
    } else if(store=="fs") {
      std::string opts;
      //opts += "streaming=ascii";
      tree = treeFactory->create("analysis1.fs","fs",false,true,opts);
    } else if(store=="hdf5") {
      std::string opts;
      tree = treeFactory->create("analysis1.hdf5","hdf5",false,true,opts);
    } else if(store=="ascii") {
      std::string opts;
      tree = treeFactory->create("analysis1.ascii","ascii",false,true,opts);
    } else if(store=="hbook") {
      tree = treeFactory->create("analysis1.hbook","hbook",false,true);
    }
  }
  if(!tree) {
    std::cout << "analysis :"
              << " Can't create the analysis1 tree." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"" << tree->storeName() << "\" created." << std::endl;
  }

  if(!tree->mkdir("tuples")) {
    std::cout << "analysis :"
              << " \"tuples\" directory not created." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"tuples\" directory created." << std::endl;
  }

  if(!tree->cd("tuples")) {
    std::cout << "analysis :"
              << " can't cd to \"tuples\"." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " cd to \"tuples\" done." << std::endl;
  }


  AIDA::ITupleFactory* tupleFactory = 
    analysisFactory->createTupleFactory(*tree);
  if(!tupleFactory) {
    std::cout << "analysis :"
              << " Can't create a TupleFactory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Have a TupleFactory." << std::endl;
  }

  AIDA::ITuple* tuple = 
    tupleFactory->create("tuple","tuple","double rand_gauss, double rand_BW");
  if(!tuple) {
    std::cout << "analysis :"
              << " Can't create the \"tuple\" tuple." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "analysis :"
              << " \"tuple\" created." << std::endl;
  }

  for(count=0;count<100000;count++) {
    tuple->fill(0,randomGauss.shoot());
    tuple->fill(1,randomBreitWigner.shoot());
    tuple->addRow();
  }
  tree->commit();
  std::cout << "analysis :"
            << " Produce \"" << tree->storeName() << "\"..." << std::endl;
  tree->ls();
  delete tupleFactory;

  if(store!="memory") {
    delete tree;
  } else {
    tree->rmdir("/tuples");
  }
  } //end second_tree

  delete treeFactory;

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Delete the AnalysisFactory..." << std::endl;
  }

  delete analysisFactory; // Will delete the trees.

  if(verboseLevel) {
    std::cout << "analysis :"
              << " Exiting..." << std::endl;
  }

  return 0;
}

