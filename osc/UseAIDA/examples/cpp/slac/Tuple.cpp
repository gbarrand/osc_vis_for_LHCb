
// C++ conversion of the java/Tuple.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

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
      
  std::vector<std::string> columnNames;
  columnNames.push_back("iFlat = 0");
  columnNames.push_back(" fGauss = 3.");
  columnNames.push_back(" fFlat =-2.");
  std::vector<std::string> columnClasses;
  columnClasses.push_back("int");
  columnClasses.push_back("float");
  columnClasses.push_back("float");
      
  ITuple* tuple = tf->create( "tuple", "tupleLabel", columnNames, columnClasses, "");
  if(!tuple) return EXIT_FAILURE;
      
  Random r;
  for (int i=0; i<100000; i++) {
    tuple->fill(0, r.nextInt(20) );
    tuple->fill(1, (float)r.nextGaussian() );
    tuple->fill(2, r.nextFloat() );
    tuple->addRow();
  }
      
  int colG = tuple->findColumn("fGauss");
  int colF = tuple->findColumn("fFlat");
  int colI = tuple->findColumn("iFlat");
      
  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  //IHistogram1D* h1dI = hf->createHistogram1D("h1dI",50,tuple->columnMin(colI),tuple->columnMax(colI));
  IHistogram1D* h1dI = hf->createHistogram1D("h1dI",20,0,20);
  IHistogram1D* h1dF = hf->createHistogram1D("h1dF",50,tuple->columnMin(colF),tuple->columnMax(colF));
  IHistogram1D* h1dG = hf->createHistogram1D("h1dG",50,tuple->columnMin(colG),tuple->columnMax(colG));
  IHistogram2D* h2d = hf->createHistogram2D("h2d",50,tuple->columnMin(colG),tuple->columnMax(colG),50,tuple->columnMin(colF),tuple->columnMax(colF));
  delete hf;
      
  tuple->start();
  while ( tuple->next() ) {
    h1dI->fill( tuple->getInt(colI) );
    h1dF->fill( tuple->getFloat(colF) );
    h1dG->fill( tuple->getFloat(colG) );
    h2d->fill( tuple->getFloat(colG), tuple->getFloat(colF) );
  }
      
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(2,2,0);
    plotter->region(0)->plot(*h1dI);
    plotter->region(1)->plot(*h1dF);
    plotter->region(2)->plot(*h1dG);
    plotter->region(3)->plot(*h2d);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete tf;

  delete af;

  return EXIT_SUCCESS;
}
