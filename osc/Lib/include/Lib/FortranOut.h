#ifndef Lib_FortranOut_h
#define Lib_FortranOut_h

#include <Lib/FortranFormat.h>

#include <inlib/sjust>
#include <inlib/sprintf>

/*
   When there is too few data compared to what the format describe,
  we stop (without error) at the end of data (like FORTRAN). For example :
    FormatOut out(<someWriter>,"(2(F10.2,1X,I5))");   
    out << 3.14 << 314 << Lib::endl;
  will write the two numbers and stop here.

   When the format is "exhausted" (too much data), 
  we go to next line and start again the format.
    FormatOut out(<someWriter>,"(F10.2,1X,I5)");   
    out << 3.14 << << 314 << 3.14 << 314 << Lib::endl;
  will write :
    3.14 314
    3.14 314
*/

namespace Lib {

class FortranOut {
public:
  static FortranOut& endl(FortranOut& aOut) {
    if(!aOut.status()) return aOut;
    aOut.out() << std::endl;
    return aOut;
  }
public:
  FortranOut(std::ostream& a_out,std::ostream& a_msg,
             const std::string& a_format,int a_verbose = 0)
  :f_out(a_out)
  ,f_msg(a_msg)
  ,fVerboseLevel(a_verbose)
  ,fParser(a_format,a_msg)
  ,fIterator(0)
  ,fNext(0)
  ,fStatus(false)
  {
    if(!fParser.parse()) {
      f_msg << "Lib::FortranOut::FortranOut : " 
            << " parse of " << inlib::sout(a_format) << " failed." 
            << std::endl;
      return;
    } 
    if(fVerboseLevel) fParser.result().dump(f_msg);
    fIterator = fParser.result().createIterator();
    reset();
  }

  virtual ~FortranOut() {delete fIterator;}
private:
  FortranOut(const FortranOut& aFrom)
  :f_out(aFrom.f_out)
  ,f_msg(aFrom.f_msg)
  ,fParser("",f_msg)
  {}
  FortranOut& operator =(const FortranOut&) {return *this;}
public:
  FortranOut& operator<<(long a_value){
    if(!fStatus) return *this;
    if(!fNext) fNext = next();
    if(fIterator->finished()) {
      f_out << std::endl;
      fIterator->reset();
      fNext = next();
    }
  
    DataDescriptor* desc = fNext;
    if(!desc) return *this; //No data desc in format !
  
    if(desc->fType!=DataDescriptor_I) {
      f_msg << "Lib::FortranOut::operator<<(long) :"
            << " format mismatch." << std::endl;
      desc->dump(f_msg);
      fStatus = false;
      return *this;
    }
    if(desc->fWidth<=0) {
      f_msg << "Lib::FortranOut::operator<<(long) :"
            << " null width." << std::endl;
      desc->dump(f_msg);
      return *this;
    }
    //desc->dump(f_msg);
    std::string s;
    if(!inlib::sprintf(s,desc->fWidth,desc->cformat().c_str(),a_value)) {
      f_msg << "Lib::FortranOut::operator<<(long) :"
            << " something wrong in sprintf." << std::endl;
      fStatus = false;
      return *this;
    }
    f_out << s;
    fNext = next();
    return *this;
  }
  FortranOut& operator<<(double a_value) {
    if(!fStatus) return *this;
    if(!fNext) fNext = next();
    if(fIterator->finished()) {
      f_out << std::endl;
      fIterator->reset();
      fNext = next();
    }
    DataDescriptor* desc = fNext;
    if(!desc) return *this; //No data desc in format !
  
    if( (desc->fType!=DataDescriptor_F) &&
        (desc->fType!=DataDescriptor_E) &&
        (desc->fType!=DataDescriptor_G) ){
      f_msg << "Lib::FortranOut::operator<<(double) :"
            << " format mismatch." << std::endl;
      desc->dump(f_msg);
      fStatus = false;
      return *this;
    }
    if(desc->fWidth<=0) {
      f_msg << "Lib::FortranOut::operator<<(double) :"
            << " null width." << std::endl;
      desc->dump(f_msg);
      return *this;
    }
    //desc->dump(f_msg);
    std::string s;
    if(!inlib::sprintf(s,desc->fWidth,desc->cformat().c_str(),a_value)) {
      f_msg << "Lib::FortranOut::operator<<(double) :"
            << " something wrong in sprintf." << std::endl;
      fStatus = false;
      return *this;
    }
    f_out << s;
    fNext = next();
    return *this;
  }

