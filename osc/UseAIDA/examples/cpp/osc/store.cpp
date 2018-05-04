
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

#include <AIDA/IDataPointSetFactory.h>
#include <AIDA/IDataPointSet.h>
#include <AIDA/IDataPoint.h>
#include <AIDA/IMeasurement.h>

#include <AIDA/IFunctionFactory.h>
#include <AIDA/IFunction.h>

#include <inlib/args>
#include <inlib/sys/atime>
#include <inlib/random>

#include <iostream>

static void dump_help();
static AIDA::ITree* createTree(AIDA::IAnalysisFactory&,
                               const inlib::args&,
                               int);
static bool create_histograms(AIDA::IAnalysisFactory&,AIDA::ITree&,int);
static bool create_tuples(AIDA::IAnalysisFactory&,AIDA::ITree&,int);
static bool create_clouds(AIDA::IAnalysisFactory&,AIDA::ITree&,int);
static bool create_dpss(AIDA::IAnalysisFactory&,AIDA::ITree&,int);
static bool create_functions(AIDA::IAnalysisFactory&,AIDA::ITree&,int);

static int sHistos = 10;
static int sClouds = 10;
static int sDPSs = 10;

static bool sRandom = false;
static int sEntries = 100;

int main(int aArgc,char** aArgv){
  int verboseLevel = 0;

 {//  Put everything in a block so that temporary objects
  // on the stack are deleted before the final memory decount is done. 

  inlib::args args(aArgc,aArgv);
  if(args.is_arg("-h")) {
    dump_help();
    return 0;
  }

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_store :"
              << " Can't get an AnalysisFactory." 
              << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "aida_exa_store :"
              << " AIDA found." 
              << std::endl;
  }

  AIDA::ITree* tree = createTree(*aida,args,verboseLevel);
  if(!tree) {
    delete aida;
    return 1;
  }

  sRandom = args.is_arg("-rand");

  if(!args.find<int>("-entries",sEntries)) sEntries = 100;

  //std::cout << "aida_exa_store :"
  //        << " rand " << inlib::sout(sRandom)
  //        << " shootn " << sEntries
  //        << std::endl;

  bool doHistos = args.is_arg("-histos");
  if(doHistos) {
    int value;
    if(args.find<int>("-histos",value)) sHistos = value;
  }
  bool doClouds = args.is_arg("-clouds");
  if(doClouds) {
    int value;
    if(args.find<int>("-clouds",value)) sClouds = value;
  }
  bool doDPSs = args.is_arg("-dpss");
  if(doDPSs) {
    int value;
    if(args.find<int>("-dpss",value)) sDPSs = value;
  }

  bool doTuples = args.is_arg("-tuples");
  bool doFuncs = args.is_arg("-funcs");

  bool doAll = doHistos||doTuples||doClouds||doDPSs||doFuncs ? false : true;

  if(doAll) {
    doHistos = true;
    doTuples = true;
    doClouds = true;
    doDPSs = true;
    doFuncs = true;
  }

  if(doHistos) create_histograms(*aida,*tree,verboseLevel);
  if(doTuples) create_tuples(*aida,*tree,verboseLevel);
  if(doClouds) create_clouds(*aida,*tree,verboseLevel);
  if(doDPSs) create_dpss(*aida,*tree,verboseLevel);
  if(doFuncs) create_functions(*aida,*tree,verboseLevel);

  std::cout << "aida_exa_store :"
            << " start commit..." 
            << std::endl;

  inlib::atime start = inlib::atime::now();
  if(!tree->commit()) {
    std::cout << "aida_exa_store :"
              << " can't commit." 
              << std::endl;
  }
  inlib::atime elapsed = inlib::atime::elapsed(start);
  std::cout << "aida_exa_store :"
            << " commit elapsed time : " << elapsed.value()
            << std::endl;

  if(verboseLevel) {
    std::cout << "aida_exa_store :"
              << " Delete the AnalysisFactory..." 
              << std::endl;
  }

  delete aida; // Will delete the trees.
  }

  if(verboseLevel) {
    std::cout << "aida_exa_store::main :"
              << " Exiting..." 
              << std::endl;
  }

  return 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void dump_help(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::cout << "aida_exa_store::main :" << std::endl;
  std::cout << "Options : " << std::endl;
  std::cout << "  OS> aida_exa_store -h" << std::endl;
  std::cout << "   to have help." << std::endl;
  std::cout << "  OS> aida_exa_store -verbose" << std::endl;
  std::cout << "   to switch to verbose mode." << std::endl;
  std::cout << "  OS> aida_exa_store [-tuples,-clouds,-dps,-funcs]" << std::endl;
  std::cout << "   to store only related kind of data." << std::endl;
  std::cout << "  OS> aida_exa_store -histos=<int>" << std::endl;
  std::cout << "   to store <int> histos." << std::endl;
  std::cout << "  OS> aida_exa_store -entries=<int>" << std::endl;
  std::cout << "   to fill <int> entries." << std::endl;
  std::cout << "  OS> aida_exa_store -store=[memory,xml,root,hbook]" 
            << std::endl;
  std::cout << "   to store :"
            << "     in memory"
            << "     in an xml file"
            << "     in a file at the ROOT format with BatchLab objects."
            << std::endl;
  std::cout << "  OS> aida_exa_store -store=root -streaming=[root,native]" 
            << std::endl;
  std::cout << "   to store :"
            << "     in a file at the ROOT format with ROOT objects."
            << std::endl;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
AIDA::ITree* createTree(
 AIDA::IAnalysisFactory& aAIDA
,const inlib::args& aArgs
,int aVerbose
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) {
    std::cout << "aida_exa_store :"
              << " Can't get a TreeFactory." 
              << std::endl;
    return 0;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Have a TreeFactory." 
              << std::endl;
  }

  // Decide storage type and options from arguments :
  std::string store;
  if(!aArgs.find("-store",store)) store = "memory";
  bool compress  = aArgs.is_arg("-compress");
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Choosen store type is " << inlib::sout(store) << "." 
              << std::endl;
  }

  std::string name = "aida_exa_store";

  AIDA::ITree* tree = 0;
  if(store=="memory") {
    tree = treeFactory->create();
  } else {
    if(store=="root") {
      std::string sstreaming;
      if(!aArgs.find("-streaming",sstreaming)) sstreaming = "root";
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      opts += ";streaming="+sstreaming;
      tree = treeFactory->create(name+".root","root",false,true,opts);
    } else if(store=="xml") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      tree = treeFactory->create(name+".aida","xml",false,true,opts);
    } else if(store=="hdf5") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      tree = treeFactory->create(name+".hdf5","hdf5",false,true,opts);
    } else if((store=="hbook")||((store=="zebra"))) {
      tree = treeFactory->create(name+".hbook","hbook",false,true);
    } else {
      std::cout << "aida_exa_store :"
                << " store type " << store << " unkown." << std::endl;
      return 0;
    }
  }
  delete treeFactory;
  if(!tree) {
    std::cout << "aida_exa_store :"
              << " Can't create the " << inlib::sout(name) << " tree." 
              << std::endl;
    return 0;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << inlib::sout(tree->storeName()) << " created." 
              << std::endl;
  }

  return tree;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool create_histograms(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,int aVerbose
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::cout << "aida_exa_store :"
            << " create_histograms : " << sHistos
            << std::endl;

  if(!aTree.mkdir("histograms")) {
    std::cout << "aida_exa_store :"
              << " \"histograms\" directory not created." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"histograms\" directory created." 
              << std::endl;
  }

  if(!aTree.cd("histograms")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"histograms\"." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " cd to \"histograms\" done." 
              << std::endl;
  }

  // Get a factory :
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) {
    std::cout << "aida_exa_store :"
              << " Can't get a HistogramFactory." 
              << std::endl;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Have a HistogramFactory." 
              << std::endl;
  }

  for(int index=0;index<sHistos;index++) {
  std::string sindex = inlib::to<int>(index);

  inlib::random::gauss randomGauss;
  inlib::random::flat randomFlat;
  inlib::random::bw randomBreitWigner;

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram1D* gauss = 
    factory->createHistogram1D("gauss_"+sindex,"Random gauss",100,-5,5);
  if(!gauss) {
    std::cout << "aida_exa_store :"
              << " Can't create the gauss histogram." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " gauss histogram created." 
              << std::endl;
  }

  if(sRandom) {
    for(int count=0;count<sEntries;count++) gauss->fill(randomGauss.shoot());
  } else {
    for(int count=0;count<sEntries;count++) gauss->fill(double(count%10 - 5));
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << "gauss :"
              << " entries " << gauss->entries() 
              << " mean " << gauss->mean() << " rms " << gauss->rms() 
              << std::endl;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " gauss histogram filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram1D* flat = 
    factory->createHistogram1D("flat_"+sindex,"Random flat",100,0,1);
  if(!flat) {
    std::cout << "aida_exa_store :"
              << " Can't create the flat histogram." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " flat histogram created." 
              << std::endl;
  }

  if(sRandom) {
    for(int count=0;count<sEntries;count++) flat->fill(randomFlat.shoot());
  } else {
    for(int count=0;count<sEntries;count++) flat->fill(double(count%10)/10.0);
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " flat histogram filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram2D* gauss_bw =
    factory->createHistogram2D("gauss_bw_"+sindex,
                               "BW/Gauss",
                               100,-5,5,
                               100,-5,5);
  if(!gauss_bw) {
    std::cout << "aida_exa_store :"
              << " Can't create the gauss_bw histogram." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " gauss_bw histogram created." 
              << std::endl;
  }

  if(sRandom) {
    for(int count=0;count<sEntries;count++)
      gauss_bw->fill(randomGauss.shoot(),randomBreitWigner.shoot());
  } else {
    for(int count=0;count<sEntries;count++)
      gauss_bw->fill(double(count%10-5),double(count%10-5));
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << "gauss_bw :"
              << " entries " << gauss_bw->entries() 
              << " meanX " << gauss_bw->meanX() 
              << " rmsX " << gauss_bw->rmsX() 
              << " meanY " << gauss_bw->meanY() 
              << " rmsY " << gauss_bw->rmsY() 
              << std::endl;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " gauss_bw histogram filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IHistogram1D* bw = 
    factory->createHistogram1D("bw_"+sindex,"Random BW",100,-5,5);
  if(!bw) {
    std::cout << "aida_exa_store :"
              << " Can't create the bw histogram." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " bw histogram created." 
              << std::endl;
  }

  if(sRandom) {
    for(int count=0;count<sEntries;count++) 
      bw->fill(randomBreitWigner.shoot());
  } else {
    for(int count=0;count<sEntries;count++) 
      bw->fill(double(count%10-5));
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " bw histogram filled." 
              << std::endl;
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
  edges.push_back(0.0);
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
    factory->createHistogram1D("gauss_vabins_"+sindex,
                               "Random gauss variable bins",
                               edges);
  if(!gauss_vabins) {
    std::cout << "aida_exa_store :"
              << " Can't create the gauss_vabins histogram." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " gauss_vabins histogram created." 
              << std::endl;
  }

  inlib::random::gauss randomGauss2(0,2);

  if(sRandom) {
    for(int count=0;count<sEntries;count++) 
      gauss_vabins->fill(randomGauss2.shoot());
  } else {
    for(int count=0;count<sEntries;count++) 
      gauss_vabins->fill(double(count%8-4));
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " gauss_vabins histogram filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::IProfile1D* profile = 
    factory->createProfile1D("profile_"+sindex,
                             "Profile",
                             100,-5,5,-2,2);
  if(!profile) {
    std::cout << "aida_exa_store :"
              << " Can't create the profile histogram." 
             << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " profile histogram created." 
              << std::endl;
  }

  if(sRandom) {
    for(int count=0;count<sEntries;count++) 
      profile->fill(randomGauss.shoot(),randomBreitWigner.shoot());
  } else {
    for(int count=0;count<sEntries;count++) 
      profile->fill(double(count%10-5),double(count%4-2));
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << "profile :"
              << " entries " << gauss_bw->entries() 
              << " meanX " << gauss_bw->meanX() 
              << " rmsX " << gauss_bw->rmsX() 
              << std::endl;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " profile histogram filled." 
              << std::endl;
  }


  } //index

  delete factory;

  if(!aTree.cd("..")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"..\"." << std::endl;
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool create_tuples(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,int aVerbose
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::cout << "aida_exa_store : create_tuples." << std::endl;

  if(!aTree.mkdir("tuples")) {
    std::cout << "aida_exa_store :"
              << " \"tuples\" directory not created." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"tuples\" directory created." 
              << std::endl;
  }

  if(!aTree.cd("tuples")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"tuples\"." 
              << std::endl;
    return false;
  }

  // Get a factory :
  AIDA::ITupleFactory* factory = aAIDA.createTupleFactory(aTree);
  if(!factory) {
    std::cout << "aida_exa_store :"
              << " Can't get a TupleFactory." 
              << std::endl;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Have a TupleFactory." 
              << std::endl;
  }

  inlib::random::gauss randomGauss;
  inlib::random::bw randomBreitWigner;

  ////////////////////////////
  ////////////////////////////
 {AIDA::ITuple* tuple1 = factory->create("tuple1","tuple1",
                                         "double v1, double v2");
  if(!tuple1) {
    std::cout << "aida_exa_store :"
              << " Can't create the tuple1 tuple." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " tuple1 created." 
              << std::endl;
  }

  // Fill so that an histo will give a step function :
  // The tuple will have 2 columns (filled with same data)
  // and :
  //     0 + 1 + 2 + .... + (n-1) = (n-1) * n/2
  // rows. Then below : n = 10, then 45 rows.
  for(int count=0;count<10;count++) {
    for(int i=0;i<count;i++) {
      tuple1->fill(0,(double)count);
      tuple1->fill(1,(double)count);
      tuple1->addRow();
    }
  }}

  ////////////////////////////
  ////////////////////////////
 {AIDA::ITuple* tuple2 = 
    factory->create("tuple2","tuple2",
                    "double rand_gauss, double rand_BW, double step");
  if(!tuple2) {
    std::cout << "aida_exa_store :"
              << " Can't create the tuple2 tuple." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " tuple2 created." 
              << std::endl;
  }

  for(int count=0;count<5000;count++) {
    tuple2->fill(0,randomGauss.shoot());
    tuple2->fill(1,randomBreitWigner.shoot());
    tuple2->fill(2,(double)count);
    tuple2->addRow();
  }}

  ////////////////////////////
  ////////////////////////////
 {AIDA::ITuple* tuple3 = 
    factory->create("tuple3","tuple3",
      "double rand_gauss, ITuple tuple = { double rand_BW, double step}");
  if(!tuple3) {
    std::cout << "aida_exa_store :"
              << " Can't create the tuple3 tuple." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " tuple3 created." 
              << std::endl;
  }

  //for(int count=0;count<5000;count++) {
  for(int count=0;count<10;count++) {
    tuple3->fill(0,randomGauss.shoot());

    AIDA::ITuple* subTuple = tuple3->getTuple(1);
    for(int scount=0;scount<100;scount++) {
      subTuple->fill(0,randomBreitWigner.shoot());
      subTuple->fill(1,(double)scount);
      subTuple->addRow();
    }  

    tuple3->addRow();
  }}

  if(aVerbose) aTree.ls();

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Produce " << inlib::sout(aTree.storeName()) << "..." 
              << std::endl;
    aTree.ls();
    std::cout << "aida_exa_store :"
              << " pwd " << inlib::sout(aTree.pwd()) << "." 
              << std::endl;
  }

  delete factory;

  if(!aTree.cd("..")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"..\"." << std::endl;
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool create_clouds(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,int aVerbose
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::cout << "aida_exa_store : create_clouds." << std::endl;

  if(!aTree.mkdir("clouds")) {
    std::cout << "aida_exa_store :"
              << " \"clouds\" directory not created." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"clouds\" directory created." 
              << std::endl;
  }

  if(!aTree.cd("clouds")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"clouds\"." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " cd to \"clouds\" done." 
              << std::endl;
  }

  // Get a factory :
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) {
    std::cout << "aida_exa_store :"
              << " Can't get a CloudFactory." 
              << std::endl;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Have a CloudFactory." 
              << std::endl;
  }

  for(int index=0;index<sClouds;index++) {
  std::string sindex = inlib::to<int>(index);

  inlib::random::gauss randomGauss;
  inlib::random::flat randomFlat;
  inlib::random::bw randomBreitWigner;

  ////////////////////////////
  ////////////////////////////
  AIDA::ICloud1D* gauss = 
    factory->createCloud1D("gauss_"+sindex,"Random gauss");
  if(!gauss) {
    std::cout << "aida_exa_store :"
              << " Can't create an cloud." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"gauss\" cloud created." 
              << std::endl;
  }

  int count;
  for(count=0;count<5000;count++) gauss->fill(randomGauss.shoot());

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << "gauss :"
              << " entries " << gauss->entries() 
              << " mean " << gauss->mean() << " rms " << gauss->rms() 
              << std::endl;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"gauss\" cloud filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::ICloud1D* flat = factory->createCloud1D("flat_"+sindex,"Random flat");
  if(!flat) {
    std::cout << "aida_exa_store :"
              << " Can't create the flat cloud." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"flat\" cloud created." 
              << std::endl;
  }

  for(count=0;count<5000;count++) flat->fill(randomFlat.shoot());

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"flat\" cloud filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::ICloud2D* gauss_bw = 
    factory->createCloud2D("gauss_bw_"+sindex,"BW/Gauss");
  if(!gauss_bw) {
    std::cout << "aida_exa_store :"
              << " Can't create the gauss_bw cloud." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"gauss_bw\" cloud created." 
              << std::endl;
  }

  for(count=0;count<1000;count++) {
    gauss_bw->fill(randomGauss.shoot(),randomBreitWigner.shoot());
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << "gauss_bw :"
              << " entries " << gauss_bw->entries() 
              << " meanX " << gauss_bw->meanX() 
              << " rmsX " << gauss_bw->rmsX() 
              << " meanY " << gauss_bw->meanY() 
              << " rmsY " << gauss_bw->rmsY() 
              << std::endl;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"gauss_bw\" cloud filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::ICloud1D* bw = factory->createCloud1D("BW_"+sindex,"Random BW");
  if(!bw) {
    std::cout << "aida_exa_store :"
              << " Can't create the bw cloud." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " bw cloud created." 
              << std::endl;
  }

  for(count=0;count<5000;count++) bw->fill(randomBreitWigner.shoot());

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " bw cloud filled." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
  AIDA::ICloud3D* flat_gauss_bw = 
    factory->createCloud3D("flat_gauss_bw_"+sindex,"Flat/Gauss/BW");
  if(!flat_gauss_bw) {
    std::cout << "aida_exa_store :"
              << " Can't create the flat_gauss_bw cloud." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " flat_gauss_bw cloud created." 
              << std::endl;
  }

  for(count=0;count<5000;count++) {
    flat_gauss_bw->fill(randomFlat.shoot(),
                        randomGauss.shoot(),
                        randomBreitWigner.shoot());
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " flat_gauss_bw cloud filled." 
              << std::endl;
  }

  } //index

  delete factory;

  if(!aTree.cd("..")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"..\"." << std::endl;
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool create_dpss(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,int aVerbose
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::cout << "aida_exa_store : create_dpss." << std::endl;

  if(!aTree.mkdir("dps")) {
    std::cout << "aida_exa_store :"
              << " \"dps\" directory not created." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"dps\" directory created." 
              << std::endl;
  }

  if(!aTree.cd("dps")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"dps\"." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " cd to \"dps\" done." 
              << std::endl;
  }

  // Get a factory :
  AIDA::IDataPointSetFactory* factory = aAIDA.createDataPointSetFactory(aTree);
  if(!factory) {
    std::cout << "aida_exa_store :"
              << " Can't get a DataPointSetFactory." 
              << std::endl;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Have a DataPointSetFactory." 
              << std::endl;
  }

  for(int index=0;index<sDPSs;index++) {
  std::string sindex = inlib::to<int>(index);

  inlib::random::gauss randomGauss;
  inlib::random::flat randomFlat;
  inlib::random::bw randomBreitWigner;

  ////////////////////////////
  ////////////////////////////
  AIDA::IDataPointSet* flat_gauss_bw = 
    factory->create("flat_gauss_bw_"+sindex,"Flat/Gauss/BW",3);
  if(!flat_gauss_bw) {
    std::cout << "aida_exa_store :"
              << " Can't create the flat_gauss_bw data point set." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " flat_gauss_bw data point set created." 
              << std::endl;
  }

  int count;
  for(count=0;count<5000;count++) {
    AIDA::IDataPoint* point = flat_gauss_bw->addPoint();
   {AIDA::IMeasurement* m = point->coordinate(0);
    m->setValue(randomFlat.shoot());
    m->setErrorMinus(0.01);
    m->setErrorPlus(0.02);}
   {AIDA::IMeasurement* m = point->coordinate(1);
    m->setValue(randomGauss.shoot());
    m->setErrorMinus(0.01);
    m->setErrorPlus(0.02);}
   {AIDA::IMeasurement* m = point->coordinate(2);
    m->setValue(randomBreitWigner.shoot());
    m->setErrorMinus(0.01);
    m->setErrorPlus(0.02);}
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " flat_gauss_bw data point set filled." 
              << std::endl;
  }

  } //index

  delete factory;

  if(!aTree.cd("..")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"..\"." << std::endl;
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool create_functions(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,int aVerbose
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::cout << "aida_exa_store : create_functions." << std::endl;

  if(!aTree.mkdir("function")) {
    std::cout << "aida_exa_store :"
              << " \"function\" directory not created." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " \"function\" directory created." 
              << std::endl;
  }

  if(!aTree.cd("function")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"function\"." 
              << std::endl;
    return false;
  }
  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " cd to \"function\" done." 
              << std::endl;
  }

  // Get a factory :
  AIDA::IFunctionFactory* factory = aAIDA.createFunctionFactory(aTree);
  if(!factory) {
    std::cout << "aida_exa_store :"
              << " Can't get a FunctionFactory." 
              << std::endl;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " Have a FunctionFactory." 
              << std::endl;
  }

  ////////////////////////////
  ////////////////////////////
 {std::string model = "G";
  AIDA::IFunction* func = factory->createFunctionByName("gauss",model);
  if(!func) {
    std::cout << "aida_exa_store :"
              << " Can't create the " << model << " function." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " " << model << " function created." 
              << std::endl;
  }}

  ////////////////////////////
  ////////////////////////////
 {AIDA::IFunction* func = 
    factory->createFunctionFromScript
      ("scripted",1,"gauss(x,H,M,S)+breit(x,A,O,W)","","","");
  if(!func) {
    std::cout << "aida_exa_store :"
              << " Can't create the scripted function." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  std::vector<double> params;
  params.push_back(2);    // H
  params.push_back(0);    // M
  params.push_back(0.1);  // S
  params.push_back(1);    // A
  params.push_back(0.5);  // O
  params.push_back(0.1);  // W
  func->setParameters(params);

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " scripted function created." 
              << std::endl;
  }}

  ////////////////////////////
  ////////////////////////////
 {std::string model = 
    "compiled:rmason_func;$OSC_HOME_DIR/Resources/AIDA/examples/data/rmason_func.cpp";
  AIDA::IFunction* func = factory->createFunctionByName("rmason",model);
  if(!func) {
    std::cout << "aida_exa_store :"
              << " Can't create the " << model << " function." 
              << std::endl;
    delete factory;
    aTree.cd("..");
    return false;
  }

  // Starting values :
  std::vector<double> params;
  params.push_back(1);       // gauss_H
  params.push_back(0);       // gauss_M
  params.push_back(0.1);     // gauss_W
  params.push_back(1);       // cauchy_H
  params.push_back(0.5);     // cauchy_C
  params.push_back(2*0.2);   // cauchy_W
  params.push_back(0);       // pol_0
  params.push_back(0.01);    // pol_1
  func->setParameters(params);

  if(aVerbose) {
    std::cout << "aida_exa_store :"
              << " " << model << " function created." 
              << std::endl;
  }}

  delete factory;

  if(!aTree.cd("..")) {
    std::cout << "aida_exa_store :"
              << " can't cd to \"..\"." << std::endl;
    return false;
  }

  return true;
}
