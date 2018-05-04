#ifndef Slash_UI_IIB_h
#define Slash_UI_IIB_h

#include <string>
#include <vector>

namespace Slash {namespace UI {class IWidgetClass;}}

// inlib::xml::tree is not an interface.
// But we put IIB anyway in Slash to remove
// the dependancy of the OnX plugin toward OnX
// includes.

namespace inlib {namespace xml {class tree;}}

namespace Slash {
namespace UI {

class IIB {
public:
  virtual ~IIB() {};
public:
  virtual std::string tree() const = 0;
  virtual std::vector< std::pair<bool,std::string> > parseDecorations(const std::string&) const =0;
  virtual const std::vector<std::string>& callbackLanguageList() const = 0;
  virtual bool propertyType(const std::string&,std::string&) const = 0;

  virtual bool loadReferenceFile(inlib::xml::tree&, const std::string&) = 0;
  virtual inlib::xml::tree* topItemML() const = 0;
};

} //UI
} //Slash

#endif
