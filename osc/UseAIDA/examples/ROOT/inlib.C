
//////////////////////////////////////////////////////////////
/// from inlib/random, math //////////////////////////////////
//////////////////////////////////////////////////////////////

#include <cstdlib>

double rflat() {
  // Shoot random numbers in [0,1] according a flat distribution.
  double value  = (double)::rand();
  value /= (double)RAND_MAX;
  return value;
}

double rgauss(double a_mean,double a_std_dev) {
  double v1,v2,r,fac;
  do {
    v1 = 2.0 * rflat() - 1.0;
    v2 = 2.0 * rflat() - 1.0;
    r = v1*v1 + v2*v2;
  }   while ( r > 1.0 );
  fac = ::sqrt(-2.0*::log(r)/r);
  return (v2 * fac) * a_std_dev + a_mean;
}

double half_pi(){return 1.5707963267948965580E0;}

double rbw(double a_mean,double a_gamma) {
  double rval = 2.0 * rflat() - 1.0;
  double displ = 0.5 * a_gamma * ::tan(rval * half_pi());
  return a_mean + displ;
}

