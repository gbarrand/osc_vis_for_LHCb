
// C++ conversion of the java/ChainedAndFilteredTuplesExample.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

#include <iostream>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) {
    std::cout << "AIDA not found." << std::endl;
    return EXIT_FAILURE;
  }

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  ITupleFactory* tf = af->createTupleFactory(*tree);
  if(!tf) return EXIT_FAILURE;
      
  Random r;
        
  // Create and fill 4 different ITuples
  ITuple* tup1 = tf->create("tup1","tup1","int n, double x");
  ITuple* tup2 = tf->create("tup2","tup2","int n, double x");
  ITuple* tup3 = tf->create("tup3","tup3","int n, double x");
  ITuple* tup4 = tf->create("tup4","tup4","int n, double x");
  if(!tup1) return EXIT_FAILURE;
  if(!tup2) return EXIT_FAILURE;
  if(!tup3) return EXIT_FAILURE;
  if(!tup4) return EXIT_FAILURE;
        
  for ( int i = 0; i < 20; i++ ) {
    tup1->fill(0,i);
    tup2->fill(0,i+20);
    tup3->fill(0,i+40);
    tup4->fill(0,i+60);

    tup1->fill(1, r.nextDouble()*10.);
    tup2->fill(1, r.nextDouble()*10.);
    tup3->fill(1, r.nextDouble()*10.);
    tup4->fill(1, r.nextDouble()*10.);

    tup1->addRow();
    tup2->addRow();
    tup3->addRow();
    tup4->addRow();
  }

  // Create a chain
  std::vector<ITuple*> set;
  set.push_back(tup1);
  set.push_back(tup2);
  set.push_back(tup3);
  set.push_back(tup4);
  ITuple* chain = tf->createChained("ChainedTuple", "New Chained Tuple", set);
  if(!chain) {
    std::cout << "can't create chained tuple." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "\n\nChained Tuple:" << std::endl;
  chain->start();
  while (chain->next()) 
    std::cout << chain->getInt(0) 
              << "\t" 
              << chain->getDouble(1) 
              << std::endl;

  // Create IFilter and filtered ITuple
  IFilter* filter = tf->createFilter("n>14 && n<46");
  if(!filter) return EXIT_FAILURE;
  filter->initialize(*chain);
  ITuple* filteredTuple = 
    tf->createFiltered("FilteredTuple", *chain, *filter);
  delete filter;
  if(!filteredTuple) return EXIT_FAILURE;

  std::cout << "\n\nFiltered Tuple:" << std::endl;
  filteredTuple->start();
  while (filteredTuple->next()) 
    std::cout << filteredTuple->getInt(0) 
              << "\t" 
              << filteredTuple->getDouble(1) 
              << std::endl;

  delete tf;

  delete af;

  std::cout << "exiting..." << std::endl;

  return EXIT_SUCCESS;
}
