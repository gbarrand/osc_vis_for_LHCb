#include <AIDA/IConstants.h>

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 

#include <stdio.h>
int main(int aArgc,char** aArgv) {
  printf("This example is not available with AIDA-3.2.x.\n");
  return 0;
}

#else

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>
#include <AIDA/IPlotterFactory.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>

#include <AIDA/ITupleColumn.h>

#include <Lib/Arguments.h>
#include <Lib/Randoms.h>
#include <Lib/Printer.h>
#include <Lib/Debug.h>
#include <Lib/smanip.h>

#include <iostream>

static bool write(AIDA::IAnalysisFactory&,Lib::Arguments&);
static bool read(AIDA::IAnalysisFactory&,Lib::Arguments&);

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,char** aArgv
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::checkByClass(true);

  Lib::Arguments args(aArgc,aArgv);
  bool help = args.isAnArgument("-h");
  if(help) {
    std::cout << "BatchLab_exa_columns :" << std::endl;
    std::cout << "Options : " << std::endl;
    std::cout << "  OS> BatchLab_exa_columns -h" << std::endl;
    std::cout << "   to have help." << std::endl;
    std::cout << "  OS> BatchLab_exa_columns -verbose" << std::endl;
    std::cout << "   to switch to verbose mode." << std::endl;
    std::cout << "  OS> BatchLab_exa_columns "
              << "-store=[memory,xml,root,hbook]" 
              << std::endl;
    std::cout << "   to store :"
              << "     in memory"
              << "     in an xml file"
              << "     in a file at the ROOT format with BatchLab objects."
              << std::endl;
    std::cout << "  OS> BatchLab_exa_columns -store=root -export=root" 
              << std::endl;
    std::cout << "   to store :"
              << "     in a file at the ROOT format with ROOT objects."
              << std::endl;
    std::cout << "  OS> BatchLab_exa_columns -plotting" << std::endl;
    std::cout << "   to run without the plotting (batch mode)." << std::endl;
    std::cout << "  OS> BatchLab_exa_columns -plotter=[BatchLab,Anaphe]" 
              << std::endl;
    std::cout << "   to use a plotter of another implementation." << std::endl;
    return 0;
  }

  int verboseLevel = 0;

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't get an AnalysisFactory." << std::endl;
    return false;
  }

  if(args.isAnArgument("-read")) {
    read(*aida,args);
  } else {
    write(*aida,args);
  }

  delete aida;

  // A simple check to see if object creation/destruction balance is correct :
  Lib::Printer printer;
  Lib::Debug::balance(printer);

  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " Exiting..." << std::endl;
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
bool write(
 AIDA::IAnalysisFactory& aAIDA
,Lib::Arguments& args
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int verboseLevel = 0;

  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " Have an AnalysisFactory." << std::endl;
  }

  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't get a TreeFactory." << std::endl;
    return false;
  }

  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " Have a TreeFactory." << std::endl;
  }

  // Create some random engines :
  Lib::RandomGauss randomGauss;
  Lib::RandomBreitWigner randomBreitWigner;

  // Decide storage type and options from arguments :
  std::string store;
  if(!args.find("-store",store)) store = "memory";
  Lib::smanip::tolowercase(store);
  bool compress  = args.isAnArgument("-compress");
  std::string sexport;
  if(!args.find("-export",sexport)) sexport = "BatchLab";
  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " Choosen store type is \"" << store << "\"." << std::endl;
  }

  AIDA::ITree* tree = 0;
  if(store=="memory") {
    tree = treeFactory->create();
  } else {
    if(store=="root") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      opts += ";export="+sexport;
      tree = treeFactory->create
               ("BatchLab_exa_columns.root","root",false,true,opts);
    } else if(store=="xml") {
      std::string opts = "compress=";
      opts += compress ? "yes" : "no";
      tree = treeFactory->create
               ("BatchLab_exa_columns.aida","xml",false,true,opts);
    } else if(store=="hdf5") {
      std::string opts;
      tree = treeFactory->create
               ("BatchLab_exa_columns.hdf5","hdf5",false,true,opts);
    } else if((store=="hbook")||((store=="zebra"))) {
      tree = treeFactory->create
               ("BatchLab_exa_columns.hbook","hbook",false,true);
    } else {
      std::cout << "BatchLab_exa_columns :"
                << " store type " << store << " unkown." << std::endl;
      return false;
    }
  }
  if(!tree) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't create the BatchLab_exa_columns tree." 
              << std::endl;
    return false;
  }
  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " \"" << tree->storeName() << "\" created." << std::endl;
  }

  if(!tree->mkdir("tuples")) {
    std::cout << "BatchLab_exa_columns :"
              << " can't make directory \"tuples\"." << std::endl;
    return false;
  }

  if(!tree->cd("tuples")) {
    std::cout << "BatchLab_exa_columns :"
              << " can't cd to \"tuples\"." << std::endl;
    return false;
  }

  std::cout << "BatchLab_exa_columns :"
            << " pwd \"" << tree->pwd() << "\"." << std::endl;

  // Get a tuple factory :
  AIDA::ITupleFactory* tupleFactory = aAIDA.createTupleFactory(*tree);
  if(!tupleFactory) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't get a TupleFactory." << std::endl;
    return false;
  }

  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " Have a TupleFactory." << std::endl;
  }

  // Create some tuples :
  AIDA::ITuple* tuple = 
    tupleFactory->create("tuple","tuple",
                         "double v1, double v2,double[] av3");
  if(!tuple) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't create the \"tuple\" tuple." << std::endl;
    return false;
  }

  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " \"tuple\" created." << std::endl;
  }

  AIDA::ITupleColumn<double>* col_v1 = 
    AIDA::findTupleColumn<double>
      (*tuple,"v1","AIDA::ITupleColumn<double>");
  if(!col_v1) {
    std::cout << "BatchLab_exa_columns :"
              << " column v1 not found." << std::endl;
    return false;
  }
  AIDA::ITupleColumn<double>* col_v2 = 
    AIDA::findTupleColumn<double>
      (*tuple,"v2","AIDA::ITupleColumn<double>");
  if(!col_v2) {
    std::cout << "BatchLab_exa_columns :"
              << " column v2 not found." << std::endl;
    return false;
  }
  AIDA::ITupleColumn< std::vector<double> >* col_av3 = 
    AIDA::findTupleColumn< std::vector<double> >
      (*tuple,"av3","AIDA::ITupleColumn<std::vector<double>>");
  if(!col_av3) {
    std::cout << "BatchLab_exa_columns :"
              << " column av3 not found." << std::endl;
    return false;
  }

  double v2;
  col_v2->bind(&v2);

 {for(int count=0;count<10000;count++) {
    col_v1->fill(randomGauss.shoot());
    v2 = randomBreitWigner.shoot();
    std::vector<double> v3;
    v3.push_back(count);
    v3.push_back(count+1);
    v3.push_back(count+2);
    col_av3->fill(v3);
    tuple->addRow();
  }}

  tree->cd("..");

  if(!tree->commit()) {
    std::cout << "BatchLab_exa_columns :"
              << " can't commit." << std::endl;
    return false;
  }

  std::cout << "BatchLab_exa_columns :"
            << " Produce \"" << tree->storeName() << "\"..." << std::endl;
  tree->ls(".",true);

  std::cout << "BatchLab_exa_columns :"
            << " pwd \"" << tree->pwd() << "\"." << std::endl;

  delete tupleFactory;
  if(store!="memory") delete tree;

  delete treeFactory;

  if(verboseLevel) {
    std::cout << "BatchLab_exa_columns :"
              << " Delete the AnalysisFactory..." << std::endl;
  }

  return true;
}

