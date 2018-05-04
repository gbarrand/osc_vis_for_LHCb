#ifndef BatchLab_MidnightFitter_h
#define BatchLab_MidnightFitter_h

/*
 * BatchLab encapsulation of the Midnight optimizer.
 * 
 */

// Inheritance :
#include <BatchLab/Core/BaseFitter.h>

#include <inlib/cast>
#include <inlib/f2cmn>

#include <Lib/Debug.h>
#include <AIDA/IFunction.h>

namespace BatchLab {

class MidnightFitter : public BaseFitter {
public: //IManagedObject
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::MidnightFitter)
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
      f_out << "BatchLab::MidnightFitter::baseFit :"
          << " given function has no parameters." << std::endl;
      return false;
    }
    std::vector<std::string> names = aFunction.parameterNames();
    if(names.size()!=fparams.size()) {
      f_out << "BatchLab::MidnightFitter::baseFit :"
          << " bad given function parameter names size." << std::endl;
      return false;
    }
  
    FCN fcn(aFunction,aPts,aVs,aEs);
  
    inlib::f2cmn::fitter minuit(f_out,0);
    minuit.SetFCN(function,&fcn);
  
    int status;
    for(unsigned int index=0;index<names.size();index++) {
      double start = fparams[index];
      AIDA::IFitParameterSettings& ps = fitParameterSettings(names[index]);
      double step = ps.stepSize();
      //if(step==0) step = 0.01;
      minuit.mnparm(index,
  		  names[index].c_str(),
  		  start,
  		  step,
  		  0,0,status);
    }
  
    double args[10];
    args[0] = 0;
    minuit.mnexcm("SET PRI",args,1,status);
    minuit.mnexcm("MIGRAD",args,0,status);
    //minuit.mnexcm("MINOS",args,0,status);
    
    // Output.
    aChi2 = fcn.chi2();
  
   {aErrs.resize(names.size());
    for(unsigned int index=0;index<names.size();index++) {
      std::string s;
      double value,err;
      double bmin,bmax;
      minuit.mnpout(index,s,
  		  value,err,
  		  bmin,bmax,status);
      aErrs[index] = err;
    }}
  
    return true;  
  }

public:
  MidnightFitter(std::ostream& a_out,
                 AIDA::IFunctionFactory& aFunctionFactory,
                 AIDA::ITupleFactory& aTupleFactory)
  :BaseFitter(a_out,aFunctionFactory,aTupleFactory,"midnight"){
    Lib::Debug::increment("BatchLab::MidnightFitter");
  }
  virtual ~MidnightFitter(){
    Lib::Debug::decrement("BatchLab::MidnightFitter");
  }
private:
  class FCN {
  public:
    virtual double operator()(const std::vector<double>& aParams) const {
      INLIB_SELF(FCN);
      self.fChi2 = 0;
      unsigned int npt = fPts.size();
      fFunction.setParameters(aParams);
   //{printf("debug : param number : %d\n",aParams.size());
   // for(unsigned int i=0;i<aParams.size();i++) {
   // printf("debug :    %d %g\n",i,aParams[i]);
   //}}
      for(unsigned int i=0;i<npt;i++) {
        double value = fFunction.value(fPts[i]);
        double chinew = (fVs[i] - value)/fEs[i];
        self.fChi2 += (chinew * chinew);
      }
      return self.fChi2;
    }
    virtual double chi2() const { return fChi2;}
  public:
    FCN(AIDA::IFunction& aFunction,
        const std::vector< std::vector<double> >& aPts,
        const std::vector<double>& aVs,
        const std::vector<double>& aEs)
    :fFunction(aFunction),fPts(aPts),fVs(aVs),fEs(aEs),fChi2(0){
    }
    virtual ~FCN() {}
  private:
    AIDA::IFunction& fFunction;
    const std::vector< std::vector<double> >& fPts;
    const std::vector<double>& fVs;
    const std::vector<double>& fEs;
    double fChi2;
  };
  
  static bool function(int& aParamn,double* /*aGrad*/,
                       double& aValue,double* aParams,
                       int /*aStatus*/,void* aTag) {
    FCN* func = (FCN*)aTag;
    std::vector<double> params;
    params.resize(aParamn,0);
    for(int index=0;index<aParamn;index++) params[index] = aParams[index];
    aValue = (*func)(params);
    return true;
  }
};

}

#endif
