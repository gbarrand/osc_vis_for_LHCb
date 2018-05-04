#ifndef Rio_Args_h
#define Rio_Args_h

#include <vector>
#include <string>

namespace Rio {

class Args {
public:
  Args(int,char**);
  Args(const Args&);
  virtual ~Args();
  Args& operator=(const Args&);
  bool isAnArgument(const std::string&) const;
  unsigned int number() const;
  bool find(const std::string&,std::string&) const;
  bool last(std::string&) const;
private:
  std::vector<std::string> fArgs;
};

}

#endif



