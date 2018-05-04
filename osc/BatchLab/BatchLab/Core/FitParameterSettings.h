#ifndef BatchLab_FitParameterSettings_h
#define BatchLab_FitParameterSettings_h

#include <AIDA/IFitParameterSettings.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

namespace BatchLab {

class FitParameterSettings : public virtual AIDA::IFitParameterSettings {
public: //AIDA::IFitParameterSettings

  virtual std::string name() const {return fName;}
  virtual double stepSize() const {return fStepSize;}
  virtual double upperBound() const {return fUpperBound;}
  virtual double lowerBound() const {return fLowerBound;}
  virtual bool isBound() const {return false;}
  virtual bool isFixed() const {return fFixed;}

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual void setStepSize(double aStepSize){fStepSize = aStepSize;}
  virtual void setBounds(double aLowerBound,double aUpperBound) {
    fLowerBound = aLowerBound;
    fUpperBound = aUpperBound;
  }
#else
  virtual bool setStepSize(double aStepSize) {
    fStepSize = aStepSize;
    return true;
  }
  virtual bool setBounds(double aLowerBound,double aUpperBound){
    fLowerBound = aLowerBound;
    fUpperBound = aUpperBound;
    return true;
  }
#endif
  virtual void removeBounds(){}
  virtual void setFixed(bool aFixed){fFixed = aFixed;}
  virtual void setLowerBound(double aLowerBound){fLowerBound = aLowerBound;}
  virtual void setUpperBound(double aUpperBound){fUpperBound = aUpperBound;}
  virtual void reset() {
    fStepSize = 1;
    fLowerBound = 0;
    fUpperBound = 0;
    fFixed = false;
  }
public:
  FitParameterSettings(const std::string& aName)
  :fName(aName),fStepSize(1),fLowerBound(0),fUpperBound(0),fFixed(false)
  {}

  FitParameterSettings(const FitParameterSettings& aFrom)
  :AIDA::IFitParameterSettings()
  ,fName(aFrom.fName),fStepSize(aFrom.fStepSize)
  ,fLowerBound(aFrom.fLowerBound),fUpperBound(aFrom.fUpperBound)
  ,fFixed(aFrom.fFixed)
  {}

  FitParameterSettings& operator =(const FitParameterSettings& aFrom){
    fName = aFrom.fName;
    fStepSize = aFrom.fStepSize;
    fLowerBound = aFrom.fLowerBound;
    fUpperBound = aFrom.fUpperBound;
    fFixed = aFrom.fFixed;
    return *this;
  }

  virtual ~FitParameterSettings(){}
private:
  std::string fName;
  double fStepSize;
  double fLowerBound;
  double fUpperBound;
  bool fFixed;
};

}

#endif
