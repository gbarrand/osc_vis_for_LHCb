
// this :
#include <Lib/ItemML.h>

#include <Lib/Out.h>
////#include <Lib/Debug.h>
#include <Lib/smanip.h>
#include <Lib/sout.h>
#include <Lib/Cast.h>
#include <Lib/Element.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML::ItemML(
 const std::string& aTagName
,ItemMLFactory& aItemMLFactory
,ItemML* aParent
)
:fTagName(aTagName)
,fItemMLFactory(aItemMLFactory)
,fParent(aParent)
,fSaveFlag(true)
,fData1(0)
,fData2(0)
,fDataInt(0)
,fDepth(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::increment("Lib::ItemML");
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML::~ItemML(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  clear();

  ////Lib::Debug::decrement("Lib::ItemML");
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML::ItemML(
 const ItemML& aFrom
)
:fTagName(aFrom.fTagName)
,fItemMLFactory(aFrom.fItemMLFactory)
,fParent(0)
,fSaveFlag(0)
,fData1(0)
,fData2(0)
,fDataInt(0)
,fDepth(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::increment("Lib::ItemML");
}
Lib::ItemML& Lib::ItemML::operator=(const ItemML&){return *this;}
//////////////////////////////////////////////////////////////////////////////
void* Lib::ItemML::cast(
 const std::string& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if_Lib_SCast(Lib::ItemML)
  else return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::clear(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAtbs.clear();

 {std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();it = fChildren.erase(it)) 
    delete (*it);}

 {std::list<Element*>::iterator it;
  for(it=fProperties.begin();it!=fProperties.end();it = fProperties.erase(it)) 
    delete (*it);}

}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::addAttribute(
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
void Lib::ItemML::addProperty(
 const std::string& aName
,const std::vector<Attribute>& aAtbs
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fProperties.push_back(new Element(aName,aAtbs,aValue));
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::isProperty(
 const std::string& aProperty
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Element*>::const_iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    if(aProperty==(*it)->name()) return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::isAttribute(
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
bool Lib::ItemML::propertyValue(
 const std::string& aProperty
,std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  std::list<Element*>::const_iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    if(aProperty==(*it)->name()) {
      aValue = (*it)->value();
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::propertyValues(
 const std::string& aProperty
,std::vector<std::string>& aValues
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValues.clear();
  std::list<Element*>::const_iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    if(aProperty==(*it)->name()) {
      aValues.push_back((*it)->value());
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::deleteProperty(
 const std::string& aProperty
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Element*>::iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    if(aProperty==(*it)->name()) {
      fProperties.remove(*it);
      delete (*it);
      break;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::deleteProperty(
  Lib::Element* aProperty
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fProperties.remove(aProperty);
  delete aProperty;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::print(
 Slash::Core::IWriter& aPrinter
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Out out(aPrinter);
  out << "Lib::ItemML::print :" 
      << " -----> " << this << " parent : " << fParent << Lib::endl;
  out << " data1 : " << fData1 
      << " data2 : " << fData2 
      << " dataInt : " << fDataInt
      << Lib::endl;

 {unsigned int atbn = fAtbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    out << " attribute : " << Lib::sout(fAtbs[index].first) << " " 
        << Lib::sout(fAtbs[index].second) << Lib::endl;
  }}

 {std::list<Element*>::iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    out << " property : \"" << (*it)->name() << "\" \"" 
        << (*it)->value() << "\"" << Lib::endl;
  }}

 {std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    (*it)->print(aPrinter);
  }}
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::printXML(
 Slash::Core::IWriter& aPrinter
,const std::string& aSpaces
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Out out(aPrinter);

  // begin tag :
  out << aSpaces << "<" << fTagName;
 {unsigned int atbn = fAtbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    out << " " << fAtbs[index].first << "=" << Lib::sout(fAtbs[index].second);
  }}
  out << ">" << Lib::endl;

 {std::list<Element*>::const_iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {

    const std::vector<Attribute>& atbs = (*it)->attributes();

    bool isCallback = false;

    out << aSpaces << "  <" << (*it)->name();
    unsigned int atbn = atbs.size();
    for(unsigned int index=0;index<atbn;index++) {
      out << " " << atbs[index].first << "=" << Lib::sout(atbs[index].second);
      if(atbs[index].first=="exec") isCallback = true;
    }
    if((*it)->value().empty()) {
      out << "/>" << Lib::endl;
    } else {
      out << ">";
      std::string value = Lib::smanip::toxml((*it)->value());
      if(isCallback) {
        if(value.find("\\n\\")==std::string::npos) {
          out << value;
        } else {
          out << Lib::endl;
          Lib::smanip::replace(value,"\\n\\","@OnX@");
          Lib::smanip::replace(value,"@OnX@","\\n\\\n");
          Lib::smanip::strip(value,Lib::smanip::trailing,' ');
          out << value;
          unsigned int l = value.size();
          if(l && value[l-1]!='\n') out << Lib::endl;
          out << aSpaces << "  ";
        }
      } else {
        out << value;
      }
      out << "</" << (*it)->name() << ">" << Lib::endl;
    }
  }}

  // End tag :
  out << aSpaces << "</" << fTagName << ">" << Lib::endl;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::setAttributes(
 const std::vector<Attribute>& aAtbs
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAtbs = aAtbs; 
}
//////////////////////////////////////////////////////////////////////////////
const std::list<Lib::Element*>& Lib::ItemML::properties(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fProperties;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Lib::ItemML::tagName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fTagName;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::attributeValue(
 const std::string& aAtt
,std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  unsigned int linen = fAtbs.size();
  for(unsigned int count=0;count<linen;count++) {
    if(fAtbs[count].first==aAtt) {
      aValue = fAtbs[count].second;
      return true;
    }
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::attributeValue(
 const std::string& aProperty
,const std::string& aAtt
,std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aValue = "";
  if(aProperty.empty()) {
    unsigned int linen = fAtbs.size();
    for(unsigned int count=0;count<linen;count++) {
      if(fAtbs[count].first==aAtt) {
        aValue = fAtbs[count].second;
        return true;
      }
    }
    return false;
  } else {
    // Get on the first match.
    std::list<Element*>::const_iterator it;
    for(it=fProperties.begin();it!=fProperties.end();++it) {
      if(aProperty==(*it)->name()) {
        if((*it)->attributeValue(aAtt,aValue)) return true;
      }
    }
    return false;
  }
}
/////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::removeAttribute(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Attribute>::iterator it = fAtbs.begin();
  unsigned int linen = fAtbs.size();
  for(unsigned int count=0;count<linen;count++,it++) {
    if(fAtbs[count].first==aName) {
      fAtbs.erase(it);
      return true; //Found and removed.
    }
  }
  return false; //Not found.
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::setData(
 void* aData1
,void* aData2
,int aDataInt
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fData1 = aData1;
  fData2 = aData2;
  fDataInt = aDataInt;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::getData(
 void*& aData1
,void*& aData2
,int& aDataInt
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aData1 = fData1;
  aData2 = fData2;
  aDataInt = fDataInt;
}
//////////////////////////////////////////////////////////////////////////////
void* Lib::ItemML::getData1(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData1;
}
//////////////////////////////////////////////////////////////////////////////
void* Lib::ItemML::getData2(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData2;
}
//////////////////////////////////////////////////////////////////////////////
int Lib::ItemML::getDataInt(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDataInt;
}
bool Lib::ItemML::invalidate() {return true;}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::setDepth(
 unsigned int aDepth
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fDepth = aDepth;
}
//////////////////////////////////////////////////////////////////////////////
unsigned int Lib::ItemML::depth(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDepth;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::setSaveFlag(
 bool aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fSaveFlag = aValue;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::saveFlag(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fSaveFlag;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::setFile(
 const std::string& aFile
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fFile = aFile;
}
//////////////////////////////////////////////////////////////////////////////
std::string Lib::ItemML::file(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFile;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::setPropertyValue(
 const std::string& aProperty
,const std::string& aValue
,int aIndex
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Element*>::const_iterator it;
  int index = 0;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    if(aProperty==(*it)->name()) {
      if(index==aIndex) {
        (*it)->setValue(aValue);
        return true;
      } else {
        index ++;
      }
    }
  }
  // Not found, add one :
  std::vector<Attribute> atts;
  addProperty(aProperty,atts,aValue);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::setAttributeValue(
 const std::string& aProperty
,const std::string& aAtt
,const std::string& aValue
,int aIndex
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aProperty.empty()) {
    unsigned int linen = fAtbs.size();
    for(unsigned int count=0;count<linen;count++) {
      if(fAtbs[count].first==aAtt) {
        fAtbs[count].second = aValue;
        return true;
      }
    }
    // Not found, add it :
    fAtbs.push_back(Attribute(aAtt,aValue));
    return true;
  } else {
    std::list<Element*>::const_iterator it;
    int index = 0;
    for(it=fProperties.begin();it!=fProperties.end();++it) {
     if(aProperty==(*it)->name()) {
        if(index==aIndex) {
          (*it)->setAttributeValue(aAtt,aValue);
          return true;
        } else { 
          index ++;
        }
      }
    }
    return false;
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::propertyValueBoolean(
 const std::string& aProperty
,bool& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string value;
  if(!propertyValue(aProperty,value)) return false;
  return smanip::tobool(value,aValue);
}
//////////////////////////////////////////////////////////////////////////////
const std::vector<Lib::ItemML::Attribute>& Lib::ItemML::attributes(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAtbs;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::postExecute(
 ExecFunction aFunction
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aFunction) return;
  
  if(!aFunction(*this,aTag)) return;

  std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    (*it)->postExecute(aFunction,aTag);
  }
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::postExecuteBackward(
 ExecFunction aFunction
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aFunction) return;
  
  std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    (*it)->postExecuteBackward(aFunction,aTag);
  }

  if(!aFunction(*this,aTag)) return;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::parent(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fParent;
}
//////////////////////////////////////////////////////////////////////////////
const std::list<Lib::ItemML*>& Lib::ItemML::children(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fChildren;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::findItem(
 const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //OPTIMIZE:
  //std::string s;
  //attributeValue("","name",s);
  //if(aName==s) return const_cast<Lib::ItemML*>(this);

 {unsigned int linen = fAtbs.size();
  for(unsigned int count=0;count<linen;count++) {
    if(fAtbs[count].first=="name") {
      if(fAtbs[count].second==aName) return const_cast<Lib::ItemML*>(this);
      break;
    }
  }}

  // Look children :
  std::list<ItemML*>::const_iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    ItemML* item = (*it)->findItem(aName);
    if(item) return item;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::findItemWithTag(
 const std::string& aTag
,const std::string& aName
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aTag==tagName()) {
    std::string s;
    attributeValue("","name",s);
    if(aName==s) return const_cast<Lib::ItemML*>(this);
  }

  // Look children :
  std::list<ItemML*>::const_iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    ItemML* item = (*it)->findItemWithTag(aTag,aName);
    if(item) return item;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::findByAttribute(
 const std::string& aAttribute
,const std::string& aValue
,bool aUpDown //true
,bool aLeftRight //true
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aUpDown) {
    std::string s;
    attributeValue("",aAttribute,s);
    if(s==aValue) return const_cast<Lib::ItemML*>(this);

    if(aLeftRight) {
      std::list<ItemML*>::const_iterator it;
      for(it=fChildren.begin();it!=fChildren.end();++it) {
        ItemML* itemML = 
          (*it)->findByAttribute(aAttribute,aValue,aUpDown,aLeftRight);
        if(itemML) return itemML;
      }
    } else {
      std::list<ItemML*>::const_reverse_iterator it;
      for(it=fChildren.rbegin();it!=fChildren.rend();++it) {
        ItemML* itemML = 
          (*it)->findByAttribute(aAttribute,aValue,aUpDown,aLeftRight);
        if(itemML) return itemML;
      }
    }
  } else {
    if(aLeftRight) {
      std::list<ItemML*>::const_iterator it;
      for(it=fChildren.begin();it!=fChildren.end();++it) {
        ItemML* itemML = 
          (*it)->findByAttribute(aAttribute,aValue,aUpDown,aLeftRight);
        if(itemML) return itemML;
      }
    } else {
      std::list<ItemML*>::const_reverse_iterator it;
      for(it=fChildren.rbegin();it!=fChildren.rend();++it) {
        ItemML* itemML = 
          (*it)->findByAttribute(aAttribute,aValue,aUpDown,aLeftRight);
        if(itemML) return itemML;
      }
    }

    std::string s;
    attributeValue("",aAttribute,s);
    if(s==aValue) return const_cast<Lib::ItemML*>(this);

  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::findByTag(
 const std::string& aTag
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(tagName()==aTag) return const_cast<Lib::ItemML*>(this);
  // Look children :
  std::list<ItemML*>::const_iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    ItemML* itemML = (*it)->findByTag(aTag);
    if(itemML) return itemML;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::addChild(
 Lib::ItemML* aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fChildren.push_back(aItemML);
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::setParent(
 Lib::ItemML* aParent
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fParent = aParent;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::addAfter(
 Lib::ItemML* aItemML
,Lib::ItemML* aEntry
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    if((*it)==aItemML) {
      fChildren.insert(it,aEntry);
      return;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::isFirstChild(
 const Lib::ItemML& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fChildren.empty()) return false;
  return ( (*(fChildren.begin()))==&aItemML) ? true : false;
}
//////////////////////////////////////////////////////////////////////////////
int Lib::ItemML::numberOfChildren(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fChildren.size();
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::isLastChild(
 const Lib::ItemML& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fChildren.empty()) return false;
  return ( fChildren.back()==&aItemML) ? true : false;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::previousChild(
 const Lib::ItemML& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    if((*it)==&aItemML) {
      if(it==fChildren.begin()) return 0;
      --it;
      return *it;
    }
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::nextChild(
 const Lib::ItemML& aItemML
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    if((*it)==&aItemML) {
      std::list<ItemML*>::iterator last = fChildren.end();
      --last;
      if(it==last) return 0;
      ++it;
      return *it;
    }
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::removeChild(
 Lib::ItemML*& aItemML
,bool aDelete //default true
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fChildren.remove(aItemML);
  if(aDelete) {
    delete aItemML;
    aItemML = 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::replaceChild(
 Lib::ItemML* aOld
,Lib::ItemML* aNew
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    if((*it)==aOld) {
      (*it) = aNew;
      return true; //replacement done.
    }
  }
  return false; //no replacement done.
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::deleteChildren(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();it = fChildren.erase(it)) 
    delete (*it);}
}
//////////////////////////////////////////////////////////////////////////////
const Lib::ItemML* Lib::ItemML::findByPropertyInSameLevel(
 const std::string& aName
,const std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Look children :
  std::list<ItemML*>::const_iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    std::string s;
    if((*it)->propertyValue(aName,s) && (aValue==s)) return *it;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemML::createCopy(
 Lib::ItemML* aParent
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ItemML* itemML = fItemMLFactory.create(fTagName,fAtbs,aParent);
  if(!itemML) return 0;
  itemML->fAtbs = fAtbs;
  //FIXME : fSaveFlag
  
 {std::list<Element*>::const_iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    itemML->fProperties.push_back(new Element(*(*it)));
  }}

 {std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    //FIXME : could we have mismatch parent/child ?
    ItemML* obj = (*it)->createCopy(itemML);
    if(!obj) {
      delete itemML;
      return 0;
    }
    itemML->addChild(obj);
  }}

  return itemML;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::copy(
 const Lib::ItemML& aFrom
,eCopyWhat aWhat //def = all
,bool aClear //=true
)
//////////////////////////////////////////////////////////////////////////////
// Copy data (atbs, propertis, children) of aFrom onto this.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if((aWhat==ALL)||(aWhat==ATTRIBUTES)) {
    if(aClear) fAtbs.clear();
    
   {std::vector<Attribute>::const_iterator it;
    for(it=aFrom.fAtbs.begin();it!=aFrom.fAtbs.end();++it) {
      fAtbs.push_back(*it);
    }}
  }

  if((aWhat==ALL)||(aWhat==PROPERTIES)) {

    if(aClear) {
      std::list<Element*>::iterator it;
      for(it=fProperties.begin();
          it!=fProperties.end();
          it = fProperties.erase(it)) delete (*it);
    }

    std::list<Element*>::const_iterator it;
    for(it=aFrom.fProperties.begin();it!=aFrom.fProperties.end();++it) {
      fProperties.push_back(new Element(*(*it)));
    }
  }

  if((aWhat==ALL)||(aWhat==CHILDREN)) {

    if(aClear) {
      std::list<ItemML*>::iterator it;
      for(it=fChildren.begin();
          it!=fChildren.end();
          it = fChildren.erase(it)) delete (*it);
    }

   {std::list<ItemML*>::const_iterator it;
    for(it=aFrom.fChildren.begin();it!=aFrom.fChildren.end();++it) {
      //FIXME : could we have mismatch parent/child ?
      ItemML* obj = (*it)->createCopy(this);
      if(!obj) {
        // Something wrong, cleanup this.
        clear();
        return false;
      }
      addChild(obj);
    }}
  }

  //FIXME : fSaveFlag
  
  return true;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::updateTree(
 const Lib::ItemML& aOld
)
//////////////////////////////////////////////////////////////////////////////
// We map the opened elements in the old tree within this tree.
// Algorithm based on the existence of a "label" property.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  const std::list<Lib::ItemML*>& list = children();
  std::list<Lib::ItemML*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) {
    std::string slabel;
    if((*it)->propertyValue("label",slabel)) {
      // Try to find a same label in the same level :
      const Lib::ItemML* item = aOld.findByPropertyInSameLevel("label",slabel);
      if(item) {
        bool sopened;
        if(item->propertyValueBoolean("opened",sopened) && sopened) {
          (*it)->setPropertyValue("opened","true");
        }
        if(aOld.numberOfChildren()) (*it)->updateTree(*item);
      }
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::replace(
 const std::string& aOld
,const std::string& aNew
)
//////////////////////////////////////////////////////////////////////////////
// Used by the obuild template system.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {unsigned int atbn = fAtbs.size();
  for(unsigned int index=0;index<atbn;index++) {
    std::string& value = fAtbs[index].second;
    Lib::smanip::replace(value,aOld,aNew);
  }}

 {std::list<Element*>::iterator it;
  for(it=fProperties.begin();it!=fProperties.end();++it) {
    (*it)->replace(aOld,aNew);
  }}

 {std::list<ItemML*>::iterator it;
  for(it=fChildren.begin();it!=fChildren.end();++it) {
    (*it)->replace(aOld,aNew);
  }}
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
namespace Lib {
class EmptyAttributeVisitor {
public:
  EmptyAttributeVisitor(Slash::Core::IWriter& aPrinter)
  :fPrinter(aPrinter),fStatus(true){}
public:
  Slash::Core::IWriter& fPrinter;
  bool fStatus;
};
}
//////////////////////////////////////////////////////////////////////////////
static bool checkItem(
 Lib::ItemML& aItemML
,void* aTag
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::EmptyAttributeVisitor* visitor = (Lib::EmptyAttributeVisitor*)aTag;

 {const std::vector<Lib::ItemML::Attribute>& atbs = aItemML.attributes();
  unsigned int atbn = atbs.size();       
  for(unsigned int index=0;index<atbn;index++) {
    const std::string& atb = atbs[index].first;
    const std::string& atbv = atbs[index].second;
    if(atbv.empty()) {
      Lib::Out out(visitor->fPrinter);
      out << "Lib::ItemML::checkItem :"
          << " for XML item " << Lib::sout(aItemML.tagName())
          << ", attribute " << Lib::sout(atb) << " has an empty value."
          << Lib::endl;
      visitor->fStatus = false;
    }
  }}

  const std::list<Lib::Element*> props = aItemML.properties();
  std::list<Lib::Element*>::const_iterator it;
  for(it=props.begin();it!=props.end();++it) {
    const std::vector<Lib::ItemML::Attribute>& atbs = (*it)->attributes();
    unsigned int atbn = atbs.size();       
    for(unsigned int index=0;index<atbn;index++) {
      const std::string& atb = atbs[index].first;
      const std::string& atbv = atbs[index].second;
      if(atbv.empty()) {
        Lib::Out out(visitor->fPrinter);
        out << "Lib::ItemM::checkItem :"
            << " for XML item " << Lib::sout(aItemML.tagName())
            << ", attribute " << Lib::sout(atb) << " has an empty value."
            << Lib::endl;
        visitor->fStatus = false;
      }
    }
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::hasEmptyAttributeValue(
 Slash::Core::IWriter& aPrinter
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::EmptyAttributeVisitor visitor(aPrinter);
  visitor.fStatus = true;
  const_cast<Lib::ItemML*>(this)->postExecute(checkItem,&visitor);
  return visitor.fStatus;
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::collectByTag(
 Lib::ItemML& aItemML
,const std::string& aTag
,std::vector<Lib::ItemML*>& aItems
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aItemML.tagName()==aTag) aItems.push_back(&aItemML);

  const std::list<Lib::ItemML*>& list = aItemML.children();
  std::list<Lib::ItemML*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) collectByTag(*(*it),aTag,aItems);
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::collectByAttribute(
 Lib::ItemML& aItemML
,const std::string& aTag
,std::vector<Lib::ItemML*>& aItems
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string value;
  if(aItemML.attributeValue(aTag,value)) aItems.push_back(&aItemML);    

  const std::list<Lib::ItemML*>& list = aItemML.children();
  std::list<Lib::ItemML*>::const_iterator it;
  for(it=list.begin();it!=list.end();++it) 
    collectByAttribute(*(*it),aTag,aItems);
}
//////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::unique(
 std::vector<Lib::ItemML*>& aItems
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Lib::ItemML*> items2;

  std::vector<Lib::ItemML*>::const_iterator it;
  for(it=aItems.begin();it!=aItems.end();++it) {
    std::string name;
    if(!(*it)->attributeValue("","name",name)) continue;
    if(name.empty()) continue;

    bool found = false;
   {std::vector<Lib::ItemML*>::iterator it2;
    for(it2=items2.begin();it2!=items2.end();++it2) {
      std::string name2;
      (*it2)->attributeValue("","name",name2);
      if(name2==name) {
        found = true;
        break;
      }   
    }}
    if(!found) {
      items2.push_back(*it);
    }
  }

  aItems = items2;
}
/////////////////////////////////////////////////////////////////////////////
void Lib::ItemML::removeProperties(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Element*>::iterator it;
  for(it=fProperties.begin();it!=fProperties.end();) {
    if((*it)->name()==aName) {
      Element* element = *it;
      it = fProperties.erase(it);
      delete element;
    } else {
      ++it;
    }
  }
}

/*
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Conveniency for frequent attribute.
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::nameAttValue(
 std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return attributeValue("","name",aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::classAttValue(
 std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return attributeValue("","class",aValue);
}
//////////////////////////////////////////////////////////////////////////////
bool Lib::ItemML::pathAttValue(
 std::string& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return attributeValue("","path",aValue);
}
*/
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Lib::ItemMLFactory::ItemMLFactory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::increment("Lib::ItemMLFactory");
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemMLFactory::~ItemMLFactory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ////Lib::Debug::decrement("Lib::ItemMLFactory");
}
//////////////////////////////////////////////////////////////////////////////
Lib::ItemML* Lib::ItemMLFactory::create(
 const std::string& aTagName
,const std::vector<Lib::ItemML::Attribute>& aAtbs
,ItemML* aParent
) 
//////////////////////////////////////////////////////////////////////////////
// It does not add the new ItemML in parent.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ItemML* itemML = new ItemML(aTagName,*this,aParent);
  itemML->setAttributes(aAtbs);
  return itemML;
}
