#ifndef Slash_UI_IStyle_h
#define Slash_UI_IStyle_h

#include <string>
#include <map>

namespace Slash {
namespace UI {

class IStyle {
public:
  virtual ~IStyle() {};
public:
  virtual void set(const std::string& key,const std::string& value) = 0;
  virtual void remove(const std::string& key) = 0;
  virtual bool value(const std::string& key,std::string& value) const = 0;
  virtual std::map<std::string,std::string> items() const = 0;

  virtual void copy(const Slash::UI::IStyle& from) = 0;

  virtual std::string toString() const = 0;
};

}}

#endif
