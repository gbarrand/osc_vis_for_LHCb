
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

static void fitting(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Get facilities :
  AIDA::IPlotter* plotter = amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;
  AIDA::IFitter* fitter = amanip::create_fitter(a_aida);
  if(!fitter) return;

  plotter->setTitle("Fitting examples");
  plotter->createRegions(2,3,0);
    
  /////////// Gauss ///////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->currentRegion();

    inlib::random::gauss r(0,1);
    AIDA::IHistogram1D* hgauss = 
      amanip::create_histogram_1D
        (a_aida,*mem,"hgauss","Rand gauss",100,-5,5);
    if(!hgauss) return;

    for(int index=0;index<10000;index++)  hgauss->fill(r.shoot(),1);

    region.setTitle("Gaussian fit.");
    region.setParameter("plotter.titleAutomated","FALSE");

    hgauss->annotation().addItem("plotter.modeling","wire_boxes");
    region.plot(*hgauss);

    AIDA::IFitResult* fitResult = fitter->fit(*hgauss,"G");
    if(fitResult) {
      AIDA::IFunction& fitFunction = fitResult->fittedFunction(); 
      region.plot(fitFunction);
      delete fitResult;
    }
  }

  /////////// Breit Wigner ///////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();
    AIDA::IHistogram1D* hbw = 
      amanip::create_histogram_1D
        (a_aida,*mem,"hbw","Rand BW",100,-5,5);
    if(!hbw) return;

    inlib::random::bw rbw(0,1);

    for(int index=0;index<10000;index++)  hbw->fill(rbw.shoot(),1);

    region.setTitle("Breit-Wigner fit.");
    region.setParameter("plotter.titleAutomated","FALSE");

    hbw->annotation().addItem("plotter.modeling","wire_boxes");
    region.plot(*hbw);

    AIDA::IFitResult* fitResult = fitter->fit(*hbw,"BW");
    if(fitResult) {
      AIDA::IFunction& fitFunction = fitResult->fittedFunction();
      region.plot(fitFunction);
      delete fitResult;
    }
  }

  /////////// Exponential ///////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();

    AIDA::IHistogram1D* hexp = 
      amanip::create_histogram_1D
        (a_aida,*mem,"hexp","Exp",100,0,10);
    if(!hexp) return;

/*
    AIDA::IFunction* func = 
      amanip::create_function(a_aida,*mem,"exponential","E");
    if(!func) return;

    std::vector<double> params;
    params.push_back(0.4);
    params.push_back(1);
    func->setParameters(params);

    for(int index=0;index<100;index++)  {
      double x = index * 0.1 - 5 + 0.05;
      std::vector<double> args;
      args.push_back(x);
      hexp->fill(x,func->value(args));
    }
*/

    inlib::random::exp rexp(0.5); //0.5*exp(-x*0.5) //rate=0.5

    for(int index=0;index<10000;index++)  hexp->fill(rexp.shoot(),1);

    region.setTitle("Exponential fit.");
    region.setParameter("plotter.titleAutomated","FALSE");

    hexp->annotation().addItem("plotter.modeling","wire_boxes");
    region.plot(*hexp);

    AIDA::IFitResult* fitResult = fitter->fit(*hexp,"E");
    if(fitResult) {
      AIDA::IFunction& fitFunction = fitResult->fittedFunction();
      region.plot(fitFunction);
      delete fitResult;
    }
  }

  /////////// Polynomial ///////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();

    AIDA::IFunction* func = 
      amanip::create_function(a_aida,*mem,"p2","Polynomial 2","P2");
    if(!func) return;

    std::vector<double> params;
    params.push_back(1);
    params.push_back(2);
    params.push_back(3);
    func->setParameters(params);

    AIDA::IHistogram1D* hpol = 
      amanip::create_histogram_1D(a_aida,*mem,"hp2","P2",100,-5,5);
    if(!hpol) return;

    for(int index=0;index<100;index++)  {
      double x = index * 0.1 - 5 + 0.05;
      std::vector<double> args;
      args.push_back(x);
      hpol->fill(x,func->value(args));
    }

    region.setTitle("Polynomial fit.");
    region.setParameter("plotter.titleAutomated","FALSE");

    hpol->annotation().addItem("plotter.modeling","wire_boxes");
    region.plot(*hpol);

    AIDA::IFitResult* fitResult = fitter->fit(*hpol,"P2");
    if(fitResult) {
      AIDA::IFunction& fitFunction = fitResult->fittedFunction();
      region.plot(fitFunction);
      delete fitResult;
    }
  }

  /////////// Gauss + pol2 /////////////////////////////////
  {
    //  Trickier fit. An expression is given using the usual 
    // mathematical functions (cos, exp, ...) and using 
    // the functions :
    //   gauss(x,H,M,W)    :  H * exp(-0.5 * ((x - M)/W)**2 );
    //   bw(x,H,M,W)       :  H/(1. + (2 * (x - M)/W)**2 );
    //   expo(x,A,B)       : exp(A + B x)
    //   pol1(x,A,B)       : A + B x
    //   pol2(x,A,B,C)     : A + B x + C x**2 
    //   pol3(x,A,B,C,D)   : A + B x + C x**2 + D x**3
    //   pol4(x,A,B,C,D,E) : A + B x + C x**2 + D x**3 + E x**4 
    //  These functions have parameters that are going to be 
    // used as fitting parameters.

    AIDA::IHistogram1D* hgauss_pol2 = 
      amanip::create_histogram_1D
        (a_aida,*mem,"hgauss_pol2","Rand gauss",100,-5,5);
    if(!hgauss_pol2) return;

    inlib::random::gauss r(0,1);
    for(int index=0;index<10000;index++)  {
      hgauss_pol2->fill(r.shoot(),1);
    }

    AIDA::IPlotterRegion& region = plotter->next();
    region.setTitle("Scripted fit.");
    region.setParameter("plotter.titleAutomated","FALSE");

    hgauss_pol2->annotation().addItem("plotter.modeling","wire_boxes");
    region.plot(*hgauss_pol2);

    AIDA::IFunction* func = 
      amanip::create_scripted_function
        (a_aida,*mem,"scripted",1,"gauss(x,H,M,W)+pol2(x,A,B,C)");
    if(!func) {
      a_out << "Can't create scripted function." << std::endl;
    } else {
      // Starting values :
      std::vector<double> params;
      params.push_back(400); // H
      params.push_back(0);   // M
      params.push_back(4);   // W
      params.push_back(1);   // A
      params.push_back(0);   // B
      params.push_back(-1);  // C
      func->setParameters(params);

      AIDA::IFitResult* fitResult = fitter->fit(*hgauss_pol2,*func);
      if(fitResult) {
        AIDA::IFunction& fitFunction = fitResult->fittedFunction();
        region.plot(fitFunction);
        delete fitResult;
      }
    }

  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete fitter;
  delete plotter;
}
int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_fitting :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  fitting(*aida,std::cout,"",true);
  delete aida;
  return 0;
}
