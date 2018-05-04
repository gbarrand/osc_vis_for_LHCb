/*
 * Here we have a simple example showing how to :
 * - declare a user class (MyObject class) suitable for doing IO with Rio.
 * - declare the user class (with the MyObjectClass class) 
 *   to the user dictionary (MyDictionary class).
 * - open a file in write then in read mode using the user dictionary.
 * - write and read some data in the file.
 * - organize the file by using the Rio::Directory.
 * 
 * Stored data are instances of the MyObject class and also
 * simple data types (int, float) declared to a Rio::Tree.
 * 
 * To use this program :
 *    OS> Rio_test.exe -create 
 *      to create the Rio_test.root file.
 *    OS> Rio_test.exe -read
 *      to read the Rio_test.root file.
 */

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/BaseClass.h>
#include <Rio/Core/Printer.h>
#include <Rio/Core/File.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Debug.h>

#include <Rio/Tree/TreeDictionary.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/LeafObject.h>
#include <Rio/Tree/LeafString.h>
#include <Rio/Tree/Leaf.h>

#include <iostream>
#include <stdio.h> //sprintf
#include <stdlib.h>

/* 
 * A class to declare to the IO system. 
 */
class MyObject : public Rio::Named {
  /* 
   * A cpp macro to help setting the "introspection"
   * for the MyObject class.
   */
  RIO_META_HEADER(MyObject)
public: //Rio::IObject:
  /*
   * A cast method that permits to avoid good part of the
   * dynamic_cast (and then be more efficient).
   */
  virtual void* cast(const Rio::IClass& aClass) const {
    if(&aClass==&fClass) 
      return Rio_SCast(MyObject);
    else 
      return Named::cast(aClass);
  }
  /*
   * The famous streamer. Here you say what you want to store
   * and retreive of your objects.
   */
  virtual bool stream(Rio::IBuffer& aBuffer) {
    if (aBuffer.isReading()) {
      short v;
      if(!aBuffer.readVersion(v)) return false;
      if(!Named::stream(aBuffer)) return false;
      if(!aBuffer.read(fMyInt)) return false;
      if(fVerbose) 
        std::cout << "MyObject::stream : " << this << " name : " << fName
                  << " read : " << fMyInt << std::endl;
    } else {
      if(fVerbose)
        std::cout << "MyObject::stream : " << this
                  << " write : " << fMyInt << std::endl;
      if(!aBuffer.writeVersion(fClass.version())) return false;
      if(!Named::stream(aBuffer)) return false;
      if(!aBuffer.write(fMyInt)) return false;
    }
    return true;
  }
public:
  MyObject(const Rio::IDictionary& aDictionary,
           const std::string& aName = "",bool aVerbose = false)
  :Rio::Named(aDictionary,aName,"My object")
  ,fClass(MyObject::is(aDictionary)),fVerbose(aVerbose),fMyInt(314) {
    if(fVerbose) 
      std::cout << "MyObject::MyObject : called for " 
                << this << " ." << std::endl;
  }
  virtual ~MyObject(){
    if(fVerbose) 
      std::cout << "MyObject::~MyObject : called for " 
                << this << " name : " << fName << std::endl;
  }
  int myInt () const { return fMyInt;}
  void setMyInt(int aInt){ fMyInt = aInt;}
private:
  bool fVerbose;
  /* 
   * The data !
   */
  int fMyInt;
};

/*
 * A cpp macro to help setting the "introspection"
 * for the MyObject class.
 */
RIO_META_SOURCE(MyObject)

/*
 * A class to describe the MyObject class. It consists
 * mainly to a factory. One instance of the MyObjectClass
 * must be declared to your dictionary.
 */
class MyObjectClass : public Rio::BaseClass {
public: //Rio::IClass
  virtual Rio::IObject* create(const Rio::Arguments& aArgs) {
    Rio::IDirectory* directory = 
      (Rio::IDirectory*)Rio::Args_findDirectory(aArgs);
    if(!directory) {
      fOut << "MyObjectClass::create : no directory given." << std::endl;
      return 0;
    }
    const Rio::IDictionary& dico = directory->file().dictionary();
    Rio::IObject* object = 
      new MyObject(dico,"",directory->file().verboseLevel()?true:false);
    directory->appendObject(object);
    return object;
  }
  virtual unsigned int checkSum() const { return 0;}
public:
  /*
   * The declared class name is "TFoo" (it could be "MyObject").
   * The declared class version is zero. The class version
   * could be used in the MyObject::streamer for further evolution
   * of the MyObject class (handling of the "schema evolution").
   */
  MyObjectClass(std::ostream& aOut):
  Rio::BaseClass("MyObject","TFoo",0),fOut(aOut){}
  virtual ~MyObjectClass(){}
private:
  std::ostream& fOut;
};

