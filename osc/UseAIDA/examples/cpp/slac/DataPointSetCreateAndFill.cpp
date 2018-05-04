
// C++ conversion of the java/DataPointSetCreateAndFill.java example.

#include <AIDA/AIDA.h>

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

  IDataPointSetFactory* dpsf = af->createDataPointSetFactory(*tree);
  if(!dpsf) return EXIT_FAILURE;

  // Create a one dimensional IDataPointSet.
  IDataPointSet* dps1D = dpsf->create("dps1D","one dimensional IDataPointSet",1);
  if(!dps1D) return EXIT_FAILURE;
      
  // Fill the one dimensional IDataPointSet
  double yVals1D[] = { 0.32, 0.45, 0.36, 0.29, 0.34 };
  double yErrP1D[] = { 0.06, 0.07, 0.03, 0.07, 0.04 };

  int number = 5;
  int i;
  for ( i = 0; i<number; i++ ) {
    dps1D->addPoint();
    dps1D->point(i)->coordinate(0)->setValue( yVals1D[i] );
    dps1D->point(i)->coordinate(0)->setErrorPlus( yErrP1D[i] );
  }

  // Create a two dimensional IDataPointSet.
  IDataPointSet* dps2D = dpsf->create("dps2D","two dimensional IDataPointSet",2);
  delete dpsf;
  if(!dps2D) return EXIT_FAILURE;

  // Fill the two dimensional IDataPointSet
  double yVals2D[] = { 0.12, 0.22, 0.35, 0.42, 0.54 , 0.61 };
  double yErrP2D[] = { 0.01, 0.02, 0.03, 0.03, 0.04 , 0.04 };
  double yErrM2D[] = { 0.02, 0.02, 0.02, 0.04, 0.06 , 0.05 };
  double xVals2D[] = { 1.5, 2.6, 3.4, 4.6, 5.5 , 6.4 };
  double xErrP2D[] = { 0.5, 0.5, 0.4, 0.4, 0.5 , 0.5 };

  number = 6;
  for ( i = 0; i<number; i++ ) {
    dps2D->addPoint();
    dps2D->point(i)->coordinate(0)->setValue( xVals2D[i] );
    dps2D->point(i)->coordinate(0)->setErrorPlus( xErrP2D[i] );
    dps2D->point(i)->coordinate(1)->setValue( yVals2D[i] );
    dps2D->point(i)->coordinate(1)->setErrorPlus( yErrP2D[i] );
    dps2D->point(i)->coordinate(1)->setErrorMinus( yErrM2D[i] );
  }
      
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  //IPlotter* plotter = pf->create("Plot IDataPointSets");
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(2,1,0);
    plotter->region(0)->plot(*dps1D);
    plotter->region(1)->plot(*dps2D);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete af;

  return EXIT_SUCCESS;
}