  FortranOut& operator<<(bool a_value) {
    if(!fStatus) return *this;
    if(!fNext) fNext = next();
    if(fIterator->finished()) {
      f_out << std::endl;
      fIterator->reset();
      fNext = next();
    }
  
    DataDescriptor* desc = fNext;
    if(!desc) return *this; //No data desc in format !
  
    if(desc->fType!=DataDescriptor_L) {
      f_msg << "Lib::FortranOut::operator<<(bool) :"
            << " format mismatch." << std::endl;
      desc->dump(f_msg);
      fStatus = false;
      return *this;
    }
    if(desc->fWidth<=0) {
      f_msg << "Lib::FortranOut::operator<<(bool) :"
            << " null width." << std::endl;
      desc->dump(f_msg);
      return *this;
    }
    //desc->dump(f_msg);
    std::string s;
    if(a_value) s = "T";
    else       s = "F";
    inlib::justify(s,desc->fWidth,inlib::side_right);
    f_out << s;
    fNext = next();
    return *this;
  }

  FortranOut& operator<<(const std::string& a_value) {
    if(!fStatus) return *this;
    if(!fNext) fNext = next();
    if(fIterator->finished()) {
      f_out << std::endl;
      fIterator->reset();
      fNext = next();
    }
  
    DataDescriptor* desc = fNext;
    if(!desc) return *this; //No data desc in format !
  
    if(desc->fType!=DataDescriptor_A) {
      f_msg << "Lib::FortranOut::operator<<(const std::string&) :"
            << " format mismatch." << std::endl;
      desc->dump(f_msg);
      fStatus = false;
      return *this;
    }
    //desc->dump(f_msg);
    if(desc->fWidth>0) { 
      std::string s(a_value);
      inlib::justify(s,desc->fWidth,inlib::side_right);
      f_out << s;
    } else {
      f_out << a_value;
    }
    fNext = next();
    return *this;
  }
  
public:
  FortranOut& operator<<(const char* a_value){
    return (*this) << std::string(a_value);
  }
  FortranOut& operator<<(int a_value){return (*this) << (long)a_value;}
  FortranOut& operator<<(float a_value){return (*this) << (double)a_value;}
  FortranOut& operator<<(unsigned int a_value){return (*this)<<(long)a_value;}
  FortranOut& operator<<(unsigned long a_value){return (*this)<<(long)a_value;}
  FortranOut& operator<<(const void* a_value){return (*this)<<(long)a_value;}

  typedef FortranOut& (*FortranOutFunc)(FortranOut&); //For Lib::endl.
  FortranOut& operator<<(FortranOutFunc aFunc){return aFunc(*this);}

  FortranOut& operator<<(char a_value) {
    std::string s;
    s += a_value;
    return (*this) << s;
  }

  FortranOut& operator<<(const std::vector<double>& a_value) {
    if(!fStatus) return *this;
    std::vector<double>::const_iterator it;
    for(it=a_value.begin();it!=a_value.end();++it) {
      (*this) << *it;
      if(!fStatus) break;
    }
    return *this;
  }
public:
  std::ostream& out() const {return f_out;}
  std::ostream& msg() const {return f_msg;}

  void reset() {
    if(!fIterator) return;
    // If we have an iterator, the given format had been parsed and is ok.
    fIterator->reset();
    fNext = 0;
    fStatus = true;
  }
  bool status() const {return fStatus;}
private:
  DataDescriptor* next() {
    while(true) {
      Lib::DataDescriptor* desc = fIterator->next();
      if(!desc) return 0;
      if(desc->fType==DataDescriptor_X) {
        f_out << " ";
      } else if(desc->fType==DataDescriptor_SLASH) {
        f_out << std::endl;
      } else {
        return desc;
      }
    }
  }
private:
  std::ostream& f_out;
  std::ostream& f_msg;
  int fVerboseLevel;
  std::string fFormat;
  FortranFormat fParser;
  DataDescriptorIterator* fIterator;
  DataDescriptor* fNext;
  bool fStatus;
};

}

#endif
