
// C++ conversion of the java/SimpleFit.java example.

#include <AIDA/AIDA.h>

#include "Random.h"

using namespace AIDA;

#include <stdlib.h>

#include <iostream>

#include <math.h>
#ifndef M_PI
#define M_PI       3.1415926535897931160E0
#define M_PI_2     1.5707963267948965580E0  
#endif

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
  IHistogram1D* gaussHist = hf->createHistogram1D("gaussHist","Gaussian Histogram",100,-5,5);
  ICloud1D* gaussCloud = hf->createCloud1D("gaussCloud","Gaussian Cloud");
  if(!gaussHist) return EXIT_FAILURE;
  if(!gaussCloud) return EXIT_FAILURE;
      
  ITupleFactory* tf = af->createTupleFactory(*tree);
  if(!tf) return EXIT_FAILURE;
  ITuple* tuple = tf->create("tuple","Tuple Example","double gaussDistr");
  if(!tuple) return EXIT_FAILURE;
  
  Random r;
  for (int i=0; i<10000; i++) {
    double x = r.nextGaussian();
    gaussHist->fill(x);
    gaussCloud->fill(x);
    tuple->fill(0,x);
    tuple->addRow();
  }


  // Do Fit
  IFitFactory* ff = af->createFitFactory(); 
  if(!ff) return EXIT_FAILURE;
  IFitter* fitter = ff->createFitter("Chi2","minuit");
  if(!fitter) { //GB
    std::cout << "Can't create fitter."<< std::endl;
    return EXIT_FAILURE;
  }

  // Chi2 fit with Minuit
  IFitResult* minuitChi2Fit = fitter->fit(*gaussHist,"g");
  if(!minuitChi2Fit) { //GB
    std::cout << "Can't fit gaussHist."<< std::endl;
    return EXIT_FAILURE;
  }    

  // Least Squares fit with Minuit
  fitter->setFitMethod("LS");
  IFitResult* minuitLeastSquaresFit = fitter->fit(*gaussHist,"g");      
  if(!minuitLeastSquaresFit) { //GB
    std::cout << "Can't fit gaussHist LS."<< std::endl;
    return EXIT_FAILURE;
  }

  // Binned Maximum Likelihood fit with Minuit
  fitter->setFitMethod("binnedMaximumLikelihood");
  IFitResult* minuitBinnedMaxLikelihoodFit = fitter->fit(*gaussHist,"g"); 
  if(!minuitBinnedMaxLikelihoodFit) { //GB
    std::cout << "Can't fit gaussHist binnedMaximumLikelihood."<< std::endl;
    return EXIT_FAILURE;
  }

  // Unbinned Maximum Likelihood fit with Uncmin
  fitter->setEngine("uncmin");
  fitter->setFitMethod("uml");
  IFitResult* uncminUMLFitToCloud = fitter->fit(*gaussCloud,"g");
  if(!uncminUMLFitToCloud) { //GB
    std::cout << "Can't fit gaussCloud uncmin uml."<< std::endl;
    return EXIT_FAILURE;
  }

  IFitData* fitData = ff->createFitData();
  if(!fitData) { //GB
    std::cout << "Can't create fit data."<< std::endl;
    return EXIT_FAILURE;
  }
  std::vector<std::string> gaussColumn;
  gaussColumn.push_back("gaussDistr");
  fitData->createConnection(*tuple,gaussColumn);

  IFitResult* uncminUMLFitToTuple = fitter->fit(*fitData,"g");
  if(!uncminUMLFitToTuple) { //GB
    std::cout << "Can't fit fitData."<< std::endl;
    return EXIT_FAILURE;
  }

  IHistogram1D* gaussProj  = hf->createHistogram1D("gaussProj","Gaussian Histogram Projected from ITuple",100,-5,5);
  if(!gaussProj) return EXIT_FAILURE;
  IEvaluator* eval = tf->createEvaluator("gaussDistr"); 
  if(!eval) return EXIT_FAILURE;
  tuple->project( *gaussProj, *eval);
  delete eval;

  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegion(0,0,.66,1)->plot(*gaussHist);
    plotter->destroyRegions();
    plotter->createRegion(0,0,.66,1)->plot(*gaussHist);
    plotter->region(0)->plot( minuitChi2Fit->fittedFunction() );
    plotter->region(0)->plot( minuitLeastSquaresFit->fittedFunction() );
    plotter->region(0)->plot( minuitBinnedMaxLikelihoodFit->fittedFunction() );
    
    gaussCloud->convert(100,gaussCloud->lowerEdge(),gaussCloud->upperEdge());
    plotter->createRegion(.66,0,.33,.5)->plot(gaussCloud->histogram());
    
    IFunction& cloudFunc = uncminUMLFitToCloud->fittedFunction();
    IModelFunction* cloudFuncModel = dynamic_cast<IModelFunction*>(&cloudFunc);
    if(cloudFuncModel) { //GB
      cloudFuncModel->normalize(false);
    }

    double gaussCloudNorm = 
      gaussCloud->entries() * ( gaussCloud->upperEdge()-gaussCloud->lowerEdge() )/gaussCloud->histogram().axis().bins();
    double cloudFuncNorm = 
      gaussCloudNorm/(::sqrt(2*M_PI)* cloudFunc.parameter("sigma"));
    cloudFunc.setParameter("amplitude",cloudFuncNorm);
    plotter->region(1)->plot( cloudFunc );

    plotter->createRegion(.66,.5,.33,.5)->plot( *gaussProj );

    IFunction& tupleFunc = uncminUMLFitToTuple->fittedFunction();
    IModelFunction* tupleFuncModel = dynamic_cast<IModelFunction*>(&tupleFunc);
    if(tupleFuncModel) { //GB
      tupleFuncModel->normalize(false); 
    }
    
    double gaussProjNorm = gaussProj->entries()*( gaussProj->axis().upperEdge()-gaussProj->axis().lowerEdge() )/gaussProj->axis().bins();
    double tupleFuncNorm = gaussProjNorm/(::sqrt(2*M_PI)*tupleFunc.parameter("sigma"));

    tupleFunc.setParameter("amplitude",tupleFuncNorm);
    plotter->region(2)->plot( tupleFunc );
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete fitter;
  delete minuitChi2Fit;
  delete minuitLeastSquaresFit;
  delete minuitBinnedMaxLikelihoodFit;
  delete uncminUMLFitToCloud;
  delete fitData;
  delete uncminUMLFitToTuple;

  delete ff;

  delete tf;

  delete hf;

  delete af;

  return EXIT_SUCCESS;
}
