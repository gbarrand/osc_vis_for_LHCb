
//NOTE : we put this .cxx here to be able to build
//       an AIDA app with "pure headers".

#include <BatchLab/Core/Main.h>

#include <inlib/args>

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 

extern "C" {
AIDA::IAnalysisFactory* AIDA_createAnalysisFactory(){
  return new BatchLab::Main(std::vector<std::string>());
}
}

#else

extern "C" {
AIDA::IAnalysisFactory* AIDA_createAnalysisFactory(const std::string& aOptions){
  inlib::args args(aOptions,";",true);
  return new BatchLab::Main(args.tovector());
}
}

#endif

extern "C" {
Slash::AIDA::IAnalysisFactory* Slash_AIDA_createAnalysisFactory(const std::string& aOptions){
  inlib::args args(aOptions,";",true);
  return new BatchLab::Main(args.tovector());
}
}

