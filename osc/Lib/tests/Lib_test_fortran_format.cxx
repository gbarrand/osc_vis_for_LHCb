
#include <Lib/FortranOut.h>

#include <inlib/args>

#include <Lib/Debug.h>

#include <iostream>

int main(int a_argc,char** a_argv) {
 {inlib::args args(a_argc,a_argv);

  int verbose = 0;
  std::string sverbose;
  if(args.find("-verbose",sverbose)) {
    if(sverbose.empty()) verbose = 1;
  }

  std::string sformat;
  if(!args.find("-format",sformat)) {
    sformat = "(5(1X,G13.7))";
  }

 {Lib::FortranFormat proc(sformat,std::cout,verbose);
  std::cout << "Lib_fortran_format : " 
            << " parse : " << sformat
            << std::endl;
  if(!proc.parse()) {
    std::cout << "Lib_fortran_format : " 
              << " parse failed."<< std::endl;
  } else {
    proc.result().dump(std::cout);
  }}

  int I = 314;
  int J = 315;
  double R = 3.14;

 {sformat = "(I10,F10)";
  std::cout << "Lib_fortran_format : " 
            << " f77out : " << sformat
            << std::endl;
  Lib::FortranOut f77out(std::cout,std::cout,sformat);
  f77out << I << R << Lib::FortranOut::endl;}

 {sformat = "(3(2I10,F10))";
  std::cout << "Lib_fortran_format : " 
            << " f77out : " << sformat
            << std::endl;
  Lib::FortranOut f77out(std::cout,std::cout,sformat);
  for(int index=0;index<2;index++) {
    f77out.reset();
    f77out 
      << I << J << R 
      << I << J << R 
      << I << J << R 
      << Lib::FortranOut::endl;
  }}

 {sformat = "(3(2(i5,2X,A),F10.2))";
  std::cout << "Lib_fortran_format : " 
            << " f77out : " << sformat
            << std::endl;
  Lib::FortranOut f77out(std::cout,std::cout,sformat);
  for(int index=0;index<2;index++) {
    f77out.reset();
    f77out 
      << I << "ab" << J << "cd" << R 
      << I << "ab" << J << "cd" << R 
      << I << "ab" << J << "cd" << R 
      << Lib::FortranOut::endl;
  }}

 {sformat = "(2(5X,10F5.0,/))";
  std::cout << "Lib_fortran_format : " 
            << " f77out : " << sformat
            << std::endl;
  Lib::FortranOut f77out(std::cout,std::cout,sformat,verbose);
  std::vector<double> values;
  values.push_back(1);
  values.push_back(2);
  values.push_back(3);
  values.push_back(4);
  values.push_back(5);
  values.push_back(6);
  values.push_back(7);
  values.push_back(8);
  values.push_back(9);
  values.push_back(10);

  values.push_back(9.1);
  values.push_back(8.1);
  values.push_back(7.1);
  values.push_back(6.1);
  values.push_back(5.1);
  values.push_back(4.1);
  values.push_back(3.1);
  values.push_back(2.1);
  values.push_back(1.1);
  values.push_back(0.1);

  values.push_back(6.2);
  values.push_back(4.2);
  values.push_back(3.2);
  values.push_back(2.2);
  values.push_back(1.2);
  values.push_back(1.2);
  values.push_back(2.2);
  values.push_back(3.2);
  values.push_back(4.2);
  values.push_back(5.2);
  for(int index=0;index<2;index++) {
    f77out.reset();
    f77out << values << Lib::FortranOut::endl;
  }}

 }

  Lib::Debug::balance(std::cout);
 
  return 0;
}
