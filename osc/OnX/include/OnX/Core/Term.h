#ifndef OnX_Term_h
#define OnX_Term_h 

#include <string>

namespace Slash {namespace UI{class IUI;}}

namespace OnX {

class Term {
public:
  Term(const std::string&);
  virtual ~Term();
public:
  bool isValid() const;
  void setPrompt(const std::string&);
  void initPrompt(const std::string&);
  enum eKind { no_input, input, escape};
  bool getInput(eKind&,std::string&);
  std::string prompt() const;
public:
  static bool is_there_stdin_input(bool&);
public:
  virtual std::string ask(const std::string&);
public:
  virtual void cyclicExecute(Slash::UI::IUI&,const std::string&);
  virtual std::string ask(Slash::UI::IUI&,const std::string&);
protected:
  void set_prompt();
protected:
  std::string fPrompt;
  int fCounter;
  std::string f_prompt;
};

}

#endif


