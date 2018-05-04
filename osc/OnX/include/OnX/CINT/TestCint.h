//
// To test Cint wrapping.
//

namespace Slash {namespace Core {class IWriter;}}

class TestCint {
public:
 TestCint(Slash::Core::IWriter&);
 virtual ~TestCint();
 void dump();
private:
 Slash::Core::IWriter& fPrinter;
};
