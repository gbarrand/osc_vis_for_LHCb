
// C++ conversion of the java/ScanAndContour.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>
#include <math.h>

#include <iostream>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  //ITree* tree = trf->create("storeForFittingExample");
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  IHistogram1D* hist = hf->createHistogram1D("hist","Test Histogram",100,-5,5);
  delete hf;
  if(!hist) return EXIT_FAILURE;
      
  Random r;
  for (int i=0; i<10000; i++) {
    double x = r.nextGaussian();
    hist->fill(x);
  }

  // Do Fit
  IFitFactory* ff = af->createFitFactory(); 
  if(!ff) return EXIT_FAILURE;
  IFitter* fitter = ff->createFitter("Chi2","minuit");
  IFitData* fitData = ff->createFitData();
  delete ff;
  if(!fitter) return EXIT_FAILURE;
  if(!fitData) return EXIT_FAILURE;

  fitData->create1DConnection(*hist);

  IFitResult* fitResult = fitter->fit(*fitData,"g");
  if(!fitResult) { //GB
    std::cout << "Can't fit data."<< std::endl;
    return EXIT_FAILURE;
  }

  int meanIndex  = fitResult->fittedFunction().indexOfParameter("mean");
  double meanVal = fitResult->fittedParameters()[meanIndex];
  double meanErr = fitResult->errors()[meanIndex];
  IDataPointSet* meanScan = 
    fitter->createScan1D(*fitData, fitResult->fittedFunction(),"mean",20, meanVal-3*meanErr, meanVal+3*meanErr);
  if(!meanScan) return EXIT_FAILURE;

  //int sigmaIndex  = fitResult->fittedFunction().indexOfParameter("sigma");
  //double sigmaVal = fitResult->fittedParameters()[sigmaIndex];
  //double sigmaErr = fitResult->errors()[sigmaIndex];
  IDataPointSet* oneSigmaContour = fitter->createContour(*fitData, *fitResult, "mean", "sigma", 10, 1);
  IDataPointSet* twoSigmaContour = fitter->createContour(*fitData, *fitResult, "mean", "sigma", 10, 2);
  if(!oneSigmaContour) return EXIT_FAILURE;
  if(!twoSigmaContour) return EXIT_FAILURE;

  delete fitter;

  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->destroyRegions();
    plotter->createRegion(0,0,.66,1)->plot(*hist);
    plotter->region(0)->plot(fitResult->fittedFunction());
    plotter->createRegion(.66,0,.33,.5)->plot( *meanScan );
    plotter->createRegion(.66,.5,.33,.5)->plot( *twoSigmaContour );
    plotter->region(2)->plot( *oneSigmaContour );
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  //FIXME : GB : are they managed ?
  //delete meanScan;
  //delte oneSigmaContour;
  //delete twoSigmaContour;

  delete fitResult;
  delete fitData;

  delete af;

  return EXIT_SUCCESS;
}
