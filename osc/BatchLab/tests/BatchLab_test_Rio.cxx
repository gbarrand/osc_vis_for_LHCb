
#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>

#define DEBUG_TREE

#ifdef DEBUG_TREE
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>
#include <AIDA/IEvaluator.h>
#include <AIDA/IFilter.h>
#endif

//#include <Lib/Debug.h>
#include <iostream>

int main (int aArgc,char** aArgv) {
 {if(aArgc<=1) {
    std::cout << "Give a root file as first argument." << std::endl;
  } else {
    AIDA::IAnalysisFactory* analysisFactory = AIDA_createAnalysisFactory(); 
    if(analysisFactory) {
      AIDA::ITreeFactory* treeFactory = analysisFactory->createTreeFactory();
      if(treeFactory) {

        AIDA::ITree* rootTree = 
          treeFactory->create(aArgv[1],"ROOT",true,true);

        rootTree->ls();
      
        // Ok over analysis.root 
        //  (produced by BatchLab/examples/cpp/BatchLab_analysis.cpp program).
        rootTree->cd("histograms");
        rootTree->ls();
        rootTree->cd("..");
        rootTree->cd("tuples");
        rootTree->ls();

#ifdef DEBUG_TREE
        AIDA::ITupleFactory* tf = 
          analysisFactory->createTupleFactory(*rootTree); 
        AIDA::IHistogramFactory* hf = 
          analysisFactory->createHistogramFactory(*rootTree);

        AIDA::ITuple* tuple1 = tf->create("tuple1","","");
        AIDA::IHistogram1D* t1_v2 = 
          hf->createHistogram1D("t1_v2","tuple1/v2",10,0,10);
        AIDA::IEvaluator* evaluator = tf->createEvaluator("v2");
        AIDA::IFilter* filter = tf->createFilter("");
        if(evaluator && filter) tuple1->project(*t1_v2,*evaluator,*filter);
        delete evaluator;
        delete filter;
        
        delete hf;
        delete tf;
#endif

        delete treeFactory;
      }
      delete analysisFactory;
    }
  }}

 //Lib::Debug::balance(printer);

  return 0;
}
