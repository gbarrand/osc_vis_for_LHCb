#ifndef AIDA_Loader_h
#define AIDA_Loader_h

#include <string>

namespace AIDA {

class IAnalysisFactory;

typedef void (*Procedure)();

class Loader {
public:
  Loader(const std::string& name,bool quiet = false,bool verbose = false);
  virtual ~Loader();
  bool open();
  Procedure find(const std::string&) const;
  const std::string& fileName() const;
  bool isOpened() const;
  IAnalysisFactory* findAIDA(const std::string& = "");
protected:
  void* fModule;
  std::string fName;
  std::string fError;
  bool fVerbose;
};

}

#endif




