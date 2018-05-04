
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>

#include <inlib/args>
#include <inlib/touplow>
#include <inlib/sprintf>

#include <Lib/Debug.h>

#include <iostream>

// Extracted from TRandom to be able to compare with ROOT.
class Rndm {
public:
  Rndm():fSeed(65539){}
  double shoot(int i = 0) {
    const double kCONS = 4.6566128730774E-10;
    const int kMASK24 = 2147483392;
    fSeed *= 69069;      
    // Set lower 8 bits to zero to assure exact float
    unsigned int jy = (fSeed&kMASK24);
    if (jy) return kCONS*jy;
    return shoot();
  }
private:
  unsigned int fSeed;
};

//////////////////////////////////////////////////////////////////////////////
void write(
 const std::string& aStore
,bool aCompress
,int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::IAnalysisFactory* analysisFactory = AIDA_createAnalysisFactory();
  if(!analysisFactory) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get an AnalysisFactory." << std::endl;
    return;
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Have an AnalysisFactory." << std::endl;
  }

  AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
  if(!treeFactory) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get a TreeFactory." << std::endl;
    return;
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Have a TreeFactory." << std::endl;
  }

  int count;

  AIDA::ITree* tree = 0;
  std::string storeType;
  std::string storeName;
  std::string option;
  option += aCompress ? "compress=yes" : "compress=no";
  storeName = "BatchLab_tuple_test";
  if(aStore=="root") {
    storeType = "root";
    storeName += ".root";
    option += std::string(";export=BatchLab");
  } else if(aStore=="xml") {
    storeType = "xml";
    storeName += ".aida";
  } else if(aStore=="hdf5") {
    storeType = "hdf5";
    storeName += ".hdf5";
  } else if(aStore=="fs") {
    storeType = "fs";
    storeName += ".fs";
  } else if(aStore=="zip") {
    storeType = "zip";
    storeName += ".zip";
  }
  tree = treeFactory->create(storeName,storeType,false,true,option);
  if(!tree) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get a Tree." << std::endl;
    return;
  }
  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " \"" << storeName << "\" created." << std::endl;
  }

  // Get a tuple factory :
  AIDA::ITupleFactory* tupleFactory = 
    analysisFactory->createTupleFactory(*tree);
  if(!tupleFactory) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get a TupleFactory." << std::endl;
    return; 
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Have a TupleFactory." << std::endl;
  }


  // Create a tuple :
  int icol;
  std::string scols;
  for(icol=0;icol<50;icol++) {
    if(icol) scols+= ",";
    std::string s;
    inlib::sprintf(s,32,"%d",icol);
    scols += "int v"+s;
  }
  for(icol=0;icol<50;icol++) {
    scols+= ",";
    std::string s;
    inlib::sprintf(s,32,"%d",50+icol);
    scols += "float v"+s;
  }
  //std::cout << scols << std::endl;
  AIDA::ITuple* tuple = tupleFactory->create("tuple","tuple",scols);
  if(!tuple) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't create the \"tuple\" tuple." << std::endl;
    return;
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " \"tuple\" created." << std::endl;
  }

  for(count=0;count<10000;count++) {
    for(icol=0;icol<50;icol++) {
      tuple->fill(icol,100*count+icol);
    }
    for(icol=0;icol<50;icol++) {
      tuple->fill(50+icol,(float)(100*count+icol));
    }
    tuple->addRow();
  }

  /*
  // Slower : we fill twice a std::vector !
  std::vector<float> row(100);
  for(count=0;count<100000;count++) {
    for(icol=0;icol<50;icol++) {
      row[icol] = 100*count+icol;
    }
    for(icol=0;icol<50;icol++) {
      row[50+icol] = 100*count+icol;
    }
    tuple->fill(row);
  }
  */

  tree->ls();

  if(!tree->commit()) {
    std::cout << "BatchLab_tuple_test::main :"
              << " can't commit." << std::endl;
  } else {

    std::cout << "BatchLab_tuple_test::main :"
              << " Produce \"" << storeName << "\"..." << std::endl;
    tree->ls();

    std::cout << "BatchLab_tuple_test::main :"
              << " pwd \"" << tree->pwd() << "\"." << std::endl;
  }

  delete tupleFactory;

  delete treeFactory;

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Delete the AnalysisFactory..." << std::endl;
  }

  delete analysisFactory; // Will delete the trees.
}
//////////////////////////////////////////////////////////////////////////////
void read(
 const std::string& aStore
,int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::IAnalysisFactory* analysisFactory = AIDA_createAnalysisFactory();
  if(!analysisFactory) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get an AnalysisFactory." << std::endl;
    return;
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Have an AnalysisFactory." << std::endl;
  }

  AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
  if(!treeFactory) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get a TreeFactory." << std::endl;
    return;
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Have a TreeFactory." << std::endl;
  }

  int count;

  AIDA::ITree* tree = 0;
  std::string storeType;
  std::string storeName;
  storeName = "BatchLab_tuple_test";
  if(aStore=="root") {
    storeType = "root";
    storeName += ".root";
  } else if(aStore=="xml") {
    storeType = "xml";
    storeName += ".aida";
  }
  tree = treeFactory->create(storeName,storeType,true,false,"");
  if(!tree) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't get a Tree." << std::endl;
    return;
  }
  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " \"" << storeName << "\" created." << std::endl;
  }

  tree->ls();

  AIDA::IManagedObject* object = tree->find("tuple");
  if(!object) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Can't find object \"tuple\"." << std::endl;
    return;
  }

  AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
  if(!tuple) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Object \"tuple\" is not an AIDA::tuple." << std::endl;
    return;
  }

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " \"tuple\" found." << std::endl;
  }

  int nentries = tuple->rows();
  std::cout << "BatchLab_tuple_test::main :"
            << " " << nentries << " entries." 
            << std::endl;
  double fraction = 0.5;

  Rndm r;
  int icol;     
  float value;

  tuple->start();
  for(count=0;count<nentries;count++) {
    if (r.shoot()<fraction) {
      for(icol=0;icol<50;icol++) {
        tuple->getFloat(icol);
      }
      for(icol=0;icol<50;icol++) {
        value = tuple->getFloat(50+icol);
      }
      //printf("debug : %d %g\n",count,value);
    }
    tuple->next();
  }

  delete treeFactory;

  if(aVerboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Delete the AnalysisFactory..." << std::endl;
  }

  delete analysisFactory; // Will delete the trees.
}
//////////////////////////////////////////////////////////////////////////////
int main(
 int a_argc
,char** a_argv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int verboseLevel = 0;

  inlib::args args(a_argc,a_argv);
  bool help = args.is_arg("-h");
  if(help) {
    std::cout << "BatchLab_tuple_test::main :" << std::endl;
    std::cout << "Options : " << std::endl;
    std::cout << "  OS> BatchLab_tuple_test -h" << std::endl;
    std::cout << "   to have help." << std::endl;
    std::cout << "  OS> BatchLab_tuple_test -verbose" << std::endl;
    std::cout << "   to switch to verbose mode." << std::endl;
    std::cout << "  OS> BatchLab_tuple_test -store=[memory,xml,root]" << std::endl;
    std::cout << "   to store :"
              << "     in memory"
              << "     in an xml file"
              << "     in a file at the ROOT format with BatchLab objects."
              << std::endl;
    std::cout << "  OS> BatchLab_tuple_test -store=root -export=root" << std::endl;
    std::cout << "   to store :"
              << "     in a file at the ROOT format with ROOT objects."
              << std::endl;
    std::cout << "  OS> BatchLab_tuple_test -plotting" << std::endl;
    std::cout << "   to run without the plotting (batch mode)." << std::endl;
    std::cout << "  OS> BatchLab_tuple_test -plotter=[BatchLab,Anaphe]" << std::endl;
    std::cout << "   to use a plotter of another implementation." << std::endl;
    return 0;
  }

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  std::string store;
  if(!args.find("-store",store)) store = "memory";
  inlib::tolowercase(store);
  bool compress  = args.is_arg("-compress");

  if(verboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Choosen store is \"" << store << "\"." << std::endl;
  }

  write(store,compress,verboseLevel);
  //read(store,verboseLevel);

  // A simple check to see if object creation/destruction balance is correct :
  Lib::Debug::balance(std::cout);

  if(verboseLevel) {
    std::cout << "BatchLab_tuple_test::main :"
              << " Exiting..." << std::endl;
  }

  return 0;
}
