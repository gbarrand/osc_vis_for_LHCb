
#include <inlib/func>

/*
 * Signature must be on one line starting with "function" in C comment. 
 * Argument left parenthesis must stick the function name.
 */

extern "C" { double rmason_func(double a_xs[1],double a_params[8]); }

/*function(8)*/ double rmason_func(double a_xs[1],double a_params[8]) {
  double x = a_xs[0];

  double G_H = a_params[0];
  double G_M = a_params[1];
  double G_W = a_params[2];
  inlib::func::gaussian gauss(G_H,G_M,G_W);

  double C_H = a_params[3];
  double C_C = a_params[4];
  double C_W = a_params[5];
  inlib::func::cauchy cauchy(C_H,C_C,C_W);

  std::vector<double> params;
  params.push_back(a_params[6]);
  params.push_back(a_params[7]);
  inlib::func::polynomial pol(params);

  return gauss.value(x)+cauchy.value(x)+pol.value(x);  
}
