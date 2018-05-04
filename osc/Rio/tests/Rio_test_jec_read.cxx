/*
 * Example of reading a tuple of 10000000 rows with six columns
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
#include <Rio/Tree/Leaf.h>

#include <iostream>

double read_column(std::ostream& aOut,Rio::Tree* tree,const std::string& aName) {
  Rio::BaseLeaf* leaf = tree->leaf(aName);
  if(!leaf) {
    aOut << " Leaf " << aName << " not found." << std::endl;
    return 0;
  }
  aOut << " Leaf " << aName << " found." << std::endl;
  Rio::LeafDouble* lf = Rio::cast<Rio::LeafDouble>(*leaf);
  if(!lf) {
    aOut << " Leaf " << aName << " not a LeafDouble." << std::endl;
    return 0;
  }
  int number = (int)tree->entries();
  Rio::Branch& branch = leaf->branch();
  double last = 0;
  for(int index=0;index<number;index++) {
    int n;
    if(!branch.findEntry(index,n)) {
      aOut << "can't get entry " << index << "." << std::endl;
      return 0;
    }
    last = lf->value();
  } 
  return last;
}
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
   * Open the file. The dictionary and a printer is given here.
   */
  Rio::IFile* file = 
    new Rio::File(dictionary,out,"Rio_jec.root",Rio::File::READ);
  //file->setVerboseLevel(1);
  if(!file->isOpen()) return 1;

  Rio::Printer printer(out);
  file->directory().print(printer);

  Rio::IObject* obj = file->directory().find("jec_tree");
  Rio::Tree* tree = obj ? Rio::cast<Rio::Tree>(*obj) : 0;
  if(!tree) {
    out << " Object \"jec_tree\" not found." << std::endl;
    return 1;
  }

  int number = (int)tree->entries();
  out << " Object \"jec_tree\" found (" << number 
      << " entries)." << std::endl;
        
  //int index;
  //out << "Head :" << std::endl;
  //for(index=0;index<10;index++) tree->show(index);
        
  //out << "Tail :" << std::endl;
  //for(index=number-10;index<number;index++) tree->show(index);

  // Read a column :        
 {Rio::BaseLeaf* leaf = tree->leaf("i");
  if(!leaf) {
    out << " Leaf \"i\" not found." << std::endl;
    return 1;
  }
  out << " Leaf \"i\" found." << std::endl;
  Rio::LeafInteger* lf = Rio::cast<Rio::LeafInteger>(*leaf);
  if(!lf) {
    out << " Leaf \"i\" not a LeafFloat." << std::endl;
    return 1;
  }
  Rio::Branch& branch = leaf->branch();
  int last = 0;
  for(int index=0;index<number;index++) {
    int n;
    if(!branch.findEntry(index,n)) {
      out << "can't get entry " << index << "." << std::endl;
      return 1;
    }
    last = lf->value();
  }
  out << " Leaf \"i\" last value " 
      << last << " (10000000 expected)."  
      << std::endl;
  }
        
  double last;
  last = read_column(out,tree,"p1");
  out << " Leaf p1 last value " 
      << last << " ( 31415926 expected)."  << std::endl;

  last = read_column(out,tree,"p2");
  out << " Leaf p2 last value " 
      << last << " ( " << 2 * 31415926 << " expected)."  << std::endl;

  last = read_column(out,tree,"p3");
  out << " Leaf p3 last value " 
      << last << " ( " << 3 * 31415926 << " expected)."  << std::endl;

  last = read_column(out,tree,"p4");
  out << " Leaf p4 last value " 
      << last << " ( " << 4 * 31415926 << " expected)."  << std::endl;

  last = read_column(out,tree,"p5");
  out << " Leaf p5 last value " 
      << last << " ( " << 5 * 31415926 << " expected)."  << std::endl;

  last = read_column(out,tree,"p6");
  out << " Leaf p6 last value " 
      << last << " ( " << 6 * 31415926 << " expected)."  << std::endl;

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
