#ifndef OnXLab_Accessors_h
#define OnXLab_Accessors_h

// Inheritance :
#include <Slash/Tools/BaseAccessor.h>
#include <Slash/Data/IVisualizer.h>

namespace Slash {namespace Core {class ISession;}}

namespace OnXLab {

#define ONXLAB_ACC_HH(a_name) \
class a_name##Accessor\
:public Slash::BaseAccessor{\
public:\
  a_name##Accessor(Slash::Core::ISession&);\
  virtual ~a_name##Accessor();\
public: /*Slash::Data::IAccessor*/\
  virtual std::string name() const;\
  virtual Slash::Data::IIterator* iterator();\
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data,const std::string&,void*);\
private:\
  Slash::Core::ISession& fSession;\
  std::string fName;\
};

ONXLAB_ACC_HH(SbObject_)
ONXLAB_ACC_HH(SbFunction1D_)
ONXLAB_ACC_HH(SbFunction2D_)
ONXLAB_ACC_HH(SbBin1D_)
ONXLAB_ACC_HH(SbBin2D_)

#define ONXLAB_ACC_VIS_HH(a_name) \
class a_name##Accessor\
:public Slash::BaseAccessor\
,public virtual Slash::Data::IVisualizer {\
public:\
  a_name##Accessor(Slash::Core::ISession&);\
  virtual ~a_name##Accessor();\
public: /*Slash::Data::IAccessor*/\
  virtual std::string name() const;\
  virtual Slash::Data::IIterator* iterator();\
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data,const std::string&,void*);\
public: /*Slash::Data::IVisualizer*/\
  virtual void beginVisualize(const Slash::UI::IStyle&);\
  virtual void beginVisualize();\
  virtual void visualize(Slash::Data::IAccessor::Data,void*);\
  virtual void endVisualize();\
private:\
  Slash::Core::ISession& fSession;\
  std::string fName;\
};

ONXLAB_ACC_VIS_HH(ManagedObject)
ONXLAB_ACC_VIS_HH(Function)

}

#endif
