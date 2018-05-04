#ifndef RandGauss_h
#define RandGauss_h

#include <cstdlib>
#include <cmath>

class RandGauss{
public:
  RandGauss(double aMean,double aSigma):fMean(aMean),fSigma(aSigma){}
  double fire() const {
    double v1,v2,r,fac;
    double flat1,flat2;
    do {
      flat1 = ((double)::rand())/((double)RAND_MAX);
      flat2 = ((double)::rand())/((double)RAND_MAX);
      v1 = 2.0 * flat1 - 1.0;
      v2 = 2.0 * flat2 - 1.0;
      r = v1*v1 + v2*v2;
    } while ( r > 1.0 );
    fac = ::sqrt(-2.0*::log(r)/r);
    return (v2 * fac) * fSigma + fMean;
  }
private:
  double fMean;
  double fSigma;
};

#endif
