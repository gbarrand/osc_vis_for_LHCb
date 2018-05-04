#ifndef Lib_Element_h
#define Lib_Element_h 

#include <vector>
#include <string>

namespace Lib {

class Element {
public:
  typedef std::pair<std::string,std::string> Attribute;
public:
  Element(const std::string&,
           const std::vector<Attribute>&,
           const std::string&);
  Element(const Element&);
  ~Element();
  std::string name() const;
  const std::vector<Attribute>& attributes() const;
  std::string value() const;
  void setValue(const std::string&);
  bool isAttribute(const std::string&) const;
  bool attributeValue(const std::string&,std::string&) const;
  void addAttribute(const std::string&,const std::string&);
  bool setAttributeValue(const std::string&,const std::string&);
  void replace(const std::string&,const std::string&);
private:
  std::string fName;
  std::vector<Attribute> fAtbs;
  std::string fValue;
};

}

#endif
