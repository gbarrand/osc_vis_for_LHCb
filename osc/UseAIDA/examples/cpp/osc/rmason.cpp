//
//  This program reads a rmason.ascii file that can be found compressed
// under UseAIDA/<version>/examples/data.
//
//  Usage :
//    OS> AIDA_example_rmason[_vis] [-compiled,-scripted,-dld]
//  Example :
//    OS> AIDA_example_rmason          # by using a batch kit (no plotting)
//    OS> AIDA_example_rmason_vis dld  # by using a kit having plotting. 
//

/////////////////////////////////////////////////////////////////////////
/// Setup the rmason_function class : ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#include <vector>

#include <cmath>
#ifndef M_PI
#define M_PI       3.1415926535897931160E0
#define M_PI_2     1.5707963267948965580E0  
#endif

#include <cstdio>

// Some common functions :
double gaussian( double aX,double aScale,double aMean,double aSigma) {
  double value = (aX - aMean)/aSigma;
  return aScale * ::exp(-0.5 * value * value);
}
double cauchy(double aX,double aHeight,double aCenter,double aWidth) {
  double b = aWidth/2.;
  double value = (aX - aCenter)/b;
  return aHeight/((value * value + 1)*b*M_PI);
}
double polynomial(double aX,const std::vector<double>& aParams) {
  double value = 0;
  double xxx = 1;
  unsigned int pn = aParams.size();
  for(unsigned int i=0;i<pn;i++) {
    value += aParams[i] * xxx;
    xxx = xxx * aX;
  }
  return value;
}

#include <AIDA/IFunction.h>
#include <AIDA/IAnnotation.h>

// Dummy AIDA::IAnnotation implementation :
class Annotation : public virtual AIDA::IAnnotation {
public: //AIDA/IAnnotation
  virtual bool addItem(const std::string&,const std::string&,bool = true) {
    return false;
  }
  virtual bool removeItem(const std::string&) {return false;}
  virtual std::string value(const std::string&) const {return "";}
  virtual bool visibility(const std::string&) const {return false;}
  virtual void setValue(const std::string&,const std::string&) {}
  virtual void setSticky(const std::string&,bool) {}
  virtual int size() const {return 0;}
  virtual std::string key(int) const {return "";}
  virtual std::string value(int) const {return "";}
  virtual void reset() {}
  //AIDA-v3r3p0
  virtual bool isSticky(const std::string&) const {return false;}
  virtual bool hasKey(const std::string&) const {return false;}
  virtual int findKey(const std::string&) const {return (-1);}
public:
  Annotation() {}
  Annotation(const Annotation&) {}
  virtual ~Annotation() {}
  Annotation& operator=(const Annotation&) { return *this;}
};

