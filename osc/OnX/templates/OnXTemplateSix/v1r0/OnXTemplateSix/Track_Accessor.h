#ifndef OnXTemplateSix_Track_Accessor_h
#define OnXTemplateSix_Track_Accessor_h

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Code to declare the user class Track to the DataAccessor    ///////////////
// OnX system.                                                 ///////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// The Track_Accessor (or Track_Type) permits to manipulate the data 
// from callbacks.
// See OnXTemplateSix/source/Callbacks/OnXTemplateSix_Examples_Tracks callback.
//////////////////////////////////////////////////////////////////////////////

// Inheritance :
#include <Slash/Tools/InventorAccessor.h>

// Etc :
#include <Slash/Data/IAccessor.h>

namespace User {class Framework;}
namespace User {class Track;}

namespace OnXTemplateSix {

class Track_Accessor : public Slash::InventorAccessor {
public:
  Track_Accessor(Slash::Core::ISession&,
                 Slash::UI::ISceneHandler&,
                 const User::Framework&);
  virtual ~Track_Accessor();
public: //Slash::Data::IAccessor
  virtual std::string name() const;
  virtual Slash::Data::IIterator* iterator();
  virtual Slash::Core::IValue* findValue(Slash::Data::IAccessor::Data,
                                         const std::string&,void*);
public: //OnX::IType
  virtual void visualize(Slash::Data::IAccessor::Data,void*);
private:
  std::string fName;
  const User::Framework& fUserFramework;
};

}

#endif
