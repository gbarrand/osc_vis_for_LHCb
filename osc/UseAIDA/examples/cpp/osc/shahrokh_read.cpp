#include "AIDA/AIDA.h"
using namespace AIDA;

using namespace std; //GB

#include <iostream>

int main()
{
  IAnalysisFactory *analysisFactory = AIDA_createAnalysisFactory();

  ITreeFactory *treeFactory = analysisFactory->createTreeFactory();

  ITree *tree = treeFactory->create("gaussian.aida","xml",true,false);
  delete treeFactory;
  if(!tree) {
    cout << "file gaussian.aida not found." << std::endl;
    cout << "run shahrokh_write first." << std::endl;
    return 0;
  }

  IManagedObject* managedObject= tree->find("/tuplePhoton");
  if(!managedObject) {
    cout << "can't find tuple /tuplePhoton." << std::endl;
    cout<<"this pointer is NULL:"<<endl;
  }

  if(managedObject)  cout<<"managedObject pointer is pointed to:"<<managedObject->name()<<endl;

  AIDA::ITuple *tuple = dynamic_cast<AIDA::ITuple*>(managedObject);
  if(!tuple) {
    std::cout << "can't cast IManagedObject to ITuple." << std::endl;
    return 0;
  }

  tuple->start();
  while ( tuple->next() ) {

    double r1 = tuple->getDouble(0); //r1
    double r2 = tuple->getDouble(1); //r2
    double r3 = tuple->getDouble(2); //r3

    std::cout << " r1 " << r1 
              << ", r2 = " << r2 
              << ", r3 = " << r3 
              << std::endl;
  }

  tree->close();  

  delete analysisFactory;

  return 0;
}
