
#include <Lib/Processor.h>

#include <Lib/Debug.h>

#include <inlib/sys/atime>
#include <inlib/vmanip>
#include <cmath>

#include <iostream>

int main(int,char**) {


 {Lib::Processor proc
     ("((x>0.1)&&(y<=2))&&((name==\"Hel*o\")&&(z!=2))",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  vs.push_back(Lib::Value(1.));
  vs.push_back(Lib::Value(0.));
  vs.push_back(Lib::Value("Hello"));
  vs.push_back(Lib::Value(3));
  std::string serror;
  bool res;
  proc.cut(vs,res,serror);
  std::cout << "result : " << res << " (true expected)" << std::endl;
 }

 {Lib::Processor proc("((layer>=3)&&(layer<=6))&&(E<0)",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  vs.push_back(Lib::Value(1));
  vs.push_back(Lib::Value(0.1));
  std::string serror;
  bool res;
  proc.cut(vs,res,serror);
  std::cout << "result : " << res << " (false expected)" << std::endl;
  vs.clear();
  vs.push_back(Lib::Value(4));
  vs.push_back(Lib::Value(-0.1));
  proc.cut(vs,res,serror);
  std::cout << "result : " << res << " (true expected)" << std::endl;}

 {Lib::Processor proc("((layer>=3)&&(layer<=6))&&(E>=0)",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  vs.push_back(Lib::Value(1));
  vs.push_back(Lib::Value(0.1));
  std::string serror;
  bool res;
  proc.cut(vs,res,serror);
  std::cout << "result : " << res << " (false expected)" << std::endl;
  vs.clear();
  vs.push_back(Lib::Value(4));
  vs.push_back(Lib::Value(-0.1));
  proc.cut(vs,res,serror);
  std::cout << "result : " << res << " (false expected)" << std::endl;}

 {Lib::Processor proc("!(layer>=3)",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  vs.push_back(Lib::Value(1));
  std::string serror;
  bool res;
  proc.cut(vs,res,serror);
  std::cout << "result : (not) " << res << " (true expected)" << std::endl;}

 {Lib::Processor proc("!((layer>=3)&&!(layer<=6))&&(E>=0)",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  vs.push_back(Lib::Value(1));
  vs.push_back(Lib::Value(0.1));
  std::string serror;
  bool res;
  proc.cut(vs,res,serror);
  std::cout << "result : (not) " << res << " (true expected)" << std::endl;}

 //  gauss(x,H,M,W)    :  H * exp(-0.5 * ((x - M)/W)**2 );
 //  pol2(x,A,B,C)     : A + B x + C x**2 
 {Lib::Processor proc("gauss(x,H,M,W)+pol2(x,A,B,C)",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  vs.push_back(Lib::Value(1.)); //x
  vs.push_back(Lib::Value(1.)); //H
  vs.push_back(Lib::Value(0.)); //M
  vs.push_back(Lib::Value(2.)); //W
  vs.push_back(Lib::Value(1.)); //A
  vs.push_back(Lib::Value(2.)); //B
  vs.push_back(Lib::Value(3.)); //C
  Lib::Value res;
  std::string serror;
  proc.execute(vs,res,serror);
  double d;
  Lib::Value::to_double(res,d);

  std::cout << "result : " <<  d << " (6.8825 expected)" << std::endl;}

 {Lib::Processor proc("cos(M_PI)",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  std::vector<Lib::Value> vs;
  Lib::Value res;
  std::string serror;
  proc.execute(vs,res,serror);
  double d;
  Lib::Value::to_double(res,d);
  std::cout << "result : " << d << " (-1 expected)" << std::endl;}

 {inlib::atime begin = inlib::atime::now();
  unsigned rown = 77622;
  double e = 1;
  double vx = 1;
  double vy = 1;
  double tot = 0;
  std::cout << " begin..." << std::endl;
  for(unsigned int row=0;row<rown;row++) {
    tot += log10(fabs(e)*sqrt(vx*vx+vy*vy));
    //tot += e*vx*vy;
  }
  inlib::atime end = inlib::atime::elapsed(begin);
  std::cout << " elapsed : " << end.seconds() << " s " 
      << tot << " (11683.3 expected)" << std::endl;}

 std::cout << " sizeof(Variable) = " << (unsigned int)sizeof(Lib::Value) 
      << std::endl;
 {inlib::atime begin = inlib::atime::now();
   Lib::Processor proc("log10(abs(e)*sqrt(vx**2+vy**2))",std::cout);
  //Lib::Processor proc("sqrt(vx**2+vy**2)",std::cout);
  //Lib::Processor proc("log10(e)*vx*vy",std::cout);
  //Lib::Processor proc("log10(abs(e))",std::cout);
  std::vector<std::string> vars;
  proc.findVariables(vars);
  inlib::dump(vars,std::cout);
  proc.compile(vars);
  unsigned int number = vars.size();
  std::vector<Lib::Value> vals;
  vals.resize(number,Lib::Value(1.));
  unsigned rown = 77622;
  double tot = 0;
  Lib::Value res;
  double d;
  std::cout << " begin..." << std::endl;
  for(unsigned int row=0;row<rown;row++) {
    //for(unsigned int count=0;count<number;count++) {
    //  vals[count].set(3.14);
    //}
    std::string serror;
    proc.execute(vals,res,serror);
    Lib::Value::to_double(res,d);
    tot += d;
  }
  inlib::atime end = inlib::atime::elapsed(begin);
  std::cout << " elapsed : " << end.seconds() << " s "
      << tot << " (11683.3 expected)" << std::endl;}

 Lib::Debug::balance(std::cout);
 
 return 0;
}
