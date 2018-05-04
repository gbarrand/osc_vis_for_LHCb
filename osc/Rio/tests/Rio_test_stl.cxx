
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/Printer.h>
#include <Rio/Core/File.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Array_double.h>
//#include <Rio/Core/smanip.h>

#include <Rio/Tree/TreeDictionary.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/LeafElement.h>
#include <Rio/Tree/Branch.h>

#include <iostream>

#include <cstdio>
#include <cstdlib>

int main(int aArgc,char* aArgv[]) {

  Rio::Debug::checkByClass(true);

  std::ostream& out = std::cout;

 {std::string arg1 = (aArgc>=2) ? aArgv[1] : "";
  std::string arg2 = (aArgc>=3) ? aArgv[2] : "";

  /*
   * Instanciate the dictionary.
   */
  bool verbose = (arg2=="-verbose" ?true:false);

  Rio::TreeDictionary myDictionary(out);
  myDictionary.initialize();

  if(arg1=="-create") {

    /*
     * Create a file. The dictionary and a printer is given here.
     */
    Rio::IFile* file = 
      new Rio::File(myDictionary,out,
                    "Rio_test_stl.root",Rio::File::CREATE);
    if(arg2=="-verbose") file->setVerboseLevel(1);

    Rio::Tree* myTree = new Rio::Tree(file->directory(),"myTree","My tree");
    
    /////////////////////////////////////////
    // First branch with a Rio::Array_double :
    /////////////////////////////////////////
    Rio::IClass* Rio_Array_double_class = 
      myDictionary.findClass("Rio::Array_double");
    if(!Rio_Array_double_class) {
      out << " can't find class for Rio::Array_double" << std::endl;
      return 1;
    }
    Rio::Array_double* arrayD = new Rio::Array_double(myDictionary);
    myTree->createBranchElement
      ("arrayD",*Rio_Array_double_class,arrayD);

    /////////////////////////////////////////////
    // Second branch with a std::vector<double> :
    /////////////////////////////////////////////
    Rio::IClass* std_vector_double_class = 
      myDictionary.findClass("std::vector<double>");
    if(!std_vector_double_class) {
      out << " can't find class for std::vector<double>" << std::endl;
      return 1;
    }
    std::vector<double>* array_double = new std::vector<double>;
    myTree->createBranchElement
      ("array_double",*std_vector_double_class,array_double);

/*
    /////////////////////////////////////////////
    // Third branch with a std::vector<double> :
    /////////////////////////////////////////////
    //std::string* std_string = new std::string;
    //myTree->createBranch("std_string",
    //  (void*)std_string->c_str(),"std_string/C");
    char std_string[256];
    myTree->createBranch("std_string",(void*)std_string,"std_string/C");
*/

    for(int count=0;count<50000;count++) {

      arrayD->resize(3);
      double* vec = arrayD->array();
      vec[0] = count;
      vec[1] = count+1;
      vec[2] = count+2;

      array_double->resize(5);
      (*array_double)[0] = count;
      (*array_double)[1] = count+1;
      (*array_double)[2] = count+2;
      (*array_double)[3] = count+3;
      (*array_double)[4] = count+4;

      //Rio::smanip::printf(*std_string,64,"item_%d",count);
      //sprintf(std_string,"item_%d",count);

      int n;
      if(!myTree->fill(n)) {
        out << " can't fill Tree " << std::endl;
        break;
      }
    }

    delete arrayD;
    delete array_double;
    //delete std_string;
    
    Rio::Printer printer(out);
    file->directory().print(printer);

    /*
     * Write data in the file->
     */
    int n;
    if(!file->write(n)) {
      out << " can't write file->" << std::endl;
    }

    /*
     * Close the file. (It does not write !).
     */ 
    file->close();

    delete file;

  } else if(arg1=="-read") {

    Rio::IFile* file = 
      new Rio::File(myDictionary,out,
                    "Rio_test_stl.root",Rio::File::READ);
    if(!file->isOpen()) return EXIT_FAILURE;
    if(arg2=="-verbose") file->setVerboseLevel(1);
    Rio::Printer printer(out);
    file->directory().print(printer);

    Rio::IObject* obj = file->directory().find("myTree");
    Rio::Tree* tree = obj ? Rio::cast<Rio::Tree>(*obj) : 0;
    if(!tree) {
      out << " Object \"myTree\" not found." << std::endl;
      return 1;
    }

    int number = (int)tree->entries();
    out << " Object \"myTree\" found (" << number 
        << " entries)." << std::endl;
        
    /////////////////////////////////////////
    // First branch with a Rio::Array_double :
    /////////////////////////////////////////
    Rio::Branch* branch_arrayD = tree->findBranch("arrayD");
    if(!branch_arrayD) {
      out << " Branch \"arrayD\" not found." << std::endl;
      return 1;
    }
    Rio::Array_double* arrayD = new Rio::Array_double(myDictionary);
    if(!branch_arrayD->setAddress(arrayD)) {
      out << "can't set address." << std::endl;
      return 1;
    }

    /////////////////////////////////////////////
    // Second branch with a std::vector<double> :
    /////////////////////////////////////////////
    Rio::Branch* branch_array_double = tree->findBranch("array_double");
    if(!branch_array_double) {
      out << " Branch \"array_double\" not found." << std::endl;
      return 1;
    }
    std::vector<double>* array_double = new std::vector<double>;
    if(!branch_array_double->setAddress(array_double)) {
      out << "can't set address." << std::endl;
      return 1;
    }

/*
    /////////////////////////////////////////////
    // Third branch with a std::string :
    /////////////////////////////////////////////
    Rio::Branch* branch_std_string = tree->findBranch("std_string");
    if(!branch_std_string) {
      out << " Branch \"std_double\" not found." << std::endl;
      return 1;
    }
    std::string* std_string = new std::string;
    if(!branch_std_string->setAddress((void*)std_string->c_str())) {
      out << "can't set address." << std::endl;
      return 1;
    }
*/

   {for(int index=0;index<10;index++) {
      int n;
      if(!branch_arrayD->findEntry(index,n)) {
        out << "can't get entry " << index << "." << std::endl;
        break;
      }
      printf("arrayD : %d %lu : %d %g\n",
	     index,(unsigned long)arrayD,
             (int)arrayD->size(),arrayD->array()[2]);
      //printf("arrayD : %d %lu : %d\n",
        //index,arrayD,arrayD->size());
    }}

   {for(int index=0;index<10;index++) {
      int n;
      if(!branch_array_double->findEntry(index,n)) {
        out << "can't get entry " << index << "." << std::endl;
        break;
      }
      //printf("array_double : %d %lu : <double> : %d\n",index,
      //  array_double,array_double->size());
      printf("array_double : %d %lu : <double> : %d %g\n",index,
	     (unsigned long)array_double,
             (int)array_double->size(),(*array_double)[2]);
    }}

/*
   {for(int index=0;index<10;index++) {
      int n;
      if(!branch_std_string->findEntry(index,n)) {
        out << "can't get entry " << index << "." << std::endl;
        break;
      }
      printf("std_string : %d %lu : %s\n",index,
        std_string,std_string->c_str());
    }}
    delete std_string;
*/

    delete array_double;
    delete arrayD;

    /*
     * Close the file->
     */
    file->close();

    delete file;
  } else {
    out << "Bad argument : [-create, -read] [-verbose] expected." << std::endl;
  }

  }

 /*
  * To do a simple instance counting. It should produce zero message.
  */
  Rio::Debug::check(out);

  return 0;
}


