#ifndef Lib_FortranFormatLexer_h
#define Lib_FortranFormatLexer_h 

#include <string>
#include <ostream>

namespace Lib {

// For FotranFormatToken.h :
class DataDescriptor;

class FortranFormatLexer {
public:
  FortranFormatLexer(std::ostream&,int = 0);
private:
  FortranFormatLexer(const FortranFormatLexer&);
  FortranFormatLexer& operator=(const FortranFormatLexer&);
public:
  void set(const std::string&);
  std::ostream& out() const;
  int verboseLevel() const;
public:
  int input(char*,int); // Can't be avoided.
private:
  int fVerboseLevel;
  std::string fString;
  char* fCurrent;
  char* fEnd;
  std::ostream& f_out;
};

}

#endif
