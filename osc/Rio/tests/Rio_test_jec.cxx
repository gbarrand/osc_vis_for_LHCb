/*
 * Example of writing a tuple of 10000000 rows with six columns
 * in a ROOT file by using Rio.
 */

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/Printer.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/File.h>

#include <Rio/Tree/TreeDictionary.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>

#include <math.h>   //M_PI
#ifndef M_PI
#define M_PI       3.1415926535897931160E0
#define M_PI_2     1.5707963267948965580E0  
#endif

#include <iostream>

//////////////////////////////////////////////////////////////////////////////
int main(
 int /*aArgc*/
,char** /*aArgv*/
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{

  Rio::Debug::checkByClass(true);

  std::ostream& out = std::cout;

 {/*
   * Instanciate the dictionary.
   */
  Rio::TreeDictionary dictionary(out);

  /*
   * Initialize the dictionary with default classes.
   */
  dictionary.initialize();

  /*
   * Create a file. The dictionary and a printer is given here.
   */
  Rio::IFile* file = 
    new Rio::File(dictionary,out,"Rio_jec.root",Rio::File::CREATE);
  //file->setVerboseLevel(1);
  if(!file->isOpen()) return 1;
  //file->setCompressionLevel(0);
  file->setCompressionLevel(1);
  int bufsz = 32000;
  //int bufsz = 320000;

  /*
   * Create some directory and put some data in them.
   */

  Rio::Tree* tree = new Rio::Tree(file->directory(),"jec_tree","JEC tree");
  /*
   * Rio::Tree instances will be deleted by the d_trees directory destructor.
   */ 
    
  /*
   * Create branches.
   */
  int i;
  tree->createBranch("i",&i,"i/I",bufsz);
  double p1,p2,p3,p4,p5,p6;
  tree->createBranch("p1",&p1,"p1/D",bufsz);
  tree->createBranch("p2",&p2,"p2/D",bufsz);
  tree->createBranch("p3",&p3,"p3/D",bufsz);
  tree->createBranch("p4",&p4,"p4/D",bufsz);
  tree->createBranch("p5",&p5,"p5/D",bufsz);
  tree->createBranch("p6",&p6,"p6/D",bufsz);

  /*
   * Fill the tree.
   */
  int nevent = 10000000;
  //int nevent = 1000000;
  //int nevent = 100000;
  for(int index=0;index<nevent;index++) {
    i = index+1;
    p1 =     M_PI * i;
    p2 = 2 * M_PI * i;
    p3 = 3 * M_PI * i;
    p4 = 4 * M_PI * i;
    p5 = 5 * M_PI * i;
    p6 = 6 * M_PI * i;
    int n;
    if(!tree->fill(n)) {
      out << " can't fill Tree " << std::endl;
      break;
    }
  }
    
  tree->autoSave();

  Rio::Printer printer(out);
  file->directory().print(printer);

  /*
   * Close the file. Close does not write !
   */ 
  file->close();

  delete file;

  }

 /*
  * To do a simple instance counting. It should produce zero message.
  */
  Rio::Debug::check(out);

  return 0;
}
