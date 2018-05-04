#ifndef Slash_UI_IPlotter_h
#define Slash_UI_IPlotter_h

#include <string>

class SoNode; //Ok since a forward cast does not tie to a library.

namespace Slash {

namespace UI {

class IPlotter {
public:
  virtual ~IPlotter() {}
public:
  virtual void* cast(const std::string&) const = 0;
public:
  virtual SoNode* nodeOfInterest(const std::string& what) const = 0;
};

} //UI

} //Slash

#endif
