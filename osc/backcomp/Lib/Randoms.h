#ifndef Lib_Randoms_h
#define Lib_Randoms_h

#include <cstdlib>   
#include <cmath>   

namespace Lib {

class RandomFlat {
public:
  double shoot(){
    // Shoot random numbers in [0,1] according a flat distribution.
    double value  = (double)::rand();
    value /= (double)RAND_MAX;
    return value;
  }
};


class RandomGauss {
public:
  RandomGauss(double aMean = 0,double aStdDev =1) 
  :fMean(aMean),fStdDev(aStdDev){}
  double shoot(){
    //  Shoot random numbers according a 
    // gaussian distribution of mean 0 and sigma 1.
    double v1,v2,r,fac;
    do {
      v1 = 2.0 * fFlat.shoot() - 1.0;
      v2 = 2.0 * fFlat.shoot() - 1.0;
      r = v1*v1 + v2*v2;
    }   while ( r > 1.0 );
    fac = ::sqrt(-2.0*::log(r)/r);
    return (v2 * fac) * fStdDev + fMean;
  }
private:
  RandomFlat fFlat;
  double fMean;
  double fStdDev;
};

}

#endif