class rmason_function : public virtual AIDA::IFunction {
// Code specific to rmason_func :
public:
  rmason_function() {
    // one variable :
    fVariableNames.push_back("x[0]");
    // parameters :
    unsigned int paramn = 8;
    fParams.resize(paramn,0);
    for(unsigned int i=0;i<paramn;i++) {
      char s[32];
      ::sprintf(s,"p%d",i);
      fParamNames.push_back(s);
    }
  }
public: //AIDA/IFunction
  virtual int dimension() const {return 1;}
  virtual double value(const std::vector<double>& aVariables) const {
    if(aVariables.size()!=1) return 0;
    double x = aVariables[0];
    std::vector<double> params;
    params.push_back(fParams[6]);
    params.push_back(fParams[7]);
    return gaussian(x,fParams[0],fParams[1],fParams[2])+
           cauchy(x,fParams[3],fParams[4],fParams[5])+
           polynomial(x,params);  
  }
// Code generic :
private:
  enum {NotFound = -1};
public: //AIDA/IFunction
  virtual void* cast(const std::string& aClass) const {
    if(aClass=="AIDA::IFunction") {
      return (void*)static_cast<const AIDA::IFunction*>(this);
    } else {
      return 0;
    }
  }
  virtual std::string title() const {return fTitle;}
  virtual bool setTitle(const std::string& aTitle) {
    fTitle = aTitle;
    return true;
  }
  virtual AIDA::IAnnotation& annotation() {return fAnnotation;}
  virtual const AIDA::IAnnotation& annotation() const {return fAnnotation;}
  virtual bool isEqual(const IFunction&) const {return false;}
  virtual const std::vector<double>& gradient
    (const std::vector<double>&) const {return fGradient;}
  virtual bool providesGradient() const {return false;}
  virtual std::string variableName(int aIndex) const {
    if((aIndex<0)||(aIndex>=(int)fVariableNames.size())) return "";
    return fVariableNames[aIndex];
  }
  virtual std::vector<std::string> variableNames() const {
    return fVariableNames;
  }
  virtual std::vector<std::string> parameterNames() const {return fParamNames;}
  virtual const std::vector<double>& parameters() const {return fParams;}
  virtual int numberOfParameters() const {return fParams.size();}
  virtual int indexOfParameter(const std::string& aName) const {
    unsigned int number = fParamNames.size();
    for(unsigned int i=0;i<number;i++) {
      if(fParamNames[i]==aName) return i;
    }
    return NotFound;
  }
  virtual std::string codeletString() const {return fCodelet;}
  virtual bool setParameter(const std::string& aName,double aValue) {
    if(fParamNames.size()!=fParams.size()) return false;
    unsigned int number = fParamNames.size();
    for(unsigned int i=0;i<number;i++) {
      if(fParamNames[i]==aName) {
        fParams[i] = aValue;
        return true;
      }
    }
    return false;
  }
  virtual bool setParameters(const std::vector<double>& aParams) {
    fParams = aParams;
    return true;
  }
  virtual std::string normalizationParameter() const {return "";}//AIDA-v3r3p0
  virtual double parameter(const std::string& aName) const {
    if(fParamNames.size()!=fParams.size()) return 0; //FIXME : throw
    unsigned int number = fParamNames.size();
    for(unsigned int i=0;i<number;i++) {
      if(fParamNames[i]==aName) {
        return fParams[i];
      }
    }
    return 0; //FIXME : throw.
  }
public:
  rmason_function(const rmason_function& aFrom)
    :fAnnotation(aFrom.fAnnotation)
    ,fTitle(aFrom.fTitle)
    ,fCodelet(aFrom.fCodelet)
    ,fParams(aFrom.fParams)
    ,fParamNames(aFrom.fParamNames)
    ,fVariableNames(aFrom.fVariableNames)
    ,fGradient(aFrom.fGradient)
  {
  }
  virtual ~rmason_function() {}
private:
  Annotation fAnnotation;
  std::string fTitle;
  std::string fCodelet;
  std::vector<double> fParams;
  std::vector<std::string> fParamNames;
  std::vector<std::string> fVariableNames;
  std::vector<double> fGradient;
};

/////////////////////////////////////////////////////////////////////////
/// The main : //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ICloud2D.h>

#include <AIDA/IFitFactory.h>
#include <AIDA/IFitter.h>
#include <AIDA/IFitResult.h>

#include <AIDA/IFunctionFactory.h>
#include <AIDA/IFunction.h>

#include <AIDA/IPlotterFactory.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>

#include <amanip/all.h>

#include <iostream>

#define CAST(aObject,aClass) (aClass*)(aObject)->cast(#aClass)

