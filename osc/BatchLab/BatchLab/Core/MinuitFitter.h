#ifndef BatchLab_MinuitFitter_h
#define BatchLab_MinuitFitter_h

/*
 * BatchLab encapsulation of the LCG/MinuitC++ optimizer.
 * 
 */

// Inheritance :
#include <BatchLab/Core/BaseFitter.h>

#include <inlib/cast>
#include <Lib/Debug.h>

#include <AIDA/IFunction.h>

#include <Minuit/FCNBase.h>
#include <Minuit/MnUserParameters.h>
#include <Minuit/MnMigrad.h>
#include <Minuit/FunctionMinimum.h>
#include <Minuit/MnPrint.h>

namespace BatchLab {

class MinuitFitter : public BaseFitter {
public: //IManagedObject
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::MinuitFitter)
    else return BaseFitter::cast(a_class);
  }
public: //BatchLab::BaseFitter :
  virtual bool baseFit(AIDA::IFunction& aFunction,
                       const std::vector< std::vector<double> >& aPts,
                       const std::vector<double>& aVs,
                       const std::vector<double>& aEs,
                       double& aChi2,std::vector<double>& aErrs){
    aChi2 = 0;
    aErrs.clear();
    const std::vector<double>& fparams = aFunction.parameters();
    if(fparams.size()<=0) {
      f_out << "BatchLab::MinuitFitter::baseFit :"
          << " given function has no parameters." << std::endl;
      return false;
    }
    std::vector<std::string> names = aFunction.parameterNames();
    if(names.size()!=fparams.size()) {
      f_out << "BatchLab::MinuitFitter::baseFit :"
          << " bad given function parameter names size." << std::endl;
      return false;
    }
  
    MnUserParameters mnparams;
    for(unsigned int index=0;index<names.size();index++) {
      double start = fparams[index];
      AIDA::IFitParameterSettings& ps = fitParameterSettings(names[index]);
      double step = ps.stepSize();
      //if(step==0) step = 0.01;
      mnparams.add(names[index].c_str(), start, step);
      //mnparams.setLimits("mean", mean-0.01, mean+0.01);
    }
  
    UserMinimizer fcn(aFunction,aPts,aVs,aEs);
    MnMigrad migrad(fcn, mnparams); //Will modify aFunction parameters.
  
    FunctionMinimum fmin = migrad();
    if(!fmin.isValid()) {
      f_out << "BatchLab::MinuitFitter::baseFit :"
          << " bad FunctionMinimum not valid." << std::endl;
      return false;
    }
  
    f_out << "MnMigrad : " << fmin << std::endl;
  
    aChi2 = fcn.chi2();
  
    aErrs.resize(names.size());
   {for(unsigned int index=0;index<names.size();index++) {
      aErrs[index] = mnparams.error(names[index].c_str());
    }}
  
    return true;
  }
public:
  MinuitFitter(std::ostream& a_out,
               AIDA::IFunctionFactory& aFunctionFactory,
               AIDA::ITupleFactory& aTupleFactory)
  :BaseFitter(a_out,aFunctionFactory,aTupleFactory,"minuitc++"){
    Lib::Debug::increment("BatchLab::MinuitFitter");
  }
  virtual ~MinuitFitter(){
    Lib::Debug::decrement("BatchLab::MinuitFitter");
  }

private:
  class UserMinimizer : public FCNBase {
  public: //FCNBase
    virtual double up() const { return fError;}
    virtual double operator()(const std::vector<double>& aParams) const {
      INLIB_SELF(UserMinimizer);
      self.fChi2 = 0;
      unsigned int npt = fPts.size();
      fFunction.setParameters(aParams);
      for(unsigned int i=0;i<npt;i++) {
        double value = fFunction.value(fPts[i]);
        double chinew = (fVs[i] - value)/fEs[i];
        self.fChi2 += (chinew * chinew);
      }
      return self.fChi2;
    }
  public:
    UserMinimizer(AIDA::IFunction& aFunction,
                  const std::vector< std::vector<double> >& aPts,
                  const std::vector<double>& aVs,
                  const std::vector<double>& aEs)
    :fFunction(aFunction),fPts(aPts),fVs(aVs),fEs(aEs),fError(1),fChi2(0){
    }
    virtual ~UserMinimizer() {}
    double chi2() const { return fChi2;}
  private:
    AIDA::IFunction& fFunction;
    const std::vector< std::vector<double> >& fPts;
    const std::vector<double>& fVs;
    const std::vector<double>& fEs;
    double fError;  
    double fChi2;
  };

};

}

#endif
