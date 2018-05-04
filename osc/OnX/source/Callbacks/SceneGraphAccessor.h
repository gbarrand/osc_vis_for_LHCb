#ifndef OnX_SceneGraphAccessor_h
#define OnX_SceneGraphAccessor_h

// Inheritance :
#include <Slash/Tools/BaseAccessor.h>

namespace Slash {namespace Core {class ISession;}}
class SoNode;

namespace OnX {

class SceneGraph {
public:
  SceneGraph():fSep(0),fParent(0),fInfo(0){}
public:
  SoNode* fSep;
  SoNode* fParent;
  SoNode* fInfo;
};

class SceneGraphAccessor : public Slash::BaseAccessor {
public:
  SceneGraphAccessor(Slash::Core::ISession&);
  virtual ~SceneGraphAccessor();
public: //Slash::Data::IAccessor
  virtual std::string name() const;
  virtual Slash::Data::IIterator* iterator();
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data,const std::string&,void*);
  virtual void destroy(Slash::Data::IAccessor::Data,void*);
  virtual void set(Slash::Data::IAccessor::Data,
                   const std::string&,const std::string&,void*);
private:
  void getSceneGraphs(const std::vector<std::string>& aArgs);
  void clear();
private:
  Slash::Core::ISession& fSession;
  std::string fName;  
  SceneGraph* fSceneGraphs;
  unsigned int fNumber;
};

}

#endif