int main(int aArgc,char** aArgv){
  std::string arg1 = aArgc>=2?aArgv[1]:"-compiled";

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "AIDA_example_rmason :"
              << " Can't get an AnalysisFactory." << std::endl;
    return 1;
  }

  ////////////////////////////////////////////////////////////////////
  /// Read the data into a tuple : ///////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << "AIDA_example_rmason :"
              << " Can't get a TreeFactory." << std::endl;
    return 1;
  }

  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/rmason.ascii.gz";
  AIDA::ITree* tree = 
    treeFactory->create(fileName,"ascii",true,false);
  if(!tree) {
    std::cout << "AIDA_example_rmason :"
              << " Can't create rmason.ascii tree." << std::endl;
    return 1;
  }

  AIDA::ITuple* tuple = amanip::find_tuple(*tree,"rmason",true,std::cout);
  if(!tuple) {
    std::cout << "AIDA_example_rmason :"
              << " Can't create the \"tuple\" tuple." << std::endl;
    return 1;
  }

  std::cout << "cols = " << tuple->columns() 
            << ", rows = " << tuple->rows() << std::endl;

  ////////////////////////////////////////////////////////////////////
  /// Creating and fill a Cloud2D to fit : ///////////////////////////
  ////////////////////////////////////////////////////////////////////
  AIDA::ITree* memTree = treeFactory->create();
  delete treeFactory;
  // Get an histogram factory :
  AIDA::IHistogramFactory* histoFactory = 
    aida->createHistogramFactory(*memTree);
  if(!histoFactory) {
    std::cout << "AIDA_example_rmason :"
              << " Can't get a HistogramFactory." << std::endl;
    return 1;
  }

  AIDA::ICloud2D* cloud = 
    histoFactory->createCloud2D("cloud","Cloud");
  if(!cloud) {
    std::cout << "AIDA_example_rmason :"
              << " Can't create the Cloud2D." << std::endl;
  } 

  delete histoFactory;

  tuple->start();
  while(tuple->next()) {

    double x = tuple->getDouble(0);
    double v = tuple->getDouble(2);

    cloud->fill(x,v,1);

    //std::cout << "x = " << x << ", v = " << v << std::endl;
  }
    
  ////////////////////////////////////////////////////////////////////
  /// Fitting : //////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  AIDA::IFitFactory* fitFactory = aida->createFitFactory(); 
  if(!fitFactory) {
    std::cout << "AIDA_example_rmason :"
              << " Can't get a FitFactory." << std::endl;
    return 1;
  }

  //std::string engine = "minuitc++";
  std::string engine = "midnight";
  AIDA::IFitter* fitter = fitFactory->createFitter("",engine,"");
  delete fitFactory;
  if(!fitter) {
    std::cout << "AIDA_example_rmason :"
              << " Can't get a fitter." << std::endl;
    return 1;
  }

  AIDA::IFunction* function = 0;
  if(arg1=="-compiled") {
    function = new rmason_function();
  } else if(arg1=="-scripted") {
    AIDA::IFunctionFactory* functionFactory = 
      aida->createFunctionFactory(*memTree); 
    if(!functionFactory) {
      std::cout << "AIDA_example_rmason :"
                << " Can't get a function factory." << std::endl;
      return 1;
    }
    // Fitting done with a scripted function :
    function = 
      functionFactory->createFunctionByName
      //("fit_function","scripted:gauss(x,H,M,W)");
      //("fit_function","scripted:gauss(x,H,M,W)+cauchy(x,A,B,C)");
      ("fit_function","scripted:gauss(x,H,M,W)+cauchy(x,A,B,C)+pol1(x,D,E)");
    delete functionFactory;
  } else if(arg1=="-dld") {
    AIDA::IFunctionFactory* functionFactory = 
      aida->createFunctionFactory(*memTree); 
    if(!functionFactory) {
      std::cout << "AIDA_example_rmason :"
                << " Can't get a function factory." << std::endl;
      return 1;
    }
    // Fitting done with an dynamically loaded function 
    //  found in rmason_func.cpp :
    function = 
      functionFactory->createFunctionByName
        ("fit_function","compiled:rmason_func.cpp");
    delete functionFactory;
  }

  if(!function) {
    std::cout << "AIDA_example_rmason :"
              << " Can't get the function to fit with." << std::endl;
    return 1;
  }

  // Starting values :
  std::vector<double> params;
  params.push_back(1);       // H
  params.push_back(16000);   // M
  params.push_back(1000);    // W
  params.push_back(10);      // A
  params.push_back(18400);   // B
  params.push_back(10);      // C
  params.push_back(0);       // D
  params.push_back(0.00001); // E
  function->setParameters(params);

  AIDA::IFitResult* fitResult = fitter->fit(*cloud,*function);
  if(!fitResult) {
    std::cout << "AIDA_example_rmason :"
              << " fit failed." << std::endl;
    return 1;
  }
  delete fitResult;
  delete fitter;

 {// Plotting :
  AIDA::IPlotterFactory* pf = aida->createPlotterFactory(aArgc,aArgv);
  if(!pf) {
    std::cout << "AIDA_example_rmason :"
              << " can't get a plotter factory." 
              << std::endl;
  } else {
    AIDA::IPlotter* plotter = pf->create();
    delete pf;
    if(!plotter) {
      std::cout << "AIDA_example_rmason :"
                << " can't create a plotter." 
                << std::endl;
    } else {
      plotter->createRegions(1,1,0);
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.plot(*cloud);
      region.plot(*function);
      plotter->show();
      plotter->interact();
      delete plotter;
    }
  }}

  ////////////////////////////////////////////////////////////////////
  /// Leaving : //////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  delete aida; // Will delete the trees.

  std::cout << "AIDA_example_rmason :"
            << " Exiting..." << std::endl;

  return 0;
}

