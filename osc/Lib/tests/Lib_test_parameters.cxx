#include <inlib/params>

#include <Lib/Value.h>

int main() {

  inlib::params<std::string,double> dparams("",0);
  inlib::params<std::string,std::string> sparams("","");

  inlib::params<std::string,Lib::Value> SIGMA("",Lib::Value());
  SIGMA.remove("xxx");

  return 0;
}
