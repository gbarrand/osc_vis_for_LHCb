
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

template <class T,class LeafT>
T read_column(std::ostream& aOut,Rio::Tree* tree,const std::string& aName) {
  Rio::BaseLeaf* leaf = tree->leaf(aName);
  if(!leaf) {
    aOut << " Leaf " << aName << " not found." << std::endl;
    return 0;
  }
  aOut << " Leaf " << aName << " found." << std::endl;
  LeafT* lf = Rio::cast<LeafT>(*leaf);
  if(!lf) {
    aOut << " Leaf " << aName << " not a LeafDouble." << std::endl;
    return 0;
  }
  int number = (int)tree->entries();
  Rio::Branch& branch = leaf->branch();
  T last = 0;
  for(int index=0;index<number;index++) {
    int n;
    if(!branch.findEntry(index,n)) {
      aOut << "can't get entry " << index << "." << std::endl;
      return 0;
    }
    last = lf->value();
    aOut << " " << index << " " << last << std::endl;
  } 
  return last;
}

int main(int,char**) {

  Rio::Debug::checkByClass(true);

  std::ostream& out = std::cout;

 {Rio::TreeDictionary dictionary(out);

  dictionary.initialize();

  Rio::IFile* file = 
    new Rio::File(dictionary,out,"pawdemo.root",Rio::File::READ);
  //file->setVerboseLevel(1);
  if(!file->isOpen()) return 1;

  Rio::Printer printer(out);
  file->directory().print(printer);

  Rio::IDirectory* d_STAFF = file->directory().findDirectory("STAFF");
  if(!d_STAFF) {
    out << " Directory STAFF not found." << std::endl;
    return 1;
  }

  Rio::IObject* obj = d_STAFF->find("h10");

  Rio::Tree* tree = obj ? Rio::cast<Rio::Tree>(*obj) : 0;
  if(!tree) {
    out << " Object \"h10\" not found." << std::endl;
    return 1;
  }

  int number = (int)tree->entries();
  out << " Object \"jec_tree\" found (" << number 
      << " entries)." << std::endl;
        
  int index;
  out << "Head :" << std::endl;
  for(index=0;index<number;index++) tree->show(index);
  /*
  */
  //read_column<float,Rio::LeafFloat>(out,tree,"X");
        
  file->close();

  delete file;

  }

  Rio::Debug::check(out);

  return 0;
}
