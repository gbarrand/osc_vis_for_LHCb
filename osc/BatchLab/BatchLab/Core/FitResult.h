#ifndef BatchLab_FitResult_h
#define BatchLab_FitResult_h

#include <AIDA/IFitResult.h>

namespace AIDA {
  class IFitParameterSettings;
}
#include <AIDA/IFunction.h>

#include <Lib/Debug.h>

namespace BatchLab {

class FitResult : public virtual AIDA::IFitResult {
public: //AIDA::IFitResult
  virtual bool isValid() {return true;}
  virtual AIDA::IFunction& fittedFunction() {return fFunction;}
  virtual double quality() {return fQuality;}
  virtual int ndf() {return fNDF;}
  virtual int fitStatus() {return 0;}
  virtual double covMatrixElement(int,int) {return 0;}
  virtual std::string fitMethodName() {return fFitMethodName;}
  virtual std::string engineName() {return fEngineName;}
  virtual std::string dataDescription() {return fDataDescription;}
  virtual std::vector<std::string> constraints() {return fConstraints;}
  virtual AIDA::IFitParameterSettings* fitParameterSettings(const std::string& /*aName*/) {return 0;}
  virtual const std::vector<double>& fittedParameters() const {
    return fFittedParameters;
  }
  virtual std::vector<std::string> fittedParameterNames() const {
    return fFittedParameterNames;
  }
  virtual double fittedParameter(const std::string& /*aName*/) {return 0;}
  virtual const std::vector<double>& errors() const {return fErrors;}
  virtual const std::vector<double>& errorsPlus() const {return fErrorsPlus;}
  virtual const std::vector<double>& errorsMinus() const {return fErrorsMinus;}

public:
  FitResult(AIDA::IFunction& aFunction,
            const std::vector<double>& aErrs,
            int aNDF,double aQuality) 
  :fFunction(aFunction)
  ,fQuality(aQuality)
  ,fNDF(aNDF) {
    Lib::Debug::increment("BatchLab::FitResult");

    fFittedParameters = aFunction.parameters();
    fFittedParameterNames = aFunction.parameterNames();
  
    fErrors.resize(aErrs.size());
    fErrorsPlus.resize(aErrs.size());
    fErrorsMinus.resize(aErrs.size());
    for(unsigned int index=0;index<aErrs.size();index++) {
      fErrors[index] = aErrs[index];
      fErrorsPlus[index] = aErrs[index]/2;
      fErrorsMinus[index] = -aErrs[index]/2;
    }  
  }
  virtual ~FitResult() {
    Lib::Debug::decrement("BatchLab::FitResult");
  }
private:
  AIDA::IFunction& fFunction;
  std::string fFitMethodName;
  std::string fEngineName;
  std::string fDataDescription;
  std::vector<std::string> fConstraints;
  std::vector<double> fFittedParameters;
  std::vector<std::string> fFittedParameterNames;
  std::vector<double> fErrors;
  std::vector<double> fErrorsPlus;
  std::vector<double> fErrorsMinus;
  double fQuality;
  int fNDF;
};

}

#endif
