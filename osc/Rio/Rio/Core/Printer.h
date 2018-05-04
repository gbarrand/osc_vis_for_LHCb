#ifndef Rio_Printer_h
#define Rio_Printer_h

#include <Rio/Interfaces/IPrinter.h>

namespace Rio {

class Printer : public virtual IPrinter {
public: //Rio::IPrinter
  virtual std::ostream& out() const;
  virtual void indent();
  virtual void increment();
  virtual void decrement();
public:
  Printer(std::ostream&);
  Printer(const Printer&);
  virtual ~Printer();
  Printer& operator=(const Printer&);
private:
  std::ostream& fOut;
  int fLevel;
};

}

#endif
