#ifndef OnX_SoNodeAccessor_h
#define OnX_SoNodeAccessor_h

// Inheritance :
#include <Slash/Tools/BaseAccessor.h>
#include <Slash/Data/IVisualizer.h>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace UI {class ISceneHandler;}}

class SoNode;

namespace OnX {

class SoNodeAccessor 
: public Slash::BaseAccessor 
,public virtual Slash::Data::IVisualizer {
public:
  SoNodeAccessor(Slash::Core::ISession&);
  virtual ~SoNodeAccessor();
public: //Slash::Data::IAccessor
  virtual std::string name() const;
  virtual Slash::Data::IIterator* iterator();
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data,const std::string&,void*);
public:
  virtual void beginVisualize();
  virtual void beginVisualize(const Slash::UI::IStyle&);
  virtual void visualize(Slash::Data::IAccessor::Data,void*);
  virtual void endVisualize() {}
private:
  void getSoNodes(const std::vector<std::string>& aArgs);
  void clear();
private:
  Slash::Core::ISession& fSession;
  std::string fName;  
  SoNode** fSoNodes;
  unsigned int fNumber;
  Slash::UI::ISceneHandler* fSH;
  std::string fPlacement;
};

}

#endif
