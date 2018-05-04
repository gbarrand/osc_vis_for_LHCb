#ifndef Lib_calc_h
#define Lib_calc_h

#include <string>
#include <vector>
#include <ostream>

#include <Lib/Processor.h>

namespace Lib {

bool calc(
 std::ostream& a_out
,const std::string& aFormula
,const std::vector<double>& aVals
,double& aValue
){
  Lib::Processor proc(aFormula,a_out);
  std::vector<std::string> vars;
  if(!proc.findVariables(vars)) {
    a_out << "Lib::calc :"
          << " can't find variables in " << inlib::sout(aFormula) << "."
          << std::endl;
    return false;
  }
  //Lib::smanip::write(vars,printer);
  if(!proc.compile(vars)) {
    a_out << "Lib::calc :"
          << " can't compile " << inlib::sout(aFormula) << "."
          << std::endl;
    return false;
  }
  unsigned int number = vars.size();
  if(aVals.size()!=number) {
    a_out << "Lib::calc :"
          << " not enough values. " 
          << (int)number << " required."
          << std::endl;
    return false;
  }
  std::vector<Lib::Value> vals;
  vals.resize(number,Lib::Value(0.));

 {std::vector<double>::const_iterator it1 = aVals.begin();
  std::vector<Lib::Value>::iterator it2 = vals.begin();
  for(;it1!=aVals.end();++it1,++it2) (*it2).set(*it1);}

  Lib::Value res;
  std::string serror;
  if(!proc.execute(vals,res,serror)) {
    a_out << "Lib::calc :"
          << " execution failure of " << inlib::sout(aFormula)
          << ". Error message " << inlib::sout(serror)
          << std::endl;
    return false;
  }
  return Lib::Value::to_double(res,aValue);
}

bool calc(
 std::ostream& a_out
,const std::string& aFormula
,const std::vector<std::string>& aVals
,std::string& aValue
){
  Lib::Processor proc(aFormula,a_out);
  std::vector<std::string> vars;
  if(!proc.findVariables(vars)) {
    a_out << "Lib::calc :"
          << " can't find variables in " << inlib::sout(aFormula) << "."
          << std::endl;
    return false;
  }
  //Lib::smanip::write(vars,printer);
  if(!proc.compile(vars)) {
    a_out << "Lib::calc :"
          << " can't compile " << inlib::sout(aFormula) << "."
          << std::endl;
    return false;
  }
  unsigned int number = vars.size();
  if(aVals.size()!=number) {
    a_out << "Lib::calc :"
          << " not enough values. " 
          << (int)number << " required."
          << std::endl;
    return false;
  }
  std::vector<Lib::Value> vals;
  vals.resize(number,Lib::Value(0.));

 {std::vector<std::string>::const_iterator it1 = aVals.begin();
  std::vector<Lib::Value>::iterator it2 = vals.begin();
  for(;it1!=aVals.end();++it1,++it2) {
    double d;
    if(!inlib::to<double>(*it1,d)) {
      a_out << "Lib::calc :"
            << " argument " << inlib::sout(*it1) << " not a double."
            << std::endl;
      return false;
    }
    (*it2).set(d);
  }}

  Lib::Value res;
  std::string serror;
  if(!proc.execute(vals,res,serror)) {
    a_out << "Lib::calc :"
          << " exection failure of " << inlib::sout(aFormula)
          << ". Error message " << inlib::sout(serror)
          << std::endl;
    return false;
  }

  if(res.type()!=Slash::Core::IValue::DOUBLE) {
    a_out << "Lib::calc :"
          << " result not a double."
          << std::endl;
    return false;
  }

  aValue = Lib::Value::toString(res);
  return true;
}

}

#endif
