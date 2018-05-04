#include <Lib/Functions.h>

#include <iostream>

int main() {

  // LAL variable syntax :
 {std::string script = "x2+2*x0";
   Lib::Function::Scripted func(script,std::cout);
  if(!func.isValid()) {
    std::cout << "Lib_functions : problem with funciotn." << std::endl;
    return 1;
  }

  std::cout << "script : " << inlib::sout(script) << std::endl;
  std::vector<std::string> vars = func.variableNames();
  for(unsigned int index=0;index<vars.size();index++) {
    std::cout << " " << vars[index] << std::endl;
  }

  std::vector<double> xs(2);
  xs[0] = 1; //x0
  xs[1] = 2; //x2
  double val;
  std::string serror;
  if(!func.value(xs,val,serror)) {
    std::cout << "Lib_functions : problem to get value." << std::endl;
    return 1;
  }
  std::cout << "Should be 4 : " << val << std::endl;}

  // xyz variable syntax :
 {std::string script = "y+2*x";
   Lib::Function::Scripted func(script,std::cout);
  if(!func.isValid()) {
    std::cout << "Lib_functions : problem with funciotn." << std::endl;
    return 1;
  }

  std::cout << "script : " << inlib::sout(script) << std::endl;
  std::vector<std::string> vars = func.variableNames();
  for(unsigned int index=0;index<vars.size();index++) {
    std::cout << " " << vars[index] << std::endl;
  }

  std::vector<double> xs(2);
  xs[0] = 1; //x
  xs[1] = 2; //y
  double val;
  std::string serror;
  if(!func.value(xs,val,serror)) {
    std::cout << "Lib_functions : problem to get value." << std::endl;
    return 1;
  }
  std::cout << "Should be 4 : " << val << std::endl;}

  // AIDA/SLAC variable syntax :
 {std::string script = "x[2]+2*x[0]";
   Lib::Function::Scripted func(script,std::cout);
  if(!func.isValid()) {
    std::cout << "Lib_functions : problem with funciotn." << std::endl;
    return 1;
  }

  std::cout << "script : " << inlib::sout(script) << std::endl;
  std::vector<std::string> vars = func.variableNames();
  for(unsigned int index=0;index<vars.size();index++) {
    std::cout << " " << vars[index] << std::endl;
  }

  std::vector<double> xs(2);
  xs[0] = 1; //x[0]
  xs[1] = 2; //x[2]
  double val;
  std::string serror;
  if(!func.value(xs,val,serror)) {
    std::cout << "Lib_functions : problem to get value." << std::endl;
    return 1;
  }
  std::cout << "Should be 4 : " << val << std::endl;}

 {std::string script = "gauss(x[0],H,M,W)+pol2(x[1],A,B,C)";
   Lib::Function::Scripted func(script,std::cout);
  if(!func.isValid()) {
    std::cout << "Lib_functions : problem with funciotn." << std::endl;
    return 1;
  }

  std::cout << "script : " << inlib::sout(script) << std::endl;
  std::vector<std::string> vars = func.variableNames();
  for(unsigned int index=0;index<vars.size();index++) {
    std::cout << " " << vars[index] << std::endl;
  }}


  return 0;
}

