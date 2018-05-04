/*
 * Example of writing a tuple in an hippodraw ascii file.
 */


#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>

#include <inlib/random>

#include <iostream>

int main(int,char**){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "hippo_write :"
              << " Can't get an AnalysisFactory." << std::endl;
    return 1;
  }

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << "hippo_write :"
              << " Can't get a TreeFactory." << std::endl;
    return 1;
  }

  AIDA::ITree* tree = treeFactory->create("hippo.tnt","hippodraw",false,true);
  if(!tree) {
    std::cout << "hippo_write :"
              << " Can't get a Tree." << std::endl;
    return 1;
  }

  if(!tree->mkdir("tuples")) {
    std::cout << "hippo_write :"
              << " can't make directory \"tuples\"." << std::endl;
    return 1;
  }

  if(!tree->cd("tuples")) {
    std::cout << "hippo_write :"
              << " can't cd to \"tuples\"." << std::endl;
    return 1;
  }

  // Get a tuple factory :
  AIDA::ITupleFactory* tupleFactory = aida->createTupleFactory(*tree);
  if(!tupleFactory) {
    std::cout << "hippo_write :"
              << " Can't get a TupleFactory." << std::endl;
    return 1;
  }


  AIDA::ITuple* tuple = tupleFactory->create("tuple","my hippo tuple",
                                             "double rgauss, double rbw");
  if(!tuple) {
    std::cout << "hippo_write :"
              << " Can't create the \"tuple\" tuple." << std::endl;
    return 1;
  }

  inlib::random::gauss rg;
  inlib::random::bw rbw;
  for(int count=0;count<1000;count++) {
    tuple->fill(0,rg.shoot());
    tuple->fill(1,rbw.shoot());
    tuple->addRow();
  }

  if(!tree->commit()) {
    std::cout << "hippo_write :"
              << " can't commit." << std::endl;
    return 1;
  }

  delete tupleFactory;
  delete treeFactory;
  delete aida;

  return 0;
}

