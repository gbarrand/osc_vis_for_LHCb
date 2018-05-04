
// C++ conversion of the java/CreateAndFitDataPointSet.java example.

#include <AIDA/AIDA.h>

using namespace AIDA;

#include <stdlib.h>

#define array_size(aArray) (sizeof(aArray)/sizeof(double))

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  // Create a tree loading the AIDA objects stored in an AIDA file.
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IDataPointSetFactory* dpsf = af->createDataPointSetFactory(*tree);
  if(!dpsf) return EXIT_FAILURE;

  // Create a two dimensional IDataPointSet.
  IDataPointSet* dataPointSet = dpsf->create("dataPointSet","two dimensional IDataPointSet",2);
  delete dpsf;
  if(!dataPointSet) return EXIT_FAILURE;

  // Fill the two dimensional IDataPointSet
  double yVals2D[] = { 0.12, 0.22, 0.35, 0.42, 0.54 , 0.61 };
  double yErrP2D[] = { 0.01, 0.02, 0.03, 0.03, 0.04 , 0.04 };
  double yErrM2D[] = { 0.02, 0.02, 0.02, 0.04, 0.06 , 0.05 };
  double xVals2D[] = { 1.5, 2.6, 3.4, 4.6, 5.5 , 6.4 };
  double xErrP2D[] = { 0.5, 0.5, 0.4, 0.4, 0.5 , 0.5 };

  for ( unsigned int i = 0; i<array_size(yVals2D); i++ ) {
    dataPointSet->addPoint();
    dataPointSet->point(i)->coordinate(0)->setValue( xVals2D[i] );
    dataPointSet->point(i)->coordinate(0)->setErrorPlus( xErrP2D[i] );
    dataPointSet->point(i)->coordinate(1)->setValue( yVals2D[i] );
    dataPointSet->point(i)->coordinate(1)->setErrorPlus( yErrP2D[i] );
    dataPointSet->point(i)->coordinate(1)->setErrorMinus( yErrM2D[i] );
  }


  IFunctionFactory* funcf  = af->createFunctionFactory(*tree);
  if(!funcf) return EXIT_FAILURE;
  //Create a 1d second order polynomial
  IFunction* p2 = funcf->createFunctionFromScript("p2", 1, "a+b*x[0]+c*x[0]*x[0]", "a,b,c","",""); 
  delete funcf;
  if(!p2) return EXIT_FAILURE;
      
  // Do Fit
  IFitFactory* ff = af->createFitFactory(); 
  if(!ff) return EXIT_FAILURE;
  IFitter* fitter = ff->createFitter("Chi2","uncmin");
  IFitData* data = ff->createFitData();
  delete ff;
  if(!fitter) return EXIT_FAILURE;
  if(!data) return EXIT_FAILURE;
  data->create1DConnection(*dataPointSet,0,1);

  IFitResult* result = fitter->fit(*data,*p2);
  delete fitter;
  if(!result) return EXIT_FAILURE;
      
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    // Display the results
    plotter->createRegions();
    plotter->region(0)->plot( *dataPointSet );
    plotter->region(0)->plot( result->fittedFunction() );
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete result;

  delete af;

  return EXIT_SUCCESS;
}
