#ifndef Lib_BaseML_h
#define Lib_BaseML_h 

#include <vector>
#include <string>
#include <queue>

#include <expat.h>
//#include <Lib/ItemML.h> //For Lib::ItemML::Attribute typedef

namespace Slash {namespace Core {class IWriter;}}

namespace Slash {namespace Core {class IFileGetter;}}

namespace Lib {

class ItemMLFactory;
class ItemML;

class BaseML {
public:
  BaseML(ItemMLFactory&,int = 0);
  virtual ~BaseML();
private:
  BaseML(const BaseML&);
  BaseML& operator=(const BaseML&);
public:
  bool loadFile(const std::string&,bool,Slash::Core::IWriter&,bool push_dir = true,bool getit = true);
  bool loadString(const std::string&,Slash::Core::IWriter&);
  bool loadBuffer(unsigned int,const char*,Slash::Core::IWriter&);
  void setTags(const std::vector<std::string>&);
  const ItemML* topItem() const;
  ItemML* topItem();
  void empty();
  bool isTag(const std::string&) const;
  void setFileGetter(Slash::Core::IFileGetter*);
  int errors() const;
public:
  virtual bool visitEndElement(ItemML&,bool&);
private:
  void clear();
  bool readFile(const std::string&,
                 XML_StartElementHandler,XML_EndElementHandler,void*,
                bool,Slash::Core::IWriter&);
  bool solveReferences(Lib::ItemML&,bool,Slash::Core::IWriter&);
  bool parseFile(const std::string&,
                   XML_StartElementHandler,XML_EndElementHandler,void*,
                 bool,Slash::Core::IWriter&);
  bool parseBuffer(unsigned int,const char*,
                   XML_StartElementHandler,XML_EndElementHandler,void*,
                   Slash::Core::IWriter&);
  static void characterDataHandler(void*,const XML_Char*,int);
  static void startElement(void*,const XML_Char*,const XML_Char**);
  static void endElement(void*,const XML_Char*);
private:
  ItemMLFactory& fItemMLFactory;
  std::vector<std::string> fTags;
  int fVerboseLevel;
  ItemML* fTop;
  ItemML* fCurrent;
  //std::vector<Lib::ItemML::Attribute> fAtbs;
  std::vector< std::pair<std::string,std::string> > fAtbs;
  std::string fValue;
  Slash::Core::IFileGetter* fFileGetter;
  Slash::Core::IWriter* fPrinter;
  unsigned int fDepth;
  bool fAbort;
  std::queue<std::string> fDirs;
protected:
  int fErrors;
};

}

#endif
