#ifndef Slash_Tools_Style_h
#define Slash_Tools_Style_h

// inheritance :
#include <Slash/UI/IStyle.h>

#include <map>

namespace Slash {

class Style : public virtual Slash::UI::IStyle {
public:
  virtual void set(const std::string& aKey,const std::string& aValue) {
    Items::iterator it = fItems.find(aKey);    
    if(it==fItems.end()) {
      fItems[aKey] = aValue; //not found. add.
    } else {
      (*it).second = aValue; //found. override value.
    }
  }

  virtual void remove(const std::string& aKey) {
    Items::iterator it = fItems.find(aKey);    
    if(it==fItems.end()) return;
    fItems.erase(it);
  }
  virtual bool value(const std::string& aKey,std::string& aValue) const {
    Items::const_iterator it = fItems.find(aKey);    
    if(it==fItems.end()) {aValue = "";return false;}
    aValue = (*it).second;
    return true;
  }

  virtual std::map<std::string,std::string> items() const {return fItems;}

  virtual void copy(const Slash::UI::IStyle& aFrom) {fItems = aFrom.items();}

  virtual std::string toString() const {
    // return a \n separated list of [<name> <value>] suited for
    // HEPVis SbStyle.
    std::string s;
    Items::const_iterator it;
    for(it=fItems.begin();it!=fItems.end();++it) {
      if(it!=fItems.begin()) s += "\n";
      s += (*it).first;      
      s += " ";
      s += (*it).second;      
    }
    return s;
  }

public:
  Style(){}
  Style(const Style& aFrom)
  : Slash::UI::IStyle()
  ,fItems(aFrom.fItems)
  {}
  Style& operator=(const Style& aFrom){
    fItems = aFrom.fItems;
    return *this;
  }
  virtual ~Style(){};
private:
  typedef std::map<std::string,std::string> Items;
  Items fItems;
private:
  static void check_instantiation(){Style o;}  
};

}

#endif
