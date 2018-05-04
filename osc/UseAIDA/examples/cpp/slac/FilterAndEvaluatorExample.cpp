
// C++ conversion of the java/FilterAndEvaluatorExample.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

#include <iostream>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  ITupleFactory* tf = af->createTupleFactory(*tree);
  if(!tf) return EXIT_FAILURE;
      
  Random r;

  ITuple* tuple = tf->create("TupleTreeName","Title: Test Tuple", "int n, double x, double y, double z");

  for ( int i = 0; i < 30; i++ ) {
    double v1 = r.nextDouble()*10.;
    double v2 = r.nextDouble()*10.;
    double v3 = 5.3*v1+2.1*v1*v2 + 10.7;
    tuple->fill(0,i);
    tuple->fill(1,v1);
    tuple->fill(2,v2);
    tuple->fill(3,v3);
    tuple->addRow();
  }

  // Create IFilter and initialize it to this ITuple
  IFilter* filter = tf->createFilter("5.3*x+2.1*y*x + 10.7 > 50.");
  if(!filter) return EXIT_FAILURE;
  filter->initialize(*tuple);

  // Create IEvaluator and initialize it to this ITuple
  IEvaluator* evaluator = tf->createEvaluator("(1.5*x*x-5.2*y*x + 4*sin(y))/85");
  if(!evaluator) return EXIT_FAILURE;
  evaluator->initialize(*tuple);

  delete tf;

  // Example 1: Filter ITuple Data
  tuple->start();
  while (tuple->next()) {
    if (filter->accept()) {
      std::cout << tuple->getInt(0) 
                << "\t Row passed, evaluate: "
                << evaluator->evaluateDouble()
                << std::endl;
    } else {
      std::cout << tuple->getInt(0)
                << "\t Row failed, do not evaluate"
                << std::endl;
    }                            
  }

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;

  // Use IHistogramFactory to create two empty histograms
  IHistogram1D* h1 = hf->createHistogram1D("hist-1","Use All Data",50,-10,10);
  IHistogram1D* h2 = hf->createHistogram1D("hist-2","Use Filtered Data",50,-10, 10);
  delete hf;
  if(!h1) return EXIT_FAILURE;
  if(!h2) return EXIT_FAILURE;
 
  // Example 2: Fill histograms from ITuple
  tuple->project(*h1, *evaluator);
  tuple->project(*h2, *evaluator, *filter);

  // Plot histograms
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(2,1,0);
    plotter->region(0)->plot(*h1);
    plotter->region(1)->plot(*h2);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete evaluator;
  delete filter;

  delete af;

  return EXIT_SUCCESS;
}
