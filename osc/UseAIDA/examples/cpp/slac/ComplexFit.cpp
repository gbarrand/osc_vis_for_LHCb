
// C++ conversion of the java/ComplexFit.java example.

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
  //ITree* tree = trf->create("storeForFittingExample");
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;
  IHistogram2D* hist  = hf->createHistogram2D("hist","Test Histogram",100,-5,15,50,-5,5);
  if(!hist) return EXIT_FAILURE;
      
  Random r;
  int i;
  for (i=0; i<10000; i++) {
    double x = r.nextGaussian()+5;
    if ( r.nextDouble() > 0.8 ) x = 2.5*r.nextGaussian()+5;
    double y = r.nextGaussian();
    hist->fill(x,y);
  }
      
  // Do Fit
  IFitFactory* ff = af->createFitFactory(); 
  if(!ff) return EXIT_FAILURE;
  IFitter* fitter = ff->createFitter("Chi2","minuit");
  IFitData* fitData = ff->createFitData();
  delete ff;
  if(!fitter) return EXIT_FAILURE;
  if(!fitData) return EXIT_FAILURE;
  fitData->create2DConnection(*hist);

  IFunctionFactory* funcf  = af->createFunctionFactory(*tree);
  if(!funcf) return EXIT_FAILURE;
  std::string script = "N*(a*exp( -(x[0]-mu0)*(x[0]-mu0)/(2*s0*s0) )+";
  script += "(1-a)*exp( -(x[0]-mu1)*(x[0]-mu1)/(2*s1*s1) ))*exp( -(x[1]-mu2)*";
  script += "(x[1]-mu2)/(2*s2*s2) )";
  IFunction* func = funcf->createFunctionFromScript("twoDdistr",2,script,"N,a,mu0,s0,mu1,s1,mu2,s2","","");
  delete funcf;
  if(!func) return EXIT_FAILURE;

  std::vector<double> initialPars;
  initialPars.push_back(1);
  initialPars.push_back(0.8);
  initialPars.push_back(5);
  initialPars.push_back(1);
  initialPars.push_back(5);
  initialPars.push_back(2);
  initialPars.push_back(0);
  initialPars.push_back(1);
  func->setParameters( initialPars );

  fitter->fitParameterSettings("mu2").setFixed(true);
  fitter->fitParameterSettings("a").setBounds(0.5,0.9);
  fitter->fitParameterSettings("a").setStepSize(0.001);
  fitter->fitParameterSettings("s1").setBounds(2,4);
  fitter->fitParameterSettings("s1").setStepSize(0.1);
  fitter->setConstraint("s0 = s2");
  fitter->setConstraint("mu0 = mu1");


  IFitResult* fitResult = fitter->fit(*fitData,*func);
  delete fitter;
  if(!fitResult) return EXIT_FAILURE;
      
  std::cout << "Chi2 = " << fitResult->quality() << std::endl;
    
  std::vector<double> fPars     = fitResult->fittedParameters();
  std::vector<double> fParErrs  = fitResult->errors();
  std::vector<std::string> fParNames = fitResult->fittedParameterNames();

  for(i=0; i< fitResult->fittedFunction().numberOfParameters(); i++ ) 
    std::cout << fParNames[i] 
              << " : " << fPars[i] << " +- " << fParErrs[i]
              << std::endl;


  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    // Show results
    plotter->destroyRegions();
    plotter->createRegion(0,0,.66,1)->plot(*hist);
    plotter->createRegion(.66,0,.33,.5)->plot( *hf->projectionX("projX",*hist) );
    plotter->createRegion(.66,.5,.33,.5)->plot( *hf->projectionY("projY",*hist) );
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete fitResult;

  delete hf;

  delete af;

  return EXIT_SUCCESS;
}
