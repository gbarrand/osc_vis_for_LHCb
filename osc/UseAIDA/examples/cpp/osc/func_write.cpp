
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IFunctionFactory.h>
#include <AIDA/IFunction.h>

#include <inlib/args>
#include <inlib/sys/atime>

#include <iostream>

int main(int aArgc,char** aArgv){
  inlib::atime begin(inlib::atime::now());

  int verboseLevel = 0;

 {//  Put everything in a block so that temporary objects
  // on the stack are deleted before the final memory decount is done. 

  inlib::args args(aArgc,aArgv);
  bool help = args.is_arg("-h");
  if(help) {
    std::cout << "func_write :" << std::endl;
    std::cout << "Options : " << std::endl;
    std::cout << "  OS> func_write -h" << std::endl;
    std::cout << "   to have help." << std::endl;
    std::cout << "  OS> func_write -verbose" << std::endl;
    std::cout << "   to switch to verbose mode." << std::endl;
    return 0;
  }

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "func_write :"
              << " Can't get an analysis factory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " Have an analysis factory." << std::endl;
  }

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << "func_write :"
              << " Can't get a tree factory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " Have a tree factory." << std::endl;
  }

  // Creating a XML tree in memory
  AIDA::ITree* tree = 
    treeFactory->create("func_write.aida","xml",false,true,"");
  if(!tree) {
    std::cout << "func_write :"
              << " Can't get the default in memory tree." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " Have the default in memory tree." << std::endl;
  }

  tree->mkdir("/functions");

  // Get an function factory :
  AIDA::IFunctionFactory* functionFactory = 
    aida->createFunctionFactory(*tree);
  if(!functionFactory) {
    std::cout << "func_write :"
              << " Can't get a function factory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " Have a function factory." << std::endl;
  }

  ////////////////////////////
  // Create some functions :
  ////////////////////////////
 {AIDA::IFunction* function = 
    functionFactory->createFunctionByName("/functions/gaussian","g");
  if(!function) {
    std::cout << "func_write :"
              << " Can't create a g function." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " gaussian function created." << std::endl;
  }

  function->setParameter("amplitude",10);
  function->setParameter("mean",5);
  function->setParameter("sigma",2);
 } //End gaussian.


 {AIDA::IFunction* function = 
    functionFactory->createFunctionFromScript
      ("/functions/scripted",2,"gauss(x[0],H,M,W)+pol2(x[1],A,B,C)","","","");
  if(!function) {
    std::cout << "func_write :"
              << " Can't create a scripted function." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " scripted function created." << std::endl;
  }

  std::vector<double> params;
  params.push_back(400); // H
  params.push_back(0);   // M
  params.push_back(4);   // W
  params.push_back(1);   // A
  params.push_back(0);   // B
  params.push_back(-1);  // C
  function->setParameters(params);
 } //End scripted.


  tree->commit();

  delete functionFactory;
  delete treeFactory;
  delete aida;
  }

  inlib::atime end = inlib::atime::elapsed(begin);

  if(verboseLevel) {
    std::cout << "func_write :"
              << " elapsed time : " 
              << end.value() << " seconds."
              << std::endl;
  }

  if(verboseLevel) {
    std::cout << "func_write :"
              << " Exiting..." << std::endl;
  }

  return 0;
}

