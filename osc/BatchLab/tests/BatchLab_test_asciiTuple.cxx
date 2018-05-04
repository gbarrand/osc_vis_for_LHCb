
#include <BatchLab/Analysis.h>

//////////////////////////////////////////////////////////////////////////////
int main(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

 {//  Put everything in a block so that temporary objects
  // on the stack are deleted before the final memory decount is done. 

   // Will read data from stdin :
   BatchLab::AsciiStore asciiStore("stdin");
   BatchLab::AsciiTuple* tuple = 
     new BatchLab::AsciiTuple("","",&asciiStore,"v1 v2 v3 v4");
   tuple->print();
   
   delete tuple;
   //tupleManager->clearExtent();
   //delete tupleManager;
  }

  // A simple check to see if memory allocation/free balance is correct :
  Lib::Memory::dump();

  return 0;
}
