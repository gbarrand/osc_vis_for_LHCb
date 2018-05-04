#ifndef Lib_ProcessorLexer_h
#define Lib_ProcessorLexer_h 

#include <vector>
#include <string>
#include <ostream>

namespace Lib {

class ProcessorLexer {
public:
  ProcessorLexer(std::ostream&);
  ProcessorLexer(const ProcessorLexer&);
  ProcessorLexer& operator=(const ProcessorLexer&);
public:
  void set(const std::string&);
  std::vector<std::string>& variables();
  const std::string& variable(unsigned int) const;
  std::ostream& out() const;
public:
  int input(char*,int); // Can't be avoided.
private:
  //const std::string& fString;
  std::string fString;
  char* fCurrent;
  char* fEnd;
  std::vector<std::string> fVariables;
  std::ostream& f_out;
};

}

#endif
