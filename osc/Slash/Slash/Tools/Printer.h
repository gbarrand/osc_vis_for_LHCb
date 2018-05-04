#ifndef Slash_Tools_Printer_h
#define Slash_Tools_Printer_h

#include <Slash/Core/IWriter.h>

#include <cstdio>

namespace Slash {

class Printer : public virtual Slash::Core::IWriter {
public: //Lib::Slash::Core::IWriter
  virtual bool write(const std::string& a_string) {
    ::printf("%s",a_string.c_str());
    return true;
  }
  virtual void disable(){}
  virtual void enable(){}
  virtual bool enabled() const {return true;}
  virtual bool flush(){
    if(::fflush(stdout)!=0) return false;
    return true;
  }
public:
  Printer(){}
  Printer(const Printer& a_from):Slash::Core::IWriter(a_from){}
  virtual ~Printer(){}
public:
  Printer& operator =(const Printer&){return *this;}
};

}

#endif
