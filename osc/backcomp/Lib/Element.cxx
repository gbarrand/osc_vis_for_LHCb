// this :
#include <Lib/Element.h>

////#include <Lib/Debug.h>
#include <Lib/smanip.h>

//////////////////////////////////////////////////////////////////////////////
Lib::Element::Element(
 const std::string& aName
,const std::vector<Attribute>& aAtbs
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fName = aName;
  fAtbs = aAtbs; 
  fValue = aValue;
  ////Lib::Debug::increment("Lib::Element");
}
//////////////////////////////////////////////////////////////////////////////
Lib::Element::Element(
 const Element& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fName = aFrom.fName;
  fAtbs = aFrom.fAtbs; 
  fValue = aFrom.fValue;
  ////Lib::Debug::increment("Lib::Element");
}
//////////////////////////////////////////////////////////////////////////////
Lib::Element::~Element(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::decrement("Lib::Element");
}
//////////////////////////////////////////////////////////////////////////////
std::string Lib::Element::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<Lib::Element::Attribute>& Lib::Element::attributes(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAtbs;
}
//////////////////////////////////////////////////////////////////////////////
std::string Lib::Element::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fValue;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::Element::setValue(
 const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fValue = aValue;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::Element::isAttribute(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int number = fAtbs.size();
  for(unsigned int index=0;index<number;index++) {
    if(fAtbs[index].first==aName) return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::Element::addAttribute(
 const std::string& aName
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
// No check is done about an existing aName.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAtbs.push_back(Attribute(aName,aValue));
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::Element::attributeValue(
 const std::string& aAttribute
,std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int atbn = fAtbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    if(fAtbs[index].first==aAttribute) {
      aValue = fAtbs[index].second;
      return true;
    }
  }
  aValue = "";
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::Element::setAttributeValue(
 const std::string& aAttribute
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int atbn = fAtbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    if(fAtbs[index].first==aAttribute) {
      fAtbs[index].second = aValue;
      return true;
    }
  }
  // Not found, add one :
  fAtbs.push_back(Attribute(aAttribute,aValue));
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::Element::replace(
 const std::string& aOld
,const std::string& aNew
)
//////////////////////////////////////////////////////////////////////////////
// Used by the obuild template system.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int atbn = fAtbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    std::string& value = fAtbs[index].second;
    Lib::smanip::replace(value,aOld,aNew);
  }
  Lib::smanip::replace(fValue,aOld,aNew);
}