#include <AIDA/IManagedObject.h>

//////////////////////////////////////////////////////////////////////////////
bool read(
 AIDA::IAnalysisFactory& aAIDA
,Lib::Arguments& args
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't get a TreeFactory." << std::endl;
    return false;
  }

  std::string store;
  if(!args.find("-store",store)) store = "memory";
  Lib::smanip::tolowercase(store);

  std::string opts;
  AIDA::ITree* tree = 
    treeFactory->create
               ("BatchLab_exa_columns."+store,store,true,false,opts);
  if(!tree) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't create the BatchLab_exa_columns tree." 
              << std::endl;
    return false;
  }

  // Find the tuple :
  AIDA::IManagedObject* mo = tree->find("/tuples/tuple");
  if(!mo) {
    std::cout << "BatchLab_exa_columns :"
              << " Can't find the \"tuple\" tuple." << std::endl;
    return false;
  }
  AIDA::ITuple *tuple = dynamic_cast<AIDA::ITuple*>(mo);
  if(!tuple) {
    std::cout << "BatchLab_exa_columns :"
              << "can't cast an AIDA::IManagedObject to an AIDA::ITuple." 
              << std::endl;
    return false;
  }

  AIDA::ITupleColumn<double>* col_v1 = 
    AIDA::findTupleColumn<double>
      (*tuple,"v1","AIDA::ITupleColumn<double>");
  if(!col_v1) {
    std::cout << "BatchLab_exa_columns :"
              << " column v1 not found." << std::endl;
    return false;
  }
  AIDA::ITupleColumn<double>* col_v2 = 
    AIDA::findTupleColumn<double>
      (*tuple,"v2","AIDA::ITupleColumn<double>");
  if(!col_v2) {
    std::cout << "BatchLab_exa_columns :"
              << " column v2 not found." << std::endl;
    return false;
  }
  AIDA::ITupleColumn< std::vector<double> >* col_av3 = 
    AIDA::findTupleColumn< std::vector<double> >
      (*tuple,"av3","AIDA::ITupleColumn<std::vector<double>>");
  if(!col_av3) {
    std::cout << "BatchLab_exa_columns :"
              << " column av3 not found." << std::endl;
    return false;
  }

  tuple->start();
  while(tuple->next()) {
    double v1 = col_v1->value();
    double v2 = col_v2->value();
    std::vector<double> av3 = col_av3->value();

    std::cout << " v1 " << v1
              << ", v2 = " << v2
              << ", av3.size = " << av3.size()
              << ", av3[0] = " << av3[0]
              << std::endl;
  }

  delete tree;
  delete treeFactory;

  return true;
}

#endif