/*
 * Create the dictionary. Your dictionary should, in some way or another
 * (inheritance or composition) give access to an instance of the 
 * Rio::TreeDictionary. The simplest is to inherit.
 */
class MyDictionary : public Rio::TreeDictionary {
public: //Rio::IDictionary
  virtual bool initialize() {
    if(!::Rio::TreeDictionary::initialize()) return false;
    /*
     * Create and declare an instance of the MyObject class
     * describer to the dictionary.
     */
    Rio::IClass* cl = new MyObjectClass(fOut);
    addClass(cl);

    return true;
  }
public:
  MyDictionary(std::ostream& aOut,bool aVerbose)
    :Rio::TreeDictionary(aOut){
  }
  virtual ~MyDictionary(){}
};


int main(int aArgc,char* aArgv[]) {

  Rio::Debug::checkByClass(true);

  /*
   * Create a printer (concreete instance of the Rio::IPrinter)
   * that will be declared to the system. It will be used 
   * for error messages, debug informations, etc...
   */

  std::ostream& out = std::cout;
  Rio::Printer printer(out);

 {std::string arg1 = (aArgc>=2) ? aArgv[1] : "";
  std::string arg2 = (aArgc>=3) ? aArgv[2] : "";

  /*
   * Instanciate the dictionary.
   */
  bool verbose = (arg2=="-verbose" ?true:false);
  MyDictionary myDictionary(out,verbose);
  myDictionary.initialize();

  if(arg1=="-create") {

    /*
     * Create a file. The dictionary and a printer is given here.
     */
    Rio::IFile* file = 
      new Rio::File(myDictionary,out,"Rio_test.root",Rio::File::CREATE);
    if(arg2=="-verbose") file->setVerboseLevel(1);

    /*
     * Create some directory and put some data in them.
     */
    Rio::IDirectory* d_histos = file->directory().mkdir("histos","histograms");
    Rio::IDirectory* d_ecal = d_histos->mkdir("ecal");
    if(d_ecal) {
      d_ecal->appendObject(new MyObject(myDictionary,"myObject",verbose));
      d_ecal->appendObject(new MyObject(myDictionary,"myObject1",verbose));
      d_ecal->appendObject(new MyObject(myDictionary,"myObject2",verbose));
      /*
       * MyObject instances will be deleted by the d_ecal directory destructor.
       */ 
    }
    d_histos->mkdir("hcal");
    Rio::IDirectory* d_trees = file->directory().mkdir("trees");

    Rio::Tree* myTree = new Rio::Tree(*d_trees,"myTree","My tree");
    /*
     * Rio::Tree instances will be deleted by the d_trees directory destructor.
     */ 
    
    /*
     * Some data to put in the tree :
     */
    static struct {
      int v1;
      /* WIN32 : inserting a double involves that all fields
       * are mapped to the size of the double ! then the branch
       * computation of each field address is out !
       * (It was true with ROOT too...)
       *double v2;
       */
      float v3;
    } st;
    double v4;
    short v5;
    char v6;
    char v7[64];

    /*
     * Create branches.
     */
    //tree->createBranch("st",&st,"v1/I:v2/D:v3/F");
    myTree->createBranch("st",&st,"v1/I:v3/F");
    myTree->createBranch("double",&v4,"v4/D");
    myTree->createBranch("short",&v5,"v5/S");
    myTree->createBranch("char",&v6,"v6/B");
    myTree->createBranch("string",v7,"v7/C");

    /**
     * We have to delete the below object ourself.
     */
    MyObject* myObject = new MyObject(myDictionary,"myObject3",verbose);
    /**
     * Take care, we have to pass the address of a IObject* 
     * (and not of a MyObject*). (Simple no ?).
     */
    Rio::IObject* obj = static_cast<Rio::IObject*>(myObject);
    myTree->createBranch("myObjects",MyObject::is(myDictionary),&obj);

    /*
     * Fill the tree.
     */
    unsigned char cmax = 0xFF;
    unsigned short smax = 0xFFFF;
    //unsigned int nevt = 50000;
    unsigned int nevt = 100000; //1.6 M
    //unsigned int nevt = 1000000; //16 M
    //unsigned int nevt = 10*1000000; //160 M
    //unsigned int nevt = 1000*1000000; //16 G
    for(int count=0;count<nevt;count++) {
      st.v1 = count;
      //st.v2 = 2 * count;
      st.v3 = 3.0F * count;
      v4 = 3 * count;
      v5 = count%smax;
      v6 = count%cmax;
      ::sprintf(v7,"hello_%d",count);

      myObject->setMyInt(5 * count); //May exceed an int.

      int n;
      if(!myTree->fill(n)) {
        out << " can't fill Tree " << std::endl;
        break;
      }
    }
    
    // file->directory().remove("histos;*");

    file->directory().print(printer);

    /*
     * Write data in the file :
     */
    int n;
    if(!file->write(n)) {
      out << " can't write file." << std::endl;
    }

    /*
     * Close the file. Close does not write !
     */ 
    file->close();

    delete file;

    delete myObject;

  } else if(arg1=="-recreate") {
    Rio::File file(myDictionary,out,"Rio_test.root",Rio::File::RECREATE);
    if(!file.isOpen()) return EXIT_FAILURE;
    file.close();
  } else if(arg1=="-update") {
    Rio::File file(myDictionary,out,"Rio_test.root",Rio::File::UPDATE);
    if(!file.isOpen()) return EXIT_FAILURE;
    file.close();
  } else if(arg1=="-read") {

    /*
     * Open a file for reading. Here too, the dictionary and a printer
     * is given. The "create" method of the various IClass instances
     * declared to the dictionary will be used to create back instances
     * of Rio::Directory, Rio::Tree and also of the user MyObject class.
     */
    Rio::IFile* file = 
      new Rio::File(myDictionary,out,"Rio_test.root",Rio::File::READ);
    if(!file->isOpen()) return EXIT_FAILURE;
    if(arg2=="-verbose") file->setVerboseLevel(1);
    file->directory().print(printer);

    /* 
     * Get back directories and data.
     */
    Rio::IDirectory* d_histos = file->directory().findDirectory("histos");
    if(!d_histos) {
      out << " Directory \"histos\" not found." << std::endl;
    } else {
      //d_histos->ls();
      Rio::IDirectory* d_ecal = d_histos->findDirectory("ecal");
      if(!d_ecal) {
        out << " Directory \"ecal\" not found." << std::endl;
      } else {
        //d_ecal->ls();
        /*
         * Returned object is managed by the file, we do not
         * have to delete it ourself.
         */
        Rio::IObject* obj = d_ecal->find("myObject;1");
        MyObject* myObject = obj ? Rio::cast<MyObject>(*obj) : 0;
        if(!myObject) {
          out << " Object \"myObject\" not found." << std::endl;
        } else {
          if(myObject->myInt()==314) 
            out << " myObject.fMyInt " << myObject->myInt() 
                << " : Ouai !!!!." << std::endl;
          else
            out << " myObject.fMyInt " << myObject->myInt()
                << " : should be 314 ; beurk !!!!." << std::endl;
        }
       {Rio::IObject* obj = d_ecal->find("myObject2");
        MyObject* myObject2 = obj ? Rio::cast<MyObject>(*obj) : 0;
        if(!myObject2) {
          out << " Object \"myObject2\" not found." << std::endl;
        }}
      }
    }

    Rio::IDirectory* d_trees = file->directory().findDirectory("trees");
    if(!d_trees) {
      out << " Directory \"trees\" not found." << std::endl;
    } else {
      Rio::IObject* obj = d_trees->find("myTree");
      Rio::Tree* tree = obj ? Rio::cast<Rio::Tree>(*obj) : 0;
      if(!tree) {
        out << " Object \"myTree\" not found." << std::endl;
      } else {
        int number = (int)tree->entries();
        out << " Object \"myTree\" found (" << number 
            << " entries)." << std::endl;
        
        int index;
        out << "Head :" << std::endl;
        for(index=0;index<10;index++) tree->show(index);
        
        out << "Tail :" << std::endl;
        for(index=number-10;index<number;index++) tree->show(index);
        
        Rio::BaseLeaf* leaf_v3 = tree->leaf("v3");
        if(!leaf_v3) {
          out << " Leaf \"v3\" not found." << std::endl;
        } else {
          out << " Leaf \"v3\" found." << std::endl;
          Rio::LeafFloat* lf = Rio::cast<Rio::LeafFloat>(*leaf_v3);
          if(!lf) {
            out << " Leaf \"v3\" not a LeafFloat." << std::endl;
          } else {
            Rio::Branch& branch_v3 = leaf_v3->branch();
            int n;
            for(index=0;index<5;index++) {
              if(!branch_v3.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              float val = lf->value();
              out << "v3 " << index << " = " << val 
                  << " (" << 3*index << " expected)." << std::endl;
            }
            for(index=number-5;index<number;index++) {
              if(!branch_v3.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              float val = lf->value();
              out << "v3 " << index << " = " << val 
                  << " (" << 3*index << " expected)." << std::endl;
            }
          }
        }
        
        Rio::BaseLeaf* leaf_v4 = tree->leaf("v4");
        if(!leaf_v4) {
          out << " Leaf \"v4\" not found." << std::endl;
        } else {
          out << " Leaf \"v4\" found." << std::endl;
          Rio::LeafDouble* lf = Rio::cast<Rio::LeafDouble>(*leaf_v4);
          if(!lf) {
            out << " Leaf \"v4\" not a LeafDouble." << std::endl;
          } else {
            Rio::Branch& branch_v4 = leaf_v4->branch();
            int n;
            for(index=0;index<10;index++) {
              if(!branch_v4.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              double val = lf->value();
              out << "v4 " << index << " = " << val 
                  << " (" << 3*index << " expected)." << std::endl;
            }
            for(index=number-10;index<number;index++) {
              if(!branch_v4.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              double val = lf->value();
              out << "v4 " << index << " = " << val 
                  << " (" << 3*index << " expected)." << std::endl;
            }
          }
        }
        
        Rio::BaseLeaf* leaf_v7 = tree->leaf("v7");
        if(!leaf_v7) {
          out << " Leaf \"v7\" not found." << std::endl;
        } else {
          out << " Leaf \"v7\" found." << std::endl;
          Rio::LeafString* ls = Rio::cast<Rio::LeafString>(*leaf_v7);
          if(!ls) {
            out << " Leaf \"v7\" not a LeafString." << std::endl;
          } else {
            Rio::Branch& branch_v7 = leaf_v7->branch();
            int n;
            for(index=0;index<10;index++) {
              if(!branch_v7.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              std::string val = ls->value();
              out << "v7 " << index << " = " << val 
                  << " (hello_" << index << " expected)." << std::endl;
            }
            for(index=number-10;index<number;index++) {
              if(!branch_v7.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              std::string val = ls->value();
              out << "v7 " << index << " = " << val 
                  << " (hello_" << index << " expected)." << std::endl;
            }
          }
        }
        
        Rio::BaseLeaf* leaf_myObjects = tree->leaf("myObjects");
        if(!leaf_myObjects) {
          out << " Leaf \"myObjects\" not found." << std::endl;
        } else {
          Rio::LeafObject* lo = Rio::cast<Rio::LeafObject>(*leaf_myObjects);
          if(!lo) {
            out << " Leaf \"myObjects\" not a LeafObject." << std::endl;
          } else {
            out << " Leaf \"myObjects\" found." << std::endl;
            Rio::Branch& branch_myObjects = leaf_myObjects->branch();
            int n;
            for(index=0;index<10;index++) {
              if(!branch_myObjects.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              Rio::IObject* obj = lo->object();
              if(!obj) {
                out << " get null object." << std::endl;
              } else {
                MyObject* myObj = Rio::cast<MyObject>(*obj);
                if(!myObj) {
                  out << " object not a MyObject." << std::endl;
                } else {
                  out << "obj " << index << " = " << myObj->myInt() 
                      << " (" << 5*index << " expected)." << std::endl;
                }
              }
            }
            for(index=number-10;index<number;index++) {
              if(!branch_myObjects.findEntry(index,n)) {
                out << "can't get entry " << index << "." << std::endl;
                break;
              }
              Rio::IObject* obj = lo->object();
              if(!obj) {
                out << " get null object." << std::endl;
              } else {
                MyObject* myObj = Rio::cast<MyObject>(*obj);
                if(!myObj) {
                  out << " object not a MyObject." << std::endl;
                } else {
                  out << "obj " << index << " = " << myObj->myInt() 
                      << " (" << 5*index << " expected)." << std::endl;
                }
              }
            }
          }
        }
        
      }
      // file->directory().remove("histos");
    }
    file->directory().print(printer);

    /*
     * Close the file :
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
