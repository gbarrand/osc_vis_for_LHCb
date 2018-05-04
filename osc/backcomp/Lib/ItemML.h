#ifndef Lib_ItemML_h
#define Lib_ItemML_h 

#include <string>
#include <vector>
#include <list>

namespace Slash {namespace Core {class IWriter;}}

namespace Lib {

class ItemMLFactory;

class Element;

class ItemML {
public:
  ItemML(const std::string&,ItemMLFactory&,ItemML*);
  virtual ~ItemML();
  virtual void* cast(const std::string&) const;
private:
  ItemML(const ItemML&);
  ItemML& operator=(const ItemML&);
public:
  typedef std::pair<std::string,std::string> Attribute;
  typedef bool (*ExecFunction)(ItemML&,void*);
public:
  static void collectByTag(Lib::ItemML&,const std::string&,
                           std::vector<Lib::ItemML*>&);
  static void collectByAttribute(Lib::ItemML&,const std::string&,
                           std::vector<Lib::ItemML*>&);
  static void unique(std::vector<Lib::ItemML*>&);
public:
  void addAttribute(const std::string&,const std::string&);
  void addProperty(const std::string&,
                   const std::vector<Attribute>&,
                   const std::string&);
  void print(Slash::Core::IWriter&);
  void printXML(Slash::Core::IWriter&,const std::string& = "");
  bool isAttribute(const std::string&) const;
  bool isProperty(const std::string&) const;
  bool propertyValue(const std::string&,std::string&) const;
  bool propertyValues(const std::string&,std::vector<std::string>&) const;
  void setAttributes(const std::vector<Attribute>&);
  bool attributeValue(const std::string&,std::string&) const;
  bool attributeValue(const std::string&,const std::string&,std::string&) const;
  const std::list<Element*>& properties() const;
  void setSaveFlag(bool);
  bool saveFlag() const;
  void setFile(const std::string&);
  std::string file() const;
  bool setPropertyValue(const std::string&,const std::string&,int = 0);
  bool setAttributeValue(const std::string&,const std::string&,const std::string&,int = 0);
  bool removeAttribute(const std::string&);
  void removeProperties(const std::string&);
  bool propertyValueBoolean(const std::string&,bool&) const;
  void deleteProperty(const std::string&);
  void deleteProperty(Lib::Element*);
  const std::vector<Attribute>& attributes() const;
  void postExecute(ExecFunction,void*);
  void postExecuteBackward(ExecFunction,void*);

  ItemML* parent() const;
  const std::list<ItemML*>& children() const;
  ItemML* findItem(const std::string&) const;
  ItemML* findItemWithTag(const std::string&,const std::string&) const;
  ItemML* findByAttribute(const std::string&,const std::string&,bool upDown = true,bool leftRight = true) const;
  const ItemML* findByPropertyInSameLevel(const std::string&,
                                          const std::string&) const;
  Lib::ItemML* findByTag(const std::string&) const;
  void addChild(ItemML*);
  void setParent(ItemML*);
  void addAfter(ItemML*,ItemML*);
  int numberOfChildren() const;
  bool isFirstChild(const ItemML&);
  bool isLastChild(const ItemML&);
  ItemML* previousChild(const ItemML&);
  ItemML* nextChild(const ItemML&);
  void removeChild(ItemML*&,bool = true);
  bool replaceChild(ItemML*,Lib::ItemML*);
  void deleteChildren();
  ItemML* createCopy(ItemML*);
  enum eCopyWhat { ATTRIBUTES, PROPERTIES, CHILDREN, ALL };
  bool copy(const ItemML&,eCopyWhat = ALL,bool = true);

  //FIXME : begin : the below should be handled in derived classes.
  void setData(void*,void*,int);
  void* getData1() const;
  void* getData2() const;
  int getDataInt() const;
  void getData(void*&,void*&,int&) const;
  virtual bool invalidate();
  //FIXME : end.

  // Used by BaseML :
  void setDepth(unsigned int aDepth);
  unsigned int depth() const;

  const std::string& tagName() const;

  void updateTree(const ItemML&);

  void replace(const std::string&,const std::string&);
  //bool nameAttValue(std::string&) const;
  //bool classAttValue(std::string&) const;
  //bool pathAttValue(std::string&) const;

  bool hasEmptyAttributeValue(Slash::Core::IWriter&) const;
private:
  void clear();
private:
  std::string fTagName;
  ItemMLFactory& fItemMLFactory;
  ItemML* fParent;
  std::list<ItemML*> fChildren;
  std::list<Element*> fProperties;
  std::vector<Attribute> fAtbs;
  std::string fFile;
  bool fSaveFlag;
  void* fData1;
  void* fData2;
  int fDataInt;
  int fDepth;
};

class ItemMLFactory {
public:
  ItemMLFactory();
  virtual ~ItemMLFactory();
public:
  virtual ItemML* create(const std::string&, //tagName
                         const std::vector<ItemML::Attribute>&, //atbs
                         ItemML*); //parent
};

}

#endif
