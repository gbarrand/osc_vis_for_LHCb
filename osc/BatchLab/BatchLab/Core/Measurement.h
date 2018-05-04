#ifndef BatchLab_Measurement_h
#define BatchLab_Measurement_h

#include <AIDA/IMeasurement.h>

#include <Lib/Debug.h>

namespace BatchLab {

class Measurement : public virtual AIDA::IMeasurement {
public: //AIDA/IMeasurement
  virtual double value() const {return fValue;}
  virtual double errorPlus() const {return fErrorPlus;}
  virtual double errorMinus() const {return fErrorMinus;}
  virtual bool setValue(double aValue) {
    fValue = aValue;
    return true;
  }
  virtual bool setErrorPlus(double aErrorPlus) { 
    fErrorPlus = aErrorPlus;
    return true;
  }
  virtual bool setErrorMinus(double aErrorMinus) { 
    fErrorMinus = aErrorMinus;
    return true;
  }
public:
  inline Measurement():fValue(0),fErrorPlus(0),fErrorMinus(0){
    Lib::Debug::increment("BatchLab::Measurement");
  }
  inline Measurement(double aValue,double aErrorPlus,double aErrorMinus)
  :fValue(aValue)
  ,fErrorPlus(aErrorPlus)
  ,fErrorMinus(aErrorMinus)
  {
    Lib::Debug::increment("BatchLab::Measurement");
  }
  virtual ~Measurement(){
    Lib::Debug::decrement("BatchLab::Measurement");
  }
public:
  inline Measurement(const Measurement& aFrom)
  :AIDA::IMeasurement(aFrom)
  ,fValue(aFrom.fValue)
  ,fErrorPlus(aFrom.fErrorPlus)
  ,fErrorMinus(aFrom.fErrorMinus)
  {
    Lib::Debug::increment("BatchLab::Measurement");
  }
  inline Measurement& operator=(const Measurement& aFrom) {
    fValue = aFrom.fValue;
    fErrorPlus = aFrom.fErrorPlus;
    fErrorMinus = aFrom.fErrorMinus;
    return *this;
  }
private:
  double fValue;
  double fErrorPlus;
  double fErrorMinus;
};

}

#endif
