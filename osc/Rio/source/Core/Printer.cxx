// this :
#include <Rio/Core/Printer.h>

Rio::Printer::Printer(std::ostream& aOut):fOut(aOut),fLevel(0){}

Rio::Printer::Printer(const Printer& aFrom)
:IPrinter()
,fOut(aFrom.fOut),fLevel(aFrom.fLevel)
{}

Rio::Printer::~Printer(){}

Rio::Printer& Rio::Printer::operator=(const Printer& aFrom){
  //fOut
  fLevel = aFrom.fLevel;
  return *this;
}

std::ostream& Rio::Printer::out() const {return fOut;}

void Rio::Printer::indent() {
  // Indent level :
  for (int i = 0; i < fLevel; i++) fOut << " ";
}

void Rio::Printer::increment() {fLevel++;}

void Rio::Printer::decrement() {fLevel--;}
