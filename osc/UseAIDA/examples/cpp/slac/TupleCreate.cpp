
// C++ conversion of the java/TupleCreate.java example.

#include <AIDA/AIDA.h>

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
      
  std::vector<std::string> columnNames;
  columnNames.push_back("iFlat = 0");
  columnNames.push_back(" fGauss = 3.");
  columnNames.push_back(" fFlat =-2.");
  std::vector<std::string> columnClasses;
  columnClasses.push_back("int");
  columnClasses.push_back("float");
  columnClasses.push_back("float");

  ITuple* tuple = tf->create( "tuple1", "tupleLabel1", columnNames, columnClasses, "");
  if(!tuple) return EXIT_FAILURE;
      
  std::string columnString("int iFlat=0, float fGauss = 3., fFlat=-2.");
  ITuple* tuple2 = tf->create( "tuple2", "tupleLabel2", columnString,"");
  if(!tuple2) return EXIT_FAILURE;
      
  delete tf;

  delete af;

  return EXIT_SUCCESS;
}
