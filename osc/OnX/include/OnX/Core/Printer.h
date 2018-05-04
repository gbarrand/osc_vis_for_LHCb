#ifndef OnX_Printer_h
#define OnX_Printer_h

// Inheritance :
#include <Slash/Core/IWriter.h>

namespace Slash {namespace Core {class ISession;}}

namespace OnX {

class Printer : public Slash::Core::IWriter {
public: //Slash::Core::IWriter
  virtual bool write(const std::string&);
  virtual void disable();
  virtual void enable();
  virtual bool enabled() const;
  virtual bool flush();
public:
  Printer(); 
  virtual ~Printer();
  void setSession(Slash::Core::ISession*);
private:
  Slash::Core::ISession* fSession;
  bool fEnabled;
  int fLevel;
  void* fFILE;
};

}

#endif


