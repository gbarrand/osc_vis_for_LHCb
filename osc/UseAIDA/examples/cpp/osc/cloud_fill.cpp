
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>

#include <inlib/args>
#include <inlib/sys/atime>
#include <inlib/random>

#include <iostream>

int main(int argc,char* argv[]){
  inlib::atime begin(inlib::atime::now());

  int verboseLevel = 0;

 {//  Put everything in a block so that temporary objects
  // on the stack are deleted before the final memory decount is done. 

  inlib::args args(argc,argv);
  bool help = args.is_arg("-h");
  if(help) {
    std::cout << "cloud_fill :" << std::endl;
    std::cout << "Options : " << std::endl;
    std::cout << "  OS> BatchLab_cloud -h" << std::endl;
    std::cout << "   to have help." << std::endl;
    std::cout << "  OS> BatchLab_cloud -verbose" << std::endl;
    std::cout << "   to switch to verbose mode." << std::endl;
    std::cout << "  OS> BatchLab_cloud -entries=<integer>" << std::endl;
    std::cout << "   give number of entries to fill the cloud." << std::endl;
    std::cout << "   Default is 1000000." << std::endl;
    return 0;
  }

  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verboseLevel = 1;
  }

  int entries = 1000000;
  //int entries = 10000000;
  std::string sentries;
  if(args.find("-entries",sentries)) {
    int ientries;
    if(inlib::to<int>(sentries,ientries)) entries = ientries;
  }

  AIDA::IAnalysisFactory* analysisFactory = AIDA_createAnalysisFactory();
  if(!analysisFactory) {
    std::cout << "cloud_fill :"
              << " Can't get an analysis factory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " Have an analysis factory." << std::endl;
  }

  AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
  if(!treeFactory) {
    std::cout << "cloud_fill :"
              << " Can't get a tree factory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " Have a tree factory." << std::endl;
  }

  // Creating a tree in memory
  AIDA::ITree* memoryTree = treeFactory->create();
  if(!memoryTree) {
    std::cout << "cloud_fill :"
              << " Can't get the default in memory tree." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " Have the default in memory tree." << std::endl;
  }

  // Get an histogram factory :
  AIDA::IHistogramFactory* histogramFactory = 
    analysisFactory->createHistogramFactory(*memoryTree);
  if(!histogramFactory) {
    std::cout << "cloud_fill :"
              << " Can't get a histogram factory." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " Have a histogram factory." << std::endl;
  }

  ////////////////////////////
  // Create some data :
  ////////////////////////////
  AIDA::ICloud2D* cloud2D = 
    histogramFactory->createCloud2D("cloud2D","Cloud2D");
  if(!cloud2D) {
    std::cout << "cloud_fill :"
              << " Can't create a cloud2D." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " \"cloud2D\" cloud created." << std::endl;
  }

  AIDA::ICloud3D* cloud3D = 
    histogramFactory->createCloud3D("cloud3D","Cloud3D");
  if(!cloud3D) {
    std::cout << "cloud_fill :"
              << " Can't create a cloud3D." << std::endl;
    return 1;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " \"cloud3D\" cloud created." << std::endl;
  }

  inlib::random::gauss rg;

 {inlib::atime beginFill = inlib::atime::now();
  for(int count=0;count<entries;count++)
    cloud2D->fill(rg.shoot(),rg.shoot(),1);
  inlib::atime endFill = inlib::atime::elapsed(beginFill);
  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " fill 2D elapsed time : " 
              << endFill.value() << " seconds."
              << std::endl;
  }}

 {inlib::atime beginFill(inlib::atime::now());
  for(int count=0;count<entries;count++)
    cloud3D->fill(rg.shoot(),rg.shoot(),rg.shoot(),1);
  inlib::atime endFill = inlib::atime::elapsed(beginFill);
  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " fill 3D elapsed time : " 
              << endFill.value() << " seconds."
              << std::endl;
  }}

  if(verboseLevel) {
    std::cout << "cloud_fill :" << std::endl;
    std::cout << " Cloud 2D : " << cloud2D->title() << std::endl;
    std::cout << " entries = " << cloud2D->entries() << std::endl;
    std::cout << " meanX = " << cloud2D->meanX() 
              << " rmsX = " << cloud2D->rmsX()
              << std::endl;
    std::cout << " meanY = " << cloud2D->meanY() 
              << " rmsY = " << cloud2D->rmsY()
              << std::endl;
    std::cout << std::endl;
    std::cout << " Cloud 3D : " << cloud3D->title() << std::endl;
    std::cout << " entries = " << cloud3D->entries() << std::endl;
    std::cout << " meanX = " << cloud3D->meanX() 
              << " rmsX = " << cloud3D->rmsX()
              << std::endl;
    std::cout << " meanY = " << cloud3D->meanY() 
              << " rmsY = " << cloud3D->rmsY()
              << std::endl;
    std::cout << " meanZ = " << cloud3D->meanZ() 
              << " rmsZ = " << cloud3D->rmsZ()
              << std::endl;
  }


  delete histogramFactory;
  delete treeFactory;
  delete analysisFactory;
  }

  inlib::atime end = inlib::atime::elapsed(begin);

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " elapsed time : " 
              << end.value() << " seconds."
              << std::endl;
  }

  if(verboseLevel) {
    std::cout << "cloud_fill :"
              << " Exiting..." << std::endl;
  }

  return 0;
}

