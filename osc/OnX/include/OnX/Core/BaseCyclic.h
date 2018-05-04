#ifndef OnX_BaseCyclic_h
#define OnX_BaseCyclic_h 

#include <Slash/UI/ICyclic.h>

namespace Slash {namespace Core {class ISession;}}
namespace Slash {namespace UI {class IScriptManager;}}

namespace OnX {

class BaseCyclic : public virtual Slash::UI::ICyclic {
public: //Slash::UI::ICyclic
  virtual std::string name() const;
  virtual void setDelay(int);
  virtual void setCallback(Slash::UI::ICyclicCallback*);
  virtual void setScript(const std::string&,const std::string&);
  virtual void execute();
  virtual int delay() const;
  virtual int iterations() const;
  virtual std::string interpreter() const;
  virtual std::string script() const;
public:
  BaseCyclic(Slash::Core::ISession&,const std::string&,
             int,const std::string&,const std::string&);
  virtual ~BaseCyclic();
protected:
  Slash::Core::ISession& fSession;
  std::string fName;
  int fDelay;
  Slash::UI::ICyclicCallback* fCallback;
  std::string fInterpreter;
  std::string fScript;
  Slash::UI::IScriptManager* fScriptManager;
  int fCounter;
};

}

#endif

