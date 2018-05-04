#ifndef Lib_ColumnsLexer_h
#define Lib_ColumnsLexer_h 

#include <vector>
#include <string>
#include <iostream>

namespace Lib {

class ColumnsLexer {
public:
  ColumnsLexer(std::ostream&,int = 0);
private:
  ColumnsLexer(const ColumnsLexer& a_from):f_out(a_from.f_out){}
public:
  ColumnsLexer& operator=(const ColumnsLexer&);
public:
  void set(const std::string&);
  std::vector<std::string>& variables();
  std::string variable(unsigned int) const;
  std::ostream& out() const;
  int verboseLevel() const;
public:
  int input(char*,int); // Can't be avoided.
private:
  int fVerboseLevel;
  std::string fString;
  char* fCurrent;
  char* fEnd;
  std::vector<std::string> fVariables;
  std::ostream& f_out;
};

}

#endif
