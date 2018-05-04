#ifndef BatchLab_Annotation_h
#define BatchLab_Annotation_h

// Inheritance :
#include <AIDA/IAnnotation.h>
#include <Slash/Store/IStorable.h>

#include <BatchLab/Core/T_Vector.h>

namespace BatchLab {

class Item : public virtual Slash::Store::IStorable {
public: //IVisited
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Store::IStorable)
    else INLIB_IF_CAST(BatchLab::Item)
    else return 0;
  }

  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,Item::s_class(),Item::s_visit)) return false;
  
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
  
    if(!aVisitor.visit("fKey",fKey)) return false;
    if(!aVisitor.visit("fValue",fValue)) return false;
    if(!aVisitor.visit("fSticky",fSticky)) return false;
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }

  virtual bool read(Slash::Store::IVisitor& aVisitor) {
    if(!aVisitor.begin(*this)) return false;
  
    int version;
    if(!aVisitor.visit(version)) return false;
  
    if(!aVisitor.visit(fKey)) return false;
    if(!aVisitor.visit(fValue)) return false;
    if(!aVisitor.visit(fSticky)) return false;
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }
public:
  Item(){}
  Item(const std::string& aKey,const std::string& aValue,bool aSticky)
  :fKey(aKey),fValue(aValue),fSticky(aSticky){}
  virtual ~Item(){}
public:
  Item(const Item& aFrom)
  :Slash::Store::IStorable(aFrom)
  ,fKey(aFrom.fKey)
  ,fValue(aFrom.fValue)
  ,fSticky(aFrom.fSticky)
  {}
  Item& operator=(const Item& aFrom) {
    fKey = aFrom.fKey;
    fValue = aFrom.fValue;
    fSticky = aFrom.fSticky;
    return *this;
  }
private:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Item");return s_v;
  }
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Item* local = INLIB_CONST_CAST(aObject,BatchLab::Item);
    if(!local) return false;
    return local->Item::visit(aVisitor);
  }
public:
  std::string fKey;
  std::string fValue;
  bool fSticky;
};

class Annotation 
:public virtual AIDA::IAnnotation 
,public virtual Slash::Store::IStorable {
public: //AIDA/IAnnotation
  virtual bool addItem(const std::string& aKey,const std::string& aValue,
                       bool aSticky = true){
    // If item exist, override values.
    Item* note = findItem(aKey);
    if(note){
      note->fValue = aValue;
      note->fSticky = aSticky;
      return false;
    }
    fItems.push_back(Item(aKey,aValue,aSticky));
    return true;
  }

  virtual bool removeItem(const std::string& aKey) {
    /// Remove the item, if it is not sticky, indicated by a given key
    std::vector<Item>::iterator it;
    for(it=fItems.begin();it!=fItems.end();++it) {
      if((*it).fKey==aKey) {
        if((*it).fSticky) return false;
        fItems.erase(it);
        return true;
      }
    }
    return false;
  }
  
  virtual std::string value(const std::string& aKey) const {
    // Retrieve the value for a given key
    const Item* note = findItem(aKey);
    if(!note) return "";
    return note->fValue;
  }
  
  virtual bool visibility(const std::string& aKey) const {
    /// Retrieve the visibility flag for a given key
    const Item* note = findItem(aKey);
    if(!note) return true;
    return note->fSticky;
  }

  virtual void setValue(const std::string& aKey,const std::string& aValue) {
    Item* note = findItem(aKey);
    if(!note) return;
    note->fValue = aValue;
  }

  virtual void setSticky(const std::string& aKey,bool aSticky) {
    Item* note = findItem(aKey);
    if(!note) return;
    note->fSticky = aSticky;
  }

  virtual int size() const {return fItems.size();}

  virtual std::string key(int aIndex) const {
    if(aIndex<0) return "";
    if(aIndex>=int(fItems.size())) return "";
    return fItems[aIndex].fKey;
  }

  virtual std::string value(int aIndex) const {
    if(aIndex<0) return "";
    if(aIndex>=int(fItems.size())) return "";
    return fItems[aIndex].fValue;
  }

  virtual void reset() {
    // Do not remove sticky items.
    std::vector<Item>::iterator it;
    for(it=fItems.begin();it!=fItems.end();) {
      if((*it).fSticky) {
        ++it;
      } else {
        it = fItems.erase(it);
      }
    }
  }

  //AIDA-v3r3p0
  virtual bool isSticky(const std::string& aKey) const {
    const Item* note = findItem(aKey);
    if(!note) return false; //FIXME
    return note->fSticky;
  }

  virtual bool isSticky(int aIndex) const {
    if(aIndex<0) return false;
    if(aIndex>=int(fItems.size())) return false;
    return fItems[aIndex].fSticky;
  }

  virtual bool hasKey(const std::string& aKey) const {
    return findItem(aKey) ? true : false;
  }

  virtual int findKey(const std::string& aKey) const {
    int index = 0;
    std::vector<Item>::const_iterator it;
    for(it=fItems.begin();it!=fItems.end();++it) {
      if((*it).fKey==aKey) return index;
      index++;
    }
    return NotFound;
  }

public: //IVisited
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Store::IStorable)
    else INLIB_IF_CAST(AIDA::IAnnotation)
    else INLIB_IF_CAST(BatchLab::Annotation)
    else return 0;
  }

  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,Annotation::s_class(),Annotation::s_visit))
      return false;
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;  
    if(!aVisitor.visit("fItems",fItems)) return false;
    if(!aVisitor.end(*this)) return false;
    return true;
  }
  
  virtual bool read(Slash::Store::IVisitor& aVisitor) {
    if(!aVisitor.begin(*this)) return false;
    int version;
    if(!aVisitor.visit(version)) return false;  
    if(!fItems.read(aVisitor)) return false;  
    if(!aVisitor.end(*this)) return false;
    return true;
  }
public:
  Annotation():fItems("BatchLab::Item"){addItem("Title","",true);}
  virtual ~Annotation(){}
public:
  Annotation(const Annotation& aFrom)
  :AIDA::IAnnotation(aFrom)
  ,Slash::Store::IStorable(aFrom)
  ,fItems(aFrom.fItems)
  {}
  Annotation& operator=(const Annotation& aFrom){
    fItems = aFrom.fItems;
    return *this;
  }
public:
  void clear(){fItems.clear();}

  const std::vector<BatchLab::Item>& items() const {return fItems;}
  std::vector<BatchLab::Item>& items() {return fItems;}
private:
  Item* findItem(const std::string& aKey) {
    std::vector<Item>::iterator it;
    for(it=fItems.begin();it!=fItems.end();++it) {
      if((*it).fKey==aKey) {
        return &(*it);
      }
    }
    return 0;
  }

  const Item* findItem(const std::string& aKey) const {
    std::vector<Item>::const_iterator it;
    for(it=fItems.begin();it!=fItems.end();++it) {
      if((*it).fKey==aKey) {
        return &(*it);
      }
    }
    return 0;
  }
private:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Annotation");return s_v;
  }
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Annotation* local = INLIB_CONST_CAST(aObject,BatchLab::Annotation);
    if(!local) return false;
    return local->Annotation::visit(aVisitor);
  }
private:
  static const int NotFound = -1;
private:
  Vector<Item> fItems;
};

}

#endif
