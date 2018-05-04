
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IKey.h>
#include <Rio/Interfaces/IObject.h>

#include <Rio/Core/Args.h>
#include <Rio/Core/File.h>
#include <Rio/Core/Printer.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Key.h>

#include <Rio/Tree/TreeDictionary.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BaseLeaf.h>

#include <stdlib.h>

#include <iostream>

static void Rio_Directory_print(Rio::IDirectory&,Rio::IPrinter&);

//////////////////////////////////////////////////////////////////////////////
int main(
 int aArgc
,char* aArgv[]
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::ostream& out = std::cout;

 {Rio::Args args(aArgc,aArgv);

  if(args.number()<2) {
    out << "Usage :" << std::endl;
    out << "  OS> Rio_rio [-verbose,-recursive] <file name>" << std::endl;
    return EXIT_FAILURE;
  }
  std::string fileName;
  if(!args.last(fileName)) {
    out << "Usage :" << std::endl;
    out << "  OS> Rio_rio [-verbose] <file name>" << std::endl;
    return EXIT_FAILURE;
  }
  bool verbose = args.isAnArgument("-verbose");
  bool recursive  = args.isAnArgument("-recursive");

  Rio::TreeDictionary dico(out);
  dico.initialize();

  if(verbose) dico.out();
  
  Rio::File file(dico,out,fileName,Rio::File::READ);
  if(!file.isOpen()) return EXIT_FAILURE;
  if(verbose) file.setVerboseLevel(1);
  Rio::Printer printer(out);
  if(recursive) 
    Rio_Directory_print(file.directory(),printer);
  else 
    file.directory().print(printer);
  
  file.close();}

  Rio::Debug::check(out);

  return 0;
}

#define Rio_tab "\t"

//////////////////////////////////////////////////////////////////////////////
static void Rio_Directory_print(
 Rio::IDirectory& aDirectory
,Rio::IPrinter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
// A recursive directory printing.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aPrinter.indent();
  std::ostream& out = aPrinter.out();

  out << "TDirectory" << "*" << Rio_tab << Rio_tab << aDirectory.name() 
      << Rio_tab << aDirectory.title() << std::endl;
  aPrinter.increment();

 {std::vector<Rio::IObject*> objs = aDirectory.objects();
  std::vector<Rio::IObject*>::const_iterator it;
  for(it=objs.begin();it!=objs.end();++it) {
    aPrinter.indent();
    (*it)->out(out);
  }}
  
 {std::vector<Rio::IKey*> keys = aDirectory.keys();
  std::vector<Rio::IKey*>::const_iterator it;
  for(it=keys.begin();it!=keys.end();++it) {
    aPrinter.indent();
    out << "KEY: " << (*it)->className() 
        << Rio_tab << (*it)->name() << ";" << (*it)->cycle() 
        << Rio_tab << (*it)->title() 
        << Rio_tab << "klen = " << (*it)->keyLength()
        << Rio_tab << "objsize = " << (*it)->objectSize()
        << Rio_tab << "nbytes = " << (*it)->numberOfBytes()
        << std::endl;
    if((*it)->className()=="TDirectory") {
      Rio::IDirectory* dir = aDirectory.findDirectory((*it)->name());
      if(dir) Rio_Directory_print(*dir,aPrinter);
    } else if((*it)->className()=="TTree") {
      Rio::IObject* obj = aDirectory.find((*it)->name());
      if(!obj) {
        out << " Can't get Rio::Tree in file." << std::endl;
      } else {
        Rio::Tree* tree = Rio::cast<Rio::Tree>(*obj);
        if(!tree) {
          out << " Can't get Rio::Tree in file." << std::endl;
        } else {
          aPrinter.increment();
          aPrinter.indent();
          out << "Tree : " << tree->name()
              << " entries = " << (int)tree->entries()
              << std::endl;
  
          std::vector<Rio::Branch*>& branches = tree->branches();
          std::vector<Rio::Branch*>::const_iterator itb;
          for(itb=branches.begin();itb!=branches.end();++itb) {
            aPrinter.increment();
            aPrinter.indent();
            out << "Branch : " << (*itb)->name()
                << std::endl;
  
            std::vector<Rio::BaseLeaf*>& leaves = (*itb)->leaves();
            std::vector<Rio::BaseLeaf*>::const_iterator itl;
            for(itl=leaves.begin();itl!=leaves.end();++itl) {
              aPrinter.increment();
              aPrinter.indent();
              out << "BaseLeaf : " << (*itl)->name()
                  << " class = " << (*itl)->isA().name()
                  << std::endl;
              aPrinter.decrement();
            }
  
            aPrinter.decrement();
          }
          aPrinter.decrement();
        }
      }
    }
  }}
  
  aPrinter.decrement();
}
