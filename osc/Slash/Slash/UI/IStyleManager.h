#ifndef Slash_UI_IStyleManager_h
#define Slash_UI_IStyleManager_h

#include <string>
#include <vector>

namespace Slash {

namespace UI {

class IStyleManager {
public:
  virtual ~IStyleManager() {};
public:
  virtual bool isStyle(const std::string&) const = 0;
  virtual std::vector<std::string> styles() const = 0;
  virtual void removeStyle(const std::string& name) = 0;

  // various ways to add styles :
  virtual bool loadFile(const std::string&) = 0;
  virtual bool loadString(const std::string&) = 0;
  // value is a "\n" separated list of [<name> <value>].
  // If name exists, we override previous value.  
  virtual void addStyle(const std::string& name,const std::string& value) = 0;

  // various ways to get a style :
  virtual bool findStyle(const std::string&,std::string&) const = 0;

};

} //UI

} //Slash

#endif
