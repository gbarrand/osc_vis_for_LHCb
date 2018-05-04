#ifndef BatchLab_FitFactory_h
#define BatchLab_FitFactory_h

#include <AIDA/IFitFactory.h>

#include <Lib/Debug.h>

#include <BatchLab/Core/MinuitFitter.h>
#include <BatchLab/Core/MidnightFitter.h>
#include <BatchLab/Core/FitData.h>

#include <AIDA/IFunctionFactory.h>

#include <vector> //If AIDA-v3r2p1

namespace BatchLab {

class FitFactory : public virtual AIDA::IFitFactory {
public: //AIDA/IFitFactory
  virtual AIDA::IFitData* createFitData(){return new BatchLab::FitData();}
  virtual AIDA::IFitter* createFitter(const std::string& /*aFitterType*/,
                                      const std::string& aEngineType,
                                      const std::string& /*aOptions*/){
    //FIXME : dynamic load a fitter.
    // Not managed.
    std::string engine = aEngineType;
    inlib::tolowercase(engine);
    if(engine=="minuitc++") {
      return new BatchLab::MinuitFitter
                   (f_out,fFunctionFactory,fTupleFactory);
    } else {
      //Default.
      return new BatchLab::MidnightFitter
                   (f_out,fFunctionFactory,fTupleFactory);
    }
  }

  virtual std::vector<std::string> availableFitMethods() const { //AIDA-v3r3p0
    std::vector<std::string> vs;
    vs.push_back("chi2");
    return vs;
  }
  virtual std::vector<std::string> availableFitEngines() const { //AIDA-v3r3p0
    std::vector<std::string> vs;
    vs.push_back("minuitc++");
    vs.push_back("midnight");
    return vs;
  }

public:
  FitFactory(std::ostream& a_out,
             AIDA::IFunctionFactory& aFunctionFactory,
             AIDA::ITupleFactory& aTupleFactory)
  :f_out(a_out)
  ,fFunctionFactory(aFunctionFactory)
  ,fTupleFactory(aTupleFactory) {
    Lib::Debug::increment("BatchLab::FitFactory");
  }
  virtual ~FitFactory() {
    Lib::Debug::decrement("BatchLab::FitFactory");
  }
private:
  std::ostream& f_out;
  AIDA::IFunctionFactory& fFunctionFactory;
  AIDA::ITupleFactory& fTupleFactory;
};

}

#endif
