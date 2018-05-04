//
// To test Cint wrapping.
//

#include <Lib/Out.h>

#include <OnX/CINT/TestCint.h>

TestCint::TestCint(Slash::Core::IWriter& aPrinter):fPrinter(aPrinter) {
  Lib::Out out(fPrinter);
  out << "Executing TestCint::TestCint" << Lib::endl;
}
TestCint::~TestCint() {
  Lib::Out out(fPrinter);
  out << "Executing TestCint::~TestCint" << Lib::endl;
}
void TestCint::dump() {
  Lib::Out out(fPrinter);
  out << "Executing TestCint::dump" << Lib::endl;
}
