#include "Minuit/ModularFunctionMinimizer.h"
#include "Minuit/MinimumSeedGenerator.h"
#include "Minuit/AnalyticalGradientCalculator.h"
#include "Minuit/Numerical2PGradientCalculator.h"
#include "Minuit/MinimumBuilder.h"
#include "Minuit/MinimumSeed.h"
#include "Minuit/FunctionMinimum.h"
#include "Minuit/MnUserParameterState.h"
#include "Minuit/MnUserParameters.h"
#include "Minuit/MnUserCovariance.h"
#include "Minuit/MnUserTransformation.h"
#include "Minuit/MnUserFcn.h"
#include "Minuit/FCNBase.h"
#include "Minuit/FCNGradientBase.h"
#include "Minuit/MnStrategy.h"
#include "Minuit/MnHesse.h"
#include "Minuit/MnLineSearch.h"
#include "Minuit/MnParabolaPoint.h"

// #include "Minuit/MnUserParametersPrint.h"

FunctionMinimum ModularFunctionMinimizer::minimize(const FCNBase& fcn, const std::vector<double>& par, const std::vector<double>& err, unsigned int stra, unsigned int maxfcn, double toler) const {

  MnUserParameterState st(par, err);
  MnFcn mfcn(fcn);
  MnStrategy strategy(stra);
  Numerical2PGradientCalculator gc(mfcn, st.trafo(), strategy);

  if(maxfcn == 0) {
	  unsigned int size = static_cast < unsigned int > ( par.size() );
//	  maxfcn = 200 + 100*par.size() + 5*par.size()*par.size();
	  maxfcn = 200 + 100*size + 5*size*size;
  }

  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);
  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
}

FunctionMinimum ModularFunctionMinimizer::minimize(const FCNGradientBase& fcn, const std::vector<double>& par, const std::vector<double>& err, unsigned int stra, unsigned int maxfcn, double toler) const {

  MnUserParameterState st(par, err);
  MnFcn mfcn(fcn);
  MnStrategy strategy(stra);
  AnalyticalGradientCalculator gc(fcn, st.trafo());

  if(maxfcn == 0) {
	  unsigned int size = static_cast< unsigned int > ( par.size() );
//	  maxfcn = 200 + 100*par.size() + 5*par.size()*par.size();
	  maxfcn = 200 + 100*size + 5*size*size;
  }

  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);
  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
}

FunctionMinimum ModularFunctionMinimizer::minimize(const FCNBase& fcn, const std::vector<double>& par, const std::vector<double>& cov, unsigned int nrow, unsigned int stra, unsigned int maxfcn, double toler) const {

  MnUserParameterState st(par, cov, nrow);
  MnFcn mfcn(fcn);
  MnStrategy strategy(stra);
  Numerical2PGradientCalculator gc(mfcn, st.trafo(), strategy);

  if(maxfcn == 0) {
//	  maxfcn = 200 + 100*par.size() + 5*par.size()*par.size();
	  unsigned int size = static_cast < unsigned int > ( par.size() );
	  maxfcn = 200 + 100*size + 5*size*size;
  }

  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);
  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
} 
 
FunctionMinimum ModularFunctionMinimizer::minimize(const FCNGradientBase& fcn, const std::vector<double>& par, const std::vector<double>& cov, unsigned int nrow, unsigned int stra, unsigned int maxfcn, double toler) const {

  MnUserParameterState st(par, cov, nrow);
  MnFcn mfcn(fcn);
  MnStrategy strategy(stra);
  AnalyticalGradientCalculator gc(fcn, st.trafo());

  if(maxfcn == 0) {
//	  maxfcn = 200 + 100*par.size() + 5*par.size()*par.size();
      unsigned int size = static_cast < unsigned int > ( par.size() );
      maxfcn = 200 + 100*size + 5*size*size;
  }

  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);
  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
}

FunctionMinimum ModularFunctionMinimizer::minimize(const FCNBase& fcn, const MnUserParameters& upar, const MnStrategy& strategy, unsigned int maxfcn, double toler) const {

  MnUserParameterState st(upar);
  MnUserFcn mfcn(fcn, st.trafo());
  Numerical2PGradientCalculator gc(mfcn, st.trafo(), strategy);

  unsigned int npar = st.variableParameters();
  if(maxfcn == 0) maxfcn = 200 + 100*npar + 5*npar*npar;

  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);
  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
}

FunctionMinimum ModularFunctionMinimizer::minimize(const FCNBase& fcn, const MnUserParameters& upar, const MnUserCovariance& cov, const MnStrategy& strategy, unsigned int maxfcn, double toler) const {

  MnUserParameterState st(upar, cov);
  MnUserFcn mfcn(fcn, st.trafo());
  Numerical2PGradientCalculator gc(mfcn, st.trafo(), strategy);

  unsigned int npar = st.variableParameters();
  if(maxfcn == 0) maxfcn = 200 + 100*npar + 5*npar*npar;

  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);
  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
}

FunctionMinimum ModularFunctionMinimizer::minimize(const FCNBase& fcn, const MnUserParameterState& st, const MnStrategy& strategy, unsigned int maxfcn, double toler) const {

  MnUserFcn mfcn(fcn, st.trafo());
  Numerical2PGradientCalculator gc(mfcn, st.trafo(), strategy);

  unsigned int npar = st.variableParameters();
  if(maxfcn == 0) maxfcn = 200 + 100*npar + 5*npar*npar;
  MinimumSeed mnseeds = seedGenerator()(mfcn, gc, st, strategy);

  return minimize(mfcn, gc, mnseeds, strategy, maxfcn, toler);
}

FunctionMinimum ModularFunctionMinimizer::minimize(const MnFcn& mfcn, const GradientCalculator& gc, const MinimumSeed& seed, const MnStrategy& strategy, unsigned int maxfcn, double toler) const {

  return builder().minimum(mfcn, gc, seed, strategy, maxfcn, toler*mfcn.up());
}

