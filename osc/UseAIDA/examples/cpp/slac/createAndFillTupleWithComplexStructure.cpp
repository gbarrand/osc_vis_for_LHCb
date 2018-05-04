
// C++ conversion of the java/createAndFillTupleWithComplexStructure.java example.

#include <AIDA/AIDA.h>
#include <AIDA/ITupleEntry.h> //FIXME : 3.2.1 : not in AIDA.h.

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  //ITree* tree = trf->create("testTupleWithComplexStructure.aida","xml",false,false,"compress=no");
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  ITupleFactory* tf = af->createTupleFactory(*tree);
  if(!tf) return EXIT_FAILURE;
      
  std::string columnString = "int event =0,  tracks =0, ITuple momentums  = { double px = .2, py = 3.,pz = 0., ITuple hits = {int x,y,z} }, float ipx, ipy, ipz";
    
  ITuple* tuple = tf->create("tuple", "label",columnString,"");
  if(!tuple) return EXIT_FAILURE;

  delete tf;      
    
  Random r;
  int events = 100;
    
  for ( int i=0; i<events; i++ ) {
    tuple->fill(0, i);
    
    int tracks = r.nextInt(10);
    tuple->fill(1,tracks);
      
    ITuple* momentum = tuple->getTuple( 2 );
    if(!momentum) return EXIT_FAILURE;
      
    for ( int j = 0; j<tracks; j++ ) {
      momentum->fill(0,r.nextGaussian());
      momentum->fill(1,r.nextGaussian());
      momentum->fill(2,r.nextGaussian());
        
      int nHits = r.nextInt(20);
        
      ITuple* hits = momentum->getTuple( 3 );
      if(!hits) return EXIT_FAILURE;
      for ( int k = 0; k<nHits; k++ ) {
        hits->fill(0,r.nextInt(40));
        hits->fill(1,r.nextInt(40));
        hits->fill(2,r.nextInt(40));
        hits->addRow();
      } // end of hits loop
      momentum->addRow();
    }// end of tracks loop
      
    tuple->fill(3,(float)r.nextGaussian());
    tuple->fill(4,(float)r.nextGaussian());
    tuple->fill(5,(float)r.nextGaussian());
    tuple->addRow();
  }//end of loop over events


  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  IHistogram1D* pxHist = hf->createHistogram1D("pxHist",100,-5,5);

  delete hf;
  if(!pxHist) return EXIT_FAILURE;
      
  tuple->start();
  while ( tuple->next() ) {
    ITupleEntry* momTupleEntry = (ITupleEntry*)tuple->getObject(2);
    if(!momTupleEntry) return EXIT_FAILURE;
    ITuple* momTuple = dynamic_cast<ITuple*>(momTupleEntry);
    if(!momTuple) return EXIT_FAILURE;

    momTuple->start();
    while ( momTuple->next() ) 
      pxHist->fill( momTuple->getDouble(0) );
  }
    
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(1,1,0);
    plotter->region(0)->plot(*pxHist);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}
    
  delete af;

  return EXIT_SUCCESS;
}
